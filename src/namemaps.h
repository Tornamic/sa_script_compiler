#pragma once

#include "common.h"

// g_ap* tables hold a name and its value (cast to const char*) per entry
#define NAME_VALUE_PAIR_SIZE 2

// Resources the script refers to, each written to a file by WriteResourcesUsedByScriptFiles
enum eScriptResource
{
    RESOURCE_CUTSCENES,
    RESOURCE_ANIMS,
    RESOURCE_ANIM_DICTIONARIES,
    RESOURCE_SPEECH_CONTEXTS,
    RESOURCE_MODELS,
    RESOURCE_SPECIAL_CHARACTERS,
    RESOURCE_PARTICLES,
    NUM_SCRIPT_RESOURCES
};

struct tNamedId;

// --------------------------------------------------------------------------- constant name tables

extern const char* g_apszCameraModes[];            // 0x00497D70 names for g_mapCameraModes, "" terminated
extern const char* g_apszPads[];                   // 0x00497E54 names for g_mapPads, "" terminated
extern const char* g_apszPadSticks[];              // 0x00497E60 names for g_mapPadSticks, "" terminated
extern const char* g_apszBlipDisplayTypes[];       // 0x00497EB4 names for g_mapBlipDisplayTypes, "" terminated
extern const char* g_apszPedTypes[];               // 0x00497EC8 names for g_mapPedTypes, "" terminated
extern const char* g_apszColours[];                // 0x00497F4C names for g_mapColours, "" terminated
extern const char* g_apszCameraSwitchModes[];      // 0x00497F6C names for g_mapCameraSwitchModes, "" terminated
extern const char* g_apszFadeTypes[];              // 0x00497F7C names for g_mapFadeTypes, "" terminated
extern const char* g_apszShadowTypes[];            // 0x00497F88 names for g_mapShadowTypes, "" terminated
extern const char* g_apszWeaponTypes[];            // 0x00497FA8 names for g_mapWeaponTypes, "" terminated
extern const char* g_apszCarLockStates[];          // 0x00498094 names for g_mapCarLockStates, "" terminated
extern const char* g_apszWeatherTypes[];           // 0x004980B8 names for g_mapWeatherTypes, "" terminated
extern const char* g_apszKillFrenzyStates[];       // 0x00498118 names for g_mapKillFrenzyStates, "" terminated
extern const char* g_apszExplosionTypes[];         // 0x00498130 names for g_mapExplosionTypes, "" terminated
extern const char* g_apszPickupTypes[];            // 0x00498168 names for g_mapPickupTypes, "" terminated
extern const char* g_apszGarageTypes[];            // 0x004981C8 names for g_mapGarageTypes, "" terminated
extern const char* g_apszCarBombTypes[];           // 0x00498288 names for g_mapCarBombTypes, "" terminated
extern const char* g_apszCarColours[];             // 0x004982A4 names for g_mapCarColours, "" terminated
extern const char* g_apszGangs[];                  // 0x004982CC names for g_mapGangs, "" terminated
extern const char* g_apszCoronaTypes[];            // 0x004982F8 names for g_mapCoronaTypes, "" terminated
extern const char* g_apszFlareTypes[];             // 0x00498328 names for g_mapFlareTypes, "" terminated
extern const char* g_apszRadarSprites[];           // 0x00498338 names for g_mapRadarSprites, "" terminated
extern const char* g_apszDrivingModes[];           // 0x0049843C names for g_mapDrivingModes, "" terminated
extern const char* g_apszFonts[];                  // 0x0049845C names for g_mapFonts, "" terminated
extern const char* g_apszCarMissions[];            // 0x00498470 names for g_mapCarMissions, "" terminated
extern const char* g_apszFormations[];             // 0x00498588 names for g_mapFormations, "" terminated
extern const char* g_apszWaitStates[];             // 0x004985B0 names for g_mapWaitStates, "" terminated
extern const char* g_apszCamZooms[];               // 0x00498644 names for g_mapCamZooms, "" terminated
extern const char* g_apszEntityStatus[];           // 0x00498654 names for g_mapEntityStatus, "" terminated
extern const char* g_apszLevels[];                 // 0x0049868C names for g_mapLevels, "" terminated
extern const char* g_apszCounterDisplayTypes[];    // 0x004986A0 names for g_mapCounterDisplayTypes, "" terminated
extern const char* g_apszMotionBlurTypes[];        // 0x004986AC names for g_mapMotionBlurTypes, "" terminated
extern const char* g_apHudFlashItems[];            // 0x004986D8 name/value pairs for g_mapHudFlashItems, "" terminated
extern const char* g_apszCarDoors[];               // 0x00498740 names for g_mapCarDoors, "" terminated
extern const char* g_apszRadioStations[];          // 0x00498760 names for g_mapRadioStations, "" terminated
extern const char* g_apszFacingDirections[];       // 0x00498794 names for g_mapFacingDirections, "" terminated
extern const char* g_apszReplayCamModes[];         // 0x004987A8 names for g_mapReplayCamModes, "" terminated
extern const char* g_apszCarWheels[];              // 0x004987B8 names for g_mapCarWheels, "" terminated
extern const char* g_apszTempActions[];            // 0x004987D0 names for g_mapTempActions, "" terminated
extern const char* g_apszMoods[];                  // 0x00498834 names for g_mapMoods, "" terminated
extern const char* g_apszSetPieces[];              // 0x00498864 names for g_mapSetPieces, "" terminated
extern const char* g_apszTimerDirections[];        // 0x0049888C names for g_mapTimerDirections, "" terminated
extern const char* g_apBones[];                    // 0x00498898 name/value pairs for g_mapBones, "" terminated
extern const char* g_apszAnnouncementTracks[];     // 0x004989A0 names for g_mapAnnouncementTracks, "" terminated
extern const char* g_apszPedMoveStates[];          // 0x004989AC names for g_mapPedMoveStates, "" terminated
extern const char* g_apStats[];                    // 0x004989D0 name/value pairs for g_mapStats, "" terminated
extern const char* g_apszTaskStatus[];             // 0x00499360 names for g_mapTaskStatus, "" terminated
extern const char* g_apDuckingTypes[];             // 0x00499384 name/value pairs for g_mapDuckingTypes, "" terminated
extern const char* g_apszFollowRouteModes[];       // 0x004993AC names for g_mapFollowRouteModes, "" terminated
extern const char* g_apszSplineModes[];            // 0x004993C0 names for g_mapSplineModes, "" terminated
extern const char* g_apszCarLightOverrides[];      // 0x004993D4 names for g_mapCarLightOverrides, "" terminated
extern const char* g_apszCarPanels[];              // 0x004993E4 names for g_mapCarPanels, "" terminated
extern const char* g_apszDefaultTaskAllocators[];  // 0x00499408 names for g_mapDefaultTaskAllocators, "" terminated
extern const char* g_apszCheckpointTypes[];        // 0x00499424 names for g_mapCheckpointTypes, "" terminated
extern const char* g_apszVehicleUpgrades[];        // 0x00499450 names for g_mapVehicleUpgrades, "" terminated
extern const char* g_apszDriveByTypes[];           // 0x00499498 names for g_mapDriveByTypes, "" terminated
extern const char* g_apHoldOrientateTypes[];   // 0x004994C0 name/value pairs for g_mapHoldOrientateTypes, "" terminated
extern const char* g_apszPedBodyParts[];       // 0x004994D8 names for g_mapPedBodyParts, "" terminated
extern const char* g_apszKeyboardKeysPS2[];    // 0x00499528 keyboard keys for SAN_ANDREAS (PS2)
extern const char* g_apszKeyboardKeysPC[];     // 0x004996D8 keyboard keys for the PC and Xbox targets
extern const char* g_apszRadarModes[];         // 0x0049994C names for g_mapRadarModes, "" terminated
extern const char* g_apszAcquaintanceTypes[];  // 0x0049995C names for g_mapAcquaintanceTypes, "" terminated
extern const char* g_apszCarPlateDesigns[];    // 0x00499974 names for g_mapCarPlateDesigns, "" terminated
extern const char* g_apszStatIncrements[];     // 0x00499988 names for g_mapStatIncrements, "" terminated
extern const char* g_apszPopcycleZones[];      // 0x00499A78 names for g_mapPopcycleZones, "" terminated
extern const char* g_apszWinchTypes[];         // 0x00499AD0 names for g_mapWinchTypes, "" terminated
extern const char* g_apszPlaybackDisplayModes[];  // 0x00499AE4 names for g_mapPlaybackDisplayModes, "" terminated
extern const char* g_apszConversationStates[];    // 0x00499AFC names for g_mapConversationStates, "" terminated
extern const char*
    g_apDecisionMakerPedTypes[];           // 0x00499B10 name/value pairs for g_mapDecisionMakerPedTypes, "" terminated
extern const char*
    g_apszDecisionMakerMissionPedTypes[];  // 0x00499B68 names for g_mapDecisionMakerMissionPedTypes, "" terminated
extern const char* g_apszDecisionMakerGroupTypes[];  // 0x00499B80 names for g_mapDecisionMakerGroupTypes, "" terminated
extern const char* g_apMeleeCombos[];                // 0x00499B88 name/value pairs for g_mapMeleeCombos, "" terminated
extern const char* g_apszWeaponSkills[];             // 0x00499BB0 names for g_mapWeaponSkills, "" terminated
extern const char* g_apBuyableTypes[];               // 0x00499BC0 name/value pairs for g_mapBuyableTypes, "" terminated
extern const char* g_apszSelectedWeaponTypes[];      // 0x00499BD8 names for g_mapSelectedWeaponTypes, "" terminated
extern const char* g_apszInteriorTypes[];            // 0x00499BF0 names for g_mapInteriorTypes, "" terminated
extern const char* g_apszPopRaces[];                 // 0x00499C00 names for g_mapPopRaces, "" terminated
extern const char* g_apszBirdTypes[];                // 0x00499C44 names for g_mapBirdTypes, "" terminated
extern const char* g_apszFontOrientations[];         // 0x00499C54 names for g_mapFontOrientations, "" terminated
extern const char* g_apszCarClasses[];               // 0x00499C64 names for g_mapCarClasses, "" terminated
extern const char* g_apszKnockOffBikeTypes[];        // 0x00499C98 names for g_mapKnockOffBikeTypes, "" terminated
extern const char* g_apszSwirlTypes[];               // 0x00499CAC names for g_mapSwirlTypes, "" terminated
extern const char* g_apszClothesTextures[];          // 0x00499CC0 names for g_mapClothesTextures, "" terminated
extern const char* g_apszCoordBlipAppearances[];     // 0x00499D10 names for g_mapCoordBlipAppearances, "" terminated
extern const char* g_apszHudColours[];               // 0x00499D20 names for g_mapHudColours, "" terminated
extern const char* g_apszGlobalSpeechContexts[];     // 0x00499D68 speech contexts for SAN_ANDREAS and SAN_ANDREAS_XBOX
extern const char*
    g_apszGlobalSpeechContextsPC[];  // 0x0049A300 speech contexts for SAN_ANDREAS_PC and SAN_ANDREAS_XBOX360
extern const char* g_apszCutsceneTrackStates[];  // 0x0049A8A8 names for g_mapCutsceneTrackStates, "" terminated
extern const char* g_apszDoorDamageStates[];     // 0x0049A8C0 names for g_mapDoorDamageStates, "" terminated
extern const char* g_apszSwimStates[];           // 0x0049A8D8 names for g_mapSwimStates, "" terminated
extern const char* g_apEntryExitFlags[];         // 0x0049A8F8 name/value pairs for g_mapEntryExitFlags, "" terminated
extern const char* g_apZoomLevels[];             // 0x0049A978 name/value pairs for g_mapZoomLevels, "" terminated
extern const char* g_apszProcLevels[];           // 0x0049A9A8 names for g_mapProcLevels, "" terminated
extern const char* g_apszLanguages[];            // 0x0049A9B8 names for g_mapLanguages, "" terminated
extern const char* g_apszHandSignalAnims[];      // 0x0049A9D0 names for g_mapHandSignalAnims, "" terminated

// ------------------------------------------------------------------------------------- containers

extern std::map<std::string, int> g_mapFlareTypes;         // 0x01582FE8
extern std::map<std::string, int>
    g_aLocalVariableMaps[MAX_SCOPES];                      // 0x01582FF0 local variable name -> index, one per scope
extern std::map<std::string, int> g_mapPedTypes;           // 0x01583AE0
extern std::map<std::string, int> g_mapCoronaTypes;        // 0x01583AE8
extern std::map<std::string, int> g_mapCarLightOverrides;  // 0x01583AF0
extern std::map<std::string, int> g_mapWidgetIds;          // 0x01583AF8
extern std::map<std::string, int> g_mapCameraSwitchModes;  // 0x01583B00
extern std::map<std::string, int> g_mapPopcycleZones;      // 0x01583B08
extern std::map<std::string, int> g_mapZoomLevels;         // 0x01583B10
extern std::map<std::string, int> g_mapHudFlashItems;      // 0x01583B18
extern std::map<std::string, int> g_mapPedMoveStates;      // 0x01583B20
extern std::map<std::string, int> g_mapPads;               // 0x01583B28
extern std::set<std::string> g_aResourcesUsedByScript[NUM_SCRIPT_RESOURCES];  // 0x01583B30 models, sounds... used, see
                                                                              // WriteResourcesUsedByScriptFiles
extern std::map<std::string, int> g_mapHoldOrientateTypes;                    // 0x01583B68
extern std::map<std::string, int> g_mapAllEnumConstants;                      // 0x01583B70
extern std::map<std::string, int> g_mapCamZooms;                              // 0x01583B78
extern std::map<std::string, int> g_mapCarLockStates;                         // 0x01583B80
extern std::map<std::string, int> g_mapWeaponSkills;                          // 0x01583B88
extern std::map<std::string, int> g_mapRadarSprites;                          // 0x01583B90
extern std::map<std::string, int> g_mapCarPanels;                             // 0x01583B98
extern std::map<std::string, int> g_mapDefaultModels;                         // 0x01583BA0
extern std::map<std::string, int> g_mapCarMissions;                           // 0x01583BA8
extern std::map<std::string, int> g_mapTimerDirections;                       // 0x01583BB0
extern std::map<std::string, int> g_mapPedEventChoices;                       // 0x01583BB8
extern std::map<std::string, int> g_mapShadowTypes;                           // 0x01583BC0
extern std::map<std::string, int> g_mapRadarModes;                            // 0x01583BC8
extern std::map<std::string, int> g_mapEntityStatus;                          // 0x01583BD0
extern std::map<std::string, int> g_mapSoundEffects;                          // 0x01583BD8
extern std::map<std::string, int> g_mapBlipDisplayTypes;                      // 0x01583BE0
extern std::map<std::string, int> g_mapCutsceneTrackStates;                   // 0x01583BE8
extern std::map<std::string, int> g_mapDriveByTypes;                          // 0x01583BF0
extern std::map<std::string, int> g_mapMeleeCombos;                           // 0x01583BF8
extern std::map<std::string, int> g_mapIntConstants;                          // 0x01583C00
extern std::map<std::string, int> g_mapGangs;                                 // 0x01583C08
extern std::map<std::string, int> g_mapDecisionMakerPedTypes;                 // 0x01583C10
extern std::map<std::string, int> g_mapExplosionTypes;                        // 0x01583C18
extern std::map<std::string, int> g_mapClothesTextures;                       // 0x01583C20
extern std::map<std::string, int> g_mapObjectModels;                          // 0x01583C28
extern std::map<std::string, int> g_mapFacingDirections;                      // 0x01583C30
extern std::map<std::string, int> g_mapGroupEventChoices;                     // 0x01583C38
extern std::map<std::string, float> g_mapFloatConstants;                      // 0x01583C40
extern std::map<std::string, int> g_mapDecisionMakerMissionPedTypes;          // 0x01583C48
extern std::map<std::string, int> g_mapPedTaskChoices;                        // 0x01583C50
extern std::map<std::string, int> g_mapAnnouncementTracks;                    // 0x01583C58
extern std::map<std::string, int> g_mapWeaponTypes;                           // 0x01583C60
extern std::map<std::string, int> g_mapCounterDisplayTypes;                   // 0x01583C68
extern std::set<std::string> g_mapAllLocalVariableNames;         // 0x01583C70 every local variable name of every scope
extern std::map<std::string, int> g_mapPadSticks;                // 0x01583C78
extern std::map<std::string, int> g_mapCheckpointTypes;          // 0x01583C80
extern std::map<std::string, int> g_mapProcLevels;               // 0x01583C88
extern std::map<std::string, int> g_mapEntryExitFlags;           // 0x01583C90
extern std::map<std::string, int> g_mapPlaybackDisplayModes;     // 0x01583C98
extern std::map<std::string, int> g_mapCommandNames;             // 0x01583CA0 command name -> command id
extern std::map<std::string, int> g_mapFonts;                    // 0x01583CA8
extern std::map<std::string, int> g_mapHIDMapping;               // 0x01583CB0
extern std::map<std::string, int> g_mapDecisionMakerGroupTypes;  // 0x01583CB8
extern std::map<std::string, int> g_mapLanguages;                // 0x01583CC0
extern std::map<std::string, int> g_mapHandSignalAnims;          // 0x01583CC8
extern std::map<std::string, int> g_mapInteriorTypes;            // 0x01583CD0
extern std::map<std::string, int> g_mapConversationStates;       // 0x01583CD8
extern std::map<std::string, int> g_mapFontOrientations;         // 0x01583CE0
extern std::map<std::string, int> g_mapRadioStations;            // 0x01583CE8
extern std::map<std::string, int> g_mapBones;                    // 0x01583CF0
extern std::map<std::string, int> g_mapHudColours;               // 0x01583CF8
extern std::map<std::string, int> g_mapSplineModes;              // 0x01583D00
extern std::map<std::string, int> g_mapDefaultTaskAllocators;    // 0x01583D08
extern std::map<std::string, int> g_mapCarColours;               // 0x01583D10
extern std::map<std::string, int> g_mapGarageTypes;              // 0x01583D18
extern std::map<std::string, int> g_mapSelectedWeaponTypes;      // 0x01583D20
extern std::map<std::string, int> g_mapFadeTypes;                // 0x01583D28
extern std::map<std::string, int> g_mapCarWheels;                // 0x01583D30
extern std::map<std::string, int> g_mapKeyboardKeys;             // 0x01583D38
extern std::map<std::string, int> g_mapCarBombTypes;             // 0x01583D40
extern std::map<std::string, int> g_mapDrivingModes;             // 0x01583D48
extern std::map<std::string, int> g_mapPedBodyParts;             // 0x01583D50
extern std::map<std::string, int> g_mapDuckingTypes;             // 0x01583D58
extern std::map<std::string, int> g_mapSwirlTypes;               // 0x01583D60
extern std::map<std::string, int> g_mapCameraModes;              // 0x01583D68
extern std::map<std::string, int> g_mapCarPlateDesigns;          // 0x01583D70
extern std::map<std::string, int> g_mapFormations;               // 0x01583D78
extern std::map<std::string, int> g_mapKillFrenzyStates;         // 0x01583D80
extern std::map<std::string, int> g_mapKnockOffBikeTypes;        // 0x01583D88
extern std::map<std::string, int> g_mapGlobalSpeechContexts;     // 0x01583D90
extern std::map<std::string, int> g_mapCoordBlipAppearances;     // 0x01583D98
extern std::map<std::string, int> g_mapFollowRouteModes;         // 0x01583DA0
extern std::map<std::string, int> g_mapDoorDamageStates;         // 0x01583DA8
extern std::map<std::string, int> g_mapWaitStates;               // 0x01583DB0
extern std::map<std::string, int> g_mapReplayCamModes;           // 0x01583DB8
extern std::map<std::string, int> g_mapBuyableTypes;             // 0x01583DC0
extern std::map<std::string, int> g_mapStats;                    // 0x01583DC8
extern std::map<std::string, int> g_mapStatIncrements;           // 0x01583DD0
extern std::map<std::string, int> g_mapColours;                  // 0x01583DD8
extern std::map<std::string, int> g_mapCarClasses;               // 0x01583DE0
extern std::map<std::string, int> g_mapMotionBlurTypes;          // 0x01583DE8
extern std::map<std::string, int> g_mapWinchTypes;               // 0x01583DF0
extern std::map<std::string, int> g_mapTempActions;              // 0x01583DF8
extern std::map<std::string, int> g_mapWeatherTypes;             // 0x01583E00
extern std::map<std::string, int> g_mapPopRaces;                 // 0x01583E08
extern std::map<std::string, int> g_mapPickupTypes;              // 0x01583E10
extern std::map<std::string, int> g_mapCarDoors;                 // 0x01583E18
extern std::map<std::string, int> g_mapLabels;                   // 0x01583E20 label -> line
extern std::map<std::string, int> g_mapVehicleUpgrades;          // 0x01583E28
extern std::map<std::string, int> g_mapLevels;                   // 0x01583E30
extern std::map<std::string, int> g_mapSwimStates;               // 0x01583E38
extern std::map<std::string, int> g_mapTaskStatus;               // 0x01583E40
extern std::map<std::string, int> g_mapMoods;                    // 0x01583E48
extern std::map<std::string, int> g_mapAcquaintanceTypes;        // 0x01583E50
extern std::map<std::string, int> g_mapGlobalVariables;          // 0x01583E58 global variable name -> index
extern std::map<std::string, int> g_mapGroupTaskChoices;         // 0x01583E60
extern std::map<std::string, int> g_mapSetPieces;                // 0x01583E68
extern std::map<std::string, int> g_mapBirdTypes;                // 0x01583E70

// namemaps.cpp

bool AddToMap(std::map<std::string, int>* pMap, const char* pszName, int nValue, bool bCheckSoundPrefix);  // 0x0040AD30
bool AddToFloatMap(
    std::map<std::string, float>* pMap, const char* pszName, float fValue, bool bCheckSoundPrefix);        // 0x0040AE90
void AddToSet(std::set<std::string>* pSet, const char* pszName);                                           // 0x0040B150
void AddStringTableToMap(
    std::map<std::string, int>* pMap, const char** apszNames, bool bCheckSoundPrefix);                     // 0x0040B330
void AddNamedIdArrayToMap(
    std::map<std::string, int>* pMap, const tNamedId* aNamedIds, bool bCheckSoundPrefix);                  // 0x0040B650
void AddStringValuePairsToMap(
    std::map<std::string, int>* pMap, const char** apPairs, bool bCheckSoundPrefix);                       // 0x0040B970
int GetValueFromMap(const char* pszName, const std::map<std::string, int>* pMap, bool bRecordUsage);       // 0x0040BC90
bool FindInMap(const char* pszName, std::map<std::string, int>* pMap, int* pnValue, bool bRecordUsage);    // 0x0040BE30
bool FindInFloatMap(const char* pszName, std::map<std::string, float>* pMap, float* pfValue);              // 0x0040BFE0
void BuildConstantMaps();                                                                                  // 0x0040C120
bool IsInSet(const char* pszName, std::set<std::string>* pSet);                                            // 0x0040CB50
void WriteResourcesUsedByScriptFiles();                                                                    // 0x0040CC80
void ClearAllNameMaps();                                                                                   // 0x0040CEE0
