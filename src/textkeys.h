#pragma once

#include "common.h"

struct tDisplayedTextKey
{
    char szKey[TEXT_LABEL_SIZE];
    char szMissionName[SCRIPT_FILE_NAME_SIZE];
    uint16_t nUsageCount;
    bool bInMission;
};

struct CTextKeyList
{
    tDisplayedTextKey aKeys[MAX_TEXT_KEYS];
    uint16_t nNumKeys;
    void AddToListOfDisplayedTextKeys(const char* pszKey, const char* pszMissionName, bool bInMission);
    void AddToListOfDisplayedTextKeys2(int loc, uint16_t nCmd, uint16_t nArg);
    int InitTextKeyList();
    void WriteMissionTextKeysFile();
    bool GetScriptNameFromFileIndex(int16_t nFileIndex, char* pszNameOut);
};

// ---------------------------------------------------------------------------- displayed text keys

extern CTextKeyList g_DisplayedTextKeys;  // 0x02B23260

// textkeys.cpp
