// main.cpp: 0x41BA70-0x41CB4B in sc.exe
//
// The window (a menu and a list box that shows every message), the command line, and the top of a compile run.

#include "common.h"
#include "main.h"
#include "resource.h"
#include "registry.h"

#include "codegen.h"
#include "compiledlines.h"
#include "compiler.h"
#include "errors.h"
#include "loader.h"
#include "missions.h"
#include "namemaps.h"
#include "parser.h"
#include "streamedscripts.h"

#define MAIN_WINDOW_CLASS "VPSkelWinClass"
#define MAIN_WINDOW_TITLE "GTA3 Script Compiler V2249"
#define MAX_CMDLINE_ARGS 10
#define MAIN_WINDOW_WIDTH 600
#define MAIN_WINDOW_HEIGHT 365

// 0x02B225A0
HINSTANCE g_hInstance;
// 0x02B2259C
HWND g_hOutputListBox;
// 0x02B21C88
bool g_bMainWindowCreated;
// 0x02B225A8
char g_szStartupDirectory[DIRECTORY_PATH_SIZE];

// 0x02B21C90
char Config_InputFileName[INPUT_FILE_NAME_SIZE];
// 0x02B22498
char Config_TargetPath[DIRECTORY_PATH_SIZE];
// 0x0049A9E8
eTargetMode Config_TargetMode = TARGET_NONE;
// 0x02B21C89
bool Config_bWriteCommandList;
// 0x02B21C8A
bool Config_bWriteMissionOutput;
// 0x02B21C8B
bool Config_bCompilingOrigScript;
// 0x02B21C8E
bool Config_bTargetModeFromCmdLine;
// 0x02B21C8F
bool Config_bCloseWhenDone;
// 0x02B22091
bool Config_bCompileOnStartup;

// 0x02B21C8C
int16_t g_nCompilerStage;
// 0x02B22598
DWORD g_nCompileId;
// 0x02B225A4
FILE* g_pPreprocessedFile;

// 0x0041BA70
void PrintMessage(const char* pszMessage)
{ SendMessageA(g_hOutputListBox, LB_ADDSTRING, 0, (LPARAM)pszMessage); }

// 0x0041BA90
// Ticks the menu item of the target, remembers it in the registry (unless the command line chose the target), then
// reloads the platform's audio event names and rebuilds the constant maps that depend on the target.
void SetTargetMode(UINT nMenuId, HWND hWnd)
{
    HMENU hMenu = GetMenu(hWnd);

    if (!Config_bTargetModeFromCmdLine)
    {
        if (nMenuId == ID_TARGET_SAN_ANDREAS)
        {
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX360, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SAN_ANDREAS, MF_CHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_PC, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX, MF_UNCHECKED);
            Config_TargetMode = SAN_ANDREAS;
            g_Registry.WriteString("target", "san_andreas");
        }
        else if (nMenuId == ID_TARGET_SA_PC)
        {
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX360, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_PC, MF_CHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SAN_ANDREAS, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX, MF_UNCHECKED);
            Config_TargetMode = SAN_ANDREAS_PC;
            g_Registry.WriteString("target", "san_andreas_pc");
        }
        else if (nMenuId == ID_TARGET_SA_XBOX)
        {
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX360, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX, MF_CHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SAN_ANDREAS, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_PC, MF_UNCHECKED);
            Config_TargetMode = SAN_ANDREAS_XBOX;
            g_Registry.WriteString("target", "san_andreas_xbox");
        }
        else if (nMenuId == ID_TARGET_SA_XBOX360)
        {
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX360, MF_CHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_XBOX, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SAN_ANDREAS, MF_UNCHECKED);
            CheckMenuItem(hMenu, ID_TARGET_SA_PC, MF_UNCHECKED);
            Config_TargetMode = SAN_ANDREAS_XBOX360;
            g_Registry.WriteString("target", "san_andreas_xbox360");
        }
    }

    _chdir(g_szStartupDirectory);
    InitSoundNames();
    if (Config_TargetMode == SAN_ANDREAS)
    {
        LoadSoundNames("AudioEvents_PS2.txt");
    }
    else if (Config_TargetMode == SAN_ANDREAS_PC)
    {
        LoadSoundNames("AudioEvents_PC.txt");
    }
    else if (Config_TargetMode == SAN_ANDREAS_XBOX)
    {
        LoadSoundNames("AudioEvents_XBOX.txt");
    }
    else if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        LoadSoundNames("AudioEvents_PC.txt");
    }
    BuildConstantMaps();
}

// 0x0041BC30
// Looks a name up in a table of { name, id } that ends with an empty name.
int GetIdFromNamedIdArray(const char* pszName, const tNamedId* aNamedIds)
{
    uint16_t i = 0;
    while (strcmp(aNamedIds[i].szName, "") != 0)
    {
        if (strcmp(aNamedIds[i].szName, pszName) == 0)
        {
            return aNamedIds[i].nId;
        }
        i++;
    }
    return -1;
}

// 0x0041BD00
// -f <file>               compile <file> on startup (path and file are also stored in the registry)
// -close                  with -f: quit once compiled
// -san_andreas_pc         target PC
// -san_andreas_xbox360    target Xbox 360
// -missionout             write missionOutput.txt
// -cmdlist                write CommandList_SA.h
void ParseCommandLine(const char* pszCmdLine)
{
    size_t nLength = strlen(pszCmdLine);
    if (nLength == 0)
    {
        return;
    }

    char* pszCopy = new char[nLength + 1];
    strcpy(pszCopy, pszCmdLine);

    // split on spaces
    char* apszArgs[MAX_CMDLINE_ARGS];
    int nArgs = 1;
    apszArgs[0] = nullptr;
    char* p = pszCopy;
    while (*p != '\0' && nArgs != ARRAY_SIZE(apszArgs))
    {
        apszArgs[nArgs++] = p;
        while (*++p != ' ' && *p != '\0')
        {
            ;
        }
        if (*p == '\0')
        {
            break;
        }
        *p++ = '\0';
        while (*p++ == ' ')
        {
            ;
        }
        p--;
    }

    for (uint8_t i = 1; i < nArgs; i++)
    {
        const char* pszArg = apszArgs[i];
        if (strcmp(pszArg, "-f") == 0)
        {
            if (++i >= nArgs)
            {
                break;
            }
            strcpy(Config_InputFileName, apszArgs[i]);
            memset(Config_TargetPath, 0, sizeof(Config_TargetPath));
            for (int n = strlen(Config_InputFileName); n > 0; n--)
            {
                if (Config_InputFileName[n] == '\\')
                {
                    strncpy(Config_TargetPath, Config_InputFileName, n);
                    break;
                }
            }
            g_Registry.WriteString("path", Config_TargetPath);
            g_Registry.WriteString("file", GetFileNameFromPath(Config_InputFileName));
            Config_bCompileOnStartup = true;
            Config_bCompilingOrigScript = strstr(Config_InputFileName, "orig.sc") != nullptr;
        }
        else if (strcmp(pszArg, "-close") == 0)
        {
            Config_bCloseWhenDone = true;
        }
        else if (strcmp(pszArg, "-san_andreas_pc") == 0)
        {
            Config_TargetMode = SAN_ANDREAS_PC;
            g_Registry.WriteString("target", "san_andreas_pc");
            Config_bTargetModeFromCmdLine = true;
        }
        else if (strcmp(pszArg, "-san_andreas_xbox360") == 0)
        {
            Config_TargetMode = SAN_ANDREAS_XBOX360;
            g_Registry.WriteString("target", "san_andreas_xbox360");
            Config_bTargetModeFromCmdLine = true;
        }
        else if (strcmp(pszArg, "-missionout") == 0)
        {
            Config_bWriteMissionOutput = true;
        }
        else if (strcmp(pszArg, "-cmdlist") == 0)
        {
            Config_bWriteCommandList = true;
        }
    }

    delete[] pszCopy;
}

// 0x0041C040
void __stdcall HandleMenuCommand(HWND hWnd, int nCommandId, HWND hCtrl, int nNotifyCode)
{
    switch (nCommandId)
    {
        case ID_FILE_LOAD:
        {
            if (g_Registry.DoesValueExist("path"))
            {
                g_Registry.ReadString("path", Config_TargetPath);
            }
            else
            {
                memset(Config_TargetPath, 0, sizeof(Config_TargetPath));
            }

            OPENFILENAMEA ofn;
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.hInstance = g_hInstance;
            ofn.lpstrFilter = "Mission Scripts (*.SC)";
            ofn.lpstrCustomFilter = nullptr;
            ofn.nMaxCustFilter = 0;
            ofn.nFilterIndex = 1;
            ofn.lpstrFile = Config_InputFileName;
            ofn.nMaxFile = INPUT_FILE_NAME_SIZE - 1;
            ofn.lpstrFileTitle = nullptr;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = Config_TargetPath;
            ofn.lpstrTitle = nullptr;
            ofn.Flags = OFN_READONLY | OFN_FILEMUSTEXIST;
            ofn.nFileOffset = 0;
            ofn.nFileExtension = 0;
            ofn.lpstrDefExt = nullptr;
            ofn.lCustData = 0;
            ofn.lpfnHook = nullptr;
            ofn.lpTemplateName = nullptr;
            if (GetOpenFileNameA(&ofn) != 0)
            {
                DisplaySelectedFileName(Config_InputFileName);
                memset(Config_TargetPath, 0, sizeof(Config_TargetPath));
                GetDirectoryFromPath(Config_InputFileName, Config_TargetPath);
                g_Registry.WriteString("path", Config_TargetPath);
                g_Registry.WriteString("file", GetFileNameFromPath(Config_InputFileName));
            }
            else
            {
                CommDlgExtendedError();
            }
            break;
        }

        case ID_FILE_EXIT:
            PostQuitMessage(0);
            break;

        case ID_RUN_PARSER:
        case ID_RUN_MULTIFILE:
            SendMessageA(g_hOutputListBox, LB_RESETCONTENT, SendMessageA(g_hOutputListBox, LB_GETCOUNT, 0, 0) - 1, 0);
            CompileScriptFile(nCommandId);
            // scroll to the last message
            SendMessageA(g_hOutputListBox, LB_SETTOPINDEX, SendMessageA(g_hOutputListBox, LB_GETCOUNT, 0, 0) - 1, 0);
            break;

        case ID_TARGET_SAN_ANDREAS:
            SetTargetMode(ID_TARGET_SAN_ANDREAS, hWnd);
            break;
        case ID_TARGET_SA_PC:
            SetTargetMode(ID_TARGET_SA_PC, hWnd);
            break;
        case ID_TARGET_SA_XBOX:
            SetTargetMode(ID_TARGET_SA_XBOX, hWnd);
            break;
        case ID_TARGET_SA_XBOX360:
            SetTargetMode(ID_TARGET_SA_XBOX360, hWnd);
            break;
    }
}

// 0x0041C2B0
// Reads the main script and every file it pulls in (foreign gosubs, subscripts, missions, streamed scripts), then
// compiles. nCommandId (the menu command that started the run) is not used.
bool CompileScriptFile(int nCommandId)
{
    bool bSuccess = false;
    char szMessage[100];

    sprintf(szMessage, "Interpreting file %s", Config_InputFileName);
    PrintMessage(szMessage);

    for (int i = strlen(Config_InputFileName); i > 0; i--)
    {
        if (Config_InputFileName[i] == '.')
        {
            strncpy(Config_FileNameNoExt, Config_InputFileName, i);
            Config_FileNameNoExt[i] = '\0';
            break;
        }
    }

    InitMainScriptFileInfo();
    InitModelNameList();
    InitSubscriptsAndForeignGosubs();
    InitCompiledLines();
    InitMissionScripts();
    InitStreamedScripts();
    InitVarConsole();
    InitProgressList();
    InitMissingSoundEffectsList();
    InitSoundEffectUsageLists();
    ClearAllNameMaps();

    g_nCompilerStage = STAGE_MAIN_SCRIPT;
    g_pPreprocessedFile = fopen("fakeMainOutputFile.txt", "wb");
    if (ReadScriptFile(FILE_INDEX_MAIN) >= 0)
    {
        g_nCompilerStage = STAGE_FOREIGN_GOSUBS;
        if (ReadForeignGosubFiles() >= 0)
        {
            g_nCompilerStage = STAGE_SUBSCRIPTS;
            if (ReadSubscriptFiles() >= 0)
            {
                g_nCompilerStage = STAGE_MISSIONS;
                if (ReadMissionScriptFiles() >= 0)
                {
                    g_nCompilerStage = STAGE_STREAMED_SCRIPTS;
                    if (ReadStreamedScriptFiles() >= 0)
                    {
                        PrintMessage("Compiling . . .");
                        g_nCompilerStage = STAGE_COMPILING;

                        if (g_Registry.DoesValueExist("compileid"))
                        {
                            g_Registry.ReadDWORD("compileid", &g_nCompileId);
                            g_nCompileId++;
                            if (g_nCompileId == UINT32_MAX)
                            {
                                g_nCompileId = 1;
                            }
                            g_Registry.WriteDWORD("compileid", g_nCompileId);
                        }
                        else
                        {
                            g_nCompileId = 1;
                            g_Registry.WriteDWORD("compileid", 1);
                        }

                        if (CompileAllLines() >= 0)
                        {
                            PrintMessage("Compiled successfully");
                            bSuccess = true;
                        }
                        else
                        {
                            PrintMessage("ERROR Compiling");
                        }
                    }
                }
            }
        }
    }
    else
    {
        PrintMessage("ERROR Reading file");
    }

    fclose(g_pPreprocessedFile);
    FreeCompiledLines();
    return bSuccess;
}

// 0x0041C4E0
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

    switch (uMsg)
    {
        case WM_CREATE:
            GetClientRect(hWnd, &rect);
            g_hOutputListBox =
                CreateWindowExA(0, "LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS, 0,
                    0, rect.right, rect.bottom, hWnd, (HMENU)IDC_OUTPUT_LISTBOX, g_hInstance, nullptr);
            ShowWindow(g_hOutputListBox, SW_SHOW);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            GetClientRect(hWnd, &rect);
            MoveWindow(g_hOutputListBox, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
            return 0;

        case WM_COMMAND:
            HandleMenuCommand(hWnd, LOWORD(wParam), (HWND)lParam, HIWORD(wParam));
            return 0;
    }
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// 0x0041C620
void DisplaySelectedFileName(const char* pszFileName)
{
    char szMessage[100];

    PrintMessage("   ");
    sprintf(szMessage, "File selected: %s", pszFileName);
    PrintMessage(szMessage);
    PrintMessage("    ");
}

// 0x0041C6A0
char* GetFileNameFromPath(char* pszPath)
{
    char* pszSlash = strrchr(pszPath, '\\');
    if (pszSlash != nullptr)
    {
        return pszSlash + 1;
    }
    return nullptr;
}

// 0x0041C6C0
void GetDirectoryFromPath(const char* pszPath, char* pszDirectoryOut)
{
    for (int i = strlen(pszPath); i > 0; i--)
    {
        if (pszPath[i] == '\\')
        {
            strncpy(pszDirectoryOut, pszPath, i);
            return;
        }
    }
}

// 0x0041C700
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    char szBuffer[128];

    GetCurrentDirectoryA(sizeof(g_szStartupDirectory), g_szStartupDirectory);
    ParseCommandLine(lpCmdLine);

    WNDCLASSA wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_MAINICON));
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.lpszMenuName = MAKEINTRESOURCEA(IDR_MAINMENU);
    wc.lpszClassName = MAIN_WINDOW_CLASS;
    if (RegisterClassA(&wc) == 0u)
    {
        return 0;
    }

    HWND hWnd = CreateWindowExA(0, MAIN_WINDOW_CLASS, MAIN_WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);
    g_bMainWindowCreated = true;
    if (!Config_bTargetModeFromCmdLine)
    {
        Config_TargetMode = SAN_ANDREAS;
    }
    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    // AI editor choices, then the model names from the game's data files
    InitEventChoices();
    LoadEventChoices("../Scripts/aiedit/PedEventChoices.txt", false);
    LoadEventChoices("../Scripts/aiedit/GroupEventChoices.txt", true);
    InitTaskChoices();
    LoadTaskChoices("../Scripts/aiedit/PedTaskChoices.txt", false);
    LoadTaskChoices("../Scripts/aiedit/GroupTaskChoices.txt", true);
    InitValidTasksForEvents();
    LoadValidTasksForEvents("../Scripts/aiedit/PedEditConfig.txt", false);
    LoadValidTasksForEvents("../Scripts/aiedit/GroupEditConfig.txt", true);
    LoadDataFile("gta.dat");
    InitDefaultModels();
    LoadIDE("vehicles.ide");
    LoadIDE("peds.ide");
    LoadIDE("default.ide");

    // the last file loaded
    if (g_Registry.DoesValueExist("file"))
    {
        g_Registry.ReadString("path", Config_TargetPath);
        g_Registry.ReadString("file", szBuffer);
        sprintf(Config_InputFileName, "%s\\%s", Config_TargetPath, szBuffer);
        DisplaySelectedFileName(Config_InputFileName);
    }

    // enums shared with the game code
#ifdef SERGEANUR_PATCH
    // the headers sit next to sc.exe. A missing one leaves its table without the "" terminator and
    // AddStringTableToMap then crashes on a nullptr entry
    ReadWidgetIDs("TouchInterface_SA.h");
    ReadHIDMapping("HID_Base.h");
    if (Config_bWriteCommandList)
        BuildCommandList("CommandList_SA.h");
#else
    ReadWidgetIDs("..\\..\\..\\..\\Plugins\\SanAndreas\\Source\\SanAndreas\\Private\\FrontEnd\\TouchInterface_SA.h");
    ReadHIDMapping("..\\..\\..\\..\\Plugins\\GTABase\\Source\\GTABase\\Classes\\HID_Base.h");
    if (Config_bWriteCommandList)
    {
        BuildCommandList(
            "..\\..\\..\\..\\Plugins\\SanAndreas\\Source\\SanAndreas\\Private\\FrontEnd\\CommandList_SA.h");
    }
#endif

    // the target last used
    if (g_Registry.DoesValueExist("target"))
    {
        g_Registry.ReadString("target", szBuffer);
        if (strcmp(szBuffer, "miami") == 0 || strcmp(szBuffer, "san_andreas") == 0)
        {
            SetTargetMode(ID_TARGET_SAN_ANDREAS, hWnd);
        }
        else if (strcmp(szBuffer, "san_andreas_pc") == 0)
        {
            SetTargetMode(ID_TARGET_SA_PC, hWnd);
        }
        else if (strcmp(szBuffer, "san_andreas_xbox") == 0)
        {
            SetTargetMode(ID_TARGET_SA_XBOX, hWnd);
        }
        else if (strcmp(szBuffer, "san_andreas_xbox360") == 0)
        {
            SetTargetMode(ID_TARGET_SA_XBOX360, hWnd);
        }
    }
    else
    {
        SetTargetMode(ID_TARGET_SA_PC, hWnd);
    }

    if (Config_bCompileOnStartup && CompileScriptFile(ID_RUN_MULTIFILE) && Config_bCloseWhenDone)
    {
        PostQuitMessage(0);
        return 0;
    }

    MSG msg;
    while (GetMessageA(&msg, nullptr, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return msg.wParam;
}
