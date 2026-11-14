// loader.cpp: 0x40F270-0x41061E in sc.exe

#include "common.h"
#include "loader.h"
#include "main.h"
#include "parser.h"

// 0x015870C0
tNamedId g_aDefaultModels[MAX_DEFAULT_MODELS];
// 0x016EB150
tNamedId g_aObjectModels[MAX_OBJECT_MODELS];
// 0x017E41B0
uint16_t g_nNumDefaultModels;
// 0x017E41B8
char g_aUsedModelNames[MAX_USED_MODEL_NAMES][MODEL_NAME_SIZE];
// 0x017E66C0
uint16_t g_nNumUsedModelNames;

// 0x01591020
tNamedId g_aPedEventChoices[MAX_EVENT_CHOICES];
// 0x01592010
tNamedId g_aPedTaskChoices[MAX_TASK_CHOICES];
// 0x01594298
tValidTasksForEvent g_aPedValidTasksForEvents[MAX_EVENT_CHOICES];
// 0x01598078
tNamedId g_aGroupEventChoices[MAX_EVENT_CHOICES];
// 0x01599068
tNamedId g_aGroupTaskChoices[MAX_TASK_CHOICES];
// 0x0159B2F0
tValidTasksForEvent g_aGroupValidTasksForEvents[MAX_EVENT_CHOICES];

// 0x0159F0D0
tNamedId g_aSoundNames[MAX_SOUND_NAMES];

// 0x0040F270 ?GetModelNameIndex@@YAHPBD@Z
int GetModelNameIndex(const char* pszModelName)
{
    int nIndex = 0;
    uint16_t i = 0;
    bool bFound = false;
    while (i < g_nNumUsedModelNames && !bFound)
    {
        if (strcmp(pszModelName, g_aUsedModelNames[nIndex]) == 0)
        {
            bFound = true;
        }
        else
        {
            i++;
        }
        nIndex = i;
    }
    if (!bFound)
    {
        strcpy(g_aUsedModelNames[g_nNumUsedModelNames], pszModelName);
        g_nNumUsedModelNames++;
        return g_nNumUsedModelNames - 1;
    }
    return nIndex;
}

// 0x0040F340 ?IsValidTaskForEvent@@YADGGD@Z
bool IsValidTaskForEvent(uint16_t nEvent, uint16_t nTask, bool bGroup)
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aPedValidTasksForEvents); i++)
    {
        if (!bGroup)
        {
            if (g_aPedValidTasksForEvents[i].nEvent == nEvent)
            {
                for (uint16_t j = 0; j < ARRAY_SIZE(g_aPedValidTasksForEvents[i].aTasks); j++)
                {
                    if (g_aPedValidTasksForEvents[i].aTasks[j] == nTask)
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            if (g_aGroupValidTasksForEvents[i].nEvent == nEvent)
            {
                for (uint16_t j = 0; j < ARRAY_SIZE(g_aGroupValidTasksForEvents[i].aTasks); j++)
                {
                    if (g_aGroupValidTasksForEvents[i].aTasks[j] == nTask)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// 0x0040F3D0 ?StringContainsLetter@@YADPBD@Z
bool StringContainsLetter(const char* pszString)
{
    uint16_t nLen = strlen(pszString);
    for (uint16_t i = 0; i < nLen; ++i)
    {
        if (pszString[i] >= 'a' && pszString[i] <= 'z')
        {
            return true;
        }
        if (pszString[i] >= 'A' && pszString[i] <= 'Z')
        {
            return true;
        }
    }
    return false;
}

// 0x0040F420 ?InitDefaultModels@@YAXXZ
void InitDefaultModels()
{
    g_nNumDefaultModels = 0;
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aDefaultModels); i++)
    {
        strcpy(g_aDefaultModels[i].szName, "");
        g_aDefaultModels[i].nId = NO_ID;
    }
}

// 0x0040F450 ?InitEventChoices@@YAXXZ
void InitEventChoices()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aPedEventChoices); i++)
    {
        strcpy(g_aPedEventChoices[i].szName, "");
        g_aPedEventChoices[i].nId = NO_ID;
        strcpy(g_aGroupEventChoices[i].szName, "");
        g_aGroupEventChoices[i].nId = NO_ID;
    }
}

// 0x0040F480 ?InitModelNameList@@YAHXZ
int InitModelNameList()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aUsedModelNames); i++)
    {
        for (uint16_t j = 0; j < ARRAY_SIZE(g_aUsedModelNames[i]); j++)
        {
            g_aUsedModelNames[i][j] = 0;
        }
    }
    g_nNumUsedModelNames = 1;
    return 1;
}

// 0x0040F4A0 ?InitSoundNames@@YAXXZ
void InitSoundNames()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aSoundNames); i++)
    {
        strcpy(g_aSoundNames[i].szName, "");
        g_aSoundNames[i].nId = NO_ID;
    }
}

// 0x0040F4D0 ?InitTaskChoices@@YAXXZ
void InitTaskChoices()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aPedTaskChoices); i++)
    {
        strcpy(g_aPedTaskChoices[i].szName, "");
        g_aPedTaskChoices[i].nId = NO_ID;
        strcpy(g_aGroupTaskChoices[i].szName, "");
        g_aGroupTaskChoices[i].nId = NO_ID;
    }
}

// 0x0040F510 ?InitValidTasksForEvents@@YAXXZ
void InitValidTasksForEvents()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aPedValidTasksForEvents); i++)
    {
        g_aPedValidTasksForEvents[i].nEvent = -1;
        for (uint16_t j = 0; j < ARRAY_SIZE(g_aPedValidTasksForEvents[i].aTasks); j++)
        {
            g_aPedValidTasksForEvents[i].aTasks[j] = -1;
        }
        g_aGroupValidTasksForEvents[i].nEvent = -1;
        for (uint16_t j = 0; j < ARRAY_SIZE(g_aGroupValidTasksForEvents[i].aTasks); j++)
        {
            g_aGroupValidTasksForEvents[i].aTasks[j] = -1;
        }
    }
}

// 0x0040F580 ?LoadIDE@@YAHPAD@Z
int LoadIDE(const char* pszFile)
{
    char szMessage[256];
    char szLine[256];
    char szWord[128];

    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open .ide file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    while (ReadLine(pFile, szLine, ARRAY_SIZE(szLine) - 1) != -1)
    {
        uint16_t i = 0;
        uint16_t j = 0;
        while (szLine[i] == '\t' || szLine[i] == ' ' || szLine[i] == ',')
        {
            if (szLine[i] == '\0')
            {
                goto NextLine;
            }
            i++;
        }
        while (szLine[i] != '\t' && szLine[i] != ' ' && szLine[i] != '\0' && szLine[i] != ',' &&
               j < ARRAY_SIZE(szWord) - 1)
        {
            szWord[j++] = szLine[i++];
        }
        szWord[j] = '\0';
        if (IsIntegerString(szWord))
        {
            ParseIDELine(szLine, &g_nNumDefaultModels, TRUE);
        }
    NextLine:;
    }
    fclose(pFile);
    return 0;
}

// 0x0040F6D0 ?LoadEventChoices@@YAHPADD@Z
int LoadEventChoices(const char* pszFile, bool bGroup)
{
    int nId;
    char szLine[256];
    char szMessage[256];
    char szName[256];

    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    bool bEndOfFile = false;
    int i = 0;
    while (!bEndOfFile)
    {
        if (fgets(szLine, ARRAY_SIZE(szLine) - 1, pFile) == nullptr)
        {
            bEndOfFile = true;
        }
        else if (szLine[0] == '/' || szLine[0] == '\n')
        {
            continue;
        }
        if (!bEndOfFile)
        {
            sscanf(szLine, "%s %d", szName, &nId);
            if (strlen(szName) > NAME_SIZE)
            {
                sprintf(szMessage, "LoadEventChoices - Event name is too long %s", szName);
                PrintMessage(szMessage);
                return -1;
            }
            if (!bGroup)
            {
                strcpy(g_aPedEventChoices[i].szName, szName);
                g_aPedEventChoices[i].nId = nId;
            }
            else
            {
                strcpy(g_aGroupEventChoices[i].szName, szName);
                g_aGroupEventChoices[i].nId = nId;
            }
        }
        i++;
    }
    fclose(pFile);
    return 0;
}

// 0x0040F890 ?LoadObjectIDE@@YAHPADPAG@Z
int LoadObjectIDE(const char* pszFile, uint16_t* pnNumObjects)
{
    char szMessage[256];
    char szLine[256];

    bool bInSection = false;
    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open .ide file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    while (ReadLine(pFile, szLine, ARRAY_SIZE(szLine) - 1) != -1)
    {
        if (LineStartsWithWord(szLine, "objs") || LineStartsWithWord(szLine, "tobj") ||
            LineStartsWithWord(szLine, "anim"))
        {
            bInSection = true;
        }
        else if (bInSection)
        {
            if (LineStartsWithWord(szLine, "end"))
            {
                bInSection = false;
            }
            else
            {
                ParseIDELine(szLine, pnNumObjects, FALSE);
            }
        }
    }
    g_aObjectModels[*pnNumObjects].nId = 0;
    g_aObjectModels[*pnNumObjects].szName[0] = '\0';
    fclose(pFile);
    return 0;
}

#define IDE_DATA_DIRECTORY "DATA"  // IDE paths in gta.dat start with this directory
#define IDE_DATA_DIRECTORY_LENGTH (sizeof(IDE_DATA_DIRECTORY) - 1)

// 0x0040FA00 ?LoadDataFile@@YAHPAD@Z
int LoadDataFile(const char* pszFile)
{
    uint16_t nNumObjects;
    char szMessage[256];
    char szLine[256];
    char szFilename[256];
    char szBuf[255];
    char szWord[128];
    char szError[256];

    nNumObjects = 0;
    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open .dat file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    while (ReadLine(pFile, szLine, ARRAY_SIZE(szLine) - 1) != -1)
    {
        if (LineStartsWithWord(szLine, "IDE"))
        {
            if (nNumObjects >= ARRAY_SIZE(g_aObjectModels) - 1)
            {
                sprintf(szError, "**** Too many objects for the compiler to handle (%d)", ARRAY_SIZE(g_aObjectModels));
                PrintMessage(szError);
                goto Fail;
            }
            int16_t nLen = 0;
            while (szLine[nLen] != 0)
            {
                szBuf[nLen] = szLine[nLen];
                if (szLine[nLen] >= 'a' && szLine[nLen] <= 'z')
                {
                    szBuf[nLen] = szLine[nLen] - ('a' - 'A');
                }
                nLen++;
            }
            szBuf[nLen] = '\0';
            if (nLen > ARRAY_SIZE(szBuf))
            {
                sprintf(szError, "**** Line in file containing names of IDE files has more than 255 characters");
                PrintMessage(szError);
                goto Fail;
            }
            for (uint16_t i = 0; szBuf[i] != 0; i++)
            {
                if (szBuf[i] == '\t')
                {
                    szBuf[i] = ' ';
                }
            }
            for (uint16_t i = 0; i < nLen; i++)
            {
                if (szBuf[i] == ',' || szBuf[i] == '(' || szBuf[i] == ')')
                {
                    szBuf[i] = ' ';
                }
            }
            if (sscanf(szBuf, "%s", szWord) <= 0)
            {
                sprintf(szError, "**** Couldn't read first item in string");
                PrintMessage(szError);
                goto Fail;
            }
            if (strcmp(szWord, "IDE") != 0)
            {
                sprintf(szError, "**** First item in string is not IDE");
                PrintMessage(szError);
                goto Fail;
            }
            RemoveFirstWord(szBuf);
            if (sscanf(szBuf, "%s", szWord) <= 0)
            {
                sprintf(szError, "**** Couldn't read second item in string");
                PrintMessage(szError);
                goto Fail;
            }
            if (!StringContainsLetter(szWord))
            {
                sprintf(szError, "**** Second item in string is not a string");
                PrintMessage(szError);
                goto Fail;
            }
            if (sscanf(szWord, "%s", szFilename) <= 0)
            {
                sprintf(szError, "**** Couldn't read second item (string) in string");
                PrintMessage(szError);
                goto Fail;
            }
            if (strlen(szFilename) > ARRAY_SIZE(szFilename) - 1)
            {
                sprintf(szError, "**** IDE Filename is too long %s", szFilename);
                PrintMessage(szError);
                goto Fail;
            }
            if (strncmp(szFilename, IDE_DATA_DIRECTORY, IDE_DATA_DIRECTORY_LENGTH) != 0 ||
                szFilename[IDE_DATA_DIRECTORY_LENGTH] != '\\')
            {
                sprintf(szError, "**** IDE Filename does not begin with DATA %s", szFilename);
                PrintMessage(szError);
                goto Fail;
            }
            if (static_cast<int16_t>(LoadObjectIDE(&szFilename[IDE_DATA_DIRECTORY_LENGTH + 1], &nNumObjects)) < 0)
            {
                goto Fail;
            }
        }
    }
    fclose(pFile);
    return 0;
Fail:
    return -1;
}

// 0x0040FD80 ?LoadSoundNames@@YAHPAD@Z
int LoadSoundNames(const char* pszFile)
{
    int nId;
    char szLine[256];
    char szMessage[256];
    char szName[256];

    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    bool bEndOfFile = false;
    int i = 0;
    while (!bEndOfFile)
    {
        do
        {
            if (fgets(szLine, ARRAY_SIZE(szLine) - 1, pFile) == nullptr)
            {
                bEndOfFile = true;
                break;
            }
        } while (szLine[0] == '/' || szLine[0] == '\n');
        if (!bEndOfFile)
        {
            sscanf(szLine, "%s %d", szName, &nId);
            if (strlen(szName) > NAME_SIZE)
            {
                sprintf(szMessage, "LoadSoundNames - Sound name is too long %s", szName);
                PrintMessage(szMessage);
                return -1;
            }
            strcpy(g_aSoundNames[i].szName, szName);
            g_aSoundNames[i].nId = nId;
        }
        i++;
    }
    fclose(pFile);
    return 0;
}

// 0x0040FF00 ?LoadTaskChoices@@YAHPADD@Z
int LoadTaskChoices(const char* pszFile, bool bGroup)
{
    int nId;
    char szLine[256];
    char szMessage[256];
    char szName[256];

    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    bool bEndOfFile = false;
    int i = 0;
    while (!bEndOfFile)
    {
        if (fgets(szLine, ARRAY_SIZE(szLine) - 1, pFile) == nullptr)
        {
            bEndOfFile = true;
        }
        else if (szLine[0] == '/' || szLine[0] == '\n')
        {
            continue;
        }
        if (!bEndOfFile)
        {
            sscanf(szLine, "%s %d", szName, &nId);
            if (strlen(szName) > NAME_SIZE)
            {
                sprintf(szMessage, "LoadTaskChoices - Task name is too long %s", szName);
                PrintMessage(szMessage);
                return -1;
            }
            if (!bGroup)
            {
                strcpy(g_aPedTaskChoices[i].szName, szName);
                g_aPedTaskChoices[i].nId = nId;
            }
            else
            {
                strcpy(g_aGroupTaskChoices[i].szName, szName);
                g_aGroupTaskChoices[i].nId = nId;
            }
        }
        i++;
    }
    fclose(pFile);
    return 0;
}

#define EVENT_PREFIX "EVENT_"
#define EVENT_PREFIX_LENGTH (sizeof(EVENT_PREFIX) - 1)

// 0x004100C0 ?LoadValidTasksForEvents@@YAHPADD@Z
int LoadValidTasksForEvents(const char* pszFile, bool bGroup)
{
    char szLine[1024];
    char szMessage[256];

    FILE* pFile = fopen(pszFile, "r");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open file %s", pszFile);
        PrintMessage(szMessage);
        return -1;
    }
    uint16_t nEvent = 0;
    while (ReadLine(pFile, szLine, ARRAY_SIZE(szLine)) != -1)
    {
        if (strncmp(szLine, EVENT_PREFIX, EVENT_PREFIX_LENGTH) == 0)
        {
            char* pszToken = strtok(szLine, ", ");
            int nId = GetIdFromNamedIdArray(pszToken, bGroup ? g_aGroupEventChoices : g_aPedEventChoices);
            if (nId < 0)
            {
                sprintf(szMessage, "LoadValidTasksForEvents - Couldn't find %s in list of Event names", pszToken);
                PrintMessage(szMessage);
                return -1;
            }
            if (!bGroup)
            {
                g_aPedValidTasksForEvents[nEvent].nEvent = nId;
            }
            else
            {
                g_aGroupValidTasksForEvents[nEvent].nEvent = nId;
            }
            uint16_t nTask = 0;
            pszToken = strtok(nullptr, ", ");
            while (pszToken != nullptr)
            {
                nId = GetIdFromNamedIdArray(pszToken, bGroup ? g_aGroupTaskChoices : g_aPedTaskChoices);
                if (nId < 0)
                {
                    sprintf(szMessage, "LoadValidTasksForEvents - Couldn't find %s in list of Task names", pszToken);
                    PrintMessage(szMessage);
                    return -1;
                }
                if (!bGroup)
                {
                    g_aPedValidTasksForEvents[nEvent].aTasks[nTask] = nId;
                }
                else
                {
                    g_aGroupValidTasksForEvents[nEvent].aTasks[nTask] = nId;
                }
                nTask++;
                pszToken = strtok(nullptr, ", ");
            }
            nEvent++;
        }
    }
    fclose(pFile);
    return 0;
}

// 0x004102D0 ?ParseIDELine@@YAHPADPAGF@Z
int ParseIDELine(const char* pszLine, uint16_t* pnCount, int16_t bDefaultModels)
{
    int nId;
    char szBuf[255];
    char szWord[128];
    char szMessage[256];
    char szName[NAME_SIZE];

    if (bDefaultModels == FALSE)
    {
        if (*pnCount >= ARRAY_SIZE(g_aObjectModels) - 1)
        {
            sprintf(szMessage, "**** Too many objects for the compiler to handle (%d)", ARRAY_SIZE(g_aObjectModels));
            PrintMessage(szMessage);
            return -1;
        }
    }
    else if (bDefaultModels == TRUE && *pnCount >= ARRAY_SIZE(g_aDefaultModels) - 1)
    {
        sprintf(szMessage, "**** Too many car and ped models for the compiler to handle (%d)",
            ARRAY_SIZE(g_aDefaultModels));
        PrintMessage(szMessage);
        return -1;
    }
    int16_t nLen = 0;
    while (pszLine[nLen] != 0)
    {
        szBuf[nLen] = pszLine[nLen];
        if (pszLine[nLen] >= 'a' && pszLine[nLen] <= 'z')
        {
            szBuf[nLen] = pszLine[nLen] - ('a' - 'A');
        }
        nLen++;
    }
    szBuf[nLen] = '\0';
    if (nLen > ARRAY_SIZE(szBuf))
    {
        sprintf(szMessage, "**** Line in object data has more than 255 characters");
        PrintMessage(szMessage);
        return -1;
    }
    for (uint16_t i = 0; szBuf[i] != 0; i++)
    {
        if (szBuf[i] == '\t')
        {
            szBuf[i] = ' ';
        }
    }
    for (uint16_t i = 0; i < nLen; i++)
    {
        if (szBuf[i] == ',' || szBuf[i] == '(' || szBuf[i] == ')')
        {
            szBuf[i] = ' ';
        }
    }
    if (sscanf(szBuf, "%s", szWord) <= 0)
    {
        sprintf(szMessage, "**** Couldn't read first item in string");
        PrintMessage(szMessage);
        return -1;
    }
    if (szWord[0] != '#')
    {
        if (!IsIntegerString(szWord))
        {
            sprintf(szMessage, "**** First item in string is not an integer");
            PrintMessage(szMessage);
            return -1;
        }
        if (sscanf(szWord, "%d", &nId) <= 0)
        {
            sprintf(szMessage, "**** Couldn't read first item (integer) in string");
            PrintMessage(szMessage);
            return -1;
        }
        RemoveFirstWord(szBuf);
        if (sscanf(szBuf, "%s", szWord) <= 0)
        {
            sprintf(szMessage, "**** Couldn't read second item in string");
            PrintMessage(szMessage);
            return -1;
        }
        if (!StringContainsLetter(szWord))
        {
            sprintf(szMessage, "**** Second item in string is not a string");
            PrintMessage(szMessage);
            return -1;
        }
        if (sscanf(szWord, "%s", szName) <= 0)
        {
            sprintf(szMessage, "**** Couldn't read second item (string) in string");
            PrintMessage(szMessage);
            return -1;
        }
        if (strlen(szName) > NAME_SIZE)
        {
            sprintf(szMessage, "**** Object name is too long %s", szName);
            PrintMessage(szMessage);
            return -1;
        }
        if (bDefaultModels == FALSE)
        {
            g_aObjectModels[*pnCount].nId = nId;
            strcpy(g_aObjectModels[*pnCount].szName, szName);
        }
        else if (bDefaultModels == TRUE)
        {
            g_aDefaultModels[*pnCount].nId = nId;
            strcpy(g_aDefaultModels[*pnCount].szName, szName);
        }
        (*pnCount)++;
    }
    return 0;
}
