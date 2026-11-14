// compiler.cpp: 0x401E80-0x40740F in sc.exe

#include "common.h"
#include "compiler.h"
#include "namemaps.h"
#include "errors.h"
#include "compiledlines.h"
#include "main.h"
#include "locals.h"
#include "parser.h"
#include "variableusage.h"
#include "codegen.h"
#include "streamedscripts.h"
#include "missions.h"
#include "loader.h"
#include "entitytypes.h"
#include "textkeys.h"

// 0x00B79070
int16_t g_nNumScriptNames;
// 0x00B79078
char g_aScriptNames[MAX_SCRIPT_NAMES][SCRIPT_NAME_SIZE];
// 0x00B79D60
int g_aScriptNameLOC[MAX_SCRIPT_NAMES];

// 0x00B79072
bool g_bCollectable1TotalSet;
// 0x00B79073
bool g_bProgressTotalSet;
// 0x00B79D5E
bool g_bMissionRespectTotalSet;
// 0x00B79D5F
bool g_bTotalNumberOfMissionsSet;
// 0x01445CD4
int g_nTotalCollectables;
// 0x01445CD8
int g_nTotalProgress;
// 0x01445CDC
int g_nTotalMissionRespect;
// 0x01445CE0
int g_nTotalMissionsPassed;
// 0x01445CE4
int g_nTotalMissionsGiven;

// 0x00B79074
int g_nCodeOffset;
// 0x00B7A288
uint8_t g_aCompiledCodeBuffer[COMPILED_CODE_BUFFER_SIZE];
// 0x00F4AB88
int g_aCodeOffsetOfLine[MAX_LINES];
// 0x01462F10
uint8_t* g_pCompiledCode;
// 0x01462F14
int g_nCompiledCodeBufferSize;

// 0x01194A88
int16_t g_aScopeOfLine[MAX_LINES];
// 0x012B9A08
int g_aScopeStartLOC[MAX_SCOPES];
// 0x01406000
uint16_t g_nNumScopes;

// 0x00B79D5C
int16_t g_nNumGlobalVars;
// 0x012B9F80
char g_aGlobalVarNames[MAX_GLOBAL_VARS][NAME_SIZE];
// 0x013F2780
uint16_t g_aGlobalVarArraySize[MAX_GLOBAL_VARS];
// 0x013FC3C0
int16_t g_aGlobalVarDeclaredInFile[MAX_GLOBAL_VARS];
// 0x014064D0
int16_t g_aGlobalVarEntityType[MAX_GLOBAL_VARS];
// 0x01410110
uint8_t g_aGlobalVarUsageFlags[MAX_GLOBAL_VARS];
// 0x01414F30
int g_aGlobalVarOffset[MAX_GLOBAL_VARS];
// 0x014287B0
int16_t g_aGlobalVarType[MAX_GLOBAL_VARS];
// 0x014323F0
int g_aSaveVarOffsets[MAX_GLOBAL_VARS];
// 0x01445CCC
int16_t g_nNumSaveVars;
// 0x01445CD0
int g_nGlobalVarSpaceSize;

// 0x01406008
tVarConsoleEntry g_aVarConsole[MAX_VAR_CONSOLE_ENTRIES];
// 0x014064C8
int16_t g_nNumVarConsoleEntries;

// 0x01445C70
int g_nStatFloatsWritten;
// 0x01445C74
int g_nStatInt32sWritten;
// 0x01445C78
int g_nStatInt16sWritten;
// 0x01445C7C
int g_nStatVarIndicesWritten;
// 0x01445C80
int g_nStatInt8sWritten;
// 0x01445C84
int g_nStatTextLabelsWritten;
// 0x01445C88
int g_nStatStringsWritten;
// 0x01445C8C
int g_nStatUnknown1;
// 0x01445C90
int g_nStatUnknown2;
// 0x01445C94
float g_fStatUnknown3;
// 0x01445C98
int g_nStatJumpsInt8;
// 0x01445C9C
int g_nStatJumpsInt16;
// 0x01445CA0
int g_nStatJumpsInt32;
// 0x01445CA4
int g_nStatArgEndOfArgs;
// 0x01445CA8
int g_nStatArgInt32;
// 0x01445CAC
int g_nStatArgInt16;
// 0x01445CB0
int g_nStatArgInt8;
// 0x01445CB4
int g_nStatArgGlobalVar;
// 0x01445CB8
int g_nStatArgLocalVar;
// 0x01445CBC
int g_nStatArgFloat;
// 0x01445CC0
int g_nStatArgGlobalArray;
// 0x01445CC4
int g_nStatArgLocalArray;
// 0x01445CC8
int g_nStatCommandsWritten;
// 0x01445CE8
int g_aCommandUsageCount[NUM_COMMANDS];
// 0x01448730
int g_aSortedCommandIds[NUM_COMMANDS];
// 0x0144B178
int g_aSortedCommandUsageCount[NUM_COMMANDS];

// 0x01406002
bool g_bSwitchHasDefault;
// 0x0144872C
int g_nSwitchLOC;
// 0x0144B174
int g_nEndSwitchLOC;
// 0x0144DBBC
int g_nSwitchDefaultLOC;
// 0x0144DBC0
int g_aSwitchBreakLOCs[MAX_SWITCH_BREAKS];
// 0x0144DCD8
uint16_t g_nNumSwitchBreaks;
// 0x0144DCDC
uint16_t g_nNumSwitchCases;
// 0x0144DCE0
tSwitchCase g_aSwitchCases[MAX_SWITCH_CASES];
// 0x0144DF10
int g_nSwitchCount;

// 0x0144DF14
FILE* pAlphaText;
// 0x0144DF18
FILE* pMainText;
// 0x0144DF1C
uint16_t g_nNumAlphabeticalTextKeys;
// 0x0144DF20
char g_aAlphabeticalTextKeys[MAX_TEXT_KEYS][TEXT_KEY_BUFFER_SIZE];

// inlined into AddGlobalVariableToVarConsole (its error string is emitted before the caller's)
static int16_t AddToVarConsoleArray(int nVar, int nArrayIndex, bool bIsFloat, int loc)
{
    if (g_nNumVarConsoleEntries >= ARRAY_SIZE(g_aVarConsole))
    {
        ReportCompilationError("**** AddToVarConsoleArray - Too many VarConsole variables", loc);
        return -1;
    }
    strncpy(g_aVarConsole[g_nNumVarConsoleEntries].szName, g_aGlobalVarNames[nVar], NAME_SIZE);
    g_aVarConsole[g_nNumVarConsoleEntries].nArrayIndex = nArrayIndex;
    g_aVarConsole[g_nNumVarConsoleEntries].nOffset =
        g_aGlobalVarOffset[nVar] + VAR_SIZE * nArrayIndex + GLOBAL_VARS_START;
    g_aVarConsole[g_nNumVarConsoleEntries].bIsFloat = bIsFloat;
    g_nNumVarConsoleEntries++;
    return 0;
}

// 0x00401E80 ?AddGlobalVariableToVarConsole@@YAFHD@Z
int16_t AddGlobalVariableToVarConsole(int loc, bool bIsFloat)
{
    int16_t nIndexGlobalVar;
    int16_t nIndexLocalVar;
    char szName[64];

    int16_t nArrayIndex =
        ParseVariableName(GetArgStringAtLOC(loc, 0, 0, true), szName, loc, &nIndexGlobalVar, &nIndexLocalVar);
    if (nArrayIndex < 0)
    {
        ReportCompilationError("**** AddGlobalVariableToVarConsole - ARRAY error", loc);
        return -1;
    }

    int16_t nVar = GetGlobalVariableIndex(szName);
    if (nVar < 0)
    {
        ReportCompilationError("**** THIS GLOBAL VARIABLE IS NOT DEFINED (2)", loc);
        return -1;
    }

    int16_t nType = g_aGlobalVarType[nVar];
    uint16_t nArraySize = g_aGlobalVarArraySize[nVar];
    if ((nType == VAR_TYPE_INT && bIsFloat) || (nType == VAR_TYPE_FLOAT && !bIsFloat))
    {
        ReportCompilationError("**** ARGUMENT TYPE MISMATCH - E.G. DECLARED AS VAR_INT USED AS FLOAT", loc);
        return -1;
    }

    if (nIndexGlobalVar >= 0 || nIndexLocalVar >= 0)
    {
        ReportCompilationError("**** CAN'T USE A VARIABLE TO INDEX INTO AN ARRAY WITH THIS COMMAND", loc);
        return -1;
    }

    if (nArrayIndex != 0 && nArrayIndex >= nArraySize)
    {
        ReportCompilationError("Array Index is greater than or equal to the size of the array", loc);
        return -1;
    }

    if (static_cast<uint32_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START) >= UINT16_MAX)
    {
        ReportCompilationError("Global Variable Index too big for UInt16", loc);
        return -1;
    }

    return AddToVarConsoleArray(nVar, nArrayIndex, bIsFloat, loc);
}

// 0x00402070 ?AddToAlphabeticalTextKeyList@@YAXPBD@Z
void AddToAlphabeticalTextKeyList(const char* pszKey)
{
    char szMessage[256];

    if (strlen(pszKey) > TEXT_LABEL_SIZE - 1)
    {
        sprintf(szMessage, "ERROR %s Text Key is too long", pszKey);
        PrintMessage(szMessage);
    }

    uint16_t i = 0;
    while (i < g_nNumAlphabeticalTextKeys)
    {
        if (strcmp(g_aAlphabeticalTextKeys[i], pszKey) == 0)
        {
            return;
        }
        i++;
    }
    strcpy(g_aAlphabeticalTextKeys[g_nNumAlphabeticalTextKeys], pszKey);
    g_nNumAlphabeticalTextKeys++;
}

// inlined into CompileLine (its three error calls share one call site: `mov eax,offset str; push esi; push eax`)
static int AddScriptName(int loc, uint16_t nCmd)
{
    char szScriptName[SCRIPT_NAME_SIZE];

    if (GetArgTypeAtLOC(loc, nCmd, 0, true) != CARG_NAME && GetArgTypeAtLOC(loc, nCmd, 0, true) != CARG_INT)
    {
        ReportCompilationError("**** Expected string identifier here", loc);
        return -1;
    }
    if (strlen(GetArgStringAtLOC(loc, nCmd, 0, true)) > TEXT_LABEL_SIZE - 1)
    {
        ReportCompilationError("**** String identifier is too long (only 7 chars please)", loc);
        return -1;
    }
    strcpy(szScriptName, GetArgStringAtLOC(loc, nCmd, 0, true));
    for (uint16_t i = 0; i < g_nNumScriptNames; i++)
    {
        if (strcmp(g_aScriptNames[i], szScriptName) == 0)
        {
            ReportCompilationError("**** Script Name has already been used", loc);
            return -1;
        }
    }
    strcpy(g_aScriptNames[g_nNumScriptNames], szScriptName);
    g_aScriptNameLOC[g_nNumScriptNames++] = loc;
    return 0;
}

// 0x00402150 ?CompileLine@@YAHH@Z
int CompileLine(int loc)
{
    for (int16_t nCmd = 0; nCmd < MAX_COMMANDS_PER_LINE; nCmd++)
    {
        int16_t nAlternative = GetAlternativeIndexAtLOC(loc, nCmd, false);
        if (nAlternative >= 0)
        {
            int16_t i = 0;
            SetCommandAtLOC(loc, nCmd, g_aCommandAlternatives[nAlternative].aCommands[i], false);
            while (DoArgumentsMatchCommand(loc, nCmd, GetCommandAtLOC(loc, nCmd, true)) < 0)
            {
                if (GetCommandAtLOC(loc, nCmd, true) < 0)
                {
                    ReportCompilationError("**** COULDN'T MATCH ALTERNATIVE WHILST COMPILING", loc);
                    return -1;
                }
                i++;
                SetCommandAtLOC(loc, nCmd, g_aCommandAlternatives[nAlternative].aCommands[i], false);
            }
        }

        if (GetCommandAtLOC(loc, nCmd, false) == COMMAND_SCRIPT_NAME)
        {
            if (AddScriptName(loc, nCmd) < 0)
            {
                return -1;
            }
        }

        switch (GetCommandAtLOC(loc, nCmd, false))
        {
            case COMMAND_NOP:
            case COMMAND_START_SCOPE:
            case COMMAND_END_SCOPE:
                break;
            case COMMAND_ENDREPEAT:
                ReportCompilationError("**** ONE ENDREPEAT TOO MANY", loc);
                return -1;
            case COMMAND_IF:
                ReportCompilationError("**** DIDN'T EXPECT TO FIND AN IF HERE", loc);
                return -1;
            case COMMAND_IFNOT:
                ReportCompilationError("**** DIDN'T EXPECT TO FIND AN IFNOT HERE", loc);
                return -1;
            case COMMAND_ELSE:
                ReportCompilationError("**** DIDN'T EXPECT TO FIND AN ELSE HERE", loc);
                return -1;
            case COMMAND_ENDIF:
                ReportCompilationError("**** ONE ENDIF TOO MANY", loc);
                return -1;
            case COMMAND_ENDWHILE:
                ReportCompilationError("**** ONE ENDWHILE TOO MANY", loc);
                return -1;
            case COMMAND_SWITCH:
                ReportCompilationError(
                    "**** ALL SWITCH COMMANDS SHOULD HAVE BEEN HANDLED BY NOW - Graeme has made a mistake", loc);
                return -1;
            case COMMAND_ENDSWITCH:
                ReportCompilationError("**** ONE ENDSWITCH TOO MANY", loc);
                return -1;
            case COMMAND_CASE:
                ReportCompilationError("**** ONE CASE TOO MANY", loc);
                return -1;
            case COMMAND_DEFAULT:
                ReportCompilationError("**** ONE DEFAULT TOO MANY", loc);
                return -1;
            case COMMAND_BREAK:
                ReportCompilationError("**** ONE BREAK TOO MANY", loc);
                return -1;
            case COMMAND_START_NEW_SCRIPT:
            case COMMAND_CREATE_PLAYER:
            case COMMAND_SET_VAR_INT_TO_VAR_INT:
            case COMMAND_SET_LVAR_INT_TO_LVAR_INT:
            case COMMAND_SET_VAR_INT_TO_LVAR_INT:
            case COMMAND_SET_LVAR_INT_TO_VAR_INT:
            case COMMAND_CREATE_CHAR:
            case COMMAND_CREATE_CAR:
            case COMMAND_STORE_CAR_CHAR_IS_IN:
            case COMMAND_STORE_CAR_PLAYER_IS_IN:
            case COMMAND_CREATE_OBJECT:
            case COMMAND_CREATE_CHAR_INSIDE_CAR:
            case COMMAND_CREATE_GANG_CAR:
            case COMMAND_CREATE_CAR_GENERATOR:
            case COMMAND_ADD_BLIP_FOR_CAR_OLD:
            case COMMAND_ADD_BLIP_FOR_CHAR_OLD:
            case COMMAND_ADD_BLIP_FOR_OBJECT_OLD:
            case COMMAND_ADD_BLIP_FOR_COORD_OLD:
            case COMMAND_ADD_BLIP_FOR_CAR:
            case COMMAND_ADD_BLIP_FOR_CHAR:
            case COMMAND_ADD_BLIP_FOR_OBJECT:
            case COMMAND_ADD_BLIP_FOR_CONTACT_POINT:
            case COMMAND_ADD_BLIP_FOR_COORD:
            case COMMAND_ADD_CONTINUOUS_SOUND:
            case COMMAND_CREATE_CHAR_AS_PASSENGER:
            case COMMAND_GET_PLAYER_CHAR:
            case COMMAND_CREATE_PICKUP:
            case COMMAND_GRAB_PHONE:
            case COMMAND_ADD_PEDS_IN_AREA_TO_COLL:
            case COMMAND_ADD_PEDS_IN_VEHICLE_TO_COLL:
            case COMMAND_CREATE_OBJECT_NO_OFFSET:
            case COMMAND_ADD_SPRITE_BLIP_FOR_CAR:
            case COMMAND_ADD_SPRITE_BLIP_FOR_CHAR:
            case COMMAND_ADD_SPRITE_BLIP_FOR_OBJECT:
            case COMMAND_ADD_SPRITE_BLIP_FOR_CONTACT_POINT:
            case COMMAND_ADD_SPRITE_BLIP_FOR_COORD:
            case COMMAND_START_SCRIPT_FIRE:
            case COMMAND_GET_RANDOM_CHAR_IN_AREA:
            case COMMAND_GET_RANDOM_CHAR_IN_ZONE:
            case COMMAND_CREATE_MONEY_PICKUP:
            case COMMAND_CREATE_CUTSCENE_OBJECT:
            case COMMAND_CREATE_CUTSCENE_HEAD:
            case COMMAND_START_CAR_FIRE:
            case COMMAND_START_CHAR_FIRE:
            case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_AREA:
            case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_ZONE:
            case COMMAND_CREATE_PICKUP_WITH_AMMO:
            case COMMAND_CREATE_FLOATING_PACKAGE:
            case COMMAND_CREATE_RANDOM_CHAR:
            case COMMAND_GRAB_CATALINA_HELI:
            case COMMAND_ADD_SPHERE:
            case COMMAND_STORE_CAR_CHAR_IS_IN_NO_SAVE:
            case COMMAND_STORE_CAR_PLAYER_IS_IN_NO_SAVE:
            case COMMAND_ADD_BLIP_FOR_PICKUP_OLD:
            case COMMAND_ADD_BLIP_FOR_PICKUP:
            case COMMAND_ADD_SPRITE_BLIP_FOR_PICKUP:
            case COMMAND_GET_CHAR_IN_CAR_PASSENGER_SEAT:
            case COMMAND_GET_CHASE_CAR:
            case COMMAND_GET_CLOSEST_OBJECT_OF_TYPE:
            case COMMAND_GET_RANDOM_COP_IN_AREA:
            case COMMAND_GET_RANDOM_COP_IN_ZONE:
            case COMMAND_GET_DRIVER_OF_CAR:
            case COMMAND_GET_REMOTE_CONTROLLED_CAR:
            case COMMAND_INITIALISE_OBJECT_PATH:
            case COMMAND_CREATE_PROTECTION_PICKUP:
            case COMMAND_ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD:
            case COMMAND_ADD_SHORT_RANGE_BLIP_FOR_COORD:
            case COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_COORD:
            case COMMAND_CREATE_SWAT_ROPE:
            case COMMAND_CREATE_LOCKED_PROPERTY_PICKUP:
            case COMMAND_CREATE_FORSALE_PROPERTY_PICKUP:
            case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_AREA_NO_SAVE:
            case COMMAND_CREATE_CLOTHES_PICKUP:
            case COMMAND_CREATE_RANDOM_CHAR_AS_DRIVER:
            case COMMAND_CREATE_RANDOM_CHAR_AS_PASSENGER:
            case COMMAND_ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT:
            case COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT:
            case COMMAND_GET_RANDOM_ICE_CREAM_CUSTOMER_IN_AREA:
            case COMMAND_GET_RANDOM_ICE_CREAM_CUSTOMER_IN_ZONE:
            case COMMAND_LOAD_CHAR_DECISION_MAKER:
            case COMMAND_OPEN_SEQUENCE_TASK:
            case COMMAND_ADD_ATTRACTOR:
            case COMMAND_CREATE_CHAR_AT_ATTRACTOR:
            case COMMAND_CREATE_GROUP:
            case COMMAND_OPEN_THREAT_LIST:
            case COMMAND_CREATE_PED_GENERATOR:
            case COMMAND_CREATE_FX_SYSTEM:
            case COMMAND_CREATE_FX_SYSTEM_WITH_DIRECTION:
            case COMMAND_CREATE_FX_SYSTEM_ON_CHAR:
            case COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION:
            case COMMAND_CREATE_FX_SYSTEM_ON_CAR:
            case COMMAND_CREATE_FX_SYSTEM_ON_CAR_WITH_DIRECTION:
            case COMMAND_CREATE_FX_SYSTEM_ON_OBJECT:
            case COMMAND_CREATE_FX_SYSTEM_ON_OBJECT_WITH_DIRECTION:
            case COMMAND_GET_CHAR_AT_SCRIPTED_ATTRACTOR:
            case COMMAND_OPEN_FRIEND_LIST:
            case COMMAND_LOAD_GROUP_DECISION_MAKER:
            case COMMAND_CREATE_SEARCHLIGHT:
            case COMMAND_CREATE_SEARCHLIGHT_ON_VEHICLE:
            case COMMAND_ADD_BLIP_FOR_SEARCHLIGHT:
            case COMMAND_CREATE_CHECKPOINT:
            case COMMAND_CREATE_MISSION_TRAIN:
            case COMMAND_GET_TRAIN_CABOOSE:
            case COMMAND_GET_RANDOM_CAR_IN_SPHERE:
            case COMMAND_GET_RANDOM_CAR_IN_SPHERE_NO_SAVE:
            case COMMAND_GET_RANDOM_CHAR_IN_SPHERE:
            case COMMAND_GET_TRAIN_CARRIAGE:
            case COMMAND_GRAB_ENTITY_ON_WINCH:
            case COMMAND_GRAB_ENTITY_ON_ROPE_FOR_OBJECT:
            case COMMAND_IS_CHAR_IN_ANY_SEARCHLIGHT:
            case COMMAND_GET_TRAILER_ATTACHED_TO_CAB:
            case COMMAND_GET_CAB_ATTACHED_TO_TRAILER:
            case COMMAND_GET_PLAYER_GROUP:
            case COMMAND_COPY_CHAR_DECISION_MAKER:
            case COMMAND_COPY_GROUP_DECISION_MAKER:
            case COMMAND_GET_CAR_CHAR_IS_USING:
            case COMMAND_FIND_NEAREST_MULTIBUILDING:
            case COMMAND_GET_GROUP_LEADER:
            case COMMAND_GET_CLOSEST_STEALABLE_OBJECT:
            case COMMAND_GET_VEHICLE_CHAR_IS_STANDING_ON:
            case COMMAND_ADD_BLIP_FOR_DEAD_CHAR:
            case COMMAND_GET_RANDOM_CHAR_IN_SPHERE_ONLY_DRUGS_BUYERS:
            case COMMAND_GET_RANDOM_CHAR_IN_SPHERE_NO_BRAIN:
            case COMMAND_START_NEW_STREAMED_SCRIPT:
            case COMMAND_GET_USER_OF_CLOSEST_MAP_ATTRACTOR:
            case COMMAND_COPY_SHARED_CHAR_DECISION_MAKER:
            case COMMAND_LOAD_SHARED_CHAR_DECISION_MAKER:
            case COMMAND_GET_CAR_BLOCKING_CAR:
            case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE:
            case COMMAND_CREATE_CAR_GENERATOR_WITH_PLATE:
            case COMMAND_STORE_CAR_CHAR_IS_ATTACHED_TO_NO_SAVE:
            case COMMAND_GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE:
            {
                int8_t nResult;
                eScriptCommands nCommand = GetCommandAtLOC(loc, nCmd, true);
                if (nCommand == COMMAND_SET_VAR_INT_TO_VAR_INT || nCommand == COMMAND_SET_LVAR_INT_TO_LVAR_INT ||
                    nCommand == COMMAND_SET_VAR_INT_TO_LVAR_INT || nCommand == COMMAND_SET_LVAR_INT_TO_VAR_INT)
                {
                    nResult = CheckAssignmentVariableTypes(loc, nCmd);
                }
                else if (nCommand == COMMAND_START_NEW_SCRIPT)
                {
                    nResult = CheckStartNewScriptArguments(loc, nCmd);
                }
                else if (nCommand == COMMAND_START_NEW_STREAMED_SCRIPT)
                {
                    nResult = CheckStartNewStreamedScriptArguments(loc, nCmd);
                }
                else
                {
                    nResult = RecordCreatedEntityType(loc, nCmd);
                }
                if (nResult < 0)
                {
                    return -1;
                }
            }
                // fall through
            default:
                if (CompileCommand(loc, nCmd) < 0)
                {
                    return -1;
                }
                break;
        }
    }
    return 0;
}

// 0x00402DB0 ?CompileAllLines@@YAFXZ
int16_t CompileAllLines()
{
    char szMessage[128];
    char szTemp[TEXT_KEY_BUFFER_SIZE];

    pAlphaText = fopen("alpha_text.txt", "w");
    if (pAlphaText == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open alphabetical text file key usage file - check it is writable");
        PrintMessage(szMessage);
        return -1;
    }
    pMainText = fopen("main_text.txt", "w");
    if (pMainText == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open text file version of the compiled script - check it is writable");
        PrintMessage(szMessage);
        fclose(pAlphaText);
        return -1;
    }

    g_nNumAlphabeticalTextKeys = 0;
    for (int i = 0; i < ARRAY_SIZE(g_aAlphabeticalTextKeys); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aAlphabeticalTextKeys[i]); j++)
        {
            g_aAlphabeticalTextKeys[i][j] = 0;
        }
    }

    sprintf(szMessage, "Resolving loops");
    PrintMessage(szMessage);
    if (static_cast<int16_t>(ResolveLoops()) < 0)
    {
        fclose(pAlphaText);
        fclose(pMainText);
        return -1;
    }
    sprintf(szMessage, "Finished resolving loops");
    PrintMessage(szMessage);

    g_nNumGlobalVars = 0;
    g_nGlobalVarSpaceSize = 0;
    g_nNumSaveVars = 0;
    g_nNumScriptNames = 0;
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aScriptNames); i++)
    {
        g_aScriptNames[i][0] = '\0';
    }
    for (int i = 0; i < ARRAY_SIZE(g_aSortedCommandUsageCount); i++)
    {
        g_aSortedCommandUsageCount[i] = 0;
    }
    for (int i = 0; i < ARRAY_SIZE(g_aSortedCommandIds); i++)
    {
        g_aSortedCommandIds[i] = 0;
    }
    for (int i = 0; i < ARRAY_SIZE(g_aCommandUsageCount); i++)
    {
        g_aCommandUsageCount[i] = 0;
    }
    g_DisplayedTextKeys.InitTextKeyList();

    g_bCollectable1TotalSet = false;
    g_nTotalCollectables = 0;
    g_bProgressTotalSet = false;
    g_nTotalProgress = 0;
    g_bMissionRespectTotalSet = false;
    g_nTotalMissionRespect = 0;
    g_bTotalNumberOfMissionsSet = false;
    g_nTotalMissionsPassed = 0;
    g_nTotalMissionsGiven = 0;

    LocalVariables::Init();
    for (int i = 0; i < MAX_GLOBAL_VARS; i++)
    {
        g_aGlobalVarNames[i][0] = '\0';
        g_aGlobalVarEntityType[i] = 0;
        g_aGlobalVarDeclaredInFile[i] = -1;
        g_aGlobalVarUsageFlags[i] = 0;
        g_aGlobalVarArraySize[i] = 0;
    }

    bool bInScope = false;
    uint32_t loc;
    const char* pszError = nullptr;
    g_nNumScopes = 0;
    for (loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        if (bInScope)
        {
            g_aScopeOfLine[loc] = g_nNumScopes;
            if (GetCommandAtLOC(loc, 0, false) == COMMAND_START_SCOPE)
            {
                pszError = "**** CANNOT NEST SCOPE BLOCKS";
                break;
            }
            if (GetCommandAtLOC(loc, 0, false) == COMMAND_END_SCOPE)
            {
                bInScope = false;
                g_nNumScopes++;
            }
        }
        else
        {
            g_aScopeOfLine[loc] = NO_SCOPE;
            if (GetCommandAtLOC(loc, 0, false) == COMMAND_START_SCOPE)
            {
                bInScope = true;
                g_aScopeStartLOC[static_cast<int16_t>(g_nNumScopes)] = loc;
            }
            else if (GetCommandAtLOC(loc, 0, false) == COMMAND_END_SCOPE)
            {
                pszError = "**** CANNOT END SCOPE. WASN'T IN ONE";
                break;
            }
        }
    }
    if (pszError == nullptr && bInScope)
    {
        pszError = "**** STILL WITHIN SCOPE AT END OF FILE";
    }
    if (pszError != nullptr)
    {
        ReportCompilationError(pszError, loc);
        fclose(pAlphaText);
        fclose(pMainText);
        return -1;
    }
    if (static_cast<int16_t>(DeclareVariables()) < 0)
    {
        fclose(pAlphaText);
        fclose(pMainText);
        return -1;
    }

    WriteScriptHeader();
    for (uint32_t loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        g_aCodeOffsetOfLine[loc] = g_nCodeOffset;
        if (g_nNumStreamedScripts > 0 && loc == g_aStreamedScripts[g_nCurrentStreamedScript + 1].nStartLOC)
        {
            g_nCurrentStreamedScript++;
            g_aStreamedScripts[g_nCurrentStreamedScript].nStartOffset = g_nCodeOffset;
            if (g_nCurrentStreamedScript == 0 && NumberOfMissionScripts == 0)
            {
                SizeOfMainScript = g_nCodeOffset;
            }
        }
        if (NumberOfMissionScripts > 0 && loc == g_aMissionScripts[g_nCurrentMissionScript + 1].nStartLOC)
        {
            g_nCurrentMissionScript++;
            g_aMissionScripts[g_nCurrentMissionScript].nStartOffset = g_nCodeOffset;
            if (g_nCurrentMissionScript == 0)
            {
                SizeOfMainScript = g_nCodeOffset;
            }
        }
        if (g_nNumSubscripts > 0 && loc == g_aSubscripts[g_nCurrentSubscript + 1].nStartLOC)
        {
            g_nCurrentSubscript++;
            g_aSubscripts[g_nCurrentSubscript].nStartOffset = g_nCodeOffset;
            if (g_nCurrentSubscript == 0)
            {
                g_nFirstSubscriptOffset = g_nCodeOffset;
            }
        }
        if (CompileLine(loc) < 0)
        {
            fclose(pAlphaText);
            fclose(pMainText);
            return -1;
        }
    }

    int nScriptStart;
    for (int i = 0; i < g_nNumLabelRefs; i++)
    {
        uint32_t nOffset = g_aLabelRefCodeOffset[i] + CODE_SIZE_DATA_TYPE;
        int nTarget;
        if (NumberOfMissionScripts > 0)
        {
            if (nOffset < static_cast<uint32_t>(g_aMissionScripts[0].nStartOffset))
            {
                int nLOC = *reinterpret_cast<int*>(&g_pCompiledCode[nOffset]);
                if (Config_TargetMode == SAN_ANDREAS_XBOX360)
                {
                    nLOC = static_cast<uint32_t>(nLOC) >> 24 | ((nLOC >> 8) & 0xFF00) | ((nLOC << 8) & 0xFF0000) |
                           (nLOC << 24);
                }
                if (nLOC >= INT8_MIN && nLOC < INT8_MAX + 1)
                {
                    g_nStatJumpsInt8--;
                }
                else if (nLOC >= INT16_MIN && nLOC < INT16_MAX + 1)
                {
                    g_nStatJumpsInt16--;
                }
                else
                {
                    g_nStatJumpsInt32--;
                }
                nTarget = g_aCodeOffsetOfLine[nLOC];
            }
            else
            {
                int nLOC = *reinterpret_cast<int*>(&g_pCompiledCode[nOffset]);
                if (Config_TargetMode == SAN_ANDREAS_XBOX360)
                {
                    nLOC = static_cast<uint32_t>(nLOC) >> 24 | ((nLOC >> 8) & 0xFF00) | ((nLOC << 8) & 0xFF0000) |
                           (nLOC << 24);
                }
                if (nLOC >= INT8_MIN && nLOC < INT8_MAX + 1)
                {
                    g_nStatJumpsInt8--;
                }
                else if (nLOC >= INT16_MIN && nLOC < INT16_MAX + 1)
                {
                    g_nStatJumpsInt16--;
                }
                else
                {
                    g_nStatJumpsInt32--;
                }
                nTarget = g_aCodeOffsetOfLine[nLOC];
                if (nTarget >= g_aMissionScripts[0].nStartOffset)
                {
                    int8_t nType;
                    int8_t nTargetType;
                    int nScript = GetScriptIndexFromOffset(nOffset, &nType);
                    GetScriptIndexFromOffset(nTarget, &nTargetType);
                    if (nType == SCRIPT_TYPE_MISSION)
                    {
                        nScriptStart = g_aMissionScripts[nScript].nStartOffset;
                    }
                    else if (nType == SCRIPT_TYPE_STREAMED)
                    {
                        nScriptStart = g_aStreamedScripts[nScript].nStartOffset;
                    }
                    nTarget = nScriptStart - nTarget;
                }
            }
        }
        else
        {
            int nLOC = *reinterpret_cast<int*>(&g_pCompiledCode[nOffset]);
            if (Config_TargetMode == SAN_ANDREAS_XBOX360)
            {
                nLOC = static_cast<uint32_t>(nLOC) >> 24 | ((nLOC >> 8) & 0xFF00) | ((nLOC << 8) & 0xFF0000) |
                       (nLOC << 24);
            }
            if (nLOC >= INT8_MIN && nLOC < INT8_MAX + 1)
            {
                g_nStatJumpsInt8--;
            }
            else if (nLOC >= INT16_MIN && nLOC < INT16_MAX + 1)
            {
                g_nStatJumpsInt16--;
            }
            else
            {
                g_nStatJumpsInt32--;
            }
            nTarget = g_aCodeOffsetOfLine[nLOC];
        }
        if (nTarget >= INT8_MIN && nTarget < INT8_MAX + 1)
        {
            g_nStatJumpsInt8++;
        }
        else if (nTarget >= INT16_MIN && nTarget < INT16_MAX + 1)
        {
            g_nStatJumpsInt16++;
        }
        else
        {
            g_nStatJumpsInt32++;
        }
        if (Config_TargetMode == SAN_ANDREAS_XBOX360)
        {
            nTarget = static_cast<uint32_t>(nTarget) >> 24 | ((nTarget >> 8) & 0xFF00) | ((nTarget << 8) & 0xFF0000) |
                      (nTarget << 24);
        }
        *reinterpret_cast<int*>(&g_pCompiledCode[nOffset]) = nTarget;
    }

    if (static_cast<int16_t>(WriteCompiledScript()) < 0)
    {
        fclose(pAlphaText);
        fclose(pMainText);
        return -1;
    }

    WriteCommandUsageFile();
    FILE* pFile = fopen("ordered_commands.txt", "w");
    if (pFile == nullptr)
    {
        char szError[128];
        sprintf(szError, "**** Couldn't open ordered command file - check it is writable");
        PrintMessage(szError);
    }
    else
    {
        for (uint16_t i = 0; i < ARRAY_SIZE(g_aCommandInfo); i++)
        {
            fprintf(pFile, "\t{%d,\"%s\"},\n", i, g_aCommandInfo[i].szName);
        }
        fclose(pFile);
    }
    WriteKeywordFiles(false);
    WriteKeywordFiles(true);
    WriteVariableUsageFile();
    WriteVariableInfoFile();
    WriteSaveInfoFile();

    if (g_nNumAlphabeticalTextKeys > 0)
    {
        for (uint16_t i = 0; i < g_nNumAlphabeticalTextKeys; i++)
        {
            for (uint16_t j = i; j < g_nNumAlphabeticalTextKeys; j++)
            {
                if (strcmp(g_aAlphabeticalTextKeys[i], g_aAlphabeticalTextKeys[j]) > 0)
                {
                    strcpy(szTemp, g_aAlphabeticalTextKeys[i]);
                    strcpy(g_aAlphabeticalTextKeys[i], g_aAlphabeticalTextKeys[j]);
                    strcpy(g_aAlphabeticalTextKeys[j], szTemp);
                }
            }
        }
        for (uint16_t i = 0; i < g_nNumAlphabeticalTextKeys; i++)
        {
            fprintf(pAlphaText, "%s\n", g_aAlphabeticalTextKeys[i]);
        }
    }
    fclose(pAlphaText);
    fclose(pMainText);

    WriteProgressListFile();
    WriteMissingSoundEffectsFile();
    WriteUnusedSoundEffectsFiles();
    WriteResourcesUsedByScriptFiles();
    g_DisplayedTextKeys.WriteMissionTextKeysFile();
    PrintMessage(" ");
    PrintMessage(" ");
    PrintMessage(" ");
    sprintf(szMessage, "Main script size  %d bytes", SizeOfMainScript);
    PrintMessage(szMessage);
    if (SizeOfLargestMissionScript != 0)
    {
        sprintf(
            szMessage, "Largest mission script %s size %d bytes", g_szLargestMissionScript, SizeOfLargestMissionScript);
        PrintMessage(szMessage);
    }
    sprintf(szMessage, "Compiler generated %d bytes from %d lines of code", g_nCodeOffset, g_nNumLines);
    PrintMessage(szMessage);
    sprintf(szMessage, "Globals %d, Most Locals %d", g_nNumGlobalVars, LargestNumberOfLocalVars);
    PrintMessage(szMessage);
    return 0;
}

// Arguments of the SWITCH_START command CreateSwitchTable fills in (the cases follow as value, label pairs,
// continued in SWITCH_CONTINUED commands)
enum eSwitchStartArgs
{
    SWITCH_ARG_VAR = 0,
    SWITCH_ARG_NUM_CASES,
    SWITCH_ARG_HAS_DEFAULT,
    SWITCH_ARG_DEFAULT_LABEL,
    SWITCH_ARG_FIRST_CASE,
};
#define SWITCH_UNUSED_CASE_VALUE -1  // value of the padding cases after the last one

// 0x004037D0 ?CreateSwitchTable@@YAHXZ
int CreateSwitchTable()
{
    char szEndSwitchLabel[NAME_SIZE];
    char szLabel[NAME_SIZE];

    for (uint16_t i = 0; i < g_nNumSwitchCases; i++)
    {
        for (uint16_t j = i + 1; j < g_nNumSwitchCases; j++)
        {
            if (g_aSwitchCases[i].nValue > g_aSwitchCases[j].nValue)
            {
                int nValue = g_aSwitchCases[i].nValue;
                g_aSwitchCases[i].nValue = g_aSwitchCases[j].nValue;
                g_aSwitchCases[j].nValue = nValue;
                int nLOC = g_aSwitchCases[i].nLOC;
                g_aSwitchCases[i].nLOC = g_aSwitchCases[j].nLOC;
                g_aSwitchCases[j].nLOC = nLOC;
            }
            else if (g_aSwitchCases[i].nValue == g_aSwitchCases[j].nValue)
            {
                ReportCompilationError(
                    "**** CreateSwitchTable - Can't have two CASEs with the same value", g_aSwitchCases[j].nLOC);
                return -1;
            }
        }
    }

    sprintf(szEndSwitchLabel, "SPECIAL_LABEL_ENDSWITCH_%d:", g_nSwitchCount);
    if (RegisterLabel(szEndSwitchLabel, g_nEndSwitchLOC) < 0)
    {
        ReportCompilationError(
            "**** CreateSwitchTable - Error when registering Label for the ENDSWITCH", g_nEndSwitchLOC);
        return -1;
    }

    SetArgTypeAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_NUM_CASES, CARG_INT, true);
    SetArgIntValueAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_NUM_CASES, g_nNumSwitchCases, false);
    SetArgTypeAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_HAS_DEFAULT, CARG_INT, true);
    SetArgIntValueAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_HAS_DEFAULT, static_cast<int>(g_bSwitchHasDefault), false);
    SetArgTypeAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_DEFAULT_LABEL, CARG_NAME, true);
    if (g_bSwitchHasDefault)
    {
        sprintf(szLabel, "SPECIAL_LABEL_SWITCH_DEFAULT_%d:", g_nSwitchCount);
        if (RegisterLabel(szLabel, g_nSwitchDefaultLOC) < 0)
        {
            ReportCompilationError(
                "**** CreateSwitchTable - Error when registering Label for the DEFAULT", g_nSwitchDefaultLOC);
            return -1;
        }
        SetArgStringAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_DEFAULT_LABEL, szLabel, false);
    }
    else
    {
        SetArgStringAtLOC(g_nSwitchLOC, 0, SWITCH_ARG_DEFAULT_LABEL, szEndSwitchLabel, false);
    }

    uint16_t i = 0;
    uint8_t nCmd = 0;
    uint8_t nArg = SWITCH_ARG_FIRST_CASE;
    while (i < g_nNumSwitchCases)
    {
        SetArgTypeAtLOC(g_nSwitchLOC, nCmd, nArg, CARG_INT, true);
        SetArgIntValueAtLOC(g_nSwitchLOC, nCmd, nArg, g_aSwitchCases[i].nValue, false);
        nArg++;
        sprintf(szLabel, "SPECIAL_LABEL_SWITCH_CASE_%d_%d:", g_nSwitchCount, i);
        if (RegisterLabel(szLabel, g_aSwitchCases[i].nLOC) < 0)
        {
            ReportCompilationError(
                "**** CreateSwitchTable - Error when registering Label for a CASE", g_aSwitchCases[i].nLOC);
            return -1;
        }
        SetArgTypeAtLOC(g_nSwitchLOC, nCmd, nArg, CARG_NAME, true);
        SetArgStringAtLOC(g_nSwitchLOC, nCmd, nArg, szLabel, false);
        nArg++;
        i++;
        if (nArg >= MAX_ARGS && i < g_nNumSwitchCases)
        {
            nCmd++;
            nArg = 0;
            if (nCmd >= MAX_COMMANDS_PER_LINE)
            {
                ReportCompilationError(
                    "**** CreateSwitchTable - SWITCH has too many CASEs for the compiler to handle", g_nSwitchLOC);
                return -1;
            }
            SetCommandAtLOC(g_nSwitchLOC, nCmd, COMMAND_SWITCH_CONTINUED, true);
            SetAlternativeIndexAtLOC(g_nSwitchLOC, nCmd, ALT_NONE, false);
        }
    }

    while (nArg < MAX_ARGS)
    {
        SetArgTypeAtLOC(g_nSwitchLOC, nCmd, nArg, CARG_INT, true);
        SetArgIntValueAtLOC(g_nSwitchLOC, nCmd, nArg, SWITCH_UNUSED_CASE_VALUE, false);
        nArg++;
        SetArgTypeAtLOC(g_nSwitchLOC, nCmd, nArg, CARG_NAME, true);
        SetArgStringAtLOC(g_nSwitchLOC, nCmd, nArg, szEndSwitchLabel, false);
        nArg++;
    }

    for (uint16_t i = 0; i < g_nNumSwitchBreaks; i++)
    {
        SetCommandAtLOC(g_aSwitchBreakLOCs[i], 0, COMMAND_GOTO, false);
        SetAlternativeIndexAtLOC(g_aSwitchBreakLOCs[i], 0, ALT_NONE, false);
        SetArgTypeAtLOC(g_aSwitchBreakLOCs[i], 0, 0, CARG_NAME, true);
        SetArgStringAtLOC(g_aSwitchBreakLOCs[i], 0, 0, szEndSwitchLabel, false);
    }
    return 0;
}

// 0x00403C30 ?IsGlobalVariable@@YA_NPAD@Z
bool IsGlobalVariable(const char* pszName)
{ return GetValueFromMap(pszName, &g_mapGlobalVariables, false) != -1; }

// 0x00403C50 ?GetGlobalVariableIndex@@YAFPAD@Z
int16_t GetGlobalVariableIndex(const char* pszName)
{
    int nIndex = GetValueFromMap(pszName, &g_mapGlobalVariables, false);
    if (nIndex > INT16_MAX || nIndex < INT16_MIN)
    {
        ReportCompilationError("**** ERROR Int32 -> Int16", -1);
    }
    return nIndex;
}

// 0x00403C90 ?GetLabelLOC@@YAHPAD@Z
int GetLabelLOC(const char* pszLabel)
{ return GetValueFromMap(pszLabel, &g_mapLabels, false); }

// 0x00403CB0 ?DeclareVariables@@YAHXZ
int DeclareVariables()
{
    int16_t nIndexGlobalVar = -1;
    int16_t nIndexLocalVar = -1;
    char szName[NAME_SIZE];

    for (uint32_t loc = 0; loc < g_nNumLines; loc++)
    {
        switch (GetCommandAtLOC(loc, 0, false))
        {
            case COMMAND_VAR_INT:
            case COMMAND_VAR_FLOAT:
            case COMMAND_SAVE_VAR_INT:
            case COMMAND_SAVE_VAR_FLOAT:
            case COMMAND_VAR_TEXT_LABEL:
            case COMMAND_VAR_TEXT_LABEL16:
                for (int16_t nArg = 0; nArg < MAX_ARGS; nArg++)
                {
                    if (GetArgStringAtLOC(loc, 0, nArg, false) == nullptr)
                    {
                        break;
                    }

                    char* pszArg = GetArgStringAtLOC(loc, 0, nArg, true);
                    if (strncmp(pszArg, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                    {
                        ReportCompilationError("**** VARIABLE NAME STARTS WITH SOUND_", loc);
                        return -1;
                    }

                    int16_t nArraySize = ParseVariableName(
                        GetArgStringAtLOC(loc, 0, nArg, true), szName, loc, &nIndexGlobalVar, &nIndexLocalVar);
                    if (nArraySize < 0)
                    {
                        return -1;
                    }

                    if (nIndexGlobalVar != -1 || nIndexLocalVar != -1)
                    {
                        ReportCompilationError(
                            "**** ARRAY CAN ONLY BE DECLARED WITH A NUMBER BETWEEN THE SQUARE BRACKETS", loc);
                        return -1;
                    }

                    if (nArraySize == 0)
                    {
                        nArraySize = 1;
                    }

                    if (IsGlobalVariable(szName) || IsLocalVariableNameInUse(szName))
                    {
                        ReportCompilationErrorWithName("**** VARIABLE NAME EXISTS ALREADY", szName, loc);
                        return -1;
                    }

                    int nIntValue;
                    float fFloatValue;
                    if (static_cast<int>(GetIntConstantValue(szName, &nIntValue)) == 1 ||
                        static_cast<int>(GetFloatConstantValue(szName, &fFloatValue)) == 1)
                    {
                        ReportCompilationErrorWithName(
                            "**** VARIABLE NAME EXISTS ALREADY AS A STRING CONSTANT", szName, loc);
                        return -1;
                    }

                    if (nArraySize > MAX_ARRAY_SIZE)
                    {
                        ReportCompilationError("**** ARRAY IS TOO BIG", loc);
                        return -1;
                    }

                    if (g_nNumGlobalVars >= MAX_GLOBAL_VARS)
                    {
                        ReportCompilationError("**** TOO MANY GLOBAL VARS", loc);
                        return -1;
                    }

                    strcpy(g_aGlobalVarNames[g_nNumGlobalVars], szName);
                    if (!AddToMap(&g_mapGlobalVariables, szName, g_nNumGlobalVars, true))
                    {
                        ReportCompilationError("**** ERROR WHEN ADDING TO THE MAP OF GLOBAL VARIABLES", loc);
                        return -1;
                    }

                    g_aGlobalVarArraySize[g_nNumGlobalVars] = nArraySize;
                    int16_t nFileIndex;
                    GetFileIndexAndLineFromLOC(loc, &nFileIndex);
                    g_aGlobalVarDeclaredInFile[g_nNumGlobalVars] = nFileIndex;

                    for (uint16_t i = 0; i < nArraySize; i++)
                    {
                        if (g_nNumGlobalVars >= MAX_GLOBAL_VARS)
                        {
                            ReportCompilationError("**** TOO MANY GLOBAL VARS", loc);
                            return -1;
                        }

                        g_aGlobalVarOffset[g_nNumGlobalVars] = g_nGlobalVarSpaceSize;

                        if (GetCommandAtLOC(loc, 0, true) == COMMAND_SAVE_VAR_INT ||
                            GetCommandAtLOC(loc, 0, true) == COMMAND_SAVE_VAR_FLOAT)
                        {
                            g_aSaveVarOffsets[g_nNumSaveVars++] = g_nGlobalVarSpaceSize;
                        }

                        if (GetCommandAtLOC(loc, 0, true) == COMMAND_VAR_INT ||
                            GetCommandAtLOC(loc, 0, true) == COMMAND_SAVE_VAR_INT)
                        {
                            g_aGlobalVarType[g_nNumGlobalVars++] = VAR_TYPE_INT;
                            g_nGlobalVarSpaceSize += VAR_SIZE;
                        }
                        else if (GetCommandAtLOC(loc, 0, true) == COMMAND_VAR_FLOAT ||
                                 GetCommandAtLOC(loc, 0, true) == COMMAND_SAVE_VAR_FLOAT)
                        {
                            g_aGlobalVarType[g_nNumGlobalVars++] = VAR_TYPE_FLOAT;
                            g_nGlobalVarSpaceSize += VAR_SIZE;
                        }
                        else if (GetCommandAtLOC(loc, 0, true) == COMMAND_VAR_TEXT_LABEL)
                        {
                            g_aGlobalVarType[g_nNumGlobalVars++] = VAR_TYPE_TEXT_LABEL;
                            g_nGlobalVarSpaceSize += TEXT_LABEL_SIZE;
                        }
                        else if (GetCommandAtLOC(loc, 0, true) == COMMAND_VAR_TEXT_LABEL16)
                        {
                            g_aGlobalVarType[g_nNumGlobalVars++] = VAR_TYPE_TEXT_LABEL16;
                            g_nGlobalVarSpaceSize += TEXT_LABEL16_SIZE;
                        }
                    }
                }
                break;

            case COMMAND_LVAR_INT:
            case COMMAND_LVAR_FLOAT:
            case COMMAND_LVAR_TEXT_LABEL:
            case COMMAND_LVAR_TEXT_LABEL16:
                if (NumberOfMissionScripts > 0 && loc >= g_aMissionScripts[0].nStartLOC &&
                    (g_nNumStreamedScripts == 0 || loc < g_aStreamedScripts[0].nStartLOC))
                {
                    LocalVariables::SetMissionScope(g_aScopeOfLine[loc]);
                }

                for (int16_t nArg = 0; nArg < MAX_ARGS; nArg++)
                {
                    if (GetArgStringAtLOC(loc, 0, nArg, false) == nullptr)
                    {
                        break;
                    }

                    char* pszArg = GetArgStringAtLOC(loc, 0, nArg, true);
                    if (strncmp(pszArg, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                    {
                        ReportCompilationError("**** VARIABLE NAME STARTS WITH SOUND_", loc);
                        return -1;
                    }

                    if (g_aScopeOfLine[loc] < 0)
                    {
                        ReportCompilationError("**** LOCAL VARS ARE NOT ALLOWED OUTSIDE OF SCOPE BLOCK", loc);
                        return -1;
                    }

                    int16_t nArraySize = ParseVariableName(
                        GetArgStringAtLOC(loc, 0, nArg, true), szName, loc, &nIndexGlobalVar, &nIndexLocalVar);
                    if (nArraySize < 0)
                    {
                        return -1;
                    }

                    if (nIndexGlobalVar != -1 || nIndexLocalVar != -1)
                    {
                        ReportCompilationError(
                            "**** ARRAY CAN ONLY BE DECLARED WITH A NUMBER BETWEEN THE SQUARE BRACKETS", loc);
                        return -1;
                    }

                    if (nArraySize == 0)
                    {
                        nArraySize = 1;
                    }

                    if (IsGlobalVariable(szName))
                    {
                        ReportCompilationError("**** CANNOT HAVE GLOBALS AND LOCALS WITH THE SAME NAME", loc);
                        return -1;
                    }

                    if (DoesLocalVariableExistInScope(szName, g_aScopeOfLine[loc]))
                    {
                        ReportCompilationErrorWithName("**** VARIABLE ALREADY EXISTS IN THIS BLOCK (1)", szName, loc);
                        return -1;
                    }

                    int nIntValue;
                    float fFloatValue;
                    if (static_cast<int>(GetIntConstantValue(szName, &nIntValue)) == 1 ||
                        static_cast<int>(GetFloatConstantValue(szName, &fFloatValue)) == 1)
                    {
                        ReportCompilationErrorWithName(
                            "**** VARIABLE NAME EXISTS ALREADY AS A STRING CONSTANT", szName, loc);
                        return -1;
                    }

                    if (nArraySize > MAX_ARRAY_SIZE)
                    {
                        ReportCompilationError("**** ARRAY IS TOO BIG", loc);
                        return -1;
                    }

                    for (uint16_t i = 0; i < nArraySize; i++)
                    {
                        int16_t nNewVar;
                        int16_t nDummy;

                        LocalVariables::AddLocalVar(g_aScopeOfLine[loc], &nNewVar);
                        if (i == 0)
                        {
                            if (!LocalVariables::SetName(g_aScopeOfLine[loc], nNewVar, szName))
                            {
                                ReportCompilationError("**** ERROR WHEN SETTING THE NAME OF A LOCAL VARIABLE", loc);
                                return -1;
                            }
                            LocalVariables::SetArraySize(g_aScopeOfLine[loc], nNewVar, nArraySize);
                        }

                        if (GetCommandAtLOC(loc, 0, true) == COMMAND_LVAR_INT)
                        {
                            LocalVariables::SetType(g_aScopeOfLine[loc], nNewVar, VAR_TYPE_INT);
                        }
                        else if (GetCommandAtLOC(loc, 0, true) == COMMAND_LVAR_FLOAT)
                        {
                            LocalVariables::SetType(g_aScopeOfLine[loc], nNewVar, VAR_TYPE_FLOAT);
                        }
                        else if (GetCommandAtLOC(loc, 0, true) == COMMAND_LVAR_TEXT_LABEL)
                        {
                            LocalVariables::SetType(g_aScopeOfLine[loc], nNewVar, VAR_TYPE_TEXT_LABEL);
                            LocalVariables::AddLocalVar(g_aScopeOfLine[loc], &nDummy);
                        }
                        else if (GetCommandAtLOC(loc, 0, true) == COMMAND_LVAR_TEXT_LABEL16)
                        {
                            LocalVariables::SetType(g_aScopeOfLine[loc], nNewVar, VAR_TYPE_TEXT_LABEL16);
                            LocalVariables::AddLocalVar(g_aScopeOfLine[loc], &nDummy);
                            LocalVariables::AddLocalVar(g_aScopeOfLine[loc], &nDummy);
                            LocalVariables::AddLocalVar(g_aScopeOfLine[loc], &nDummy);
                        }
                    }
                }
                break;

            case COMMAND_CREATE_COLLECTABLE1:
                g_nTotalCollectables++;
                break;

            case COMMAND_PLAYER_MADE_PROGRESS:
                if (GetArgIntValueAtLOC(loc, 0, 0, true) != 1)
                {
                    ReportCompilationError("**** PLAYER_MADE_PROGRESS should be followed by 1", loc);
                    return -1;
                }
                g_nTotalProgress += GetArgIntValueAtLOC(loc, 0, 0, true);
                break;

            case COMMAND_REGISTER_MISSION_GIVEN:
                g_nTotalMissionsGiven++;
                break;

            case COMMAND_REGISTER_MISSION_PASSED:
            case COMMAND_REGISTER_ODDJOB_MISSION_PASSED:
                g_nTotalMissionsPassed++;
                break;

            case COMMAND_ADD_INT_TO_VAR_CONSOLE:
                if (AddGlobalVariableToVarConsole(loc, false) < 0)
                {
                    ReportCompilationError("**** ERROR IN ADD_INT_TO_VAR_CONSOLE", loc);
                    return -1;
                }
                break;

            case COMMAND_ADD_FLOAT_TO_VAR_CONSOLE:
                if (AddGlobalVariableToVarConsole(loc, true) < 0)
                {
                    ReportCompilationError("**** ERROR IN ADD_FLOAT_TO_VAR_CONSOLE", loc);
                    return -1;
                }
                break;

            case COMMAND_CONST_INT:
                ReportCompilationError("**** CONST_INT - shouldn't get here", loc);
                return -1;

            case COMMAND_AWARD_PLAYER_MISSION_RESPECT:
                g_nTotalMissionRespect += GetArgIntValueAtLOC(loc, 0, 0, true);
                break;
        }

        switch (GetCommandAtLOC(loc, 0, false))
        {
            case COMMAND_VAR_INT:
            case COMMAND_VAR_FLOAT:
            case COMMAND_LVAR_INT:
            case COMMAND_LVAR_FLOAT:
            case COMMAND_SAVE_VAR_INT:
            case COMMAND_SAVE_VAR_FLOAT:
            case COMMAND_VAR_TEXT_LABEL:
            case COMMAND_LVAR_TEXT_LABEL:
            case COMMAND_VAR_TEXT_LABEL16:
            case COMMAND_LVAR_TEXT_LABEL16:
            case COMMAND_ADD_INT_TO_VAR_CONSOLE:
            case COMMAND_ADD_FLOAT_TO_VAR_CONSOLE:
                SetCommandAtLOC(loc, 0, COMMAND_NOP, false);
                break;
        }
    }
    return 0;
}

#define STRING_ARG_BUFFER_SIZE 128

// 0x00404720 ?HandleStringArgument@@YAHHGG@Z
int HandleStringArgument(int loc, uint16_t nCmd, uint16_t nArg)
{
    char szString[STRING_ARG_BUFFER_SIZE];

    if (GetArgTypeAtLOC(loc, nCmd, nArg, true) != CARG_NAME && GetArgTypeAtLOC(loc, nCmd, nArg, true) != CARG_INT)
    {
        ReportCompilationError("**** Expected string identifier here", loc);
        return -1;
    }

    uint16_t nLen = strlen(GetArgStringAtLOC(loc, nCmd, nArg, true));
    if (nLen > ARRAY_SIZE(szString) - 1)
    {
        ReportCompilationError("**** String is too long (only 127 chars please)", loc);
        return -1;
    }

    strcpy(szString, GetArgStringAtLOC(loc, nCmd, nArg, true));
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (i + 1 < nLen)
        {
            szString[i] = szString[i + 1];
        }
        else
        {
            szString[i] = 0;
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szString[i] == STRING_PLACEHOLDER_SPACE)
        {
            szString[i] = ' ';
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szString[i] == STRING_PLACEHOLDER_SLASH)
        {
            szString[i] = '/';
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szString[i] == STRING_PLACEHOLDER_COMMA)
        {
            szString[i] = ',';
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szString[i] == STRING_PLACEHOLDER_OPEN_BRACKET)
        {
            szString[i] = '(';
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szString[i] == STRING_PLACEHOLDER_CLOSE_BRACKET)
        {
            szString[i] = ')';
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szString[i] == STRING_PLACEHOLDER_TAB)
        {
            szString[i] = '\t';
        }
    }

    for (uint16_t i = 0; i < CODE_SIZE_STRING / CODE_SIZE_INT32; i++)
    {
        WriteInt32((reinterpret_cast<uint32_t*>(szString))[i]);
    }
    g_nStatStringsWritten++;
    fprintf(pMainText, "%s ", szString);
    return 0;
}

// 0x00404930 ?HandleArrayArgument@@YAHHGFDH@Z
int HandleArrayArgument(int loc, uint16_t nCmd, int16_t nArg, int8_t nVarKind, int nScriptLabelLOC)
{
    char szVarName[NAME_SIZE];
    int16_t nIndexGlobalVar = -1;
    int16_t nIndexLocalVar = -1;
    int16_t nType;
    uint16_t nArraySize;
    int nVar;

    eArgType nArgType = g_aCommandInfo[GetCommandAtLOC(loc, nCmd, true)].aArgTypes[nArg];
    int16_t nArrayIndex =
        ParseVariableName(GetArgStringAtLOC(loc, nCmd, nArg, true), szVarName, loc, &nIndexGlobalVar, &nIndexLocalVar);
    if (nArrayIndex < 0)
    {
        ReportCompilationError("**** HandleArrayArgument - ARRAY error", loc);
        return -1;
    }

    if (nVarKind == VAR_KIND_GLOBAL)
    {
        nVar = GetGlobalVariableIndex(szVarName);
        if (nVar < 0)
        {
            ReportCompilationError("**** THIS GLOBAL VARIABLE IS NOT DEFINED (2)", loc);
            return -1;
        }
        nType = g_aGlobalVarType[nVar];
        nArraySize = g_aGlobalVarArraySize[nVar];
        if ((nType == VAR_TYPE_INT && nArgType != ARG_VAR_INT) ||
            (nType == VAR_TYPE_FLOAT && nArgType != ARG_VAR_FLOAT) ||
            (nType == VAR_TYPE_TEXT_LABEL && nArgType != ARG_VAR_TEXT_LABEL) ||
            (nType == VAR_TYPE_TEXT_LABEL16 && nArgType != ARG_VAR_TEXT_LABEL16))
        {
            ReportCompilationError("**** ARGUMENT TYPE MISMATCH - E.G. DECLARED AS VAR_INT USED AS FLOAT", loc);
            return -1;
        }
    }
    else if (nVarKind == VAR_KIND_LOCAL)
    {
        nVar = GetLocalVariableIndex(szVarName, loc);
        if (nVar < 0)
        {
            ReportCompilationError("**** THIS VARIABLE IS NOT DEFINED IN THIS BLOCK (2)", loc);
            return -1;
        }
        nArraySize = LocalVariables::GetArraySize(g_aScopeOfLine[loc], nVar);
        nType = LocalVariables::GetType(g_aScopeOfLine[loc], nVar);
        if ((nType == VAR_TYPE_INT && nArgType != ARG_LVAR_INT) ||
            (nType == VAR_TYPE_FLOAT && nArgType != ARG_LVAR_FLOAT) ||
            (nType == VAR_TYPE_TEXT_LABEL && nArgType != ARG_LVAR_TEXT_LABEL) ||
            (nType == VAR_TYPE_TEXT_LABEL16 && nArgType != ARG_LVAR_TEXT_LABEL16))
        {
            ReportCompilationError("**** ARGUMENT TYPE MISMATCH - E.G. DECLARED AS LVAR_INT USED AS FLOAT", loc);
            return -1;
        }
    }
    else if (nVarKind == VAR_KIND_SCRIPT_PARAM)
    {
        nVar = GetGlobalVariableIndex(szVarName);
        if (nVar >= 0)
        {
            nArraySize = g_aGlobalVarArraySize[nVar];
            nType = g_aGlobalVarType[nVar];
            if (LocalVariables::GetType(g_aScopeOfLine[nScriptLabelLOC], nArg - 1) != nType)
            {
                ReportCompilationError(
                    "**** ARGUMENT TYPE MISMATCH (NAME-GLOBAL)  - E.G. DECLARED AS VAR_INT USED AS FLOAT", loc);
                return -1;
            }
            nVarKind = VAR_KIND_GLOBAL;
        }
        else
        {
            nVar = GetLocalVariableIndex(szVarName, loc);
            if (nVar >= 0)
            {
                nArraySize = LocalVariables::GetArraySize(g_aScopeOfLine[loc], nVar);
                nType = LocalVariables::GetType(g_aScopeOfLine[loc], nVar);
                if (LocalVariables::GetType(g_aScopeOfLine[nScriptLabelLOC], nArg - 1) != nType)
                {
                    ReportCompilationError(
                        "**** ARGUMENT TYPE MISMATCH (NAME-LOCAL)  - E.G. DECLARED AS LVAR_INT USED AS FLOAT", loc);
                    return -1;
                }
                nVarKind = VAR_KIND_LOCAL;
            }
            else
            {
                ReportCompilationError("**** NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", loc);
                return -1;
            }
        }
    }
    else if (nVarKind == VAR_KIND_ANY)
    {
        nVar = GetGlobalVariableIndex(szVarName);
        if (nVar >= 0)
        {
            nType = g_aGlobalVarType[nVar];
            nArraySize = g_aGlobalVarArraySize[nVar];
            if ((nType == VAR_TYPE_INT && nArgType != ARG_INT && nArgType != ARG_RETURN_INT) ||
                (nType == VAR_TYPE_FLOAT && nArgType != ARG_FLOAT && nArgType != ARG_RETURN_FLOAT) ||
                (nType == VAR_TYPE_TEXT_LABEL && nArgType != ARG_VAR_TEXT_LABEL) ||
                (nType == VAR_TYPE_TEXT_LABEL16 && nArgType != ARG_VAR_TEXT_LABEL16))
            {
                ReportCompilationError("**** ARGUMENT TYPE MISMATCH - E.G. DECLARED AS VAR_INT USED AS FLOAT", loc);
                return -1;
            }
            nVarKind = VAR_KIND_GLOBAL;
        }
        else
        {
            nVar = GetLocalVariableIndex(szVarName, loc);
            if (nVar >= 0)
            {
                nArraySize = LocalVariables::GetArraySize(g_aScopeOfLine[loc], nVar);
                nType = LocalVariables::GetType(g_aScopeOfLine[loc], nVar);
                if ((nType == VAR_TYPE_INT && nArgType != ARG_INT && nArgType != ARG_RETURN_INT) ||
                    (nType == VAR_TYPE_FLOAT && nArgType != ARG_FLOAT && nArgType != ARG_RETURN_FLOAT) ||
                    (nType == VAR_TYPE_TEXT_LABEL && nArgType != ARG_LVAR_TEXT_LABEL) ||
                    (nType == VAR_TYPE_TEXT_LABEL16 && nArgType != ARG_LVAR_TEXT_LABEL16))
                {
                    ReportCompilationError(
                        "**** ARGUMENT TYPE MISMATCH - E.G. DECLARED AS LVAR_INT USED AS FLOAT", loc);
                    return -1;
                }
                nVarKind = VAR_KIND_LOCAL;
            }
            else
            {
                ReportCompilationError("**** NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", loc);
                return -1;
            }
        }
    }
    else
    {
        ReportCompilationError("**** HandleArrayArgument - Graeme has made a mistake", loc);
        return -1;
    }

    if (nIndexGlobalVar < 0 && nIndexLocalVar < 0)
    {
        if (nArrayIndex != 0 && nArrayIndex >= nArraySize)
        {
            ReportCompilationError("Array Index is greater than or equal to the size of the array", loc);
            return -1;
        }
        if (nVarKind == VAR_KIND_GLOBAL)
        {
            nVar += nArrayIndex;
            if (static_cast<uint32_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START) >= UINT16_MAX)
            {
                ReportCompilationError("Global Variable Index too big for UInt16", loc);
                return -1;
            }
            if (nType == VAR_TYPE_TEXT_LABEL)
            {
                WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL, true);
            }
            else if (nType == VAR_TYPE_TEXT_LABEL16)
            {
                WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL16, true);
            }
            else
            {
                WriteArgumentDataType(DATA_TYPE_GLOBAL_VAR, true);
            }
            AddToArrayAccessMap(VAR_KIND_GLOBAL, nType, nVar, g_nCodeOffset, szVarName);
            WriteVariableIndex(static_cast<uint16_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START));
            fprintf(pMainText, "%s[%d] ", GetArgStringAtLOC(loc, nCmd, nArg, true),
                g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START);
        }
        else if (nVarKind == VAR_KIND_LOCAL)
        {
            int nOffset = nVar;
            if (nType == VAR_TYPE_TEXT_LABEL)
            {
                WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL, true);
                nOffset += nArrayIndex * VARS_PER_TEXT_LABEL;
            }
            else if (nType == VAR_TYPE_TEXT_LABEL16)
            {
                WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL16, true);
                nOffset += nArrayIndex * VARS_PER_TEXT_LABEL16;
            }
            else
            {
                WriteArgumentDataType(DATA_TYPE_LOCAL_VAR, true);
                nOffset += nArrayIndex;
            }
            if (nOffset >= UINT16_MAX)
            {
                ReportCompilationError("Local Variable Index too big for UInt16", loc);
                return -1;
            }
            AddToArrayAccessMap(VAR_KIND_LOCAL, nType, nOffset, g_nCodeOffset, szVarName);
            WriteVariableIndex(nOffset);
            fprintf(pMainText, "%s(%d) ", GetArgStringAtLOC(loc, nCmd, nArg, true), nOffset);
        }
    }
    else
    {
        uint16_t nIndexOffset;
        uint16_t nFlags;
        if (nIndexGlobalVar < 0)
        {
            nFlags = 0;
            nIndexOffset = nIndexLocalVar;
        }
        else
        {
            nFlags = ARRAY_INDEX_GLOBAL_VAR;
            if (static_cast<uint32_t>(g_aGlobalVarOffset[nIndexGlobalVar] + GLOBAL_VARS_START) >= UINT16_MAX)
            {
                ReportCompilationError("Global Variable Index for Array is too big for UInt16", loc);
                return -1;
            }
            nIndexOffset = g_aGlobalVarOffset[nIndexGlobalVar] + GLOBAL_VARS_START;
        }
        nFlags |= nArraySize | (nType << ARRAY_ELEMENT_TYPE_SHIFT);

        if (nVarKind == VAR_KIND_GLOBAL)
        {
            if (static_cast<uint32_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START) >= UINT16_MAX)
            {
                ReportCompilationError("Base address of Global Array is too big for UInt16", loc);
                return -1;
            }
            if (nType == VAR_TYPE_TEXT_LABEL)
            {
                WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL_ARRAY, true);
            }
            else if (nType == VAR_TYPE_TEXT_LABEL16)
            {
                WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL16_ARRAY, true);
            }
            else
            {
                WriteArgumentDataType(DATA_TYPE_GLOBAL_ARRAY, true);
            }
            AddToArrayAccessMap(VAR_KIND_GLOBAL, nType, nVar, g_nCodeOffset, szVarName);
            WriteVariableIndex(static_cast<uint16_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START));
            fprintf(pMainText, "%s[%d] ", GetArgStringAtLOC(loc, nCmd, nArg, true),
                g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START);
        }
        else if (nVarKind == VAR_KIND_LOCAL)
        {
            if (nType == VAR_TYPE_TEXT_LABEL)
            {
                WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL_ARRAY, true);
            }
            else if (nType == VAR_TYPE_TEXT_LABEL16)
            {
                WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL16_ARRAY, true);
            }
            else
            {
                WriteArgumentDataType(DATA_TYPE_LOCAL_ARRAY, true);
            }
            AddToArrayAccessMap(VAR_KIND_LOCAL, nType, nVar, g_nCodeOffset, szVarName);
            WriteVariableIndex(nVar);
            fprintf(pMainText, "%s(%d) ", GetArgStringAtLOC(loc, nCmd, nArg, true), nVar);
        }
        WriteVariableIndex(nIndexOffset);
        WriteVariableIndex(nFlags);
    }
    return 0;
}

// 0x00405050 ?HandleTextStringArgument@@YAHHGF@Z
int HandleTextStringArgument(int loc, uint16_t nCmd, int16_t nArg)
{
    char szName[NAME_SIZE];
    char szText[NAME_SIZE];
    char szVarName[NAME_SIZE];
    int16_t nIndexGlobalVar = -1;
    int16_t nIndexLocalVar = -1;

    if (GetArgTypeAtLOC(loc, nCmd, nArg, true) == CARG_NAME)
    {
        strncpy(szName, GetArgStringAtLOC(loc, nCmd, nArg, true), NAME_SIZE);
        switch (GetCommandAtLOC(loc, nCmd, false))
        {
            case COMMAND_LOAD_SPECIAL_CHARACTER:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_SPECIAL_CHARACTERS], szName);
                }
                break;
            case COMMAND_SET_ANIM_GROUP_FOR_CHAR:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_LOAD_CUTSCENE:
                if (nArg == 0)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_CUTSCENES], szName);
                }
                break;
            case COMMAND_SET_CUTSCENE_ANIM:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_SET_CHAR_ANIM_SPEED:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_SET_CUTSCENE_ANIM_TO_LOOP:
                if (nArg == 0)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_REQUEST_ANIMATION:
                if (nArg == 0)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_HAS_ANIMATION_LOADED:
                if (nArg == 0)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_REMOVE_ANIMATION:
                if (nArg == 0)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_LOAD_UNCOMPRESSED_ANIM:
                if (nArg == 0)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
            case COMMAND_CREATE_FX_SYSTEM_ON_CHAR:
            case COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION:
            case COMMAND_CREATE_FX_SYSTEM_ON_CAR:
            case COMMAND_CREATE_FX_SYSTEM_ON_CAR_WITH_DIRECTION:
            case COMMAND_CREATE_FX_SYSTEM_ON_OBJECT:
            case COMMAND_CREATE_FX_SYSTEM_ON_OBJECT_WITH_DIRECTION:
                AddToSet(&g_aResourcesUsedByScript[RESOURCE_PARTICLES], szName);
                break;
            case COMMAND_TASK_PLAY_ANIM:
            case COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE:
            case COMMAND_TASK_DIE_NAMED_ANIM:
            case COMMAND_TASK_PLAY_ANIM_WITH_FLAGS:
            case COMMAND_TASK_PLAY_ANIM_SECONDARY:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                else if (nArg == 2)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_IS_CHAR_PLAYING_ANIM:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_SET_CHAR_ANIM_PLAYING_FLAG:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_GET_CHAR_ANIM_CURRENT_TIME:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_SET_CHAR_ANIM_CURRENT_TIME:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_GET_CHAR_ANIM_TOTAL_TIME:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_CREATE_FX_SYSTEM:
                AddToSet(&g_aResourcesUsedByScript[RESOURCE_PARTICLES], szName);
                break;
            case COMMAND_TASK_PICK_UP_OBJECT:
                if (nArg == 7)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                else if (nArg == 8)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_EXTEND_PATROL_ROUTE:
                if (nArg == 3)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                else if (nArg == 4)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_PLAY_OBJECT_ANIM:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                else if (nArg == 2)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM:
                if (nArg == 6)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                else if (nArg == 7)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_SET_OBJECT_ANIM_SPEED:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_IS_OBJECT_PLAYING_ANIM:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_SET_OBJECT_ANIM_PLAYING_FLAG:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_GET_OBJECT_ANIM_CURRENT_TIME:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_SET_OBJECT_ANIM_CURRENT_TIME:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_GET_OBJECT_ANIM_TOTAL_TIME:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                break;
            case COMMAND_ATTACH_ANIMS_TO_MODEL:
                if (nArg == 1)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
            case COMMAND_TASK_PICK_UP_SECOND_OBJECT:
                if (nArg == 7)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIMS], szName);
                }
                else if (nArg == 8)
                {
                    AddToSet(&g_aResourcesUsedByScript[RESOURCE_ANIM_DICTIONARIES], szName);
                }
                break;
        }

        if (szName[0] == '$')
        {
            int16_t nArrayIndex = ParseVariableName(
                GetArgStringAtLOC(loc, nCmd, nArg, true), szVarName, loc, &nIndexGlobalVar, &nIndexLocalVar);
            if (nArrayIndex < 0)
            {
                ReportCompilationError("**** HandleArrayArgument - ARRAY error", loc);
                return -1;
            }

            int16_t nType;
            uint16_t nArraySize;
            int8_t nVarKind;
            int nVar = GetGlobalVariableIndex(szVarName);
            if (nVar >= 0)
            {
                nType = g_aGlobalVarType[nVar];
                nArraySize = g_aGlobalVarArraySize[nVar];
                if (nType != VAR_TYPE_TEXT_LABEL && nType != VAR_TYPE_TEXT_LABEL16)
                {
                    ReportCompilationError(
                        "**** HandleTextStringArgument - ARGUMENT TYPE MISMATCH - VARIABLE SHOULD BE "
                        "DECLARED AS VAR_TEXT_LABEL OR VAR_TEXT_LABEL16",
                        loc);
                    return -1;
                }
                nVarKind = VAR_KIND_GLOBAL;
            }
            else
            {
                nVar = GetLocalVariableIndex(szVarName, loc);
                if (nVar >= 0)
                {
                    nArraySize = LocalVariables::GetArraySize(g_aScopeOfLine[loc], nVar);
                    nType = LocalVariables::GetType(g_aScopeOfLine[loc], nVar);
                    if (nType != VAR_TYPE_TEXT_LABEL && nType != VAR_TYPE_TEXT_LABEL16)
                    {
                        ReportCompilationError(
                            "**** HandleTextStringArgument - ARGUMENT TYPE MISMATCH - VARIABLE SHOULD "
                            "BE DECLARED AS LVAR_TEXT_LABEL OR LVAR_TEXT_LABEL16",
                            loc);
                        return -1;
                    }
                    nVarKind = VAR_KIND_LOCAL;
                }
                else
                {
                    ReportCompilationError("**** NON EXISTING VARIABLE (IN THIS SCOPE ANYWAY)", loc);
                    return -1;
                }
            }

            if (nIndexGlobalVar < 0 && nIndexLocalVar < 0)
            {
                if (nArrayIndex != 0 && nArrayIndex >= nArraySize)
                {
                    ReportCompilationError("Array Index is greater than or equal to the size of the array", loc);
                    return -1;
                }
                if (nVarKind == VAR_KIND_GLOBAL)
                {
                    if (static_cast<uint32_t>(g_aGlobalVarOffset[nVar + nArrayIndex] + GLOBAL_VARS_START) >= UINT16_MAX)
                    {
                        ReportCompilationError("Global Variable Index too big for UInt16", loc);
                        return -1;
                    }
                    if (nType == VAR_TYPE_TEXT_LABEL)
                    {
                        WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL, true);
                    }
                    else if (nType == VAR_TYPE_TEXT_LABEL16)
                    {
                        WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL16, true);
                    }
                    WriteVariableIndex(
                        static_cast<uint16_t>(g_aGlobalVarOffset[nVar + nArrayIndex] + GLOBAL_VARS_START));
                    fprintf(pMainText, "%s[%d] ", GetArgStringAtLOC(loc, nCmd, nArg, true),
                        g_aGlobalVarOffset[nVar + nArrayIndex] + GLOBAL_VARS_START);
                }
                else if (nVarKind == VAR_KIND_LOCAL)
                {
                    int nOffset = nVar;
                    if (nType == VAR_TYPE_TEXT_LABEL)
                    {
                        WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL, true);
                        nOffset += nArrayIndex * VARS_PER_TEXT_LABEL;
                    }
                    else if (nType == VAR_TYPE_TEXT_LABEL16)
                    {
                        WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL16, true);
                        nOffset += nArrayIndex * VARS_PER_TEXT_LABEL16;
                    }
                    if (nOffset >= UINT16_MAX)
                    {
                        ReportCompilationError("Local Variable Index too big for UInt16", loc);
                        return -1;
                    }
                    WriteVariableIndex(nOffset);
                    fprintf(pMainText, "%s(%d) ", GetArgStringAtLOC(loc, nCmd, nArg, true), nOffset);
                }
            }
            else
            {
                uint16_t nIndexOffset;
                uint16_t nFlags;
                if (nIndexGlobalVar < 0)
                {
                    nFlags = 0;
                    nIndexOffset = nIndexLocalVar;
                }
                else
                {
                    nFlags = ARRAY_INDEX_GLOBAL_VAR;
                    if (static_cast<uint32_t>(g_aGlobalVarOffset[nIndexGlobalVar] + GLOBAL_VARS_START) >= UINT16_MAX)
                    {
                        ReportCompilationError("Global Variable Index for Array is too big for UInt16", loc);
                        return -1;
                    }
                    nIndexOffset = g_aGlobalVarOffset[nIndexGlobalVar] + GLOBAL_VARS_START;
                }
                nFlags |= nArraySize | (nType << ARRAY_ELEMENT_TYPE_SHIFT);

                if (nVarKind == VAR_KIND_GLOBAL)
                {
                    if (static_cast<uint32_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START) >= UINT16_MAX)
                    {
                        ReportCompilationError("Base address of Global Array is too big for UInt16", loc);
                        return -1;
                    }
                    if (nType == VAR_TYPE_TEXT_LABEL)
                    {
                        WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL_ARRAY, true);
                    }
                    else if (nType == VAR_TYPE_TEXT_LABEL16)
                    {
                        WriteArgumentDataType(DATA_TYPE_GLOBAL_TEXT_LABEL16_ARRAY, true);
                    }
                    WriteVariableIndex(static_cast<uint16_t>(g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START));
                    fprintf(pMainText, "%s[%d] ", GetArgStringAtLOC(loc, nCmd, nArg, true),
                        g_aGlobalVarOffset[nVar] + GLOBAL_VARS_START);
                }
                else if (nVarKind == VAR_KIND_LOCAL)
                {
                    if (nType == VAR_TYPE_TEXT_LABEL)
                    {
                        WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL_ARRAY, true);
                    }
                    else if (nType == VAR_TYPE_TEXT_LABEL16)
                    {
                        WriteArgumentDataType(DATA_TYPE_LOCAL_TEXT_LABEL16_ARRAY, true);
                    }
                    WriteVariableIndex(nVar);
                    fprintf(pMainText, "%s(%d) ", GetArgStringAtLOC(loc, nCmd, nArg, true), nVar);
                }
                WriteVariableIndex(nIndexOffset);
                WriteVariableIndex(nFlags);
                return 0;
            }
        }
        else
        {
            char* pszString = GetArgStringAtLOC(loc, nCmd, nArg, true);
            fprintf(pMainText, "%s ", pszString);
            if (pszString[0] == '&')
            {
                if (static_cast<uint16_t>(strlen(pszString)) == 1)
                {
                    ReportCompilationError("**** String has nothing after &", loc);
                    return -1;
                }
                SetArgStringAtLOC(loc, nCmd, nArg, pszString + 1, false);
            }

            uint16_t nLen = strlen(GetArgStringAtLOC(loc, nCmd, nArg, true));
            strcpy(szText, GetArgStringAtLOC(loc, nCmd, nArg, true));
            if (g_aCommandInfo[GetCommandAtLOC(loc, nCmd, true)].aArgTypes[nArg] == ARG_TEXT_LABEL)
            {
                if (nLen > TEXT_LABEL_SIZE - 1)
                {
                    ReportCompilationError("**** String identifier is too long (only 7 chars please)", loc);
                    return -1;
                }
                WriteArgumentDataType(DATA_TYPE_TEXT_LABEL, true);
                WriteInt32((reinterpret_cast<uint32_t*>(szText))[0]);
                WriteInt32((reinterpret_cast<uint32_t*>(szText))[1]);
                g_nStatTextLabelsWritten++;
                g_DisplayedTextKeys.AddToListOfDisplayedTextKeys2(loc, nCmd, nArg);
                return 0;
            }

            if (nLen > ARRAY_SIZE(szText))
            {
                ReportCompilationError("**** Long String is too long", loc);
                return -1;
            }
            WriteArgumentDataType(DATA_TYPE_STRING, true);
            int nOffset = g_nCodeOffset;
            g_pCompiledCode[nOffset] = nLen;
            g_nStatInt8sWritten++;
            g_nCodeOffset++;
            for (uint16_t i = 0; i < nLen; i++)
            {
                g_pCompiledCode[g_nCodeOffset] = szText[i];
                g_nStatInt8sWritten++;
                g_nCodeOffset++;
            }
            g_nStatTextLabelsWritten++;
        }
        return 0;
    }
    else
    {
        ReportCompilationError("**** Expected string identifier here", loc);
        return -1;
    }
}

// 0x00405A10 ?InitVarConsole@@YAHXZ
int InitVarConsole()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aVarConsole); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aVarConsole[i].szName); j++)
        {
            g_aVarConsole[i].szName[j] = 0;
        }
        g_aVarConsole[i].nArrayIndex = 0;
        g_aVarConsole[i].nOffset = 0;
        g_aVarConsole[i].bIsFloat = false;
    }
    g_nNumVarConsoleEntries = 0;
    return 0;
}

// Where ParseVariableName is in "NAME[INDEX]"
enum eParseVariableNameState
{
    PARSE_NAME = 0,
    PARSE_OPEN_BRACKET,
    PARSE_INDEX,
    PARSE_CLOSE_BRACKET,
};

// 0x00405A60 ?ParseVariableName@@YAFPAD0HPAF1@Z
int16_t ParseVariableName(
    const char* pszString, char* pszNameOut, int loc, int16_t* pnIndexGlobalVar, int16_t* pnIndexLocalVar)
{
    char szIndex[NAME_SIZE];
    for (uint16_t j = 0; j < ARRAY_SIZE(szIndex); j++)
    {
        szIndex[j] = 0;
    }
    uint16_t nIndexLen = 0;

    if (pszNameOut != nullptr)
    {
        strcpy(pszNameOut, pszString);
    }
    if (pnIndexGlobalVar != nullptr)
    {
        *pnIndexGlobalVar = -1;
    }
    if (pnIndexLocalVar != nullptr)
    {
        *pnIndexLocalVar = -1;
    }

    int8_t nState = PARSE_NAME;
    uint16_t nLen = strlen(pszString);
    int16_t i = 0;
    int nArrayIndex = 0;
    while (i < nLen)
    {
        if (pszString[i] == ']')
        {
            ReportCompilationError("**** UNMATCHED ] IN ARRAY DECLARATION", loc);
            return -1;
        }
        if (pszString[i] == '[')
        {
            if (nState != PARSE_NAME)
            {
                ReportCompilationError("**** MORE THAN ONE [ IN ARRAY DECLARATION", loc);
                return -1;
            }
            nState = PARSE_OPEN_BRACKET;
            if (pszNameOut != nullptr)
            {
                pszNameOut[i] = 0;
            }
            i++;
            while (i < nLen && nState != PARSE_CLOSE_BRACKET)
            {
                if (pszString[i] == ']')
                {
                    if (nState != PARSE_INDEX)
                    {
                        ReportCompilationError("**** NO NUMBER BETWEEN SQUARE BRACKETS IN ARRAY DECLARATION", loc);
                        return -1;
                    }
                    nState = PARSE_CLOSE_BRACKET;
                    szIndex[nIndexLen] = 0;
                    if ((pnIndexGlobalVar != nullptr) || (pnIndexLocalVar != nullptr))
                    {
                        if (pnIndexGlobalVar != nullptr)
                        {
                            *pnIndexGlobalVar = GetGlobalVariableIndex(szIndex);
                            if (*pnIndexGlobalVar >= 0)
                            {
                                if (g_aGlobalVarType[*pnIndexGlobalVar] != VAR_TYPE_INT)
                                {
                                    ReportCompilationError(
                                        "**** VARIABLE BETWEEN SQUARE BRACKETS IS NOT A VAR_INT", loc);
                                    return -1;
                                }
                                nArrayIndex = 1;
                                SetVariableReadWriteFlags(loc, *pnIndexGlobalVar, true, true, false);
                                continue;
                            }
                        }
                        if (pnIndexLocalVar != nullptr)
                        {
                            *pnIndexLocalVar = GetLocalVariableIndex(szIndex, loc);
                            if (*pnIndexLocalVar >= 0)
                            {
                                if (LocalVariables::GetType(g_aScopeOfLine[loc], *pnIndexLocalVar) != VAR_TYPE_INT)
                                {
                                    ReportCompilationError(
                                        "**** VARIABLE BETWEEN SQUARE BRACKETS IS NOT A VAR_INT", loc);
                                    return -1;
                                }
                                nArrayIndex = 1;
                                SetVariableReadWriteFlags(loc, *pnIndexLocalVar, false, true, false);
                                continue;
                            }
                        }
                        if (IsIntegerString(szIndex))
                        {
                            nArrayIndex = atoi(szIndex);
                            if (nArrayIndex < 0)
                            {
                                ReportCompilationError("**** NUMBER BETWEEN SQUARE BRACKETS MUST BE POSITIVE ", loc);
                                return -1;
                            }
                        }
                        else
                        {
                            nArrayIndex = GetValueFromMap(szIndex, &g_mapIntConstants, true);
                            if (nArrayIndex < 0)
                            {
                                ReportCompilationError(
                                    "**** CHARACTER BETWEEN SQUARE BRACKETS IS NOT A NUMBER IN ARRAY DECLARATION", loc);
                                return -1;
                            }
                        }
                    }
                    else
                    {
                        nArrayIndex = 1;
                    }
                }
                else
                {
                    nState = PARSE_INDEX;
                    szIndex[nIndexLen] = pszString[i];
                    nIndexLen++;
                    i++;
                    if (nIndexLen >= ARRAY_SIZE(szIndex))
                    {
                        ReportCompilationError("**** STRING BETWEEN SQUARE BRACKETS IS TOO LONG", loc);
                        return -1;
                    }
                }
            }
        }
        i++;
    }

    if (nState == PARSE_NAME)
    {
        int16_t nGlobalVar = GetGlobalVariableIndex(pszString);
        if (nGlobalVar >= 0)
        {
            if (g_aGlobalVarArraySize[nGlobalVar] > 1)
            {
                ReportCompilationError(
                    "**** GLOBAL VARIABLE HAS BEEN DECLARED AS AN ARRAY - EXPECTED SQUARE BRACKETS", loc);
                return -1;
            }
        }
        else
        {
            int16_t nLocalVar = GetLocalVariableIndex(pszString, loc);
            if (nLocalVar >= 0 &&
                static_cast<uint16_t>(LocalVariables::GetArraySize(g_aScopeOfLine[loc], nLocalVar)) > 1)
            {
                ReportCompilationError(
                    "**** LOCAL VARIABLE HAS BEEN DECLARED AS AN ARRAY - EXPECTED SQUARE BRACKETS", loc);
                return -1;
            }
        }
    }

    if (pszNameOut[0] == '$')
    {
        uint16_t nNameLen = strlen(pszNameOut);
        for (int16_t j = 0; j < nNameLen; j++)
        {
            pszNameOut[j] = pszNameOut[j + 1];
        }
    }
    return nArrayIndex;
}

// 0x00405E20 ?WriteScriptHeader@@YAXXZ
void WriteScriptHeader()
{
    int nOffset;
    int16_t nValue;

    g_nStatFloatsWritten = 0;
    g_nStatInt32sWritten = 0;
    g_nStatInt16sWritten = 0;
    g_nStatVarIndicesWritten = 0;
    g_nStatInt8sWritten = 0;
    g_nStatTextLabelsWritten = 0;
    g_nStatStringsWritten = 0;
    g_nStatUnknown1 = 0;
    g_nStatUnknown2 = 0;
    g_fStatUnknown3 = 1.0f;
    g_nStatJumpsInt8 = 0;
    g_nStatJumpsInt16 = 0;
    g_nStatJumpsInt32 = 0;
    g_nStatArgEndOfArgs = 0;
    g_nStatArgInt32 = 0;
    g_nStatArgInt16 = 0;
    g_nStatArgInt8 = 0;
    g_nStatArgGlobalVar = 0;
    g_nStatArgLocalVar = 0;
    g_nStatArgFloat = 0;
    g_nStatArgGlobalArray = 0;
    g_nStatArgLocalArray = 0;
    g_nStatCommandsWritten = 0;
    g_nCodeOffset = 0;
    g_pCompiledCode = g_aCompiledCodeBuffer;
    g_nCompiledCodeBufferSize = ARRAY_SIZE(g_aCompiledCodeBuffer);
    g_nNumLabelRefs = 0;

    // segment 0: GOTO over the global variable space
    nOffset = g_nCodeOffset;
    nValue = COMMAND_GOTO;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_COMMAND;
    g_nStatInt16sWritten++;
    g_nStatCommandsWritten++;
    WriteArgumentDataType(DATA_TYPE_INT32, true);
    int nNextSegment = g_nGlobalVarSpaceSize + SEGMENT_HEADER_SIZE;
    WriteInt32(nNextSegment);

    if (Config_TargetMode == GTA_3)
    {
        nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = 'l';
        g_nCodeOffset++;
        g_nStatInt8sWritten++;
    }
    else if (Config_TargetMode == VICE_CITY)
    {
        nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = 'm';
        g_nCodeOffset++;
        g_nStatInt8sWritten++;
    }
    else if (Config_TargetMode == SAN_ANDREAS || Config_TargetMode == SAN_ANDREAS_PC ||
             Config_TargetMode == SAN_ANDREAS_XBOX || Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = 's';
        g_nCodeOffset++;
        g_nStatInt8sWritten++;
    }
    for (int i = 0; i < g_nGlobalVarSpaceSize; i++)
    {
        nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = 0;
        g_nCodeOffset++;
    }

    // segment 1: model names
    nOffset = g_nCodeOffset;
    nValue = COMMAND_GOTO;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_COMMAND;
    g_nStatInt16sWritten++;
    g_nStatCommandsWritten++;
    int nSize = MODEL_NAME_SIZE * g_nNumUsedModelNames;
    WriteArgumentDataType(DATA_TYPE_INT32, true);
    nNextSegment = nNextSegment + (SEGMENT_HEADER_SIZE + 2 * CODE_SIZE_INT16) + nSize;
    WriteInt32(nNextSegment);
    nOffset = g_nCodeOffset;
    g_pCompiledCode[nOffset] = SEGMENT_MODELS;
    g_nCodeOffset++;
    g_nStatInt8sWritten++;

    nOffset = g_nCodeOffset;
    nValue = g_nNumUsedModelNames;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_INT16;
    g_nStatInt16sWritten++;
    nOffset = g_nCodeOffset;
    nValue = 0;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_INT16;
    g_nStatInt16sWritten++;
    for (int i = 0; i < g_nNumUsedModelNames; i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aUsedModelNames[i]); j++)
        {
            nOffset = g_nCodeOffset;
            g_pCompiledCode[nOffset] = g_aUsedModelNames[i][j];
            g_nCodeOffset++;
            g_nStatInt8sWritten++;
        }
    }

    // segment 2: mission scripts
    nOffset = g_nCodeOffset;
    nValue = COMMAND_GOTO;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_COMMAND;
    g_nStatInt16sWritten++;
    g_nStatCommandsWritten++;
    nSize = GetMissionScriptHeaderSize();
    WriteArgumentDataType(DATA_TYPE_INT32, true);
    nNextSegment = nNextSegment + SEGMENT_HEADER_SIZE + nSize;
    WriteInt32(nNextSegment);
    nOffset = g_nCodeOffset;
    g_pCompiledCode[nOffset] = SEGMENT_MISSIONS;
    g_nCodeOffset++;
    g_nStatInt8sWritten++;
    WriteMultiScriptDataAt = g_nCodeOffset;
    for (int i = 0; i < nSize; i++)
    {
        nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = 0;
        g_nCodeOffset++;
        g_nStatInt8sWritten++;
    }

    // segment 3: streamed scripts
    nOffset = g_nCodeOffset;
    nValue = COMMAND_GOTO;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_COMMAND;
    g_nStatInt16sWritten++;
    g_nStatCommandsWritten++;
    nSize = GetStreamedScriptHeaderSize();
    WriteArgumentDataType(DATA_TYPE_INT32, true);
    nNextSegment = nNextSegment + SEGMENT_HEADER_SIZE + nSize;
    WriteInt32(nNextSegment);
    nOffset = g_nCodeOffset;
    g_pCompiledCode[nOffset] = SEGMENT_STREAMED_SCRIPTS;
    g_nCodeOffset++;
    g_nStatInt8sWritten++;
    g_nStreamedScriptHeaderOffset = g_nCodeOffset;
    for (int i = 0; i < nSize; i++)
    {
        nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = 0;
        g_nCodeOffset++;
        g_nStatInt8sWritten++;
    }

    // segment 4: var console
    nOffset = g_nCodeOffset;
    nValue = COMMAND_GOTO;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_COMMAND;
    g_nStatInt16sWritten++;
    g_nStatCommandsWritten++;
    int nNumEntries = g_nNumVarConsoleEntries;
    WriteArgumentDataType(DATA_TYPE_INT32, true);
    nNextSegment = nNextSegment + (SEGMENT_HEADER_SIZE + 2 * CODE_SIZE_INT16) + nNumEntries * sizeof(tVarConsoleEntry);
    WriteInt32(nNextSegment);
    nOffset = g_nCodeOffset;
    g_pCompiledCode[nOffset] = SEGMENT_VAR_CONSOLE;
    g_nCodeOffset++;
    g_nStatInt8sWritten++;

    nOffset = g_nCodeOffset;
    nValue = g_nNumVarConsoleEntries;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_INT16;
    g_nStatInt16sWritten++;
    nOffset = g_nCodeOffset;
    nValue = 0;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_INT16;
    g_nStatInt16sWritten++;
    for (int i = 0; i < g_nNumVarConsoleEntries; i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aVarConsole[i].szName); j++)
        {
            nOffset = g_nCodeOffset;
            g_pCompiledCode[nOffset] = g_aVarConsole[i].szName[j];
            g_nCodeOffset++;
            g_nStatInt8sWritten++;
        }
        WriteInt32(g_aVarConsole[i].nArrayIndex);
        WriteInt32(g_aVarConsole[i].nOffset);
        WriteInt32(static_cast<uint8_t>(g_aVarConsole[i].bIsFloat));
    }

    // segment 5: sizes
    nOffset = g_nCodeOffset;
    nValue = COMMAND_GOTO;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nValue = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nValue;
    g_nCodeOffset += CODE_SIZE_COMMAND;
    g_nStatInt16sWritten++;
    g_nStatCommandsWritten++;
    WriteArgumentDataType(DATA_TYPE_INT32, true);
    WriteInt32(nNextSegment + (SEGMENT_HEADER_SIZE + 2 * CODE_SIZE_INT32));
    nOffset = g_nCodeOffset;
    g_pCompiledCode[nOffset] = SEGMENT_SIZES;
    g_nCodeOffset++;
    g_nStatInt8sWritten++;
    WriteInt32(g_nGlobalVarSpaceSize);
    WriteInt32(g_nCompileId);
}

// 0x00406420 ?RegisterSwitchLine@@YAHHFH@Z
int RegisterSwitchLine(int loc, int16_t nCommand, int nCaseValue)
{
    switch (nCommand)
    {
        case COMMAND_SWITCH:
            g_nSwitchLOC = loc;
            SetCommandAtLOC(loc, 0, COMMAND_SWITCH_START, false);
            SetAlternativeIndexAtLOC(loc, 0, ALT_NONE, false);
            return 0;
        case COMMAND_ENDSWITCH:
            g_nEndSwitchLOC = loc;
            SetCommandAtLOC(loc, 0, COMMAND_NOP, false);
            return 0;
        case COMMAND_DEFAULT:
            if (g_bSwitchHasDefault)
            {
                ReportCompilationError("**** RegisterSwitchLine - SWITCH statement has two DEFAULTs", loc);
                return -1;
            }
            g_bSwitchHasDefault = true;
            g_nSwitchDefaultLOC = loc;
            SetCommandAtLOC(loc, 0, COMMAND_NOP, false);
            return 0;
        case COMMAND_BREAK:
            if (g_nNumSwitchBreaks >= ARRAY_SIZE(g_aSwitchBreakLOCs))
            {
                ReportCompilationError("**** RegisterSwitchLine - Too many BREAKs in SWITCH statement", loc);
                return -1;
            }
            g_aSwitchBreakLOCs[g_nNumSwitchBreaks++] = loc;
            return 0;
        case COMMAND_CASE:
            if (g_nNumSwitchCases >= ARRAY_SIZE(g_aSwitchCases))
            {
                ReportCompilationError("**** RegisterSwitchLine - Too many CASEs in SWITCH statement", loc);
                return -1;
            }
            g_aSwitchCases[g_nNumSwitchCases].nLOC = loc;
            g_aSwitchCases[g_nNumSwitchCases].nValue = nCaseValue;
            g_nNumSwitchCases++;
            SetCommandAtLOC(loc, 0, COMMAND_NOP, false);
            return 0;
        default:
            ReportCompilationError("**** RegisterSwitchLine - Function can't handle this command", loc);
            return -1;
    }
}

// 0x00406590 ?ResolveLoops@@YAHXZ
int ResolveLoops()
{
    char szLabel[32];
    char szElseLabel[32];
    char szEndLabel[32];
    uint32_t loc;
    uint32_t nEndLOC;
    int nDepth;

    int nRepeatCount = 0;
    for (loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        if (GetCommandAtLOC(loc, 0, false) == COMMAND_REPEAT)
        {
            nEndLOC = loc;
            nDepth = 0;
            do
            {
                nEndLOC++;
                if (nEndLOC >= static_cast<uint32_t>(g_nNumLines))
                {
                    ReportCompilationError("**** COULDN'T FIND MATCH FOR 'REPEAT'", loc);
                    return -1;
                }
                if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_ENDREPEAT)
                {
                    nDepth--;
                }
                else if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_REPEAT)
                {
                    nDepth++;
                }
            } while (nDepth >= 0);

            int nTimes = GetArgIntValueAtLOC(loc, 0, 0, true);
            SetCommandAtLOC(loc, 0, COMMAND_NOP, false);
            SetAlternativeIndexAtLOC(loc, 0, ALT_SET, false);
            SetArgTypeAtLOC(loc, 0, 0, CARG_NAME, false);
            SetArgStringAtLOC(loc, 0, 0, GetArgStringAtLOC(loc, 0, 1, true), false);
            SetArgTypeAtLOC(loc, 0, 1, CARG_INT, false);
            SetArgIntValueAtLOC(loc, 0, 1, 0, false);
            sprintf(szLabel, "SPECIAL_LABEL_REPEATS_%d:", nRepeatCount);
            if (RegisterLabel(szLabel, loc + 1) < 0)
            {
                return -1;
            }
            SetCommandAtLOC(nEndLOC, 0, COMMAND_NOP, false);
            SetAlternativeIndexAtLOC(nEndLOC, 0, ALT_ADD_THING_TO_THING, false);
            SetArgTypeAtLOC(nEndLOC, 0, 0, CARG_NAME, true);
            SetArgStringAtLOC(nEndLOC, 0, 0, GetArgStringAtLOC(loc, 0, 0, true), true);
            SetArgTypeAtLOC(nEndLOC, 0, 1, CARG_INT, true);
            SetArgIntValueAtLOC(nEndLOC, 0, 1, 1, true);
            SetCommandAtLOC(nEndLOC, 1, COMMAND_NOP, true);
            SetAlternativeIndexAtLOC(nEndLOC, 1, ALT_IS_THING_GREATER_OR_EQUAL_TO_THING, true);
            SetArgTypeAtLOC(nEndLOC, 1, 0, CARG_NAME, true);
            SetArgStringAtLOC(nEndLOC, 1, 0, GetArgStringAtLOC(loc, 0, 0, true), true);
            SetArgTypeAtLOC(nEndLOC, 1, 1, CARG_INT, true);
            SetArgIntValueAtLOC(nEndLOC, 1, 1, nTimes, true);
            SetCommandAtLOC(nEndLOC, 2, COMMAND_GOTO_IF_FALSE, true);
            SetAlternativeIndexAtLOC(nEndLOC, 2, ALT_NONE, true);
            SetArgTypeAtLOC(nEndLOC, 2, 0, CARG_NAME, true);
            SetArgStringAtLOC(nEndLOC, 2, 0, szLabel, true);
            nRepeatCount++;
        }
    }

    int nIfCount = 0;
    for (loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        int nCommand = GetCommandAtLOC(loc, 0, false);
        if (nCommand == COMMAND_IF || nCommand == COMMAND_IFNOT)
        {
            nDepth = 0;
            nEndLOC = loc;
            BOOL bHasElse = FALSE;
            uint32_t nElseLOC;
            do
            {
                nEndLOC++;
                if (nEndLOC >= static_cast<uint32_t>(g_nNumLines))
                {
                    ReportCompilationError("**** COULDN'T FIND MATCH FOR 'IF'", loc);
                    return -1;
                }
                if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_ENDIF)
                {
                    nDepth--;
                }
                else if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_IF ||
                         GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_IFNOT)
                {
                    nDepth++;
                }
                if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_ELSE && nDepth == 0)
                {
                    if (bHasElse != 0)
                    {
                        ReportCompilationError("**** TWO ELSES IN ONE IF BLOCK?", loc);
                        return -1;
                    }
                    bHasElse = TRUE;
                    sprintf(szElseLabel, "SPECIAL_LABEL_IFS_ELSE_%d:", nIfCount);
                    if (RegisterLabel(szElseLabel, nEndLOC + 1) < 0)
                    {
                        return -1;
                    }
                    nElseLOC = nEndLOC;
                }
            } while (nDepth >= 0);

            sprintf(szLabel, "SPECIAL_LABEL_IFS_%d:", nIfCount);
            if (RegisterLabel(szLabel, nEndLOC + 1) < 0)
            {
                return -1;
            }
            SetCommandAtLOC(loc, 0, COMMAND_AND_OR, false);
            int8_t nSlot = GetFirstFreeCommandSlotAtLOC(loc);
            if (nSlot == -1)
            {
                ReportCompilationError("**** IF HAS TOO MANY ANDS/ORS TO ADD GOTO", loc);
                return -1;
            }
            int8_t nCmd = nSlot;
            SetCommandAtLOC(loc, nCmd, static_cast<int>(nCommand == COMMAND_IF) + COMMAND_GOTO_IF_TRUE, true);
            SetAlternativeIndexAtLOC(loc, nCmd, ALT_NONE, true);
            SetArgTypeAtLOC(loc, nCmd, 0, CARG_NAME, true);
            if (bHasElse == 0)
            {
                SetArgStringAtLOC(loc, nCmd, 0, szLabel, true);
            }
            else
            {
                SetArgStringAtLOC(loc, nCmd, 0, szElseLabel, true);
                SetCommandAtLOC(nElseLOC, 0, COMMAND_GOTO, false);
                SetAlternativeIndexAtLOC(nElseLOC, 0, ALT_NONE, false);
                SetArgTypeAtLOC(nElseLOC, 0, 0, CARG_NAME, true);
                SetArgStringAtLOC(nElseLOC, 0, 0, szLabel, true);
            }
            SetCommandAtLOC(nEndLOC, 0, COMMAND_NOP, false);
            nIfCount++;
        }
    }

    int nWhileCount = 0;
    for (loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        int nCommand = GetCommandAtLOC(loc, 0, false);
        if (nCommand == COMMAND_WHILE || nCommand == COMMAND_WHILENOT)
        {
            nEndLOC = loc;
            nDepth = 0;
            do
            {
                nEndLOC++;
                if (nEndLOC >= static_cast<uint32_t>(g_nNumLines))
                {
                    ReportCompilationError("**** COULDN'T FIND MATCH FOR 'WHILE'", loc);
                    return -1;
                }
                if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_ENDWHILE)
                {
                    nDepth--;
                }
                else if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_WHILE ||
                         GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_WHILENOT)
                {
                    nDepth++;
                }
            } while (nDepth >= 0);

            sprintf(szLabel, "SPECIAL_LABEL_WHILES_A_%d:", nWhileCount);
            if (RegisterLabel(szLabel, loc) < 0)
            {
                return -1;
            }
            sprintf(szEndLabel, "SPECIAL_LABEL_WHILES_B_%d:", nWhileCount);
            if (RegisterLabel(szEndLabel, nEndLOC + 1) < 0)
            {
                return -1;
            }
            SetCommandAtLOC(loc, 0, COMMAND_AND_OR, false);
            int8_t nSlot = GetFirstFreeCommandSlotAtLOC(loc);
            if (nSlot == -1)
            {
                ReportCompilationError("**** WHILE HAS TOO MANY ANDS/ORS TO ADD GOTO", loc);
                return -1;
            }
            int8_t nCmd = nSlot;
            SetCommandAtLOC(loc, nCmd, static_cast<int>(nCommand == COMMAND_WHILE) + COMMAND_GOTO_IF_TRUE, true);
            SetAlternativeIndexAtLOC(loc, nCmd, ALT_NONE, true);
            SetArgTypeAtLOC(loc, nCmd, 0, CARG_NAME, true);
            SetArgStringAtLOC(loc, nCmd, 0, szEndLabel, true);
            SetCommandAtLOC(nEndLOC, 0, COMMAND_GOTO, false);
            SetArgTypeAtLOC(nEndLOC, 0, 0, CARG_NAME, true);
            SetArgStringAtLOC(nEndLOC, 0, 0, szLabel, true);
            nWhileCount++;
        }
    }

    g_nSwitchCount = 0;
    for (loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        if (GetCommandAtLOC(loc, 0, false) == COMMAND_SWITCH)
        {
            g_nSwitchLOC = -1;
            g_nEndSwitchLOC = -1;
            g_bSwitchHasDefault = false;
            g_nNumSwitchBreaks = 0;
            for (int i = 0; i < ARRAY_SIZE(g_aSwitchBreakLOCs); i++)
            {
                g_aSwitchBreakLOCs[i] = 0;
            }
            g_nSwitchDefaultLOC = -1;
            for (uint16_t i = 0; i < ARRAY_SIZE(g_aSwitchCases); i++)
            {
                g_aSwitchCases[i].nValue = 0;
                g_aSwitchCases[i].nLOC = -1;
            }
            g_nNumSwitchCases = 0;
            if (static_cast<int16_t>(RegisterSwitchLine(loc, COMMAND_SWITCH, -1)) < 0)
            {
                return -1;
            }

            nEndLOC = loc;
            nDepth = 0;
            do
            {
                nEndLOC++;
                if (nEndLOC >= static_cast<uint32_t>(g_nNumLines))
                {
                    ReportCompilationError("**** COULDN'T FIND MATCH FOR 'SWITCH'", loc);
                    return -1;
                }
                if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_ENDSWITCH)
                {
                    if (static_cast<int16_t>(RegisterSwitchLine(nEndLOC, COMMAND_ENDSWITCH, -1)) < 0)
                    {
                        return -1;
                    }
                    nDepth--;
                }
                else
                {
                    if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_SWITCH)
                    {
                        ReportCompilationError("**** CAN'T HAVE ONE SWITCH INSIDE ANOTHER", nEndLOC);
                        return -1;
                    }
                    if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_DEFAULT)
                    {
                        if (static_cast<int16_t>(RegisterSwitchLine(nEndLOC, COMMAND_DEFAULT, -1)) < 0)
                        {
                            return -1;
                        }
                    }
                    else if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_BREAK)
                    {
                        if (static_cast<int16_t>(RegisterSwitchLine(nEndLOC, COMMAND_BREAK, -1)) < 0)
                        {
                            return -1;
                        }
                    }
                    else if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_CASE)
                    {
                        if (GetArgTypeAtLOC(nEndLOC, 0, 0, true) != CARG_INT)
                        {
                            ReportCompilationError("**** PARAMETER AFTER CASE SHOULD BE AN INTEGER", nEndLOC);
                            return -1;
                        }
                        if (static_cast<int16_t>(RegisterSwitchLine(
                                nEndLOC, COMMAND_CASE, GetArgIntValueAtLOC(nEndLOC, 0, 0, true))) < 0)
                        {
                            return -1;
                        }
                    }
                }
            } while (nDepth >= 0);

            if (static_cast<int16_t>(CreateSwitchTable()) < 0)
            {
                return -1;
            }
            g_nSwitchCount++;
        }
    }

    int nSkipCount = 0;
    for (loc = 0; loc < static_cast<uint32_t>(g_nNumLines); loc++)
    {
        if (GetCommandAtLOC(loc, 0, false) == COMMAND_SKIP_CUTSCENE_START)
        {
            nEndLOC = loc;
            nDepth = 0;
            do
            {
                nEndLOC++;
                if (nEndLOC >= static_cast<uint32_t>(g_nNumLines))
                {
                    ReportCompilationError("**** COULDN'T FIND MATCH FOR 'SKIP_CUTSCENE_START'", loc);
                    return -1;
                }
                if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_SKIP_CUTSCENE_END)
                {
                    nDepth--;
                }
                else if (GetCommandAtLOC(nEndLOC, 0, false) == COMMAND_SKIP_CUTSCENE_START)
                {
                    ReportCompilationError("**** CANNOT NEST 'SKIP_CUTSCENE_START' COMMANDS", loc);
                    return -1;
                }
            } while (nDepth >= 0);

            sprintf(szLabel, "SPECIAL_LABEL_SKIP_A_%d:", nSkipCount);
            if (RegisterLabel(szLabel, nEndLOC) < 0)
            {
                return -1;
            }
            SetCommandAtLOC(loc, 0, COMMAND_SKIP_CUTSCENE_START_INTERNAL, true);
            SetArgTypeAtLOC(loc, 0, 0, CARG_NAME, true);
            SetArgStringAtLOC(loc, 0, 0, szLabel, true);
            nSkipCount++;
        }
    }
    return 1;
}

// 0x00406FD0 ?DoArgumentsMatchCommand@@YAHHGF@Z
int DoArgumentsMatchCommand(int loc, uint16_t nCmd, int16_t nCommand)
{
    char szName[NAME_SIZE];
    int nIntValue;
    float fValue;
    int16_t nGlobalVar;
    int16_t nLocalVar;

    int16_t nArg = 0;
    while (true)
    {
        switch (GetArgTypeAtLOC(loc, nCmd, nArg, false))
        {
            case CARG_INT:
                if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_INT_CONST)
                {
                    return -1;
                }
                break;
            case CARG_FLOAT:
                if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_FLOAT_CONST)
                {
                    return -1;
                }
                break;
            case CARG_NAME:
                if (*GetArgStringAtLOC(loc, nCmd, nArg, true) == '&' &&
                    (g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_TEXT_LABEL ||
                        g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_TEXT_LABEL16))
                {
                    break;
                }
                if (ParseVariableName(GetArgStringAtLOC(loc, nCmd, nArg, true), szName, loc, nullptr, nullptr) < 0)
                {
                    return -1;
                }

                nGlobalVar = GetGlobalVariableIndex(szName);
                if (nGlobalVar >= 0)
                {
                    if (g_aGlobalVarType[nGlobalVar] == VAR_TYPE_INT)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_VAR_INT)
                        {
                            return -1;
                        }
                    }
                    else if (g_aGlobalVarType[nGlobalVar] == VAR_TYPE_FLOAT)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_VAR_FLOAT)
                        {
                            return -1;
                        }
                    }
                    else if (g_aGlobalVarType[nGlobalVar] == VAR_TYPE_TEXT_LABEL)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_VAR_TEXT_LABEL &&
                            g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_TEXT_LABEL)
                        {
                            return -1;
                        }
                        goto CheckTextLabelVariable;
                    }
                    else if (g_aGlobalVarType[nGlobalVar] == VAR_TYPE_TEXT_LABEL16)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_VAR_TEXT_LABEL16 &&
                            g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_TEXT_LABEL16)
                        {
                            return -1;
                        }
                        goto CheckTextLabelVariable;
                    }
                    break;
                }

                nLocalVar = GetLocalVariableIndex(szName, loc);
                if (nLocalVar >= 0)
                {
                    if (LocalVariables::GetType(g_aScopeOfLine[loc], nLocalVar) == VAR_TYPE_INT)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_LVAR_INT)
                        {
                            return -1;
                        }
                    }
                    else if (LocalVariables::GetType(g_aScopeOfLine[loc], nLocalVar) == VAR_TYPE_FLOAT)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_LVAR_FLOAT)
                        {
                            return -1;
                        }
                    }
                    else if (LocalVariables::GetType(g_aScopeOfLine[loc], nLocalVar) == VAR_TYPE_TEXT_LABEL)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_LVAR_TEXT_LABEL &&
                            g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_TEXT_LABEL)
                        {
                            return -1;
                        }
                        goto CheckTextLabelVariable;
                    }
                    else if (LocalVariables::GetType(g_aScopeOfLine[loc], nLocalVar) == VAR_TYPE_TEXT_LABEL16)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_LVAR_TEXT_LABEL16 &&
                            g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_TEXT_LABEL16)
                        {
                            return -1;
                        }
                    CheckTextLabelVariable:
                        if (*GetArgStringAtLOC(loc, nCmd, nArg, true) != '$')
                        {
                            ReportCompilationError("**** TEXT LABEL VARIABLE DOESN'T BEGIN WITH $", loc);
                            return -1;
                        }
                    }
                    break;
                }

                if (static_cast<int>(GetIntConstantValue(szName, &nIntValue)) == 1)
                {
                    if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_INT)
                    {
                        return -1;
                    }
                    SetArgIntValueAtLOC(loc, nCmd, nArg, nIntValue, false);
                    SetArgTypeAtLOC(loc, nCmd, nArg, CARG_INT, false);
                }
                else if (static_cast<int>(GetFloatConstantValue(szName, &fValue)) == 1)
                {
                    if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_FLOAT)
                    {
                        return -1;
                    }
                    SetArgFloatValueAtLOC(loc, nCmd, nArg, fValue, false);
                    SetArgTypeAtLOC(loc, nCmd, nArg, CARG_FLOAT, false);
                }
                else if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_TEXT_LABEL &&
                         g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_TEXT_LABEL16)
                {
                    if (strncmp(szName, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                    {
                        if (g_aCommandInfo[nCommand].aArgTypes[nArg] != ARG_INT)
                        {
                            return -1;
                        }
                        SetArgIntValueAtLOC(loc, nCmd, nArg, MISSING_SOUND_ID, false);
                        SetArgTypeAtLOC(loc, nCmd, nArg, CARG_INT, false);
                    }
                    else
                    {
                        ReportCompilationErrorWithName("**** COULD NOT FIND THIS VARIABLE", szName, loc);
                        return -1;
                    }
                }
                break;
            case CARG_NONE:
                if (g_aCommandInfo[nCommand].aArgTypes[nArg] == ARG_NONE)
                {
                    return 1;
                }
                return -1;
        }
        nArg++;
    }
}
