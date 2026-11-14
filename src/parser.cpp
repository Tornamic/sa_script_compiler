// parser.cpp: 0x415690-0x41B709 in sc.exe

#include "common.h"
#include "parser.h"
#include "expressions.h"
#include "variableusage.h"
#include "streamedscripts.h"
#include "missions.h"
#include "codegen.h"
#include "main.h"
#include "compiledlines.h"
#include "namemaps.h"
#include "errors.h"
#include "loader.h"

// 0x017E66C8
char* g_apszWidgetIds[MAX_WIDGET_IDS];
// 0x017E6AC8
char* g_apszHIDMappings[MAX_HID_MAPPINGS];
// 0x017E6EC8
int g_nNumWidgetIds;
// 0x017E6ECC
int g_nNumHIDMappings;

// 0x02A366D0
int g_nNumLines;
// 0x02A366D4
int16_t g_nCommentDepth;

// 0x02A366D8
char g_aProgressScripts[MAX_PROGRESS_SCRIPTS][SCRIPT_FILE_NAME_SIZE];
// 0x02A37678
uint16_t g_nNumProgressScripts;

// 0x02A3767C
uint16_t g_nNumMissingSoundEffects;
// 0x02A37680
char g_aMissingSoundEffects[MAX_MISSING_SOUND_EFFECTS][NAME_SIZE];
// 0x02A47080
char g_aUsedSoundEffects[MAX_SOUND_EFFECTS][NAME_SIZE];
// 0x02AB4680
uint16_t g_nNumUsedSoundEffects;
// 0x02AB4684
uint16_t g_nNumUnusedSoundEffects;
// 0x02AB4688
char g_aUnusedSoundEffects[MAX_SOUND_EFFECTS][NAME_SIZE];

// 0x00415690 ?CheckAndOrFollowsIf@@YADHD@Z
bool CheckAndOrFollowsIf(int loc, bool bIsOr)
{
    uint16_t nCount = 0;
    int nLoc = loc - 1;
    while (GetCommandAtLOC(nLoc, 0, false) == COMMAND_NOP)
    {
        nLoc--;
        nCount++;
    }
    if (bIsOr == true && (nCount != 0u))
    {
        nCount += ANDOR_OR_BASE;
    }
    if (GetCommandAtLOC(nLoc, 0, true) != COMMAND_IF && GetCommandAtLOC(nLoc, 0, true) != COMMAND_IFNOT &&
        GetCommandAtLOC(nLoc, 0, true) != COMMAND_WHILE && GetCommandAtLOC(nLoc, 0, true) != COMMAND_WHILENOT)
    {
        ReportCompilationError("**** LINE BEFORE AND/OR DOESN'T CONTAIN IF/WHILE", loc);
        return false;
    }
    if (bIsOr == false)
    {
        if (GetArgTypeAtLOC(nLoc, 0, 0, true) != CARG_INT ||
            GetArgIntValueAtLOC(nLoc, 0, 0, true) != 0 &&
                (GetArgIntValueAtLOC(nLoc, 0, 0, true) < ANDOR_AND_BASE + 1 ||
                    GetArgIntValueAtLOC(nLoc, 0, 0, true) > ANDOR_AND_BASE + MAX_ANDOR_CONDITIONS))
        {
            ReportCompilationError("**** IF/WHILE CONTAINS ANDS AND ORS OR TOO MANY ANDS", loc);
            return false;
        }
        if (GetArgIntValueAtLOC(nLoc, 0, 0, true) != nCount)
        {
            ReportCompilationError("**** STRANGE ERROR WHILE PROCESSING AN AND", loc);
            return false;
        }
    }
    else if (bIsOr == true)
    {
        if (GetArgTypeAtLOC(nLoc, 0, 0, true) != CARG_INT ||
            GetArgIntValueAtLOC(nLoc, 0, 0, true) != 0 &&
                (GetArgIntValueAtLOC(nLoc, 0, 0, true) < ANDOR_OR_BASE + 1 ||
                    GetArgIntValueAtLOC(nLoc, 0, 0, true) > ANDOR_OR_BASE + MAX_ANDOR_CONDITIONS))
        {
            ReportCompilationError("**** IF/WHILE CONTAINS ANDS AND ORS OR TOO MANY ORS", loc);
            return false;
        }
        if (GetArgIntValueAtLOC(nLoc, 0, 0, true) != nCount)
        {
            ReportCompilationError("**** STRANGE ERROR WHILE PROCESSING AN OR", loc);
            return false;
        }
    }
    return true;
}

// 0x00415880 ?AppendConditionToIf@@YADHD@Z
bool AppendConditionToIf(int loc, bool bIsOr)
{
    char szValue[12];
    int nLoc = loc - 1;
    while (GetCommandAtLOC(nLoc, 0, false) == COMMAND_NOP)
    {
        nLoc--;
    }
    int8_t nSlot = GetFirstFreeCommandSlotAtLOC(nLoc);
    if (nSlot == -1)
    {
        ReportCompilationError("**** IF/WHILE CONTAINS TOO MANY ANDS/ORS", nLoc);
        return false;
    }
    SetCommandAtLOC(nLoc, nSlot, GetCommandAtLOC(loc, 0, true), true);
    SetAlternativeIndexAtLOC(nLoc, nSlot, GetAlternativeIndexAtLOC(loc, 0, true), true);
    SetNotFlagAtLOC(nLoc, nSlot, GetNotFlagAtLOC(loc, 0, true), true);
    for (uint16_t i = 0; i < MAX_ARGS; i++)
    {
        eCompiledArgType nType = GetArgTypeAtLOC(loc, 0, i, false);
        if (nType != CARG_NONE)
        {
            SetArgTypeAtLOC(nLoc, nSlot, i, nType, true);
            if (nType == CARG_INT)
            {
                SetArgIntValueAtLOC(nLoc, nSlot, i, GetArgIntValueAtLOC(loc, 0, i, true), true);
            }
            else if (nType == CARG_FLOAT)
            {
                SetArgFloatValueAtLOC(nLoc, nSlot, i, GetArgFloatValueAtLOC(loc, 0, i, true), true);
            }
            SetArgStringAtLOC(nLoc, nSlot, i, GetArgStringAtLOC(loc, 0, i, true), true);
        }
    }
    if (!bIsOr)
    {
        if (GetArgIntValueAtLOC(nLoc, 0, 0, true) == 0)
        {
            SetArgIntValueAtLOC(nLoc, 0, 0, ANDOR_AND_BASE + 1, false);
            sprintf(szValue, "%d", GetArgIntValueAtLOC(nLoc, 0, 0, true));
            SetArgStringAtLOC(nLoc, 0, 0, szValue, false);
        }
        else
        {
            if (GetArgIntValueAtLOC(nLoc, 0, 0, true) < ANDOR_AND_BASE + 1 ||
                GetArgIntValueAtLOC(nLoc, 0, 0, true) >= ANDOR_AND_BASE + MAX_ANDOR_CONDITIONS)
            {
                ReportCompilationError("**** IF/WHILE ALREADY CONTAINS THE MAX NUMBER OF ANDS", nLoc);
                return false;
            }
            SetArgIntValueAtLOC(nLoc, 0, 0, GetArgIntValueAtLOC(nLoc, 0, 0, true) + 1, false);
            sprintf(szValue, "%d", GetArgIntValueAtLOC(nLoc, 0, 0, true));
            SetArgStringAtLOC(nLoc, 0, 0, szValue, false);
        }
    }
    else
    {
        if (GetArgIntValueAtLOC(nLoc, 0, 0, true) == 0)
        {
            SetArgIntValueAtLOC(nLoc, 0, 0, ANDOR_OR_BASE + 1, false);
            sprintf(szValue, "%d", GetArgIntValueAtLOC(nLoc, 0, 0, true));
            SetArgStringAtLOC(nLoc, 0, 0, szValue, false);
        }
        else
        {
            if (GetArgIntValueAtLOC(nLoc, 0, 0, true) < ANDOR_OR_BASE + 1 ||
                GetArgIntValueAtLOC(nLoc, 0, 0, true) >= ANDOR_OR_BASE + MAX_ANDOR_CONDITIONS)
            {
                ReportCompilationError("**** IF/WHILE ALREADY CONTAINS THE MAX NUMBER OF ORS", nLoc);
                return false;
            }
            SetArgIntValueAtLOC(nLoc, 0, 0, GetArgIntValueAtLOC(nLoc, 0, 0, true) + 1, false);
            sprintf(szValue, "%d", GetArgIntValueAtLOC(nLoc, 0, 0, true));
            SetArgStringAtLOC(nLoc, 0, 0, szValue, false);
        }
    }
    FreeCommandAtLOC(loc, 0);
    return true;
}

// 0x00415B70 ?GetFirstFreeCommandSlotAtLOC@@YADH@Z
int8_t GetFirstFreeCommandSlotAtLOC(int loc)
{
    int8_t nSlot = 0;
    bool bFound = false;
    do
    {
        if (bFound)
        {
            break;
        }
        if (GetCommandAtLOC(loc, nSlot, false) == COMMAND_NOP &&
            GetAlternativeIndexAtLOC(loc, nSlot, false) == ALT_NONE)
        {
            bFound = true;
        }
        else
        {
            nSlot++;
        }
    } while (nSlot < MAX_COMMANDS_PER_LINE);
    if (!bFound)
    {
        return -1;
    }
    return nSlot;
}

// 0x00415BD0 ?GetFloatConstantValue@@YADPADPAM@Z
bool GetFloatConstantValue(const char* pszName, float* pfValue)
{
    float fValue = -1.0f;
    if (static_cast<int>(FindInFloatMap(pszName, &g_mapFloatConstants, &fValue)) == 1)
    {
        *pfValue = fValue;
        return true;
    }
    return false;
}

// 0x00415C10 ?GetIntConstantValue@@YADPADPAH@Z
bool GetIntConstantValue(const char* pszName, int* pnValue)
{
    int nValue;
    if (static_cast<int>(FindInMap(pszName, &g_mapAllEnumConstants, &nValue, true)) != 1 &&
        static_cast<int>(FindInMap(pszName, &g_mapIntConstants, &nValue, true)) != 1)
    {
        return false;
    }
    *pnValue = nValue;
    return true;
}

// 0x00415C60 ?InitProgressList@@YAHXZ
int InitProgressList()
{
    g_nNumProgressScripts = 0;
    for (int i = 0; i < ARRAY_SIZE(g_aProgressScripts); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aProgressScripts[i]); j++)
        {
            g_aProgressScripts[i][j] = 0;
        }
    }
    return 0;
}

// 0x00415C80 ?InitMissingSoundEffectsList@@YAHXZ
int InitMissingSoundEffectsList()
{
    g_nNumMissingSoundEffects = 0;
    for (int i = 0; i < ARRAY_SIZE(g_aMissingSoundEffects); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aMissingSoundEffects[i]); j++)
        {
            g_aMissingSoundEffects[i][j] = 0;
        }
    }
    return 0;
}

// 0x00415CA0 ?InitSoundEffectUsageLists@@YAHXZ
int InitSoundEffectUsageLists()
{
    g_nNumUsedSoundEffects = 0;
    for (int i = 0; i < ARRAY_SIZE(g_aUsedSoundEffects); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aUsedSoundEffects[i]); j++)
        {
            g_aUsedSoundEffects[i][j] = 0;
        }
    }
    g_nNumUnusedSoundEffects = 0;
    for (int i = 0; i < ARRAY_SIZE(g_aUnusedSoundEffects); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aUnusedSoundEffects[i]); j++)
        {
            g_aUnusedSoundEffects[i][j] = 0;
        }
    }
    return 0;
}

// 0x00415CD0 ?GetArgumentTypeFromString@@YAHPADHGG@Z
int GetArgumentTypeFromString(const char* pszArg, int loc, uint16_t nCmd, uint16_t nArg)
{
    char szString[NAME_SIZE];
    int nValue;
    float fValue;
    if (IsIntegerString(pszArg) && sscanf(pszArg, "%d", &nValue) > 0)
    {
        SetArgIntValueAtLOC(loc, nCmd, nArg, nValue, true);
        sscanf(pszArg, "%s", szString);
        SetArgStringAtLOC(loc, nCmd, nArg, szString, true);
        return CARG_INT;
    }
    if (IsFloatString(pszArg) && sscanf(pszArg, "%f", &fValue) > 0)
    {
        SetArgFloatValueAtLOC(loc, nCmd, nArg, fValue, true);
        sscanf(pszArg, "%s", szString);
        SetArgStringAtLOC(loc, nCmd, nArg, szString, true);
        return CARG_FLOAT;
    }
    if ((pszArg[0] >= 'a' && pszArg[0] <= 'z' || pszArg[0] >= 'A' && pszArg[0] <= 'Z' || pszArg[0] == '$' ||
            pszArg[0] == '&') &&
        pszArg[strlen(pszArg) - 1] != ':' && sscanf(pszArg, "%s", szString) > 0)
    {
        SetArgStringAtLOC(loc, nCmd, nArg, szString, true);
        if (strncmp(szString, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
        {
            if (GetValueFromMap(szString, &g_mapSoundEffects, true) < 0)
            {
                bool bFound = false;
                for (uint16_t i = 0; i < g_nNumMissingSoundEffects; i++)
                {
                    if (strcmp(g_aMissingSoundEffects[i], szString) == 0)
                    {
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    strncpy(g_aMissingSoundEffects[g_nNumMissingSoundEffects], szString, NAME_SIZE);
                    g_nNumMissingSoundEffects++;
                }
            }
            if (GetValueFromMap(szString, &g_mapSoundEffects, true) >= 0)
            {
                bool bFound = false;
                for (uint16_t i = 0; i < g_nNumUsedSoundEffects; i++)
                {
                    if (strcmp(g_aUsedSoundEffects[i], szString) == 0)
                    {
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    strncpy(g_aUsedSoundEffects[g_nNumUsedSoundEffects], szString, NAME_SIZE);
                    g_nNumUsedSoundEffects++;
                }
            }
        }
        return CARG_NAME;
    }
    return CARG_INVALID;
}

// 0x00415FB0 ?IsFloatString@@YADPAD@Z
bool IsFloatString(const char* pszString)
{
    int16_t i = 0;
    bool bFloat = false;
    char c = pszString[0];
    if (c == '\0')
    {
        return false;
    }
    do
    {
        if (c < '0' || c > '9')
        {
            if (c != '.' && c != 'f' && c != 'F' && (c != '-' || i != 0))
            {
                return false;
            }
            bFloat = true;
        }
        c = pszString[++i];
    } while (c != '\0');
    return bFloat;
}

// 0x00416000 ?IsIntegerString@@YADPAD@Z
bool IsIntegerString(const char* pszString)
{
    int16_t i = 0;
    if (pszString[0] == '\0')
    {
        return false;
    }
    while (pszString[i] != '\0')
    {
        if ((pszString[i] < '0' || pszString[i] > '9') && pszString[i] != '-')
        {
            return false;
        }
        i++;
    }
    return true;
}

// 0x00416030 ?LineStartsWithWord@@YA_NPADPBD@Z
bool LineStartsWithWord(const char* pszLine, const char* pszWord)
{
    char szWord[128];
    uint16_t i = 0;
    uint16_t j = 0;
    while (pszLine[i] == '\t' || pszLine[i] == ' ')
    {
        if (pszLine[i] == '\0')
        {
            return false;
        }
        i++;
    }
    while (pszLine[i] != '\t' && pszLine[i] != ' ' && pszLine[i] != '\0' && j < ARRAY_SIZE(szWord) - 1)
    {
        szWord[j++] = pszLine[i++];
    }
    szWord[j] = '\0';
    if (strcmp(szWord, pszWord) == 0)
    {
        return true;
    }
    return false;
}

// 0x00416100 ?GetCommandIdFromName@@YAFPADH@Z
int16_t GetCommandIdFromName(const char* pszName, int loc)
{
    int16_t nId = GetValueFromMap(pszName, &g_mapCommandNames, false);
    if (nId == -1)
    {
        ReportCompilationError("**** Command not handled by game code", loc);
    }
    return nId;
}

// 0x00416140 ?ProcessLine@@YAHPAD0HF@Z
int ProcessLine(const char* pszLine, char* pszBuffer, int loc, int16_t nFileIndex)
{
    char szLabel[32];
    char szName[128];
    char szWord[128];
    char szValue[128];
    char aLines[MAX_EXPRESSION_LINES][EXPRESSION_LINE_SIZE];
    int nValue;
    bool bForeignGosub;
    bool bStreamed;
    bool bSubscript;
    bool bMission;
    if (nFileIndex == FILE_INDEX_MAIN)
    {
        bStreamed = false;
        bForeignGosub = false;
        bSubscript = false;
        bMission = false;
    }
    else if (nFileIndex < FILE_INDEX_SUBSCRIPTS)
    {
        bForeignGosub = true;
        bStreamed = false;
        bSubscript = false;
        bMission = false;
    }
    else if (nFileIndex < FILE_INDEX_MISSIONS)
    {
        bForeignGosub = false;
        bStreamed = false;
        bSubscript = true;
        bMission = false;
        nFileIndex -= FILE_INDEX_SUBSCRIPTS;
    }
    else if (nFileIndex < FILE_INDEX_STREAMED_SCRIPTS)
    {
        bForeignGosub = false;
        bMission = true;
        bStreamed = false;
        bSubscript = false;
        nFileIndex -= FILE_INDEX_MISSIONS;
    }
    else
    {
        bStreamed = true;
        nFileIndex -= FILE_INDEX_STREAMED_SCRIPTS;
        bForeignGosub = false;
        bSubscript = false;
        bMission = false;
    }

    FreeCompiledLine(loc);
    int16_t nLen = 0;
    bool bInQuotes = false;
    for (char c = pszLine[0]; c != '\0'; c = pszLine[nLen])
    {
        pszBuffer[nLen] = c;
        if (!bInQuotes)
        {
            if (c == '"')
            {
                bInQuotes = true;
            }
        }
        else
        {
            if (c == '"')
            {
                bInQuotes = false;
            }
        }
        if (c >= 'a' && c <= 'z' && !bInQuotes)
        {
            pszBuffer[nLen] = c - ('a' - 'A');
        }
        nLen++;
    }
    nValue = nLen;
    pszBuffer[nLen] = '\0';
    if (nLen >= MAX_NUM_CHARS_IN_LINE)
    {
        ReportCompilationError("**** LINE HAS MORE THAN MAX_NUM_CHARS_IN_LINE CHARACTERS", loc);
        return -1;
    }

    int16_t i = 0;
    bool bInString = false;
    int16_t nStringLen = 0;
    bool bHasString = false;
    int16_t nStringStart;
    char szString[STRING_PARTS_SIZE + 1];
    for (int k = 0; k < STRING_PARTS_SIZE; k++)
    {
        szString[k] = ' ';
    }
    szString[STRING_PARTS_SIZE] = 0;
    while (pszBuffer[i] != '\0')
    {
        if (pszBuffer[i] == '"' && bInString == false)
        {
            bool bInComment = false;
            for (int16_t j = 0; j < i; j++)
            {
                if (pszBuffer[j] == '/' && pszBuffer[j + 1] == '/')
                {
                    bInComment = true;
                    j = i;
                }
            }
            if (!bInComment)
            {
                pszBuffer[i] = 'A';
                bInString = true;
                nStringStart = i + 1;
            }
        }
        if (pszBuffer[i] == ' ' && bInString == true)
        {
            pszBuffer[i] = STRING_PLACEHOLDER_SPACE;
        }
        if (pszBuffer[i] == '/' && bInString == true)
        {
            pszBuffer[i] = STRING_PLACEHOLDER_SLASH;
        }
        if (pszBuffer[i] == ',' && bInString == true)
        {
            pszBuffer[i] = STRING_PLACEHOLDER_COMMA;
        }
        if (pszBuffer[i] == '(' && bInString == true)
        {
            pszBuffer[i] = STRING_PLACEHOLDER_OPEN_BRACKET;
        }
        if (pszBuffer[i] == ')' && bInString == true)
        {
            pszBuffer[i] = STRING_PLACEHOLDER_CLOSE_BRACKET;
        }
        if (pszBuffer[i] == '\t' && bInString == true)
        {
            pszBuffer[i] = STRING_PLACEHOLDER_TAB;
        }
        if (pszBuffer[i] == '"' && bInString == true)
        {
            bInString = false;
            bHasString = true;
            pszBuffer[i] = '\0';
        }
        if (bInString == true)
        {
            szString[nStringLen++ + STRING_PART_SHIFT(0) - 1] = pszBuffer[i];
        }
        if (nStringLen > MAX_STRING_LENGTH)
        {
            ReportCompilationError("**** STRING MUST BE LESS THAN 128 CHARS ", loc);
            return -1;
        }
        i++;
    }
    if (bInString == true)
    {
        ReportCompilationError("**** LINE HAS STRING WITHOUT A CLOSING QUOTE", loc);
        return -1;
    }
    if (bHasString == true)
    {
        for (int16_t j = 0; j < STRING_PARTS_SIZE; j++)
        {
            if (j == STRING_PART_SEPARATOR(0) || j == STRING_PART_SEPARATOR(1) || j == STRING_PART_SEPARATOR(2))
            {
                szString[j] = ' ';
            }
            if (j == STRING_PART_SEPARATOR(0) + 1 || j == STRING_PART_SEPARATOR(1) + 1 ||
                j == STRING_PART_SEPARATOR(2) + 1)
            {
                szString[j] = 'A';
            }
            if (j < STRING_PART_SEPARATOR(0))
            {
                szString[j] = szString[j + STRING_PART_SHIFT(0)];
            }
            if (j >= STRING_PART_START(1) && j < STRING_PART_SEPARATOR(1))
            {
                szString[j] = szString[j + STRING_PART_SHIFT(1)];
            }
            if (j >= STRING_PART_START(2) && j < STRING_PART_SEPARATOR(2))
            {
                szString[j] = szString[j + STRING_PART_SHIFT(2)];
            }
        }
        memmove(&pszBuffer[nStringStart], szString, sizeof(szString));
    }

    for (int16_t j = 0; pszBuffer[j] != '\0'; j++)
    {
        if (pszBuffer[j] == '\t')
        {
            pszBuffer[j] = ' ';
        }
    }

    for (int16_t j = 0; j < static_cast<int16_t>(strlen(pszBuffer)); j++)
    {
        if (pszBuffer[j] == '/' && pszBuffer[j + 1] == '*')
        {
            pszBuffer[j] = ' ';
            pszBuffer[j + 1] = ' ';
            g_nCommentDepth++;
        }
        else if (pszBuffer[j] == '*' && pszBuffer[j + 1] == '/')
        {
            pszBuffer[j] = ' ';
            pszBuffer[j + 1] = ' ';
            g_nCommentDepth--;
            if (g_nCommentDepth < 0)
            {
                ReportCompilationError("**** UNMATCHED /*  */ PAIR", loc);
                return -1;
            }
        }
        if (g_nCommentDepth > 0)
        {
            pszBuffer[j] = ' ';
        }
    }

    for (int16_t j = 0; j < nLen - 1; j++)
    {
        if (pszBuffer[j] == '/' && pszBuffer[j + 1] == '/')
        {
            pszBuffer[j] = '\0';
            nLen = j;
        }
    }
    for (int16_t j = 0; j < nLen; j++)
    {
        if (pszBuffer[j] == ',' || pszBuffer[j] == '(' || pszBuffer[j] == ')')
        {
            pszBuffer[j] = ' ';
        }
    }

    while (pszBuffer[0] == '\t' || pszBuffer[0] == ' ')
    {
        int16_t j = 0;
        while (pszBuffer[j + 1] != '\0')
        {
            pszBuffer[j] = pszBuffer[j + 1];
            j++;
        }
        pszBuffer[j] = '\0';
    }
    for (int16_t j = strlen(pszBuffer); j > 0; j--)
    {
        if (pszBuffer[j - 1] != '\t' && pszBuffer[j - 1] != ' ')
        {
            break;
        }
        pszBuffer[j - 1] = '\0';
    }

    if (sscanf(pszBuffer, "%s", szWord) > 0)
    {
        if (szWord[strlen(szWord) - 1] == ':')
        {
            if (RegisterLabel(szWord, loc) == -1)
            {
                return -1;
            }
            RemoveFirstWord(pszBuffer);
        }
        else if (strcmp(szWord, "CONST_INT") == 0)
        {
            RemoveFirstWord(pszBuffer);
            if (sscanf(pszBuffer, "%s", szName) > 0)
            {
                if ((szName[0] < 'a' || szName[0] > 'z') && (szName[0] < 'A' || szName[0] > 'Z') && szName[0] != '$' &&
                        szName[0] != '&' ||
                    szName[strlen(szName) - 1] == ':')
                {
                    ReportCompilationError("**** FIRST PARAMETER AFTER CONST_INT MUST BE A STRING", loc);
                    return -1;
                }
                RemoveFirstWord(pszBuffer);
                if (sscanf(pszBuffer, "%s", szValue) > 0)
                {
                    if (IsIntegerString(szValue))
                    {
                        int nConstant = atoi(szValue);
                        if (GetIntConstantValue(szName, &nValue))
                        {
                            ReportCompilationError("**** CONSTANT NAME EXISTS ALREADY AS A CONSTANT", loc);
                            return -1;
                        }
                        if (GetFloatConstantValue(szName, reinterpret_cast<float*>(&nValue)))
                        {
                            ReportCompilationError("**** CONSTANT NAME EXISTS ALREADY AS A FLOAT CONSTANT", loc);
                            return -1;
                        }
                        if (!AddToMap(&g_mapIntConstants, szName, nConstant, true))
                        {
                            ReportCompilationError("**** ERROR WHEN ADDING TO THE MAP OF CONSTANT INTEGERS", loc);
                            return -1;
                        }
                        pszBuffer[0] = 0;
                    }
                    else
                    {
                        ReportCompilationError("**** SECOND PARAMETER AFTER CONST_INT MUST BE AN INTEGER", loc);
                        return -1;
                    }
                }
                else
                {
                    ReportCompilationError("**** NEED AN INTEGER AS A SECOND PARAMETER AFTER CONST_INT", loc);
                    return -1;
                }
            }
            else
            {
                ReportCompilationError("**** NEED A NAME AFTER CONST_INT", loc);
                return -1;
            }
        }
        else if (strcmp(szWord, "CONST_FLOAT") == 0)
        {
            RemoveFirstWord(pszBuffer);
            if (sscanf(pszBuffer, "%s", szName) > 0)
            {
                if ((szName[0] < 'a' || szName[0] > 'z') && (szName[0] < 'A' || szName[0] > 'Z') && szName[0] != '$' &&
                        szName[0] != '&' ||
                    szName[strlen(szName) - 1] == ':')
                {
                    ReportCompilationError("**** FIRST PARAMETER AFTER CONST_FLOAT MUST BE A STRING", loc);
                    return -1;
                }
                RemoveFirstWord(pszBuffer);
                if (sscanf(pszBuffer, "%s", szValue) > 0)
                {
                    if (IsFloatString(szValue))
                    {
                        float fConstant = static_cast<float>(atof(szValue));
                        if (GetIntConstantValue(szName, &nValue))
                        {
                            ReportCompilationError("**** CONSTANT NAME EXISTS ALREADY AS A CONSTANT", loc);
                            return -1;
                        }
                        if (GetFloatConstantValue(szName, reinterpret_cast<float*>(&nValue)))
                        {
                            ReportCompilationError("**** CONSTANT NAME EXISTS ALREADY AS A FLOAT CONSTANT", loc);
                            return -1;
                        }
                        if (!AddToFloatMap(&g_mapFloatConstants, szName, fConstant, true))
                        {
                            ReportCompilationError("**** ERROR WHEN ADDING TO THE MAP OF CONSTANT FLOATS", loc);
                            return -1;
                        }
                        pszBuffer[0] = 0;
                    }
                    else
                    {
                        ReportCompilationError("**** SECOND PARAMETER AFTER CONST_FLOAT MUST BE A FLOAT", loc);
                        return -1;
                    }
                }
                else
                {
                    ReportCompilationError("**** NEED A FLOAT AS A SECOND PARAMETER AFTER CONST_FLOAT", loc);
                    return -1;
                }
            }
            else
            {
                ReportCompilationError("**** NEED A NAME AFTER CONST_FLOAT", loc);
                return -1;
            }
        }
    }

    if (nFileIndex == FILE_INDEX_MAIN || bForeignGosub)
    {
        AddFilenameToForeignGosubArray(pszBuffer);
        AddFilenameToSubscriptArray(pszBuffer);
        AddFilenameToMissionScriptArray(pszBuffer);
    }
    else if (sscanf(pszBuffer, "%s", szWord) > 0)
    {
        if (strcmp(szWord, "MISSION_START") == 0)
        {
            if (bStreamed)
            {
                if (g_aStreamedScripts[nFileIndex].bHasMissionStart)
                {
                    ReportCompilationError("**** MORE THAN ONE MISSION_START IN STREAMED SCRIPT", loc);
                    return -1;
                }
                g_aStreamedScripts[nFileIndex].bHasMissionStart = true;
                pszBuffer[0] = '\0';
            }
            else if (bMission)
            {
                if (g_aMissionScripts[nFileIndex].bHasMissionStart)
                {
                    ReportCompilationError("**** MORE THAN ONE MISSION_START IN MISSION SCRIPT", loc);
                    return -1;
                }
                g_aMissionScripts[nFileIndex].bHasMissionStart = true;
                pszBuffer[0] = '\0';
            }
            else
            {
                if (g_aSubscripts[nFileIndex].bHasMissionStart)
                {
                    ReportCompilationError("**** MORE THAN ONE MISSION_START IN SUBSCRIPT", loc);
                    return -1;
                }
                sprintf(szLabel, "SPECIAL_LABEL_SUBSCRIPT_%d:", nFileIndex);
                if (RegisterLabel(szLabel, loc) == -1)
                {
                    ReportCompilationError("**** ERROR WHEN REGISTERING LABEL FOR SUBSCRIPT", loc);
                    return -1;
                }
                g_aSubscripts[nFileIndex].bHasMissionStart = true;
                pszBuffer[0] = '\0';
            }
        }
        else if (strcmp(szWord, "MISSION_END") == 0)
        {
            if (bStreamed)
            {
                if (g_aStreamedScripts[nFileIndex].bHasMissionEnd)
                {
                    ReportCompilationError("**** MORE THAN ONE MISSION_END IN STREAMED SCRIPT", loc);
                    return -1;
                }
                g_aStreamedScripts[nFileIndex].bHasMissionEnd = true;
                strcpy(pszBuffer, "TERMINATE_THIS_SCRIPT");
            }
            else if (bMission)
            {
                if (g_aMissionScripts[nFileIndex].bHasMissionEnd)
                {
                    ReportCompilationError("**** MORE THAN ONE MISSION_END IN MISSION SCRIPT", loc);
                    return -1;
                }
                g_aMissionScripts[nFileIndex].bHasMissionEnd = true;
                strcpy(pszBuffer, "TERMINATE_THIS_SCRIPT");
            }
            else
            {
                if (g_aSubscripts[nFileIndex].bHasMissionEnd)
                {
                    ReportCompilationError("**** MORE THAN ONE MISSION_END IN SUBSCRIPT", loc);
                    return -1;
                }
                g_aSubscripts[nFileIndex].bHasMissionEnd = true;
                strcpy(pszBuffer, "TERMINATE_THIS_SCRIPT");
            }
        }
    }

    for (int16_t j = 0; j < ARRAY_SIZE(aLines); j++)
    {
        aLines[j][0] = '\0';
    }
    strcpy(aLines[0], pszBuffer);
    bool bAnd = false;
    bool bOr = false;
    bool bNot = false;
    bool bNegated = false;
    if (LineStartsWithWord(aLines[0], "AND"))
    {
        if (!CheckAndOrFollowsIf(loc, false))
        {
            return -1;
        }
        bAnd = true;
        RemoveFirstWord(aLines[0]);
    }
    else if (LineStartsWithWord(aLines[0], "OR"))
    {
        if (!CheckAndOrFollowsIf(loc, true))
        {
            return -1;
        }
        bOr = true;
        RemoveFirstWord(aLines[0]);
    }
    if (LineStartsWithWord(aLines[0], "NOT"))
    {
        if (!bAnd && !bOr)
        {
            ReportCompilationError("**** NOT AT START OF LINE WITHOUT AND/OR", loc);
            return -1;
        }
        bNot = true;
        RemoveFirstWord(aLines[0]);
    }
    if (ConvertOperatorsToCommands(aLines[0], loc, bAnd || bOr))
    {
        bNegated = true;
    }
    if ((bAnd || bOr || bNot) && aLines[1][0] != '\0')
    {
        ReportCompilationError("**** AND/OR/NOT COMMAND HAS BEEN SPREAD OVER MORE THAN ONE LINE", loc);
        return -1;
    }

    for (int16_t nCmd = 0; nCmd < ARRAY_SIZE(aLines); nCmd++)
    {
        char* pszCmdLine = aLines[nCmd];
        if (pszCmdLine[0] == '\0')
        {
            return 0;
        }
        if (sscanf(aLines[nCmd], "%s", szWord) <= 0)
        {
            continue;
        }
        int16_t nAlt = ALT_NONE;
        for (int16_t a = 0; a < ARRAY_SIZE(g_aCommandAlternatives); a++)
        {
            if (strcmp(szWord, g_aCommandAlternatives[a].szName) == 0)
            {
                nAlt = a;
                break;
            }
        }
        if (nAlt >= 0)
        {
            RemoveFirstWord(pszCmdLine);
            int16_t nOption = 0;
            SetCommandAtLOC(loc, nCmd, g_aCommandAlternatives[nAlt].aCommands[nOption], true);
            while (ParseArguments(loc, nCmd, GetCommandAtLOC(loc, nCmd, true), pszCmdLine, false) < 0)
            {
                if (GetCommandAtLOC(loc, nCmd, true) < 0)
                {
                    ReportCompilationError("**** COULDN'T MATCH ALTERNATIVE WHILST READING IN", loc);
                    return -1;
                }
                nOption++;
                SetCommandAtLOC(loc, nCmd, g_aCommandAlternatives[nAlt].aCommands[nOption], true);
            }
            SetAlternativeIndexAtLOC(loc, nCmd, nAlt, true);
            SetCommandAtLOC(loc, nCmd, COMMAND_NOP, true);
        }
        else
        {
            int16_t nCommand = GetCommandIdFromName(szWord, loc);
            if (nCommand == COMMAND_PLAYER_MADE_PROGRESS)
            {
                if (nFileIndex == FILE_INDEX_MAIN)
                {
                    strcpy(g_aProgressScripts[g_nNumProgressScripts], "main");
                }
                else if (bForeignGosub)
                {
                    strcpy(g_aProgressScripts[g_nNumProgressScripts], g_aForeignGosubFiles[nFileIndex].szName);
                }
                else if (bSubscript)
                {
                    strcpy(g_aProgressScripts[g_nNumProgressScripts], g_aSubscripts[nFileIndex].szName);
                }
                else if (bMission)
                {
                    strcpy(g_aProgressScripts[g_nNumProgressScripts], g_aMissionScripts[nFileIndex].szName);
                }
                else if (bStreamed)
                {
                    strcpy(g_aProgressScripts[g_nNumProgressScripts], g_aStreamedScripts[nFileIndex].szName);
                }
                g_nNumProgressScripts++;
            }
            if (nCommand < 0)
            {
                ReportCompilationError("**** UNKNOWN COMMAND", loc);
                return -1;
            }
            SetCommandAtLOC(loc, nCmd, nCommand, true);
            RemoveFirstWord(pszCmdLine);
            if (ParseArguments(loc, nCmd, nCommand, pszCmdLine, true) < 0)
            {
                return -1;
            }
        }
        if (bNot)
        {
            SetNotFlagAtLOC(loc, nCmd, true, true);
        }
        if (bNegated && nCmd == 1)
        {
            SetNotFlagAtLOC(loc, 1, true, true);
        }
        if (bAnd)
        {
            if (!AppendConditionToIf(loc, false))
            {
                return -1;
            }
        }
        else if (bOr)
        {
            if (!AppendConditionToIf(loc, true))
            {
                return -1;
            }
        }
    }
    return 0;
}

// 0x004177F0 ?ReadLine@@YAHPAU_iobuf@@PADG@Z
int ReadLine(FILE* pFile, char* pBuf, uint16_t nMaxLen)
{
    int16_t i = 0;
    int16_t c;
    for (c = fgetc(pFile); c != EOF; c = fgetc(pFile))
    {
        if (c == '\n')
        {
            break;
        }
        pBuf[i++] = static_cast<char>(c);
    }
    pBuf[i] = '\0';
    if (c == EOF && i == 0)
    {
        return -1;
    }
    if (i >= nMaxLen)
    {
        ReportCompilationError("**** LINE IS TOO LONG", g_nNumLines);
        return -1;
    }
    if (i == 0)
    {
        return 0;
    }
    return i;
}

// 0x00417880 ?ReadForeignGosubFiles@@YAHXZ
int16_t ReadForeignGosubFiles()
{
    char szMessage[128];
    char szInputFileName[INPUT_FILE_NAME_SIZE];
    strcpy(szInputFileName, Config_InputFileName);
    for (uint16_t i = 0; i < g_nNumForeignGosubFiles; i++)
    {
        if (ReadScriptFile(i + FILE_INDEX_FOREIGN_GOSUBS) < 0)
        {
            sprintf(szMessage, "ERROR Reading Foreign Gosub File %s", g_aForeignGosubFiles[i].szName);
            PrintMessage(szMessage);
            strcpy(Config_InputFileName, szInputFileName);
            return -1;
        }
        sprintf(szMessage, "Reading Foreign Gosub File %s. . .", g_aForeignGosubFiles[i].szName);
        PrintMessage(szMessage);
    }
    strcpy(Config_InputFileName, szInputFileName);
    return 0;
}

// 0x00417990 ?ReadSubscriptFiles@@YAHXZ
int16_t ReadSubscriptFiles()
{
    char szMessage[128];
    char szInputFileName[INPUT_FILE_NAME_SIZE];
    strcpy(szInputFileName, Config_InputFileName);
    for (uint16_t i = 0; i < g_nNumSubscripts; i++)
    {
        g_aSubscripts[i].nStartLOC = g_nNumLines;
        if (ReadScriptFile(i + FILE_INDEX_SUBSCRIPTS) < 0)
        {
            sprintf(szMessage, "ERROR Reading Subscript %s", g_aSubscripts[i].szName);
            PrintMessage(szMessage);
            strcpy(Config_InputFileName, szInputFileName);
            return -1;
        }
        sprintf(szMessage, "Reading Subscript %s. . .", g_aSubscripts[i].szName);
        PrintMessage(szMessage);
    }
    strcpy(Config_InputFileName, szInputFileName);
    return 0;
}

// 0x00417AC0 ?ReadScriptFile@@YAHF@Z
int16_t ReadScriptFile(int16_t nFileIndex)
{
    char szBuffer[1024];
    char szLine[1024];
    char szCurrentDirectory[256];
    char szMessage[256];
    uint16_t nNumLines = 0;
    bool bForeignGosub = false;
    bool bMission = false;
    bool bStreamed = false;
    bool bSubscript = false;
    bool bFirstLine = true;
    if (nFileIndex != FILE_INDEX_MAIN)
    {
        if (nFileIndex < FILE_INDEX_SUBSCRIPTS)
        {
            bForeignGosub = true;
            bSubscript = false;
            bMission = false;
            bStreamed = false;
            if (!FindFileRecursively(
                    Config_FileNameNoExt, g_aForeignGosubFiles[nFileIndex].szName, Config_InputFileName))
            {
                sprintf(szMessage, "**** Couldn't open input file %s", g_aForeignGosubFiles[nFileIndex].szName);
                PrintMessage(szMessage);
                return -1;
            }
        }
        else if (nFileIndex < FILE_INDEX_MISSIONS)
        {
            nFileIndex -= FILE_INDEX_SUBSCRIPTS;
            bForeignGosub = false;
            bSubscript = true;
            bMission = false;
            bStreamed = false;
            if (!FindFileRecursively(Config_FileNameNoExt, g_aSubscripts[nFileIndex].szName, Config_InputFileName))
            {
                sprintf(szMessage, "**** Couldn't open input file %s", g_aSubscripts[nFileIndex].szName);
                PrintMessage(szMessage);
                return -1;
            }
        }
        else if (nFileIndex < FILE_INDEX_STREAMED_SCRIPTS)
        {
            nFileIndex -= FILE_INDEX_MISSIONS;
            bForeignGosub = false;
            bSubscript = false;
            bMission = true;
            bStreamed = false;
            if (!FindFileRecursively(Config_FileNameNoExt, g_aMissionScripts[nFileIndex].szName, Config_InputFileName))
            {
                sprintf(szMessage, "**** Couldn't open input file %s", g_aMissionScripts[nFileIndex].szName);
                PrintMessage(szMessage);
                return -1;
            }
        }
        else
        {
            nFileIndex -= FILE_INDEX_STREAMED_SCRIPTS;
            bForeignGosub = false;
            bSubscript = false;
            bMission = false;
            bStreamed = true;
            if (!FindFileRecursively(Config_FileNameNoExt, g_aStreamedScripts[nFileIndex].szName, Config_InputFileName))
            {
                sprintf(szMessage, "**** Couldn't open input file %s", g_aStreamedScripts[nFileIndex].szName);
                PrintMessage(szMessage);
                return -1;
            }
        }
    }
    FILE* pFile = fopen(Config_InputFileName, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open input file %s", Config_InputFileName);
        PrintMessage(szMessage);
        return -1;
    }
    if (nFileIndex == FILE_INDEX_MAIN)
    {
        GetCurrentDirectoryA(ARRAY_SIZE(szCurrentDirectory), szCurrentDirectory);
        memset(szMessage, 0, sizeof(szMessage));
        GetDirectoryFromPath(Config_InputFileName, szMessage);
        if (strcmp(szCurrentDirectory, szMessage) != 0)
        {
            _chdir(szMessage);
            strcpy(szCurrentDirectory, szMessage);
        }
        g_nNumLines = 0;
    }
    for (g_nCommentDepth = 0; ReadLine(pFile, szLine, ARRAY_SIZE(szLine)) != -1; nNumLines++)
    {
        fwrite(szLine, strlen(szLine), 1, g_pPreprocessedFile);
        fwrite("\n", 1, 1, g_pPreprocessedFile);
        int16_t nResult = 0;
        if (nFileIndex == FILE_INDEX_MAIN || bForeignGosub)
        {
            nResult = ProcessLine(szLine, szBuffer, g_nNumLines, nFileIndex);
        }
        else
        {
            if (!LineStartsWithWord(szLine, "MISSION_START") && bFirstLine)
            {
                sprintf(szMessage, "**** First line of subscript file %s doesn't contain MISSION_START",
                    Config_InputFileName);
                PrintMessage(szMessage);
                ReportCompilationError("**** First line of subscript file doesn't contain MISSION_START", g_nNumLines);
                fclose(pFile);
                return -1;
            }
            bFirstLine = false;
            if (bSubscript)
            {
                nResult = ProcessLine(szLine, szBuffer, g_nNumLines, nFileIndex + FILE_INDEX_SUBSCRIPTS);
            }
            else if (bMission)
            {
                nResult = ProcessLine(szLine, szBuffer, g_nNumLines, nFileIndex + FILE_INDEX_MISSIONS);
            }
            else if (bStreamed)
            {
                nResult = ProcessLine(szLine, szBuffer, g_nNumLines, nFileIndex + FILE_INDEX_STREAMED_SCRIPTS);
            }
        }
        if (nResult < 0)
        {
            fclose(pFile);
            return -1;
        }
        g_nNumLines++;
    }
    if (g_nCommentDepth > 0)
    {
        ReportCompilationError("**** STILL IN /*  */ COMMENT AT END OF FILE (BAD)", g_nNumLines - 1);
        fclose(pFile);
        return -1;
    }
    fclose(pFile);
    if (nFileIndex == FILE_INDEX_MAIN)
    {
        g_nMainScriptNumLines = nNumLines;
        return 0;
    }
    if (bForeignGosub)
    {
        g_aForeignGosubFiles[nFileIndex].nNumLines = nNumLines;
        return 0;
    }
    if (bSubscript)
    {
        if (!g_aSubscripts[nFileIndex].bHasMissionEnd)
        {
            sprintf(szMessage, "**** Subscript file %s doesn't contain a MISSION_END", Config_InputFileName);
            PrintMessage(szMessage);
            ReportCompilationError("**** Subscript file doesn't contain a MISSION_END", g_nNumLines - 1);
            return -1;
        }
        g_aSubscripts[nFileIndex].nNumLines = nNumLines;
        return 0;
    }
    if (bMission)
    {
        if (!g_aMissionScripts[nFileIndex].bHasMissionEnd)
        {
            sprintf(szMessage, "**** Mission Script file %s doesn't contain a MISSION_END", Config_InputFileName);
            PrintMessage(szMessage);
            ReportCompilationError("**** Mission Script file doesn't contain a MISSION_END", g_nNumLines - 1);
            return -1;
        }
        g_aMissionScripts[nFileIndex].nNumLines = nNumLines;
        return 0;
    }
    if (!g_aStreamedScripts[nFileIndex].bHasMissionEnd)
    {
        sprintf(szMessage, "**** Streamed Script file %s doesn't contain a MISSION_END", Config_InputFileName);
        PrintMessage(szMessage);
        ReportCompilationError("**** Streamed Script file doesn't contain a MISSION_END", g_nNumLines - 1);
        return -1;
    }
    g_aStreamedScripts[nFileIndex].nNumLines = nNumLines;
    return 0;
}

// 0x00418050 ?ReadHIDMapping@@YAXPAD@Z
void ReadHIDMapping(const char* pszFile)
{
    char szLine[256];
    char szName[128];
    FILE* pFile = fopen(pszFile, "rb");
    if (pFile == nullptr)
    {
        return;
    }
    BOOL bInEnum = FALSE;
    int nLen = ReadLine(pFile, szLine, ARRAY_SIZE(szLine));
    while (nLen != -1)
    {
        if ((bInEnum == 0) && (strstr(szLine, "enum HIDMapping") != nullptr))
        {
            bInEnum = TRUE;
        }
        if (bInEnum != 0)
        {
            char* pszStart = strstr(szLine, "HID_MAPPING_");
            if (pszStart != nullptr)
            {
                char* pszEnd = strstr(pszStart, ",");
                if (pszEnd == nullptr)
                {
                    g_apszHIDMappings[g_nNumHIDMappings] = _strdup("");
                    break;
                }
                strncpy(szName, pszStart, pszEnd - pszStart);
                szName[pszEnd - pszStart] = '\0';
                g_apszHIDMappings[g_nNumHIDMappings++] = _strdup(szName);
            }
            else if (strstr(szLine, "NUM_HID_MAPPINGS") != nullptr)
            {
                g_apszHIDMappings[g_nNumHIDMappings] = _strdup("");
                break;
            }
        }
        nLen = ReadLine(pFile, szLine, ARRAY_SIZE(szLine));
    }
    fclose(pFile);
}

// 0x004181D0 ?ReadWidgetIDs@@YAXPAD@Z
void ReadWidgetIDs(const char* pszFile)
{
    char szLine[256];
    char szName[128];
    FILE* pFile = fopen(pszFile, "rb");
    if (pFile == nullptr)
    {
        return;
    }
    BOOL bInEnum = FALSE;
    int nLen = ReadLine(pFile, szLine, ARRAY_SIZE(szLine));
    while (nLen != -1)
    {
        if ((bInEnum == 0) && (strstr(szLine, "enum WidgetIDs") != nullptr))
        {
            bInEnum = TRUE;
        }
        if (bInEnum != 0)
        {
            char* pszStart = strstr(szLine, "WIDGET_");
            if (pszStart != nullptr)
            {
                char* pszEnd = strstr(pszStart, ",");
                if (pszEnd == nullptr)
                {
                    g_apszWidgetIds[g_nNumWidgetIds] = _strdup("");
                    break;
                }
                strncpy(szName, pszStart, pszEnd - pszStart);
                szName[pszEnd - pszStart] = '\0';
                g_apszWidgetIds[g_nNumWidgetIds++] = _strdup(szName);
            }
            else if (strstr(szLine, "NUM_WIDGETS") != nullptr)
            {
                g_apszWidgetIds[g_nNumWidgetIds] = _strdup("");
                break;
            }
        }
        nLen = ReadLine(pFile, szLine, ARRAY_SIZE(szLine));
    }
    fclose(pFile);
}

// 0x00418350 ?RegisterLabel@@YAFPADH@Z
int16_t RegisterLabel(char* pszLabel, int loc)
{
    pszLabel[min(MAX_LABEL_LENGTH, strlen(pszLabel) - 1)] = '\0';
    if (AddToMap(&g_mapLabels, pszLabel, loc, true))
    {
        return 1;
    }
    ReportCompilationError("**** LABEL OCCURED TWICE", loc);
    return -1;
}

// 0x004183D0 ?RemoveFirstWord@@YAHPAD@Z
int RemoveFirstWord(char* pszString)
{
    int16_t i = 0;
    while (pszString[i] == ' ')
    {
        i++;
    }
    while (pszString[i] != ' ' && pszString[i] != '\0')
    {
        i++;
    }
    int16_t nStart = i;
    while (pszString[i] != '\0')
    {
        pszString[i - nStart] = pszString[i];
        i++;
    }
    pszString[i - nStart] = '\0';
    return i - nStart;
}

// 0x00418440 ?HandleStringConstantArgument@@YAHHGGFPADD@Z
int HandleStringConstantArgument(
    int loc, uint16_t nCmd, uint16_t nArg, int16_t nCommand, const char* pszArg, bool bReportErrors)
{
    int nValue;
    bool bNegate = false;
    bool bModel = false;
    bool bFound = false;
    switch (nCommand)
    {
        case COMMAND_SET_CAR_DRIVING_STYLE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDrivingModes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CAR_MISSION:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarMissions, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_CHAR_IN_MODEL:
        case COMMAND_IS_PLAYER_IN_MODEL:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_BUTTON_PRESSED:
        case COMMAND_GET_PAD_STATE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPads, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPadSticks, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CHAR_INSIDE_CAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_POINT_CAMERA_AT_PLAYER:
        case COMMAND_POINT_CAMERA_AT_CAR:
        case COMMAND_POINT_CAMERA_AT_CHAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraModes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraSwitchModes, true);
                bFound = true;
            }
            break;
        case COMMAND_POINT_CAMERA_AT_POINT:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraSwitchModes, true);
                bFound = true;
            }
            break;
        case COMMAND_SHAKE_PAD:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPads, true);
                bFound = true;
            }
            break;
        case COMMAND_CHANGE_BLIP_COLOUR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapColours, true);
                bFound = true;
            }
            break;
        case COMMAND_DO_FADE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFadeTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_DRAW_SHADOW:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapShadowTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_AMMO_TO_PLAYER:
        case COMMAND_ADD_AMMO_TO_CHAR:
        case COMMAND_ADD_AMMO_TO_CAR:
        case COMMAND_SET_PLAYER_AMMO:
        case COMMAND_SET_CHAR_AMMO:
        case COMMAND_SET_CAR_AMMO:
        case COMMAND_GIVE_WEAPON_TO_PLAYER:
        case COMMAND_GIVE_WEAPON_TO_CHAR:
        case COMMAND_GIVE_WEAPON_TO_CAR:
        case COMMAND_SET_CURRENT_PLAYER_WEAPON:
        case COMMAND_SET_CURRENT_CHAR_WEAPON:
        case COMMAND_SET_CURRENT_CAR_WEAPON:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_MOVE_CAMERA_ALONG_SPLINE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraSwitchModes, true);
                bFound = true;
            }
            break;
        case COMMAND_CHANGE_BLIP_DISPLAY:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBlipDisplayTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_ONE_OFF_SOUND:
        case COMMAND_ADD_CONTINUOUS_SOUND:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSoundEffects, true);
                if (nValue < 0 && strncmp(pszArg, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                {
                    nValue = MISSING_SOUND_ID;
                }
                bFound = true;
            }
            break;
        case COMMAND_FORCE_WEATHER:
        case COMMAND_FORCE_WEATHER_NOW:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeatherTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CHAR_AS_PASSENGER:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_CHANGE_CAR_LOCK:
        case COMMAND_LOCK_CAR_DOORS:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarLockStates, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_PICKUP:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPickupTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_CAR_ARMED_WITH_BOMB:
        case COMMAND_ARM_CAR_WITH_BOMB:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarBombTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_CHANGE_CAR_COLOUR:
            if (nArg == 1 || nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarColours, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_GANG_PED_MODELS:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            else if (nArg == 1 || nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_GANG_CAR_MODEL:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_GANG_WEAPONS:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            else if (nArg == 1 || nArg == 2 || nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CUTSCENE_OBJECT:
        case COMMAND_LOAD_SPECIAL_MODEL:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_CHANGE_GARAGE_TYPE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGarageTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_OBJ_FOLLOW_CHAR_IN_FORMATION:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFormations, true);
                bFound = true;
            }
            break;
        case COMMAND_DISPLAY_ONSCREEN_TIMER:
        case COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapTimerDirections, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_BLIP_FOR_CAR_OLD:
        case COMMAND_ADD_BLIP_FOR_CHAR_OLD:
        case COMMAND_ADD_BLIP_FOR_OBJECT_OLD:
        case COMMAND_ADD_BLIP_FOR_PICKUP_OLD:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapColours, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBlipDisplayTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_START_KILL_FRENZY:
        case COMMAND_START_KILL_FRENZY_HEADSHOT:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            else if (nArg == 4 || nArg == 5 || nArg == 6 || nArg == 7)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_SPRITE_BLIP_FOR_CAR:
        case COMMAND_ADD_SPRITE_BLIP_FOR_CHAR:
        case COMMAND_ADD_SPRITE_BLIP_FOR_OBJECT:
        case COMMAND_ADD_SPRITE_BLIP_FOR_PICKUP:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapRadarSprites, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CAMERA_ZOOM:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCamZooms, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_TEXT_FONT:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFonts, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_EXPLOSION_IN_AREA:
        case COMMAND_IS_EXPLOSION_IN_ZONE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapExplosionTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_WAIT_STATE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWaitStates, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_MOTION_BLUR:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapMotionBlurTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CAR_STATUS:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapEntityStatus, true);
                bFound = true;
            }
            break;
        case COMMAND_CHANGE_GARAGE_TYPE_WITH_CAR_MODEL:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGarageTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_SWAP_NEAREST_BUILDING_MODEL:
            if (nArg == 4 || nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_IS_COLLISION_IN_MEMORY:
        case COMMAND_LOAD_COLLISION:
        case COMMAND_OVERRIDE_HOSPITAL_LEVEL:
        case COMMAND_OVERRIDE_POLICE_STATION_LEVEL:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapLevels, true);
                bFound = true;
            }
            break;
        case COMMAND_FORCE_RANDOM_PED_TYPE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_FLASH_HUD_OBJECT:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHudFlashItems, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_CAR_DOOR_CLOSED:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarDoors, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_RADIO_CHANNEL:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapRadioStations, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CHAR:
        case COMMAND_CREATE_SWAT_ROPE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_BLIP_FOR_COORD_OLD:
        case COMMAND_ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapColours, true);
                bFound = true;
            }
            else if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBlipDisplayTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_DRAW_CORONA:
        case COMMAND_DRAW_WEAPONSHOP_CORONA:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCoronaTypes, true);
                bFound = true;
            }
            else if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFlareTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_AREA:
        case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_AREA_NO_SAVE:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_DISPLAY_ONSCREEN_COUNTER:
        case COMMAND_DISPLAY_ONSCREEN_COUNTER_WITH_STRING:
        case COMMAND_DISPLAY_NTH_ONSCREEN_COUNTER:
        case COMMAND_DISPLAY_NTH_ONSCREEN_COUNTER_WITH_STRING:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCounterDisplayTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_CHAR_TO_CAR:
        case COMMAND_ATTACH_CHAR_TO_OBJECT:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFacingDirections, true);
                bFound = true;
            }
            else if (nArg == 7)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CAR_TEMP_ACTION:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapTempActions, true);
                bFound = true;
            }
            break;
        case COMMAND_CAN_CHAR_SEE_DEAD_CHAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_REPLAY:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapReplayCamModes, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_CAR_TYRE_BURST:
        case COMMAND_BURST_CAR_TYRE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarWheels, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_PLAYER_MOOD:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapMoods, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_SET_PIECE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSetPieces, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_PLAYER_WEARING:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapClothesTextures, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_CAN_BE_DAMAGED_BY_MEMBERS_OF_GANG:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_CUTSCENE_OBJECT_TO_BONE:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBones, true);
                bFound = true;
            }
            break;
        case COMMAND_CHECK_FOR_PED_MODEL_AROUND_PLAYER:
            if (nArg == 4 || nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_GIVE_REMOTE_CONTROLLED_MODEL_TO_PLAYER:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_SPRITE_BLIP_FOR_CONTACT_POINT:
        case COMMAND_ADD_SPRITE_BLIP_FOR_COORD:
        case COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_COORD:
        case COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapRadarSprites, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_GANG_PED_MODEL_PREFERENCE:
        case COMMAND_SET_GANG_ATTACK_PLAYER_WITH_COPS:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            break;
        case COMMAND_PLAY_ANNOUNCEMENT:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAnnouncementTracks, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_CAR_DRIVE_TO_COORD:
            if (nArg == 6)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSplineModes, true);
                bFound = true;
            }
            else if (nArg == 7)
            {
                if ((strcmp(pszArg, "FALSE") != 0) && (strcmp(pszArg, "TRUE") != 0))
                {
                    nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                    bFound = true;
                }
            }
            else if (nArg == 8)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDrivingModes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_BEAT_ZONE_SIZE:
            break;
        case COMMAND_TASK_CAR_DRIVE_WANDER:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDrivingModes, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_FOLLOW_POINT_ROUTE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedMoveStates, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFollowRouteModes, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CHAR_AT_ATTRACTOR:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            else if (nArg == 3)
            {
                nValue = GetCommandIdFromName(pszArg, loc);
                bFound = true;
            }
            break;
        case COMMAND_GET_SCRIPT_TASK_STATUS:
            if (nArg == 1)
            {
                nValue = GetCommandIdFromName(pszArg, loc);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_GROUP:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultTaskAllocators, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_KILL_CHAR_ON_FOOT_WHILE_DUCKING:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDuckingTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_PED_GENERATOR:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_PEDMODEL_AS_ATTRACTOR_USER:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_SAY:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGlobalSpeechContexts, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_PEDTYPE_AS_ATTRACTOR_USER:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_CAMERA_TO_VEHICLE:
        case COMMAND_ATTACH_CAMERA_TO_CHAR:
        case COMMAND_ATTACH_CAMERA_TO_OBJECT:
            if (nArg == 8)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraSwitchModes, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_VEHICLE:
        case COMMAND_ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_CHAR:
        case COMMAND_ATTACH_CAMERA_TO_CHAR_LOOK_AT_VEHICLE:
        case COMMAND_ATTACH_CAMERA_TO_CHAR_LOOK_AT_CHAR:
        case COMMAND_ATTACH_CAMERA_TO_OBJECT_LOOK_AT_VEHICLE:
        case COMMAND_ATTACH_CAMERA_TO_OBJECT_LOOK_AT_CHAR:
        case COMMAND_ATTACH_CAMERA_TO_OBJECT_LOOK_AT_OBJECT:
        case COMMAND_ATTACH_CAMERA_TO_CHAR_LOOK_AT_OBJECT:
        case COMMAND_ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_OBJECT:
            if (nArg == 6)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraSwitchModes, true);
                bFound = true;
            }
            break;
        case COMMAND_FORCE_CAR_LIGHTS:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarLightOverrides, true);
                bFound = true;
            }
            break;
        case COMMAND_OPEN_CAR_DOOR:
        case COMMAND_POP_CAR_DOOR:
        case COMMAND_FIX_CAR_DOOR:
        case COMMAND_DAMAGE_CAR_DOOR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarDoors, true);
                bFound = true;
            }
            break;
        case COMMAND_POP_CAR_PANEL:
        case COMMAND_FIX_CAR_PANEL:
        case COMMAND_DAMAGE_CAR_PANEL:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarPanels, true);
                bFound = true;
            }
            break;
        case COMMAND_FIX_CAR_TYRE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarWheels, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_KILL_THREATS_ON_FOOT_WHILE_DUCKING:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDuckingTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_CHAR_TO_BIKE:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFacingDirections, true);
                bFound = true;
            }
            else if (nArg == 8)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_CAR_TEMP_ACTION:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapTempActions, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CHECKPOINT:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCheckpointTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_TWO_PLAYER_CAMERA_MODE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraSwitchModes, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_CAR_MISSION:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarMissions, true);
                bFound = true;
            }
            else if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDrivingModes, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_VEHICLE_MOD:
        case COMMAND_REMOVE_VEHICLE_MOD:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_CREATE_SCRIPT_GANG_ROADBLOCK:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            break;
        case COMMAND_CLEAR_CHAR_DECISION_MAKER_EVENT_RESPONSE:
        case COMMAND_IS_CHAR_RESPONDING_TO_EVENT:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedEventChoices, true);
                bFound = true;
            }
            break;
        case COMMAND_CLEAR_GROUP_DECISION_MAKER_EVENT_RESPONSE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGroupEventChoices, true);
                bFound = true;
            }
            break;
        case COMMAND_ADD_CHAR_DECISION_MAKER_EVENT_RESPONSE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedEventChoices, true);
                if (nValue == -1)
                {
                    ReportCompilationError(
                        "ADD_DECISION_MAKER_EVENT_RESPONSE - Event name should be taken from the list of events", loc);
                    return -1;
                }
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTaskChoices, true);
                if (nValue == -1)
                {
                    ReportCompilationError(
                        "ADD_DECISION_MAKER_EVENT_RESPONSE - Task name should be taken from the list of tasks", loc);
                    return -1;
                }
                int nEvent = GetArgIntValueAtLOC(loc, nCmd, 1, true);
                if (!IsValidTaskForEvent(nEvent, nValue, false))
                {
                    ReportCompilationError(
                        "ADD_DECISION_MAKER_EVENT_RESPONSE - Can't use this task to respond to this event", loc);
                    return -1;
                }
                bFound = true;
            }
            break;
        case COMMAND_ADD_GROUP_DECISION_MAKER_EVENT_RESPONSE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGroupEventChoices, true);
                if (nValue == -1)
                {
                    ReportCompilationError(
                        "ADD_DECISION_MAKER_EVENT_RESPONSE - Event name should be taken from the list of events", loc);
                    return -1;
                }
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGroupTaskChoices, true);
                if (nValue == -1)
                {
                    ReportCompilationError(
                        "ADD_DECISION_MAKER_EVENT_RESPONSE - Task name should be taken from the list of tasks", loc);
                    return -1;
                }
                int nEvent = GetArgIntValueAtLOC(loc, nCmd, 1, true);
                if (!IsValidTaskForEvent(nEvent, nValue, true))
                {
                    ReportCompilationError(
                        "ADD_DECISION_MAKER_EVENT_RESPONSE - Can't use this task to respond to this event", loc);
                    return -1;
                }
                bFound = true;
            }
            break;
        case COMMAND_TASK_DRIVE_BY:
            if (nArg == 7)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDriveByTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_PS2_KEYBOARD_KEY_PRESSED:
        case COMMAND_IS_PS2_KEYBOARD_KEY_JUST_PRESSED:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapKeyboardKeys, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_ZONE_RADAR_COLOURS:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapRadarModes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_STAT_CHANGE_AMOUNT:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapStatIncrements, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_RELATIONSHIP:
        case COMMAND_CLEAR_RELATIONSHIP:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAcquaintanceTypes, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_CLEAR_ALL_RELATIONSHIPS:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAcquaintanceTypes, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_GO_ON_PATROL:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFollowRouteModes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_ZONE_POPULATION_TYPE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPopcycleZones, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_ZONE_GANG_STRENGTH:
        case COMMAND_GET_ZONE_GANG_STRENGTH:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGangs, true);
                bFound = true;
            }
            break;
        case COMMAND_CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarPlateDesigns, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_DECISION_MAKER:
        case COMMAND_SET_GROUP_DECISION_MAKER:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDecisionMakerPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_LOAD_GROUP_DECISION_MAKER:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDecisionMakerGroupTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_RELATIONSHIP:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAcquaintanceTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_RELATIONSHIP_SET:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAcquaintanceTypes, true);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_CLEAR_ALL_CHAR_RELATIONSHIPS:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAcquaintanceTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_WEAPONTYPE_MODEL:
        case COMMAND_GET_WEAPONTYPE_SLOT:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_WINCH_TO_HELI:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWinchTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_NEXT_DESIRED_MOVE_STATE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedMoveStates, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_GROUP_DEFAULT_TASK_ALLOCATOR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultTaskAllocators, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_VAR_INT:
        case COMMAND_SET_LVAR_INT:
        case COMMAND_IS_INT_VAR_GREATER_THAN_NUMBER:
        case COMMAND_IS_INT_LVAR_GREATER_THAN_NUMBER:
        case COMMAND_IS_NUMBER_GREATER_THAN_INT_VAR:
        case COMMAND_IS_NUMBER_GREATER_THAN_INT_LVAR:
        case COMMAND_IS_INT_VAR_GREATER_THAN_INT_VAR:
        case COMMAND_IS_INT_LVAR_GREATER_THAN_INT_LVAR:
        case COMMAND_IS_INT_VAR_GREATER_THAN_INT_LVAR:
        case COMMAND_IS_INT_LVAR_GREATER_THAN_INT_VAR:
        case COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_NUMBER:
        case COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_NUMBER:
        case COMMAND_IS_NUMBER_GREATER_OR_EQUAL_TO_INT_VAR:
        case COMMAND_IS_NUMBER_GREATER_OR_EQUAL_TO_INT_LVAR:
        case COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_INT_VAR:
        case COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_INT_LVAR:
        case COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_INT_LVAR:
        case COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_INT_VAR:
        case COMMAND_IS_INT_VAR_EQUAL_TO_NUMBER:
        case COMMAND_IS_INT_LVAR_EQUAL_TO_NUMBER:
        case COMMAND_IS_INT_VAR_EQUAL_TO_INT_VAR:
        case COMMAND_IS_INT_LVAR_EQUAL_TO_INT_LVAR:
        case COMMAND_IS_INT_VAR_EQUAL_TO_INT_LVAR:
        case COMMAND_IS_INT_VAR_NOT_EQUAL_TO_NUMBER:
        case COMMAND_IS_INT_LVAR_NOT_EQUAL_TO_NUMBER:
        case COMMAND_IS_INT_VAR_NOT_EQUAL_TO_INT_VAR:
        case COMMAND_IS_INT_LVAR_NOT_EQUAL_TO_INT_LVAR:
        case COMMAND_IS_INT_VAR_NOT_EQUAL_TO_INT_LVAR:
        case COMMAND_SET_VAR_INT_TO_VAR_INT:
        case COMMAND_SET_LVAR_INT_TO_LVAR_INT:
        case COMMAND_SET_VAR_INT_TO_LVAR_INT:
        case COMMAND_SET_LVAR_INT_TO_VAR_INT:
        case COMMAND_IS_INT_VAR_EQUAL_TO_CONSTANT:
        case COMMAND_IS_INT_LVAR_EQUAL_TO_CONSTANT:
        case COMMAND_SET_VAR_INT_TO_CONSTANT:
        case COMMAND_SET_LVAR_INT_TO_CONSTANT:
        case COMMAND_IS_INT_VAR_GREATER_THAN_CONSTANT:
        case COMMAND_IS_INT_LVAR_GREATER_THAN_CONSTANT:
        case COMMAND_IS_CONSTANT_GREATER_THAN_INT_VAR:
        case COMMAND_IS_CONSTANT_GREATER_THAN_INT_LVAR:
        case COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_CONSTANT:
        case COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_CONSTANT:
        case COMMAND_IS_CONSTANT_GREATER_OR_EQUAL_TO_INT_VAR:
        case COMMAND_IS_CONSTANT_GREATER_OR_EQUAL_TO_INT_LVAR:
        case COMMAND_SET_VAR_TEXT_LABEL:
        case COMMAND_SET_LVAR_TEXT_LABEL:
        case COMMAND_IS_VAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL:
        case COMMAND_IS_LVAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL:
        case COMMAND_SET_VAR_TEXT_LABEL16:
        case COMMAND_SET_LVAR_TEXT_LABEL16:
        case COMMAND_IS_INT_LVAR_EQUAL_TO_INT_VAR:
        case COMMAND_IS_INT_LVAR_NOT_EQUAL_TO_INT_VAR:
            nValue = GetValueFromMap(pszArg, &g_mapObjectModels, true);
            if (nValue != -1)
            {
                nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                bModel = true;
            }
            bNegate = bModel;
            bFound = bModel;
            break;
        case COMMAND_DISPLAY_PLAYBACK_RECORDED_CAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPlaybackDisplayModes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_TWO_PLAYER_CAM_MODE_SEPARATE_CARS:
        case COMMAND_SET_TWO_PLAYER_CAM_MODE_SAME_CAR_SHOOTING:
        case COMMAND_SET_TWO_PLAYER_CAM_MODE_SAME_CAR_NO_SHOOTING:
        case COMMAND_SET_TWO_PLAYER_CAM_MODE_NOT_BOTH_IN_CAR:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCameraModes, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_DRIVE_POINT_ROUTE_ADVANCED:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSplineModes, true);
                bFound = true;
            }
            else if (nArg == 4)
            {
                if ((strcmp(pszArg, "FALSE") != 0) && (strcmp(pszArg, "TRUE") != 0))
                {
                    nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                    bFound = true;
                }
            }
            else if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDrivingModes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE:
        case COMMAND_GET_CLOSEST_OBJECT_OF_TYPE:
        case COMMAND_IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED:
        case COMMAND_SET_CLOSEST_OBJECT_OF_TYPE_RENDER_SCORCHED:
        case COMMAND_HAS_OBJECT_OF_TYPE_BEEN_SMASHED:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_GIVE_MELEE_ATTACK_TO_CHAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapMeleeCombos, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_WEAPON_SKILL:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponSkills, true);
                bFound = true;
            }
            break;
        case COMMAND_FIND_NEAREST_MULTIBUILDING:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBuyableTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SELECT_WEAPONS_FOR_VEHICLE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSelectedWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_USE_ATTRACTOR_ADVANCED:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedMoveStates, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_PROCEDURAL_INTERIOR_ACTIVE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapInteriorTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_ZONE_POPULATION_RACE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPopRaces, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_BIRDS:
            if (nArg == 7)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBirdTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_GIVE_PLAYER_CLOTHES:
        case COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapClothesTextures, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_OBJECT:
        case COMMAND_REQUEST_MODEL:
        case COMMAND_HAS_MODEL_LOADED:
        case COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED:
        case COMMAND_CREATE_OBJECT_NO_OFFSET:
        case COMMAND_GET_VEHICLE_MOD_TYPE:
        case COMMAND_REQUEST_VEHICLE_MOD:
        case COMMAND_HAS_VEHICLE_MOD_LOADED:
        case COMMAND_MARK_VEHICLE_MOD_AS_NO_LONGER_NEEDED:
        case COMMAND_IS_MODEL_IN_CDIMAGE:
        case COMMAND_GET_MODEL_DIMENSIONS:
        case COMMAND_GET_MODEL_NAME_FOR_DEBUG_ONLY:
        case COMMAND_ATTACH_ANIMS_TO_MODEL:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_ADD_EXPLOSION:
        case COMMAND_ADD_EXPLOSION_NO_SOUND:
        case COMMAND_ADD_EXPLOSION_VARIABLE_SHAKE:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapExplosionTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON:
        case COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON:
        case COMMAND_HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_COPY_CHAR_DECISION_MAKER:
        case COMMAND_COPY_GROUP_DECISION_MAKER:
        case COMMAND_COPY_SHARED_CHAR_DECISION_MAKER:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDecisionMakerPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_LOAD_CHAR_DECISION_MAKER:
        case COMMAND_LOAD_SHARED_CHAR_DECISION_MAKER:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDecisionMakerMissionPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_ATTACH_FX_SYSTEM_TO_CHAR_BONE:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapBones, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_HEADING_LIMIT_FOR_ATTACHED_CHAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFacingDirections, true);
                bFound = true;
            }
            break;
        case COMMAND_DRAW_WINDOW_TEXT:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFontOrientations, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_USE_CLOSEST_MAP_ATTRACTOR:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_REGISTER_SCRIPT_BRAIN_FOR_CODE_USE:
        case COMMAND_REGISTER_OBJECT_SCRIPT_BRAIN_FOR_CODE_USE:
        case COMMAND_REGISTER_ATTRACTOR_SCRIPT_BRAIN_FOR_CODE_USE:
        case COMMAND_STREAM_SCRIPT:
        case COMMAND_HAS_STREAMED_SCRIPT_LOADED:
        case COMMAND_MARK_STREAMED_SCRIPT_AS_NO_LONGER_NEEDED:
        case COMMAND_REMOVE_STREAMED_SCRIPT:
        case COMMAND_START_NEW_STREAMED_SCRIPT:
        case COMMAND_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT:
        case COMMAND_ALLOCATE_STREAMED_SCRIPT_TO_PED_GENERATOR:
            if (nArg == 0)
            {
                nValue = GetStreamedScriptIndexByName(pszArg);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_MENU:
        case COMMAND_CREATE_MENU_GRID:
            if (nArg == 7)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFontOrientations, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_MENU_COLUMN_ORIENTATION:
            if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFontOrientations, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_CAN_BE_KNOCKED_OFF_BIKE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapKnockOffBikeTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_CLOTHES_ITEM:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapClothesTextures, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_COORD_BLIP_APPEARANCE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCoordBlipAppearances, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_HUD_COLOUR:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHudColours, true);
                bFound = true;
            }
            break;
        case COMMAND_ALLOCATE_STREAMED_SCRIPT_TO_RANDOM_PED:
            if (nArg == 0)
            {
                nValue = GetStreamedScriptIndexByName(pszArg);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_ALLOCATE_STREAMED_SCRIPT_TO_OBJECT:
            if (nArg == 0)
            {
                nValue = GetStreamedScriptIndexByName(pszArg);
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_SET_ONSCREEN_COUNTER_COLOUR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHudColours, true);
                bFound = true;
            }
            break;
        case COMMAND_CONTROL_CAR_DOOR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarDoors, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDoorDamageStates, true);
                bFound = true;
            }
            break;
        case COMMAND_DRAW_WINDOW:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSwirlTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_CURRENT_CAR_MOD:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapVehicleUpgrades, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CAR:
        case COMMAND_CREATE_GANG_CAR:
        case COMMAND_CUSTOM_PLATE_FOR_NEXT_CAR:
        case COMMAND_SUPPRESS_CAR_MODEL:
        case COMMAND_DONT_SUPPRESS_CAR_MODEL:
        case COMMAND_IS_THIS_MODEL_A_BOAT:
        case COMMAND_IS_THIS_MODEL_A_PLANE:
        case COMMAND_IS_THIS_MODEL_A_HELI:
        case COMMAND_CREATE_EMERGENCY_SERVICES_CAR:
        case COMMAND_IS_THIS_MODEL_A_TRAIN:
        case COMMAND_FORCE_RANDOM_CAR_MODEL:
        case COMMAND_GET_CAR_MODEL_VALUE:
        case COMMAND_SET_VEHICLE_CAMERA_TWEAK:
        case COMMAND_IS_THIS_MODEL_A_BIKE:
        case COMMAND_IS_THIS_MODEL_A_CAR:
        case COMMAND_BLOCK_VEHICLE_MODEL:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_CREATE_CAR_GENERATOR:
        case COMMAND_CREATE_CAR_GENERATOR_WITH_PLATE:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            else if (nArg == 5 || nArg == 6)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarColours, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_GO_STRAIGHT_TO_COORD:
        case COMMAND_TASK_FOLLOW_PATH_NODES_TO_COORD:
        case COMMAND_TASK_GO_TO_COORD_ANY_MEANS:
        case COMMAND_TASK_GO_TO_COORD_WHILE_SHOOTING:
        case COMMAND_TASK_GO_TO_COORD_WHILE_AIMING:
        case COMMAND_TASK_FOLLOW_PATH_NODES_TO_COORD_SHOOTING:
        case COMMAND_TASK_FOLLOW_PATH_NODES_TO_COORD_WITH_RADIUS:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedMoveStates, true);
                bFound = true;
            }
            break;
        case COMMAND_REGISTER_FASTEST_TIME:
        case COMMAND_INCREMENT_INT_STAT:
        case COMMAND_INCREMENT_FLOAT_STAT:
        case COMMAND_DECREMENT_INT_STAT:
        case COMMAND_DECREMENT_FLOAT_STAT:
        case COMMAND_REGISTER_INT_STAT:
        case COMMAND_REGISTER_FLOAT_STAT:
        case COMMAND_SET_INT_STAT:
        case COMMAND_SET_FLOAT_STAT:
        case COMMAND_GET_INT_STAT:
        case COMMAND_GET_FLOAT_STAT:
        case COMMAND_INCREMENT_INT_STAT_NO_MESSAGE:
        case COMMAND_INCREMENT_FLOAT_STAT_NO_MESSAGE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapStats, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_PICK_UP_OBJECT:
        case COMMAND_TASK_PICK_UP_SECOND_OBJECT:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedBodyParts, true);
                bFound = true;
            }
            else if (nArg == 6)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHoldOrientateTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_NAMED_ENTRY_EXIT_FLAG:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapEntryExitFlags, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_UP_CONVERSATION_NODE_WITH_SPEECH:
            if (nArg == 3 || nArg == 4 || nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGlobalSpeechContexts, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_PLAYER_IN_CAR_CAMERA_MODE:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapZoomLevels, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CLOSEST_ENTRY_EXIT_FLAG:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapEntryExitFlags, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE:
            if (nArg == 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_UP_CONVERSATION_NODE_WITH_SCRIPTED_SPEECH:
            if (nArg >= 3 && nArg <= 5)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSoundEffects, true);
                if (nValue < 0 && strncmp(pszArg, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                {
                    nValue = MISSING_SOUND_ID;
                }
                bFound = true;
            }
            break;
        case COMMAND_TASK_HAND_GESTURE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHandSignalAnims, true);
                bFound = true;
            }
            break;
        case COMMAND_LOAD_MISSION_AUDIO:
        case COMMAND_SET_CHAR_SAY:
        case COMMAND_REPORT_MISSION_AUDIO_EVENT_AT_OBJECT:
        case COMMAND_SET_CHAR_SAY_SCRIPT:
        case COMMAND_REPORT_MISSION_AUDIO_EVENT_AT_CHAR:
        case COMMAND_REPORT_MISSION_AUDIO_EVENT_AT_CAR:
        case COMMAND_SET_UP_CONVERSATION_END_NODE_WITH_SCRIPTED_SPEECH:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSoundEffects, true);
                if (nValue < 0 && strncmp(pszArg, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                {
                    nValue = MISSING_SOUND_ID;
                }
                bFound = true;
            }
            break;
        case COMMAND_CREATE_USER_3D_MARKER:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHudColours, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_WIDGET_PRESSED:
        case COMMAND_IS_WIDGET_RELEASED:
        case COMMAND_IS_WIDGET_DOUBLETAPPED:
        case COMMAND_IS_WIDGET_SWIPED:
        case COMMAND_IS_WIDGET_SWIPED_LEFT:
        case COMMAND_IS_WIDGET_SWIPED_RIGHT:
        case COMMAND_GET_WIDGET_VALUE:
        case COMMAND_GET_WIDGET_VALUE2:
        case COMMAND_GET_WIDGET_POSITION:
        case COMMAND_SET_WIDGET_VALUE:
        case COMMAND_SET_SLIDER_RANGE:
        case COMMAND_ADD_WIDGET_FLAG:
        case COMMAND_REMOVE_WIDGET_FLAG:
        case COMMAND_ADD_BUTTON_FLAG:
        case COMMAND_REMOVE_BUTTON_FLAG:
        case COMMAND_IS_TOUCH_ENABLED:
        case COMMAND_CREATE_SHOP_WIDGET:
        case COMMAND_ADD_SHOP_ITEM:
        case COMMAND_DELETE_WIDGET:
        case COMMAND_SET_EQUIPPED_ITEM:
        case COMMAND_SET_WIDGET_TEXTURE:
        case COMMAND_SET_WIDGET_VALUE3:
        case COMMAND_SET_WIDGET_VALUE2:
        case COMMAND_SET_WIDGET_INFO:
        case COMMAND_SET_WIDGET_INFO2:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWidgetIds, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_HID_PRESSED:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHIDMapping, true);
                bFound = true;
            }
            break;
        case COMMAND_HID_IMPLEMENTS:
        case COMMAND_IS_HID_RELEASED:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapHIDMapping, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_CAR_MODEL:
        case COMMAND_GET_NUM_OF_MODELS_KILLED_BY_PLAYER:
        case COMMAND_IS_CHAR_MODEL:
        case COMMAND_CREATE_CUTSCENE_HEAD:
        case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_ZONE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_POSITION_OF_ANALOGUE_STICKS:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPads, true);
                bFound = true;
            }
            break;
        case COMMAND_IS_CURRENT_PLAYER_WEAPON:
        case COMMAND_IS_CURRENT_CHAR_WEAPON:
        case COMMAND_GET_AMMO_IN_PLAYER_WEAPON:
        case COMMAND_GET_AMMO_IN_CHAR_WEAPON:
        case COMMAND_HAS_PLAYER_GOT_WEAPON:
        case COMMAND_HAS_CHAR_GOT_WEAPON:
        case COMMAND_REMOVE_WEAPON_FROM_CHAR:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapWeaponTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_REPORT_MISSION_AUDIO_EVENT_AT_POSITION:
            if (nArg == 3)
            {
                nValue = GetValueFromMap(pszArg, &g_mapSoundEffects, true);
                if (nValue < 0 && strncmp(pszArg, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
                {
                    nValue = MISSING_SOUND_ID;
                }
                bFound = true;
            }
            break;
        case COMMAND_CREATE_PICKUP_WITH_AMMO:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            else if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPickupTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_NUMBER_OF_SEATS_IN_MODEL:
        case COMMAND_IS_MODEL_AVAILABLE:
        case COMMAND_SET_CAR_MODEL_COMPONENTS:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_LOAD_COLLISION_WITH_SCREEN:
            if (nArg == 0)
            {
                nValue = GetValueFromMap(pszArg, &g_mapLevels, true);
                bFound = true;
            }
            break;
        case COMMAND_TASK_FOLLOW_PATROL_ROUTE:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedMoveStates, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapFollowRouteModes, true);
                bFound = true;
            }
            break;
        case COMMAND_GET_RANDOM_CAR_IN_SPHERE:
        case COMMAND_GET_RANDOM_CAR_IN_SPHERE_NO_SAVE:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_ALLOCATE_SCRIPT_TO_RANDOM_PED:
        case COMMAND_SET_PLAYER_MODEL:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_CHAR_SAY_CONTEXT:
        case COMMAND_SET_UP_CONVERSATION_END_NODE_WITH_SPEECH:
        case COMMAND_RANDOM_PASSENGER_SAY:
        case COMMAND_SET_CHAR_SAY_CONTEXT_IMPORTANT:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapGlobalSpeechContexts, true);
                bFound = true;
            }
            break;
        case COMMAND_OPEN_CAR_DOOR_A_BIT:
        case COMMAND_IS_CAR_DOOR_FULLY_OPEN:
        case COMMAND_GET_DOOR_ANGLE_RATIO:
        case COMMAND_IS_CAR_DOOR_DAMAGED:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapCarDoors, true);
                bFound = true;
            }
            break;
        case COMMAND_ALLOCATE_SCRIPT_TO_OBJECT:
        case COMMAND_DOES_OBJECT_HAVE_THIS_MODEL:
        case COMMAND_IS_LAST_BUILDING_MODEL_SHOT_BY_PLAYER:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
        case COMMAND_CLEAR_CHAR_RELATIONSHIP:
            if (nArg == 1)
            {
                nValue = GetValueFromMap(pszArg, &g_mapAcquaintanceTypes, true);
                bFound = true;
            }
            else if (nArg == 2)
            {
                nValue = GetValueFromMap(pszArg, &g_mapPedTypes, true);
                bFound = true;
            }
            break;
        case COMMAND_SET_USES_COLLISION_OF_CLOSEST_OBJECT_OF_TYPE:
        case COMMAND_GET_USER_OF_CLOSEST_MAP_ATTRACTOR:
        case COMMAND_SET_CHAR_USES_COLLISION_CLOSEST_OBJECT_OF_TYPE:
            if (nArg == 4)
            {
                nValue = GetValueFromMap(pszArg, &g_mapDefaultModels, true);
                if (nValue == -1 && GetValueFromMap(pszArg, &g_mapObjectModels, true) != -1)
                {
                    nValue = static_cast<int16_t>(GetModelNameIndex(pszArg));
                    bNegate = true;
                }
                bFound = true;
            }
            break;
    }
    if (!bFound)
    {
        if ((strcmp(pszArg, "ON") == 0) || (strcmp(pszArg, "TRUE") == 0) || (strcmp(pszArg, "DAY") == 0))
        {
            nValue = 1;
        }
        else if ((strcmp(pszArg, "OFF") == 0) || (strcmp(pszArg, "FALSE") == 0) || (strcmp(pszArg, "NIGHT") == 0))
        {
            nValue = 0;
        }
        else
        {
            return 0;
        }
    }
    if (nValue == -1)
    {
        if (bReportErrors)
        {
            ReportCompilationError("**** ARGUMENT NOT RECOGNISED AS A STRING CONSTANT", loc);
            return -1;
        }
        return 0;
    }
    if (bNegate)
    {
        nValue = -nValue;
    }
    SetArgIntValueAtLOC(loc, nCmd, nArg, nValue, false);
    SetArgTypeAtLOC(loc, nCmd, nArg, CARG_INT, false);
    return 0;
}

// 0x0041ADC0 ?FindFileRecursively@@YADPBD0PAD@Z
bool FindFileRecursively(const char* pszDirectory, const char* pszFileName, char* pszFoundPath)
{
    WIN32_FIND_DATAA findData;
    char szSubDirectory[1024];
    char szPath[1024];
    sprintf(szPath, "%s\\%s", pszDirectory, pszFileName);
    HANDLE hFind = FindFirstFileA(szPath, &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        strcpy(pszFoundPath, szPath);
        FindClose(hFind);
        return true;
    }
    FindClose(hFind);
    sprintf(szPath, "%s\\*.*", pszDirectory);
    hFind = FindFirstFileA(szPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    do
    {
        if (((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0u) && (strcmp(findData.cFileName, ".") != 0) &&
            (strcmp(findData.cFileName, "..") != 0))
        {
            sprintf(szSubDirectory, "%s\\%s", pszDirectory, findData.cFileName);
            if (FindFileRecursively(szSubDirectory, pszFileName, pszFoundPath))
            {
                FindClose(hFind);
                return true;
            }
        }
    } while (FindNextFileA(hFind, &findData) != 0);
    FindClose(hFind);
    return false;
}

// 0x0041AF80 ?ParseArguments@@YAHHGFPADD@Z
int16_t ParseArguments(int loc, uint16_t nCmd, int16_t nCommand, const char* pszArgs, bool bReportErrors)
{
    char szArg[128];
    char szArgs[1024];
    int nValue;
    float fValue;
    if (nCommand < 0)
    {
        return -1;
    }
    strcpy(szArgs, pszArgs);
    FreeArgsAtLOC(loc, nCmd);
    int16_t nArg = 0;
    while (sscanf(szArgs, "%s", szArg) > 0)
    {
        int16_t nType = GetArgumentTypeFromString(szArg, loc, nCmd, nArg);
        SetArgTypeAtLOC(loc, nCmd, nArg, static_cast<eCompiledArgType>(nType), true);
        nValue = 0;
        int16_t nCurCommand = GetCommandAtLOC(loc, nCmd, true);
        if (GetArgTypeAtLOC(loc, nCmd, nArg, false) == CARG_NAME &&
            g_aCommandInfo[nCurCommand].aArgTypes[nArg] != ARG_TEXT_LABEL)
        {
            if (static_cast<int>(
                    FindInMap(GetArgStringAtLOC(loc, nCmd, nArg, true), &g_mapIntConstants, &nValue, true)) == 1)
            {
                char* pszConstant = GetArgStringAtLOC(loc, nCmd, nArg, true);
                strstr(pszConstant, "DATE_IN_PROGRESS");
                RecordConstantUsage(pszConstant, "CONST", nValue);
                SetArgIntValueAtLOC(loc, nCmd, nArg, nValue, false);
                SetArgTypeAtLOC(loc, nCmd, nArg, CARG_INT, false);
                goto constant;
            }
            if (static_cast<int>(
                    FindInFloatMap(GetArgStringAtLOC(loc, nCmd, nArg, true), &g_mapFloatConstants, &fValue)) == 1)
            {
                SetArgFloatValueAtLOC(loc, nCmd, nArg, fValue, false);
                SetArgTypeAtLOC(loc, nCmd, nArg, CARG_FLOAT, false);
            constant:
                RecordConstantUsage(pszArgs, "", nCommand);
                nType = GetArgTypeAtLOC(loc, nCmd, nArg, true);
                goto check;
            }
        }
        if (nCommand == COMMAND_CASE && nArg == 0 && GetArgTypeAtLOC(loc, nCmd, 0, true) == CARG_NAME)
        {
            int nEnum = GetValueFromMap(GetArgStringAtLOC(loc, nCmd, 0, true), &g_mapAllEnumConstants, true);
            if (nEnum >= 0)
            {
                SetArgIntValueAtLOC(loc, nCmd, 0, nEnum, false);
                SetArgTypeAtLOC(loc, nCmd, 0, CARG_INT, false);
                nType = CARG_INT;
            }
        }
    check:
        eArgType nExpected = g_aCommandInfo[nCommand].aArgTypes[nArg];
        switch (nType)
        {
            case CARG_INT:
                if (nExpected != ARG_INT_CONST && nExpected != ARG_SCRIPT_PARAM && nExpected != ARG_INT &&
                    nExpected != ARG_TEXT_LABEL && nExpected != ARG_STRING)
                {
                    if (bReportErrors)
                    {
                        ReportCompilationError("**** ARGUMENT MISMATCH", loc);
                    }
                    return -1;
                }
                break;
            case CARG_FLOAT:
                if (nExpected != ARG_FLOAT_CONST && nExpected != ARG_SCRIPT_PARAM && nExpected != ARG_FLOAT)
                {
                    if (bReportErrors)
                    {
                        ReportCompilationError("**** ARGUMENT MISMATCH", loc);
                    }
                    return -1;
                }
                break;
            case CARG_NAME:
                if (nExpected != ARG_VAR_INT && nExpected != ARG_VAR_FLOAT && nExpected != ARG_DECLARE_VAR_INT &&
                    nExpected != ARG_DECLARE_VAR_FLOAT && nExpected != ARG_LVAR_INT && nExpected != ARG_LVAR_FLOAT &&
                    nExpected != ARG_DECLARE_LVAR_INT && nExpected != ARG_DECLARE_LVAR_FLOAT &&
                    nExpected != ARG_LABEL && nExpected != ARG_SCRIPT_PARAM && nExpected != ARG_FLOAT &&
                    nExpected != ARG_INT && nExpected != ARG_RETURN_FLOAT && nExpected != ARG_RETURN_INT &&
                    nExpected != ARG_TEXT_LABEL && nExpected != ARG_VAR_TEXT_LABEL &&
                    nExpected != ARG_LVAR_TEXT_LABEL && nExpected != ARG_DECLARE_VAR_TEXT_LABEL &&
                    nExpected != ARG_DECLARE_LVAR_TEXT_LABEL && nExpected != ARG_STRING &&
                    nExpected != ARG_TEXT_LABEL16 && nExpected != ARG_VAR_TEXT_LABEL16 &&
                    nExpected != ARG_LVAR_TEXT_LABEL16 && nExpected != ARG_DECLARE_VAR_TEXT_LABEL16 &&
                    nExpected != ARG_DECLARE_LVAR_TEXT_LABEL16)
                {
                    if (bReportErrors)
                    {
                        ReportCompilationError("**** ARGUMENT MISMATCH", loc);
                    }
                    return -1;
                }
                if (HandleStringConstantArgument(loc, nCmd, nArg, nCommand, szArg, false) < 0)
                {
                    return -1;
                }
                break;
            default:
                if (nType != nExpected)
                {
                    if (bReportErrors)
                    {
                        ReportCompilationError("**** ARGUMENT MISMATCH", loc);
                    }
                    return -1;
                }
                break;
        }
        nArg++;
        RemoveFirstWord(szArgs);
    }
    if (nArg >= MAX_ARGS)
    {
        return 0;
    }
    eArgType nExpected = g_aCommandInfo[nCommand].aArgTypes[nArg];
    if (nExpected != ARG_NONE && nExpected != ARG_DECLARE_VAR_INT && nExpected != ARG_DECLARE_VAR_FLOAT &&
        nExpected != ARG_DECLARE_LVAR_INT && nExpected != ARG_DECLARE_LVAR_FLOAT && nExpected != ARG_SCRIPT_PARAM &&
        nExpected != ARG_DECLARE_VAR_TEXT_LABEL && nExpected != ARG_DECLARE_LVAR_TEXT_LABEL &&
        nExpected != ARG_DECLARE_VAR_TEXT_LABEL16 && nExpected != ARG_DECLARE_LVAR_TEXT_LABEL16)
    {
        if (bReportErrors)
        {
            ReportCompilationError("**** WRONG NUMBER OF ARGUMENTS", loc);
        }
        return -1;
    }
    return 0;
}

// 0x0041B390 ?BuildCommandList@@YAXPAD@Z
void BuildCommandList(const char* pszFile)
{
    FILE* pFile = fopen(pszFile, "w");
    fprintf(pFile, "#ifndef _COMMANDLIST_H_\n");
    fprintf(pFile, "#define _COMMANDLIST_H_\n\n");
    fprintf(pFile, "struct commandList {\n");
    fprintf(pFile, "int number;\n");
    fprintf(pFile, "const char* name;\n");
    fprintf(pFile, "};\n");
    fprintf(pFile, "commandList CommandList[] = {\n");
    fprintf(pFile, "\n");
    for (int i = 0; i < ARRAY_SIZE(g_aCommandInfo); i++)
    {
        fprintf(pFile, "\t{%d,\"%s\"},\n", i, g_aCommandInfo[i].szName);
    }
    fprintf(pFile, "};\n");
    fprintf(pFile, "\n#endif\n\n");
    fclose(pFile);
}

// 0x0041B450 ?WriteProgressListFile@@YAXXZ
void WriteProgressListFile()
{
    FILE* pFile = fopen("progress_list.txt", "w");
    if (pFile == nullptr)
    {
        PrintMessage("**** Couldn't open text file containing the list of script files containing PROGRESS - check it "
                     "is writable");
    }
    else
    {
        for (uint16_t i = 0; i < g_nNumProgressScripts; i++)
        {
            fprintf(pFile, "%s\n", g_aProgressScripts[i]);
        }
        fclose(pFile);
    }
}

// 0x0041B4D0 ?WriteMissingSoundEffectsFile@@YAXXZ
void WriteMissingSoundEffectsFile()
{
    FILE* pFile = fopen("missing_sound_effects.txt", "w");
    if (pFile == nullptr)
    {
        PrintMessage(
            "**** Couldn't open text file containing the list of missing sound effects - check it is writable");
    }
    else
    {
        for (uint16_t i = 0; i < g_nNumMissingSoundEffects; i++)
        {
            fprintf(pFile, "%s\n", g_aMissingSoundEffects[i]);
        }
        fclose(pFile);
    }
}

// 0x0041B540 ?WriteUnusedSoundEffectsFiles@@YAXXZ
void WriteUnusedSoundEffectsFiles()
{
    std::map<std::string, int>::iterator itEnd = g_mapSoundEffects.end();
    for (std::map<std::string, int>::iterator it = g_mapSoundEffects.begin(); it != itEnd; ++it)
    {
        uint16_t i = 0;
        bool bFound = false;
        while (i < g_nNumUsedSoundEffects && !bFound)
        {
            if (strcmp(it->first.c_str(), g_aUsedSoundEffects[i]) == 0)
            {
                bFound = true;
            }
            i++;
        }
        if (!bFound)
        {
            strncpy(g_aUnusedSoundEffects[g_nNumUnusedSoundEffects], it->first.c_str(), NAME_SIZE);
            g_nNumUnusedSoundEffects++;
        }
    }
    FILE* pFile = fopen("unused_sound_effects.txt", "w");
    if (pFile == nullptr)
    {
        PrintMessage("**** Couldn't open text file containing the list of unused sound effects - check it is writable");
    }
    else
    {
        FILE* pFileNoNumbers = fopen("unused_sound_effects_without_numbers.txt", "w");
        if (pFileNoNumbers == nullptr)
        {
            PrintMessage("**** Couldn't open text file containing the list of unused sound effects without numbers - "
                         "check it is writable");
        }
        else
        {
            for (uint16_t i = 0; i < g_nNumUnusedSoundEffects; i++)
            {
                fprintf(pFile, "%s\t\t%d\n", g_aUnusedSoundEffects[i],
                    GetValueFromMap(g_aUnusedSoundEffects[i], &g_mapSoundEffects, true));
                fprintf(pFileNoNumbers, "%s\n", g_aUnusedSoundEffects[i]);
            }
            fclose(pFile);
            fclose(pFileNoNumbers);
        }
    }
}
