#pragma once

#include "common.h"

// entitytypes.cpp

int8_t CheckArgumentEntityType(int loc, uint16_t nCmd, int16_t nArg);                                  // 0x0041E150
int8_t CheckAssignmentVariableTypes(int loc, uint16_t nCmd);                                           // 0x0041E330
int8_t RecordCreatedEntityType(int loc, uint16_t nCmd);                                                // 0x0041E5A0
int8_t CheckScriptLaunchArgumentTypes(int loc, uint16_t nCmd, uint16_t nTargetScope, int nTargetLOC);  // 0x0041E770
int8_t CheckStartNewScriptArguments(int loc, uint16_t nCmd);                                           // 0x0041E8D0
int8_t CheckStartNewStreamedScriptArguments(int loc, uint16_t nCmd);                                   // 0x0041E990
