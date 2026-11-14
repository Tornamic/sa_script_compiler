// expressions.cpp: 0x410620-0x415680 in sc.exe

#include "common.h"
#include "expressions.h"
#include "errors.h"

// Token types produced by the operator tokenizer in ConvertOperatorsToCommands
enum eOperatorToken
{
    OPTOKEN_NONE,
    OPTOKEN_NAME,              // anything that isn't an operator: variables, numbers, labels, keywords
    OPTOKEN_INCREMENT,         // ++
    OPTOKEN_DECREMENT,         // --
    OPTOKEN_ADD_ASSIGN,        // +=
    OPTOKEN_SUB_ASSIGN,        // -=
    OPTOKEN_EQUALS,            // =
    OPTOKEN_MULTIPLY,          // *
    OPTOKEN_MUL_ASSIGN,        // *=
    OPTOKEN_DIVIDE,            // /
    OPTOKEN_DIV_ASSIGN,        // /=
    OPTOKEN_PLUS,              // +
    OPTOKEN_MINUS,             // -
    OPTOKEN_LESS,              // <
    OPTOKEN_GREATER,           // >
    OPTOKEN_LESS_EQUAL,        // <=
    OPTOKEN_GREATER_EQUAL,     // >=
    OPTOKEN_CSET,              // =#
    OPTOKEN_PLUS_TIMED,        // +@
    OPTOKEN_MINUS_TIMED,       // -@
    OPTOKEN_ADD_ASSIGN_TIMED,  // +=@
    OPTOKEN_SUB_ASSIGN_TIMED,  // -=@
};

#define MAX_TOKENS 32
#define TOKEN_SIZE 128

// Starts with a letter, '$' or '&' and doesn't end with ':' (not a label)
#define IS_VARIABLE_NAME(psz)                                                                           \
    (((psz)[0] >= 'a' && (psz)[0] <= 'z') || ((psz)[0] >= 'A' && (psz)[0] <= 'Z') || (psz)[0] == '$' || \
        (psz)[0] == '&') &&                                                                             \
        (psz)[strlen(psz) - 1] != ':'

// 0x00410620 ?ConvertOperatorsToCommands@@YA_NPADHD@Z
bool ConvertOperatorsToCommands(char* pszLine, int loc, bool bIsCondition)
{
    char szLineCopy[EXPRESSION_LINE_SIZE];
    char aTokens[MAX_TOKENS][TOKEN_SIZE];
    int16_t aTypes[MAX_TOKENS];

    strcpy(szLineCopy, pszLine);

    for (int16_t i = 0; i < ARRAY_SIZE(aTypes); i++)
    {
        aTypes[i] = OPTOKEN_NONE;
        aTokens[i][0] = '\0';
    }

    int16_t nPos = 0;
    int16_t nNumTokens = 0;
    while (pszLine[nPos] != '\0')
    {
        while (pszLine[nPos] == '\t' || pszLine[nPos] == ' ')
        {
            nPos++;
        }

        if (pszLine[nPos] == '+' && pszLine[nPos + 1] == '=' && pszLine[nPos + 2] == '@')
        {
            aTypes[nNumTokens] = OPTOKEN_ADD_ASSIGN_TIMED;
            nPos += 3;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '-' && pszLine[nPos + 1] == '=' && pszLine[nPos + 2] == '@')
        {
            aTypes[nNumTokens] = OPTOKEN_SUB_ASSIGN_TIMED;
            nPos += 3;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '+' && pszLine[nPos + 1] == '@')
        {
            aTypes[nNumTokens] = OPTOKEN_PLUS_TIMED;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '-' && pszLine[nPos + 1] == '@')
        {
            aTypes[nNumTokens] = OPTOKEN_MINUS_TIMED;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '+' && pszLine[nPos + 1] == '+')
        {
            aTypes[nNumTokens] = OPTOKEN_INCREMENT;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '+' && pszLine[nPos + 1] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_ADD_ASSIGN;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '-' && pszLine[nPos + 1] == '-')
        {
            aTypes[nNumTokens] = OPTOKEN_DECREMENT;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '-' && pszLine[nPos + 1] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_SUB_ASSIGN;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '*' && pszLine[nPos + 1] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_MUL_ASSIGN;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '/' && pszLine[nPos + 1] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_DIV_ASSIGN;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '<' && pszLine[nPos + 1] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_LESS_EQUAL;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '>' && pszLine[nPos + 1] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_GREATER_EQUAL;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '=' && pszLine[nPos + 1] == '#')
        {
            aTypes[nNumTokens] = OPTOKEN_CSET;
            nPos += 2;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '-' && (pszLine[nPos + 1] < '0' || pszLine[nPos + 1] > '9') &&
                 pszLine[nPos + 1] != '.')
        {
            aTypes[nNumTokens] = OPTOKEN_MINUS;
            nPos++;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '+')
        {
            aTypes[nNumTokens] = OPTOKEN_PLUS;
            nPos++;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '*')
        {
            aTypes[nNumTokens] = OPTOKEN_MULTIPLY;
            nPos++;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '/')
        {
            aTypes[nNumTokens] = OPTOKEN_DIVIDE;
            nPos++;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '=')
        {
            aTypes[nNumTokens] = OPTOKEN_EQUALS;
            nPos++;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '<')
        {
            aTypes[nNumTokens] = OPTOKEN_LESS;
            nPos++;
            nNumTokens++;
        }
        else if (pszLine[nPos] == '>')
        {
            aTypes[nNumTokens] = OPTOKEN_GREATER;
            nPos++;
            nNumTokens++;
        }
        else
        {
            int16_t nStart = nPos;
            int16_t nLen = 0;
            char c = pszLine[nPos];
            while (c != ' ' && c != '\t' && c != '\0' && c != '+' && (c != '-' || nPos == nStart) && c != '*' &&
                   c != '/' && c != '=' && c != '<' && c != '>')
            {
                aTokens[nNumTokens][nLen++] = c;
                nPos++;
                c = pszLine[nPos];
            }
            aTokens[nNumTokens][nLen] = '\0';
            if (nPos != nStart)
            {
                aTypes[nNumTokens++] = OPTOKEN_NAME;
            }
        }
    }

    char bNot = 0;  // char, not bool: see below
    if (aTypes[0] == OPTOKEN_NAME &&
        ((strcmp(aTokens[0], "IF") == 0) || (strcmp(aTokens[0], "WHILE") == 0) || (strcmp(aTokens[0], "IFNOT") == 0) ||
            (strcmp(aTokens[0], "WHILENOT") == 0)) &&
        aTypes[1] == OPTOKEN_NAME && (strcmp(aTokens[1], "NOT") == 0))
    {
        // always 1 here; a literal 'true' lets MSVC treat bNot as a bool and duplicate the epilogue at every exit
        bNot = static_cast<char>(aTypes[1]);
        for (int16_t i = 1; i < ARRAY_SIZE(aTypes) - 1; i++)
        {
            aTypes[i] = aTypes[i + 1];
        }
        for (int16_t i = 1; i < ARRAY_SIZE(aTokens) - 1; i++)
        {
            strcpy(aTokens[i], aTokens[i + 1]);
        }
        aTokens[ARRAY_SIZE(aTokens) - 1][0] = '\0';
    }

    if (aTypes[0] == OPTOKEN_DECREMENT && aTypes[1] == OPTOKEN_NAME && aTypes[2] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "SUB_THING_FROM_THING %s 1", aTokens[1]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && aTypes[1] == OPTOKEN_DECREMENT && aTypes[2] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "SUB_THING_FROM_THING %s 1", aTokens[0]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && aTypes[1] == OPTOKEN_INCREMENT && aTypes[2] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "ADD_THING_TO_THING %s 1", aTokens[0]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && aTypes[1] == OPTOKEN_EQUALS && aTypes[2] == OPTOKEN_NAME &&
             aTypes[3] == OPTOKEN_NONE)
    {
        if (bIsCondition)
        {
            sprintf(pszLine, "IS_THING_EQUAL_TO_THING %s %s", aTokens[0], aTokens[2]);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && aTypes[1] == OPTOKEN_CSET && aTypes[2] == OPTOKEN_NAME &&
             aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "CSET %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_PLUS && aTypes[4] == OPTOKEN_NAME &&
             aTypes[5] == OPTOKEN_NONE)
    {
        if (strcmp(aTokens[0], aTokens[2]) == 0)
        {
            sprintf(pszLine, "ADD_THING_TO_THING %s %s", aTokens[0], aTokens[4]);
        }
        else if (strcmp(aTokens[0], aTokens[4]) == 0)
        {
            sprintf(pszLine, "ADD_THING_TO_THING %s %s", aTokens[0], aTokens[2]);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "ADD_THING_TO_THING %s %s", aTokens[0], aTokens[4]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_MINUS && aTypes[4] == OPTOKEN_NAME &&
             aTypes[5] == OPTOKEN_NONE)
    {
        if (strcmp(aTokens[0], aTokens[2]) == 0)
        {
            sprintf(pszLine, "SUB_THING_FROM_THING %s %s", aTokens[0], aTokens[4]);
        }
        else if (strcmp(aTokens[0], aTokens[4]) == 0)
        {
            ReportCompilationError("**** CAN'T DO VAR1 = THING - VAR1", loc);
            sprintf(pszLine, "NEGATE %s", aTokens[0]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "ADD_THING_TO_THING %s %s", aTokens[0], aTokens[2]);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "SUB_THING_FROM_THING %s %s", aTokens[0], aTokens[4]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_PLUS_TIMED && aTypes[4] == OPTOKEN_NAME &&
             aTypes[5] == OPTOKEN_NONE)
    {
        if (strcmp(aTokens[0], aTokens[2]) == 0)
        {
            sprintf(pszLine, "ADD_THING_TO_THING_TIMED %s %s", aTokens[0], aTokens[4]);
        }
        else if (strcmp(aTokens[0], aTokens[4]) == 0)
        {
            ReportCompilationError("**** CAN'T DO VAR1 = THING +@ VAR1", loc);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "ADD_THING_TO_THING_TIMED %s %s", aTokens[0], aTokens[4]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_MINUS_TIMED && aTypes[4] == OPTOKEN_NAME &&
             aTypes[5] == OPTOKEN_NONE)
    {
        if (strcmp(aTokens[0], aTokens[2]) == 0)
        {
            sprintf(pszLine, "SUB_THING_FROM_THING_TIMED %s %s", aTokens[0], aTokens[4]);
        }
        else if (strcmp(aTokens[0], aTokens[4]) == 0)
        {
            ReportCompilationError("**** CAN'T DO VAR1 = THING -@ VAR1", loc);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "SUB_THING_FROM_THING_TIMED %s %s", aTokens[0], aTokens[4]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_MULTIPLY && aTypes[4] == OPTOKEN_NAME &&
             aTypes[5] == OPTOKEN_NONE)
    {
        if (strcmp(aTokens[0], aTokens[2]) == 0)
        {
            sprintf(pszLine, "MULT_THING_BY_THING %s %s", aTokens[0], aTokens[4]);
        }
        else if (strcmp(aTokens[0], aTokens[4]) == 0)
        {
            sprintf(pszLine, "MULT_THING_BY_THING %s %s", aTokens[0], aTokens[2]);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "MULT_THING_BY_THING %s %s", aTokens[0], aTokens[4]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_DIVIDE && aTypes[4] == OPTOKEN_NAME &&
             aTypes[5] == OPTOKEN_NONE)
    {
        if (strcmp(aTokens[0], aTokens[2]) == 0)
        {
            sprintf(pszLine, "DIV_THING_BY_THING %s %s", aTokens[0], aTokens[4]);
        }
        else if (strcmp(aTokens[0], aTokens[4]) == 0)
        {
            ReportCompilationError("**** CAN'T DO VAR1 = THING / VAR1", loc);
            sprintf(pszLine, "ONEOVER %s", aTokens[0]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "MULT_THING_BY_THING %s %s", aTokens[0], aTokens[2]);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "DIV_THING_BY_THING %s %s", aTokens[0], aTokens[4]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_ADD_ASSIGN &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "ADD_THING_TO_THING %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_SUB_ASSIGN &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "SUB_THING_FROM_THING %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_ADD_ASSIGN_TIMED &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "ADD_THING_TO_THING_TIMED %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_SUB_ASSIGN_TIMED &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "SUB_THING_FROM_THING_TIMED %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_MUL_ASSIGN &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "MULT_THING_BY_THING %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_DIV_ASSIGN &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "DIV_THING_BY_THING %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && IS_VARIABLE_NAME(aTokens[0]) && aTypes[1] == OPTOKEN_EQUALS &&
             aTypes[2] == OPTOKEN_NAME && aTypes[3] == OPTOKEN_NONE)
    {
        if (bIsCondition)
        {
            sprintf(pszLine, "IS_THING_EQUAL_TO_THING %s %s", aTokens[0], aTokens[2]);
        }
        else
        {
            sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[2]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && (strcmp(aTokens[0], "IF") == 0))
    {
        if (aTypes[2] == OPTOKEN_GREATER && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
            aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[1], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER_EQUAL && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_LESS && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[3], aTokens[1]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_LESS_EQUAL && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[3], aTokens[1]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_EQUALS && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_NAME && (strcmp(aTokens[2], "GOTO") == 0) && aTypes[3] == OPTOKEN_NAME &&
                 aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s", aTokens[1]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[3]);
        }
        else if (aTypes[3] == OPTOKEN_NAME && (strcmp(aTokens[3], "GOTO") == 0) && aTypes[4] == OPTOKEN_NAME &&
                 aTypes[5] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s", aTokens[1], aTokens[2]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[4]);
        }
        else if (aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) && aTypes[5] == OPTOKEN_NAME &&
                 aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s", aTokens[1], aTokens[2], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[5]);
        }
        else if (aTypes[5] == OPTOKEN_NAME && (strcmp(aTokens[5], "GOTO") == 0) && aTypes[6] == OPTOKEN_NAME &&
                 aTypes[7] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[6]);
        }
        else if (aTypes[6] == OPTOKEN_NAME && (strcmp(aTokens[6], "GOTO") == 0) && aTypes[7] == OPTOKEN_NAME &&
                 aTypes[8] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[7]);
        }
        else if (aTypes[7] == OPTOKEN_NAME && (strcmp(aTokens[7], "GOTO") == 0) && aTypes[8] == OPTOKEN_NAME &&
                 aTypes[9] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5], aTokens[6]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[8]);
        }
        else if (aTypes[8] == OPTOKEN_NAME && (strcmp(aTokens[8], "GOTO") == 0) && aTypes[9] == OPTOKEN_NAME &&
                 aTypes[10] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[9]);
        }
        else if (aTypes[9] == OPTOKEN_NAME && (strcmp(aTokens[9], "GOTO") == 0) && aTypes[10] == OPTOKEN_NAME &&
                 aTypes[11] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[10]);
        }
        else if (aTypes[10] == OPTOKEN_NAME && (strcmp(aTokens[10], "GOTO") == 0) && aTypes[11] == OPTOKEN_NAME &&
                 aTypes[12] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[11]);
        }
        else if (aTypes[11] == OPTOKEN_NAME && (strcmp(aTokens[11], "GOTO") == 0) && aTypes[12] == OPTOKEN_NAME &&
                 aTypes[13] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[12]);
        }
        else if (aTypes[12] == OPTOKEN_NAME && (strcmp(aTokens[12], "GOTO") == 0) && aTypes[13] == OPTOKEN_NAME &&
                 aTypes[14] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[13]);
        }
        else if (aTypes[13] == OPTOKEN_NAME && (strcmp(aTokens[13], "GOTO") == 0) && aTypes[14] == OPTOKEN_NAME &&
                 aTypes[15] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[14]);
        }
        else if (aTypes[14] == OPTOKEN_NAME && (strcmp(aTokens[14], "GOTO") == 0) && aTypes[15] == OPTOKEN_NAME &&
                 aTypes[16] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[15]);
        }
        else if (aTypes[15] == OPTOKEN_NAME && (strcmp(aTokens[15], "GOTO") == 0) && aTypes[16] == OPTOKEN_NAME &&
                 aTypes[17] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13], aTokens[14]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[16]);
        }
        else if (aTypes[16] == OPTOKEN_NAME && (strcmp(aTokens[16], "GOTO") == 0) && aTypes[17] == OPTOKEN_NAME &&
                 aTypes[18] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[17]);
        }
        else if (aTypes[17] == OPTOKEN_NAME && (strcmp(aTokens[17], "GOTO") == 0) && aTypes[18] == OPTOKEN_NAME &&
                 aTypes[19] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15], aTokens[16]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_TRUE %s", aTokens[18]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER_EQUAL && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_LESS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[3], aTokens[1]);
        }
        else if (aTypes[2] == OPTOKEN_LESS_EQUAL && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[3], aTokens[1]);
        }
        else if (aTypes[2] == OPTOKEN_EQUALS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s", aTokens[1]);
        }
        else if (aTypes[3] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s", aTokens[1], aTokens[2]);
        }
        else if (aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s", aTokens[1], aTokens[2], aTokens[3]);
        }
        else if (aTypes[5] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4]);
        }
        else if (aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5]);
        }
        else if (aTypes[7] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5], aTokens[6]);
        }
        else if (aTypes[8] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7]);
        }
        else if (aTypes[9] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8]);
        }
        else if (aTypes[10] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9]);
        }
        else if (aTypes[11] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10]);
        }
        else if (aTypes[12] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11]);
        }
        else if (aTypes[13] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12]);
        }
        else if (aTypes[14] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13]);
        }
        else if (aTypes[15] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13], aTokens[14]);
        }
        else if (aTypes[16] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15]);
        }
        else if (aTypes[17] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IF 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15], aTokens[16]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && (strcmp(aTokens[0], "IFNOT") == 0))
    {
        if (aTypes[2] == OPTOKEN_LESS_EQUAL && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
            aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[3], aTokens[1]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_LESS && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[3], aTokens[1]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER_EQUAL && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[1], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_EQUALS && aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) &&
                 aTypes[5] == OPTOKEN_NAME && aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[5]);
        }
        else if (aTypes[2] == OPTOKEN_NAME && (strcmp(aTokens[2], "GOTO") == 0) && aTypes[3] == OPTOKEN_NAME &&
                 aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s", aTokens[1]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[3]);
        }
        else if (aTypes[3] == OPTOKEN_NAME && (strcmp(aTokens[3], "GOTO") == 0) && aTypes[4] == OPTOKEN_NAME &&
                 aTypes[5] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s", aTokens[1], aTokens[2]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[4]);
        }
        else if (aTypes[4] == OPTOKEN_NAME && (strcmp(aTokens[4], "GOTO") == 0) && aTypes[5] == OPTOKEN_NAME &&
                 aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s", aTokens[1], aTokens[2], aTokens[3]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[5]);
        }
        else if (aTypes[5] == OPTOKEN_NAME && (strcmp(aTokens[5], "GOTO") == 0) && aTypes[6] == OPTOKEN_NAME &&
                 aTypes[7] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[6]);
        }
        else if (aTypes[6] == OPTOKEN_NAME && (strcmp(aTokens[6], "GOTO") == 0) && aTypes[7] == OPTOKEN_NAME &&
                 aTypes[8] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[7]);
        }
        else if (aTypes[7] == OPTOKEN_NAME && (strcmp(aTokens[7], "GOTO") == 0) && aTypes[8] == OPTOKEN_NAME &&
                 aTypes[9] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5], aTokens[6]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[8]);
        }
        else if (aTypes[8] == OPTOKEN_NAME && (strcmp(aTokens[8], "GOTO") == 0) && aTypes[9] == OPTOKEN_NAME &&
                 aTypes[10] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[9]);
        }
        else if (aTypes[9] == OPTOKEN_NAME && (strcmp(aTokens[9], "GOTO") == 0) && aTypes[10] == OPTOKEN_NAME &&
                 aTypes[11] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[10]);
        }
        else if (aTypes[10] == OPTOKEN_NAME && (strcmp(aTokens[10], "GOTO") == 0) && aTypes[11] == OPTOKEN_NAME &&
                 aTypes[12] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[11]);
        }
        else if (aTypes[11] == OPTOKEN_NAME && (strcmp(aTokens[11], "GOTO") == 0) && aTypes[12] == OPTOKEN_NAME &&
                 aTypes[13] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[12]);
        }
        else if (aTypes[12] == OPTOKEN_NAME && (strcmp(aTokens[12], "GOTO") == 0) && aTypes[13] == OPTOKEN_NAME &&
                 aTypes[14] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[13]);
        }
        else if (aTypes[13] == OPTOKEN_NAME && (strcmp(aTokens[13], "GOTO") == 0) && aTypes[14] == OPTOKEN_NAME &&
                 aTypes[15] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[14]);
        }
        else if (aTypes[14] == OPTOKEN_NAME && (strcmp(aTokens[14], "GOTO") == 0) && aTypes[15] == OPTOKEN_NAME &&
                 aTypes[16] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[15]);
        }
        else if (aTypes[15] == OPTOKEN_NAME && (strcmp(aTokens[15], "GOTO") == 0) && aTypes[16] == OPTOKEN_NAME &&
                 aTypes[17] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13], aTokens[14]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[16]);
        }
        else if (aTypes[16] == OPTOKEN_NAME && (strcmp(aTokens[16], "GOTO") == 0) && aTypes[17] == OPTOKEN_NAME &&
                 aTypes[18] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[17]);
        }
        else if (aTypes[17] == OPTOKEN_NAME && (strcmp(aTokens[17], "GOTO") == 0) && aTypes[18] == OPTOKEN_NAME &&
                 aTypes[19] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "ANDOR 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15], aTokens[16]);
            sprintf(pszLine + 2 * EXPRESSION_LINE_SIZE, "GOTO_IF_FALSE %s", aTokens[18]);
        }
        else if (aTypes[2] == OPTOKEN_LESS_EQUAL && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[3], aTokens[1]);
        }
        else if (aTypes[2] == OPTOKEN_LESS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[3], aTokens[1]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER_EQUAL && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_EQUALS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s", aTokens[1]);
        }
        else if (aTypes[3] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s", aTokens[1], aTokens[2]);
        }
        else if (aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s", aTokens[1], aTokens[2], aTokens[3]);
        }
        else if (aTypes[5] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4]);
        }
        else if (aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5]);
        }
        else if (aTypes[7] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5], aTokens[6]);
        }
        else if (aTypes[8] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7]);
        }
        else if (aTypes[9] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8]);
        }
        else if (aTypes[10] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9]);
        }
        else if (aTypes[11] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10]);
        }
        else if (aTypes[12] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11]);
        }
        else if (aTypes[13] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12]);
        }
        else if (aTypes[14] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13]);
        }
        else if (aTypes[15] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13], aTokens[14]);
        }
        else if (aTypes[16] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15]);
        }
        else if (aTypes[17] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "IFNOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15], aTokens[16]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && (strcmp(aTokens[0], "WHILE") == 0))
    {
        if (aTypes[2] == OPTOKEN_GREATER && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_GREATER_EQUAL && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_LESS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_THAN_THING %s %s", aTokens[3], aTokens[1]);
        }
        else if (aTypes[2] == OPTOKEN_LESS_EQUAL && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[3], aTokens[1]);
        }
        else if (aTypes[2] == OPTOKEN_EQUALS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s", aTokens[1]);
        }
        else if (aTypes[3] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s", aTokens[1], aTokens[2]);
        }
        else if (aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s", aTokens[1], aTokens[2], aTokens[3]);
        }
        else if (aTypes[5] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4]);
        }
        else if (aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5]);
        }
        else if (aTypes[7] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5], aTokens[6]);
        }
        else if (aTypes[8] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7]);
        }
        else if (aTypes[9] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8]);
        }
        else if (aTypes[10] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9]);
        }
        else if (aTypes[11] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10]);
        }
        else if (aTypes[12] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11]);
        }
        else if (aTypes[13] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12]);
        }
        else if (aTypes[14] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13]);
        }
        else if (aTypes[15] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13], aTokens[14]);
        }
        else if (aTypes[16] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15]);
        }
        else if (aTypes[17] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILE 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15], aTokens[16]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && (strcmp(aTokens[0], "WHILENOT") == 0))
    {
        if (aTypes[2] == OPTOKEN_EQUALS && aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "IS_THING_EQUAL_TO_THING %s %s", aTokens[1], aTokens[3]);
        }
        else if (aTypes[2] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s", aTokens[1]);
        }
        else if (aTypes[3] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s", aTokens[1], aTokens[2]);
        }
        else if (aTypes[4] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s", aTokens[1], aTokens[2], aTokens[3]);
        }
        else if (aTypes[5] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4]);
        }
        else if (aTypes[6] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5]);
        }
        else if (aTypes[7] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3], aTokens[4],
                aTokens[5], aTokens[6]);
        }
        else if (aTypes[8] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7]);
        }
        else if (aTypes[9] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8]);
        }
        else if (aTypes[10] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9]);
        }
        else if (aTypes[11] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2], aTokens[3],
                aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10]);
        }
        else if (aTypes[12] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11]);
        }
        else if (aTypes[13] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12]);
        }
        else if (aTypes[14] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13]);
        }
        else if (aTypes[15] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1], aTokens[2],
                aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9], aTokens[10],
                aTokens[11], aTokens[12], aTokens[13], aTokens[14]);
        }
        else if (aTypes[16] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15]);
        }
        else if (aTypes[17] == OPTOKEN_NONE)
        {
            sprintf(pszLine, "WHILENOT 0");
            sprintf(pszLine + EXPRESSION_LINE_SIZE, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", aTokens[1],
                aTokens[2], aTokens[3], aTokens[4], aTokens[5], aTokens[6], aTokens[7], aTokens[8], aTokens[9],
                aTokens[10], aTokens[11], aTokens[12], aTokens[13], aTokens[14], aTokens[15], aTokens[16]);
        }
    }
    else if (aTypes[0] == OPTOKEN_NAME && aTypes[1] == OPTOKEN_EQUALS && aTypes[2] == OPTOKEN_NAME &&
             (strcmp(aTokens[2], "ABS") == 0) && aTypes[3] == OPTOKEN_NAME && aTypes[4] == OPTOKEN_NONE &&
             (strcmp(aTokens[0], aTokens[3]) == 0))
    {
        sprintf(pszLine, "ABS %s", aTokens[0]);
    }
    else if (aTypes[0] == OPTOKEN_NAME && aTypes[1] == OPTOKEN_EQUALS && aTypes[2] == OPTOKEN_NAME &&
             (strcmp(aTokens[2], "ABS") == 0) && aTypes[3] == OPTOKEN_NAME && aTypes[4] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "SET %s %s", aTokens[0], aTokens[3]);
        sprintf(pszLine + EXPRESSION_LINE_SIZE, "ABS %s", aTokens[0]);
    }
    else if (aTypes[0] == OPTOKEN_INCREMENT && aTypes[1] == OPTOKEN_NAME && aTypes[2] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "ADD_THING_TO_THING %s 1", aTokens[1]);
    }
    else if (aTypes[1] == OPTOKEN_GREATER && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "IS_THING_GREATER_THAN_THING %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[1] == OPTOKEN_GREATER_EQUAL && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[0], aTokens[2]);
    }
    else if (aTypes[1] == OPTOKEN_LESS && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "IS_THING_GREATER_THAN_THING %s %s", aTokens[2], aTokens[0]);
    }
    else if (aTypes[1] == OPTOKEN_LESS_EQUAL && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "IS_THING_GREATER_OR_EQUAL_TO_THING %s %s", aTokens[2], aTokens[0]);
    }
    else if (aTypes[1] == OPTOKEN_EQUALS && aTypes[3] == OPTOKEN_NONE)
    {
        sprintf(pszLine, "IS_THING_EQUAL_TO_THING %s %s", aTokens[0], aTokens[2]);
    }

    return bNot != 0;
}
