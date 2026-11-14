#pragma once

#include "common.h"

// https://github.com/Sergeanur/UndefinifiedGrove
// SERGEANUR_PATCH: SA PC scripts have 32 locals with TIMERA/TIMERB at 32/33. The 3master build uses 40
// (timers at 40/41); the community patch drops it to the PC layout. aVars[LOCAL_VAR_SLOTS] stays as it is.
#ifdef SERGEANUR_PATCH
#define NUM_LOCAL_VARS 32
#else
#define NUM_LOCAL_VARS 40
#endif
#define LOCAL_VAR_TIMERA (NUM_LOCAL_VARS)
#define LOCAL_VAR_TIMERB (NUM_LOCAL_VARS + 1)
#define NUM_LOCAL_VARS_AND_TIMERS (NUM_LOCAL_VARS + 2)  // first extra var of a mission scope
#define LOCAL_VAR_SLOTS 42  // tLocalVarScope::aVars: 40 locals and the timers of the 3master build

struct tLocalVar
{
    char szName[NAME_SIZE];
    int16_t nType;
    int16_t nArraySize;
    int16_t nEntityType;
    int16_t nIndex;
    uint8_t nUsageFlags;
    tLocalVar* pNext;
};

struct tLocalVarScope
{
    tLocalVar aVars[LOCAL_VAR_SLOTS];
    bool bIsMissionScope;
    uint16_t nNumVars;
    tLocalVar* pExtraVars;
};

// -------------------------------------------------------------------------------- local variables

extern tLocalVarScope g_aLocalVarScopes[MAX_SCOPES];  // 0x01463330
extern uint16_t LargestNumberOfLocalVars;             // 0x01582FE0

// locals.cpp

bool IsLocalVariableNameInUse(const char* pszName);                        // 0x00409D00
bool DoesLocalVariableExistInScope(const char* pszName, uint16_t nScope);  // 0x00409D90
int16_t GetLocalVariableIndex(const char* pszName, int loc);               // 0x00409E20

namespace LocalVariables
{
bool IsMissionScope(uint16_t nScope);                                     // 0x00409C60
int Init();                                                               // 0x00409C80
int16_t GetEntityType(uint16_t nScope, uint16_t nVar);                    // 0x00409F10
uint8_t GetUsageFlags(uint16_t nScope, uint16_t nVar);                    // 0x00409F30
uint16_t GetNumberOfLocalVars(uint16_t nScope);                           // 0x00409F50
tLocalVar* GetLocalVar(uint16_t nScope, uint16_t nVar);                   // 0x00409F70
int16_t GetArraySize(uint16_t nScope, uint16_t nVar);                     // 0x00409FF0
char* GetName(uint16_t nScope, uint16_t nVar);                            // 0x0040A010
int16_t GetType(uint16_t nScope, uint16_t nVar);                          // 0x0040A020
void AddLocalVar(uint16_t nScope, int16_t* pnNewVar);                     // 0x0040A040
void SetEntityType(uint16_t nScope, uint16_t nVar, int16_t nEntityType);  // 0x0040A130
void SetUsageFlags(uint16_t nScope, uint16_t nVar, uint8_t nFlags);       // 0x0040A150
void SetMissionScope(uint16_t nScope);                                    // 0x0040A170
void SetArraySize(uint16_t nScope, uint16_t nVar, int16_t nArraySize);    // 0x0040A1A0
bool SetName(uint16_t nScope, uint16_t nVar, const char* pszName);        // 0x0040A1C0
void SetType(uint16_t nScope, uint16_t nVar, int16_t nType);              // 0x0040A280
}  // namespace LocalVariables
