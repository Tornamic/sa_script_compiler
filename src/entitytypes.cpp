// entitytypes.cpp: 0x41E150-0x41EA57 in sc.exe

#include "common.h"
#include "entitytypes.h"
#include "compiler.h"
#include "compiledlines.h"
#include "codegen.h"
#include "errors.h"
#include "locals.h"
#include "streamedscripts.h"

// 0x0041E150 ?CheckArgumentEntityType@@YADHGF@Z
int8_t CheckArgumentEntityType(int loc, uint16_t nCmd, int16_t nArg)
{
    char szError[128];
    char szName[NAME_SIZE];

    if (GetArgTypeAtLOC(loc, nCmd, nArg, false) != CARG_NAME)
    {
        return 0;
    }

    uint16_t nCommand = GetCommandAtLOC(loc, nCmd, true);
    uint8_t nExpectedType = g_aCommandInfo[nCommand].aArgEntityTypes[nArg];
    if (g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_LABEL ||
        g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_TEXT_LABEL ||
        g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_STRING ||
        g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_TEXT_LABEL16 || nExpectedType == TYPE_UNIMPORTANT)
    {
        return 0;
    }

    if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, nArg, true), szName, loc, nullptr, nullptr) < 0)
    {
        return -1;
    }

    uint16_t nEntityType;
    int16_t nGlobalVar = GetGlobalVariableIndex(szName);
    if (nGlobalVar >= 0)
    {
        nEntityType = g_aGlobalVarEntityType[nGlobalVar];
    }
    else
    {
        int16_t nLocalVar = GetLocalVariableIndex(szName, loc);
        if (nLocalVar >= 0)
        {
            nEntityType = LocalVariables::GetEntityType(g_aScopeOfLine[loc], nLocalVar);
        }
        else
        {
            ReportCompilationError("**** NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", loc);
            return -1;
        }
    }

    if (nEntityType == nExpectedType)
    {
        return 0;
    }

    if (nEntityType == TYPE_UNIMPORTANT)
    {
        sprintf(szError, "Argument %d : expected type %d, variable hasn't been created yet (e.g. no CREATE_CAR)", nArg,
            nExpectedType);
    }
    else
    {
        sprintf(szError,
            "Argument %d : expected type %d, variable created with type %d for example created as a vehicle but "
            "treated as a ped",
            nArg, nExpectedType, nEntityType);
    }
    ReportCompilationError(szError, loc);
    return -1;
}

// 0x0041E330 ?CheckAssignmentVariableTypes@@YADHG@Z
int8_t CheckAssignmentVariableTypes(int loc, uint16_t nCmd)
{
    bool bFirstLocal;
    bool bSecondLocal;
    char szName[NAME_SIZE];

    switch (GetCommandAtLOC(loc, nCmd, true))
    {
        case COMMAND_SET_VAR_INT_TO_VAR_INT:
            bFirstLocal = false;
            bSecondLocal = false;
            break;
        case COMMAND_SET_LVAR_INT_TO_LVAR_INT:
            bFirstLocal = true;
            bSecondLocal = true;
            break;
        case COMMAND_SET_VAR_INT_TO_LVAR_INT:
            bFirstLocal = false;
            bSecondLocal = true;
            break;
        case COMMAND_SET_LVAR_INT_TO_VAR_INT:
            bFirstLocal = true;
            bSecondLocal = false;
            break;
        default:
            ReportCompilationError("**** UNEXPECTED ASSIGNMENT OPERATION", loc);
            return -1;
    }

    uint16_t i = 0;
    int16_t nNumVars = 0;
    for (; i < MAX_ARGS; i++)
    {
        if (GetArgTypeAtLOC(loc, nCmd, i, false) == CARG_NONE)
        {
            break;
        }
        if (GetArgTypeAtLOC(loc, nCmd, i, true) == CARG_NAME)
        {
            nNumVars++;
        }
    }
    if (nNumVars != 2)
    {
        ReportCompilationError("**** ASSIGNMENT OPERATION DOES NOT HAVE 2 VARIABLES", loc);
        return -1;
    }

    if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, 1, true), szName, loc, nullptr, nullptr) < 0)
    {
        return -1;
    }

    int16_t nEntityType;
    if (bSecondLocal)
    {
        int16_t nLocalVar = GetLocalVariableIndex(szName, loc);
        if (nLocalVar < 0)
        {
            ReportCompilationError("**** EXPECTED SECOND VARIABLE TO BE LOCAL", loc);
            return -1;
        }
        nEntityType = LocalVariables::GetEntityType(g_aScopeOfLine[loc], nLocalVar);
    }
    else
    {
        int16_t nGlobalVar = GetGlobalVariableIndex(szName);
        if (nGlobalVar < 0)
        {
            ReportCompilationError("**** EXPECTED SECOND VARIABLE TO BE GLOBAL", loc);
            return -1;
        }
        nEntityType = g_aGlobalVarEntityType[nGlobalVar];
    }

    if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, 0, true), szName, loc, nullptr, nullptr) < 0)
    {
        return -1;
    }

    if (bFirstLocal)
    {
        int16_t nLocalVar = GetLocalVariableIndex(szName, loc);
        if (nLocalVar < 0)
        {
            ReportCompilationError("**** EXPECTED FIRST VARIABLE TO BE LOCAL", loc);
            return -1;
        }
        int16_t nFirstType = LocalVariables::GetEntityType(g_aScopeOfLine[loc], nLocalVar);
        if (nFirstType != TYPE_UNIMPORTANT && nFirstType != nEntityType)
        {
            ReportCompilationError("**** ASSIGNMENT OPERATION HAS BEEN CALLED WITH VARIABLES OF DIFFERENT TYPES", loc);
            return -1;
        }
        LocalVariables::SetEntityType(g_aScopeOfLine[loc], nLocalVar, nEntityType);
    }
    else
    {
        int16_t nGlobalVar = GetGlobalVariableIndex(szName);
        if (nGlobalVar < 0)
        {
            ReportCompilationError("**** EXPECTED FIRST VARIABLE TO BE GLOBAL", loc);
            return -1;
        }
        int16_t nFirstType = g_aGlobalVarEntityType[nGlobalVar];
        if (nFirstType != TYPE_UNIMPORTANT && nFirstType != nEntityType)
        {
            ReportCompilationError("**** ASSIGNMENT OPERATION HAS BEEN CALLED WITH VARIABLES OF DIFFERENT TYPES", loc);
            return -1;
        }
        g_aGlobalVarEntityType[nGlobalVar] = nEntityType;
    }
    return 0;
}

// 0x0041E5A0 ?RecordCreatedEntityType@@YADHG@Z
int8_t RecordCreatedEntityType(int loc, uint16_t nCmd)
{
    int8_t nResult = 0;
    bool bFound = false;
    char szName[NAME_SIZE];

    uint16_t nCommand = GetCommandAtLOC(loc, nCmd, true);
    for (uint16_t i = 0; i < MAX_ARGS; i++)
    {
        if (g_aCommandInfo[nCommand].aArgTypes[i] != ARG_RETURN_INT)
        {
            continue;
        }

        bFound = true;
        if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, i, true), szName, loc, nullptr, nullptr) < 0)
        {
            return -1;
        }

        int16_t nType = g_aCommandInfo[nCommand].aArgEntityTypes[i];
        if (nType == TYPE_UNIMPORTANT)
        {
            ReportCompilationError("**** Error in code - Return value has TYPE_UNIMPORTANT", loc);
            return -1;
        }

        int16_t nGlobalVar = GetGlobalVariableIndex(szName);
        if (nGlobalVar >= 0)
        {
            int16_t nOldType = g_aGlobalVarEntityType[nGlobalVar];
            if (nOldType != TYPE_UNIMPORTANT && nOldType != nType)
            {
                ReportCompilationError(
                    "Variable has already been used to create entity of a different type e.g. CREATE_CHAR then "
                    "CREATE_CAR",
                    loc);
                nResult = -1;
            }
            else
            {
                g_aGlobalVarEntityType[nGlobalVar] = nType;
                nResult = 0;
            }
        }
        else
        {
            int16_t nLocalVar = GetLocalVariableIndex(szName, loc);
            if (nLocalVar >= 0)
            {
                int16_t nOldType = LocalVariables::GetEntityType(g_aScopeOfLine[loc], nLocalVar);
                if (nOldType != TYPE_UNIMPORTANT && nOldType != nType)
                {
                    ReportCompilationError("Local variable has already been used to create entity of a different type  "
                                           "e.g. CREATE_CHAR then CREATE_CAR",
                        loc);
                    nResult = -1;
                }
                else
                {
                    LocalVariables::SetEntityType(g_aScopeOfLine[loc], nLocalVar, nType);
                    nResult = 0;
                }
            }
            else
            {
                ReportCompilationError("**** NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", loc);
                return -1;
            }
        }
    }

    if (!bFound)
    {
        ReportCompilationError("Error in code - Create command doesn't pass back an integer", loc);
        return -1;
    }
    return nResult;
}

// 0x0041E770 ?CheckScriptLaunchArgumentTypes@@YADHGGH@Z
int8_t CheckScriptLaunchArgumentTypes(int loc, uint16_t nCmd, uint16_t nTargetScope, int nTargetLOC)
{
    char szName[NAME_SIZE];

    for (uint16_t i = 1; i < MAX_ARGS; i++)
    {
        if (GetArgTypeAtLOC(loc, nCmd, i, false) == CARG_NONE)
        {
            return 0;
        }
        if (GetArgTypeAtLOC(loc, nCmd, i, true) != CARG_NAME)
        {
            continue;
        }

        if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, i, true), szName, loc, nullptr, nullptr) < 0)
        {
            return -1;
        }

        int16_t nEntityType;
        int16_t nGlobalVar = GetGlobalVariableIndex(szName);
        if (nGlobalVar >= 0)
        {
            nEntityType = g_aGlobalVarEntityType[nGlobalVar];
        }
        else
        {
            int16_t nLocalVar = GetLocalVariableIndex(szName, loc);
            if (nLocalVar < 0)
            {
                ReportCompilationError("**** NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", loc);
                return -1;
            }
            nEntityType = LocalVariables::GetEntityType(g_aScopeOfLine[loc], nLocalVar);
        }

        int16_t nTargetType = LocalVariables::GetEntityType(nTargetScope, i - 1);
        if (nTargetType != TYPE_UNIMPORTANT && nTargetType != nEntityType)
        {
            ReportCompilationError("**** SCRIPT HAS BEEN CALLED WITH VARIABLES OF DIFFERENT TYPES", nTargetLOC);
            return -1;
        }
        LocalVariables::SetEntityType(nTargetScope, i - 1, nEntityType);
    }
    return 0;
}

// 0x0041E8D0 ?CheckStartNewScriptArguments@@YADHG@Z
int8_t CheckStartNewScriptArguments(int loc, uint16_t nCmd)
{
    if (GetCommandAtLOC(loc, nCmd, true) != COMMAND_START_NEW_SCRIPT)
    {
        ReportCompilationError("**** EXPECTED START_NEW_SCRIPT", loc);
        return -1;
    }
    int nLabelLOC = GetLabelLOC(GetArgStringAtLOC(loc, nCmd, 0, true));
    if (nLabelLOC < 0)
    {
        ReportCompilationError("**** THIS LABEL DOESN'T EXIST!", loc);
        return -1;
    }
    while (GetCommandAtLOC(nLabelLOC, 0, false) == COMMAND_NOP)
    {
        nLabelLOC++;
    }
    if (g_aScopeOfLine[nLabelLOC + 1] == NO_SCOPE)
    {
        ReportCompilationError("**** EXPECTED { BEFORE LABEL", nLabelLOC + 1);
        return -1;
    }
    return CheckScriptLaunchArgumentTypes(loc, nCmd, g_aScopeOfLine[nLabelLOC + 1], nLabelLOC + 1);
}

// 0x0041E990 ?CheckStartNewStreamedScriptArguments@@YADHG@Z
int8_t CheckStartNewStreamedScriptArguments(int loc, uint16_t nCmd)
{
    if (GetCommandAtLOC(loc, nCmd, true) != COMMAND_START_NEW_STREAMED_SCRIPT)
    {
        ReportCompilationError("**** EXPECTED START_NEW_STREAMED_SCRIPT", loc);
        return -1;
    }
    int nStartLOC = g_aStreamedScripts[GetArgIntValueAtLOC(loc, nCmd, 0, true)].nStartLOC;
    if (nStartLOC < 0)
    {
        ReportCompilationError("**** COULDN'T FIND STREAMED SCRIPT WITH THIS NAME", loc);
        return -1;
    }
    while (GetCommandAtLOC(nStartLOC, 0, false) == COMMAND_NOP)
    {
        nStartLOC++;
    }
    if (g_aScopeOfLine[nStartLOC + 1] == NO_SCOPE)
    {
        ReportCompilationError("**** EXPECTED { BEFORE LABEL", nStartLOC + 1);
        return -1;
    }
    return CheckScriptLaunchArgumentTypes(loc, nCmd, g_aScopeOfLine[nStartLOC + 1], nStartLOC + 1);
}
