// streamedscripts.cpp: 0x41CB50-0x41D176 in sc.exe

#include "common.h"
#include "streamedscripts.h"
#include "missions.h"
#include "compiler.h"
#include "codegen.h"
#include "parser.h"
#include "main.h"

// 0x02B226B0
tStreamedScript g_aStreamedScripts[MAX_STREAMED_SCRIPTS];
// 0x02B23238
uint16_t g_nNumStreamedScripts;
// 0x02B2323C
uint32_t g_nLargestStreamedScriptSize;
// 0x02B23240
char g_szLargestStreamedScript[SCRIPT_FILE_NAME_SIZE];
// 0x02B23254
int g_nStreamedScriptHeaderOffset;
// 0x02B23258
int g_nCurrentStreamedScript;

// 0x0041CB50 ?AddFilenameToStreamedScriptArray@@YAXPAD@Z
void AddFilenameToStreamedScriptArray(char* pszFileName)
{
    char szMessage[256];

    if (g_nNumStreamedScripts >= ARRAY_SIZE(g_aStreamedScripts))
    {
        sprintf(szMessage, "ERROR Compiler can only handle %d streamed scripts", ARRAY_SIZE(g_aStreamedScripts));
        PrintMessage(szMessage);
    }
    if (strlen(pszFileName) >= SCRIPT_FILE_NAME_SIZE)
    {
        sprintf(szMessage, "AddFilenameToStreamedScriptArray - mission filename %s is too long\n", pszFileName);
        PrintMessage(szMessage);
    }
    bool bFound = false;
    for (uint16_t i = 0; i < g_nNumStreamedScripts; i++)
    {
        if (strcmp(g_aStreamedScripts[i].szName, pszFileName) == 0)
        {
            bFound = true;
            sprintf(pszFileName, "%d", i);
        }
    }
    if (!bFound)
    {
        strcpy(g_aStreamedScripts[g_nNumStreamedScripts].szName, pszFileName);
        sprintf(pszFileName, "%d", g_nNumStreamedScripts);
        g_nNumStreamedScripts++;
    }
}

// 0x0041CCB0 ?GetStreamedScriptIndexByName@@YAHPBD@Z
int GetStreamedScriptIndexByName(const char* pszName)
{
    uint16_t i = 0;
    while (i < g_nNumStreamedScripts)
    {
        if (strcmp(g_aStreamedScripts[i].szName, pszName) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

// 0x0041CD20 ?GetStreamedScriptHeaderSize@@YAHXZ
int GetStreamedScriptHeaderSize()
{
    if (g_nNumStreamedScripts != 0)
    {
        return (g_nNumStreamedScripts + 1) * STREAMED_HEADER_ENTRY_SIZE + STREAMED_HEADER_START_SIZE;
    }
    return STREAMED_HEADER_START_SIZE;
}

// 0x0041CD50 ?InitStreamedScripts@@YAXXZ
void InitStreamedScripts()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aStreamedScripts); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aStreamedScripts[i].szName); j++)
        {
            g_aStreamedScripts[i].szName[j] = '\0';
        }
        g_aStreamedScripts[i].nStartLOC = 0;
        g_aStreamedScripts[i].nStartOffset = 0;
        g_aStreamedScripts[i].nSize = 0;
        g_aStreamedScripts[i].bHasMissionStart = false;
        g_aStreamedScripts[i].bHasMissionEnd = false;
        g_aStreamedScripts[i].nNumLines = 0;
    }
    g_nNumStreamedScripts = 0;
    g_nLargestStreamedScriptSize = 0;
    for (uint16_t i = 0; i < ARRAY_SIZE(g_szLargestStreamedScript); i++)
    {
        g_szLargestStreamedScript[i] = '\0';
    }
    g_nStreamedScriptHeaderOffset = 0;
    g_nCurrentStreamedScript = -1;
}

// 0x0041CDD0 ?ReadStreamedScriptFiles@@YAHXZ
int16_t ReadStreamedScriptFiles()
{
    char szMainScript[INPUT_FILE_NAME_SIZE];
    char szMessage[128];

    strcpy(szMainScript, Config_InputFileName);

    for (uint16_t i = 0; i < g_nNumStreamedScripts; i++)
    {
        g_aStreamedScripts[i].nStartLOC = g_nNumLines;
        if (ReadScriptFile(i + FILE_INDEX_STREAMED_SCRIPTS) >= 0)
        {
            sprintf(szMessage, "Reading Streamed Script %s. . .", g_aStreamedScripts[i].szName);
            PrintMessage(szMessage);
        }
        else
        {
            sprintf(szMessage, "ERROR Reading Streamed Script %s", g_aStreamedScripts[i].szName);
            PrintMessage(szMessage);
            strcpy(Config_InputFileName, szMainScript);
            return -1;
        }
    }
    strcpy(Config_InputFileName, szMainScript);
    return 0;
}

// 0x0041CEF0 ?WriteStreamedScriptHeader@@YAXXZ
void WriteStreamedScriptHeader()
{
    char szBuffer[2048];

    int nCodeOffset = g_nCodeOffset;
    if (g_nNumStreamedScripts == 0)
    {
        g_nLargestStreamedScriptSize = 0;
        for (uint16_t i = 0; i < ARRAY_SIZE(g_szLargestStreamedScript); i++)
        {
            g_szLargestStreamedScript[i] = '\0';
        }
    }
    else
    {
        for (uint16_t i = 0; i < g_nNumStreamedScripts - 1; i++)
        {
            g_aStreamedScripts[i].nSize = g_aStreamedScripts[i + 1].nStartOffset - g_aStreamedScripts[i].nStartOffset;
        }
        g_aStreamedScripts[g_nNumStreamedScripts - 1].nSize =
            nCodeOffset - g_aStreamedScripts[g_nNumStreamedScripts - 1].nStartOffset;

        g_nLargestStreamedScriptSize = 0;
        for (uint16_t i = 0; i < g_nNumStreamedScripts; i++)
        {
            if (static_cast<uint32_t>(g_aStreamedScripts[i].nSize) > g_nLargestStreamedScriptSize)
            {
                g_nLargestStreamedScriptSize = g_aStreamedScripts[i].nSize;
                strcpy(g_szLargestStreamedScript, g_aStreamedScripts[i].szName);
            }
        }
    }

    g_nCodeOffset = g_nStreamedScriptHeaderOffset;
    WriteInt32(g_nLargestStreamedScriptSize);
    if (g_nNumStreamedScripts != 0)
    {
        WriteInt16(g_nNumStreamedScripts + 1);
    }
    else
    {
        WriteInt16(0);
    }
    WriteInt16(0);
    for (uint16_t i = 0; i < g_nNumStreamedScripts; i++)
    {
        bool bEnd = false;
        for (int j = 0; j < ARRAY_SIZE(g_aStreamedScripts[i].szName); j++)
        {
            if (g_aStreamedScripts[i].szName[j] == '.')
            {
                bEnd = true;
            }
            if (bEnd)
            {
                WriteArgumentDataType('\0', false);
            }
            else
            {
                WriteArgumentDataType(g_aStreamedScripts[i].szName[j], false);
            }
        }
        WriteInt32(g_aStreamedScripts[i].nStartOffset);
        WriteInt32(g_aStreamedScripts[i].nSize);
    }
    if (g_nNumStreamedScripts > 0)
    {
        for (int j = 0; j < AAA_SCRIPT_NAME_LENGTH; j++)
        {
            WriteArgumentDataType('A', false);
        }
        for (int j = 0; j < SCRIPT_FILE_NAME_SIZE - AAA_SCRIPT_NAME_LENGTH; j++)
        {
            WriteArgumentDataType('\0', false);
        }
        WriteInt32(0);
        WriteInt32(AAA_SCRIPT_SIZE);
    }
    g_nCodeOffset = nCodeOffset;

    for (int i = 0; i < g_nNumStreamedScripts; i++)
    {
        sprintf(szBuffer, "Streamed[%d] size %d StartCompileCodeIndex %d %s\n", i, g_aStreamedScripts[i].nSize,
            g_aStreamedScripts[i].nStartOffset, g_aStreamedScripts[i].szName);
        WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);
    }
}
