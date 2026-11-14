// textkeys.cpp: 0x41D180-0x41E144 in sc.exe

#include "common.h"
#include "textkeys.h"
#include "missions.h"
#include "streamedscripts.h"
#include "compiledlines.h"
#include "compiler.h"
#include "errors.h"
#include "main.h"

// 0x02B23260
CTextKeyList g_DisplayedTextKeys;

// 0x0041D180 ?AddToListOfDisplayedTextKeys@CTextKeyList@@QAEXPBD0D@Z
void CTextKeyList::AddToListOfDisplayedTextKeys(const char* pszKey, const char* pszMissionName, bool bInMission)
{
    char szMessage[256];

    AddToAlphabeticalTextKeyList(pszKey);
    uint16_t i = 0;
    bool bFound = false;
    while (i < nNumKeys && !bFound)
    {
        if (strcmp(aKeys[i].szKey, pszKey) == 0)
        {
            bFound = true;
        }
        else
        {
            i++;
        }
    }
    if (bFound)
    {
        if (strcmp(aKeys[i].szMissionName, pszMissionName) != 0)
        {
            strcpy(aKeys[i].szMissionName, pszMissionName);
            aKeys[i].nUsageCount++;
            aKeys[i].bInMission = bInMission;
        }
    }
    else
    {
        if (nNumKeys >= ARRAY_SIZE(aKeys))
        {
            sprintf(szMessage, "AddToListOfDisplayedTextKeys - too many text keys\n");
            PrintMessage(szMessage);
        }
        strcpy(aKeys[nNumKeys].szKey, pszKey);
        strcpy(aKeys[nNumKeys].szMissionName, pszMissionName);
        aKeys[nNumKeys].nUsageCount = 1;
        aKeys[nNumKeys].bInMission = bInMission;
        nNumKeys++;
    }
}

// 0x0041D360 ?AddToListOfDisplayedTextKeys2@CTextKeyList@@QAEXHGG@Z
void CTextKeyList::AddToListOfDisplayedTextKeys2(int loc, uint16_t nCmd, uint16_t nArg)
{
    char szMessage[256];
    char szMissionName[40];
    char szKey[16];
    int16_t nFileIndex;

    eScriptCommands nCommand = GetCommandAtLOC(loc, nCmd, true);
    switch (nCommand)
    {
        case COMMAND_PRINT_BIG:
        case COMMAND_PRINT:
        case COMMAND_PRINT_NOW:
        case COMMAND_PRINT_SOON:
        case COMMAND_ADD_PAGER_MESSAGE:
        case COMMAND_PRINT_WITH_NUMBER_BIG:
        case COMMAND_PRINT_WITH_NUMBER:
        case COMMAND_PRINT_WITH_NUMBER_NOW:
        case COMMAND_PRINT_WITH_NUMBER_SOON:
        case COMMAND_ADD_PAGER_MESSAGE_WITH_NUMBER:
        case COMMAND_START_KILL_FRENZY:
        case COMMAND_PRINT_BIG_Q:
        case COMMAND_PRINT_WITH_NUMBER_BIG_Q:
        case COMMAND_SET_REPEATED_PHONE_MESSAGE:
        case COMMAND_SET_PHONE_MESSAGE:
        case COMMAND_PRINT_WITH_2_NUMBERS:
        case COMMAND_PRINT_WITH_2_NUMBERS_NOW:
        case COMMAND_PRINT_WITH_2_NUMBERS_SOON:
        case COMMAND_PRINT_WITH_3_NUMBERS:
        case COMMAND_PRINT_WITH_3_NUMBERS_NOW:
        case COMMAND_PRINT_WITH_3_NUMBERS_SOON:
        case COMMAND_PRINT_WITH_4_NUMBERS:
        case COMMAND_PRINT_WITH_4_NUMBERS_NOW:
        case COMMAND_PRINT_WITH_4_NUMBERS_SOON:
        case COMMAND_PRINT_WITH_5_NUMBERS:
        case COMMAND_PRINT_WITH_5_NUMBERS_NOW:
        case COMMAND_PRINT_WITH_5_NUMBERS_SOON:
        case COMMAND_PRINT_WITH_6_NUMBERS:
        case COMMAND_PRINT_WITH_6_NUMBERS_NOW:
        case COMMAND_PRINT_WITH_6_NUMBERS_SOON:
        case COMMAND_REGISTER_MISSION_PASSED:
        case COMMAND_DISPLAY_TEXT:
        case COMMAND_START_KILL_FRENZY_HEADSHOT:
        case COMMAND_PRINT_WITH_2_NUMBERS_BIG:
        case COMMAND_PRINT_WITH_3_NUMBERS_BIG:
        case COMMAND_PRINT_WITH_4_NUMBERS_BIG:
        case COMMAND_PRINT_WITH_5_NUMBERS_BIG:
        case COMMAND_PRINT_WITH_6_NUMBERS_BIG:
        case COMMAND_PRINT_STRING_IN_STRING:
        case COMMAND_SET_2_REPEATED_PHONE_MESSAGES:
        case COMMAND_SET_2_PHONE_MESSAGES:
        case COMMAND_SET_3_REPEATED_PHONE_MESSAGES:
        case COMMAND_SET_3_PHONE_MESSAGES:
        case COMMAND_SET_4_REPEATED_PHONE_MESSAGES:
        case COMMAND_SET_4_PHONE_MESSAGES:
        case COMMAND_PRINT_STRING_IN_STRING_NOW:
        case COMMAND_PRINT_STRING_IN_STRING_SOON:
        case COMMAND_SET_5_REPEATED_PHONE_MESSAGES:
        case COMMAND_SET_5_PHONE_MESSAGES:
        case COMMAND_SET_6_REPEATED_PHONE_MESSAGES:
        case COMMAND_SET_6_PHONE_MESSAGES:
        case COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING:
        case COMMAND_DISPLAY_ONSCREEN_COUNTER_WITH_STRING:
        case COMMAND_CLEAR_THIS_PRINT:
        case COMMAND_CLEAR_THIS_BIG_PRINT:
        case COMMAND_PRINT_HELP:
        case COMMAND_DISPLAY_TEXT_WITH_NUMBER:
        case COMMAND_DISPLAY_TEXT_WITH_2_NUMBERS:
        case COMMAND_DISPLAY_NTH_ONSCREEN_COUNTER_WITH_STRING:
        case COMMAND_PRINT_HELP_WITH_NUMBER:
        case COMMAND_PRINT_HELP_FOREVER:
        case COMMAND_PRINT_HELP_FOREVER_WITH_NUMBER:
        case COMMAND_DISPLAY_TEXT_WITH_3_NUMBERS:
        case COMMAND_SET_VAR_TEXT_LABEL:
        case COMMAND_SET_LVAR_TEXT_LABEL:
        case COMMAND_IS_VAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL:
        case COMMAND_IS_LVAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL:
        case COMMAND_SET_UP_CONVERSATION_NODE:
        case COMMAND_IS_CONVERSATION_AT_NODE:
        case COMMAND_SET_UP_CONVERSATION_END_NODE:
        case COMMAND_DISPLAY_TWO_ONSCREEN_COUNTERS_WITH_STRING:
        case COMMAND_DISPLAY_NTH_TWO_ONSCREEN_COUNTERS_WITH_STRING:
        case COMMAND_DISPLAY_TEXT_WITH_FLOAT:
        case COMMAND_DRAW_RECT_WITH_TITLE:
        case COMMAND_DRAW_WINDOW_TEXT:
        case COMMAND_CREATE_MENU:
        case COMMAND_SET_MENU_COLUMN:
        case COMMAND_SET_MENU_ITEM_WITH_NUMBER:
        case COMMAND_SET_MENU_ITEM_WITH_2_NUMBERS:
        case COMMAND_DRAW_WINDOW:
        case COMMAND_CREATE_MENU_GRID:
        case COMMAND_SET_UP_CONVERSATION_NODE_WITH_SPEECH:
        case COMMAND_SET_UP_CONVERSATION_END_NODE_WITH_SPEECH:
        case COMMAND_GET_STRING_WIDTH:
        case COMMAND_GET_STRING_WIDTH_WITH_NUMBER:
        case COMMAND_SET_UP_CONVERSATION_NODE_WITH_SCRIPTED_SPEECH:
        case COMMAND_SET_AREA_NAME:
        case COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED:
        case COMMAND_SET_UP_CONVERSATION_END_NODE_WITH_SCRIPTED_SPEECH:
        case COMMAND_DISPLAY_TEXT_WIDGET:
        case COMMAND_CREATE_SHOP_WIDGET:
        case COMMAND_ADD_SHOP_ITEM:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL:
        case COMMAND_DISPLAY_TEXT_CLAMPED:
        case COMMAND_DISPLAY_TEXT_WITH_NUMBER_CLAMPED:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_TOUCH:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_HID:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_CLASSIC:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_ADAPTED:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_HID_JOYPAD:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_HID_KEYBOARD:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_ANALOG:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_DIGITAL:
        case COMMAND_PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_FLICK:
        case COMMAND_PRINT_HELP_CONDITIONAL:
        case COMMAND_PRINT_HELP_CONDITIONAL_TOUCH:
        case COMMAND_PRINT_HELP_CONDITIONAL_HID:
        case COMMAND_PRINT_HELP_CONDITIONAL_TOUCH_CLASSIC:
        case COMMAND_PRINT_HELP_CONDITIONAL_TOUCH_ADAPTED:
        case COMMAND_PRINT_HELP_CONDITIONAL_HID_JOYPAD:
        case COMMAND_PRINT_HELP_CONDITIONAL_HID_KEYBOARD:
        case COMMAND_PRINT_HELP_CONDITIONAL_TOUCH_ANALOG:
            break;
        case COMMAND_IS_PLAYER_IN_ZONE:
        case COMMAND_SET_ZONE_CAR_INFO:
        case COMMAND_IS_CHAR_IN_ZONE:
        case COMMAND_SET_CAR_DENSITY:
        case COMMAND_SET_PED_DENSITY:
        case COMMAND_SET_ZONE_PED_INFO:
        case COMMAND_LOAD_CAMERA_SPLINE:
        case COMMAND_SET_TARGET_CAR_FOR_MISSION_GARAGE:
        case COMMAND_IS_CAR_IN_MISSION_GARAGE:
        case COMMAND_LOAD_SPECIAL_CHARACTER:
        case COMMAND_ACTIVATE_GARAGE:
        case COMMAND_DEACTIVATE_GARAGE:
        case COMMAND_GET_NUMBER_OF_CARS_COLLECTED_BY_GARAGE:
        case COMMAND_HAS_CAR_BEEN_TAKEN_TO_GARAGE:
        case COMMAND_GET_RANDOM_CHAR_IN_ZONE:
        case COMMAND_LOAD_CUTSCENE:
        case COMMAND_SET_CUTSCENE_ANIM:
        case COMMAND_LOAD_SPECIAL_MODEL:
        case COMMAND_SET_CUTSCENE_HEAD_ANIM:
        case COMMAND_CHANGE_GARAGE_TYPE:
        case COMMAND_SET_ZONE_GROUP:
        case COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_ZONE:
        case COMMAND_HAS_RESPRAY_HAPPENED:
        case COMMAND_UNDRESS_CHAR:
        case COMMAND_IS_EXPLOSION_IN_ZONE:
        case COMMAND_OPEN_GARAGE:
        case COMMAND_CLOSE_GARAGE:
        case COMMAND_LOAD_SPRITE:
        case COMMAND_LOAD_TEXTURE_DICTIONARY:
        case COMMAND_SCRIPT_NAME:
        case COMMAND_CHANGE_GARAGE_TYPE_WITH_CAR_MODEL:
        case COMMAND_IS_GARAGE_OPEN:
        case COMMAND_IS_GARAGE_CLOSED:
        case COMMAND_SET_ROTATING_GARAGE_DOOR:
        case COMMAND_LOAD_MISSION_AUDIO:
        case COMMAND_HAS_IMPORT_GARAGE_SLOT_BEEN_FILLED:
        case COMMAND_NO_SPECIAL_CAMERA_FOR_THIS_GARAGE:
        case COMMAND_DOES_GARAGE_CONTAIN_CAR:
        case COMMAND_LOAD_SPLASH_SCREEN:
        case COMMAND_TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME:
        case COMMAND_GET_RANDOM_COP_IN_ZONE:
        case COMMAND_SET_CUTSCENE_ANIM_TO_LOOP:
        case COMMAND_IS_PICKUP_IN_ZONE:
        case COMMAND_SET_ZONE_CIVILIAN_CAR_INFO:
        case COMMAND_IS_PLAYER_WEARING:
        case COMMAND_CREATE_LOCKED_PROPERTY_PICKUP:
        case COMMAND_CREATE_FORSALE_PROPERTY_PICKUP:
        case COMMAND_ATTACH_CUTSCENE_OBJECT_TO_COMPONENT:
        case COMMAND_LOAD_MISSION_TEXT:
        case COMMAND_LOAD_UNCOMPRESSED_ANIM:
        case COMMAND_SET_MAXIMUM_NUMBER_OF_CARS_IN_GARAGE:
        case COMMAND_IS_PLAYER_IN_INFO_ZONE:
        case COMMAND_GET_RANDOM_ICE_CREAM_CUSTOMER_IN_ZONE:
        case COMMAND_IS_3D_COORD_IN_ZONE:
        case COMMAND_SET_ZONE_RADAR_COLOURS:
        case COMMAND_SET_ZONE_POPULATION_TYPE:
        case COMMAND_SET_ZONE_GANG_CAP:
        case COMMAND_GET_ZONE_GANG_CAP:
        case COMMAND_SET_ZONE_DEALER_STRENGTH:
        case COMMAND_GET_ZONE_DEALER_STRENGTH:
        case COMMAND_SET_ZONE_GANG_STRENGTH:
        case COMMAND_GET_ZONE_GANG_STRENGTH:
        case COMMAND_REGISTER_SCRIPT_BRAIN_FOR_CODE_USE:
        case COMMAND_REGISTER_OBJECT_SCRIPT_BRAIN_FOR_CODE_USE:
        case COMMAND_SWITCH_ENTRY_EXIT:
        case COMMAND_SET_NAMED_ENTRY_EXIT_FLAG:
        case COMMAND_SET_ZONE_POPULATION_RACE:
        case COMMAND_REGISTER_ATTRACTOR_SCRIPT_BRAIN_FOR_CODE_USE:
        case COMMAND_SET_ZONE_FOR_GANG_WARS_TRAINING:
        case COMMAND_ATTACH_ANIMS_TO_MODEL:
        case COMMAND_APPEND_TO_NEXT_CUTSCENE:
        case COMMAND_SET_SPECIFIC_ZONE_TO_TRIGGER_GANG_WAR:
        case COMMAND_SWITCH_AUDIO_ZONE:
        case COMMAND_SET_GARAGE_RESPRAY_FREE:
        case COMMAND_SET_ZONE_NO_COPS:
            return;
        default:
            sprintf(szMessage, "AddToListOfDisplayedTextKeys - command %d not handled\n", nCommand);
            PrintMessage(szMessage);
            break;
    }

    GetFileIndexAndLineFromLOC(loc, &nFileIndex);
    bool bInMission = GetScriptNameFromFileIndex(nFileIndex, szMissionName);
    strcpy(szKey, GetArgStringAtLOC(loc, nCmd, nArg, true));
    int nLen = strlen(szKey);
    if (nLen >= TEXT_LABEL_SIZE)
    {
        sprintf(szMessage, "AddToListOfDisplayedTextKeys - key %s is too long\n", szKey);
        PrintMessage(szMessage);
    }
    if (strlen(szMissionName) >= SCRIPT_FILE_NAME_SIZE)
    {
        sprintf(szMessage, "AddToListOfDisplayedTextKeys - mission filename %s is too long\n", szMissionName);
        PrintMessage(szMessage);
    }
    AddToListOfDisplayedTextKeys(szKey, szMissionName, bInMission);
    if (nCommand == COMMAND_SET_UP_CONVERSATION_NODE && nArg == 0)
    {
        if (nLen >= TEXT_LABEL_SIZE - 1)
        {
            sprintf(szMessage, "AddToListOfDisplayedTextKeys - key %s is too long for conversation node\n", szKey);
            PrintMessage(szMessage);
        }
        szKey[nLen] = 'Y';
        szKey[nLen + 1] = '\0';
        AddToListOfDisplayedTextKeys(szKey, szMissionName, bInMission);
        szKey[nLen] = 'N';
        szKey[nLen + 1] = '\0';
        AddToListOfDisplayedTextKeys(szKey, szMissionName, bInMission);
    }
}

// 0x0041DE30 ?GetScriptNameFromFileIndex@CTextKeyList@@QAEDFPAD@Z
bool CTextKeyList::GetScriptNameFromFileIndex(int16_t nFileIndex, char* pszNameOut)
{
    bool bInMission = false;
    if (nFileIndex == FILE_INDEX_MAIN)
    {
        sprintf(pszNameOut, "MAIN");
    }
    else
    {
        if (nFileIndex < FILE_INDEX_SUBSCRIPTS)
        {
            sprintf(pszNameOut, g_aForeignGosubFiles[nFileIndex - FILE_INDEX_FOREIGN_GOSUBS].szName);
        }
        else if (nFileIndex < FILE_INDEX_MISSIONS)
        {
            sprintf(pszNameOut, g_aSubscripts[nFileIndex - FILE_INDEX_SUBSCRIPTS].szName);
        }
        else if (nFileIndex < FILE_INDEX_STREAMED_SCRIPTS)
        {
            bInMission = true;
            sprintf(pszNameOut, g_aMissionScripts[nFileIndex - FILE_INDEX_MISSIONS].szName);
        }
        else
        {
            sprintf(pszNameOut, g_aStreamedScripts[nFileIndex - FILE_INDEX_STREAMED_SCRIPTS].szName);
        }
        int nLen = strlen(pszNameOut);
        bool bEnd = false;
        for (uint16_t i = 0; i < nLen; i++)
        {
            if (pszNameOut[i] == '.')
            {
                bEnd = true;
            }
            if (bEnd)
            {
                pszNameOut[i] = '\0';
            }
        }
    }
    return bInMission;
}

// 0x0041DEF0 ?InitTextKeyList@CTextKeyList@@QAEHXZ
int CTextKeyList::InitTextKeyList()
{
    for (int i = 0; i < ARRAY_SIZE(aKeys); i++)
    {
        for (int j = 0; j < ARRAY_SIZE(aKeys[i].szKey); j++)
        {
            aKeys[i].szKey[j] = '\0';
        }
        for (int j = 0; j < ARRAY_SIZE(aKeys[i].szMissionName); j++)
        {
            aKeys[i].szMissionName[j] = '\0';
        }
        aKeys[i].nUsageCount = 0;
        aKeys[i].bInMission = false;
    }
    nNumKeys = 0;
    return 0;
}

// 0x0041DF30 ?WriteMissionTextKeysFile@CTextKeyList@@QAEXXZ
void CTextKeyList::WriteMissionTextKeysFile()
{
    char szMissionName[SCRIPT_FILE_NAME_SIZE];
    char szKey[TEXT_LABEL_SIZE];

    FILE* pFile = fopen("missiontextkeys.txt", "w");
    if (pFile == nullptr)
    {
        PrintMessage("**** Couldn't open text file for storing the keys that are only used in one mission - check it "
                     "is writable");
        return;
    }
    bool bFound;
    do
    {
        bFound = false;
        for (uint16_t i = 0; i < nNumKeys; i++)
        {
            if (aKeys[i].nUsageCount == 1 && aKeys[i].bInMission)
            {
                if (!bFound)
                {
                    strcpy(szMissionName, aKeys[i].szMissionName);
                    bFound = true;
                }
                else if (strcmp(aKeys[i].szMissionName, szMissionName) < 0)
                {
                    strcpy(szMissionName, aKeys[i].szMissionName);
                }
            }
        }
        if (bFound)
        {
            int nIndex;
            do
            {
                nIndex = -1;
                for (uint16_t i = 0; i < nNumKeys; i++)
                {
                    if (aKeys[i].nUsageCount == 1 && strcmp(aKeys[i].szMissionName, szMissionName) == 0)
                    {
                        if (nIndex == -1)
                        {
                            strcpy(szKey, aKeys[i].szKey);
                            nIndex = i;
                        }
                        else if (strcmp(aKeys[i].szKey, szKey) < 0)
                        {
                            strcpy(szKey, aKeys[i].szKey);
                            nIndex = i;
                        }
                    }
                }
                if (nIndex != -1)
                {
                    fprintf(pFile, "%s %s\n", aKeys[nIndex].szKey, aKeys[nIndex].szMissionName);
                    aKeys[nIndex].nUsageCount = 0;
                }
            } while (nIndex >= 0);
        }
    } while (bFound);
    fclose(pFile);
}
