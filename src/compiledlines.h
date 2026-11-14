#pragma once

#include "common.h"

struct tCompiledArg
{
    eCompiledArgType nType;
    char szName[NAME_SIZE];
    int nValue;
};

struct tCompiledCommand
{
    int16_t nCommand;
    int16_t nAlternativeIndex;
    bool bNot;
    tCompiledArg* apArgs[MAX_ARGS];
};

// --------------------------------------------------------------------------------- compiled lines

extern uint32_t g_nPeakCompiledLinesMemory;               // 0x01583E78
extern uint32_t g_nCompiledLinesMemory;                   // 0x01583E7C
extern tCompiledCommand*
    g_apCompiledLines[MAX_LINES][MAX_COMMANDS_PER_LINE];  // 0x017E6ED0 commands of each line, allocated on demand

// compiledlines.cpp

bool AllocArgAtLOC(int loc, uint16_t nCmd, uint16_t nArg);                                         // 0x0040D4D0
bool AllocCommandAtLOC(int loc, uint16_t nCmd);                                                    // 0x0040D540
void FreeArgsAtLOC(int loc, uint16_t nCmd);                                                        // 0x0040D5C0
void FreeCompiledLine(int loc);                                                                    // 0x0040D620
void FreeCompiledLines();                                                                          // 0x0040D6B0
void FreeCommandAtLOC(int loc, uint16_t nCmd);                                                     // 0x0040D740
float GetArgFloatValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce);                   // 0x0040D7C0
int GetArgIntValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce);                       // 0x0040D7F0
char* GetArgStringAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce);                       // 0x0040D820
eCompiledArgType GetArgTypeAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce);              // 0x0040D850
int16_t GetAlternativeIndexAtLOC(int loc, uint16_t nCmd, bool bForce);                             // 0x0040D880
eScriptCommands GetCommandAtLOC(int loc, uint16_t nCmd, bool bForce);                              // 0x0040D8B0
bool GetNotFlagAtLOC(int loc, uint16_t nCmd, bool bForce);                                         // 0x0040D8E0
void InitCompiledLines();                                                                          // 0x0040D910
void SetArgFloatValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, float fValue, bool bAlloc);      // 0x0040D9C0
void SetArgIntValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, int nValue, bool bAlloc);          // 0x0040DA20
void SetArgStringAtLOC(int loc, uint16_t nCmd, uint16_t nArg, const char* pszName, bool bAlloc);   // 0x0040DA80
void SetArgTypeAtLOC(int loc, uint16_t nCmd, uint16_t nArg, eCompiledArgType nType, bool bAlloc);  // 0x0040DAE0
void SetAlternativeIndexAtLOC(int loc, uint16_t nCmd, int16_t nAlt, bool bAlloc);                  // 0x0040DB40
void SetCommandAtLOC(int loc, uint16_t nCmd, int16_t nCommand, bool bAlloc);                       // 0x0040DB80
void SetNotFlagAtLOC(int loc, uint16_t nCmd, bool bNot, bool bAlloc);                              // 0x0040DBC0
