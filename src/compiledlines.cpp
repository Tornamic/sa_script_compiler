// compiledlines.cpp: 0x40D4D0-0x40DBFD in sc.exe

#include "common.h"
#include "compiledlines.h"
#include "codegen.h"

// 0x01583E78
uint32_t g_nPeakCompiledLinesMemory;
// 0x01583E7C
uint32_t g_nCompiledLinesMemory;
// 0x017E6ED0
tCompiledCommand* g_apCompiledLines[MAX_LINES][MAX_COMMANDS_PER_LINE];

// 0x0040D4D0
bool AllocArgAtLOC(int loc, uint16_t nCmd, uint16_t nArg)
{
    tCompiledArg* pArg = new tCompiledArg;
    if (pArg == nullptr)
    {
        return false;
    }
    pArg->nType = CARG_NONE;
    for (int i = 0; i < ARRAY_SIZE(pArg->szName); i++)
    {
        pArg->szName[i] = 0;
    }
    pArg->nValue = 0;
    g_apCompiledLines[loc][nCmd]->apArgs[nArg] = pArg;
    g_nCompiledLinesMemory += sizeof(tCompiledArg);
    if (g_nCompiledLinesMemory > g_nPeakCompiledLinesMemory)
    {
        g_nPeakCompiledLinesMemory = g_nCompiledLinesMemory;
    }
    return true;
}

// 0x0040D540
bool AllocCommandAtLOC(int loc, uint16_t nCmd)
{
    tCompiledCommand* pCommand = new tCompiledCommand;
    if (pCommand == nullptr)
    {
        return false;
    }
    pCommand->nCommand = COMMAND_NOP;
    pCommand->nAlternativeIndex = ALT_NONE;
    pCommand->bNot = false;
    for (int i = 0; i < ARRAY_SIZE(pCommand->apArgs); i++)
    {
        pCommand->apArgs[i] = nullptr;
    }
    g_apCompiledLines[loc][nCmd] = pCommand;
    g_nCompiledLinesMemory += sizeof(tCompiledCommand);
    if (g_nCompiledLinesMemory > g_nPeakCompiledLinesMemory)
    {
        g_nPeakCompiledLinesMemory = g_nCompiledLinesMemory;
    }
    return true;
}

// 0x0040D5C0
void FreeArgsAtLOC(int loc, uint16_t nCmd)
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_apCompiledLines[loc][nCmd]->apArgs); i++)
    {
        if (g_apCompiledLines[loc][nCmd]->apArgs[i] != nullptr)
        {
            delete g_apCompiledLines[loc][nCmd]->apArgs[i];
            g_apCompiledLines[loc][nCmd]->apArgs[i] = nullptr;
            g_nCompiledLinesMemory -= sizeof(tCompiledArg);
        }
    }
}

// 0x0040D620
void FreeCompiledLine(int loc)
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_apCompiledLines[loc]); i++)
    {
        FreeCommandAtLOC(loc, i);
    }
}

// 0x0040D6B0
void FreeCompiledLines()
{
    for (uint32_t i = 0; i < ARRAY_SIZE(g_apCompiledLines); i++)
    {
        FreeCompiledLine(i);
    }
}

// 0x0040D740
void FreeCommandAtLOC(int loc, uint16_t nCmd)
{
    if (g_apCompiledLines[loc][nCmd] != nullptr)
    {
        FreeArgsAtLOC(loc, nCmd);
        delete g_apCompiledLines[loc][nCmd];
        g_apCompiledLines[loc][nCmd] = nullptr;
        g_nCompiledLinesMemory -= sizeof(tCompiledCommand);
    }
}

// 0x0040D7C0
float GetArgFloatValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (!bForce && (pArg == nullptr))
    {
        return 0.0f;
    }
    return *reinterpret_cast<float*>(&pArg->nValue);
}

// 0x0040D7F0
int GetArgIntValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (!bForce && (pArg == nullptr))
    {
        return 0;
    }
    return pArg->nValue;
}

// 0x0040D820
char* GetArgStringAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (!bForce && (pArg == nullptr))
    {
        return nullptr;
    }
    return pArg->szName;
}

// 0x0040D850
eCompiledArgType GetArgTypeAtLOC(int loc, uint16_t nCmd, uint16_t nArg, bool bForce)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (!bForce && (pArg == nullptr))
    {
        return CARG_NONE;
    }
    return pArg->nType;
}

// 0x0040D880
int16_t GetAlternativeIndexAtLOC(int loc, uint16_t nCmd, bool bForce)
{
    tCompiledCommand* pCommand = g_apCompiledLines[loc][nCmd];
    if (!bForce && (pCommand == nullptr))
    {
        return ALT_NONE;
    }
    return pCommand->nAlternativeIndex;
}

// 0x0040D8B0
eScriptCommands GetCommandAtLOC(int loc, uint16_t nCmd, bool bForce)
{
    tCompiledCommand* pCommand = g_apCompiledLines[loc][nCmd];
    if (!bForce && (pCommand == nullptr))
    {
        return COMMAND_NOP;
    }
    return static_cast<eScriptCommands>(pCommand->nCommand);
}

// 0x0040D8E0
bool GetNotFlagAtLOC(int loc, uint16_t nCmd, bool bForce)
{
    tCompiledCommand* pCommand = g_apCompiledLines[loc][nCmd];
    if (!bForce && (pCommand == nullptr))
    {
        return false;
    }
    return pCommand->bNot;
}

// 0x0040D910
void InitCompiledLines()
{
    g_nPeakCompiledLinesMemory = 0;
    g_nCompiledLinesMemory = 0;
    FreeCompiledLines();
}

// 0x0040D9C0
void SetArgFloatValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, float fValue, bool bAlloc)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (bAlloc && (pArg == nullptr))
    {
        AllocArgAtLOC(loc, nCmd, nArg);
        pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    }
    *reinterpret_cast<float*>(&pArg->nValue) = fValue;
}

// 0x0040DA20
void SetArgIntValueAtLOC(int loc, uint16_t nCmd, uint16_t nArg, int nValue, bool bAlloc)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (bAlloc && (pArg == nullptr))
    {
        AllocArgAtLOC(loc, nCmd, nArg);
        pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    }
    pArg->nValue = nValue;
}

// 0x0040DA80
void SetArgStringAtLOC(int loc, uint16_t nCmd, uint16_t nArg, const char* pszName, bool bAlloc)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (bAlloc && (pArg == nullptr))
    {
        AllocArgAtLOC(loc, nCmd, nArg);
        pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    }
    strncpy(pArg->szName, pszName, sizeof(pArg->szName));
}

// 0x0040DAE0
void SetArgTypeAtLOC(int loc, uint16_t nCmd, uint16_t nArg, eCompiledArgType nType, bool bAlloc)
{
    tCompiledArg* pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    if (bAlloc && (pArg == nullptr))
    {
        AllocArgAtLOC(loc, nCmd, nArg);
        pArg = g_apCompiledLines[loc][nCmd]->apArgs[nArg];
    }
    pArg->nType = nType;
}

// 0x0040DB40
void SetAlternativeIndexAtLOC(int loc, uint16_t nCmd, int16_t nAlt, bool bAlloc)
{
    tCompiledCommand* pCommand = g_apCompiledLines[loc][nCmd];
    if (bAlloc && (pCommand == nullptr))
    {
        AllocCommandAtLOC(loc, nCmd);
        pCommand = g_apCompiledLines[loc][nCmd];
    }
    pCommand->nAlternativeIndex = nAlt;
}

// 0x0040DB80
void SetCommandAtLOC(int loc, uint16_t nCmd, int16_t nCommand, bool bAlloc)
{
    tCompiledCommand* pCommand = g_apCompiledLines[loc][nCmd];
    if (bAlloc && (pCommand == nullptr))
    {
        AllocCommandAtLOC(loc, nCmd);
        pCommand = g_apCompiledLines[loc][nCmd];
    }
    pCommand->nCommand = nCommand;
}

// 0x0040DBC0
void SetNotFlagAtLOC(int loc, uint16_t nCmd, bool bNot, bool bAlloc)
{
    tCompiledCommand* pCommand = g_apCompiledLines[loc][nCmd];
    if (bAlloc && (pCommand == nullptr))
    {
        AllocCommandAtLOC(loc, nCmd);
        pCommand = g_apCompiledLines[loc][nCmd];
    }
    pCommand->bNot = bNot;
}
