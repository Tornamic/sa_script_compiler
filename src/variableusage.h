#pragma once

#include "common.h"

#define CONSTANT_NAME_SIZE 50
#define MAX_CONSTANT_USAGE 100000
#define CONSTANT_USAGE_WARNING_COUNT 1069  // RecordConstantUsage prints the count above this

struct tConstantUsage
{
    char szName[CONSTANT_NAME_SIZE];
    char szType[CONSTANT_NAME_SIZE];
    int nValue;
};

struct tArrayAccessRecord
{
    uint32_t nGlobalVar;
    int8_t nVarKind;  // eVarKind
    int16_t nIndexVar;
    uint16_t nCodeOffset;
    char szName[CONSTANT_NAME_SIZE];
};

// --------------------------------------------------------------------------------- constant usage

extern tConstantUsage g_aConstantUsage[MAX_CONSTANT_USAGE];        // 0x02B66568
extern int g_nNumConstantUsage;                                    // 0x03551668
extern int g_nNumLinesUsingConstants;                              // 0x0355166C
extern tConstantUsage g_aLinesUsingConstants[MAX_CONSTANT_USAGE];  // 0x03551670

// ----------------------------------------------------------------------------------- array access

extern std::map<int, tArrayAccessRecord> g_mapArrayAccess;  // 0x044F54F4

// variableusage.cpp

int16_t GetGlobalArrayElementOffset(int nGlobalVar);                                             // 0x0041EE00
void RecordConstantUsage(const char* pszName, const char* pszType, int16_t nValue);              // 0x0041EED0
void AddToArrayAccessMap(int8_t nVarKind, int16_t nIndexVar, uint16_t nGlobalVar, uint16_t nCodeOffset,
    const char* pszName);                                                                        // 0x0041F0E0
void SetVariableReadWriteFlags(int loc, int16_t nVar, bool bGlobal, bool bRead, bool bWritten);  // 0x0041F1C0
int8_t UpdateVariableUsageRecord(int loc, uint16_t nCmd, int16_t nArg);                          // 0x0041F260
void WriteSaveInfoFile();                                                                        // 0x0041F860
int WriteVariableInfoFile();                                                                     // 0x0041FE50
int WriteVariableUsageFile();                                                                    // 0x004201A0
