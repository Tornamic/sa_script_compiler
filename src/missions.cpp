// missions.cpp: 0x40DC00-0x40F26B in sc.exe

#include "common.h"
#include "missions.h"
#include "streamedscripts.h"
#include "compiler.h"
#include "codegen.h"
#include "parser.h"
#include "locals.h"
#include "main.h"

// 0x01583E80
tSubscript g_aSubscripts[MAX_SUBSCRIPTS];
// 0x01584F60
uint16_t g_nNumSubscripts;
// 0x01585124
int g_nFirstSubscriptOffset;
// 0x01585128
uint32_t g_nLargestSubscriptSize;
// 0x0158512C
char g_szLargestSubscript[SCRIPT_FILE_NAME_SIZE];
// 0x01585140
int g_nCurrentSubscript;

// 0x01584F68
tForeignGosubFile g_aForeignGosubFiles[MAX_FOREIGN_GOSUB_FILES];
// 0x01585120
uint16_t g_nNumForeignGosubFiles;

// 0x01585150
tMissionScript g_aMissionScripts[MAX_MISSION_SCRIPTS];
// 0x01587090
uint16_t NumberOfMissionScripts;
// 0x01587094
uint16_t NumberOfExclusiveMissionScripts;
// 0x01587098
uint32_t SizeOfMainScript;
// 0x0158709C
uint32_t SizeOfLargestMissionScript;
// 0x015870A0
char g_szLargestMissionScript[SCRIPT_FILE_NAME_SIZE];
// 0x015870B4
int WriteMultiScriptDataAt;
// 0x015870B8
int g_nCurrentMissionScript;

// 0x0040DC00 ?CalculateSubscriptSizes@@YAXXZ
void CalculateSubscriptSizes()
{
    if (g_nNumSubscripts == 0)
    {
        g_nLargestSubscriptSize = 0;
        for (uint16_t i = 0; i < ARRAY_SIZE(g_szLargestSubscript); i++)
        {
            g_szLargestSubscript[i] = '\0';
        }
    }
    else
    {
        for (uint16_t i = 0; i < g_nNumSubscripts - 1; i++)
        {
            g_aSubscripts[i].nSize = g_aSubscripts[i + 1].nStartOffset - g_aSubscripts[i].nStartOffset;
        }

        if (NumberOfMissionScripts == 0)
        {
            if (g_nNumStreamedScripts == 0)
            {
                g_aSubscripts[g_nNumSubscripts - 1].nSize =
                    g_nCodeOffset - g_aSubscripts[g_nNumSubscripts - 1].nStartOffset;
            }
            else
            {
                g_aSubscripts[g_nNumSubscripts - 1].nSize =
                    g_aStreamedScripts[0].nStartOffset - g_aSubscripts[g_nNumSubscripts - 1].nStartOffset;
            }
        }
        else
        {
            g_aSubscripts[g_nNumSubscripts - 1].nSize =
                g_aMissionScripts[0].nStartOffset - g_aSubscripts[g_nNumSubscripts - 1].nStartOffset;
        }

        g_nLargestSubscriptSize = 0;
        for (uint16_t i = 0; i < g_nNumSubscripts; i++)
        {
            if (static_cast<uint32_t>(g_aSubscripts[i].nSize) > g_nLargestSubscriptSize)
            {
                g_nLargestSubscriptSize = g_aSubscripts[i].nSize;
                strcpy(g_szLargestSubscript, g_aSubscripts[i].szName);
            }
        }
    }
}

// 0x0040DD10 ?InitSubscriptsAndForeignGosubs@@YAHXZ
int InitSubscriptsAndForeignGosubs()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aSubscripts); i++)
    {
        g_aSubscripts[i].szName[0] = '\0';
        g_aSubscripts[i].nStartLOC = 0;
        g_aSubscripts[i].nStartOffset = 0;
        g_aSubscripts[i].nSize = 0;
        g_aSubscripts[i].bHasMissionStart = false;
        g_aSubscripts[i].bHasMissionEnd = false;
        g_aSubscripts[i].nNumLines = 0;
    }
    g_nNumSubscripts = 0;
    g_nFirstSubscriptOffset = 0;
    g_nLargestSubscriptSize = 0;
    for (uint16_t i = 0; i < ARRAY_SIZE(g_szLargestSubscript); i++)
    {
        g_szLargestSubscript[i] = '\0';
    }
    g_nCurrentSubscript = -1;

    for (uint16_t i = 0; i < ARRAY_SIZE(g_aForeignGosubFiles); i++)
    {
        g_aForeignGosubFiles[i].szName[0] = '\0';
        g_aForeignGosubFiles[i].nNumLines = 0;
    }
    g_nNumForeignGosubFiles = 0;
    return 0;
}

// 0x0040DDB0 ?AddFilenameToForeignGosubArray@@YAXPAD@Z
void AddFilenameToForeignGosubArray(char* pszLine)
{
    char szBuffer[1024];
    char szWord[1024];
    char szLabel[1024];
    char szError[256];
    char szMessage[256];

    strcpy(szBuffer, pszLine);
    int8_t nArg = 0;
    while (sscanf(szBuffer, "%s", szWord) > 0)
    {
        if (nArg == 0)
        {
            if (strcmp(szWord, "GOSUB_FILE") != 0)
            {
                return;
            }
            strcpy(pszLine, szWord);
            strcat(pszLine, " ");
        }
        else if (nArg == 1)
        {
            strcat(pszLine, szWord);
            strcat(pszLine, " ");
            strcpy(szLabel, szWord);
        }
        else if (nArg == 2)
        {
            uint16_t nLen = strlen(szWord);
            if ((bool)(nLen < SCRIPT_FILE_EXTENSION_LENGTH
                           ? false
                           : strcmp(&szWord[nLen - SCRIPT_FILE_EXTENSION_LENGTH], SCRIPT_FILE_EXTENSION) == 0))
            {
                if (g_nNumForeignGosubFiles >= ARRAY_SIZE(g_aForeignGosubFiles))
                {
                    sprintf(szMessage, "ERROR Compiler can only handle %d main extension files",
                        ARRAY_SIZE(g_aForeignGosubFiles));
                    PrintMessage(szMessage);
                }
                if (strlen(szWord) >= SCRIPT_FILE_NAME_SIZE)
                {
                    sprintf(szMessage, "AddFilenameToForeignGosubArray - mission filename %s is too long\n", szWord);
                    PrintMessage(szMessage);
                }
                bool bFound = false;
                for (uint16_t i = 0; i < g_nNumForeignGosubFiles; i++)
                {
                    if (strcmp(g_aForeignGosubFiles[i].szName, szWord) == 0)
                    {
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    strcpy(g_aForeignGosubFiles[g_nNumForeignGosubFiles].szName, szWord);
                    g_nNumForeignGosubFiles++;
                }
                strcat(pszLine, szLabel);
            }
            else
            {
                sprintf(szError, "ERROR second parameter of GOSUB_FILE should be a filename");
                PrintMessage(szError);
            }
        }
        else
        {
            sprintf(szError, "ERROR GOSUB_FILE should only have two parameters");
            PrintMessage(szError);
        }
        RemoveFirstWord(szBuffer);
        nArg++;
    }
}

// 0x0040E110 ?AddFilenameToMissionScriptArray@@YAXPAD@Z
void AddFilenameToMissionScriptArray(char* pszLine)
{
    char szBuffer[1024];
    char szError[256];
    char szWord[1024];
    char szMessage[256];

    strcpy(szBuffer, pszLine);
    int8_t nArg = 0;
    bool bExclusive = false;
    bool bStreamed = false;
    while (sscanf(szBuffer, "%s", szWord) > 0)
    {
        if (nArg == 0)
        {
            if (strcmp(szWord, "LOAD_AND_LAUNCH_MISSION") == 0)
            {
                strcpy(pszLine, "LOAD_AND_LAUNCH_MISSION_INTERNAL");
                bExclusive = false;
                bStreamed = false;
            }
            else if (strcmp(szWord, "LOAD_AND_LAUNCH_MISSION_EXCLUSIVE") == 0)
            {
                strcpy(pszLine, "LOAD_AND_LAUNCH_MISSION_INTERNAL");
                bExclusive = true;
                bStreamed = false;
            }
            else if (strcmp(szWord, "REGISTER_STREAMED_SCRIPT") == 0)
            {
                strcpy(pszLine, "REGISTER_STREAMED_SCRIPT_INTERNAL");
                bExclusive = false;
                bStreamed = true;
            }
            else
            {
                return;
            }
            strcat(pszLine, " ");
        }
        else if (nArg == 1)
        {
            uint16_t nLen = strlen(szWord);
            if (nLen >= SCRIPT_FILE_EXTENSION_LENGTH &&
                strcmp(&szWord[nLen - SCRIPT_FILE_EXTENSION_LENGTH], SCRIPT_FILE_EXTENSION) == 0)
            {
                if (bStreamed)
                {
                    AddFilenameToStreamedScriptArray(szWord);
                }
                else
                {
                    if (NumberOfMissionScripts >= ARRAY_SIZE(g_aMissionScripts))
                    {
                        sprintf(szMessage, "ERROR Compiler can only handle %d mission scripts",
                            ARRAY_SIZE(g_aMissionScripts));
                        PrintMessage(szMessage);
                    }
                    if (strlen(szWord) >= SCRIPT_FILE_NAME_SIZE)
                    {
                        sprintf(
                            szMessage, "AddFilenameToMissionScriptArray - mission filename %s is too long\n", szWord);
                        PrintMessage(szMessage);
                    }
                    bool bFound = false;
                    for (uint16_t i = 0; i < NumberOfMissionScripts; i++)
                    {
                        if (strcmp(g_aMissionScripts[i].szName, szWord) == 0)
                        {
                            bFound = true;
                            sprintf(szWord, "%d", i);
                        }
                    }
                    if (!bFound)
                    {
                        strcpy(g_aMissionScripts[NumberOfMissionScripts].szName, szWord);
                        g_aMissionScripts[NumberOfMissionScripts].bExclusive = bExclusive;
                        if (bExclusive)
                        {
                            sprintf(szWord, "%d", EXCLUSIVE_MISSION_FIRST_ID - NumberOfExclusiveMissionScripts);
                            NumberOfExclusiveMissionScripts++;
                            if (NumberOfExclusiveMissionScripts > MAX_EXCLUSIVE_MISSION_SCRIPTS)
                            {
                                sprintf(szMessage,
                                    "ERROR Can only handle 3 exclusive scripts - two initialisations and one mission");
                                PrintMessage(szMessage);
                            }
                        }
                        else
                        {
                            sprintf(szWord, "%d", NumberOfMissionScripts);
                        }
                        NumberOfMissionScripts++;
                    }
                }
                strcat(pszLine, szWord);
                strcat(pszLine, " ");
            }
            else
            {
                if (bStreamed)
                {
                    sprintf(szError, "ERROR first parameter of REGISTER_STREAMED_SCRIPT should be a filename");
                }
                else if (bExclusive)
                {
                    sprintf(szError, "ERROR first parameter of LOAD_AND_LAUNCH_MISSION_EXCLUSIVE should be a filename");
                }
                else
                {
                    sprintf(szError, "ERROR first parameter of LOAD_AND_LAUNCH_MISSION should be a filename");
                }
                PrintMessage(szError);
            }
        }
        else
        {
            if (bStreamed)
            {
                sprintf(szError, "ERROR REGISTER_STREAMED_SCRIPT should only have one parameter");
            }
            else if (bExclusive)
            {
                sprintf(szError, "ERROR LOAD_AND_LAUNCH_MISSION_EXCLUSIVE should only have one parameter");
            }
            else
            {
                sprintf(szError, "ERROR LOAD_AND_LAUNCH_MISSION should only have one parameter");
            }
            PrintMessage(szError);
        }
        RemoveFirstWord(szBuffer);
        nArg++;
    }
}

// 0x0040E610 ?AddFilenameToSubscriptArray@@YAXPAD@Z
void AddFilenameToSubscriptArray(char* pszLine)
{
    char szBuffer[1024];
    char szWord[1024];
    char szError[256];
    char szMessage[256];

    strcpy(szBuffer, pszLine);
    int8_t nArg = 0;
    while (sscanf(szBuffer, "%s", szWord) > 0)
    {
        if (nArg == 0)
        {
            if (strcmp(szWord, "LAUNCH_MISSION") != 0)
            {
                return;
            }
            strcpy(pszLine, szWord);
            strcat(pszLine, " ");
        }
        else if (nArg == 1)
        {
            uint16_t nLen = strlen(szWord);
            if ((bool)(nLen < SCRIPT_FILE_EXTENSION_LENGTH
                           ? false
                           : strcmp(&szWord[nLen - SCRIPT_FILE_EXTENSION_LENGTH], SCRIPT_FILE_EXTENSION) == 0))
            {
                if (g_nNumSubscripts >= ARRAY_SIZE(g_aSubscripts))
                {
                    sprintf(szMessage, "ERROR Compiler can only handle %d mission scripts", ARRAY_SIZE(g_aSubscripts));
                    PrintMessage(szMessage);
                }
                if (strlen(szWord) >= SCRIPT_FILE_NAME_SIZE)
                {
                    sprintf(szMessage, "AddFilenameToSubscriptArray - mission filename %s is too long\n", szWord);
                    PrintMessage(szMessage);
                }
                bool bFound = false;
                for (uint16_t i = 0; i < g_nNumSubscripts; i++)
                {
                    if (strcmp(g_aSubscripts[i].szName, szWord) == 0)
                    {
                        sprintf(szWord, "SPECIAL_LABEL_SUBSCRIPT_%d", i);
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    strcpy(g_aSubscripts[g_nNumSubscripts].szName, szWord);
                    sprintf(szWord, "SPECIAL_LABEL_SUBSCRIPT_%d", g_nNumSubscripts);
                    g_nNumSubscripts++;
                }
                strcat(pszLine, szWord);
                strcat(pszLine, " ");
            }
            else
            {
                sprintf(szError, "ERROR first parameter of LAUNCH_MISSION should be a filename");
                PrintMessage(szError);
            }
        }
        else
        {
            sprintf(szError, "ERROR LAUNCH_MISSION should only have one parameter");
            PrintMessage(szError);
        }
        RemoveFirstWord(szBuffer);
        nArg++;
    }
}

// 0x0040E990 ?GetScriptIndexFromOffset@@YAHIPAD@Z
int GetScriptIndexFromOffset(uint32_t nCodeOffset, int8_t* pnScriptType)
{
    int nIndex = -1;
    *pnScriptType = SCRIPT_TYPE_MAIN;

    uint16_t i = 0;
    bool bFound = false;
    while (i < NumberOfMissionScripts && !bFound)
    {
        if (nCodeOffset >= static_cast<uint32_t>(g_aMissionScripts[i].nStartOffset))
        {
            nIndex = i;
            *pnScriptType = SCRIPT_TYPE_MISSION;
        }
        else
        {
            bFound = true;
        }
        i++;
    }

    i = 0;
    bFound = false;
    while (i < g_nNumStreamedScripts && !bFound)
    {
        if (static_cast<int>(nCodeOffset) >= g_aStreamedScripts[i].nStartOffset)
        {
            nIndex = i;
            *pnScriptType = SCRIPT_TYPE_STREAMED;
        }
        else
        {
            bFound = true;
        }
        i++;
    }
    return nIndex;
}

// 0x0040EA40 ?GetMissionScriptHeaderSize@@YAHXZ
int GetMissionScriptHeaderSize()
{ return NumberOfMissionScripts * MISSION_HEADER_ENTRY_SIZE + MISSION_HEADER_START_SIZE; }

// 0x0040EA50 ?InitMissionScripts@@YAXXZ
void InitMissionScripts()
{
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aMissionScripts); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(g_aMissionScripts[i].szName); j++)
        {
            g_aMissionScripts[i].szName[j] = '\0';
        }
        g_aMissionScripts[i].nStartLOC = 0;
        g_aMissionScripts[i].nStartOffset = 0;
        g_aMissionScripts[i].nSize = 0;
        g_aMissionScripts[i].bHasMissionStart = false;
        g_aMissionScripts[i].bHasMissionEnd = false;
        g_aMissionScripts[i].bExclusive = false;
        g_aMissionScripts[i].nNumLines = 0;
    }
    NumberOfMissionScripts = 0;
    NumberOfExclusiveMissionScripts = 0;
    SizeOfMainScript = 0;
    SizeOfLargestMissionScript = 0;
    for (uint16_t i = 0; i < ARRAY_SIZE(g_szLargestMissionScript); i++)
    {
        g_szLargestMissionScript[i] = '\0';
    }
    WriteMultiScriptDataAt = 0;
    g_nCurrentMissionScript = -1;
}

// 0x0040EAE0 ?ReadMissionScriptFiles@@YAHXZ
int16_t ReadMissionScriptFiles()
{
    char szMainScript[INPUT_FILE_NAME_SIZE];
    char szMessage[128];

    strcpy(szMainScript, Config_InputFileName);

    if (NumberOfExclusiveMissionScripts != 0)
    {
        uint16_t nExclusive = 0;
        for (uint16_t i = 0; i < NumberOfMissionScripts; i++)
        {
            if (g_aMissionScripts[i].bExclusive)
            {
                if (i != nExclusive)
                {
                    g_aMissionScripts[nExclusive] = g_aMissionScripts[i];
                }
                nExclusive++;
            }
        }
        NumberOfMissionScripts = NumberOfExclusiveMissionScripts;
    }

    for (uint16_t i = 0; i < NumberOfMissionScripts; i++)
    {
        g_aMissionScripts[i].nStartLOC = g_nNumLines;
        if (ReadScriptFile(i + FILE_INDEX_MISSIONS) >= 0)
        {
            sprintf(szMessage, "Reading Mission Script %s. . .", g_aMissionScripts[i].szName);
            PrintMessage(szMessage);
        }
        else
        {
            sprintf(szMessage, "ERROR Reading Mission Script %s", g_aMissionScripts[i].szName);
            PrintMessage(szMessage);
            strcpy(Config_InputFileName, szMainScript);
            return -1;
        }
    }
    strcpy(Config_InputFileName, szMainScript);
    return 0;
}

// 0x0040EC80 ?WriteMissionScriptHeader@@YAXXZ
void WriteMissionScriptHeader()
{
    char szText[2048];
    char szBuffer[1792];
    char szLine[256];

    int nCodeOffset = g_nCodeOffset;
    if (NumberOfMissionScripts == 0)
    {
        SizeOfLargestMissionScript = 0;
        for (uint16_t i = 0; i < ARRAY_SIZE(g_szLargestMissionScript); i++)
        {
            g_szLargestMissionScript[i] = '\0';
        }
        if (g_nNumStreamedScripts == 0)
        {
            SizeOfMainScript = nCodeOffset;
        }
    }
    else
    {
        for (uint16_t i = 0; i < NumberOfMissionScripts - 1; i++)
        {
            g_aMissionScripts[i].nSize = g_aMissionScripts[i + 1].nStartOffset - g_aMissionScripts[i].nStartOffset;
        }

        if (g_nNumStreamedScripts == 0)
        {
            g_aMissionScripts[NumberOfMissionScripts - 1].nSize =
                nCodeOffset - g_aMissionScripts[NumberOfMissionScripts - 1].nStartOffset;
        }
        else
        {
            g_aMissionScripts[NumberOfMissionScripts - 1].nSize =
                g_aStreamedScripts[0].nStartOffset - g_aMissionScripts[NumberOfMissionScripts - 1].nStartOffset;
        }

        SizeOfLargestMissionScript = 0;
        for (uint16_t i = 0; i < NumberOfMissionScripts; i++)
        {
            if (static_cast<uint32_t>(g_aMissionScripts[i].nSize) > SizeOfLargestMissionScript)
            {
                SizeOfLargestMissionScript = g_aMissionScripts[i].nSize;
                strcpy(g_szLargestMissionScript, g_aMissionScripts[i].szName);
            }
        }
    }

    sprintf(szBuffer, "CompiledCodeIndex %d\n", nCodeOffset);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);

    int nSavedCodeOffset = g_nCodeOffset;
    g_nCodeOffset = WriteMultiScriptDataAt;
    sprintf(szBuffer, "WriteMultiScriptDataAt %d\n", WriteMultiScriptDataAt);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);

    WriteInt32(SizeOfMainScript);
    WriteInt32(SizeOfLargestMissionScript);
    WriteInt16(NumberOfMissionScripts);
    WriteInt16(NumberOfExclusiveMissionScripts);
    WriteInt32(LargestNumberOfLocalVars);

    sprintf(szBuffer, "SizeOfMainScript %d\n", SizeOfMainScript);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);
    sprintf(szBuffer, "SizeOfLargestMissionScript %d\n", SizeOfLargestMissionScript);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);
    sprintf(szBuffer, "NumberOfMissionScripts %d\n", NumberOfMissionScripts);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);
    sprintf(szBuffer, "NumberOfExclusiveMissionScripts %d\n", NumberOfExclusiveMissionScripts);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);
    sprintf(szBuffer, "LocalVariables::GetLargestNumberOfLocalVars() %d\n", LargestNumberOfLocalVars);
    WriteStringToFile(g_pScriptCompileInfoFile, szBuffer);

    FILE* pMissionOutput = nullptr;
    if (Config_bWriteMissionOutput)
    {
        pMissionOutput = fopen("missionOutput.txt", "wb");
        for (int i = 0; i < g_nNumSubscripts; i++)
        {
            sprintf(szLine, "[%d]%s %d\r\n", i, g_aSubscripts[i].szName, g_aSubscripts[i].nStartOffset);
            WriteStringToFile(pMissionOutput, szLine);
            sprintf(szText, "\twriteMissionOut %s", szLine);
            WriteStringToFile(g_pScriptCompileInfoFile, szText);
        }
    }

    for (uint16_t i = 0; i < NumberOfMissionScripts; i++)
    {
        WriteInt32(g_aMissionScripts[i].nStartOffset);
        if (pMissionOutput != nullptr)
        {
            sprintf(szLine, "[%d]%s %d\n\r", i, g_aMissionScripts[i].szName, g_aMissionScripts[i].nStartOffset);
            WriteStringToFile(pMissionOutput, szLine);
            sprintf(szText, "\tmissionOutput %s", szLine);
            WriteStringToFile(g_pScriptCompileInfoFile, szText);
        }
    }
    if (pMissionOutput != nullptr)
    {
        fclose(pMissionOutput);
    }

    sprintf(szText, "NumberOfBytesWritten %d\n", g_nCodeOffset - WriteMultiScriptDataAt);
    WriteStringToFile(g_pScriptCompileInfoFile, szText);
    sprintf(szText, "GetSizeOfMissionScriptDataToSave %d\n", GetMissionScriptHeaderSize());
    WriteStringToFile(g_pScriptCompileInfoFile, szText);
    g_nCodeOffset = nSavedCodeOffset;
}

// 0x0040F240 ?WriteStringToFile@@YAXPAU_iobuf@@PBD@Z
void WriteStringToFile(FILE* pFile, const char* pszString)
{ fwrite(pszString, strlen(pszString), 1, pFile); }
