// locals.cpp: 0x409C60-0x40A29B in sc.exe

#include "common.h"
#include "locals.h"
#include "compiler.h"
#include "errors.h"
#include "main.h"
#include "namemaps.h"
extern "C" __declspec(noreturn) void __cdecl _wassert(
    const wchar_t* pszMessage, const wchar_t* pszFile, uint32_t nLine);

// 0x01463330
tLocalVarScope g_aLocalVarScopes[MAX_SCOPES];
// 0x01582FE0
uint16_t LargestNumberOfLocalVars;

namespace LocalVariables
{
bool IsMissionScope(uint16_t nScope);
int Init();
int16_t GetEntityType(uint16_t nScope, uint16_t nVar);
uint8_t GetUsageFlags(uint16_t nScope, uint16_t nVar);
uint16_t GetNumberOfLocalVars(uint16_t nScope);
tLocalVar* GetLocalVar(uint16_t nScope, uint16_t nVar);
int16_t GetArraySize(uint16_t nScope, uint16_t nVar);
char* GetName(uint16_t nScope, uint16_t nVar);
int16_t GetType(uint16_t nScope, uint16_t nVar);
void AddLocalVar(uint16_t nScope, int16_t* pnNewVar);
void SetEntityType(uint16_t nScope, uint16_t nVar, int16_t nEntityType);
void SetUsageFlags(uint16_t nScope, uint16_t nVar, uint8_t nFlags);
void SetMissionScope(uint16_t nScope);
void SetArraySize(uint16_t nScope, uint16_t nVar, int16_t nArraySize);
bool SetName(uint16_t nScope, uint16_t nVar, const char* pszName);
void SetType(uint16_t nScope, uint16_t nVar, int16_t nType);
}  // namespace LocalVariables

// 0x00409C60 ?IsMissionScope@LocalVariables@@YA_NG@Z
bool LocalVariables::IsMissionScope(uint16_t nScope)
{ return static_cast<int>(g_aLocalVarScopes[nScope].bIsMissionScope) != 0; }

// 0x00409C80 ?Init@LocalVariables@@YAHXZ
int LocalVariables::Init()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aLocalVarScopes); i++)
    {
        g_aLocalVarScopes[i].nNumVars = 0;
        g_aLocalVarScopes[i].bIsMissionScope = false;
        for (uint16_t j = 0; j < NUM_LOCAL_VARS_AND_TIMERS; j++)
        {
            tLocalVar& var = g_aLocalVarScopes[i].aVars[j];
            var.szName[0] = '\0';
            var.nType = VAR_TYPE_INT;
            var.nArraySize = 0;
            var.nEntityType = TYPE_UNIMPORTANT;
            var.nIndex = j;
            var.nUsageFlags = 0;
            var.pNext = nullptr;
        }
        tLocalVar* pVar = g_aLocalVarScopes[i].pExtraVars;
        while (pVar != nullptr)
        {
            tLocalVar* pNext = pVar->pNext;
            delete pVar;
            pVar = pNext;
        }
        g_aLocalVarScopes[i].pExtraVars = nullptr;
    }
    LargestNumberOfLocalVars = 0;
    return 0;
}

// 0x00409D00 ?IsLocalVariableNameInUse@@YA_NPAD@Z
bool IsLocalVariableNameInUse(const char* pszName)
{
    if (strcmp(pszName, "TIMERA") == 0)
    {
        return true;
    }
    if (strcmp(pszName, "TIMERB") == 0)
    {
        return true;
    }
    if (IsInSet(pszName, &g_mapAllLocalVariableNames))
    {
        return true;
    }
    return false;
}

// 0x00409D90 ?DoesLocalVariableExistInScope@@YA_NPADG@Z
bool DoesLocalVariableExistInScope(const char* pszName, uint16_t nScope)
{
    if (strcmp(pszName, "TIMERA") == 0)
    {
        return true;
    }
    if (strcmp(pszName, "TIMERB") == 0)
    {
        return true;
    }
    if (static_cast<int16_t>(GetValueFromMap(pszName, &g_aLocalVariableMaps[nScope], false)) >= 0)
    {
        return true;
    }
    return false;
}

// 0x00409E20 ?GetLocalVariableIndex@@YAFPADH@Z
int16_t GetLocalVariableIndex(const char* pszName, int loc)
{
    int16_t nScope = g_aScopeOfLine[loc];
    if (strcmp(pszName, "TIMERA") == 0)
    {
        if (nScope == NO_SCOPE)
        {
            ReportCompilationError("**** TIMERA CAN ONLY BE USED WITHIN CURLY BRACKETS", loc);
            return -1;
        }
        return LOCAL_VAR_TIMERA;
    }
    if (strcmp(pszName, "TIMERB") == 0)
    {
        if (nScope == NO_SCOPE)
        {
            ReportCompilationError("**** TIMERB CAN ONLY BE USED WITHIN CURLY BRACKETS", loc);
            return -1;
        }
        return LOCAL_VAR_TIMERB;
    }
    if (nScope == NO_SCOPE)
    {
        return -1;
    }
    return GetValueFromMap(pszName, &g_aLocalVariableMaps[static_cast<uint16_t>(nScope)], false);
}

// 0x00409F10 ?GetEntityType@LocalVariables@@YAFGG@Z
int16_t LocalVariables::GetEntityType(uint16_t nScope, uint16_t nVar)
{ return GetLocalVar(nScope, nVar)->nEntityType; }

// 0x00409F30 ?GetUsageFlags@LocalVariables@@YAEGG@Z
uint8_t LocalVariables::GetUsageFlags(uint16_t nScope, uint16_t nVar)
{ return GetLocalVar(nScope, nVar)->nUsageFlags; }

// 0x00409F50 ?GetNumberOfLocalVars@LocalVariables@@YAGG@Z
uint16_t LocalVariables::GetNumberOfLocalVars(uint16_t nScope)
{ return g_aLocalVarScopes[nScope].nNumVars; }

// 0x00409F70 ?GetLocalVar@LocalVariables@@YAPAUtLocalVar@@GG@Z
tLocalVar* LocalVariables::GetLocalVar(uint16_t nScope, uint16_t nVar)
{
    if (nVar == LOCAL_VAR_TIMERA || nVar == LOCAL_VAR_TIMERB)
    {
        return &g_aLocalVarScopes[nScope].aVars[nVar];
    }
    if (g_aLocalVarScopes[nScope].bIsMissionScope)
    {
        tLocalVar* pVar = g_aLocalVarScopes[nScope].pExtraVars;
        while (static_cast<uint16_t>(pVar->nIndex) != nVar)
        {
            pVar = pVar->pNext;
        }
        return pVar;
    }
    if (nVar >= NUM_LOCAL_VARS)
    {
        ReportCompilationError("**** Too Many LVAR's", 1);
        return nullptr;
    }
    return &g_aLocalVarScopes[nScope].aVars[nVar];
}

// 0x00409FF0 ?GetArraySize@LocalVariables@@YAFGG@Z
int16_t LocalVariables::GetArraySize(uint16_t nScope, uint16_t nVar)
{ return GetLocalVar(nScope, nVar)->nArraySize; }

// 0x0040A010 ?GetName@LocalVariables@@YAPADGG@Z
char* LocalVariables::GetName(uint16_t nScope, uint16_t nVar)
{ return GetLocalVar(nScope, nVar)->szName; }

// 0x0040A020 ?GetType@LocalVariables@@YAFGG@Z
int16_t LocalVariables::GetType(uint16_t nScope, uint16_t nVar)
{ return GetLocalVar(nScope, nVar)->nType; }

// 0x0040A040 ?AddLocalVar@LocalVariables@@YAXGPAF@Z
void LocalVariables::AddLocalVar(uint16_t nScope, int16_t* pnNewVar)
{
    if (g_aLocalVarScopes[nScope].bIsMissionScope)
    {
        tLocalVar* pLast = g_aLocalVarScopes[nScope].pExtraVars;
        uint16_t nIndex = NUM_LOCAL_VARS_AND_TIMERS;
        if (pLast == nullptr)
        {
            tLocalVar* pVar = new tLocalVar;
            pVar->szName[0] = '\0';
            pVar->nType = VAR_TYPE_INT;
            pVar->nArraySize = 0;
            pVar->nEntityType = TYPE_UNIMPORTANT;
            pVar->nIndex = nIndex;
            pVar->nUsageFlags = 0;
            pVar->pNext = nullptr;
            g_aLocalVarScopes[nScope].pExtraVars = pVar;
        }
        else
        {
            nIndex = NUM_LOCAL_VARS_AND_TIMERS + 1;
            for (tLocalVar* p = pLast->pNext; p != nullptr; p = p->pNext)
            {
                pLast = p;
                nIndex++;
            }
            tLocalVar* pVar = new tLocalVar;
            pVar->szName[0] = '\0';
            pVar->nType = VAR_TYPE_INT;
            pVar->nArraySize = 0;
            pVar->nEntityType = TYPE_UNIMPORTANT;
            pVar->nUsageFlags = 0;
            pVar->pNext = nullptr;
            pVar->nIndex = nIndex;
            pLast->pNext = pVar;
        }
        *pnNewVar = nIndex;
        nIndex++;
        g_aLocalVarScopes[nScope].nNumVars++;
        if (nIndex > LargestNumberOfLocalVars)
        {
            LargestNumberOfLocalVars = nIndex;
        }
    }
    else
    {
        uint32_t nNumVars = g_aLocalVarScopes[nScope].nNumVars;
        if (nNumVars >= NUM_LOCAL_VARS)
        {
            _wassert(L"**** ERROR TOO MANY LOCAL VARIABLES",
                L"h:\\projects\\3master\\projects\\gameface\\plugins\\gtabase\\tools\\sa_script_compiler\\locals.cpp",
                246);  // __LINE__ in the original source
        }
        *pnNewVar = nNumVars;
        g_aLocalVarScopes[nScope].nNumVars++;
    }
}

// 0x0040A130 ?SetEntityType@LocalVariables@@YAXGGF@Z
void LocalVariables::SetEntityType(uint16_t nScope, uint16_t nVar, int16_t nEntityType)
{ GetLocalVar(nScope, nVar)->nEntityType = nEntityType; }

// 0x0040A150 ?SetUsageFlags@LocalVariables@@YAXGGE@Z
void LocalVariables::SetUsageFlags(uint16_t nScope, uint16_t nVar, uint8_t nFlags)
{ GetLocalVar(nScope, nVar)->nUsageFlags = nFlags; }

// 0x0040A170 ?SetMissionScope@LocalVariables@@YAXG@Z
void LocalVariables::SetMissionScope(uint16_t nScope)
{
    if (!g_aLocalVarScopes[nScope].bIsMissionScope)
    {
        g_aLocalVarScopes[nScope].bIsMissionScope = true;
        g_aLocalVarScopes[nScope].nNumVars = NUM_LOCAL_VARS_AND_TIMERS;
    }
}

// 0x0040A1A0 ?SetArraySize@LocalVariables@@YAXGGF@Z
void LocalVariables::SetArraySize(uint16_t nScope, uint16_t nVar, int16_t nArraySize)
{ GetLocalVar(nScope, nVar)->nArraySize = nArraySize; }

// 0x0040A1C0 ?SetName@LocalVariables@@YADGGPAD@Z
bool LocalVariables::SetName(uint16_t nScope, uint16_t nVar, const char* pszName)
{
    char szMessage[128];
    tLocalVar* pVar = GetLocalVar(nScope, nVar);
    if (pVar == nullptr)
    {
        sprintf(szMessage, "Failed to add local variable %s", pszName);
        PrintMessage(szMessage);
        return false;
    }
    strcpy(pVar->szName, pszName);
    if (!AddToMap(&g_aLocalVariableMaps[nScope], pszName, nVar, true))
    {
        sprintf(szMessage, "Failed to add local variable %s to map", pszName);
        PrintMessage(szMessage);
        return false;
    }
    AddToSet(&g_mapAllLocalVariableNames, pszName);
    return true;
}

// 0x0040A280 ?SetType@LocalVariables@@YAXGGF@Z
void LocalVariables::SetType(uint16_t nScope, uint16_t nVar, int16_t nType)
{ GetLocalVar(nScope, nVar)->nType = nType; }
