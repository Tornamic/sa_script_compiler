#pragma once

#include "common.h"

#define MAX_NUM_CHARS_IN_LINE 1024  // a preprocessed line (ProcessLine's buffer)
#define MAX_WIDGET_IDS 256
#define MAX_HID_MAPPINGS 256
#define MAX_PROGRESS_SCRIPTS 200
#define MAX_MISSING_SOUND_EFFECTS 1000
#define MAX_SOUND_EFFECTS 7000
#define MAX_LABEL_LENGTH (NAME_SIZE - 2)  // RegisterLabel cuts longer labels

// ANDOR argument of IF/WHILE: 0 for one condition, ANDOR_AND_BASE + n for n ANDs, ANDOR_OR_BASE + n for n ORs
#define ANDOR_AND_BASE 0
#define ANDOR_OR_BASE 20
#define MAX_ANDOR_CONDITIONS 8

// Quoted strings are split into STRING_NUM_PARTS words of STRING_PART_SIZE characters, each written as one
// ARG_STRING argument. A space and an 'A' in front of each part makes it a name to the tokenizer.
#define STRING_PART_SIZE CODE_SIZE_STRING
#define STRING_NUM_PARTS 4
#define STRING_PART_SEPARATOR_SIZE 2
#define MAX_STRING_LENGTH (STRING_NUM_PARTS * STRING_PART_SIZE)
#define STRING_PARTS_SIZE (STRING_NUM_PARTS * (STRING_PART_SIZE + STRING_PART_SEPARATOR_SIZE))
#define STRING_PART_START(n) ((n) * (STRING_PART_SIZE + STRING_PART_SEPARATOR_SIZE))
#define STRING_PART_SEPARATOR(n) (STRING_PART_START(n) + STRING_PART_SIZE)  // the space after part n
#define STRING_PART_SHIFT(n) ((STRING_NUM_PARTS - 1 - (n)) * STRING_PART_SEPARATOR_SIZE)

// ProcessLine swaps these characters inside quoted strings for placeholders, so the string stays one token;
// HandleStringArgument swaps them back
#define STRING_PLACEHOLDER_SPACE ((char)0xD1)
#define STRING_PLACEHOLDER_SLASH ((char)0xD2)
#define STRING_PLACEHOLDER_COMMA ((char)0xD3)
#define STRING_PLACEHOLDER_OPEN_BRACKET ((char)0xD4)
#define STRING_PLACEHOLDER_CLOSE_BRACKET ((char)0xD5)
#define STRING_PLACEHOLDER_TAB ((char)0xD6)

// ------------------------------------------------------------------------ widgets and HID mapping

extern char* g_apszWidgetIds[MAX_WIDGET_IDS];      // 0x017E66C8 filled by ReadWidgetIDs, "" terminated
extern char* g_apszHIDMappings[MAX_HID_MAPPINGS];  // 0x017E6AC8 filled by ReadHIDMapping, "" terminated
extern int g_nNumWidgetIds;                        // 0x017E6EC8
extern int g_nNumHIDMappings;                      // 0x017E6ECC

// ------------------------------------------------------------------------------------------ lines

extern int g_nNumLines;          // 0x02A366D0
extern int16_t g_nCommentDepth;  // 0x02A366D4

// ------------------------------------------------------------------------------- progress scripts

extern char g_aProgressScripts[MAX_PROGRESS_SCRIPTS][SCRIPT_FILE_NAME_SIZE];  // 0x02A366D8
extern uint16_t g_nNumProgressScripts;                                        // 0x02A37678

// ---------------------------------------------------------------------------------- sound effects

extern uint16_t g_nNumMissingSoundEffects;                                 // 0x02A3767C
extern char g_aMissingSoundEffects[MAX_MISSING_SOUND_EFFECTS][NAME_SIZE];  // 0x02A37680
extern char g_aUsedSoundEffects[MAX_SOUND_EFFECTS][NAME_SIZE];             // 0x02A47080
extern uint16_t g_nNumUsedSoundEffects;                                    // 0x02AB4680
extern uint16_t g_nNumUnusedSoundEffects;                                  // 0x02AB4684
extern char g_aUnusedSoundEffects[MAX_SOUND_EFFECTS][NAME_SIZE];           // 0x02AB4688

// parser.cpp

bool CheckAndOrFollowsIf(int loc, bool bIsOr);                                                    // 0x00415690
bool AppendConditionToIf(int loc, bool bIsOr);                                                    // 0x00415880
int8_t GetFirstFreeCommandSlotAtLOC(int loc);                                                     // 0x00415B70
bool GetFloatConstantValue(const char* pszName, float* pfValue);                                  // 0x00415BD0
bool GetIntConstantValue(const char* pszName, int* pnValue);                                      // 0x00415C10
int InitProgressList();                                                                           // 0x00415C60
int InitMissingSoundEffectsList();                                                                // 0x00415C80
int InitSoundEffectUsageLists();                                                                  // 0x00415CA0
int GetArgumentTypeFromString(const char* pszArg, int loc, uint16_t nCmd, uint16_t nArg);         // 0x00415CD0
bool IsFloatString(const char* pszString);                                                        // 0x00415FB0
bool IsIntegerString(const char* pszString);                                                      // 0x00416000
bool LineStartsWithWord(const char* pszLine, const char* pszWord);                                // 0x00416030
int16_t GetCommandIdFromName(const char* pszName, int loc);                                       // 0x00416100
int ProcessLine(const char* pszLine, char* pszBuffer, int loc, int16_t nFileIndex);               // 0x00416140
int ReadLine(FILE* pFile, char* pBuf, uint16_t nMaxLen);                                          // 0x004177F0
int16_t ReadForeignGosubFiles();                                                                  // 0x00417880
int16_t ReadSubscriptFiles();                                                                     // 0x00417990
int16_t ReadScriptFile(int16_t nFileIndex);                                                       // 0x00417AC0
void ReadHIDMapping(const char* pszFile);                                                         // 0x00418050
void ReadWidgetIDs(const char* pszFile);                                                          // 0x004181D0
int16_t RegisterLabel(char* pszLabel, int loc);                                                   // 0x00418350
int RemoveFirstWord(char* pszString);                                                             // 0x004183D0
int HandleStringConstantArgument(int loc, uint16_t nCmd, uint16_t nArg, int16_t nCommand, const char* pszArg,
    bool bReportErrors);                                                                          // 0x00418440
bool FindFileRecursively(const char* pszDirectory, const char* pszFileName, char* pszFoundPath);  // 0x0041ADC0
int16_t ParseArguments(
    int loc, uint16_t nCmd, int16_t nCommand, const char* pszArgs, bool bReportErrors);           // 0x0041AF80
void BuildCommandList(const char* pszFile);                                                       // 0x0041B390
void WriteProgressListFile();                                                                     // 0x0041B450
void WriteMissingSoundEffectsFile();                                                              // 0x0041B4D0
void WriteUnusedSoundEffectsFiles();                                                              // 0x0041B540
