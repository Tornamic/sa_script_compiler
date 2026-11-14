#pragma once

#include "common.h"

#define MAX_EXCLUSIVE_MISSION_SCRIPTS 3    // two initialisations and one mission
#define EXCLUSIVE_MISSION_FIRST_ID 0xFFFF  // exclusive missions are numbered down from here
#define SCRIPT_FILE_EXTENSION ".SC"
#define SCRIPT_FILE_EXTENSION_LENGTH (sizeof(SCRIPT_FILE_EXTENSION) - 1)

// Mission script header: main script size, largest mission size, number of missions, number of exclusive ones and
// the largest number of locals, then the start offset of every mission
#define MISSION_HEADER_START_SIZE (2 * CODE_SIZE_INT32 + 2 * CODE_SIZE_INT16 + CODE_SIZE_INT32)
#define MISSION_HEADER_ENTRY_SIZE CODE_SIZE_INT32

// Kind of script a code offset belongs to (GetScriptIndexFromOffset)
enum eScriptType
{
    SCRIPT_TYPE_MAIN = -1,
    SCRIPT_TYPE_MISSION = 1,
    SCRIPT_TYPE_STREAMED = 2,
};

struct tSubscript
{
    char szName[SCRIPT_FILE_NAME_SIZE];
    int nStartLOC;
    int nStartOffset;
    int nSize;
    bool bHasMissionStart;
    bool bHasMissionEnd;
    uint16_t nNumLines;
};

struct tForeignGosubFile
{
    char szName[SCRIPT_FILE_NAME_SIZE];
    uint16_t nNumLines;
};

struct tMissionScript
{
    char szName[SCRIPT_FILE_NAME_SIZE];
    int nStartLOC;
    int nStartOffset;
    int nSize;
    bool bHasMissionStart;
    bool bHasMissionEnd;
    bool bExclusive;
    uint16_t nNumLines;
};

// ------------------------------------------------------------------------------------- subscripts

extern tSubscript g_aSubscripts[MAX_SUBSCRIPTS];          // 0x01583E80
extern uint16_t g_nNumSubscripts;                         // 0x01584F60
extern int g_nFirstSubscriptOffset;                       // 0x01585124
extern uint32_t g_nLargestSubscriptSize;                  // 0x01585128
extern char g_szLargestSubscript[SCRIPT_FILE_NAME_SIZE];  // 0x0158512C
extern int g_nCurrentSubscript;                           // 0x01585140

// ---------------------------------------------------------------------------- foreign gosub files

extern tForeignGosubFile g_aForeignGosubFiles[MAX_FOREIGN_GOSUB_FILES];  // 0x01584F68
extern uint16_t g_nNumForeignGosubFiles;                                 // 0x01585120

// -------------------------------------------------------------------------------- mission scripts

extern tMissionScript g_aMissionScripts[MAX_MISSION_SCRIPTS];  // 0x01585150
extern uint16_t NumberOfMissionScripts;                        // 0x01587090
extern uint16_t NumberOfExclusiveMissionScripts;               // 0x01587094
extern uint32_t SizeOfMainScript;                              // 0x01587098
extern uint32_t SizeOfLargestMissionScript;                    // 0x0158709C
extern char g_szLargestMissionScript[SCRIPT_FILE_NAME_SIZE];   // 0x015870A0
extern int WriteMultiScriptDataAt;                             // 0x015870B4 code offset of the multi-script data
extern int g_nCurrentMissionScript;                            // 0x015870B8

// missions.cpp

void CalculateSubscriptSizes();                                            // 0x0040DC00
int InitSubscriptsAndForeignGosubs();                                      // 0x0040DD10
void AddFilenameToForeignGosubArray(char* pszLine);                        // 0x0040DDB0
void AddFilenameToMissionScriptArray(char* pszLine);                       // 0x0040E110
void AddFilenameToSubscriptArray(char* pszLine);                           // 0x0040E610
int GetScriptIndexFromOffset(uint32_t nCodeOffset, int8_t* pnScriptType);  // 0x0040E990
int GetMissionScriptHeaderSize();                                          // 0x0040EA40
void InitMissionScripts();                                                 // 0x0040EA50
int16_t ReadMissionScriptFiles();                                          // 0x0040EAE0
void WriteMissionScriptHeader();                                           // 0x0040EC80
void WriteStringToFile(FILE* pFile, const char* pszString);                // 0x0040F240
