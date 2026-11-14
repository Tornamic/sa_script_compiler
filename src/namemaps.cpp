// namemaps.cpp: 0x40AD30-0x40D0B6 in sc.exe

#include "common.h"
#include "main.h"
#include "codegen.h"
#include "loader.h"
#include "parser.h"
#include "namemaps.h"

// 0x00497D70
const char* g_apszCameraModes[] = {
    "NONE",
    "TOPDOWN",
    "GTACLASSIC",
    "BEHINDCAR",
    "FOLLOWPED",
    "AIMING",
    "DEBUG",
    "SNIPER",
    "ROCKETLAUNCHER",
    "MODELVIEW",
    "BILL",
    "SYPHON",
    "CIRCLE_CAMERA",
    "CHEESYZOOM",
    "WHEELCAM",
    "FIXED",
    "FIRST_PERSON",
    "FLYBY",
    "CAM_ON_A_STRING",
    "REACTION",
    "FOLLOW_PED_WITH_BIND",
    "CHRIS",
    "BEHINDBOAT",
    "PLAYER_FALLEN_WATER",
    "CAM_ON_TRAIN_ROOF",
    "CAM_RUNNING_SIDE_TRAIN",
    "BLOOD_ON_THE_TRACKS",
    "IM_THE_PASSENGER_WOOWOO",
    "SYPHON_CRIM_IN_FRONT",
    "PED_DEAD_BABY",
    "PILLOWS_PAPS",
    "LOOK_AT_CARS",
    "ARRESTCAM_ONE",
    "ARRESTCAM_TWO",
    "M16_1STPERSON",
    "SPECIAL_FIXED_FOR_SYPHON",
    "FIGHT_CAM",
    "TOP_DOWN_PED",
    "LIGHTHOUSE",
    "SNIPER_RUNABOUT",
    "ROCKETLAUNCHER_RUNABOUT",
    "FIRST_PERSON_RUNABOUT",
    "M16_1STPERSON_RUNABOUT",
    "FIGHT_CAM_RUNABOUT",
    "EDITOR",
    "HELICANNON_1STPERSON",
    "CAMERA",
    "ATTACHCAM",
    "TWOPLAYER",
    "TWOPLAYER_IN_CAR_AND_SHOOTING",
    "TWOPLAYER_SEPARATE_CARS",
    "ROCKETLAUNCHER_HS",
    "ROCKETLAUNCHER_RUNABOUT_HS",
    "AIMWEAPON",
    "TWOPLAYER_SEPARATE_CARS_TOPDOWN",
    "MODE_AIMWEAPON_FROMCAR",
    "",
};

// 0x00497E54
const char* g_apszPads[] = {
    "PAD1",
    "PAD2",
    "",
};

// 0x00497E60
const char* g_apszPadSticks[] = {
    "LEFTSTICKX",
    "LEFTSTICKY",
    "RIGHTSTICKX",
    "RIGHTSTICKY",
    "LEFTSHOULDER1",
    "LEFTSHOULDER2",
    "RIGHTSHOULDER1",
    "RIGHTSHOULDER2",
    "DPADUP",
    "DPADDOWN",
    "DPADLEFT",
    "DPADRIGHT",
    "START",
    "SELECT",
    "SQUARE",
    "TRIANGLE",
    "CROSS",
    "CIRCLE",
    "LEFTSHOCK",
    "RIGHTSHOCK",
    "",
};

// 0x00497EB4
const char* g_apszBlipDisplayTypes[] = {
    "NEITHER",
    "MARKER_ONLY",
    "BLIP_ONLY",
    "BOTH",
    "",
};

// 0x00497EC8
const char* g_apszPedTypes[] = {
    "PEDTYPE_PLAYER1",
    "PEDTYPE_PLAYER2",
    "PEDTYPE_PLAYER_NETWORK",
    "PEDTYPE_PLAYER_UNUSED",
    "PEDTYPE_CIVMALE",
    "PEDTYPE_CIVFEMALE",
    "PEDTYPE_COP",
    "PEDTYPE_GANG_FLAT",
    "PEDTYPE_GANG_GROVE",
    "PEDTYPE_GANG_NMEX",
    "PEDTYPE_GANG_SFMEX",
    "PEDTYPE_GANG_VIET",
    "PEDTYPE_GANG_MAFIA",
    "PEDTYPE_GANG_TRIAD",
    "PEDTYPE_GANG_SMEX",
    "PEDTYPE_GANG9",
    "PEDTYPE_GANG10",
    "PEDTYPE_DEALER",
    "PEDTYPE_MEDIC",
    "PEDTYPE_FIRE",
    "PEDTYPE_CRIMINAL",
    "PEDTYPE_BUM",
    "PEDTYPE_PROSTITUTE",
    "PEDTYPE_SPECIAL",
    "PEDTYPE_MISSION1",
    "PEDTYPE_MISSION2",
    "PEDTYPE_MISSION3",
    "PEDTYPE_MISSION4",
    "PEDTYPE_MISSION5",
    "PEDTYPE_MISSION6",
    "PEDTYPE_MISSION7",
    "PEDTYPE_MISSION8",
    "",
};

// 0x00497F4C
const char* g_apszColours[] = {
    "RED",
    "GREEN",
    "BLUE",
    "WHITE",
    "YELLOW",
    "PURPLE",
    "CYAN",
    "",
};

// 0x00497F6C
const char* g_apszCameraSwitchModes[] = {
    "NONE",
    "INTERPOLATION",
    "JUMP_CUT",
    "",
};

// 0x00497F7C
const char* g_apszFadeTypes[] = {
    "FADE_OUT",
    "FADE_IN",
    "",
};

// 0x00497F88
const char* g_apszShadowTypes[] = {
    "SHADOW_NONE",
    "SHADOW_CAR",
    "SHADOW_PED",
    "SHADOW_EXPLOSION",
    "SHADOW_HELI",
    "SHADOW_HEADLIGHTS",
    "SHADOW_BLOOD",
    "",
};

// 0x00497FA8
const char* g_apszWeaponTypes[] = {
    "WEAPONTYPE_UNARMED",
    "WEAPONTYPE_BRASSKNUCKLE",
    "WEAPONTYPE_GOLFCLUB",
    "WEAPONTYPE_NIGHTSTICK",
    "WEAPONTYPE_KNIFE",
    "WEAPONTYPE_BASEBALLBAT",
    "WEAPONTYPE_SHOVEL",
    "WEAPONTYPE_POOL_CUE",
    "WEAPONTYPE_KATANA",
    "WEAPONTYPE_CHAINSAW",
    "WEAPONTYPE_DILDO1",
    "WEAPONTYPE_DILDO2",
    "WEAPONTYPE_VIBE1",
    "WEAPONTYPE_VIBE2",
    "WEAPONTYPE_FLOWERS",
    "WEAPONTYPE_CANE",
    "WEAPONTYPE_GRENADE",
    "WEAPONTYPE_TEARGAS",
    "WEAPONTYPE_MOLOTOV",
    "WEAPONTYPE_ROCKET",
    "WEAPONTYPE_ROCKET_HS",
    "WEAPONTYPE_FREEFALL_BOMB",
    "WEAPONTYPE_PISTOL",
    "WEAPONTYPE_PISTOL_SILENCED",
    "WEAPONTYPE_DESERT_EAGLE",
    "WEAPONTYPE_SHOTGUN",
    "WEAPONTYPE_SAWNOFF_SHOTGUN",
    "WEAPONTYPE_SPAS12_SHOTGUN",
    "WEAPONTYPE_MICRO_UZI",
    "WEAPONTYPE_MP5",
    "WEAPONTYPE_AK47",
    "WEAPONTYPE_M4",
    "WEAPONTYPE_TEC9",
    "WEAPONTYPE_COUNTRYRIFLE",
    "WEAPONTYPE_SNIPERRIFLE",
    "WEAPONTYPE_ROCKETLAUNCHER",
    "WEAPONTYPE_ROCKETLAUNCHER_HS",
    "WEAPONTYPE_FLAMETHROWER",
    "WEAPONTYPE_MINIGUN",
    "WEAPONTYPE_REMOTE_SATCHEL_CHARGE",
    "WEAPONTYPE_DETONATOR",
    "WEAPONTYPE_SPRAYCAN",
    "WEAPONTYPE_EXTINGUISHER",
    "WEAPONTYPE_CAMERA",
    "WEAPONTYPE_NIGHTVISION",
    "WEAPONTYPE_INFRARED",
    "WEAPONTYPE_PARACHUTE",
    "WEAPONTYPE_LAST_WEAPONTYPE",
    "WEAPONTYPE_ARMOUR",
    "WEAPONTYPE_RAMMEDBYCAR",
    "WEAPONTYPE_RUNOVERBYCAR",
    "WEAPONTYPE_EXPLOSION",
    "WEAPONTYPE_UZI_DRIVEBY",
    "WEAPONTYPE_DROWNING",
    "WEAPONTYPE_FALL",
    "WEAPONTYPE_UNIDENTIFIED",
    "WEAPONTYPE_ANYMELEE",
    "WEAPONTYPE_ANYWEAPON",
    "",
};

// 0x00498094
const char* g_apszCarLockStates[] = {
    "CARLOCK_NONE",
    "CARLOCK_UNLOCKED",
    "CARLOCK_LOCKED",
    "CARLOCK_LOCKOUT_PLAYER_ONLY",
    "CARLOCK_LOCKED_PLAYER_INSIDE",
    "CARLOCK_LOCKED_INITIALLY",
    "CARLOCK_FORCE_SHUT_DOORS",
    "CARLOCK_LOCKED_BUT_CAN_BE_DAMAGED",
    "",
};

// 0x004980B8
const char* g_apszWeatherTypes[] = {
    "WEATHER_EXTRASUNNY_LA",
    "WEATHER_SUNNY_LA",
    "WEATHER_EXTRASUNNY_SMOG_LA",
    "WEATHER_SUNNY_SMOG_LA",
    "WEATHER_CLOUDY_LA",
    "WEATHER_SUNNY_SF",
    "WEATHER_EXTRASUNNY_SF",
    "WEATHER_CLOUDY_SF",
    "WEATHER_RAINY_SF",
    "WEATHER_FOGGY_SF",
    "WEATHER_SUNNY_VEGAS",
    "WEATHER_EXTRASUNNY_VEGAS",
    "WEATHER_CLOUDY_VEGAS",
    "WEATHER_EXTRASUNNY_COUNTRYSIDE",
    "WEATHER_SUNNY_COUNTRYSIDE",
    "WEATHER_CLOUDY_COUNTRYSIDE",
    "WEATHER_RAINY_COUNTRYSIDE",
    "WEATHER_EXTRASUNNY_DESERT",
    "WEATHER_SUNNY_DESERT",
    "WEATHER_SANDSTORM_DESERT",
    "WEATHER_UNDERWATER",
    "WEATHER_EXTRACOLOURS_1",
    "WEATHER_EXTRACOLOURS_2",
    "",
};

// 0x00498118
const char* g_apszKillFrenzyStates[] = {
    "KILLFRENZY_INITIALLY",
    "KILLFRENZY_ONGOING",
    "KILLFRENZY_PASSED",
    "KILLFRENZY_FAILED",
    "KILLFRENZY_2PONGOING",
    "",
};

// 0x00498130
const char* g_apszExplosionTypes[] = {
    "EXPLOSION_GRENADE",
    "EXPLOSION_MOLOTOV",
    "EXPLOSION_ROCKET",
    "EXPLOSION_ROCKET_WEAK",
    "EXPLOSION_CAR",
    "EXPLOSION_CAR_QUICK",
    "EXPLOSION_BOAT",
    "EXPLOSION_HELI",
    "EXPLOSION_MINE",
    "EXPLOSION_OBJECT",
    "EXPLOSION_TANK_GRENADE",
    "EXPLOSION_SMALL",
    "EXPLOSION_TINY",
    "",
};

// 0x00498168
const char* g_apszPickupTypes[] = {
    "PICKUP_NONE",
    "PICKUP_IN_SHOP",
    "PICKUP_ON_STREET",
    "PICKUP_ONCE",
    "PICKUP_ONCE_TIMEOUT",
    "PICKUP_ONCE_TIMEOUT_SLOW",
    "PICKUP_COLLECTABLE1",
    "PICKUP_IN_SHOP_OUT_OF_STOCK",
    "PICKUP_MONEY",
    "PICKUP_MINE_INACTIVE",
    "PICKUP_MINE_ARMED",
    "PICKUP_NAUTICAL_MINE_INACTIVE",
    "PICKUP_NAUTICAL_MINE_ARMED",
    "PICKUP_FLOATINGPACKAGE",
    "PICKUP_FLOATINGPACKAGE_FLOATING",
    "PICKUP_ON_STREET_SLOW",
    "PICKUP_ASSET_REVENUE",
    "PICKUP_PROPERTY_LOCKED",
    "PICKUP_PROPERTY_FORSALE",
    "PICKUP_MONEY_DOESNTDISAPPEAR",
    "PICKUP_SNAPSHOT",
    "PICKUP_2P",
    "PICKUP_ONCE_FOR_MISSION",
    "",
};

// 0x004981C8
const char* g_apszGarageTypes[] = {
    "GARAGE_NONE",
    "GARAGE_MISSION",
    "GARAGE_BOMBSHOP1",
    "GARAGE_BOMBSHOP2",
    "GARAGE_BOMBSHOP3",
    "GARAGE_RESPRAY",
    "GARAGE_COLLECTORSITEMS",
    "GARAGE_COLLECTSPECIFICCARS",
    "GARAGE_COLLECTCARS_1",
    "GARAGE_COLLECTCARS_2",
    "GARAGE_COLLECTCARS_3",
    "GARAGE_FORCARTOCOMEOUTOF",
    "GARAGE_60SECONDS",
    "GARAGE_CRUSHER",
    "GARAGE_MISSION_KEEPCAR",
    "GARAGE_FOR_SCRIPT_TO_OPEN",
    "GARAGE_HIDEOUT_ONE",
    "GARAGE_HIDEOUT_TWO",
    "GARAGE_HIDEOUT_THREE",
    "GARAGE_FOR_SCRIPT_TO_OPEN_AND_CLOSE",
    "GARAGE_KEEPS_OPENING_FOR_SPECIFIC_CAR",
    "GARAGE_MISSION_KEEPCAR_REMAINCLOSED",
    "GARAGE_COLLECTCARS_4",
    "GARAGE_FOR_SCRIPT_TO_OPEN_FOR_CAR",
    "GARAGE_HIDEOUT_FOUR",
    "GARAGE_HIDEOUT_FIVE",
    "GARAGE_HIDEOUT_SIX",
    "GARAGE_HIDEOUT_SEVEN",
    "GARAGE_HIDEOUT_EIGHT",
    "GARAGE_HIDEOUT_NINE",
    "GARAGE_HIDEOUT_TEN",
    "GARAGE_HIDEOUT_ELEVEN",
    "GARAGE_HIDEOUT_TWELVE",
    "GARAGE_POLICE_IMPOUND_LA",
    "GARAGE_POLICE_IMPOUND_SF",
    "GARAGE_POLICE_IMPOUND_LV",
    "GARAGE_OPEN_FOR_LOW_RIDERS",
    "GARAGE_OPEN_FOR_STREET_RACERS",
    "GARAGE_OPEN_FOR_NORMAL_CARS",
    "GARAGE_HIDEOUT_THIRTEEN",
    "GARAGE_HIDEOUT_FOURTEEN",
    "GARAGE_HIDEOUT_FIFTEEN",
    "GARAGE_HIDEOUT_SIXTEEN",
    "GARAGE_CLOSE_WHEN_EMPTY",
    "GARAGE_HIDEOUT_SEVENTEEN",
    "GARAGE_HIDEOUT_EIGHTEEN",
    "GARAGE_HIDEOUT_NINETEEN",
    "",
};

// 0x00498288
const char* g_apszCarBombTypes[] = {
    "CARBOMB_NONE",
    "CARBOMB_TIMED",
    "CARBOMB_ONIGNITION",
    "CARBOMB_REMOTE",
    "CARBOMB_TIMEDACTIVE",
    "CARBOMB_ONIGNITIONACTIVE",
    "",
};

// 0x004982A4
const char* g_apszCarColours[] = {
    "CARCOLOUR_BLACK",
    "CARCOLOUR_WHITE",
    "CARCOLOUR_POLICECARBLUE",
    "CARCOLOUR_CHERRYRED",
    "CARCOLOUR_MIDNIGHTBLUE",
    "CARCOLOUR_TEMPLECURTAINPURPLE",
    "CARCOLOUR_TAXIYELLOW",
    "CARCOLOUR_STRIKINGBLUE",
    "CARCOLOUR_LIGHTBLUEGREY",
    "",
};

// 0x004982CC
const char* g_apszGangs[] = {
    "GANG_FLAT",
    "GANG_GROVE",
    "GANG_NMEX",
    "GANG_SFMEX",
    "GANG_VIET",
    "GANG_MAFIA",
    "GANG_TRIAD",
    "GANG_SMEX",
    "GANG9",
    "GANG10",
    "",
};

// 0x004982F8
const char* g_apszCoronaTypes[] = {
    "CORONATYPE_SHINYSTAR",
    "CORONATYPE_HEADLIGHT",
    "CORONATYPE_MOON",
    "CORONATYPE_REFLECTION",
    "CORONATYPE_HEADLIGHTLINE",
    "PAD_5",
    "PAD_6",
    "PAD_7",
    "CORONATYPE_STREAK",
    "CORONATYPE_TORUS",
    "CORONATYPE_NONE",
    "",
};

// 0x00498328
const char* g_apszFlareTypes[] = {
    "FLARETYPE_NONE",
    "FLARETYPE_SUN",
    "FLARETYPE_HEADLIGHTS",
    "",
};

// 0x00498338
const char* g_apszRadarSprites[] = {
    "RADAR_SPRITE_NONE",
    "RADAR_SPRITE_BORDER",
    "RADAR_SPRITE_CENTRE",
    "RADAR_SPRITE_MAP_HERE",
    "RADAR_SPRITE_NORTH",
    "RADAR_SPRITE_AIRYARD",
    "RADAR_SPRITE_GUN",
    "RADAR_SPRITE_BARBERS",
    "RADAR_SPRITE_BIG_SMOKE",
    "RADAR_SPRITE_BOATYARD",
    "RADAR_SPRITE_BURGERSHOT",
    "RADAR_SPRITE_BULLDOZER",
    "RADAR_SPRITE_CAT_PINK",
    "RADAR_SPRITE_CESAR",
    "RADAR_SPRITE_CHICKEN",
    "RADAR_SPRITE_CJ",
    "RADAR_SPRITE_CRASH1",
    "RADAR_SPRITE_DINER",
    "RADAR_SPRITE_EMMETGUN",
    "RADAR_SPRITE_ENEMYATTACK",
    "RADAR_SPRITE_FIRE",
    "RADAR_SPRITE_GIRLFRIEND",
    "RADAR_SPRITE_HOSPITAL",
    "RADAR_SPRITE_LOCO",
    "RADAR_SPRITE_MADDOG",
    "RADAR_SPRITE_MAFIA",
    "RADAR_SPRITE_MCSTRAP",
    "RADAR_SPRITE_MOD_GARAGE",
    "RADAR_SPRITE_OGLOC",
    "RADAR_SPRITE_PIZZA",
    "RADAR_SPRITE_POLICE",
    "RADAR_SPRITE_PROPERTY_GREEN",
    "RADAR_SPRITE_PROPERTY_RED",
    "RADAR_SPRITE_RACE",
    "RADAR_SPRITE_RYDER",
    "RADAR_SPRITE_SAVEHOUSE",
    "RADAR_SPRITE_SCHOOL",
    "RADAR_SPRITE_MYSTERY",
    "RADAR_SPRITE_SWEET",
    "RADAR_SPRITE_TATTOO",
    "RADAR_SPRITE_TRUTH",
    "RADAR_SPRITE_WAYPOINT",
    "RADAR_SPRITE_TORENO_RANCH",
    "RADAR_SPRITE_TRIADS",
    "RADAR_SPRITE_TRIADS_CASINO",
    "RADAR_SPRITE_TSHIRT",
    "RADAR_SPRITE_WOOZIE",
    "RADAR_SPRITE_ZERO",
    "RADAR_SPRITE_DATE_DISCO",
    "RADAR_SPRITE_DATE_DRINK",
    "RADAR_SPRITE_DATE_FOOD",
    "RADAR_SPRITE_TRUCK",
    "RADAR_SPRITE_CASH",
    "RADAR_SPRITE_FLAG",
    "RADAR_SPRITE_GYM",
    "RADAR_SPRITE_IMPOUND",
    "RADAR_SPRITE_RUNWAY_LIGHT",
    "RADAR_SPRITE_RUNWAY",
    "RADAR_SPRITE_GANG_B",
    "RADAR_SPRITE_GANG_P",
    "RADAR_SPRITE_GANG_Y",
    "RADAR_SPRITE_GANG_N",
    "RADAR_SPRITE_GANG_G",
    "RADAR_SPRITE_SPRAY",
    "",
};

// 0x0049843C
const char* g_apszDrivingModes[] = {
    "DRIVINGMODE_STOPFORCARS",
    "DRIVINGMODE_SLOWDOWNFORCARS",
    "DRIVINGMODE_AVOIDCARS",
    "DRIVINGMODE_PLOUGHTHROUGH",
    "DRIVINGMODE_STOPFORCARS_IGNORELIGHTS",
    "DRIVINGMODE_AVOIDCARS_OBEYLIGHTS",
    "DRIVINGMODE_AVOIDCARS_STOPFORPEDS_OBEYLIGHTS",
    "",
};

// 0x0049845C
const char* g_apszFonts[] = {
    "FONT_BANK",
    "FONT_STANDARD",
    "FONT_SPACEAGE",
    "FONT_HEADING",
    "",
};

// 0x00498470
const char* g_apszCarMissions[] = {
    "MISSION_NONE",
    "MISSION_CRUISE",
    "MISSION_RAMPLAYER_FARAWAY",
    "MISSION_RAMPLAYER_CLOSE",
    "MISSION_BLOCKPLAYER_FARAWAY",
    "MISSION_BLOCKPLAYER_CLOSE",
    "MISSION_BLOCKPLAYER_HANDBRAKESTOP",
    "MISSION_WAITFORDELETION",
    "MISSION_GOTOCOORDS",
    "MISSION_GOTOCOORDS_STRAIGHT",
    "MISSION_EMERGENCYVEHICLE_STOP",
    "MISSION_STOP_FOREVER",
    "MISSION_GOTOCOORDS_ACCURATE",
    "MISSION_GOTOCOORDS_STRAIGHT_ACCURATE",
    "MISSION_GOTOCOORDS_ASTHECROWSWIMS",
    "MISSION_RAMCAR_FARAWAY",
    "MISSION_RAMCAR_CLOSE",
    "MISSION_BLOCKCAR_FARAWAY",
    "MISSION_BLOCKCAR_CLOSE",
    "MISSION_BLOCKCAR_HANDBRAKESTOP",
    "MISSION_HELI_FLYTOCOORS",
    "MISSION_BOAT_ATTACK_PLAYER",
    "MISSION_PLANE_FLYTOCOORS",
    "MISSION_HELI_ATTACK_PLAYER",
    "MISSION_SLOWLY_DRIVE_TOWARDS_PLAYER_1",
    "MISSION_SLOWLY_DRIVE_TOWARDS_PLAYER_2",
    "MISSION_BLOCKPLAYER_FORWARDANDBACK",
    "MISSION_PROTECTION_REAR",
    "MISSION_PROTECTION_FRONT",
    "MISSION_ESCORT_LEFT",
    "MISSION_ESCORT_RIGHT",
    "MISSION_ESCORT_REAR",
    "MISSION_ESCORT_FRONT",
    "MISSION_GOTOCOORDS_RACING",
    "MISSION_FOLLOW_RECORDED_PATH",
    "MISSION_PLANE_ATTACK_PLAYER",
    "MISSION_PLANE_FLYINDIRECTION",
    "MISSION_PLANE_FOLLOW_ENTITY",
    "MISSION_HELI_FLYINDIRECTION",
    "MISSION_HELI_FOLLOW_ENTITY",
    "MISSION_HELI_POLICE_BEHAVIOUR",
    "MISSION_HELI_NEWS_BEHAVIOUR",
    "MISSION_HELI_FLY_AWAY_FROM_PLAYER",
    "MISSION_APPROACHPLAYER_FARAWAY",
    "MISSION_APPROACHPLAYER_CLOSE",
    "MISSION_PARK_PERPENDICULAR",
    "MISSION_PARK_PARALLEL",
    "MISSION_HELI_LAND",
    "MISSION_HELI_LAND_TOUCHING_DOWN",
    "MISSION_PARK_PERPENDICULAR_2",
    "MISSION_PARK_PARALLEL_2",
    "MISSION_HELI_KEEP_ENTITY_IN_VIEW",
    "MISSION_FOLLOWCAR_FARAWAY",
    "MISSION_FOLLOWCAR_CLOSE",
    "MISSION_KILLPED_FARAWAY",
    "MISSION_KILLPED_CLOSE",
    "MISSION_HELI_ATTACK_PLAYER_FLY_AWAY",
    "MISSION_PLANE_CRASH_AND_BURN",
    "MISSION_HELI_CRASH_AND_BURN",
    "MISSION_PLANE_ATTACK_PLAYER_POLICE",
    "MISSION_DO_DRIVEBY_CLOSE",
    "MISSION_DO_DRIVEBY_FARAWAY",
    "MISSION_PLANE_DOG_FIGHT_ENTITY",
    "MISSION_PLANE_DOG_FIGHT_PLAYER",
    "MISSION_BOAT_CIRCLEPLAYER",
    "MISSION_ESCORT_LEFT_FARAWAY",
    "MISSION_ESCORT_RIGHT_FARAWAY",
    "MISSION_ESCORT_REAR_FARAWAY",
    "MISSION_ESCORT_FRONT_FARAWAY",
    "",
};

// 0x00498588
const char* g_apszFormations[] = {
    "FORMATION_NONE",
    "FORMATION_BEHIND",
    "FORMATION_BACKLEFT",
    "FORMATION_BACKRIGHT",
    "FORMATION_FRONTLEFT",
    "FORMATION_FRONTRIGHT",
    "FORMATION_LEFT",
    "FORMATION_RIGHT",
    "FORMATION_FRONT",
    "",
};

// 0x004985B0
const char* g_apszWaitStates[] = {
    "WAITSTATE_FALSE",
    "WAITSTATE_TRAFFIC_LIGHTS",
    "WAITSTATE_CROSS_ROAD",
    "WAITSTATE_CROSS_ROAD_LOOK",
    "WAITSTATE_LOOK_PED",
    "WAITSTATE_LOOK_SHOP",
    "WAITSTATE_LOOK_ACCIDENT",
    "WAITSTATE_FACEOFF_GANG",
    "WAITSTATE_DOUBLEBACK",
    "WAITSTATE_HITWALL",
    "WAITSTATE_TURN180",
    "WAITSTATE_SURPRISE",
    "WAITSTATE_STUCK",
    "WAITSTATE_LOOK_ABOUT",
    "WAITSTATE_PLAYANIM_DUCK",
    "WAITSTATE_PLAYANIM_COWER",
    "WAITSTATE_PLAYANIM_TAXI",
    "WAITSTATE_PLAYANIM_HANDSUP",
    "WAITSTATE_PLAYANIM_HANDSCOWER",
    "WAITSTATE_PLAYANIM_CHAT",
    "WAITSTATE_FINISH_FLEE",
    "WAITSTATE_SIT_DOWN",
    "WAITSTATE_SIT_DOWN_RVRS",
    "WAITSTATE_SIT_UP",
    "WAITSTATE_SIT_IDLE",
    "WAITSTATE_USE_ATM",
    "WAITSTATE_SUN_BATHE_PRE",
    "WAITSTATE_SUN_BATHE_DOWN",
    "WAITSTATE_SUN_BATHE_IDLE",
    "WAITSTATE_RIOT",
    "WAITSTATE_FAST_FALL",
    "WAITSTATE_BOMBER",
    "WAITSTATE_STRIPPER",
    "WAITSTATE_GROUND_ATTACK",
    "WAITSTATE_LANCESITTING",
    "WAITSTATE_PLAYANIM_HANDSUP_SIMPLE",
    "",
};

// 0x00498644
const char* g_apszCamZooms[] = {
    "CAM_ZOOM_ONE",
    "CAM_ZOOM_TWO",
    "CAM_ZOOM_THREE",
    "",
};

// 0x00498654
const char* g_apszEntityStatus[] = {
    "STATUS_PLAYER",
    "STATUS_PLAYER_PLAYBACKFROMBUFFER",
    "STATUS_SIMPLE",
    "STATUS_PHYSICS",
    "STATUS_ABANDONED",
    "STATUS_WRECKED",
    "STATUS_TRAIN_MOVING",
    "STATUS_TRAIN_NOT_MOVING",
    "STATUS_PLAYER_REMOTE",
    "STATUS_PLAYER_DISABLED",
    "STATUS_TRAILER",
    "STATUS_SIMPLE_TRAILER",
    "STATUS_GHOST",
    "",
};

// 0x0049868C
const char* g_apszLevels[] = {
    "LEVEL_GENERIC",
    "LEVEL_LOSANGELES",
    "LEVEL_SANFRANCISCO",
    "LEVEL_LASVEGAS",
    "",
};

// 0x004986A0
const char* g_apszCounterDisplayTypes[] = {
    "COUNTER_DISPLAY_NUMBER",
    "COUNTER_DISPLAY_BAR",
    "",
};

// 0x004986AC
const char* g_apszMotionBlurTypes[] = {
    "MOTION_BLUR_NONE",
    "MOTION_BLUR_SNIPER",
    "MOTION_BLUR_LIGHT_SCENE",
    "MOTION_BLUR_SECURITY_CAM",
    "MOTION_BLUR_CUT_SCENE",
    "MOTION_BLUR_INTRO",
    "MOTION_BLUR_INTRO2",
    "MOTION_BLUR_SNIPER_ZOOM",
    "MOTION_BLUR_INTRO3",
    "MOTION_BLUR_INTRO4",
    "",
};

// 0x004986D8
const char* g_apHudFlashItems[] = {
    "HUD_FLASH_NONE",
    (const char*)(-1),
    "HUD_FLASH_WEAPON",
    (const char*)nullptr,
    "HUD_FLASH_CLOCK",
    (const char*)1,
    "HUD_FLASH_SCORE",
    (const char*)2,
    "HUD_FLASH_ARMOUR",
    (const char*)3,
    "HUD_FLASH_HEALTH",
    (const char*)4,
    "HUD_FLASH_WANTED",
    (const char*)5,
    "HUD_FLASH_ZONE_NAME",
    (const char*)6,
    "HUD_FLASH_CAR_NAME",
    (const char*)7,
    "HUD_FLASH_RADAR",
    (const char*)8,
    "HUD_FLASH_BLIP",
    (const char*)9,
    "HUD_FLASH_BREATH",
    (const char*)10,
    "",
    (const char*)11,
};

// 0x00498740
const char* g_apszCarDoors[] = {
    "BONNET",
    "BOOT",
    "FRONT_LEFT_DOOR",
    "FRONT_RIGHT_DOOR",
    "REAR_LEFT_DOOR",
    "REAR_RIGHT_DOOR",
    "EXTRA_DOOR",
    "",
};

// 0x00498760
const char* g_apszRadioStations[] = {
    "RS_CLASSIC_HIP_HOP",
    "RS_COUNTRY",
    "RS_CLASSIC_ROCK",
    "RS_DISCO_FUNK_SOUL",
    "RS_HOUSE_CLASSICS",
    "RS_MODERN_HIP_HOP",
    "RS_MODERN_ROCK",
    "RS_NEW_JACK_SWING",
    "RS_REGGAE",
    "RS_RARE_GROOVE",
    "RS_TALK",
    "RS_OFF",
    "",
};

// 0x00498794
const char* g_apszFacingDirections[] = {
    "FACING_FORWARD",
    "FACING_LEFT",
    "FACING_BACK",
    "FACING_RIGHT",
    "",
};

// 0x004987A8
const char* g_apszReplayCamModes[] = {
    "REPLAYCAMMODE_ASSTORED",
    "REPLAYCAMMODE_TOPDOWN",
    "REPLAYCAMMODE_FIXED",
    "",
};

// 0x004987B8
const char* g_apszCarWheels[] = {
    "FRONT_LEFT_WHEEL",
    "REAR_LEFT_WHEEL",
    "FRONT_RIGHT_WHEEL",
    "REAR_RIGHT_WHEEL",
    "ANY_WHEEL",
    "",
};

// 0x004987D0
const char* g_apszTempActions[] = {
    "TEMPACT_NONE",
    "TEMPACT_WAIT",
    "TEMPACT_EMPTYTOBEREUSED",
    "TEMPACT_REVERSE",
    "TEMPACT_HANDBRAKETURNLEFT",
    "TEMPACT_HANDBRAKETURNRIGHT",
    "TEMPACT_HANDBRAKESTRAIGHT",
    "TEMPACT_TURNLEFT",
    "TEMPACT_TURNRIGHT",
    "TEMPACT_GOFORWARD",
    "TEMPACT_SWERVELEFT",
    "TEMPACT_SWERVERIGHT",
    "TEMPACT_STUCKINTRAFFIC",
    "TEMPACT_REVERSE_LEFT",
    "TEMPACT_REVERSE_RIGHT",
    "TEMPACT_PLANE_FLY_UP",
    "TEMPACT_PLANE_FLY_STRAIGHT",
    "TEMPACT_PLANE_SHARP_LEFT",
    "TEMPACT_PLANE_SHARP_RIGHT",
    "TEMPACT_HEADON_COLLISION",
    "TEMPACT_SWERVELEFT_STOP",
    "TEMPACT_SWERVERIGHT_STOP",
    "TEMPACT_REVERSE_STRAIGHT",
    "TEMPACT_BOOST_USE_STEERING_ANGLE",
    "",
};

// 0x00498834
const char* g_apszMoods[] = {
    "MOOD_AG_UNUSED",
    "MOOD_AR",
    "MOOD_CD_UNUSED",
    "MOOD_CF_UNUSED",
    "MOOD_CG_UNUSED",
    "MOOD_CR",
    "MOOD_PG_UNUSED",
    "MOOD_PR",
    "MOOD_WG_UNUSED",
    "MOOD_WR",
    "MOOD_END_UNUSED",
    "",
};

// 0x00498864
const char* g_apszSetPieces[] = {
    "SETPIECE_NONE",
    "SETPIECE_TWOCOPCARSINALLEY",
    "SETPIECE_CARBLOCKINGPLAYERFROMSIDE",
    "SETPIECE_CARRAMMINGPLAYERFROMSIDE",
    "SETPIECE_CREATECOPPERONFOOT",
    "SETPIECE_CREATETWOCOPPERSONFOOT",
    "SETPIECE_TWOCARSBLOCKINGPLAYERFROMSIDE",
    "SETPIECE_TWOCARSRAMMINGPLAYERFROMSIDE",
    "SETPIECE_TWOFASTCOPCARSINALLEY",
    "",
};

// 0x0049888C
const char* g_apszTimerDirections[] = {
    "TIMER_UP",
    "TIMER_DOWN",
    "",
};

// 0x00498898
const char* g_apBones[] = {
    "BONE_ROOT",
    (const char*)nullptr,
    "BONE_PELVIS",
    (const char*)1,
    "BONE_SPINE",
    (const char*)2,
    "BONE_SPINE1",
    (const char*)3,
    "BONE_NECK",
    (const char*)4,
    "BONE_HEAD",
    (const char*)5,
    "BONE_L_BROW",
    (const char*)6,
    "BONE_R_BROW",
    (const char*)7,
    "BONE_JAW",
    (const char*)8,
    "BONE_R_CLAVICLE",
    (const char*)21,
    "BONE_R_UPPERARM",
    (const char*)22,
    "BONE_R_FOREARM",
    (const char*)23,
    "BONE_R_HAND",
    (const char*)24,
    "BONE_R_FINGERS",
    (const char*)25,
    "BONE_R_FINGER01",
    (const char*)26,
    "BONE_L_CLAVICLE",
    (const char*)31,
    "BONE_L_UPPERARM",
    (const char*)32,
    "BONE_L_FOREARM",
    (const char*)33,
    "BONE_L_HAND",
    (const char*)34,
    "BONE_L_FINGERS",
    (const char*)35,
    "BONE_L_FINGER01",
    (const char*)36,
    "BONE_L_THIGH",
    (const char*)41,
    "BONE_L_CALF",
    (const char*)42,
    "BONE_L_FOOT",
    (const char*)43,
    "BONE_L_TOE",
    (const char*)44,
    "BONE_R_THIGH",
    (const char*)51,
    "BONE_R_CALF",
    (const char*)52,
    "BONE_R_FOOT",
    (const char*)53,
    "BONE_R_TOE",
    (const char*)54,
    "BONE_BELLY",
    (const char*)201,
    "BONE_R_BREAST",
    (const char*)301,
    "BONE_L_BREAST",
    (const char*)302,
    "",
    (const char*)303,
};

// 0x004989A0
const char* g_apszAnnouncementTracks[] = {
    "ANNOUNCEMENT_TRACK_BRIDGE_CLOSED",
    "ANNOUNCEMENT_TRACK_BRIDGE_OPEN",
    "",
};

// 0x004989AC
const char* g_apszPedMoveStates[] = {
    "PEDMOVE_NONE",
    "PEDMOVE_STILL",
    "PEDMOVE_TURN_L",
    "PEDMOVE_TURN_R",
    "PEDMOVE_WALK",
    "PEDMOVE_JOG",
    "PEDMOVE_RUN",
    "PEDMOVE_SPRINT",
    "",
};

// 0x004989D0
const char* g_apStats[] = {
    "PROGRESS_MADE",
    (const char*)nullptr,
    "TOTAL_PROGRESS",
    (const char*)1,
    "LONGEST_BASKETBALL",
    (const char*)2,
    "DIST_FOOT",
    (const char*)3,
    "DIST_CAR",
    (const char*)4,
    "DIST_BIKE",
    (const char*)5,
    "DIST_BOAT",
    (const char*)6,
    "DIST_GOLF_CART",
    (const char*)7,
    "DIST_HELICOPTER",
    (const char*)8,
    "DIST_PLANE",
    (const char*)9,
    "LONGEST_WHEELIE_DIST",
    (const char*)10,
    "LONGEST_STOPPIE_DIST",
    (const char*)11,
    "LONGEST_2WHEEL_DIST",
    (const char*)12,
    "WEAPON_BUDGET",
    (const char*)13,
    "FASHION_BUDGET",
    (const char*)14,
    "PROPERTY_BUDGET",
    (const char*)15,
    "SPRAYING_BUDGET",
    (const char*)16,
    "LONGEST_WHEELIE_TIME",
    (const char*)17,
    "LONGEST_STOPPIE_TIME",
    (const char*)18,
    "LONGEST_2WHEEL_TIME",
    (const char*)19,
    "FOOD_BUDGET",
    (const char*)20,
    "FAT",
    (const char*)21,
    "STAMINA",
    (const char*)22,
    "BODY_MUSCLE",
    (const char*)23,
    "MAX_HEALTH",
    (const char*)24,
    "SEX_APPEAL",
    (const char*)25,
    "DIST_SWIMMING",
    (const char*)26,
    "DIST_CYCLE",
    (const char*)27,
    "DIST_TREADMILL",
    (const char*)28,
    "DIST_EXERCISE_BIKE",
    (const char*)29,
    "TATTOO_BUDGET",
    (const char*)30,
    "HAIRDRESSING_BUDGET",
    (const char*)31,
    "NOT_USED_FLOAT_6",
    (const char*)32,
    "NOT_USED_FLOAT_1",
    (const char*)33,
    "NOT_USED_FLOAT_2",
    (const char*)34,
    "MONEY_SPENT_GAMBLING",
    (const char*)35,
    "MONEY_MADE_PIMPING",
    (const char*)36,
    "MONEY_WON_GAMBLING",
    (const char*)37,
    "BIGGEST_GAMBLING_WIN",
    (const char*)38,
    "BIGGEST_GAMBLING_LOSS",
    (const char*)39,
    "LARGEST_BURGLARY_SWAG",
    (const char*)40,
    "MONEY_MADE_BURGLARY",
    (const char*)41,
    "NOT_USED_FLOAT_3",
    (const char*)42,
    "NOT_USED_FLOAT_4",
    (const char*)43,
    "LONGEST_TREADMILL_TIME",
    (const char*)44,
    "LONGEST_EXERCISE_BIKE_TIME",
    (const char*)45,
    "HEAVIEST_WEIGHT_BENCH_PRESS",
    (const char*)46,
    "HEAVIEST_WEIGHT_DUMBELLS",
    (const char*)47,
    "BEST_TIME_HOTRING",
    (const char*)48,
    "BEST_TIME_BMX",
    (const char*)49,
    "NOT_USED_INT_11",
    (const char*)50,
    "LONGEST_CHASE_TIME",
    (const char*)51,
    "LAST_CHASE_TIME",
    (const char*)52,
    "WAGE_BILL",
    (const char*)53,
    "STRIP_CLUB_BUDGET",
    (const char*)54,
    "CAR_MOD_BUDGET",
    (const char*)55,
    "TIME_SPENT_SHOPPING",
    (const char*)56,
    "NOT_USED_INT_1",
    (const char*)57,
    "NOT_USED_INT_3",
    (const char*)58,
    "NOT_USED_INT_4",
    (const char*)59,
    "NOT_USED_INT_19",
    (const char*)60,
    "NOT_USED_INT_5",
    (const char*)61,
    "TOTAL_SHOPPING_BUDGET",
    (const char*)62,
    "TIME_SPENT_UNDERWATER",
    (const char*)63,
    "RESPECT_TOTAL",
    (const char*)64,
    "RESPECT_GIRLFRIEND",
    (const char*)65,
    "RESPECT_CLOTHES",
    (const char*)66,
    "RESPECT_FITNESS",
    (const char*)67,
    "RESPECT",
    (const char*)68,
    "WEAPONTYPE_PISTOL_SKILL",
    (const char*)69,
    "WEAPONTYPE_PISTOL_SILENCED_SKILL",
    (const char*)70,
    "WEAPONTYPE_DESERT_EAGLE_SKILL",
    (const char*)71,
    "WEAPONTYPE_SHOTGUN_SKILL",
    (const char*)72,
    "WEAPONTYPE_SAWNOFF_SHOTGUN_SKILL",
    (const char*)73,
    "WEAPONTYPE_SPAS12_SHOTGUN_SKILL",
    (const char*)74,
    "WEAPONTYPE_MICRO_UZI_SKILL",
    (const char*)75,
    "WEAPONTYPE_MP5_SKILL",
    (const char*)76,
    "WEAPONTYPE_AK47_SKILL",
    (const char*)77,
    "WEAPONTYPE_M4_SKILL",
    (const char*)78,
    "WEAPONTYPE_SNIPERRIFLE_SKILL",
    (const char*)79,
    "SEX_APPEAL_CLOTHES",
    (const char*)80,
    "GAMBLING",
    (const char*)81,
    "PEOPLE_KILLED_BY_OTHERS",
    (const char*)120,
    "PEOPLE_KILLED_BY_PLAYER",
    (const char*)121,
    "CARS_DESTROYED",
    (const char*)122,
    "BOATS_DESTROYED",
    (const char*)123,
    "HELICOPTERS_DESTROYED",
    (const char*)124,
    "PROPERTY_DESTROYED",
    (const char*)125,
    "ROUNDS_FIRED",
    (const char*)126,
    "EXPLOSIVES_USED",
    (const char*)127,
    "BULLETS_HIT",
    (const char*)128,
    "TYRES_POPPED",
    (const char*)129,
    "HEADS_POPPED",
    (const char*)130,
    "WANTED_STARS_ATTAINED",
    (const char*)131,
    "WANTED_STARS_EVADED",
    (const char*)132,
    "TIMES_ARRESTED",
    (const char*)133,
    "DAYS_PASSED",
    (const char*)134,
    "TIMES_DIED",
    (const char*)135,
    "TIMES_SAVED",
    (const char*)136,
    "TIMES_CHEATED",
    (const char*)137,
    "SPRAYINGS",
    (const char*)138,
    "MAX_JUMP_DISTANCE",
    (const char*)139,
    "MAX_JUMP_HEIGHT",
    (const char*)140,
    "MAX_JUMP_FLIPS",
    (const char*)141,
    "MAX_JUMP_SPINS",
    (const char*)142,
    "BEST_STUNT",
    (const char*)143,
    "UNIQUE_JUMPS_FOUND",
    (const char*)144,
    "UNIQUE_JUMPS_DONE",
    (const char*)145,
    "MISSIONS_ATTEMPTED",
    (const char*)146,
    "MISSIONS_PASSED",
    (const char*)147,
    "TOTAL_MISSIONS",
    (const char*)148,
    "TAXI_MONEY_MADE",
    (const char*)149,
    "PASSENGERS_DELIVERED_IN_TAXI",
    (const char*)150,
    "LIVES_SAVED",
    (const char*)151,
    "CRIMINALS_CAUGHT",
    (const char*)152,
    "FIRES_EXTINGUISHED",
    (const char*)153,
    "PIZZAS_DELIVERED",
    (const char*)154,
    "ASSASSINATIONS",
    (const char*)155,
    "LATEST_DANCE_SCORE",
    (const char*)156,
    "VIGILANTE_LEVEL",
    (const char*)157,
    "AMBULANCE_LEVEL",
    (const char*)158,
    "FIREFIGHTER_LEVEL",
    (const char*)159,
    "DRIVING_SKILL",
    (const char*)160,
    "TRUCK_MISSIONS_PASSED",
    (const char*)161,
    "TRUCK_MONEY_MADE",
    (const char*)162,
    "RECRUITED_GANG_MEMBERS_KILLED",
    (const char*)163,
    "ARMOUR",
    (const char*)164,
    "ENERGY",
    (const char*)165,
    "PHOTOS_TAKEN",
    (const char*)166,
    "KILL_FRENZIES_ATTEMPTED",
    (const char*)167,
    "KILL_FRENZIES_PASSED",
    (const char*)168,
    "FLIGHT_TIME",
    (const char*)169,
    "TIMES_DROWNED",
    (const char*)170,
    "NUM_GIRLS_PIMPED",
    (const char*)171,
    "BEST_POSITION_HOTRING",
    (const char*)172,
    "FLIGHT_TIME_JETPACK",
    (const char*)173,
    "SHOOTING_RANGE_SCORE",
    (const char*)174,
    "VALET_CARS_PARKED",
    (const char*)175,
    "KILLS_SINCE_LAST_CHECKPOINT",
    (const char*)176,
    "TOTAL_LEGITIMATE_KILLS",
    (const char*)177,
    "BLOODRING_KILLS",
    (const char*)178,
    "BLOODRING_TIME",
    (const char*)179,
    "NO_MORE_HURRICANES",
    (const char*)180,
    "CITIES_PASSED",
    (const char*)181,
    "POLICE_BRIBES",
    (const char*)182,
    "CARS_STOLEN",
    (const char*)183,
    "CURRENT_GIRLFRIENDS",
    (const char*)184,
    "BAD_DATES",
    (const char*)185,
    "GIRLS_DATED",
    (const char*)186,
    "TIMES_SCORED_WITH_GIRL",
    (const char*)187,
    "DATES",
    (const char*)188,
    "GIRLS_DUMPED",
    (const char*)189,
    "NOT_USED_INT_9",
    (const char*)190,
    "HOUSES_BURGLED",
    (const char*)191,
    "SAFES_CRACKED",
    (const char*)192,
    "NOT_USED_INT_2",
    (const char*)193,
    "STOLEN_ITEMS_SOLD",
    (const char*)194,
    "EIGHT_BALLS_IN_POOL",
    (const char*)195,
    "WINS_IN_POOL",
    (const char*)196,
    "LOSSES_IN_POOL",
    (const char*)197,
    "VISITS_TO_GYM",
    (const char*)198,
    "NOT_USED_INT_6",
    (const char*)199,
    "MEALS_EATEN",
    (const char*)200,
    "QUARRY_MONEY_MADE",
    (const char*)201,
    "QUARRY_COMPLETION_TIME",
    (const char*)202,
    "NOT_USED_INT_7",
    (const char*)203,
    "NOT_USED_INT_8",
    (const char*)204,
    "HIGHEST_PED_KILLS_ON_SPREE",
    (const char*)205,
    "HIGHEST_POLICE_KILLS_ON_SPREE",
    (const char*)206,
    "HIGHEST_CIV_VEHICLES_DESTROYED_ON_SPREE",
    (const char*)207,
    "HIGHEST_COP_VEHICLES_DESTROYED_ON_SPREE",
    (const char*)208,
    "HIGHEST_TANKS_DESTROYED_ON_SPREE",
    (const char*)209,
    "PIMPING_LEVEL",
    (const char*)210,
    "KICKSTART_BEST_SCORE",
    (const char*)211,
    "BEST_LAP_TIME_HOTRING",
    (const char*)212,
    "VEHICLES_EXPORTED",
    (const char*)213,
    "VEHICLES_IMPORTED",
    (const char*)214,
    "HIGHEST_SCORE_IN_BASKETBALL_CHALLENGE",
    (const char*)215,
    "FIRES_STARTED",
    (const char*)216,
    "DRUGS_SOLD",
    (const char*)217,
    "DRUGS_BOUGHT",
    (const char*)218,
    "BEST_LAP_TIME_DIRTTRACK",
    (const char*)219,
    "BEST_TIME_DIRTTRACK",
    (const char*)220,
    "BEST_POSITION_DIRTTRACK",
    (const char*)221,
    "BEST_TIME_NRG",
    (const char*)222,
    "FLYING_SKILL",
    (const char*)223,
    "RESPECT_MISSION",
    (const char*)224,
    "UNDERWATER_BREATH_STAMINA",
    (const char*)225,
    "GORE_RATING_LEVEL",
    (const char*)226,
    "SEX_RATING_LEVEL",
    (const char*)227,
    "RESPECT_MISSION_TOTAL",
    (const char*)228,
    "BIKE_SKILL",
    (const char*)229,
    "CYCLE_SKILL",
    (const char*)230,
    "SNAPSHOTS_TAKEN",
    (const char*)231,
    "TOTAL_SNAPSHOTS",
    (const char*)232,
    "LUCK",
    (const char*)233,
    "TERRITORIES_TAKEN_OVER",
    (const char*)234,
    "TERRITORIES_LOST",
    (const char*)235,
    "NUMBER_TERRITORIES_HELD",
    (const char*)236,
    "HIGHEST_NUMBER_TERRITORIES_HELD",
    (const char*)237,
    "NUMBER_GANG_MEMBERS_RECRUITED",
    (const char*)238,
    "NUMBER_ENEMY_GANG_MEMBERS_KILLED",
    (const char*)239,
    "NUMBER_FRIENDLY_GANG_MEMBERS_KILLED",
    (const char*)240,
    "HORSESHOES_COLLECTED",
    (const char*)241,
    "TOTAL_HORSESHOES",
    (const char*)242,
    "OYSTERS_COLLECTED",
    (const char*)243,
    "TOTAL_OYSTERS",
    (const char*)244,
    "CALORIES",
    (const char*)245,
    "P2_RAMPAGE_BEST_TIME",
    (const char*)246,
    "P2_CARS_RAMPAGE_BEST_TIME",
    (const char*)247,
    "P2_PEDS_RAMPAGE_BEST_TIME",
    (const char*)248,
    "P2_HELI_RAMPAGE_BEST_TIME",
    (const char*)249,
    "P2_BIKE_RAMPAGE_BEST_TIME",
    (const char*)250,
    "KEEPIE_UPPY_BEST_TIME",
    (const char*)251,
    "GIRLFRIEND_DENISE",
    (const char*)252,
    "GIRLFRIEND_MICHELLE",
    (const char*)253,
    "GIRLFRIEND_HELENA",
    (const char*)254,
    "GIRLFRIEND_BARBARA",
    (const char*)255,
    "GIRLFRIEND_KATIE",
    (const char*)256,
    "GIRLFRIEND_MILLIE",
    (const char*)257,
    "RACETOUR_0_BEST_POSITION",
    (const char*)258,
    "RACETOUR_0_BEST_TIME",
    (const char*)259,
    "RACETOUR_1_BEST_POSITION",
    (const char*)260,
    "RACETOUR_1_BEST_TIME",
    (const char*)261,
    "RACETOUR_2_BEST_POSITION",
    (const char*)262,
    "RACETOUR_2_BEST_TIME",
    (const char*)263,
    "RACETOUR_3_BEST_POSITION",
    (const char*)264,
    "RACETOUR_3_BEST_TIME",
    (const char*)265,
    "RACETOUR_4_BEST_POSITION",
    (const char*)266,
    "RACETOUR_4_BEST_TIME",
    (const char*)267,
    "RACETOUR_5_BEST_POSITION",
    (const char*)268,
    "RACETOUR_5_BEST_TIME",
    (const char*)269,
    "RACETOUR_6_BEST_POSITION",
    (const char*)270,
    "RACETOUR_6_BEST_TIME",
    (const char*)271,
    "RACETOUR_7_BEST_POSITION",
    (const char*)272,
    "RACETOUR_7_BEST_TIME",
    (const char*)273,
    "RACETOUR_8_BEST_POSITION",
    (const char*)274,
    "RACETOUR_8_BEST_TIME",
    (const char*)275,
    "RACETOUR_9_BEST_POSITION",
    (const char*)276,
    "RACETOUR_9_BEST_TIME",
    (const char*)277,
    "RACETOUR_10_BEST_POSITION",
    (const char*)278,
    "RACETOUR_10_BEST_TIME",
    (const char*)279,
    "RACETOUR_11_BEST_POSITION",
    (const char*)280,
    "RACETOUR_11_BEST_TIME",
    (const char*)281,
    "RACETOUR_12_BEST_POSITION",
    (const char*)282,
    "RACETOUR_12_BEST_TIME",
    (const char*)283,
    "RACETOUR_13_BEST_POSITION",
    (const char*)284,
    "RACETOUR_13_BEST_TIME",
    (const char*)285,
    "RACETOUR_14_BEST_POSITION",
    (const char*)286,
    "RACETOUR_14_BEST_TIME",
    (const char*)287,
    "RACETOUR_15_BEST_POSITION",
    (const char*)288,
    "RACETOUR_15_BEST_TIME",
    (const char*)289,
    "RACETOUR_16_BEST_POSITION",
    (const char*)290,
    "RACETOUR_16_BEST_TIME",
    (const char*)291,
    "RACETOUR_17_BEST_POSITION",
    (const char*)292,
    "RACETOUR_17_BEST_TIME",
    (const char*)293,
    "RACETOUR_18_BEST_POSITION",
    (const char*)294,
    "RACETOUR_18_BEST_TIME",
    (const char*)295,
    "RACETOUR_19_BEST_TIME",
    (const char*)296,
    "RACETOUR_20_BEST_TIME",
    (const char*)297,
    "RACETOUR_21_BEST_TIME",
    (const char*)298,
    "RACETOUR_22_BEST_TIME",
    (const char*)299,
    "RACETOUR_23_BEST_TIME",
    (const char*)300,
    "RACETOUR_24_BEST_TIME",
    (const char*)301,
    "PASSED_SWEET2",
    (const char*)302,
    "PASSED_STRAP3",
    (const char*)303,
    "PASSED_ZERO1",
    (const char*)304,
    "PASSED_SYNDICATE_AND_WOOZIE",
    (const char*)305,
    "PASSED_CASINO7",
    (const char*)306,
    "PASSED_HEIST8",
    (const char*)307,
    "PASSED_MANSION2",
    (const char*)308,
    "STARTED_RIOT1",
    (const char*)309,
    "PASSED_RYDER2",
    (const char*)310,
    "PASSED_FARLIE3",
    (const char*)311,
    "PASSED_HEIST1",
    (const char*)312,
    "PASSED_SYNDICATE2",
    (const char*)313,
    "PASSED_LAFIN1",
    (const char*)314,
    "PASSED_CAT2",
    (const char*)315,
    "PASSED_SYNDICATE1",
    (const char*)316,
    "PASSED_CASINO4",
    (const char*)317,
    "PASSED_CAT1",
    (const char*)318,
    "PASSED_BCRASH1",
    (const char*)319,
    "PASSED_DESERT1",
    (const char*)320,
    "PASSED_GARAGE1",
    (const char*)321,
    "PASSED_BCESAR4",
    (const char*)322,
    "PASSED_LAFIN2",
    (const char*)323,
    "PASSED_GARAGE2",
    (const char*)324,
    "PASSED_DESERT4",
    (const char*)325,
    "PASSED_DESERT12",
    (const char*)326,
    "PASSED_CASINO3",
    (const char*)327,
    "PASSED_CASINO6",
    (const char*)328,
    "PASSED_CASINO10",
    (const char*)329,
    "PASSED_DESERT3",
    (const char*)330,
    "PASSED_DESERT5",
    (const char*)331,
    "PASSED_DESERT8",
    (const char*)332,
    "PASSED_DESERT10",
    (const char*)333,
    "PASSED_MANSION3",
    (const char*)334,
    "PASSED_RIOT1",
    (const char*)335,
    "PASSED_SCRASH1",
    (const char*)336,
    "PASSED_STRAP4",
    (const char*)337,
    "PASSED_TRUTH2",
    (const char*)338,
    "PASSED_VCRASH2",
    (const char*)339,
    "STARTED_BADLANDS",
    (const char*)340,
    "STARTED_CAT2",
    (const char*)341,
    "STARTED_CRASH1",
    (const char*)342,
    "",
    (const char*)343,
};

// 0x00499360
const char* g_apszTaskStatus[] = {
    "WAITING_TO_START_TASK",
    "PERFORMING_TASK",
    "DORMANT_TASK",
    "VACANT_STAGE",
    "GROUP_TASK_STAGE",
    "ATTRACTOR_SCRIPT_TASK_STAGE",
    "SECONDARY_TASK_STAGE",
    "FINISHED_TASK",
    "",
};

// 0x00499384
const char* g_apDuckingTypes[] = {
    "NO_DUCKING",
    (const char*)nullptr,
    "DUCK_WHEN_PLAYER_SHOOTING",
    (const char*)1,
    "DUCK_WHEN_I_SHOOT",
    (const char*)2,
    "DUCK_RANDOMLY",
    (const char*)4,
    "",
    (const char*)5,
};

// 0x004993AC
const char* g_apszFollowRouteModes[] = {
    "FOLLOW_ROUTE_ONCE",
    "FOLLOW_ROUTE_RETURN",
    "FOLLOW_ROUTE_BACKFORWARD",
    "FOLLOW_ROUTE_LOOP",
    "",
};

// 0x004993C0
const char* g_apszSplineModes[] = {
    "MODE_NORMAL",
    "MODE_ACCURATE",
    "MODE_STRAIGHTLINE",
    "MODE_RACING",
    "",
};

// 0x004993D4
const char* g_apszCarLightOverrides[] = {
    "NO_CAR_LIGHT_OVERRIDE",
    "FORCE_CAR_LIGHTS_OFF",
    "FORCE_CAR_LIGHTS_ON",
    "",
};

// 0x004993E4
const char* g_apszCarPanels[] = {
    "FRONT_LEFT_PANEL",
    "FRONT_RIGHT_PANEL",
    "REAR_LEFT_PANEL",
    "REAR_RIGHT_PANEL",
    "WINDSCREEN_PANEL",
    "FRONT_BUMPER",
    "REAR_BUMPER",
    "MAX_PANELS",
    "",
};

// 0x00499408
const char* g_apszDefaultTaskAllocators[] = {
    "DEFAULT_TASK_ALLOCATOR_FOLLOW_ANY_MEANS",
    "DEFAULT_TASK_ALLOCATOR_FOLLOW_LIMITED",
    "DEFAULT_TASK_ALLOCATOR_STAND_STILL",
    "DEFAULT_TASK_ALLOCATOR_CHAT",
    "DEFAULT_TASK_ALLOCATOR_SIT_IN_LEADER_CAR",
    "DEFAULT_TASK_ALLOCATOR_RANDOM",
    "",
};

// 0x00499424
const char* g_apszCheckpointTypes[] = {
    "CHECKPOINT_TUBE",
    "CHECKPOINT_ENDTUBE",
    "CHECKPOINT_EMPTYTUBE",
    "CHECKPOINT_TORUS",
    "CHECKPOINT_TORUS_NOFADE",
    "CHECKPOINT_TORUSROT",
    "CHECKPOINT_TORUSTHROUGH",
    "CHECKPOINT_TORUS_UPDOWN",
    "CHECKPOINT_TORUS_DOWN",
    "CHECKPOINT_NUM",
    "",
};

// 0x00499450
const char* g_apszVehicleUpgrades[] = {
    "VEHICLE_UPGRADE_BONNET",
    "VEHICLE_UPGRADE_BONNET_LEFT_RIGHT",
    "VEHICLE_UPGRADE_SPOILER",
    "VEHICLE_UPGRADE_WING",
    "VEHICLE_UPGRADE_FRONT_BULLBAR",
    "VEHICLE_UPGRADE_REAR_BULLBAR",
    "VEHICLE_UPGRADE_FRONT_LIGHTS",
    "VEHICLE_UPGRADE_ROOF",
    "VEHICLE_UPGRADE_NITRO",
    "VEHICLE_UPGRADE_HYDRAULICS",
    "VEHICLE_UPGRADE_STEREO",
    "VEHICLE_REPLACEMENT_CHASSIS",
    "VEHICLE_REPLACEMENT_WHEEL",
    "VEHICLE_REPLACEMENT_EXHAUST",
    "VEHICLE_REPLACEMENT_FRONT_BUMPER",
    "VEHICLE_REPLACEMENT_REAR_BUMPER",
    "VEHICLE_REPLACEMENT_MISC",
    "",
};

// 0x00499498
const char* g_apszDriveByTypes[] = {
    "DRIVEBY_FIXED_LHS",
    "DRIVEBY_FIXED_RHS",
    "DRIVEBY_START_FROM_LHS",
    "DRIVEBY_START_FROM_RHS",
    "DRIVEBY_AI_SIDE",
    "DRIVEBY_FIXED_FWD",
    "DRIVEBY_FIXED_BAK",
    "DRIVEBY_AI_FWD_BAK",
    "DRIVEBY_AI_ALL_DIRN",
    "",
};

// 0x004994C0
const char* g_apHoldOrientateTypes[] = {
    "HOLD_ORIENTATE_PEDHEADING",
    (const char*)1,
    "HOLD_ORIENTATE_BONE_FULL",
    (const char*)16,
    "",
    (const char*)255,
};

// 0x004994D8
const char* g_apszPedBodyParts[] = {
    "PED_NULL",
    "PED_TORSO",
    "PED_HEAD",
    "PED_UPPERARML",
    "PED_UPPERARMR",
    "PED_HANDL",
    "PED_HANDR",
    "PED_UPPERLEGL",
    "PED_UPPERLEGR",
    "PED_FOOTL",
    "PED_FOOTR",
    "PED_KNEER",
    "PED_KNEEL",
    "PED_LOWERARML",
    "PED_LOWERARMR",
    "PED_UPPERCLAVICLEL",
    "PED_UPPERCLAVICLER",
    "PED_NECK",
    "PED_JAW",
    "",
};

// 0x00499528
const char* g_apszKeyboardKeysPS2[] = {
    "PS2_KEY_UNKNOWN_0",
    "PS2_KEY_UNKNOWN_1",
    "PS2_KEY_UNKNOWN_2",
    "PS2_KEY_UNKNOWN_3",
    "PS2_KEY_A",
    "PS2_KEY_B",
    "PS2_KEY_C",
    "PS2_KEY_D",
    "PS2_KEY_E",
    "PS2_KEY_F",
    "PS2_KEY_G",
    "PS2_KEY_H",
    "PS2_KEY_I",
    "PS2_KEY_J",
    "PS2_KEY_K",
    "PS2_KEY_L",
    "PS2_KEY_M",
    "PS2_KEY_N",
    "PS2_KEY_O",
    "PS2_KEY_P",
    "PS2_KEY_Q",
    "PS2_KEY_R",
    "PS2_KEY_S",
    "PS2_KEY_T",
    "PS2_KEY_U",
    "PS2_KEY_V",
    "PS2_KEY_W",
    "PS2_KEY_X",
    "PS2_KEY_Y",
    "PS2_KEY_Z",
    "PS2_KEY_1",
    "PS2_KEY_2",
    "PS2_KEY_3",
    "PS2_KEY_4",
    "PS2_KEY_5",
    "PS2_KEY_6",
    "PS2_KEY_7",
    "PS2_KEY_8",
    "PS2_KEY_9",
    "PS2_KEY_0",
    "PS2_KEY_RETURN",
    "PS2_KEY_ESC",
    "PS2_KEY_BSPACE",
    "PS2_KEY_TAB",
    "PS2_KEY_SPACE",
    "PS2_KEY_MINUS",
    "PS2_KEY_EQUALS",
    "PS2_KEY_OPEN_SQUARE_BRACKET",
    "PS2_KEY_CLOSE_SQUARE_BRACKET",
    "PS2_KEY_UNKNOWN_49",
    "PS2_KEY_HASH",
    "PS2_KEY_SEMI_COLON",
    "PS2_KEY_AT",
    "PS2_KEY_BACK_QUOTE",
    "PS2_KEY_COMMA",
    "PS2_KEY_PERIOD",
    "PS2_KEY_SLASH",
    "PS2_KEY_CAPSLOCK",
    "PS2_KEY_F1",
    "PS2_KEY_F2",
    "PS2_KEY_F3",
    "PS2_KEY_F4",
    "PS2_KEY_F5",
    "PS2_KEY_F6",
    "PS2_KEY_F7",
    "PS2_KEY_F8",
    "PS2_KEY_F9",
    "PS2_KEY_F10",
    "PS2_KEY_F11",
    "PS2_KEY_F12",
    "PS2_KEY_PRINT_SCREEN",
    "PS2_KEY_SCROLL_LOCK",
    "PS2_KEY_PAUSE",
    "PS2_KEY_INSERT",
    "PS2_KEY_HOME",
    "PS2_KEY_PGUP",
    "PS2_KEY_DEL",
    "PS2_KEY_END",
    "PS2_KEY_PGDN",
    "PS2_KEY_RIGHT",
    "PS2_KEY_LEFT",
    "PS2_KEY_DOWN",
    "PS2_KEY_UP",
    "PS2_KEY_NUMLOCK",
    "PS2_KEY_NUMPAD_SLASH",
    "PS2_KEY_NUMPAD_ASTERISK",
    "PS2_KEY_NUMPAD_MINUS",
    "PS2_KEY_NUMPAD_PLUS",
    "PS2_KEY_ENTER",
    "PS2_KEY_NUMPAD_1",
    "PS2_KEY_NUMPAD_2",
    "PS2_KEY_NUMPAD_3",
    "PS2_KEY_NUMPAD_4",
    "PS2_KEY_NUMPAD_5",
    "PS2_KEY_NUMPAD_6",
    "PS2_KEY_NUMPAD_7",
    "PS2_KEY_NUMPAD_8",
    "PS2_KEY_NUMPAD_9",
    "PS2_KEY_NUMPAD_0",
    "PS2_KEY_NUMPAD_PERIOD",
    "PS2_KEY_BACKSLASH",
    "PS2_KEY_MENU",
    "PS2_KEY_SHIFT",
    "PS2_KEY_ALT",
    "PS2_KEY_CTRL",
    "PS2_KEY_START",
    "PS2_KEY_MAX_CODES",
    "",
};

// 0x004996D8
const char* g_apszKeyboardKeysPC[] = {
    "PS2_KEY_UNKNOWN_0",
    "PS2_KEY_UNKNOWN_1",
    "PS2_KEY_UNKNOWN_2",
    "PS2_KEY_UNKNOWN_3",
    "PS2_KEY_UNKNOWN_4",
    "PS2_KEY_UNKNOWN_5",
    "PS2_KEY_UNKNOWN_6",
    "PS2_KEY_UNKNOWN_7",
    "PS2_KEY_UNKNOWN_8",
    "PS2_KEY_UNKNOWN_9",
    "PS2_KEY_UNKNOWN_10",
    "PS2_KEY_UNKNOWN_11",
    "PS2_KEY_UNKNOWN_12",
    "PS2_KEY_UNKNOWN_13",
    "PS2_KEY_UNKNOWN_14",
    "PS2_KEY_UNKNOWN_15",
    "PS2_KEY_UNKNOWN_16",
    "PS2_KEY_UNKNOWN_17",
    "PS2_KEY_UNKNOWN_18",
    "PS2_KEY_UNKNOWN_19",
    "PS2_KEY_UNKNOWN_20",
    "PS2_KEY_UNKNOWN_21",
    "PS2_KEY_UNKNOWN_22",
    "PS2_KEY_UNKNOWN_23",
    "PS2_KEY_UNKNOWN_24",
    "PS2_KEY_UNKNOWN_25",
    "PS2_KEY_UNKNOWN_26",
    "PS2_KEY_UNKNOWN_27",
    "PS2_KEY_UNKNOWN_28",
    "PS2_KEY_UNKNOWN_29",
    "PS2_KEY_UNKNOWN_30",
    "PS2_KEY_UNKNOWN_31",
    "PS2_KEY_SPACE",
    "PS2_KEY_UNKNOWN_33",
    "PS2_KEY_UNKNOWN_34",
    "PS2_KEY_HASH",
    "PS2_KEY_UNKNOWN_36",
    "PS2_KEY_UNKNOWN_37",
    "PS2_KEY_UNKNOWN_38",
    "PS2_KEY_UNKNOWN_39",
    "PS2_KEY_UNKNOWN_40",
    "PS2_KEY_UNKNOWN_41",
    "PS2_KEY_UNKNOWN_42",
    "PS2_KEY_UNKNOWN_43",
    "PS2_KEY_UNKNOWN_44",
    "PS2_KEY_MINUS",
    "PS2_KEY_UNKNOWN_46",
    "PS2_KEY_UNKNOWN_47",
    "PS2_KEY_0",
    "PS2_KEY_1",
    "PS2_KEY_2",
    "PS2_KEY_3",
    "PS2_KEY_4",
    "PS2_KEY_5",
    "PS2_KEY_6",
    "PS2_KEY_7",
    "PS2_KEY_8",
    "PS2_KEY_9",
    "PS2_KEY_UNKNOWN_58",
    "PS2_KEY_SEMI_COLON",
    "PS2_KEY_UNKNOWN_60",
    "PS2_KEY_EQUALS",
    "PS2_KEY_UNKNOWN_62",
    "PS2_KEY_UNKNOWN_63",
    "PS2_KEY_UNKNOWN_64",
    "PS2_KEY_A",
    "PS2_KEY_B",
    "PS2_KEY_C",
    "PS2_KEY_D",
    "PS2_KEY_E",
    "PS2_KEY_F",
    "PS2_KEY_G",
    "PS2_KEY_H",
    "PS2_KEY_I",
    "PS2_KEY_J",
    "PS2_KEY_K",
    "PS2_KEY_L",
    "PS2_KEY_M",
    "PS2_KEY_N",
    "PS2_KEY_O",
    "PS2_KEY_P",
    "PS2_KEY_Q",
    "PS2_KEY_R",
    "PS2_KEY_S",
    "PS2_KEY_T",
    "PS2_KEY_U",
    "PS2_KEY_V",
    "PS2_KEY_W",
    "PS2_KEY_X",
    "PS2_KEY_Y",
    "PS2_KEY_Z",
    "PS2_KEY_OPEN_SQUARE_BRACKET",
    "PS2_KEY_UNKNOWN_92",
    "PS2_KEY_CLOSE_SQUARE_BRACKET",
    "PS2_KEY_UNKNOWN_94",
    "PS2_KEY_UNKNOWN_95",
    "PS2_KEY_BACK_QUOTE",
    "PS2_KEY_RETURN",
    "PS2_KEY_ESC",
    "PS2_KEY_BSPACE",
    "PS2_KEY_TAB",
    "PS2_KEY_UNKNOWN_101",
    "PS2_KEY_AT",
    "PS2_KEY_COMMA",
    "PS2_KEY_PERIOD",
    "PS2_KEY_SLASH",
    "PS2_KEY_CAPSLOCK",
    "PS2_KEY_F1",
    "PS2_KEY_F2",
    "PS2_KEY_F3",
    "PS2_KEY_F4",
    "PS2_KEY_F5",
    "PS2_KEY_F6",
    "PS2_KEY_F7",
    "PS2_KEY_F8",
    "PS2_KEY_F9",
    "PS2_KEY_F10",
    "PS2_KEY_F11",
    "PS2_KEY_F12",
    "PS2_KEY_PRINT_SCREEN",
    "PS2_KEY_SCROLL_LOCK",
    "PS2_KEY_PAUSE",
    "PS2_KEY_INSERT",
    "PS2_KEY_HOME",
    "PS2_KEY_PGUP",
    "PS2_KEY_DEL",
    "PS2_KEY_END",
    "PS2_KEY_PGDN",
    "PS2_KEY_RIGHT",
    "PS2_KEY_LEFT",
    "PS2_KEY_DOWN",
    "PS2_KEY_UP",
    "PS2_KEY_NUMLOCK",
    "PS2_KEY_NUMPAD_SLASH",
    "PS2_KEY_NUMPAD_ASTERISK",
    "PS2_KEY_NUMPAD_MINUS",
    "PS2_KEY_NUMPAD_PLUS",
    "PS2_KEY_ENTER",
    "PS2_KEY_NUMPAD_1",
    "PS2_KEY_NUMPAD_2",
    "PS2_KEY_NUMPAD_3",
    "PS2_KEY_NUMPAD_4",
    "PS2_KEY_NUMPAD_5",
    "PS2_KEY_NUMPAD_6",
    "PS2_KEY_NUMPAD_7",
    "PS2_KEY_NUMPAD_8",
    "PS2_KEY_NUMPAD_9",
    "PS2_KEY_NUMPAD_0",
    "PS2_KEY_NUMPAD_PERIOD",
    "PS2_KEY_BACKSLASH",
    "PS2_KEY_MENU",
    "PS2_KEY_SHIFT",
    "PS2_KEY_ALT",
    "PS2_KEY_CTRL",
    "PS2_KEY_START",
    "PS2_KEY_MAX_CODES",
    "",
};

// 0x0049994C
const char* g_apszRadarModes[] = {
    "RADARMODE_OFF",
    "RADARMODE_ON",
    "RADARMODE_FLASHING",
    "",
};

// 0x0049995C
const char* g_apszAcquaintanceTypes[] = {
    "ACQUAINTANCE_TYPE_PED_RESPECT",
    "ACQUAINTANCE_TYPE_PED_LIKE",
    "ACQUAINTANCE_TYPE_PED_IGNORE",
    "ACQUAINTANCE_TYPE_PED_DISLIKE",
    "ACQUAINTANCE_TYPE_PED_HATE",
    "",
};

// 0x00499974
const char* g_apszCarPlateDesigns[] = {
    "CARPLATE_DESIGN_SF",
    "CARPLATE_DESIGN_VEGAS",
    "CARPLATE_DESIGN_LA",
    "CARPLATEMGR_PLATEDESIGN_NUM",
    "",
};

// 0x00499988
const char* g_apszStatIncrements[] = {
    "STAT_INC_CYCLE_STAMINA",
    "STAT_INC_SWIM_STAMINA",
    "STAT_INC_SPRINT_STAMINA",
    "STAT_INC_RUNNING",
    "STAT_INC_DRIVING_SKILL",
    "STAT_INC_FLYING_SKILL",
    "STAT_INC_CYCLE_SKILL",
    "STAT_INC_MOTORBIKE_SKILL",
    "STAT_INC_BOAT_SKILL",
    "STAT_INC_BODY_MUSCLE",
    "STAT_INC_BREATH_UNDERWATER",
    "STAT_INC_MAX_HEALTH",
    "STAT_INC_PISTOL_SKILL",
    "STAT_INC_PISTOL_SILENCED_SKILL",
    "STAT_INC_DESERT_EAGLE_SKILL",
    "STAT_INC_SHOTGUN_SKILL",
    "STAT_INC_SAWNOFF_SHOTGUN_SKILL",
    "STAT_INC_SPAS12_SHOTGUN_SKILL",
    "STAT_INC_MICRO_UZI_SKILL",
    "STAT_INC_MP5_SKILL",
    "STAT_INC_AK47_SKILL",
    "STAT_INC_M4_SKILL",
    "STAT_INC_SNIPERRIFLE_SKILL",
    "STAT_DEC_FAT",
    "STAT_DEC_BODY_MUSCLE",
    "STAT_DEC_MAX_HEALTH",
    "STAT_EXERCISE_RATE_CYCLE",
    "STAT_EXERCISE_RATE_CYCLE_SPRINT",
    "STAT_EXERCISE_RATE_SWIM",
    "STAT_EXERCISE_RATE_SWIM_SPRINT",
    "STAT_EXERCISE_RATE_SPRINT",
    "STAT_EXERCISE_RATE_RUN",
    "STAT_EXERCISE_RATE_FIGHT",
    "STAT_TIMELIMIT_CYCLE_STAMINA",
    "STAT_TIMELIMIT_SWIM_STAMINA",
    "STAT_TIMELIMIT_SPRINT_STAMINA",
    "STAT_TIMELIMIT_RUNNING",
    "STAT_TIMELIMIT_DRIVING_SKILL",
    "STAT_TIMELIMIT_FLYING_SKILL",
    "STAT_TIMELIMIT_CYCLE_SKILL",
    "STAT_TIMELIMIT_MOTORBIKE_SKILL",
    "STAT_TIMELIMIT_BOAT_SKILL",
    "STAT_TIMELIMIT_FAT_ADJUST",
    "STAT_TIMELIMIT_FAT_ADJUST_STRENUOUS",
    "STAT_TIMELIMIT_BREATH_UNDERWATER",
    "STAT_TIMELIMIT_MAX_HEALTH",
    "STAT_TIMELIMIT_PISTOL_SKILL",
    "STAT_TIMELIMIT_PISTOL_SILENCED_SKILL",
    "STAT_TIMELIMIT_DESERT_EAGLE_SKILL",
    "STAT_TIMELIMIT_SHOTGUN_SKILL",
    "STAT_TIMELIMIT_SAWNOFF_SHOTGUN_SKILL",
    "STAT_TIMELIMIT_SPAS12_SHOTGUN_SKILL",
    "STAT_TIMELIMIT_MICRO_UZI_SKILL",
    "STAT_TIMELIMIT_MP5_SKILL",
    "STAT_TIMELIMIT_M4_SKILL",
    "STAT_TIMELIMIT_AK47_SKILL",
    "STAT_TIMELIMIT_SNIPERRIFLE_SKILL",
    "STAT_TIMELIMIT_DEATH_HEALTH",
    "STAT_TIMELIMIT_ADD_TO_HEALTH",
    "",
};

// 0x00499A78
const char* g_apszPopcycleZones[] = {
    "POPCYCLE_ZONE_BUSINESS",
    "POPCYCLE_ZONE_DESERT",
    "POPCYCLE_ZONE_ENTERTAINMENT",
    "POPCYCLE_ZONE_COUNTRYSIDE",
    "POPCYCLE_ZONE_RESIDENTIAL_RICH",
    "POPCYCLE_ZONE_RESIDENTIAL_AVERAGE",
    "POPCYCLE_ZONE_RESIDENTIAL_POOR",
    "POPCYCLE_ZONE_GANGLAND",
    "POPCYCLE_ZONE_BEACH",
    "POPCYCLE_ZONE_SHOPPING",
    "POPCYCLE_ZONE_PARK",
    "POPCYCLE_ZONE_INDUSTRY",
    "POPCYCLE_ZONE_ENTERTAINMENT_BUSY",
    "POPCYCLE_ZONE_SHOPPING_BUSY",
    "POPCYCLE_ZONE_SHOPPING_POSH",
    "POPCYCLE_ZONE_RESIDENTIAL_RICH_SECLUDED",
    "POPCYCLE_ZONE_AIRPORT",
    "POPCYCLE_ZONE_GOLF_CLUB",
    "POPCYCLE_ZONE_OUT_OF_TOWN_FACTORY",
    "POPCYCLE_ZONE_AIRPORT_RUNWAY",
    "POPCYCLE_ZONETYPES",
    "",
};

// 0x00499AD0
const char* g_apszWinchTypes[] = {
    "WINCHTYPE_NONE",
    "WINCHTYPE_MAGNET",
    "WINCHTYPE_PEOPLEPICKUP",
    "WINCHTYPE_RCHELI",
    "",
};

// 0x00499AE4
const char* g_apszPlaybackDisplayModes[] = {
    "PLAYBACK_DISPLAY_MODE_NONE",
    "PLAYBACK_DISPLAY_MODE_WHOLELINE",
    "PLAYBACK_DISPLAY_MODE_JUSTINFRONT",
    "PLAYBACK_DISPLAY_MODE_JUSTBEHIND",
    "PLAYBACK_DISPLAY_MODE_AROUNDVEHICLE",
    "",
};

// 0x00499AFC
const char* g_apszConversationStates[] = {
    "CONVERSATION_STAT_INACTIVE",
    "CONVERSATION_STAT_PLAYER_SPEAKING",
    "CONVERSATION_STAT_PED_SPEAKING",
    "CONVERSATION_STAT_WAITINGFORINPUT",
    "",
};

// 0x00499B10
const char* g_apDecisionMakerPedTypes[] = {
    "DM_PED_GANG",
    (const char*)65536,
    "DM_PED_COP",
    (const char*)65537,
    "DM_PED_RANDOM_NORM",
    (const char*)65538,
    "DM_PED_RANDOM_TOUGH",
    (const char*)65539,
    "DM_PED_RANDOM_WEAK",
    (const char*)65540,
    "DM_PED_FIREMAN",
    (const char*)65541,
    "DM_PED_EMPTY",
    (const char*)65542,
    "DM_PED_INDOORS",
    (const char*)65543,
    "DM_GROUP_RANDOM",
    (const char*)65544,
    "DM_GROUP_RANDOM2",
    (const char*)65545,
    "",
    (const char*)65546,
};

// 0x00499B68
const char* g_apszDecisionMakerMissionPedTypes[] = {
    "DM_PED_MISSION_EMPTY",
    "DM_PED_MISSION_NORM",
    "DM_PED_MISSION_TOUGH",
    "DM_PED_MISSION_WEAK",
    "DM_PED_MISSION_STEAL",
    "",
};

// 0x00499B80
const char* g_apszDecisionMakerGroupTypes[] = {
    "DM_GROUP_MISSION_NORM",
    "",
};

// 0x00499B88
const char* g_apMeleeCombos[] = {
    "MCOMBO_UNARMED_1",
    (const char*)4,
    "MCOMBO_UNARMED_2",
    (const char*)5,
    "MCOMBO_UNARMED_3",
    (const char*)6,
    "MCOMBO_UNARMED_4",
    (const char*)7,
    "",
    (const char*)8,
};

// 0x00499BB0
const char* g_apszWeaponSkills[] = {
    "WEAPONSKILL_POOR",
    "WEAPONSKILL_STD",
    "WEAPONSKILL_PRO",
    "",
};

// 0x00499BC0
const char* g_apBuyableTypes[] = {
    "BUYABLE_PROPERTY",
    (const char*)1,
    "BUYABLE_FURNITURE",
    (const char*)4,
    "",
    (const char*)5,
};

// 0x00499BD8
const char* g_apszSelectedWeaponTypes[] = {
    "SELECTEDWEAPON_NONE",
    "SELECTEDWEAPON_GUNS",
    "SELECTEDWEAPON_BOMBS",
    "SELECTEDWEAPON_HSMISSILES",
    "SELECTEDWEAPON_MISSILES",
    "",
};

// 0x00499BF0
const char* g_apszInteriorTypes[] = {
    "INTERIOR_HOUSE",
    "INTERIOR_SHOP",
    "INTERIOR_OFFICE",
    "",
};

// 0x00499C00
const char* g_apszPopRaces[] = {
    "POPRACE_NONE",
    "POPRACE_B",
    "POPRACE_W",
    "POPRACE_WB",
    "POPRACE_O",
    "POPRACE_OB",
    "POPRACE_OW",
    "POPRACE_OWB",
    "POPRACE_H",
    "POPRACE_HB",
    "POPRACE_HW",
    "POPRACE_HWB",
    "POPRACE_HO",
    "POPRACE_HOB",
    "POPRACE_HOW",
    "POPRACE_HOWB",
    "",
};

// 0x00499C44
const char* g_apszBirdTypes[] = {
    "BIRDTYPE_SEAGULL",
    "BIRDTYPE_VULTURE",
    "BIRDTYPE_PIGEON",
    "",
};

// 0x00499C54
const char* g_apszFontOrientations[] = {
    "FO_CENTRE",
    "FO_LEFT",
    "FO_RIGHT",
    "",
};

// 0x00499C64
const char* g_apszCarClasses[] = {
    "NORMAL_CAR",
    "POOR_FAMILY_CAR",
    "RICH_FAMILY_CAR",
    "EXECUTIVE_CAR",
    "WORKER_CAR",
    "BIG_CAR",
    "TAXI_CAR",
    "MOPED_BIKE",
    "MOTOR_BIKE",
    "LEISURE_BOAT",
    "WORKER_BOAT",
    "BICYCLE",
    "",
};

// 0x00499C98
const char* g_apszKnockOffBikeTypes[] = {
    "KNOCKOFFBIKE_DEFAULT",
    "KNOCKOFFBIKE_NEVER",
    "KNOCKOFFBIKE_ALWAYSNORMAL",
    "KNOCKOFFBIKE_EASY",
    "",
};

// 0x00499CAC
const char* g_apszSwirlTypes[] = {
    "SWIRLS_NONE",
    "SWIRLS_LEFT",
    "SWIRLS_RIGHT",
    "SWIRLS_BOTH",
    "",
};

// 0x00499CC0
const char* g_apszClothesTextures[] = {
    "CLOTHES_TEX_SHIRT",
    "CLOTHES_TEX_HEAD",
    "CLOTHES_TEX_TROUSERS",
    "CLOTHES_TEX_SHOES",
    "CLOTHES_TEX_TATTOOS1",
    "CLOTHES_TEX_TATTOOS2",
    "CLOTHES_TEX_TATTOOS3",
    "CLOTHES_TEX_TATTOOS4",
    "CLOTHES_TEX_TATTOOS5",
    "CLOTHES_TEX_TATTOOS6",
    "CLOTHES_TEX_TATTOOS7",
    "CLOTHES_TEX_TATTOOS8",
    "CLOTHES_TEX_TATTOOS9",
    "CLOTHES_TEX_NECKLACE",
    "CLOTHES_TEX_WATCH",
    "CLOTHES_TEX_GLASSES",
    "CLOTHES_TEX_HAT",
    "CLOTHES_TEX_EXTRA1",
    "CLOTHES_MAX_TEXTURES",
    "",
};

// 0x00499D10
const char* g_apszCoordBlipAppearances[] = {
    "COORD_BLIP_APPEARANCE_NORMAL",
    "COORD_BLIP_APPEARANCE_FRIEND",
    "COORD_BLIP_APPEARANCE_ENEMY",
    "",
};

// 0x00499D20
const char* g_apszHudColours[] = {
    "HUD_COLOUR_RED",
    "HUD_COLOUR_GREEN",
    "HUD_COLOUR_DARKBLUE",
    "HUD_COLOUR_LIGHTBLUE",
#ifndef SERGEANUR_PATCH  // not in SA PC: the ids after it would be off by one
    "HUD_COLOUR_NEWBLUE",
#endif
    "HUD_COLOUR_WHITE",
    "HUD_COLOUR_BLACK",
    "HUD_COLOUR_YELLOW",
    "HUD_COLOUR_PURPLE",
    "HUD_COLOUR_GREY",
    "HUD_COLOUR_DARKRED",
    "HUD_COLOUR_DARKGREEN",
    "HUD_COLOUR_LIGHTYELLOW",
    "HUD_COLOUR_DARKBLUE_FRONTEND",
    "HUD_COLOUR_DARKBLUE_RADAR",
    "HUD_COLOUR_ENTRYEXIT_YELLOW",
    "",
};

// 0x00499D68
const char* g_apszGlobalSpeechContexts[] = {
    "CONTEXT_GLOBAL_NO_SPEECH",
    "CONTEXT_GLOBAL_ABUSE_GANG_BALLAS",
    "CONTEXT_GLOBAL_ABUSE_GANG_LSV",
    "CONTEXT_GLOBAL_ABUSE_GANG_VLA",
    "CONTEXT_GLOBAL_ABUSE_GANG_FAMILIES",
    "CONTEXT_GLOBAL_ABUSE_TRIAD",
    "CONTEXT_GLOBAL_ABUSE_MAFIA",
    "CONTEXT_GLOBAL_ABUSE_RIFA",
    "CONTEXT_GLOBAL_ABUSE_DA_NANG",
    "CONTEXT_GLOBAL_ACCEPT_DATE_CALL",
    "CONTEXT_GLOBAL_ACCEPT_DATE_REQUEST",
    "CONTEXT_GLOBAL_AFTER_SEX",
    "CONTEXT_GLOBAL_APOLOGY",
    "CONTEXT_GLOBAL_ARREST",
    "CONTEXT_GLOBAL_ARREST_CRIM",
    "CONTEXT_GLOBAL_ARRESTED",
    "CONTEXT_GLOBAL_ATTACK_BY_PLAYER_LIKE",
    "CONTEXT_GLOBAL_ATTACK_GANG_BALLAS",
    "CONTEXT_GLOBAL_ATTACK_GANG_LSV",
    "CONTEXT_GLOBAL_ATTACK_GANG_VLA",
    "CONTEXT_GLOBAL_ATTACK_PLAYER",
    "CONTEXT_GLOBAL_BAR_CHAT",
    "CONTEXT_GLOBAL_BLOCKED",
    "CONTEXT_GLOBAL_BOOZE_RECEIVE",
    "CONTEXT_GLOBAL_BOOZE_REQUEST",
    "CONTEXT_GLOBAL_BUM_BACK_OFF",
    "CONTEXT_GLOBAL_BUM_BACK_OFF_2",
    "CONTEXT_GLOBAL_BUM_LATCH",
    "CONTEXT_GLOBAL_BUMP",
    "CONTEXT_GLOBAL_CAR_CRASH",
    "CONTEXT_GLOBAL_CAR_DRIVEBY_BURN_RUBBER",
    "CONTEXT_GLOBAL_CAR_DRIVEBY_TOO_FAST",
    "CONTEXT_GLOBAL_CAR_FAST",
    "CONTEXT_GLOBAL_CAR_FIRE",
    "CONTEXT_GLOBAL_CAR_FLIPPED",
    "CONTEXT_GLOBAL_CAR_GET_IN",
    "CONTEXT_GLOBAL_CAR_HIT_PED",
    "CONTEXT_GLOBAL_CAR_JUMP",
    "CONTEXT_GLOBAL_CAR_POLICE_PURSUIT",
    "CONTEXT_GLOBAL_CAR_SLOW",
    "CONTEXT_GLOBAL_CAR_WAIT_FOR_ME",
    "CONTEXT_GLOBAL_CAR_SINGALONG",
    "CONTEXT_GLOBAL_CB_CHAT",
    "CONTEXT_GLOBAL_CHASE_FOOT",
    "CONTEXT_GLOBAL_CHASED",
    "CONTEXT_GLOBAL_CHAT",
    "CONTEXT_GLOBAL_COFFEE_ACCEPT",
    "CONTEXT_GLOBAL_COFFEE_DECLINE",
    "CONTEXT_GLOBAL_CONV_DISL_CAR",
    "CONTEXT_GLOBAL_CONV_DISL_CLOTHES",
    "CONTEXT_GLOBAL_CONV_DISL_HAIR",
    "CONTEXT_GLOBAL_CONV_DISL_PHYS",
    "CONTEXT_GLOBAL_CONV_DISL_SHOES",
    "CONTEXT_GLOBAL_CONV_DISL_SMELL",
    "CONTEXT_GLOBAL_CONV_DISL_TATTOO",
    "CONTEXT_GLOBAL_CONV_DISL_WEATHER",
    "CONTEXT_GLOBAL_CONV_IGNORED",
    "CONTEXT_GLOBAL_CONV_LIKE_CAR",
    "CONTEXT_GLOBAL_CONV_LIKE_CLOTHES",
    "CONTEXT_GLOBAL_CONV_LIKE_HAIR",
    "CONTEXT_GLOBAL_CONV_LIKE_PHYS",
    "CONTEXT_GLOBAL_CONV_LIKE_SHOES",
    "CONTEXT_GLOBAL_CONV_LIKE_SMELL",
    "CONTEXT_GLOBAL_CONV_LIKE_TATTOO",
    "CONTEXT_GLOBAL_CONV_LIKE_WEATHER",
    "CONTEXT_GLOBAL_COVER_ME",
    "CONTEXT_GLOBAL_CRASH_BIKE",
    "CONTEXT_GLOBAL_CRASH_CAR",
    "CONTEXT_GLOBAL_CRASH_GENERIC",
    "CONTEXT_GLOBAL_CRIMINAL_PLEAD",
    "CONTEXT_GLOBAL_DANCE_IMPR_HIGH",
    "CONTEXT_GLOBAL_DANCE_IMPR_LOW",
    "CONTEXT_GLOBAL_DANCE_IMPR_MED",
    "CONTEXT_GLOBAL_DANCE_IMPR_NOT",
    "CONTEXT_GLOBAL_DODGE",
    "CONTEXT_GLOBAL_DRUG_AGGRESSIVE_HIGH",
    "CONTEXT_GLOBAL_DRUG_AGGRESSIVE_LOW",
    "CONTEXT_GLOBAL_DRUG_DEALER_DISLIKE",
    "CONTEXT_GLOBAL_DRUG_DEALER_HATE",
    "CONTEXT_GLOBAL_DRUG_REASONABLE_HIGH",
    "CONTEXT_GLOBAL_DRUG_REASONABLE_LOW",
    "CONTEXT_GLOBAL_DRUGGED_CHAT",
    "CONTEXT_GLOBAL_DRUGGED_IGNORE",
    "CONTEXT_GLOBAL_DRUGS_BUY",
    "CONTEXT_GLOBAL_DRUGS_SELL",
    "CONTEXT_GLOBAL_DUCK",
    "CONTEXT_GLOBAL_EYEING_PED",
    "CONTEXT_GLOBAL_EYEING_PED_THREAT",
    "CONTEXT_GLOBAL_EYEING_PLAYER",
    "CONTEXT_GLOBAL_FIGHT",
    "CONTEXT_GLOBAL_FOLLOW_ARRIVE",
    "CONTEXT_GLOBAL_FOLLOW_CONSTANT",
    "CONTEXT_GLOBAL_FOLLOW_REPLY",
    "CONTEXT_GLOBAL_GAMB_BJ_HIT",
    "CONTEXT_GLOBAL_GAMB_BJ_LOSE",
    "CONTEXT_GLOBAL_GAMB_BJ_STAY",
    "CONTEXT_GLOBAL_GAMB_BJ_WIN",
    "CONTEXT_GLOBAL_GAMB_BJ_STICK",
    "CONTEXT_GLOBAL_GAMB_BJ_DOUBLE",
    "CONTEXT_GLOBAL_GAMB_BJ_SPLIT",
    "CONTEXT_GLOBAL_GAMB_CASINO_WIN",
    "CONTEXT_GLOBAL_GAMB_CASINO_LOSE",
    "CONTEXT_GLOBAL_GAMB_CONGRATS",
    "CONTEXT_GLOBAL_GAMB_ROUL_CHAT",
    "CONTEXT_GLOBAL_GAMB_SLOT_WIN",
    "CONTEXT_GLOBAL_GANG_FULL",
    "CONTEXT_GLOBAL_GANGBANG_NO",
    "CONTEXT_GLOBAL_GANGBANG_YES",
    "CONTEXT_GLOBAL_GENERIC_HI_MALE",
    "CONTEXT_GLOBAL_GENERIC_HI_FEMALE",
    "CONTEXT_GLOBAL_GENERIC_INSULT_MALE",
    "CONTEXT_GLOBAL_GENERIC_INSULT_FEMALE",
    "CONTEXT_GLOBAL_GIVING_HEAD",
    "CONTEXT_GLOBAL_GOOD_CITIZEN",
    "CONTEXT_GLOBAL_GUN_COOL",
    "CONTEXT_GLOBAL_GUN_RUN",
    "CONTEXT_GLOBAL_HAVING_SEX",
    "CONTEXT_GLOBAL_JACKED_CAR",
    "CONTEXT_GLOBAL_JACKED_GENERIC",
    "CONTEXT_GLOBAL_JACKED_ON_STREET",
    "CONTEXT_GLOBAL_JACKING_BIKE",
    "CONTEXT_GLOBAL_JACKING_CAR_FEM",
    "CONTEXT_GLOBAL_JACKING_CAR_MALE",
    "CONTEXT_GLOBAL_JACKING_GENERIC",
    "CONTEXT_GLOBAL_JOIN_GANG_NO",
    "CONTEXT_GLOBAL_JOIN_GANG_YES",
    "CONTEXT_GLOBAL_JOIN_ME_ASK",
    "CONTEXT_GLOBAL_JOIN_ME_REJECTED",
    "CONTEXT_GLOBAL_LIKE_CAR_REPLY",
    "CONTEXT_GLOBAL_LIKE_CLOTHES_REPLY",
    "CONTEXT_GLOBAL_LIKE_DISMISS_FEMALE",
    "CONTEXT_GLOBAL_LIKE_DISMISS_MALE",
    "CONTEXT_GLOBAL_LIKE_DISMISS_REPLY",
    "CONTEXT_GLOBAL_LIKE_HAIR_REPLY",
    "CONTEXT_GLOBAL_LIKE_NEGATIVE_FEMALE",
    "CONTEXT_GLOBAL_LIKE_NEGATIVE_MALE",
    "CONTEXT_GLOBAL_LIKE_PHYS_REPLY",
    "CONTEXT_GLOBAL_LIKE_SHOES_REPLY",
    "CONTEXT_GLOBAL_LIKE_SMELL_REPLY",
    "CONTEXT_GLOBAL_LIKE_TATTOO_REPLY",
    "CONTEXT_GLOBAL_MEET_GFRIEND_AGAIN_MAYBE",
    "CONTEXT_GLOBAL_MEET_GFRIEND_AGAIN_NO",
    "CONTEXT_GLOBAL_MEET_GFRIEND_AGAIN_YES",
    "CONTEXT_GLOBAL_MOVE_IN",
    "CONTEXT_GLOBAL_MUGGED",
    "CONTEXT_GLOBAL_MUGGING",
    "CONTEXT_GLOBAL_ORDER_ATTACK_MANY",
    "CONTEXT_GLOBAL_ORDER_ATTACK_SINGLE",
    "CONTEXT_GLOBAL_ORDER_DISBAND_MANY",
    "CONTEXT_GLOBAL_ORDER_DISBAND_ONE",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_FAR_MANY",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_FAR_ONE",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_NEAR_MANY",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_NEAR_ONE",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_VNEAR_MANY",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_VNEAR_ONE",
    "CONTEXT_GLOBAL_ORDER_KEEP_UP_MANY",
    "CONTEXT_GLOBAL_ORDER_KEEP_UP_ONE",
    "CONTEXT_GLOBAL_ORDER_WAIT_MANY",
    "CONTEXT_GLOBAL_ORDER_WAIT_ONE",
    "CONTEXT_GLOBAL_PCONV_AGREE_BAD",
    "CONTEXT_GLOBAL_PCONV_AGREE_GOOD",
    "CONTEXT_GLOBAL_PCONV_ANS_NO",
    "CONTEXT_GLOBAL_PCONV_DISMISS",
    "CONTEXT_GLOBAL_PCONV_GREET_FEM",
    "CONTEXT_GLOBAL_PCONV_GREET_MALE",
    "CONTEXT_GLOBAL_PCONV_PART_FEM",
    "CONTEXT_GLOBAL_PCONV_PART_MALE",
    "CONTEXT_GLOBAL_PCONV_QUESTION",
    "CONTEXT_GLOBAL_PCONV_STATE_BAD",
    "CONTEXT_GLOBAL_PCONV_STATE_GOOD",
    "CONTEXT_GLOBAL_PICKUP_CASH",
    "CONTEXT_GLOBAL_POLICE_BOAT",
    "CONTEXT_GLOBAL_POLICE_HELICOPTER",
    "CONTEXT_GLOBAL_POLICE_OVERBOARD",
    "CONTEXT_GLOBAL_PULL_GUN",
    "CONTEXT_GLOBAL_ROPE",
    "CONTEXT_GLOBAL_RUN_FROM_FIGHT",
    "CONTEXT_GLOBAL_SAVED",
    "CONTEXT_GLOBAL_SEARCH",
    "CONTEXT_GLOBAL_SHOCKED",
    "CONTEXT_GLOBAL_SHOOT",
    "CONTEXT_GLOBAL_SHOOT_BALLAS",
    "CONTEXT_GLOBAL_SHOOT_GENERIC",
    "CONTEXT_GLOBAL_SHOOT_LSV",
    "CONTEXT_GLOBAL_SHOOT_VLA",
    "CONTEXT_GLOBAL_SHOOT_FAMILIES",
    "CONTEXT_GLOBAL_SHOP_BROWSE",
    "CONTEXT_GLOBAL_SHOP_BUY",
    "CONTEXT_GLOBAL_SHOP_SELL",
    "CONTEXT_GLOBAL_SHOP_LEAVE",
    "CONTEXT_GLOBAL_SOLICIT",
    "CONTEXT_GLOBAL_SOLICIT_GEN_NO",
    "CONTEXT_GLOBAL_SOLICIT_GEN_YES",
    "CONTEXT_GLOBAL_SOLICIT_PRO_NO",
    "CONTEXT_GLOBAL_SOLICIT_PRO_YES",
    "CONTEXT_GLOBAL_SOLICIT_THANKS",
    "CONTEXT_GLOBAL_SOLICIT_UNREASONABLE",
    "CONTEXT_GLOBAL_SOLO",
    "CONTEXT_GLOBAL_SPLIFF_RECEIVE",
    "CONTEXT_GLOBAL_SPLIFF_REQUEST",
    "CONTEXT_GLOBAL_STEALTH_ALERT_SOUND",
    "CONTEXT_GLOBAL_STEALTH_ALERT_SIGHT",
    "CONTEXT_GLOBAL_STEALTH_ALERT_GENERIC",
    "CONTEXT_GLOBAL_STEALTH_DEF_SIGHTING",
    "CONTEXT_GLOBAL_STEALTH_NOTHING_THERE",
    "CONTEXT_GLOBAL_SURROUNDED",
    "CONTEXT_GLOBAL_TAKE_TURF_LAS_COLINAS",
    "CONTEXT_GLOBAL_TAKE_TURF_LOS_FLORES",
    "CONTEXT_GLOBAL_TAKE_TURF_EAST_BEACH",
    "CONTEXT_GLOBAL_TAKE_TURF_EAST_LS",
    "CONTEXT_GLOBAL_TAKE_TURF_JEFFERSON",
    "CONTEXT_GLOBAL_TAKE_TURF_GLEN_PARK",
    "CONTEXT_GLOBAL_TAKE_TURF_IDLEWOOD",
    "CONTEXT_GLOBAL_TAKE_TURF_GANTON",
    "CONTEXT_GLOBAL_TAKE_TURF_LITTLE_MEXICO",
    "CONTEXT_GLOBAL_TAKE_TURF_WILLOWFIELD",
    "CONTEXT_GLOBAL_TAKE_TURF_PLAYA_DEL_SEVILLE",
    "CONTEXT_GLOBAL_TAKE_TURF_TEMPLE",
    "CONTEXT_GLOBAL_TARGET",
    "CONTEXT_GLOBAL_TAXI_BAIL",
    "CONTEXT_GLOBAL_TAXI_HAIL",
    "CONTEXT_GLOBAL_TAXI_HIT_PED",
    "CONTEXT_GLOBAL_TAXI_START",
    "CONTEXT_GLOBAL_TAXI_SUCCESS",
    "CONTEXT_GLOBAL_TAXI_TIP",
    "CONTEXT_GLOBAL_TRAPPED",
    "CONTEXT_GLOBAL_VALET_BAD",
    "CONTEXT_GLOBAL_VALET_GOOD",
    "CONTEXT_GLOBAL_VALET_PARK_CAR",
    "CONTEXT_GLOBAL_VAN",
    "CONTEXT_GLOBAL_VICTIM",
    "CONTEXT_GLOBAL_WEATHER_DISL_REPLY",
    "CONTEXT_GLOBAL_WEATHER_LIKE_REPLY",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM_NEG",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM_POS",
    "CONTEXT_GLOBAL_SPANKED",
    "CONTEXT_GLOBAL_SPANKING",
    "CONTEXT_GLOBAL_GREETING_GFRIEND",
    "CONTEXT_GLOBAL_PARTING_GFRIEND",
    "CONTEXT_GLOBAL_UH_HUH",
    "CONTEXT_GLOBAL_CLEAR_ATTACHED_PEDS",
    "CONTEXT_GLOBAL_AGREE_TO_DO_DRIVEBY",
    "CONTEXT_GLOBAL_AGREE_TO_LET_DRIVE",
    "CONTEXT_GLOBAL_MICHELLE_TAKE_CAR",
    "CONTEXT_GLOBAL_ACCEPT_SEX",
    "CONTEXT_GLOBAL_DECLINE_SEX",
    "CONTEXT_GLOBAL_GIVE_NUMBER_YES",
    "CONTEXT_GLOBAL_GIVE_NUMBER_NO",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM",
    "CONTEXT_GLOBAL_GANGBANG",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM_POS_REPLY",
    "CONTEXT_GLOBAL_DRIVE_THROUGH_TAUNT",
    "CONTEXT_GLOBAL_ATTACK_CAR",
    "CONTEXT_GLOBAL_TIP_CAR",
    "CONTEXT_GLOBAL_CHASE_CAR",
    "CONTEXT_GLOBAL_ENEMY_GANG_WASTED",
    "CONTEXT_GLOBAL_PLAYER_WASTED",
    "CONTEXT_GLOBAL_CHASE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DATE_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DATE_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_MEAL_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_MEAL_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DRIVE_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DRIVE_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DANCE_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DANCE_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_SEX_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_SEX_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_BORED_1",
    "CONTEXT_GLOBAL_GFRIEND_BORED_2",
    "CONTEXT_GLOBAL_GFRIEND_STORY",
    "CONTEXT_GLOBAL_GFRIEND_LIKE_MEAL_DEST",
    "CONTEXT_GLOBAL_GFRIEND_LIKE_CLUB_DEST",
    "CONTEXT_GLOBAL_GFRIEND_OFFER_DANCE",
    "CONTEXT_GLOBAL_GFRIEND_ENJOYED_MEAL_HIGH",
    "CONTEXT_GLOBAL_GFRIEND_ENJOYED_EVENT_LOW",
    "CONTEXT_GLOBAL_GFRIEND_ENJOYED_CLUB_HIGH",
    "CONTEXT_GLOBAL_GFRIEND_TAKE_HOME_HAPPY",
    "CONTEXT_GLOBAL_GFRIEND_TAKE_HOME_ANGRY",
    "CONTEXT_GLOBAL_GFRIEND_COFFEE",
    "CONTEXT_GLOBAL_GFRIEND_MOAN",
    "CONTEXT_GLOBAL_GFRIEND_HEAD",
    "CONTEXT_GLOBAL_GFRIEND_CLIMAX_HIGH",
    "CONTEXT_GLOBAL_GFRIEND_CLIMAX_LOW",
    "CONTEXT_GLOBAL_GFRIEND_SEX_GOOD",
    "CONTEXT_GLOBAL_GFRIEND_SEX_BAD",
    "CONTEXT_GLOBAL_GFRIEND_MEET_AGAIN",
    "CONTEXT_GLOBAL_GFRIEND_JEALOUS",
    "CONTEXT_GLOBAL_GFRIEND_JEALOUS_REPLY",
    "CONTEXT_GLOBAL_GFRIEND_GOODBYE_HAPPY",
    "CONTEXT_GLOBAL_GFRIEND_GOODBYE_ANGRY",
    "CONTEXT_GLOBAL_GFRIEND_LEFT_BEHIND",
    "CONTEXT_GLOBAL_GFRIEND_HELLO",
    "CONTEXT_GLOBAL_GFRIEND_GOODBYE",
    "CONTEXT_GLOBAL_GFRIEND_PICKUP_LOCATION",
    "CONTEXT_GLOBAL_GFRIEND_PARK_UP",
    "CONTEXT_GLOBAL_GFRIEND_PARK_LOCATION_HATE",
    "CONTEXT_GLOBAL_GFRIEND_GIFT_LIKE",
    "CONTEXT_GLOBAL_GFRIEND_CHANGE_RADIO_FAVE",
    "CONTEXT_GLOBAL_GFRIEND_CHANGE_RADIO_BACK",
    "CONTEXT_GLOBAL_GFRIEND_DO_A_DRIVEBY",
    "CONTEXT_GLOBAL_GFRIEND_START_A_FIGHT",
    "CONTEXT_GLOBAL_GFRIEND_REJECT_DATE",
    "CONTEXT_GLOBAL_GFRIEND_REQUEST_TO_DRIVE_CAR",
    "CONTEXT_GLOBAL_GFRIEND_DROP_PLAYER_DRIVE_AWAY",
    "CONTEXT_GLOBAL_GFRIEND_DISLIKE_CURRENT_ZONE",
    "CONTEXT_GLOBAL_GFRIEND_LIKE_CURRENT_ZONE",
    "CONTEXT_GLOBAL_GFRIEND_HIT_BY_PLAYER_WARNING",
    "CONTEXT_GLOBAL_GFRIEND_DUMP_PLAYER_LIVE",
    "CONTEXT_GLOBAL_GFRIEND_DUMP_PLAYER_PHONE",
    "CONTEXT_GLOBAL_GFRIEND_SEX_APPEAL_TOO_LOW",
    "CONTEXT_GLOBAL_GFRIEND_PHYSIQUE_CRITIQUE",
    "CONTEXT_GLOBAL_GFRIEND_INTRO",
    "CONTEXT_GLOBAL_GFRIEND_NEG_RESPONSE",
    "CONTEXT_GLOBAL_GFRIEND_POS_RESPONSE",
    "CONTEXT_GLOBAL_BOXING_CHEER",
    "CONTEXT_GLOBAL_BOUGHT_ENOUGH",
    "CONTEXT_GLOBAL_GIVE_PRODUCT",
    "CONTEXT_GLOBAL_NO_MONEY",
    "CONTEXT_GLOBAL_PLAYER_SICK",
    "CONTEXT_GLOBAL_REMOVE_TATTOO",
    "CONTEXT_GLOBAL_SHOP_CLOSED",
    "CONTEXT_GLOBAL_SHOW_CHANGINGROOM",
    "CONTEXT_GLOBAL_SHOP_CHAT",
    "CONTEXT_GLOBAL_TAKE_A_SEAT",
    "CONTEXT_GLOBAL_THANKS_FOR_CUSTOM",
    "CONTEXT_GLOBAL_WELCOME_TO_SHOP",
    "CONTEXT_GLOBAL_WHAT_WANT",
    "CONTEXT_GLOBAL_PHOTO_CARL",
    "CONTEXT_GLOBAL_PHOTO_CHEESE",
    "CONTEXT_GLOBAL_SINGING",
    "CONTEXT_GLOBAL_STOMACH_RUMBLE",
    "CONTEXT_GLOBAL_BREATHING",
    "CONTEXT_GLOBAL_PAIN_START",
    "CONTEXT_GLOBAL_PAIN_COUGH,",
    "CONTEXT_GLOBAL_PAIN_DEATH_DROWN",
    "CONTEXT_GLOBAL_PAIN_DEATH_HIGH",
    "CONTEXT_GLOBAL_PAIN_DEATH_LOW",
    "CONTEXT_GLOBAL_PAIN_HIGH",
    "CONTEXT_GLOBAL_PAIN_LOW",
    "CONTEXT_GLOBAL_PAIN_ON_FIRE",
    "CONTEXT_GLOBAL_PAIN_PANIC",
    "CONTEXT_GLOBAL_PAIN_SPRAYED",
    "CONTEXT_GLOBAL_PAIN_CJ_BOXING",
    "CONTEXT_GLOBAL_PAIN_CJ_GRUNT",
    "CONTEXT_GLOBAL_PAIN_CJ_PANT_IN",
    "CONTEXT_GLOBAL_PAIN_CJ_PANT_OUT",
    "CONTEXT_GLOBAL_PAIN_CJ_PUKE",
    "CONTEXT_GLOBAL_PAIN_CJ_STRAIN",
    "CONTEXT_GLOBAL_PAIN_CJ_STRAIN_EXHALE",
    "CONTEXT_GLOBAL_PAIN_CJ_SWIM_GASP",
    "CONTEXT_GLOBAL_PAIN_CJ_DROWNING",
    "CONTEXT_GLOBAL_PAIN_CJ_HIGH_FALL",
    "CONTEXT_GLOBAL_PAIN_END",
    "CONTEXT_GLOBAL_END",
    "",
};

// 0x0049A300
const char* g_apszGlobalSpeechContextsPC[] = {
    "CONTEXT_GLOBAL_NO_SPEECH",
    "CONTEXT_GLOBAL_ABUSE_GANG_BALLAS",
    "CONTEXT_GLOBAL_ABUSE_GANG_LSV",
    "CONTEXT_GLOBAL_ABUSE_GANG_VLA",
    "CONTEXT_GLOBAL_ABUSE_GANG_FAMILIES",
    "CONTEXT_GLOBAL_ABUSE_TRIAD",
    "CONTEXT_GLOBAL_ABUSE_MAFIA",
    "CONTEXT_GLOBAL_ABUSE_RIFA",
    "CONTEXT_GLOBAL_ABUSE_DA_NANG",
    "CONTEXT_GLOBAL_ACCEPT_DATE_CALL",
    "CONTEXT_GLOBAL_ACCEPT_DATE_REQUEST",
    "CONTEXT_GLOBAL_AFTER_SEX",
    "CONTEXT_GLOBAL_APOLOGY",
    "CONTEXT_GLOBAL_ARREST",
    "CONTEXT_GLOBAL_ARREST_CRIM",
    "CONTEXT_GLOBAL_ARRESTED",
    "CONTEXT_GLOBAL_ATTACK_BY_PLAYER_LIKE",
    "CONTEXT_GLOBAL_ATTACK_GANG_BALLAS",
    "CONTEXT_GLOBAL_ATTACK_GANG_LSV",
    "CONTEXT_GLOBAL_ATTACK_GANG_VLA",
    "CONTEXT_GLOBAL_ATTACK_PLAYER",
    "CONTEXT_GLOBAL_BAR_CHAT",
    "CONTEXT_GLOBAL_BLOCKED",
    "CONTEXT_GLOBAL_BOOZE_RECEIVE",
    "CONTEXT_GLOBAL_BOOZE_REQUEST",
    "CONTEXT_GLOBAL_BUM_BACK_OFF",
    "CONTEXT_GLOBAL_BUM_BACK_OFF_2",
    "CONTEXT_GLOBAL_BUM_LATCH",
    "CONTEXT_GLOBAL_BUMP",
    "CONTEXT_GLOBAL_CAR_CRASH",
    "CONTEXT_GLOBAL_CAR_DRIVEBY_BURN_RUBBER",
    "CONTEXT_GLOBAL_CAR_DRIVEBY_TOO_FAST",
    "CONTEXT_GLOBAL_CAR_FAST",
    "CONTEXT_GLOBAL_CAR_FIRE",
    "CONTEXT_GLOBAL_CAR_FLIPPED",
    "CONTEXT_GLOBAL_CAR_GET_IN",
    "CONTEXT_GLOBAL_CAR_HIT_PED",
    "CONTEXT_GLOBAL_CAR_JUMP",
    "CONTEXT_GLOBAL_CAR_POLICE_PURSUIT",
    "CONTEXT_GLOBAL_CAR_SLOW",
    "CONTEXT_GLOBAL_CAR_WAIT_FOR_ME",
    "CONTEXT_GLOBAL_CAR_SINGALONG",
    "CONTEXT_GLOBAL_CB_CHAT",
    "CONTEXT_GLOBAL_CHASE_FOOT",
    "CONTEXT_GLOBAL_CHASED",
    "CONTEXT_GLOBAL_CHAT",
    "CONTEXT_GLOBAL_COFFEE_ACCEPT",
    "CONTEXT_GLOBAL_COFFEE_DECLINE",
    "CONTEXT_GLOBAL_CONV_DISL_CAR",
    "CONTEXT_GLOBAL_CONV_DISL_CLOTHES",
    "CONTEXT_GLOBAL_CONV_DISL_HAIR",
    "CONTEXT_GLOBAL_CONV_DISL_PHYS",
    "CONTEXT_GLOBAL_CONV_DISL_SHOES",
    "CONTEXT_GLOBAL_CONV_DISL_SMELL",
    "CONTEXT_GLOBAL_CONV_DISL_TATTOO",
    "CONTEXT_GLOBAL_CONV_DISL_WEATHER",
    "CONTEXT_GLOBAL_CONV_IGNORED",
    "CONTEXT_GLOBAL_CONV_LIKE_CAR",
    "CONTEXT_GLOBAL_CONV_LIKE_CLOTHES",
    "CONTEXT_GLOBAL_CONV_LIKE_HAIR",
    "CONTEXT_GLOBAL_CONV_LIKE_PHYS",
    "CONTEXT_GLOBAL_CONV_LIKE_SHOES",
    "CONTEXT_GLOBAL_CONV_LIKE_SMELL",
    "CONTEXT_GLOBAL_CONV_LIKE_TATTOO",
    "CONTEXT_GLOBAL_CONV_LIKE_WEATHER",
    "CONTEXT_GLOBAL_COVER_ME",
    "CONTEXT_GLOBAL_CRASH_BIKE",
    "CONTEXT_GLOBAL_CRASH_CAR",
    "CONTEXT_GLOBAL_CRASH_GENERIC",
    "CONTEXT_GLOBAL_CRIMINAL_PLEAD",
    "CONTEXT_GLOBAL_DANCE_IMPR_HIGH",
    "CONTEXT_GLOBAL_DANCE_IMPR_LOW",
    "CONTEXT_GLOBAL_DANCE_IMPR_MED",
    "CONTEXT_GLOBAL_DANCE_IMPR_NOT",
    "CONTEXT_GLOBAL_DODGE",
    "CONTEXT_GLOBAL_DRUG_AGGRESSIVE_HIGH",
    "CONTEXT_GLOBAL_DRUG_AGGRESSIVE_LOW",
    "CONTEXT_GLOBAL_DRUG_DEALER_DISLIKE",
    "CONTEXT_GLOBAL_DRUG_DEALER_HATE",
    "CONTEXT_GLOBAL_DRUG_REASONABLE_HIGH",
    "CONTEXT_GLOBAL_DRUG_REASONABLE_LOW",
    "CONTEXT_GLOBAL_DRUGGED_CHAT",
    "CONTEXT_GLOBAL_DRUGGED_IGNORE",
    "CONTEXT_GLOBAL_DRUGS_BUY",
    "CONTEXT_GLOBAL_DRUGS_SELL",
    "CONTEXT_GLOBAL_DUCK",
    "CONTEXT_GLOBAL_EYEING_PED",
    "CONTEXT_GLOBAL_EYEING_PED_THREAT",
    "CONTEXT_GLOBAL_EYEING_PLAYER",
    "CONTEXT_GLOBAL_FIGHT",
    "CONTEXT_GLOBAL_FOLLOW_ARRIVE",
    "CONTEXT_GLOBAL_FOLLOW_CONSTANT",
    "CONTEXT_GLOBAL_FOLLOW_REPLY",
    "CONTEXT_GLOBAL_GAMB_BJ_HIT",
    "CONTEXT_GLOBAL_GAMB_BJ_LOSE",
    "CONTEXT_GLOBAL_GAMB_BJ_STAY",
    "CONTEXT_GLOBAL_GAMB_BJ_WIN",
    "CONTEXT_GLOBAL_GAMB_BJ_STICK",
    "CONTEXT_GLOBAL_GAMB_BJ_DOUBLE",
    "CONTEXT_GLOBAL_GAMB_BJ_SPLIT",
    "CONTEXT_GLOBAL_GAMB_CASINO_WIN",
    "CONTEXT_GLOBAL_GAMB_CASINO_LOSE",
    "CONTEXT_GLOBAL_GAMB_CONGRATS",
    "CONTEXT_GLOBAL_GAMB_ROUL_CHAT",
    "CONTEXT_GLOBAL_GAMB_SLOT_WIN",
    "CONTEXT_GLOBAL_GANG_FULL",
    "CONTEXT_GLOBAL_GANGBANG_NO",
    "CONTEXT_GLOBAL_GANGBANG_YES",
    "CONTEXT_GLOBAL_GENERIC_HI_MALE",
    "CONTEXT_GLOBAL_GENERIC_HI_FEMALE",
    "CONTEXT_GLOBAL_GENERIC_INSULT_MALE",
    "CONTEXT_GLOBAL_GENERIC_INSULT_FEMALE",
    "CONTEXT_GLOBAL_GIVING_HEAD",
    "CONTEXT_GLOBAL_GOOD_CITIZEN",
    "CONTEXT_GLOBAL_GUN_COOL",
    "CONTEXT_GLOBAL_GUN_RUN",
    "CONTEXT_GLOBAL_HAVING_SEX",
    "CONTEXT_GLOBAL_HAVING_SEX_MUFFLED",
    "CONTEXT_GLOBAL_JACKED_CAR",
    "CONTEXT_GLOBAL_JACKED_GENERIC",
    "CONTEXT_GLOBAL_JACKED_ON_STREET",
    "CONTEXT_GLOBAL_JACKING_BIKE",
    "CONTEXT_GLOBAL_JACKING_CAR_FEM",
    "CONTEXT_GLOBAL_JACKING_CAR_MALE",
    "CONTEXT_GLOBAL_JACKING_GENERIC",
    "CONTEXT_GLOBAL_JOIN_GANG_NO",
    "CONTEXT_GLOBAL_JOIN_GANG_YES",
    "CONTEXT_GLOBAL_JOIN_ME_ASK",
    "CONTEXT_GLOBAL_JOIN_ME_REJECTED",
    "CONTEXT_GLOBAL_LIKE_CAR_REPLY",
    "CONTEXT_GLOBAL_LIKE_CLOTHES_REPLY",
    "CONTEXT_GLOBAL_LIKE_DISMISS_FEMALE",
    "CONTEXT_GLOBAL_LIKE_DISMISS_MALE",
    "CONTEXT_GLOBAL_LIKE_DISMISS_REPLY",
    "CONTEXT_GLOBAL_LIKE_HAIR_REPLY",
    "CONTEXT_GLOBAL_LIKE_NEGATIVE_FEMALE",
    "CONTEXT_GLOBAL_LIKE_NEGATIVE_MALE",
    "CONTEXT_GLOBAL_LIKE_PHYS_REPLY",
    "CONTEXT_GLOBAL_LIKE_SHOES_REPLY",
    "CONTEXT_GLOBAL_LIKE_SMELL_REPLY",
    "CONTEXT_GLOBAL_LIKE_TATTOO_REPLY",
    "CONTEXT_GLOBAL_MEET_GFRIEND_AGAIN_MAYBE",
    "CONTEXT_GLOBAL_MEET_GFRIEND_AGAIN_NO",
    "CONTEXT_GLOBAL_MEET_GFRIEND_AGAIN_YES",
    "CONTEXT_GLOBAL_MOVE_IN",
    "CONTEXT_GLOBAL_MUGGED",
    "CONTEXT_GLOBAL_MUGGING",
    "CONTEXT_GLOBAL_ORDER_ATTACK_MANY",
    "CONTEXT_GLOBAL_ORDER_ATTACK_SINGLE",
    "CONTEXT_GLOBAL_ORDER_DISBAND_MANY",
    "CONTEXT_GLOBAL_ORDER_DISBAND_ONE",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_FAR_MANY",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_FAR_ONE",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_NEAR_MANY",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_NEAR_ONE",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_VNEAR_MANY",
    "CONTEXT_GLOBAL_ORDER_FOLLOW_VNEAR_ONE",
    "CONTEXT_GLOBAL_ORDER_KEEP_UP_MANY",
    "CONTEXT_GLOBAL_ORDER_KEEP_UP_ONE",
    "CONTEXT_GLOBAL_ORDER_WAIT_MANY",
    "CONTEXT_GLOBAL_ORDER_WAIT_ONE",
    "CONTEXT_GLOBAL_PCONV_AGREE_BAD",
    "CONTEXT_GLOBAL_PCONV_AGREE_GOOD",
    "CONTEXT_GLOBAL_PCONV_ANS_NO",
    "CONTEXT_GLOBAL_PCONV_DISMISS",
    "CONTEXT_GLOBAL_PCONV_GREET_FEM",
    "CONTEXT_GLOBAL_PCONV_GREET_MALE",
    "CONTEXT_GLOBAL_PCONV_PART_FEM",
    "CONTEXT_GLOBAL_PCONV_PART_MALE",
    "CONTEXT_GLOBAL_PCONV_QUESTION",
    "CONTEXT_GLOBAL_PCONV_STATE_BAD",
    "CONTEXT_GLOBAL_PCONV_STATE_GOOD",
    "CONTEXT_GLOBAL_PICKUP_CASH",
    "CONTEXT_GLOBAL_POLICE_BOAT",
    "CONTEXT_GLOBAL_POLICE_HELICOPTER",
    "CONTEXT_GLOBAL_POLICE_OVERBOARD",
    "CONTEXT_GLOBAL_PULL_GUN",
    "CONTEXT_GLOBAL_ROPE",
    "CONTEXT_GLOBAL_RUN_FROM_FIGHT",
    "CONTEXT_GLOBAL_SAVED",
    "CONTEXT_GLOBAL_SEARCH",
    "CONTEXT_GLOBAL_SHOCKED",
    "CONTEXT_GLOBAL_SHOOT",
    "CONTEXT_GLOBAL_SHOOT_BALLAS",
    "CONTEXT_GLOBAL_SHOOT_GENERIC",
    "CONTEXT_GLOBAL_SHOOT_LSV",
    "CONTEXT_GLOBAL_SHOOT_VLA",
    "CONTEXT_GLOBAL_SHOOT_FAMILIES",
    "CONTEXT_GLOBAL_SHOP_BROWSE",
    "CONTEXT_GLOBAL_SHOP_BUY",
    "CONTEXT_GLOBAL_SHOP_SELL",
    "CONTEXT_GLOBAL_SHOP_LEAVE",
    "CONTEXT_GLOBAL_SOLICIT",
    "CONTEXT_GLOBAL_SOLICIT_GEN_NO",
    "CONTEXT_GLOBAL_SOLICIT_GEN_YES",
    "CONTEXT_GLOBAL_SOLICIT_PRO_NO",
    "CONTEXT_GLOBAL_SOLICIT_PRO_YES",
    "CONTEXT_GLOBAL_SOLICIT_THANKS",
    "CONTEXT_GLOBAL_SOLICIT_UNREASONABLE",
    "CONTEXT_GLOBAL_SOLO",
    "CONTEXT_GLOBAL_SPLIFF_RECEIVE",
    "CONTEXT_GLOBAL_SPLIFF_REQUEST",
    "CONTEXT_GLOBAL_STEALTH_ALERT_SOUND",
    "CONTEXT_GLOBAL_STEALTH_ALERT_SIGHT",
    "CONTEXT_GLOBAL_STEALTH_ALERT_GENERIC",
    "CONTEXT_GLOBAL_STEALTH_DEF_SIGHTING",
    "CONTEXT_GLOBAL_STEALTH_NOTHING_THERE",
    "CONTEXT_GLOBAL_SURROUNDED",
    "CONTEXT_GLOBAL_TAKE_TURF_LAS_COLINAS",
    "CONTEXT_GLOBAL_TAKE_TURF_LOS_FLORES",
    "CONTEXT_GLOBAL_TAKE_TURF_EAST_BEACH",
    "CONTEXT_GLOBAL_TAKE_TURF_EAST_LS",
    "CONTEXT_GLOBAL_TAKE_TURF_JEFFERSON",
    "CONTEXT_GLOBAL_TAKE_TURF_GLEN_PARK",
    "CONTEXT_GLOBAL_TAKE_TURF_IDLEWOOD",
    "CONTEXT_GLOBAL_TAKE_TURF_GANTON",
    "CONTEXT_GLOBAL_TAKE_TURF_LITTLE_MEXICO",
    "CONTEXT_GLOBAL_TAKE_TURF_WILLOWFIELD",
    "CONTEXT_GLOBAL_TAKE_TURF_PLAYA_DEL_SEVILLE",
    "CONTEXT_GLOBAL_TAKE_TURF_TEMPLE",
    "CONTEXT_GLOBAL_TARGET",
    "CONTEXT_GLOBAL_TAXI_BAIL",
    "CONTEXT_GLOBAL_TAXI_HAIL",
    "CONTEXT_GLOBAL_TAXI_HIT_PED",
    "CONTEXT_GLOBAL_TAXI_START",
    "CONTEXT_GLOBAL_TAXI_SUCCESS",
    "CONTEXT_GLOBAL_TAXI_TIP",
    "CONTEXT_GLOBAL_TRAPPED",
    "CONTEXT_GLOBAL_VALET_BAD",
    "CONTEXT_GLOBAL_VALET_GOOD",
    "CONTEXT_GLOBAL_VALET_PARK_CAR",
    "CONTEXT_GLOBAL_VAN",
    "CONTEXT_GLOBAL_VICTIM",
    "CONTEXT_GLOBAL_WEATHER_DISL_REPLY",
    "CONTEXT_GLOBAL_WEATHER_LIKE_REPLY",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM_NEG",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM_POS",
    "CONTEXT_GLOBAL_SPANKED",
    "CONTEXT_GLOBAL_SPANKING",
    "CONTEXT_GLOBAL_SPANKING_MUFFLED",
    "CONTEXT_GLOBAL_GREETING_GFRIEND",
    "CONTEXT_GLOBAL_PARTING_GFRIEND",
    "CONTEXT_GLOBAL_UH_HUH",
    "CONTEXT_GLOBAL_CLEAR_ATTACHED_PEDS",
    "CONTEXT_GLOBAL_AGREE_TO_DO_DRIVEBY",
    "CONTEXT_GLOBAL_AGREE_TO_LET_DRIVE",
    "CONTEXT_GLOBAL_MICHELLE_TAKE_CAR",
    "CONTEXT_GLOBAL_ACCEPT_SEX",
    "CONTEXT_GLOBAL_DECLINE_SEX",
    "CONTEXT_GLOBAL_GIVE_NUMBER_YES",
    "CONTEXT_GLOBAL_GIVE_NUMBER_NO",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM",
    "CONTEXT_GLOBAL_GANGBANG",
    "CONTEXT_GLOBAL_WHERE_YOU_FROM_POS_REPLY",
    "CONTEXT_GLOBAL_DRIVE_THROUGH_TAUNT",
    "CONTEXT_GLOBAL_ATTACK_CAR",
    "CONTEXT_GLOBAL_TIP_CAR",
    "CONTEXT_GLOBAL_CHASE_CAR",
    "CONTEXT_GLOBAL_ENEMY_GANG_WASTED",
    "CONTEXT_GLOBAL_PLAYER_WASTED",
    "CONTEXT_GLOBAL_CHASE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DATE_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DATE_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_MEAL_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_MEAL_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DRIVE_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DRIVE_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DANCE_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_DANCE_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_REQ_SEX_DESPERATE",
    "CONTEXT_GLOBAL_GFRIEND_REQ_SEX_NORMAL",
    "CONTEXT_GLOBAL_GFRIEND_BORED_1",
    "CONTEXT_GLOBAL_GFRIEND_BORED_2",
    "CONTEXT_GLOBAL_GFRIEND_STORY",
    "CONTEXT_GLOBAL_GFRIEND_LIKE_MEAL_DEST",
    "CONTEXT_GLOBAL_GFRIEND_LIKE_CLUB_DEST",
    "CONTEXT_GLOBAL_GFRIEND_OFFER_DANCE",
    "CONTEXT_GLOBAL_GFRIEND_ENJOYED_MEAL_HIGH",
    "CONTEXT_GLOBAL_GFRIEND_ENJOYED_EVENT_LOW",
    "CONTEXT_GLOBAL_GFRIEND_ENJOYED_CLUB_HIGH",
    "CONTEXT_GLOBAL_GFRIEND_TAKE_HOME_HAPPY",
    "CONTEXT_GLOBAL_GFRIEND_TAKE_HOME_ANGRY",
    "CONTEXT_GLOBAL_GFRIEND_COFFEE",
    "CONTEXT_GLOBAL_GFRIEND_MOAN",
    "CONTEXT_GLOBAL_GFRIEND_MOAN_MUFFLED",
    "CONTEXT_GLOBAL_GFRIEND_HEAD",
    "CONTEXT_GLOBAL_GFRIEND_CLIMAX_HIGH",
    "CONTEXT_GLOBAL_GFRIEND_CLIMAX_HIGH_MUFFLED",
    "CONTEXT_GLOBAL_GFRIEND_CLIMAX_LOW",
    "CONTEXT_GLOBAL_GFRIEND_SEX_GOOD",
    "CONTEXT_GLOBAL_GFRIEND_SEX_GOOD_MUFFLED",
    "CONTEXT_GLOBAL_GFRIEND_SEX_BAD",
    "CONTEXT_GLOBAL_GFRIEND_MEET_AGAIN",
    "CONTEXT_GLOBAL_GFRIEND_JEALOUS",
    "CONTEXT_GLOBAL_GFRIEND_JEALOUS_REPLY",
    "CONTEXT_GLOBAL_GFRIEND_GOODBYE_HAPPY",
    "CONTEXT_GLOBAL_GFRIEND_GOODBYE_ANGRY",
    "CONTEXT_GLOBAL_GFRIEND_LEFT_BEHIND",
    "CONTEXT_GLOBAL_GFRIEND_HELLO",
    "CONTEXT_GLOBAL_GFRIEND_GOODBYE",
    "CONTEXT_GLOBAL_GFRIEND_PICKUP_LOCATION",
    "CONTEXT_GLOBAL_GFRIEND_PARK_UP",
    "CONTEXT_GLOBAL_GFRIEND_PARK_LOCATION_HATE",
    "CONTEXT_GLOBAL_GFRIEND_GIFT_LIKE",
    "CONTEXT_GLOBAL_GFRIEND_CHANGE_RADIO_FAVE",
    "CONTEXT_GLOBAL_GFRIEND_CHANGE_RADIO_BACK",
    "CONTEXT_GLOBAL_GFRIEND_DO_A_DRIVEBY",
    "CONTEXT_GLOBAL_GFRIEND_START_A_FIGHT",
    "CONTEXT_GLOBAL_GFRIEND_REJECT_DATE",
    "CONTEXT_GLOBAL_GFRIEND_REQUEST_TO_DRIVE_CAR",
    "CONTEXT_GLOBAL_GFRIEND_DROP_PLAYER_DRIVE_AWAY",
    "CONTEXT_GLOBAL_GFRIEND_DISLIKE_CURRENT_ZONE",
    "CONTEXT_GLOBAL_GFRIEND_LIKE_CURRENT_ZONE",
    "CONTEXT_GLOBAL_GFRIEND_HIT_BY_PLAYER_WARNING",
    "CONTEXT_GLOBAL_GFRIEND_DUMP_PLAYER_LIVE",
    "CONTEXT_GLOBAL_GFRIEND_DUMP_PLAYER_PHONE",
    "CONTEXT_GLOBAL_GFRIEND_SEX_APPEAL_TOO_LOW",
    "CONTEXT_GLOBAL_GFRIEND_PHYSIQUE_CRITIQUE",
    "CONTEXT_GLOBAL_GFRIEND_INTRO",
    "CONTEXT_GLOBAL_GFRIEND_NEG_RESPONSE",
    "CONTEXT_GLOBAL_GFRIEND_POS_RESPONSE",
    "CONTEXT_GLOBAL_BOXING_CHEER",
    "CONTEXT_GLOBAL_BOUGHT_ENOUGH",
    "CONTEXT_GLOBAL_GIVE_PRODUCT",
    "CONTEXT_GLOBAL_NO_MONEY",
    "CONTEXT_GLOBAL_PLAYER_SICK",
    "CONTEXT_GLOBAL_REMOVE_TATTOO",
    "CONTEXT_GLOBAL_SHOP_CLOSED",
    "CONTEXT_GLOBAL_SHOW_CHANGINGROOM",
    "CONTEXT_GLOBAL_SHOP_CHAT",
    "CONTEXT_GLOBAL_TAKE_A_SEAT",
    "CONTEXT_GLOBAL_THANKS_FOR_CUSTOM",
    "CONTEXT_GLOBAL_WELCOME_TO_SHOP",
    "CONTEXT_GLOBAL_WHAT_WANT",
    "CONTEXT_GLOBAL_PHOTO_CARL",
    "CONTEXT_GLOBAL_PHOTO_CHEESE",
    "CONTEXT_GLOBAL_SINGING",
    "CONTEXT_GLOBAL_STOMACH_RUMBLE",
    "CONTEXT_GLOBAL_BREATHING",
    "CONTEXT_GLOBAL_PAIN_START",
    "CONTEXT_GLOBAL_PAIN_COUGH,",
    "CONTEXT_GLOBAL_PAIN_DEATH_DROWN",
    "CONTEXT_GLOBAL_PAIN_DEATH_HIGH",
    "CONTEXT_GLOBAL_PAIN_DEATH_LOW",
    "CONTEXT_GLOBAL_PAIN_HIGH",
    "CONTEXT_GLOBAL_PAIN_LOW",
    "CONTEXT_GLOBAL_PAIN_ON_FIRE",
    "CONTEXT_GLOBAL_PAIN_PANIC",
    "CONTEXT_GLOBAL_PAIN_SPRAYED",
    "CONTEXT_GLOBAL_PAIN_CJ_BOXING",
    "CONTEXT_GLOBAL_PAIN_CJ_GRUNT",
    "CONTEXT_GLOBAL_PAIN_CJ_PANT_IN",
    "CONTEXT_GLOBAL_PAIN_CJ_PANT_OUT",
    "CONTEXT_GLOBAL_PAIN_CJ_PUKE",
    "CONTEXT_GLOBAL_PAIN_CJ_STRAIN",
    "CONTEXT_GLOBAL_PAIN_CJ_STRAIN_EXHALE",
    "CONTEXT_GLOBAL_PAIN_CJ_SWIM_GASP",
    "CONTEXT_GLOBAL_PAIN_CJ_DROWNING",
    "CONTEXT_GLOBAL_PAIN_CJ_HIGH_FALL",
    "CONTEXT_GLOBAL_PAIN_END",
    "CONTEXT_GLOBAL_END",
    "",
};

// 0x0049A8A8
const char* g_apszCutsceneTrackStates[] = {
    "CUTSCENE_TRACK_STOPPED",
    "CUTSCENE_TRACK_LOADING",
    "CUTSCENE_TRACK_LOADED",
    "CUTSCENE_TRACK_PLAYING",
    "CUTSCENE_TRACK_STOPPING",
    "",
};

// 0x0049A8C0
const char* g_apszDoorDamageStates[] = {
    "DT_DOOR_INTACT",
    "DT_DOOR_SWINGING_FREE",
    "DT_DOOR_BASHED",
    "DT_DOOR_BASHED_AND_SWINGING_FREE",
    "DT_DOOR_MISSING",
    "",
};

// 0x0049A8D8
const char* g_apszSwimStates[] = {
    "SWIM_IDLE",
    "SWIM_BREAST",
    "SWIM_CRAWL",
    "SWIM_DIVE",
    "SWIM_UNDERWATER",
    "SWIM_JUMP",
    "SWIM_JUST_ENTERED_WATER",
    "",
};

// 0x0049A8F8
const char* g_apEntryExitFlags[] = {
    "ENTRYEXITS_FLAG_RUBBISH",
    (const char*)1,
    "ENTRYEXITS_FLAG_OLDSTYLE",
    (const char*)2,
    "ENTRYEXITS_FLAG_LINKED",
    (const char*)4,
    "ENTRYEXITS_FLAG_SHOP",
    (const char*)8,
    "ENTRYEXITS_FLAG_SHOPEXIT",
    (const char*)16,
    "ENTRYEXITS_FLAG_CAR_WARP",
    (const char*)32,
    "ENTRYEXITS_FLAG_BIKE_WARP",
    (const char*)64,
    "ENTRYEXITS_FLAG_MIRROR",
    (const char*)128,
    "ENTRYEXITS_FLAG_WARP_GROUP",
    (const char*)256,
    "ENTRYEXITS_FLAG_NO_WARP",
    (const char*)512,
    "ENTRYEXITS_FLAG_RANDOM_OPENTIME",
    (const char*)1024,
    "ENTRYEXITS_FLAG_DONT_WARP_PED",
    (const char*)2048,
    "ENTRYEXITS_FLAG_IN_USE",
    (const char*)8192,
    "ENTRYEXITS_FLAG_ENABLED",
    (const char*)16384,
    "ENTRYEXITS_FLAG_TEMPORARY",
    (const char*)32768,
    "",
    (const char*)65536,
};

// 0x0049A978
const char* g_apZoomLevels[] = {
    "ZOOM_ZERO",
    (const char*)nullptr,
    "ZOOM_ONE",
    (const char*)1,
    "ZOOM_TWO",
    (const char*)2,
    "ZOOM_THREE",
    (const char*)3,
    "ZOOM_FIVE",
    (const char*)5,
    "",
    (const char*)6,
};

// 0x0049A9A8
const char* g_apszProcLevels[] = {
    "PROC_LEVEL_OK",
    "PROC_LEVEL_MED",
    "PROC_LEVEL_HIGH",
    "",
};

// 0x0049A9B8
const char* g_apszLanguages[] = {
    "LANGUAGE_ENGLISH",
    "LANGUAGE_FRENCH",
    "LANGUAGE_GERMAN",
    "LANGUAGE_ITALIAN",
    "LANGUAGE_SPANISH",
    "",
};

// 0x0049A9D0
const char* g_apszHandSignalAnims[] = {
    "ANIM_HANDSIGNAL1",
    "ANIM_HANDSIGNAL2",
    "ANIM_HANDSIGNAL3",
    "ANIM_HANDSIGNAL4",
    "ANIM_HANDSIGNAL5",
    "",
};

// 0x01582FE8
std::map<std::string, int> g_mapFlareTypes;
// 0x01582FF0
std::map<std::string, int> g_aLocalVariableMaps[MAX_SCOPES];
// 0x01583AE0
std::map<std::string, int> g_mapPedTypes;
// 0x01583AE8
std::map<std::string, int> g_mapCoronaTypes;
// 0x01583AF0
std::map<std::string, int> g_mapCarLightOverrides;
// 0x01583AF8
std::map<std::string, int> g_mapWidgetIds;
// 0x01583B00
std::map<std::string, int> g_mapCameraSwitchModes;
// 0x01583B08
std::map<std::string, int> g_mapPopcycleZones;
// 0x01583B10
std::map<std::string, int> g_mapZoomLevels;
// 0x01583B18
std::map<std::string, int> g_mapHudFlashItems;
// 0x01583B20
std::map<std::string, int> g_mapPedMoveStates;
// 0x01583B28
std::map<std::string, int> g_mapPads;
// 0x01583B30
std::set<std::string> g_aResourcesUsedByScript[NUM_SCRIPT_RESOURCES];
// 0x01583B68
std::map<std::string, int> g_mapHoldOrientateTypes;
// 0x01583B70
std::map<std::string, int> g_mapAllEnumConstants;
// 0x01583B78
std::map<std::string, int> g_mapCamZooms;
// 0x01583B80
std::map<std::string, int> g_mapCarLockStates;
// 0x01583B88
std::map<std::string, int> g_mapWeaponSkills;
// 0x01583B90
std::map<std::string, int> g_mapRadarSprites;
// 0x01583B98
std::map<std::string, int> g_mapCarPanels;
// 0x01583BA0
std::map<std::string, int> g_mapDefaultModels;
// 0x01583BA8
std::map<std::string, int> g_mapCarMissions;
// 0x01583BB0
std::map<std::string, int> g_mapTimerDirections;
// 0x01583BB8
std::map<std::string, int> g_mapPedEventChoices;
// 0x01583BC0
std::map<std::string, int> g_mapShadowTypes;
// 0x01583BC8
std::map<std::string, int> g_mapRadarModes;
// 0x01583BD0
std::map<std::string, int> g_mapEntityStatus;
// 0x01583BD8
std::map<std::string, int> g_mapSoundEffects;
// 0x01583BE0
std::map<std::string, int> g_mapBlipDisplayTypes;
// 0x01583BE8
std::map<std::string, int> g_mapCutsceneTrackStates;
// 0x01583BF0
std::map<std::string, int> g_mapDriveByTypes;
// 0x01583BF8
std::map<std::string, int> g_mapMeleeCombos;
// 0x01583C00
std::map<std::string, int> g_mapIntConstants;
// 0x01583C08
std::map<std::string, int> g_mapGangs;
// 0x01583C10
std::map<std::string, int> g_mapDecisionMakerPedTypes;
// 0x01583C18
std::map<std::string, int> g_mapExplosionTypes;
// 0x01583C20
std::map<std::string, int> g_mapClothesTextures;
// 0x01583C28
std::map<std::string, int> g_mapObjectModels;
// 0x01583C30
std::map<std::string, int> g_mapFacingDirections;
// 0x01583C38
std::map<std::string, int> g_mapGroupEventChoices;
// 0x01583C40
std::map<std::string, float> g_mapFloatConstants;
// 0x01583C48
std::map<std::string, int> g_mapDecisionMakerMissionPedTypes;
// 0x01583C50
std::map<std::string, int> g_mapPedTaskChoices;
// 0x01583C58
std::map<std::string, int> g_mapAnnouncementTracks;
// 0x01583C60
std::map<std::string, int> g_mapWeaponTypes;
// 0x01583C68
std::map<std::string, int> g_mapCounterDisplayTypes;
// 0x01583C70
std::set<std::string> g_mapAllLocalVariableNames;
// 0x01583C78
std::map<std::string, int> g_mapPadSticks;
// 0x01583C80
std::map<std::string, int> g_mapCheckpointTypes;
// 0x01583C88
std::map<std::string, int> g_mapProcLevels;
// 0x01583C90
std::map<std::string, int> g_mapEntryExitFlags;
// 0x01583C98
std::map<std::string, int> g_mapPlaybackDisplayModes;
// 0x01583CA0
std::map<std::string, int> g_mapCommandNames;
// 0x01583CA8
std::map<std::string, int> g_mapFonts;
// 0x01583CB0
std::map<std::string, int> g_mapHIDMapping;
// 0x01583CB8
std::map<std::string, int> g_mapDecisionMakerGroupTypes;
// 0x01583CC0
std::map<std::string, int> g_mapLanguages;
// 0x01583CC8
std::map<std::string, int> g_mapHandSignalAnims;
// 0x01583CD0
std::map<std::string, int> g_mapInteriorTypes;
// 0x01583CD8
std::map<std::string, int> g_mapConversationStates;
// 0x01583CE0
std::map<std::string, int> g_mapFontOrientations;
// 0x01583CE8
std::map<std::string, int> g_mapRadioStations;
// 0x01583CF0
std::map<std::string, int> g_mapBones;
// 0x01583CF8
std::map<std::string, int> g_mapHudColours;
// 0x01583D00
std::map<std::string, int> g_mapSplineModes;
// 0x01583D08
std::map<std::string, int> g_mapDefaultTaskAllocators;
// 0x01583D10
std::map<std::string, int> g_mapCarColours;
// 0x01583D18
std::map<std::string, int> g_mapGarageTypes;
// 0x01583D20
std::map<std::string, int> g_mapSelectedWeaponTypes;
// 0x01583D28
std::map<std::string, int> g_mapFadeTypes;
// 0x01583D30
std::map<std::string, int> g_mapCarWheels;
// 0x01583D38
std::map<std::string, int> g_mapKeyboardKeys;
// 0x01583D40
std::map<std::string, int> g_mapCarBombTypes;
// 0x01583D48
std::map<std::string, int> g_mapDrivingModes;
// 0x01583D50
std::map<std::string, int> g_mapPedBodyParts;
// 0x01583D58
std::map<std::string, int> g_mapDuckingTypes;
// 0x01583D60
std::map<std::string, int> g_mapSwirlTypes;
// 0x01583D68
std::map<std::string, int> g_mapCameraModes;
// 0x01583D70
std::map<std::string, int> g_mapCarPlateDesigns;
// 0x01583D78
std::map<std::string, int> g_mapFormations;
// 0x01583D80
std::map<std::string, int> g_mapKillFrenzyStates;
// 0x01583D88
std::map<std::string, int> g_mapKnockOffBikeTypes;
// 0x01583D90
std::map<std::string, int> g_mapGlobalSpeechContexts;
// 0x01583D98
std::map<std::string, int> g_mapCoordBlipAppearances;
// 0x01583DA0
std::map<std::string, int> g_mapFollowRouteModes;
// 0x01583DA8
std::map<std::string, int> g_mapDoorDamageStates;
// 0x01583DB0
std::map<std::string, int> g_mapWaitStates;
// 0x01583DB8
std::map<std::string, int> g_mapReplayCamModes;
// 0x01583DC0
std::map<std::string, int> g_mapBuyableTypes;
// 0x01583DC8
std::map<std::string, int> g_mapStats;
// 0x01583DD0
std::map<std::string, int> g_mapStatIncrements;
// 0x01583DD8
std::map<std::string, int> g_mapColours;
// 0x01583DE0
std::map<std::string, int> g_mapCarClasses;
// 0x01583DE8
std::map<std::string, int> g_mapMotionBlurTypes;
// 0x01583DF0
std::map<std::string, int> g_mapWinchTypes;
// 0x01583DF8
std::map<std::string, int> g_mapTempActions;
// 0x01583E00
std::map<std::string, int> g_mapWeatherTypes;
// 0x01583E08
std::map<std::string, int> g_mapPopRaces;
// 0x01583E10
std::map<std::string, int> g_mapPickupTypes;
// 0x01583E18
std::map<std::string, int> g_mapCarDoors;
// 0x01583E20
std::map<std::string, int> g_mapLabels;
// 0x01583E28
std::map<std::string, int> g_mapVehicleUpgrades;
// 0x01583E30
std::map<std::string, int> g_mapLevels;
// 0x01583E38
std::map<std::string, int> g_mapSwimStates;
// 0x01583E40
std::map<std::string, int> g_mapTaskStatus;
// 0x01583E48
std::map<std::string, int> g_mapMoods;
// 0x01583E50
std::map<std::string, int> g_mapAcquaintanceTypes;
// 0x01583E58
std::map<std::string, int> g_mapGlobalVariables;
// 0x01583E60
std::map<std::string, int> g_mapGroupTaskChoices;
// 0x01583E68
std::map<std::string, int> g_mapSetPieces;
// 0x01583E70
std::map<std::string, int> g_mapBirdTypes;

// 0x0040AD30
// ?AddToMap@@YADPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@std@@@2@@std@@PADHD@Z
bool AddToMap(std::map<std::string, int>* pMap, const char* pszName, int nValue, bool bCheckSoundPrefix)
{
    char szMessage[128];
    int nOldValue;

    if (bCheckSoundPrefix && strncmp(pszName, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
    {
        sprintf(szMessage, "**** AddToMap - %s Name begins with SOUND_", pszName);
        PrintMessage(szMessage);
        return false;
    }
    bool bFound = FindInMap(pszName, pMap, &nOldValue, false);
    if (nOldValue != nValue && bFound)
    {
        sprintf(szMessage, "**** AddToMap - %s Name already exists in this list", pszName);
        PrintMessage(szMessage);
        return false;
    }
    pMap->insert(std::map<std::string, int>::value_type(pszName, nValue));
    return true;
}

// 0x0040AE90
// ?AddToFloatMap@@YADPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@MU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@M@std@@@2@@std@@PADMD@Z
bool AddToFloatMap(std::map<std::string, float>* pMap, const char* pszName, float fValue, bool bCheckSoundPrefix)
{
    char szMessage[128];
    float fOldValue;

    if (bCheckSoundPrefix && strncmp(pszName, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
    {
        sprintf(szMessage, "**** AddToMap - %s Name begins with SOUND_", pszName);
        PrintMessage(szMessage);
        return false;
    }
    bool bFound = FindInFloatMap(pszName, pMap, &fOldValue);
    if (fOldValue != fValue && bFound)
    {
        sprintf(szMessage, "**** AddToMap - %s Name already exists in this list", pszName);
        PrintMessage(szMessage);
        return false;
    }
    pMap->insert(std::map<std::string, float>::value_type(pszName, fValue));
    return true;
}

// 0x0040B150
// ?AddToSet@@YAXPAV?$set@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@U?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@PAD@Z
void AddToSet(std::set<std::string>* pSet, const char* pszName)
{
    if (!IsInSet(pszName, pSet))
    {
        pSet->insert(pszName);
    }
}

// 0x0040B330
// ?AddStringTableToMap@@YAXPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@std@@@2@@std@@PAPBDD@Z
void AddStringTableToMap(std::map<std::string, int>* pMap, const char** apszNames, bool bCheckSoundPrefix)
{
    char szMessage[128];

    pMap->clear();
    for (uint16_t i = 0; strcmp(apszNames[i], "") != 0; i++)
    {
        if (bCheckSoundPrefix && strncmp(apszNames[i], SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
        {
            sprintf(szMessage, "**** %s Name begins with SOUND_", apszNames[i]);
            PrintMessage(szMessage);
        }
        int nValue = GetValueFromMap(apszNames[i], pMap, false);
        if (nValue == i || nValue == -1)
        {
            pMap->insert(std::map<std::string, int>::value_type(apszNames[i], i));
            nValue = GetValueFromMap(apszNames[i], &g_mapAllEnumConstants, false);
            if (nValue == i || nValue == -1)
            {
                g_mapAllEnumConstants.insert(std::map<std::string, int>::value_type(apszNames[i], i));
            }
            else
            {
                sprintf(szMessage, "**** %s Name already exists in one of the lists", apszNames[i]);
                PrintMessage(szMessage);
            }
        }
        else
        {
            sprintf(szMessage, "**** %s Name already exists in this list", apszNames[i]);
            PrintMessage(szMessage);
        }
    }
}

// 0x0040B650
// ?AddNamedIdArrayToMap@@YAXPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@std@@@2@@std@@PAUtNamedId@@D@Z
void AddNamedIdArrayToMap(std::map<std::string, int>* pMap, const tNamedId* aNamedIds, bool bCheckSoundPrefix)
{
    char szMessage[128];

    pMap->clear();
    for (uint16_t i = 0; strcmp(aNamedIds[i].szName, "") != 0; i++)
    {
        if (bCheckSoundPrefix && strncmp(aNamedIds[i].szName, SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
        {
            sprintf(szMessage, "**** %s Name begins with SOUND_", aNamedIds[i].szName);
            PrintMessage(szMessage);
        }
        int nValue = GetValueFromMap(aNamedIds[i].szName, pMap, false);
        if (nValue == aNamedIds[i].nId || nValue == -1)
        {
            pMap->insert(std::map<std::string, int>::value_type(aNamedIds[i].szName, aNamedIds[i].nId));
            nValue = GetValueFromMap(aNamedIds[i].szName, &g_mapAllEnumConstants, false);
            if (nValue == aNamedIds[i].nId || nValue == -1)
            {
                g_mapAllEnumConstants.insert(
                    std::map<std::string, int>::value_type(aNamedIds[i].szName, aNamedIds[i].nId));
            }
            else
            {
                sprintf(szMessage, "**** %s Name already exists in one of the lists", aNamedIds[i].szName);
                PrintMessage(szMessage);
            }
        }
        else
        {
            sprintf(szMessage, "**** %s Name already exists in this list", aNamedIds[i].szName);
            PrintMessage(szMessage);
        }
    }
}

// 0x0040B970
// ?AddStringValuePairsToMap@@YAXPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@std@@@2@@std@@PAPBDD@Z
void AddStringValuePairsToMap(std::map<std::string, int>* pMap, const char** apPairs, bool bCheckSoundPrefix)
{
    char szMessage[128];

    pMap->clear();
    for (uint16_t i = 0; strcmp(apPairs[i * NAME_VALUE_PAIR_SIZE], "") != 0; i++)
    {
        if (bCheckSoundPrefix && strncmp(apPairs[i * NAME_VALUE_PAIR_SIZE], SOUND_PREFIX, SOUND_PREFIX_LENGTH) == 0)
        {
            sprintf(szMessage, "**** %s Name begins with SOUND_", apPairs[i * NAME_VALUE_PAIR_SIZE]);
            PrintMessage(szMessage);
        }
        int nValue = GetValueFromMap(apPairs[i * NAME_VALUE_PAIR_SIZE], pMap, false);
        if (nValue == *reinterpret_cast<int*>(&apPairs[i * NAME_VALUE_PAIR_SIZE + 1]) || nValue == -1)
        {
            pMap->insert(std::map<std::string, int>::value_type(
                apPairs[i * NAME_VALUE_PAIR_SIZE], *reinterpret_cast<int*>(&apPairs[i * NAME_VALUE_PAIR_SIZE + 1])));
            nValue = GetValueFromMap(apPairs[i * NAME_VALUE_PAIR_SIZE], &g_mapAllEnumConstants, false);
            if (nValue == *reinterpret_cast<int*>(&apPairs[i * NAME_VALUE_PAIR_SIZE + 1]) || nValue == -1)
            {
                g_mapAllEnumConstants.insert(std::map<std::string, int>::value_type(apPairs[i * NAME_VALUE_PAIR_SIZE],
                    *reinterpret_cast<int*>(&apPairs[i * NAME_VALUE_PAIR_SIZE + 1])));
            }
            else
            {
                sprintf(
                    szMessage, "**** %s Name already exists in one of the lists", apPairs[i * NAME_VALUE_PAIR_SIZE]);
                PrintMessage(szMessage);
            }
        }
        else
        {
            sprintf(szMessage, "**** %s Name already exists in this list", apPairs[i * NAME_VALUE_PAIR_SIZE]);
            PrintMessage(szMessage);
        }
    }
}

// 0x0040BC90
// ?GetValueFromMap@@YAHPADPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@std@@@2@@std@@D@Z
int GetValueFromMap(const char* pszName, const std::map<std::string, int>* pMap, bool bRecordUsage)
{
    if (strcmp(pszName, "ONE") == 0)
    {
        printf(";lk'k'#");
    }

    std::map<std::string, int>::const_iterator it = pMap->find(pszName);
    int nValue;
    if (it == pMap->end())
    {
        nValue = -1;
    }
    else
    {
        nValue = it->second;
        if (bRecordUsage)
        {
            if (GetValueFromMap(pszName, &g_mapDefaultModels, false) != -1 ||
                GetValueFromMap(pszName, &g_mapObjectModels, false) != -1)
            {
                AddToSet(&g_aResourcesUsedByScript[RESOURCE_MODELS], pszName);
            }
            else if (GetValueFromMap(pszName, &g_mapGlobalSpeechContexts, false) != -1)
            {
                AddToSet(&g_aResourcesUsedByScript[RESOURCE_SPEECH_CONTEXTS], pszName);
            }
        }
    }
    return nValue;
}

// 0x0040BE30
// ?FindInMap@@YADPADPAV?$map@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HU?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@U?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@std@@@2@@std@@PAHD@Z
bool FindInMap(const char* pszName, std::map<std::string, int>* pMap, int* pnValue, bool bRecordUsage)
{
    if (strcmp(pszName, "ONE") == 0)
    {
        printf(";lk'k'#");
    }

    std::map<std::string, int>::iterator it = pMap->find(pszName);
    if (it == pMap->end())
    {
        return false;
    }

    if (bRecordUsage)
    {
        if (GetValueFromMap(pszName, &g_mapDefaultModels, false) != -1 ||
            GetValueFromMap(pszName, &g_mapObjectModels, false) != -1)
        {
            AddToSet(&g_aResourcesUsedByScript[RESOURCE_MODELS], pszName);
        }
        else if (GetValueFromMap(pszName, &g_mapGlobalSpeechContexts, false) != -1)
        {
            AddToSet(&g_aResourcesUsedByScript[RESOURCE_SPEECH_CONTEXTS], pszName);
        }
    }
    *pnValue = it->second;
    return true;
}

// 0x0040BFE0
bool FindInFloatMap(const char* pszName, std::map<std::string, float>* pMap, float* pfValue)
{
    std::map<std::string, float>::iterator it = pMap->find(pszName);
    if (it == pMap->end())
    {
        return false;
    }
    *pfValue = it->second;
    return true;
}

// 0x0040C120 ?BuildConstantMaps@@YAXXZ
void BuildConstantMaps()
{
    char szMessage[128];

    g_mapCommandNames.clear();
    for (uint32_t i = 0; i < ARRAY_SIZE(g_aCommandInfo); i++)
    {
        if (g_aCommandInfo[i].bHandledByGame)
        {
            if (g_mapCommandNames.find(g_aCommandInfo[i].szName) != g_mapCommandNames.end())
            {
                sprintf(szMessage, "**** Command name already exists in the map : %s", g_aCommandInfo[i].szName);
                PrintMessage(szMessage);
                return;
            }
            g_mapCommandNames.insert(std::map<std::string, int>::value_type(g_aCommandInfo[i].szName, i));
        }
    }

    g_mapAllEnumConstants.clear();
    AddNamedIdArrayToMap(&g_mapDefaultModels, g_aDefaultModels, true);
    AddNamedIdArrayToMap(&g_mapPedEventChoices, g_aPedEventChoices, true);
    AddNamedIdArrayToMap(&g_mapPedTaskChoices, g_aPedTaskChoices, true);
    AddNamedIdArrayToMap(&g_mapGroupEventChoices, g_aGroupEventChoices, true);
    AddNamedIdArrayToMap(&g_mapGroupTaskChoices, g_aGroupTaskChoices, true);
    AddStringTableToMap(&g_mapCameraModes, g_apszCameraModes, true);
    AddStringTableToMap(&g_mapPads, g_apszPads, true);
    AddStringTableToMap(&g_mapPadSticks, g_apszPadSticks, true);
    AddStringTableToMap(&g_mapPedTypes, g_apszPedTypes, true);
    AddStringTableToMap(&g_mapColours, g_apszColours, true);
    AddStringTableToMap(&g_mapBlipDisplayTypes, g_apszBlipDisplayTypes, true);
    AddStringTableToMap(&g_mapCameraSwitchModes, g_apszCameraSwitchModes, true);
    AddStringTableToMap(&g_mapFadeTypes, g_apszFadeTypes, true);
    AddStringTableToMap(&g_mapShadowTypes, g_apszShadowTypes, true);
    AddStringTableToMap(&g_mapWeaponTypes, g_apszWeaponTypes, true);
    AddNamedIdArrayToMap(&g_mapSoundEffects, g_aSoundNames, false);
    AddStringTableToMap(&g_mapWeatherTypes, g_apszWeatherTypes, true);
    AddStringTableToMap(&g_mapKillFrenzyStates, g_apszKillFrenzyStates, true);
    AddStringTableToMap(&g_mapCarLockStates, g_apszCarLockStates, true);
    AddStringTableToMap(&g_mapExplosionTypes, g_apszExplosionTypes, true);
    AddStringTableToMap(&g_mapPickupTypes, g_apszPickupTypes, true);
    AddStringTableToMap(&g_mapGarageTypes, g_apszGarageTypes, true);
    AddStringTableToMap(&g_mapCarBombTypes, g_apszCarBombTypes, true);
    AddStringTableToMap(&g_mapCarColours, g_apszCarColours, true);
    AddNamedIdArrayToMap(&g_mapObjectModels, g_aObjectModels, true);
    AddStringTableToMap(&g_mapGangs, g_apszGangs, true);
    AddStringTableToMap(&g_mapCoronaTypes, g_apszCoronaTypes, true);
    AddStringTableToMap(&g_mapFlareTypes, g_apszFlareTypes, true);
    AddStringTableToMap(&g_mapRadarSprites, g_apszRadarSprites, true);
    AddStringTableToMap(&g_mapDrivingModes, g_apszDrivingModes, true);
    AddStringTableToMap(&g_mapCarMissions, g_apszCarMissions, true);
    AddStringTableToMap(&g_mapFormations, g_apszFormations, true);
    AddStringTableToMap(&g_mapWaitStates, g_apszWaitStates, true);
    AddStringTableToMap(&g_mapCamZooms, g_apszCamZooms, true);
    AddStringTableToMap(&g_mapFonts, g_apszFonts, true);
    AddStringTableToMap(&g_mapEntityStatus, g_apszEntityStatus, true);
    AddStringTableToMap(&g_mapLevels, g_apszLevels, true);
    AddStringTableToMap(&g_mapCounterDisplayTypes, g_apszCounterDisplayTypes, true);
    AddStringTableToMap(&g_mapMotionBlurTypes, g_apszMotionBlurTypes, true);
    AddStringValuePairsToMap(&g_mapHudFlashItems, g_apHudFlashItems, true);
    AddStringTableToMap(&g_mapCarDoors, g_apszCarDoors, true);
    AddStringTableToMap(&g_mapRadioStations, g_apszRadioStations, true);
    AddStringTableToMap(&g_mapFacingDirections, g_apszFacingDirections, true);
    AddStringTableToMap(&g_mapReplayCamModes, g_apszReplayCamModes, true);
    AddStringTableToMap(&g_mapCarWheels, g_apszCarWheels, true);
    AddStringTableToMap(&g_mapTempActions, g_apszTempActions, true);
    AddStringTableToMap(&g_mapMoods, g_apszMoods, true);
    AddStringTableToMap(&g_mapSetPieces, g_apszSetPieces, true);
    AddStringTableToMap(&g_mapTimerDirections, g_apszTimerDirections, true);
    AddStringValuePairsToMap(&g_mapBones, g_apBones, true);
    AddStringTableToMap(&g_mapAnnouncementTracks, g_apszAnnouncementTracks, true);
    AddStringTableToMap(&g_mapPedMoveStates, g_apszPedMoveStates, true);
    AddStringValuePairsToMap(&g_mapStats, g_apStats, true);
    AddStringTableToMap(&g_mapTaskStatus, g_apszTaskStatus, true);
    AddStringValuePairsToMap(&g_mapDuckingTypes, g_apDuckingTypes, true);
    AddStringTableToMap(&g_mapFollowRouteModes, g_apszFollowRouteModes, true);
    AddStringTableToMap(&g_mapSplineModes, g_apszSplineModes, true);
    AddStringTableToMap(&g_mapCarLightOverrides, g_apszCarLightOverrides, true);
    AddStringTableToMap(&g_mapCarPanels, g_apszCarPanels, true);
    AddStringTableToMap(&g_mapDefaultTaskAllocators, g_apszDefaultTaskAllocators, true);
    AddStringTableToMap(&g_mapCheckpointTypes, g_apszCheckpointTypes, true);
    AddStringTableToMap(&g_mapVehicleUpgrades, g_apszVehicleUpgrades, true);
    AddStringTableToMap(&g_mapDriveByTypes, g_apszDriveByTypes, true);
    AddStringValuePairsToMap(&g_mapHoldOrientateTypes, g_apHoldOrientateTypes, true);
    AddStringTableToMap(&g_mapPedBodyParts, g_apszPedBodyParts, true);

    if (Config_TargetMode == SAN_ANDREAS)
    {
        AddStringTableToMap(&g_mapKeyboardKeys, g_apszKeyboardKeysPS2, true);
    }
    else if (Config_TargetMode == SAN_ANDREAS_PC || Config_TargetMode == SAN_ANDREAS_XBOX ||
             Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        AddStringTableToMap(&g_mapKeyboardKeys, g_apszKeyboardKeysPC, true);
    }
    else
    {
        sprintf(szMessage, "**** Don't know which keyboard_keys list to use for this target");
        PrintMessage(szMessage);
        return;
    }

    AddStringTableToMap(&g_mapRadarModes, g_apszRadarModes, true);
    AddStringTableToMap(&g_mapAcquaintanceTypes, g_apszAcquaintanceTypes, true);
    AddStringTableToMap(&g_mapStatIncrements, g_apszStatIncrements, true);
    AddStringTableToMap(&g_mapPopcycleZones, g_apszPopcycleZones, true);
    AddStringTableToMap(&g_mapCarPlateDesigns, g_apszCarPlateDesigns, true);
    AddStringTableToMap(&g_mapWinchTypes, g_apszWinchTypes, true);
    AddStringTableToMap(&g_mapPlaybackDisplayModes, g_apszPlaybackDisplayModes, true);
    AddStringValuePairsToMap(&g_mapDecisionMakerPedTypes, g_apDecisionMakerPedTypes, true);
    AddStringTableToMap(&g_mapDecisionMakerMissionPedTypes, g_apszDecisionMakerMissionPedTypes, true);
    AddStringTableToMap(&g_mapDecisionMakerGroupTypes, g_apszDecisionMakerGroupTypes, true);
    AddStringValuePairsToMap(&g_mapMeleeCombos, g_apMeleeCombos, true);
    AddStringTableToMap(&g_mapWeaponSkills, g_apszWeaponSkills, true);
    AddStringValuePairsToMap(&g_mapBuyableTypes, g_apBuyableTypes, true);
    AddStringTableToMap(&g_mapSelectedWeaponTypes, g_apszSelectedWeaponTypes, true);
    AddStringTableToMap(&g_mapInteriorTypes, g_apszInteriorTypes, true);
    AddStringTableToMap(&g_mapPopRaces, g_apszPopRaces, true);
    AddStringTableToMap(&g_mapBirdTypes, g_apszBirdTypes, true);
    AddStringTableToMap(&g_mapConversationStates, g_apszConversationStates, true);
    AddStringTableToMap(&g_mapFontOrientations, g_apszFontOrientations, true);
    AddStringTableToMap(&g_mapCarClasses, g_apszCarClasses, true);
    AddStringTableToMap(&g_mapKnockOffBikeTypes, g_apszKnockOffBikeTypes, true);
    AddStringTableToMap(&g_mapClothesTextures, g_apszClothesTextures, true);
    AddStringTableToMap(&g_mapCoordBlipAppearances, g_apszCoordBlipAppearances, true);
    AddStringTableToMap(&g_mapHudColours, g_apszHudColours, true);

    if (Config_TargetMode == SAN_ANDREAS || Config_TargetMode == SAN_ANDREAS_XBOX)
    {
        AddStringTableToMap(&g_mapGlobalSpeechContexts, g_apszGlobalSpeechContexts, true);
    }
    else if (Config_TargetMode == SAN_ANDREAS_PC || Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        AddStringTableToMap(&g_mapGlobalSpeechContexts, g_apszGlobalSpeechContextsPC, true);
    }
    else
    {
        sprintf(szMessage, "**** Don't know which global_speech_context list to use for this target");
        PrintMessage(szMessage);
        return;
    }

    AddStringTableToMap(&g_mapCutsceneTrackStates, g_apszCutsceneTrackStates, true);
    AddStringTableToMap(&g_mapDoorDamageStates, g_apszDoorDamageStates, true);
    AddStringTableToMap(&g_mapSwirlTypes, g_apszSwirlTypes, true);
    AddStringTableToMap(&g_mapSwimStates, g_apszSwimStates, true);
    AddStringValuePairsToMap(&g_mapEntryExitFlags, g_apEntryExitFlags, true);
    AddStringValuePairsToMap(&g_mapZoomLevels, g_apZoomLevels, true);
    AddStringTableToMap(&g_mapProcLevels, g_apszProcLevels, true);
    AddStringTableToMap(&g_mapLanguages, g_apszLanguages, true);
    AddStringTableToMap(&g_mapHandSignalAnims, g_apszHandSignalAnims, true);
    AddStringTableToMap(&g_mapWidgetIds, (const char**)g_apszWidgetIds, true);
    AddStringTableToMap(&g_mapHIDMapping, (const char**)g_apszHIDMappings, true);
}

// 0x0040CB50
// ?IsInSet@@YA_NPADPAV?$set@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@U?$less@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z
bool IsInSet(const char* pszName, std::set<std::string>* pSet)
{
    std::set<std::string>::iterator it = pSet->find(pszName);
    return it != pSet->end();
}

// 0x0040CC80 ?WriteResourcesUsedByScriptFiles@@YAXXZ
void WriteResourcesUsedByScriptFiles()
{
    char szMessage[256];
    char aszFileNames[NUM_SCRIPT_RESOURCES][NAME_SIZE];

    strcpy(aszFileNames[RESOURCE_CUTSCENES], "cutscenes_used_by_script.txt");
    strcpy(aszFileNames[RESOURCE_ANIMS], "anims_used_by_script.txt");
    strcpy(aszFileNames[RESOURCE_ANIM_DICTIONARIES], "anim_dictionaries_used_by_script.txt");
    strcpy(aszFileNames[RESOURCE_SPEECH_CONTEXTS], "speech_contexts_used_by_script.txt");
    strcpy(aszFileNames[RESOURCE_MODELS], "models_used_by_script.txt");
    strcpy(aszFileNames[RESOURCE_SPECIAL_CHARACTERS], "special_characters_used_by_script.txt");
    strcpy(aszFileNames[RESOURCE_PARTICLES], "particles_used_by_script.txt");

    for (int i = 0; i < ARRAY_SIZE(aszFileNames); i++)
    {
        FILE* pFile = fopen(aszFileNames[i], "w");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open  %s  - check it is writable", aszFileNames[i]);
            return;
        }
        for (std::set<std::string>::iterator it = g_aResourcesUsedByScript[i].begin(),
                                             itEnd = g_aResourcesUsedByScript[i].end();
            it != itEnd; it++)
        {
            fprintf(pFile, "%s\n", it->c_str());
        }
        fprintf(pFile, "\n");
        fclose(pFile);
    }
}

// 0x0040CEE0 ?ClearAllNameMaps@@YAXXZ
void ClearAllNameMaps()
{
    g_mapGlobalVariables.clear();
    for (uint32_t i = 0; i < ARRAY_SIZE(g_aLocalVariableMaps); i++)
    {
        g_aLocalVariableMaps[i].clear();
    }
    g_mapAllLocalVariableNames.clear();
    for (int i = 0; i < ARRAY_SIZE(g_aResourcesUsedByScript); i++)
    {
        g_aResourcesUsedByScript[i].clear();
    }
    g_mapLabels.clear();
    g_mapIntConstants.clear();
    g_mapFloatConstants.clear();
}
