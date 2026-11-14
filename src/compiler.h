#pragma once

#include "common.h"

#define MAX_SCRIPT_NAMES 330
#define SCRIPT_NAME_SIZE 10  // SCRIPT_NAME: up to 7 chars
#define COMPILED_CODE_BUFFER_SIZE 4000000
#define MAX_VAR_CONSOLE_ENTRIES 16
#define MAX_SWITCH_CASES 70
#define MAX_SWITCH_BREAKS 70
#define TEXT_KEY_BUFFER_SIZE 10                // text key in the alphabetical list: up to 7 chars
#define MAX_ARRAY_SIZE 255                     // written in the low byte of the array flags
#define SPECIAL_LABEL_PREFIX "SPECIAL_LABEL_"  // labels the compiler makes up (loops, switches...)
#define SPECIAL_LABEL_PREFIX_LENGTH (sizeof(SPECIAL_LABEL_PREFIX) - 1)
#define NO_SCOPE -1                            // g_aScopeOfLine of a line outside { }

struct tVarConsoleEntry
{
    char szName[NAME_SIZE];
    int nArrayIndex;
    int nOffset;
    bool bIsFloat;
};

struct tSwitchCase
{
    int nValue;
    int nLOC;
};

// ----------------------------------------------------------------------------------- script names

extern int16_t g_nNumScriptNames;                                // 0x00B79070
extern char g_aScriptNames[MAX_SCRIPT_NAMES][SCRIPT_NAME_SIZE];  // 0x00B79078 SCRIPT_NAME of each script
extern int g_aScriptNameLOC[MAX_SCRIPT_NAMES];                   // 0x00B79D60 line of each SCRIPT_NAME

// ------------------------------------------------------------------------------------ stat totals

extern bool g_bCollectable1TotalSet;      // 0x00B79072
extern bool g_bProgressTotalSet;          // 0x00B79073
extern bool g_bMissionRespectTotalSet;    // 0x00B79D5E
extern bool g_bTotalNumberOfMissionsSet;  // 0x00B79D5F
extern int g_nTotalCollectables;          // 0x01445CD4
extern int g_nTotalProgress;              // 0x01445CD8
extern int g_nTotalMissionRespect;        // 0x01445CDC
extern int g_nTotalMissionsPassed;        // 0x01445CE0
extern int g_nTotalMissionsGiven;         // 0x01445CE4

// ---------------------------------------------------------------------------------- compiled code

extern int g_nCodeOffset;                                         // 0x00B79074 write position in the compiled code
extern uint8_t g_aCompiledCodeBuffer[COMPILED_CODE_BUFFER_SIZE];  // 0x00B7A288
extern int g_aCodeOffsetOfLine[MAX_LINES];                        // 0x00F4AB88
extern uint8_t* g_pCompiledCode;                                  // 0x01462F10
extern int g_nCompiledCodeBufferSize;                             // 0x01462F14

// ----------------------------------------------------------------------------------------- scopes

extern int16_t g_aScopeOfLine[MAX_LINES];  // 0x01194A88
extern int g_aScopeStartLOC[MAX_SCOPES];   // 0x012B9A08
extern uint16_t g_nNumScopes;              // 0x01406000

// ------------------------------------------------------------------------------- global variables

extern int16_t g_nNumGlobalVars;                             // 0x00B79D5C
extern char g_aGlobalVarNames[MAX_GLOBAL_VARS][NAME_SIZE];   // 0x012B9F80
extern uint16_t g_aGlobalVarArraySize[MAX_GLOBAL_VARS];      // 0x013F2780
extern int16_t g_aGlobalVarDeclaredInFile[MAX_GLOBAL_VARS];  // 0x013FC3C0
extern int16_t g_aGlobalVarEntityType[MAX_GLOBAL_VARS];      // 0x014064D0
extern uint8_t g_aGlobalVarUsageFlags[MAX_GLOBAL_VARS];      // 0x01410110
extern int g_aGlobalVarOffset[MAX_GLOBAL_VARS];              // 0x01414F30
extern int16_t g_aGlobalVarType[MAX_GLOBAL_VARS];            // 0x014287B0
extern int g_aSaveVarOffsets[MAX_GLOBAL_VARS];               // 0x014323F0
extern int16_t g_nNumSaveVars;                               // 0x01445CCC
extern int g_nGlobalVarSpaceSize;                            // 0x01445CD0

// ------------------------------------------------------------------------------------ var console

extern tVarConsoleEntry g_aVarConsole[MAX_VAR_CONSOLE_ENTRIES];  // 0x01406008
extern int16_t g_nNumVarConsoleEntries;                          // 0x014064C8

// ------------------------------------------------------------------------------------- statistics

extern int g_nStatFloatsWritten;                      // 0x01445C70
extern int g_nStatInt32sWritten;                      // 0x01445C74
extern int g_nStatInt16sWritten;                      // 0x01445C78
extern int g_nStatVarIndicesWritten;                  // 0x01445C7C
extern int g_nStatInt8sWritten;                       // 0x01445C80
extern int g_nStatTextLabelsWritten;                  // 0x01445C84
extern int g_nStatStringsWritten;                     // 0x01445C88
extern int g_nStatUnknown1;                           // 0x01445C8C only reset (to 0) by WriteScriptHeader
extern int g_nStatUnknown2;                           // 0x01445C90 only reset (to 0) by WriteScriptHeader
extern float g_fStatUnknown3;                         // 0x01445C94 only reset (to 1.0f) by WriteScriptHeader
extern int g_nStatJumpsInt8;                          // 0x01445C98
extern int g_nStatJumpsInt16;                         // 0x01445C9C
extern int g_nStatJumpsInt32;                         // 0x01445CA0
extern int g_nStatArgEndOfArgs;                       // 0x01445CA4
extern int g_nStatArgInt32;                           // 0x01445CA8
extern int g_nStatArgInt16;                           // 0x01445CAC
extern int g_nStatArgInt8;                            // 0x01445CB0
extern int g_nStatArgGlobalVar;                       // 0x01445CB4
extern int g_nStatArgLocalVar;                        // 0x01445CB8
extern int g_nStatArgFloat;                           // 0x01445CBC
extern int g_nStatArgGlobalArray;                     // 0x01445CC0
extern int g_nStatArgLocalArray;                      // 0x01445CC4
extern int g_nStatCommandsWritten;                    // 0x01445CC8
extern int g_aCommandUsageCount[NUM_COMMANDS];        // 0x01445CE8
extern int g_aSortedCommandIds[NUM_COMMANDS];         // 0x01448730
extern int g_aSortedCommandUsageCount[NUM_COMMANDS];  // 0x0144B178

// ----------------------------------------------------------------------------------------- switch

extern bool g_bSwitchHasDefault;                      // 0x01406002
extern int g_nSwitchLOC;                              // 0x0144872C
extern int g_nEndSwitchLOC;                           // 0x0144B174
extern int g_nSwitchDefaultLOC;                       // 0x0144DBBC
extern int g_aSwitchBreakLOCs[MAX_SWITCH_BREAKS];     // 0x0144DBC0
extern uint16_t g_nNumSwitchBreaks;                   // 0x0144DCD8
extern uint16_t g_nNumSwitchCases;                    // 0x0144DCDC
extern tSwitchCase g_aSwitchCases[MAX_SWITCH_CASES];  // 0x0144DCE0
extern int g_nSwitchCount;                            // 0x0144DF10

// -------------------------------------------------------------------------------------- text keys

extern FILE* pAlphaText;                                                   // 0x0144DF14
extern FILE* pMainText;                                                    // 0x0144DF18
extern uint16_t g_nNumAlphabeticalTextKeys;                                // 0x0144DF1C
extern char g_aAlphabeticalTextKeys[MAX_TEXT_KEYS][TEXT_KEY_BUFFER_SIZE];  // 0x0144DF20

// compiler.cpp

int16_t AddGlobalVariableToVarConsole(int loc, bool bIsFloat);                                        // 0x00401E80
void AddToAlphabeticalTextKeyList(const char* pszKey);                                                // 0x00402070
int CompileLine(int loc);                                                                             // 0x00402150
int16_t CompileAllLines();                                                                            // 0x00402DB0
int CreateSwitchTable();                                                                              // 0x004037D0
bool IsGlobalVariable(const char* pszName);                                                           // 0x00403C30
int16_t GetGlobalVariableIndex(const char* pszName);                                                  // 0x00403C50
int GetLabelLOC(const char* pszLabel);                                                                // 0x00403C90
int DeclareVariables();                                                                               // 0x00403CB0
int HandleStringArgument(int loc, uint16_t nCmd, uint16_t nArg);                                      // 0x00404720
int HandleArrayArgument(int loc, uint16_t nCmd, int16_t nArg, int8_t nVarKind, int nScriptLabelLOC);  // 0x00404930
int HandleTextStringArgument(int loc, uint16_t nCmd, int16_t nArg);                                   // 0x00405050
int InitVarConsole();                                                                                 // 0x00405A10
int16_t ParseVariableName(const char* pszString, char* pszNameOut, int loc, int16_t* pnIndexGlobalVar,
    int16_t* pnIndexLocalVar);                                                                        // 0x00405A60
void WriteScriptHeader();                                                                             // 0x00405E20
int RegisterSwitchLine(int loc, int16_t nCommand, int nCaseValue);                                    // 0x00406420
int ResolveLoops();                                                                                   // 0x00406590
int DoArgumentsMatchCommand(int loc, uint16_t nCmd, int16_t nCommand);                                // 0x00406FD0
