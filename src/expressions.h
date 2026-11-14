#pragma once

#include "common.h"

// ConvertOperatorsToCommands writes each command it makes into the next line of the caller's buffer
#define EXPRESSION_LINE_SIZE 1024
#define MAX_EXPRESSION_LINES 8

// expressions.cpp

bool ConvertOperatorsToCommands(char* pszLine, int loc, bool bIsCondition);  // 0x00410620
