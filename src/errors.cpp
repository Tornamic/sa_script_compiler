// errors.cpp: 0x409650-0x409C60 in sc.exe

#include "common.h"
#include "errors.h"
#include "main.h"
#include "missions.h"
#include "streamedscripts.h"
#include "parser.h"

// 0x01462F20
uint16_t g_nMainScriptNumLines;
// 0x01462F28
char g_szMainScriptFileName[FILE_PATH_SIZE];

// Defined outside main.cpp: main.cpp writes it without a range check, so it never saw the size.
// Which file really owns it is not known; errors.cpp reads it (FindFileRecursively's directory).
// 0x02B22098
char Config_FileNameNoExt[FILE_PATH_SIZE];

// 0x00409650 ?GetFileIndexAndLineFromLOC@@YAHHPAF@Z
int GetFileIndexAndLineFromLOC(int loc, int16_t* pnFileIndex)
{
    if (loc >= g_nMainScriptNumLines && g_nMainScriptNumLines != 0)
    {
        loc -= g_nMainScriptNumLines;
        for (uint16_t i = 0; i < ARRAY_SIZE(g_aForeignGosubFiles); i++)
        {
            if (loc < 0)
            {
                break;
            }
            if (loc < g_aForeignGosubFiles[i].nNumLines ||
                (g_aForeignGosubFiles[i].nNumLines == 0 && g_nCompilerStage == STAGE_FOREIGN_GOSUBS))
            {
                *pnFileIndex = i + FILE_INDEX_FOREIGN_GOSUBS;
                return loc;
            }
            loc -= g_aForeignGosubFiles[i].nNumLines;
        }
        for (uint16_t i = 0; i < ARRAY_SIZE(g_aSubscripts); i++)
        {
            if (loc < 0)
            {
                break;
            }
            if (loc < g_aSubscripts[i].nNumLines ||
                (g_aSubscripts[i].nNumLines == 0 && g_nCompilerStage == STAGE_SUBSCRIPTS))
            {
                *pnFileIndex = i + FILE_INDEX_SUBSCRIPTS;
                return loc;
            }
            loc -= g_aSubscripts[i].nNumLines;
        }
        for (uint16_t i = 0; i < ARRAY_SIZE(g_aMissionScripts); i++)
        {
            if (loc < 0)
            {
                break;
            }
            if (loc < g_aMissionScripts[i].nNumLines ||
                (g_aMissionScripts[i].nNumLines == 0 && g_nCompilerStage == STAGE_MISSIONS))
            {
                *pnFileIndex = i + FILE_INDEX_MISSIONS;
                return loc;
            }
            loc -= g_aMissionScripts[i].nNumLines;
        }
        for (uint16_t i = 0; i < ARRAY_SIZE(g_aStreamedScripts); i++)
        {
            if (loc < 0)
            {
                break;
            }
            if (loc < g_aStreamedScripts[i].nNumLines ||
                (g_aStreamedScripts[i].nNumLines == 0 && g_nCompilerStage == STAGE_STREAMED_SCRIPTS))
            {
                *pnFileIndex = i + FILE_INDEX_STREAMED_SCRIPTS;
                return loc;
            }
            loc -= g_aStreamedScripts[i].nNumLines;
        }
        *pnFileIndex = FILE_INDEX_MAIN;
        return 0;
    }

    *pnFileIndex = FILE_INDEX_MAIN;

    return loc;
}

// 0x004097A0 ?InitMainScriptFileInfo@@YAXXZ
void InitMainScriptFileInfo()
{
    g_nMainScriptNumLines = 0;
    strcpy(g_szMainScriptFileName, Config_InputFileName);
}

// 0x004097D0 ?ReportCompilationErrorWithName@@YAXPADPBDH@Z
void ReportCompilationErrorWithName(const char* pszError, const char* pszName, int loc)
{
    char szOpen[] = " (";
    char szClose[] = ") ";
    char szError[128];

    strcpy(szError, pszError);
    if (pszName != nullptr || *pszName != '\0')
    {
        strcat(szError, szOpen);
        strcat(szError, pszName);
        strcat(szError, szClose);
    }
    ReportCompilationError(szError, loc);
}

#define ERROR_CONTEXT_LINES 3       // lines of the source shown before and after the error line
#define ERROR_LINE_TEXT_LENGTH 100  // characters of each source line shown

// 0x00409900 ?ReportCompilationError@@YAXPADH@Z
void ReportCompilationError(const char* pszError, int loc)
{
    char szArrow[28];
    int16_t nFileIndex;
    char szLine[1024];
    char szPath[1028];
    char szMessage[128];
    FILE* pFile;

    strcpy(szArrow, "    <-----");
    int nLine = GetFileIndexAndLineFromLOC(loc, &nFileIndex);
    sprintf(szMessage, "Error On Line %d in ", nLine);
    if (nFileIndex == FILE_INDEX_MAIN)
    {
        pFile = fopen(g_szMainScriptFileName, "r");
        strcat(szMessage, "main script");
    }
    else
    {
        if (nFileIndex < FILE_INDEX_SUBSCRIPTS)
        {
            FindFileRecursively(Config_FileNameNoExt, g_aForeignGosubFiles[nFileIndex].szName, szPath);
            strcat(szMessage, g_aForeignGosubFiles[nFileIndex].szName);
        }
        else if (nFileIndex < FILE_INDEX_MISSIONS)
        {
            nFileIndex -= FILE_INDEX_SUBSCRIPTS;
            FindFileRecursively(Config_FileNameNoExt, g_aSubscripts[nFileIndex].szName, szPath);
            strcat(szMessage, g_aSubscripts[nFileIndex].szName);
        }
        else if (nFileIndex < FILE_INDEX_STREAMED_SCRIPTS)
        {
            nFileIndex -= FILE_INDEX_MISSIONS;
            FindFileRecursively(Config_FileNameNoExt, g_aMissionScripts[nFileIndex].szName, szPath);
            strcat(szMessage, g_aMissionScripts[nFileIndex].szName);
        }
        else
        {
            nFileIndex -= FILE_INDEX_STREAMED_SCRIPTS;
            FindFileRecursively(Config_FileNameNoExt, g_aStreamedScripts[nFileIndex].szName, szPath);
            strcat(szMessage, g_aStreamedScripts[nFileIndex].szName);
        }
        pFile = fopen(szPath, "r");
    }
    PrintMessage(szMessage);

    int nStart = nLine - ERROR_CONTEXT_LINES;
    if (nLine < ERROR_CONTEXT_LINES)
    {
        nStart = 0;
    }
    int i;
    for (i = 0; i <= nStart; i++)
    {
        if (ReadLine(pFile, szLine, ARRAY_SIZE(szLine)) == -1)
        {
            i = nLine + ERROR_CONTEXT_LINES + 1;
        }
    }
    PrintMessage("   ");
    for (; i < nLine + ERROR_CONTEXT_LINES; i++)
    {
        if (ReadLine(pFile, szLine, ARRAY_SIZE(szLine)) == -1)
        {
            break;
        }
        sprintf(szMessage, "%d:  ", i);
        strncat(szMessage, szLine, ERROR_LINE_TEXT_LENGTH);
        if (i == nLine)
        {
            strcat(szMessage, szArrow);
        }
        PrintMessage(szMessage);
    }
    fclose(pFile);
    PrintMessage(pszError);
    PrintMessage("   ");
}
