#pragma once

#include "common.h"

#define MODEL_NAME_SIZE 24
#define MAX_USED_MODEL_NAMES 395
#define MAX_DEFAULT_MODELS 600   // car and ped models (default.ide)
#define MAX_OBJECT_MODELS 15000  // object models of the IDE files listed in gta.dat
#define MAX_EVENT_CHOICES 60     // decision maker events
#define MAX_TASK_CHOICES 130     // decision maker tasks
#define MAX_TASKS_PER_EVENT 65
#define MAX_SOUND_NAMES 20000
#define NO_ID 0xFFFF  // nId of an unused tNamedId

struct tNamedId
{
    char szName[NAME_SIZE];
    int nId;
};

struct tValidTasksForEvent
{
    int nEvent;
    int aTasks[MAX_TASKS_PER_EVENT];
};

// ----------------------------------------------------------------------------------------- models

extern tNamedId g_aDefaultModels[MAX_DEFAULT_MODELS];                  // 0x015870C0
extern tNamedId g_aObjectModels[MAX_OBJECT_MODELS];                    // 0x016EB150
extern uint16_t g_nNumDefaultModels;                                   // 0x017E41B0
extern char g_aUsedModelNames[MAX_USED_MODEL_NAMES][MODEL_NAME_SIZE];  // 0x017E41B8
extern uint16_t g_nNumUsedModelNames;                                  // 0x017E66C0

// -------------------------------------------------------------------------------- decision makers

extern tNamedId g_aPedEventChoices[MAX_EVENT_CHOICES];                      // 0x01591020
extern tNamedId g_aPedTaskChoices[MAX_TASK_CHOICES];                        // 0x01592010
extern tValidTasksForEvent g_aPedValidTasksForEvents[MAX_EVENT_CHOICES];    // 0x01594298
extern tNamedId g_aGroupEventChoices[MAX_EVENT_CHOICES];                    // 0x01598078
extern tNamedId g_aGroupTaskChoices[MAX_TASK_CHOICES];                      // 0x01599068
extern tValidTasksForEvent g_aGroupValidTasksForEvents[MAX_EVENT_CHOICES];  // 0x0159B2F0

// ----------------------------------------------------------------------------------------- sounds

extern tNamedId g_aSoundNames[MAX_SOUND_NAMES];  // 0x0159F0D0

// loader.cpp

int GetModelNameIndex(const char* pszModelName);                         // 0x0040F270
bool IsValidTaskForEvent(uint16_t nEvent, uint16_t nTask, bool bGroup);  // 0x0040F340
bool StringContainsLetter(const char* pszString);                        // 0x0040F3D0
void InitDefaultModels();                                                // 0x0040F420
void InitEventChoices();                                                 // 0x0040F450
int InitModelNameList();                                                 // 0x0040F480
void InitSoundNames();                                                   // 0x0040F4A0
void InitTaskChoices();                                                  // 0x0040F4D0
void InitValidTasksForEvents();                                          // 0x0040F510
int LoadIDE(const char* pszFile);                                        // 0x0040F580
int LoadEventChoices(const char* pszFile, bool bGroup);                  // 0x0040F6D0
int LoadObjectIDE(const char* pszFile, uint16_t* pnNumObjects);          // 0x0040F890
int LoadDataFile(const char* pszFile);                                   // 0x0040FA00
int LoadSoundNames(const char* pszFile);                                 // 0x0040FD80
int LoadTaskChoices(const char* pszFile, bool bGroup);                   // 0x0040FF00
int LoadValidTasksForEvents(const char* pszFile, bool bGroup);           // 0x004100C0
int ParseIDELine(
    const char* pszLine, uint16_t* pnCount, int16_t bDefaultModels);     // 0x004102D0 bDefaultModels: TRUE or FALSE
