#pragma once

#include "common.h"

// ------------------------------------------------------------------------------- main script file

extern uint16_t g_nMainScriptNumLines;               // 0x01462F20
extern char g_szMainScriptFileName[FILE_PATH_SIZE];  // 0x01462F28

// errors.cpp

int GetFileIndexAndLineFromLOC(int loc, int16_t* pnFileIndex);                            // 0x00409650
void InitMainScriptFileInfo();                                                            // 0x004097A0
void ReportCompilationErrorWithName(const char* pszError, const char* pszName, int loc);  // 0x004097D0
void ReportCompilationError(const char* pszError, int loc);                               // 0x00409900
