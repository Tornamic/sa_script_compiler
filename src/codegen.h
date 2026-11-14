#pragma once

#include "common.h"

struct tNamedId;

#define COMMAND_NAME_SIZE 56
#define MAX_ALTERNATIVE_COMMANDS 20  // commands one generic name can stand for
#define MAX_LABEL_REFS 99999         // label arguments in the whole script

// ------------------------------------------------------------------------------ compiled code layout

// Sizes of the values written to the compiled code
#define CODE_SIZE_INT8 1
#define CODE_SIZE_INT16 2
#define CODE_SIZE_INT32 4
#define CODE_SIZE_FLOAT 4
#define CODE_SIZE_COMMAND CODE_SIZE_INT16   // command id
#define CODE_SIZE_DATA_TYPE CODE_SIZE_INT8  // eDataType in front of every argument
#define CODE_SIZE_STRING 32                 // ARG_STRING: fixed-size block
#define CODE_SIZE_GOTO (CODE_SIZE_COMMAND + CODE_SIZE_DATA_TYPE + CODE_SIZE_INT32)  // GOTO with an int32 label

// The script header is a chain of segments. Each starts with a GOTO over its data and one byte: the target
// letter for the first segment (the global variables), the eHeaderSegment number for the others.
#define SEGMENT_HEADER_SIZE (CODE_SIZE_GOTO + CODE_SIZE_INT8)
#define GLOBAL_VARS_START SEGMENT_HEADER_SIZE  // code offset of global variable space

#define COMMAND_NOT_FLAG 0x8000                // command id bit: the condition result is inverted (IFNOT, NOT ...)
#define ARRAY_INDEX_GLOBAL_VAR 0x8000          // array flags bit: the index variable is a global one
#define ARRAY_ELEMENT_TYPE_SHIFT 8             // array flags: eVarType of the elements (the low byte is the array size)

// Data type in front of every argument in the compiled code
enum eDataType : uint8_t
{
    DATA_TYPE_END_OF_ARGS,
    DATA_TYPE_INT32,
    DATA_TYPE_GLOBAL_VAR,
    DATA_TYPE_LOCAL_VAR,
    DATA_TYPE_INT8,
    DATA_TYPE_INT16,
    DATA_TYPE_FLOAT,
    DATA_TYPE_GLOBAL_ARRAY,
    DATA_TYPE_LOCAL_ARRAY,
    DATA_TYPE_TEXT_LABEL,
    DATA_TYPE_GLOBAL_TEXT_LABEL,
    DATA_TYPE_LOCAL_TEXT_LABEL,
    DATA_TYPE_GLOBAL_TEXT_LABEL_ARRAY,
    DATA_TYPE_LOCAL_TEXT_LABEL_ARRAY,
    DATA_TYPE_STRING,
    DATA_TYPE_TEXT_LABEL16,
    DATA_TYPE_GLOBAL_TEXT_LABEL16,
    DATA_TYPE_LOCAL_TEXT_LABEL16,
    DATA_TYPE_GLOBAL_TEXT_LABEL16_ARRAY,
    DATA_TYPE_LOCAL_TEXT_LABEL16_ARRAY,
};

// Number written after the GOTO of each header segment but the first
enum eHeaderSegment
{
    SEGMENT_MODELS,
    SEGMENT_MISSIONS,
    SEGMENT_STREAMED_SCRIPTS,
    SEGMENT_VAR_CONSOLE,
    SEGMENT_SIZES,
};

// Index into g_aCommandAlternatives
enum eCommandAlternative
{
    ALT_NONE = -1,  // the command was given by its own name
    ALT_SET,
    ALT_CSET,
    ALT_ADD_THING_TO_THING,
    ALT_SUB_THING_FROM_THING,
    ALT_MULT_THING_BY_THING,
    ALT_DIV_THING_BY_THING,
    ALT_IS_THING_EQUAL_TO_THING,
    ALT_IS_THING_NOT_EQUAL_TO_THING,
    ALT_IS_THING_GREATER_THAN_THING,
    ALT_IS_THING_GREATER_OR_EQUAL_TO_THING,
    ALT_ADD_THING_TO_THING_TIMED,
    ALT_SUB_THING_FROM_THING_TIMED,
    ALT_ABS,
    ALT_IS_STRING_EMPTY,
    ALT_IS_BIT_SET,
    ALT_SET_BIT,
    ALT_CLEAR_BIT,
    ALT_STRING_CAT,
    NUM_COMMAND_ALTERNATIVES
};

struct tCommandInfo
{
    bool bHandledByGame;
    char szName[COMMAND_NAME_SIZE];
    eArgType aArgTypes[MAX_ARGS];
    eEntityType aArgEntityTypes[MAX_ARGS];
};

struct tCommandAlternative
{
    char szName[COMMAND_NAME_SIZE];
    int16_t aCommands[MAX_ALTERNATIVE_COMMANDS];  // -1 terminated
};

// ---------------------------------------------------------------------------------- command table

extern tCommandInfo
    g_aCommandInfo[NUM_COMMANDS];  // 0x0045A000 name, argument types and argument entity types of every command
extern tCommandAlternative
    g_aCommandAlternatives[NUM_COMMAND_ALTERNATIVES];  // 0x004976B0 generic names (SET, ADD_THING_TO_THING...) and the
                                                       // commands they can mean

// ------------------------------------------------------------------------------- label references

extern FILE* g_pScriptCompileInfoFile;                       // 0x0049B368
extern int g_nNumLabelRefs;                                  // 0x0049B36C
extern int g_aLabelRefCodeOffset[MAX_LABEL_REFS];            // 0x0049B370 code offset of each label argument to patch
extern int g_aLabelRefLOC[MAX_LABEL_REFS];                   // 0x004FCDF0 line of each label argument
extern char g_aLabelRefName[MAX_LABEL_REFS + 1][NAME_SIZE];  // 0x0055E870 name of each label argument

// codegen.cpp

void WriteIntegerArgument(int nValue);                                                                     // 0x00407440
void WriteFloat(float fValue);                                                                             // 0x004074F0
void WriteInt16(int16_t nValue);                                                                           // 0x00407560
void WriteInt32(uint32_t nValue);                                                                          // 0x004075B0
void WriteArgumentDataType(uint8_t nType, bool bCountStats);                                               // 0x00407640
int WriteKeywordFiles(bool bMacFormat);                                                                    // 0x00407720
void WriteKeywordList(const char* pszPrefix, const char** apszNames, const char* pszSuffix, FILE* pFile);  // 0x00408120
void WriteKeywordListFromArray(
    const char* pszPrefix, const tNamedId* aNamedIds, const char* pszSuffix, FILE* pFile);                 // 0x004081C0
void WriteKeywordListFromPairs(
    const char* pszPrefix, const char** apPairs, const char* pszSuffix, FILE* pFile);                      // 0x00408260
int WriteCompiledScript();                                                                                 // 0x00408300
int CompileCommand(int loc, uint16_t nCmd);                                                                // 0x00408870
void WriteVariableIndex(int16_t nIndex);                                                                   // 0x00409370
int WriteCommandUsageFile();                                                                               // 0x004093C0
