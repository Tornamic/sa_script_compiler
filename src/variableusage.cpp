// variableusage.cpp: 0x41EE00-0x420606 in sc.exe

#include "common.h"
#include "variableusage.h"
#include "compiler.h"
#include "compiledlines.h"
#include "codegen.h"
#include "locals.h"
#include "errors.h"
#include "textkeys.h"
#include "namemaps.h"
#include "missions.h"
#include "streamedscripts.h"
#include "main.h"

// 0x02B66568
tConstantUsage g_aConstantUsage[MAX_CONSTANT_USAGE];
// 0x03551668
int g_nNumConstantUsage;
// 0x0355166C
int g_nNumLinesUsingConstants;
// 0x03551670
tConstantUsage g_aLinesUsingConstants[MAX_CONSTANT_USAGE];

// 0x044F54F4
std::map<int, tArrayAccessRecord> g_mapArrayAccess;

// 0x0041EE00 ?GetGlobalArrayElementOffset@@YAFH@Z
int16_t GetGlobalArrayElementOffset(int nGlobalVar)
{
    int16_t nOffset = 1;

    if (strcmp(g_aGlobalVarNames[nGlobalVar], "") == 0)
    {
        while (strcmp(g_aGlobalVarNames[nGlobalVar - nOffset], "") == 0)
        {
            nOffset++;
        }
        return nOffset;
    }
    return -1;
}

// 0x0041EED0 ?RecordConstantUsage@@YAXPAD0F@Z
void RecordConstantUsage(const char* pszName, const char* pszType, int16_t nValue)
{
    if (strcmp(pszType, "CONST") == 0)
    {
        for (int i = 0; i < g_nNumConstantUsage; i++)
        {
            if (strcmp(g_aConstantUsage[i].szName, pszName) == 0 && g_aConstantUsage[i].nValue == nValue)
            {
                return;
            }
        }

        if (g_nNumConstantUsage > CONSTANT_USAGE_WARNING_COUNT)
        {
            fprintf(stdout, "BitUsageCount %d", g_nNumConstantUsage);
        }

        strcpy(g_aConstantUsage[g_nNumConstantUsage].szName, pszName);
        strcpy(g_aConstantUsage[g_nNumConstantUsage].szType, pszType);
        g_aConstantUsage[g_nNumConstantUsage++].nValue = nValue;
    }
    else
    {
        for (int i = 0; i < g_nNumLinesUsingConstants; i++)
        {
            if (strcmp(g_aLinesUsingConstants[i].szName, pszName) == 0 &&
                strcmp(g_aLinesUsingConstants[i].szType, pszType) == 0 && g_aLinesUsingConstants[i].nValue == nValue)
            {
                return;
            }
        }

        strcpy(g_aLinesUsingConstants[g_nNumLinesUsingConstants].szName, pszName);
        strcpy(g_aLinesUsingConstants[g_nNumLinesUsingConstants].szType, pszType);
        g_aLinesUsingConstants[g_nNumLinesUsingConstants].nValue = nValue;
        g_nNumLinesUsingConstants++;
    }
}

// 0x0041F0E0 ?AddToArrayAccessMap@@YAXDFGGPAD@Z
void AddToArrayAccessMap(
    int8_t nVarKind, int16_t nIndexVar, uint16_t nGlobalVar, uint16_t nCodeOffset, const char* pszName)
{
    if (g_mapArrayAccess.find(nCodeOffset) == g_mapArrayAccess.end())
    {
        tArrayAccessRecord record;
        record.nVarKind = nVarKind;
        record.nIndexVar = nIndexVar;
        record.nGlobalVar = nGlobalVar;
        record.nCodeOffset = nCodeOffset;
        strcpy(record.szName, pszName);
        g_mapArrayAccess.insert(std::map<int, tArrayAccessRecord>::value_type(nCodeOffset, record));
    }
}

// 0x0041F1C0 ?SetVariableReadWriteFlags@@YAXHFDDD@Z
void SetVariableReadWriteFlags(int loc, int16_t nVar, bool bGlobal, bool bRead, bool bWritten)
{
    if (bGlobal)
    {
        if (bRead)
        {
            if ((g_aGlobalVarUsageFlags[nVar] & VAR_USAGE_WRITTEN) == 0)
            {
                g_aGlobalVarUsageFlags[nVar] |= VAR_USAGE_READ_BEFORE_WRITTEN;
            }
            g_aGlobalVarUsageFlags[nVar] |= VAR_USAGE_READ;
        }
        if (bWritten)
        {
            g_aGlobalVarUsageFlags[nVar] |= VAR_USAGE_WRITTEN;
        }
    }
    else
    {
        uint8_t nFlags = LocalVariables::GetUsageFlags(g_aScopeOfLine[loc], nVar);
        if (bRead)
        {
            if ((nFlags & VAR_USAGE_WRITTEN) == 0)
            {
                nFlags |= VAR_USAGE_READ_BEFORE_WRITTEN;
            }
            nFlags |= VAR_USAGE_READ;
        }
        if (bWritten)
        {
            nFlags |= VAR_USAGE_WRITTEN;
        }
        LocalVariables::SetUsageFlags(g_aScopeOfLine[loc], nVar, nFlags);
    }
}

// 0x0041F260 ?UpdateVariableUsageRecord@@YADHGF@Z
int8_t UpdateVariableUsageRecord(int loc, uint16_t nCmd, int16_t nArg)
{
    char szName[NAME_SIZE];
    bool bGlobal;
    bool bRead;
    bool bWritten;
    bool bHandled;
    int16_t nVar;

    eScriptCommands nCommand = GetCommandAtLOC(loc, nCmd, true);
    if (GetArgTypeAtLOC(loc, nCmd, nArg, false) != CARG_NAME)
    {
        return 0;
    }

    eArgType* pArgType = &g_aCommandInfo[nCommand].aArgTypes[nArg];
    if (*pArgType == ARG_LABEL || *pArgType == ARG_TEXT_LABEL || *pArgType == ARG_STRING ||
        *pArgType == ARG_TEXT_LABEL16)
    {
        return 0;
    }

    if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, nArg, true), szName, loc, nullptr, nullptr) < 0)
    {
        return -1;
    }

    nVar = GetGlobalVariableIndex(szName);
    if (nVar >= 0)
    {
        bGlobal = true;
    }
    else
    {
        nVar = GetLocalVariableIndex(szName, loc);
        if (nVar >= 0)
        {
            bGlobal = false;
        }
        else
        {
            ReportCompilationErrorWithName(
                "**** UpdateVariableUsageRecord - NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", szName, loc);
            return -1;
        }
    }

    bRead = false;
    bWritten = false;
    bHandled = false;

    switch (nCommand)
    {
        case COMMAND_SET_VAR_INT:
        case COMMAND_SET_VAR_FLOAT:
        case COMMAND_SET_LVAR_INT:
        case COMMAND_SET_LVAR_FLOAT:
        case COMMAND_SET_VAR_INT_TO_VAR_INT:
        case COMMAND_SET_LVAR_INT_TO_LVAR_INT:
        case COMMAND_SET_VAR_FLOAT_TO_VAR_FLOAT:
        case COMMAND_SET_LVAR_FLOAT_TO_LVAR_FLOAT:
        case COMMAND_SET_VAR_FLOAT_TO_LVAR_FLOAT:
        case COMMAND_SET_LVAR_FLOAT_TO_VAR_FLOAT:
        case COMMAND_SET_VAR_INT_TO_LVAR_INT:
        case COMMAND_SET_LVAR_INT_TO_VAR_INT:
        case COMMAND_CSET_VAR_INT_TO_VAR_FLOAT:
        case COMMAND_CSET_VAR_FLOAT_TO_VAR_INT:
        case COMMAND_CSET_LVAR_INT_TO_VAR_FLOAT:
        case COMMAND_CSET_LVAR_FLOAT_TO_VAR_INT:
        case COMMAND_CSET_VAR_INT_TO_LVAR_FLOAT:
        case COMMAND_CSET_VAR_FLOAT_TO_LVAR_INT:
        case COMMAND_CSET_LVAR_INT_TO_LVAR_FLOAT:
        case COMMAND_CSET_LVAR_FLOAT_TO_LVAR_INT:
        case COMMAND_GENERATE_RANDOM_FLOAT:
        case COMMAND_GENERATE_RANDOM_INT:
        case COMMAND_SET_VAR_INT_TO_CONSTANT:
        case COMMAND_SET_LVAR_INT_TO_CONSTANT:
        case COMMAND_SET_VAR_TEXT_LABEL:
        case COMMAND_SET_LVAR_TEXT_LABEL:
        case COMMAND_SET_VAR_TEXT_LABEL16:
        case COMMAND_SET_LVAR_TEXT_LABEL16:
            if (nArg == 0)
            {
                bWritten = true;
                bHandled = true;
            }
            break;
        case COMMAND_REPEAT:
            if (nArg == 1)
            {
                bWritten = true;
                bHandled = true;
            }
            break;
        case COMMAND_DECLARE_MISSION_FLAG_FOR_CONTACT:
            if (nArg == 1)
            {
                bHandled = true;
            }
            break;
        case COMMAND_START_NEW_SCRIPT:
        case COMMAND_START_NEW_STREAMED_SCRIPT:
            if (nArg > 0)
            {
                bRead = true;
                bHandled = true;
            }
            break;
        case COMMAND_VAR_INT:
        case COMMAND_VAR_FLOAT:
        case COMMAND_LVAR_INT:
        case COMMAND_LVAR_FLOAT:
        case COMMAND_DISPLAY_ONSCREEN_TIMER:
        case COMMAND_CLEAR_ONSCREEN_TIMER:
        case COMMAND_DISPLAY_ONSCREEN_COUNTER:
        case COMMAND_CLEAR_ONSCREEN_COUNTER:
        case COMMAND_DECLARE_MISSION_FLAG:
        case COMMAND_SAVE_VAR_INT:
        case COMMAND_SAVE_VAR_FLOAT:
        case COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING:
        case COMMAND_DISPLAY_ONSCREEN_COUNTER_WITH_STRING:
        case COMMAND_DISPLAY_NTH_ONSCREEN_COUNTER:
        case COMMAND_DISPLAY_NTH_ONSCREEN_COUNTER_WITH_STRING:
        case COMMAND_SET_ONSCREEN_COUNTER_FLASH_WHEN_FIRST_DISPLAYED:
        case COMMAND_VAR_TEXT_LABEL:
        case COMMAND_LVAR_TEXT_LABEL:
        case COMMAND_VAR_TEXT_LABEL16:
        case COMMAND_LVAR_TEXT_LABEL16:
        case COMMAND_DISPLAY_TWO_ONSCREEN_COUNTERS:
        case COMMAND_DISPLAY_TWO_ONSCREEN_COUNTERS_WITH_STRING:
        case COMMAND_DISPLAY_NTH_TWO_ONSCREEN_COUNTERS:
        case COMMAND_DISPLAY_NTH_TWO_ONSCREEN_COUNTERS_WITH_STRING:
            bHandled = true;
            break;
    }

    if (!bHandled)
    {
        switch (*pArgType)
        {
            case ARG_VAR_INT:
            case ARG_VAR_FLOAT:
            case ARG_LVAR_INT:
            case ARG_LVAR_FLOAT:
            case ARG_INT:
            case ARG_FLOAT:
            case ARG_VAR_TEXT_LABEL:
            case ARG_LVAR_TEXT_LABEL:
            case ARG_VAR_TEXT_LABEL16:
            case ARG_LVAR_TEXT_LABEL16:
                bRead = true;
                break;
            case ARG_RETURN_INT:
            case ARG_RETURN_FLOAT:
                bWritten = true;
                break;
            case ARG_NONE:
            case ARG_INT_CONST:
            case ARG_FLOAT_CONST:
            case ARG_DECLARE_VAR_INT:
            case ARG_DECLARE_VAR_FLOAT:
            case ARG_DECLARE_LVAR_INT:
            case ARG_DECLARE_LVAR_FLOAT:
            case ARG_SCRIPT_PARAM:
            case ARG_LABEL:
            case ARG_NAME:
            case ARG_TEXT_LABEL:
            case ARG_DECLARE_VAR_TEXT_LABEL:
            case ARG_DECLARE_LVAR_TEXT_LABEL:
            case ARG_STRING:
            case ARG_TEXT_LABEL16:
            case ARG_DECLARE_VAR_TEXT_LABEL16:
            case ARG_DECLARE_LVAR_TEXT_LABEL16:
            case ARG_UNKNOWN:
                ReportCompilationErrorWithName(
                    "**** UpdateVariableUsageRecord- UNEXPECTED VARIABLE TYPE ", szName, loc);
                return -1;
            default:
                ReportCompilationErrorWithName(
                    "**** UpdateVariableUsageRecord - UNEXPECTED VARIABLE TYPE ", szName, loc);
                return -1;
        }
    }

    if (bWritten && bRead)
    {
        ReportCompilationError(
            "**** UpdateVariableUsageRecord - VARIABLE CAN'T HAVE BEEN READ AND WRITTEN AT THE SAME TIME ", loc);
        return -1;
    }

    SetVariableReadWriteFlags(loc, nVar, bGlobal, bRead, bWritten);
    return 0;
}

// 0x0041F860 ?WriteSaveInfoFile@@YAXXZ
void WriteSaveInfoFile()
{
    char szScriptName[52];
    uint16_t nFileIndex;

    FILE* pFile = fopen(Config_bCompilingOrigScript ? "GTASASaveInfoOrig.dat" : "GTASASaveInfo.dat", "wb");
    if (pFile != nullptr)
    {
        fprintf(pFile, "//Globalvariables V2 %d\n", g_nNumGlobalVars);
        fprintf(pFile, "//MissionName, VariableName, arrayCount, index, used\n");
        for (int i = 0; i < g_nNumGlobalVars; i++)
        {
            if (GetGlobalArrayElementOffset(i) == -1)
            {
                int nUsed = g_aGlobalVarUsageFlags[i] & VAR_USAGE_READ_OR_WRITTEN;
                int nOffset = g_aGlobalVarOffset[i];
                int16_t nType = g_aGlobalVarType[i];
                g_DisplayedTextKeys.GetScriptNameFromFileIndex(g_aGlobalVarDeclaredInFile[i], szScriptName);
                fprintf(pFile, "%s,%s,%d,%d,%x,%d\n", szScriptName, g_aGlobalVarNames[i], g_aGlobalVarArraySize[i],
                    nOffset, nUsed, nType);
            }
        }
        fprintf(pFile, "//EndGlobalvariables\n");

        fprintf(pFile, "//LocalVariables\n");
        fprintf(pFile, "//MissionName, Variable, Size, Index, used\n");
        for (int nScope = 0; nScope < static_cast<int16_t>(g_nNumScopes); nScope++)
        {
            uint32_t nNumVars = LocalVariables::GetNumberOfLocalVars(nScope);
            int nVar = 0;
            if (LocalVariables::IsMissionScope(nScope))
            {
                nVar = NUM_LOCAL_VARS_AND_TIMERS;
            }
            for (; nVar < static_cast<int>(nNumVars); nVar++)
            {
                uint8_t nFlags = LocalVariables::GetUsageFlags(nScope, nVar);
                char* pszName = LocalVariables::GetName(nScope, nVar);
                int16_t nArraySize = LocalVariables::GetArraySize(nScope, nVar);
                if (strcmp(pszName, "") != 0)
                {
                    GetFileIndexAndLineFromLOC(g_aScopeStartLOC[nScope], reinterpret_cast<int16_t*>(&nFileIndex));
                    g_DisplayedTextKeys.GetScriptNameFromFileIndex(nFileIndex, szScriptName);
                    fprintf(pFile, "%s,%s,%d,%d,%d\n", szScriptName, pszName, nArraySize, nVar,
                        static_cast<int>((nFlags & VAR_USAGE_READ_OR_WRITTEN) != 0));
                }
            }
        }
        fprintf(pFile, "//EndLocalVariables\n");

        fprintf(pFile, "//Labels %d\n", g_mapLabels.size());
        fprintf(pFile, "//Name, LineNum, Address\n");
        for (std::map<std::string, int>::iterator it = g_mapLabels.begin(); it != g_mapLabels.end(); ++it)
        {
            if (strncmp(it->first.c_str(), SPECIAL_LABEL_PREFIX, SPECIAL_LABEL_PREFIX_LENGTH) != 0)
            {
                fprintf(pFile, "%s,%d,%d\n", it->first.c_str(), it->second, g_aCodeOffsetOfLine[it->second]);
            }
        }
        fprintf(pFile, "//EndLabels\n");

        fprintf(pFile, "//ScriptNames %d\n", g_nNumScriptNames);
        fprintf(pFile, "//Name, Index, Line\n");
        for (int i = 0; i < g_nNumScriptNames; i++)
        {
            fprintf(pFile, "%s,%d,%d\n", g_aScriptNames[i], i, g_aScriptNameLOC[i]);
        }
        fprintf(pFile, "//EndScriptNames\n");

        fprintf(pFile, "//VariableInfo %d\n", g_mapArrayAccess.size());
        fprintf(pFile, "//Index, Address, ArgumentType, TypeOfArrayElement\n");
        fprintf(pFile, "//EndVariableInfo\n");

        fprintf(pFile, "//ConstInfo %d\n", g_nNumConstantUsage);
        for (int i = 0; i < g_nNumConstantUsage; i++)
        {
            fprintf(pFile, "%s,%d\n", g_aConstantUsage[i].szName, g_aConstantUsage[i].nValue);
        }
        fprintf(pFile, "//EndConstInfo\n");

        fprintf(pFile, "//ConstVals %d\n", g_nNumLinesUsingConstants);
        fprintf(pFile, "//EndConstVals\n");

        fprintf(pFile, "//Scripts %d %d %d %d\n", g_nNumForeignGosubFiles, g_nNumSubscripts, NumberOfMissionScripts,
            g_nNumStreamedScripts);
        for (int i = 0; i < g_nNumForeignGosubFiles; i++)
        {
            fprintf(pFile, "Foreign[%d],%d,%s\n", i, g_aForeignGosubFiles[i].nNumLines, g_aForeignGosubFiles[i].szName);
        }

        fprintf(pFile, "//Subscripts %d\n", g_nNumSubscripts);
        fprintf(pFile, "//Index, Filename, startCompiled, StartLine, NumberLines, NumBytes\n");
        for (int i = 0; i < g_nNumSubscripts; i++)
        {
            fprintf(pFile, "%s,%d,%d,%d,%d,%d,%d\n", g_aSubscripts[i].szName, i + FILE_INDEX_SUBSCRIPTS, i,
                g_aSubscripts[i].nStartOffset, g_aSubscripts[i].nStartLOC, g_aSubscripts[i].nNumLines,
                g_aSubscripts[i].nSize);
        }
        fprintf(pFile, "//EndSubscripts\n");

        fprintf(pFile, "//Mission %d\n", NumberOfMissionScripts);
        fprintf(pFile, "//Index, ScriptIndex, Filename, startCompiled, StartLine, NumberLines, NumBytes\n");
        for (int i = 0; i < NumberOfMissionScripts; i++)
        {
            fprintf(pFile, "%s,%d,%d,%d,%d,%d,%d\n", g_aMissionScripts[i].szName, i + FILE_INDEX_MISSIONS, i,
                g_aMissionScripts[i].nStartOffset, g_aMissionScripts[i].nStartLOC, g_aMissionScripts[i].nNumLines,
                g_aMissionScripts[i].nSize);
        }
        fprintf(pFile, "//EndMission\n");

        fprintf(pFile, "//Streamed %d\n", g_nNumStreamedScripts);
        fprintf(pFile, "//Index, Filename, startCompiled, StartLine, NumberLines, NumBytes\n");
        for (int i = 0; i < g_nNumStreamedScripts; i++)
        {
            fprintf(pFile, "%s,%d,%d,%d,%d,%d,%d\n", g_aStreamedScripts[i].szName, i + FILE_INDEX_STREAMED_SCRIPTS, i,
                g_aStreamedScripts[i].nStartOffset, g_aStreamedScripts[i].nStartLOC, g_aStreamedScripts[i].nNumLines,
                g_aStreamedScripts[i].nSize);
        }
        fprintf(pFile, "//EndStreamed\n");

        fclose(pFile);
    }
}

// 0x0041FE50 ?WriteVariableInfoFile@@YAHXZ
int WriteVariableInfoFile()
{
    char szMessage[128];
    char szScriptName[40];
    uint16_t nFileIndex;

    FILE* pFile = fopen("variable_info.txt", "w");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open variable usage file - check it is writable");
        PrintMessage(szMessage);
        return -1;
    }

    fprintf(pFile, "//Global variables\n\n\n");
    fprintf(pFile, "struct stScriptVariables {\n\tchar* MissionName;\n\tchar* VariableName;\n\tint arrayCount\n"
                   "\tint index;\n\tint used;\n};\n\n");
    fprintf(pFile, "Script, Name, Size, UsedCount\n");
    fprintf(pFile, "stScriptVariables globalVariables = {\n");
    for (int16_t i = 0; i < g_nNumGlobalVars; i++)
    {
        if (GetGlobalArrayElementOffset(i) == -1)
        {
            uint8_t nFlags = g_aGlobalVarUsageFlags[i];
            int nOffset = g_aGlobalVarOffset[i];
            g_DisplayedTextKeys.GetScriptNameFromFileIndex(g_aGlobalVarDeclaredInFile[i], szScriptName);
            fprintf(pFile, "\t{\"%s\", \"%s\", %d, %d %x},", szScriptName, g_aGlobalVarNames[i],
                g_aGlobalVarArraySize[i], nOffset, nFlags & VAR_USAGE_READ_OR_WRITTEN);
            if ((g_aGlobalVarUsageFlags[i] & VAR_USAGE_READ_BEFORE_WRITTEN) != 0)
            {
                fprintf(pFile, " //read before being written\n");
            }
            else if ((g_aGlobalVarUsageFlags[i] & VAR_USAGE_READ_OR_WRITTEN) == 0)
            {
                fprintf(pFile, " // Not Used\n");
            }
            else
            {
                fprintf(pFile, "\n");
            }
        }
    }
    fprintf(pFile, "};\n");

    fprintf(pFile, "\n\n\n\n//Local variables\n\n");
    fprintf(pFile, "stScriptVariables localVariables = {\n");
    for (int16_t nScope = 0; nScope < static_cast<int16_t>(g_nNumScopes); nScope++)
    {
        uint16_t nNumVars = LocalVariables::GetNumberOfLocalVars(nScope);
        int16_t nVar;
        if (LocalVariables::IsMissionScope(nScope))
        {
            nVar = NUM_LOCAL_VARS_AND_TIMERS;
        }
        else
        {
            nVar = 0;
        }
        for (; nVar < nNumVars; nVar++)
        {
            uint8_t nFlags = LocalVariables::GetUsageFlags(nScope, nVar);
            char* pszName = LocalVariables::GetName(nScope, nVar);
            uint16_t nArraySize = LocalVariables::GetArraySize(nScope, nVar);
            if (strcmp(pszName, "") != 0)
            {
                bool bUsed = (nFlags & VAR_USAGE_READ_OR_WRITTEN) != 0;
                GetFileIndexAndLineFromLOC(g_aScopeStartLOC[nScope], reinterpret_cast<int16_t*>(&nFileIndex));
                g_DisplayedTextKeys.GetScriptNameFromFileIndex(nFileIndex, szScriptName);
                fprintf(pFile, "\t{\"%s\", \"%s\", %d, %d, %d},", szScriptName, pszName, nArraySize, nVar,
                    static_cast<int>(bUsed != 0));
                if ((nFlags & VAR_USAGE_READ_BEFORE_WRITTEN) != 0)
                {
                    fprintf(pFile, " //read before being written\n");
                }
                else if (!bUsed)
                {
                    fprintf(pFile, " // Not Used\n");
                }
                else
                {
                    fprintf(pFile, "\n");
                }
            }
        }
    }
    fprintf(pFile, "};\n");
    fclose(pFile);
    return 0;
}

// 0x004201A0 ?WriteVariableUsageFile@@YAHXZ
int WriteVariableUsageFile()
{
    char szMessage[128];
    char szScriptName[40];
    uint16_t nFileIndex;

    FILE* pFile = fopen("variable_usage.txt", "w");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open variable usage file - check it is writable");
        PrintMessage(szMessage);
        return -1;
    }

    fprintf(pFile, "Global variables\n\n\n");
    for (int16_t i = 0; i < g_nNumGlobalVars; i++)
    {
        bool bPrinted = false;
        if (GetGlobalArrayElementOffset(i) == -1)
        {
            if ((g_aGlobalVarUsageFlags[i] & VAR_USAGE_READ) == 0)
            {
                g_DisplayedTextKeys.GetScriptNameFromFileIndex(g_aGlobalVarDeclaredInFile[i], szScriptName);
                if (g_aGlobalVarArraySize[i] > 1)
                {
                    fprintf(pFile, "%s %s[%d] is never read\n", szScriptName, g_aGlobalVarNames[i],
                        g_aGlobalVarArraySize[i]);
                }
                else
                {
                    fprintf(pFile, "%s %s is never read\n", szScriptName, g_aGlobalVarNames[i]);
                }
                bPrinted = true;
            }
            if ((g_aGlobalVarUsageFlags[i] & VAR_USAGE_WRITTEN) == 0)
            {
                g_DisplayedTextKeys.GetScriptNameFromFileIndex(g_aGlobalVarDeclaredInFile[i], szScriptName);
                if (g_aGlobalVarArraySize[i] > 1)
                {
                    fprintf(pFile, "%s %s[%d] is never written\n", szScriptName, g_aGlobalVarNames[i],
                        g_aGlobalVarArraySize[i]);
                }
                else
                {
                    fprintf(pFile, "%s %s is never written\n", szScriptName, g_aGlobalVarNames[i]);
                }
                bPrinted = true;
            }
            if (bPrinted)
            {
                fprintf(pFile, "\n");
            }
        }
    }

    for (int16_t i = 0; i < g_nNumGlobalVars; i++)
    {
        if ((g_aGlobalVarUsageFlags[i] & VAR_USAGE_READ_OR_WRITTEN) == 0)
        {
            g_DisplayedTextKeys.GetScriptNameFromFileIndex(g_aGlobalVarDeclaredInFile[i], szScriptName);
            if (GetGlobalArrayElementOffset(i) == -1)
            {
                if (g_aGlobalVarArraySize[i] > 1)
                {
                    fprintf(pFile, "%s %s[%d]\n", szScriptName, g_aGlobalVarNames[i], g_aGlobalVarArraySize[i]);
                }
                else
                {
                    fprintf(pFile, "%s %s\n", szScriptName, g_aGlobalVarNames[i]);
                }
            }
        }
    }

    fprintf(pFile, "\n\n\n\nLocal variables\n\n\n");
    for (int16_t nScope = 0; nScope < static_cast<int16_t>(g_nNumScopes); nScope++)
    {
        uint16_t nNumVars = LocalVariables::GetNumberOfLocalVars(nScope);
        int16_t nVar;
        if (LocalVariables::IsMissionScope(nScope))
        {
            nVar = NUM_LOCAL_VARS_AND_TIMERS;
        }
        else
        {
            nVar = 0;
        }
        for (; nVar < nNumVars; nVar++)
        {
            bool bPrinted = false;
            uint8_t nFlags = LocalVariables::GetUsageFlags(nScope, nVar);
            char* pszName = LocalVariables::GetName(nScope, nVar);
            uint16_t nArraySize = LocalVariables::GetArraySize(nScope, nVar);
            if (strcmp(pszName, "") != 0)
            {
                if ((nFlags & VAR_USAGE_READ) == 0)
                {
                    GetFileIndexAndLineFromLOC(g_aScopeStartLOC[nScope], reinterpret_cast<int16_t*>(&nFileIndex));
                    g_DisplayedTextKeys.GetScriptNameFromFileIndex(nFileIndex, szScriptName);
                    if (nArraySize > 1)
                    {
                        fprintf(pFile, "%s %s[%d] is never read\n", szScriptName, pszName, nArraySize);
                    }
                    else
                    {
                        fprintf(pFile, "%s %s is never read\n", szScriptName, pszName);
                    }
                    bPrinted = true;
                }
                if ((nFlags & VAR_USAGE_WRITTEN) == 0)
                {
                    GetFileIndexAndLineFromLOC(g_aScopeStartLOC[nScope], reinterpret_cast<int16_t*>(&nFileIndex));
                    g_DisplayedTextKeys.GetScriptNameFromFileIndex(nFileIndex, szScriptName);
                    if (nArraySize > 1)
                    {
                        fprintf(pFile, "%s %s[%d] is never written\n", szScriptName, pszName, nArraySize);
                    }
                    else
                    {
                        fprintf(pFile, "%s %s is never written\n", szScriptName, pszName);
                    }
                    bPrinted = true;
                }
                if (bPrinted)
                {
                    fprintf(pFile, "\n");
                }
            }
        }
    }

    fclose(pFile);
    return 0;
}
