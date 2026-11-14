#pragma once

#include "common.h"

struct tNamedId;

#define INPUT_FILE_NAME_SIZE 0x401
#define DIRECTORY_PATH_SIZE 256

// What the compiler is doing (g_nCompilerStage): reading each kind of file, then compiling
enum eCompilerStage
{
    STAGE_MAIN_SCRIPT,
    STAGE_FOREIGN_GOSUBS,
    STAGE_SUBSCRIPTS,
    STAGE_MISSIONS,
    STAGE_STREAMED_SCRIPTS,
    STAGE_COMPILING,
};

enum eTargetMode : int8_t
{
    TARGET_NONE = -1,  // until set
    GTA_3,             // older targets, still known to WriteScriptHeader
    VICE_CITY,
    SAN_ANDREAS,
    SAN_ANDREAS_PC,
    SAN_ANDREAS_XBOX,
    SAN_ANDREAS_XBOX360,
};

// ----------------------------------------------------------------------------------------------- window

extern HINSTANCE g_hInstance;                   // 0x02B225A0 (never assigned: WinMain uses its parameter)
extern HWND g_hOutputListBox;                   // 0x02B2259C list box filling the main window, see PrintMessage
extern bool g_bMainWindowCreated;               // 0x02B21C88
extern char
    g_szStartupDirectory[DIRECTORY_PATH_SIZE];  // 0x02B225A8 working directory at startup (data files are there)

// ----------------------------------------------------------------------------------------------- configuration

extern char Config_InputFileName[INPUT_FILE_NAME_SIZE];  // 0x02B21C90 main .sc file (full path)
extern char Config_FileNameNoExt[];  // 0x02B22098 the same without extension, char[FILE_PATH_SIZE]. Declared without
                                     //   a size: CompileScriptFile has no range check when writing it
extern char Config_TargetPath[DIRECTORY_PATH_SIZE];  // 0x02B22498 directory of the main .sc file
extern eTargetMode Config_TargetMode;                // 0x0049A9E8 -1 until set
extern bool Config_bWriteCommandList;                // 0x02B21C89 -cmdlist: write CommandList_SA.h
extern bool Config_bWriteMissionOutput;              // 0x02B21C8A -missionout: write missionOutput.txt
extern bool Config_bCompilingOrigScript;             // 0x02B21C8B -f with a file named *orig.sc*
extern bool Config_bTargetModeFromCmdLine;           // 0x02B21C8E -san_andreas_pc / -san_andreas_xbox360 given
extern bool Config_bCloseWhenDone;                   // 0x02B21C8F -close: quit after compiling (with -f)
extern bool Config_bCompileOnStartup;                // 0x02B22091 -f <file>: compile it straight away

// ----------------------------------------------------------------------------------------------- compiler run

extern int16_t g_nCompilerStage;   // 0x02B21C8C 0 main file, 1 foreign gosubs, ..., 5 compiling
extern DWORD g_nCompileId;         // 0x02B22598 incremented on every compile, kept in the registry
extern FILE* g_pPreprocessedFile;  // 0x02B225A4 fakeMainOutputFile.txt

// ----------------------------------------------------------------------------------------------- main.cpp

void PrintMessage(const char* pszMessage);                                                 // 0x0041BA70
void SetTargetMode(UINT nMenuId, HWND hWnd);                                               // 0x0041BA90
int GetIdFromNamedIdArray(const char* pszName, const tNamedId* aNamedIds);                 // 0x0041BC30
void ParseCommandLine(const char* pszCmdLine);                                             // 0x0041BD00
void __stdcall HandleMenuCommand(HWND hWnd, int nCommandId, HWND hCtrl, int nNotifyCode);  // 0x0041C040
bool CompileScriptFile(int nCommandId);                                                    // 0x0041C2B0
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);              // 0x0041C4E0
void DisplaySelectedFileName(const char* pszFileName);                                     // 0x0041C620
char* GetFileNameFromPath(char* pszPath);                                                  // 0x0041C6A0
void GetDirectoryFromPath(const char* pszPath, char* pszDirectoryOut);                     // 0x0041C6C0
