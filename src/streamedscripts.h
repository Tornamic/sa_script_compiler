#pragma once

#include "common.h"

// Streamed script header: largest size, number of entries and a 0, then name, start offset and size per script
#define STREAMED_HEADER_START_SIZE (CODE_SIZE_INT32 + 2 * CODE_SIZE_INT16)
#define STREAMED_HEADER_ENTRY_SIZE (SCRIPT_FILE_NAME_SIZE + 2 * CODE_SIZE_INT32)
// The last entry is a dummy "AAA" script, stored as aaa.scm: global variable space size and compile id
#define AAA_SCRIPT_NAME_LENGTH 3
#define AAA_SCRIPT_SIZE (2 * CODE_SIZE_INT32)

struct tStreamedScript
{
    char szName[SCRIPT_FILE_NAME_SIZE];
    int nStartLOC;
    int nStartOffset;
    int nSize;
    bool bHasMissionStart;
    bool bHasMissionEnd;
    uint16_t nNumLines;
};

// ------------------------------------------------------------------------------- streamed scripts

extern tStreamedScript g_aStreamedScripts[MAX_STREAMED_SCRIPTS];  // 0x02B226B0
extern uint16_t g_nNumStreamedScripts;                            // 0x02B23238
extern uint32_t g_nLargestStreamedScriptSize;                     // 0x02B2323C
extern char g_szLargestStreamedScript[SCRIPT_FILE_NAME_SIZE];     // 0x02B23240
extern int g_nStreamedScriptHeaderOffset;                         // 0x02B23254
extern int g_nCurrentStreamedScript;                              // 0x02B23258

// streamedscripts.cpp

void AddFilenameToStreamedScriptArray(char* pszFileName);  // 0x0041CB50
int GetStreamedScriptIndexByName(const char* pszName);     // 0x0041CCB0
int GetStreamedScriptHeaderSize();                         // 0x0041CD20
void InitStreamedScripts();                                // 0x0041CD50
int16_t ReadStreamedScriptFiles();                         // 0x0041CDD0
void WriteStreamedScriptHeader();                          // 0x0041CEF0
