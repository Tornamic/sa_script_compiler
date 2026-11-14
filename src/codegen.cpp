// codegen.cpp: 0x407440-0x4095C6 in sc.exe

#include "common.h"
#include "main.h"
#include "compiler.h"
#include "loader.h"
#include "codegen.h"
#include "namemaps.h"
#include "parser.h"
#include "missions.h"
#include "streamedscripts.h"
#include "compiledlines.h"
#include "errors.h"
#include "locals.h"
#include "entitytypes.h"
#include "variableusage.h"

// 0x0045A000
tCommandInfo g_aCommandInfo[NUM_COMMANDS] = {
    {true, "NOP", {}, {}},                                                                         // 0x000
    {true, "WAIT", {ARG_INT}, {}},                                                                 // 0x001
    {true, "GOTO", {ARG_LABEL}, {}},                                                               // 0x002
    {true, "SHAKE_CAM", {ARG_INT}, {}},                                                            // 0x003
    {true, "SET_VAR_INT", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                       // 0x004
    {true, "SET_VAR_FLOAT", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                                 // 0x005
    {true, "SET_LVAR_INT", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                                     // 0x006
    {true, "SET_LVAR_FLOAT", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                               // 0x007
    {true, "ADD_VAL_TO_INT_VAR", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                // 0x008
    {true, "ADD_VAL_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                          // 0x009
    {true, "ADD_VAL_TO_INT_LVAR", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                              // 0x00A
    {true, "ADD_VAL_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                        // 0x00B
    {true, "SUB_VAL_FROM_INT_VAR", {ARG_VAR_INT, ARG_INT_CONST}, {}},                              // 0x00C
    {true, "SUB_VAL_FROM_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                        // 0x00D
    {true, "SUB_VAL_FROM_INT_LVAR", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                            // 0x00E
    {true, "SUB_VAL_FROM_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                      // 0x00F
    {true, "MULT_INT_VAR_BY_VAL", {ARG_VAR_INT, ARG_INT_CONST}, {}},                               // 0x010
    {true, "MULT_FLOAT_VAR_BY_VAL", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                         // 0x011
    {true, "MULT_INT_LVAR_BY_VAL", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                             // 0x012
    {true, "MULT_FLOAT_LVAR_BY_VAL", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                       // 0x013
    {true, "DIV_INT_VAR_BY_VAL", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                // 0x014
    {true, "DIV_FLOAT_VAR_BY_VAL", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                          // 0x015
    {true, "DIV_INT_LVAR_BY_VAL", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                              // 0x016
    {true, "DIV_FLOAT_LVAR_BY_VAL", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                        // 0x017
    {true, "IS_INT_VAR_GREATER_THAN_NUMBER", {ARG_VAR_INT, ARG_INT_CONST}, {}},                    // 0x018
    {true, "IS_INT_LVAR_GREATER_THAN_NUMBER", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                  // 0x019
    {true, "IS_NUMBER_GREATER_THAN_INT_VAR", {ARG_INT_CONST, ARG_VAR_INT}, {}},                    // 0x01A
    {true, "IS_NUMBER_GREATER_THAN_INT_LVAR", {ARG_INT_CONST, ARG_LVAR_INT}, {}},                  // 0x01B
    {true, "IS_INT_VAR_GREATER_THAN_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                     // 0x01C
    {true, "IS_INT_LVAR_GREATER_THAN_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                 // 0x01D
    {true, "IS_INT_VAR_GREATER_THAN_INT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                   // 0x01E
    {true, "IS_INT_LVAR_GREATER_THAN_INT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                   // 0x01F
    {true, "IS_FLOAT_VAR_GREATER_THAN_NUMBER", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},              // 0x020
    {true, "IS_FLOAT_LVAR_GREATER_THAN_NUMBER", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},            // 0x021
    {true, "IS_NUMBER_GREATER_THAN_FLOAT_VAR", {ARG_FLOAT_CONST, ARG_VAR_FLOAT}, {}},              // 0x022
    {true, "IS_NUMBER_GREATER_THAN_FLOAT_LVAR", {ARG_FLOAT_CONST, ARG_LVAR_FLOAT}, {}},            // 0x023
    {true, "IS_FLOAT_VAR_GREATER_THAN_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},             // 0x024
    {true, "IS_FLOAT_LVAR_GREATER_THAN_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},         // 0x025
    {true, "IS_FLOAT_VAR_GREATER_THAN_FLOAT_LVAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},           // 0x026
    {true, "IS_FLOAT_LVAR_GREATER_THAN_FLOAT_VAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},           // 0x027
    {true, "IS_INT_VAR_GREATER_OR_EQUAL_TO_NUMBER", {ARG_VAR_INT, ARG_INT_CONST}, {}},             // 0x028
    {true, "IS_INT_LVAR_GREATER_OR_EQUAL_TO_NUMBER", {ARG_LVAR_INT, ARG_INT_CONST}, {}},           // 0x029
    {true, "IS_NUMBER_GREATER_OR_EQUAL_TO_INT_VAR", {ARG_INT_CONST, ARG_VAR_INT}, {}},             // 0x02A
    {true, "IS_NUMBER_GREATER_OR_EQUAL_TO_INT_LVAR", {ARG_INT_CONST, ARG_LVAR_INT}, {}},           // 0x02B
    {true, "IS_INT_VAR_GREATER_OR_EQUAL_TO_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},              // 0x02C
    {true, "IS_INT_LVAR_GREATER_OR_EQUAL_TO_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},          // 0x02D
    {true, "IS_INT_VAR_GREATER_OR_EQUAL_TO_INT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},            // 0x02E
    {true, "IS_INT_LVAR_GREATER_OR_EQUAL_TO_INT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},            // 0x02F
    {true, "IS_FLOAT_VAR_GREATER_OR_EQUAL_TO_NUMBER", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},       // 0x030
    {true, "IS_FLOAT_LVAR_GREATER_OR_EQUAL_TO_NUMBER", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},     // 0x031
    {true, "IS_NUMBER_GREATER_OR_EQUAL_TO_FLOAT_VAR", {ARG_FLOAT_CONST, ARG_VAR_FLOAT}, {}},       // 0x032
    {true, "IS_NUMBER_GREATER_OR_EQUAL_TO_FLOAT_LVAR", {ARG_FLOAT_CONST, ARG_LVAR_FLOAT}, {}},     // 0x033
    {true, "IS_FLOAT_VAR_GREATER_OR_EQUAL_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},      // 0x034
    {true, "IS_FLOAT_LVAR_GREATER_OR_EQUAL_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},  // 0x035
    {true, "IS_FLOAT_VAR_GREATER_OR_EQUAL_TO_FLOAT_LVAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},    // 0x036
    {true, "IS_FLOAT_LVAR_GREATER_OR_EQUAL_TO_FLOAT_VAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},    // 0x037
    {true, "IS_INT_VAR_EQUAL_TO_NUMBER", {ARG_VAR_INT, ARG_INT_CONST}, {}},                        // 0x038
    {true, "IS_INT_LVAR_EQUAL_TO_NUMBER", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                      // 0x039
    {true, "IS_INT_VAR_EQUAL_TO_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                         // 0x03A
    {true, "IS_INT_LVAR_EQUAL_TO_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                     // 0x03B
    {true, "IS_INT_VAR_EQUAL_TO_INT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                       // 0x03C
    {false, "IS_INT_VAR_NOT_EQUAL_TO_NUMBER", {ARG_VAR_INT, ARG_INT_CONST}, {}},                   // 0x03D
    {false, "IS_INT_LVAR_NOT_EQUAL_TO_NUMBER", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                 // 0x03E
    {false, "IS_INT_VAR_NOT_EQUAL_TO_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                    // 0x03F
    {false, "IS_INT_LVAR_NOT_EQUAL_TO_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                // 0x040
    {false, "IS_INT_VAR_NOT_EQUAL_TO_INT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                  // 0x041
    {true, "IS_FLOAT_VAR_EQUAL_TO_NUMBER", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                  // 0x042
    {true, "IS_FLOAT_LVAR_EQUAL_TO_NUMBER", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                // 0x043
    {true, "IS_FLOAT_VAR_EQUAL_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                 // 0x044
    {true, "IS_FLOAT_LVAR_EQUAL_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},             // 0x045
    {true, "IS_FLOAT_VAR_EQUAL_TO_FLOAT_LVAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},               // 0x046
    {false, "IS_FLOAT_VAR_NOT_EQUAL_TO_NUMBER", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},             // 0x047
    {false, "IS_FLOAT_LVAR_NOT_EQUAL_TO_NUMBER", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},           // 0x048
    {false, "IS_FLOAT_VAR_NOT_EQUAL_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},            // 0x049
    {false, "IS_FLOAT_LVAR_NOT_EQUAL_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},        // 0x04A
    {false, "IS_FLOAT_VAR_NOT_EQUAL_TO_FLOAT_LVAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},          // 0x04B
    {false, "GOTO_IF_TRUE", {ARG_LABEL}, {}},                                                      // 0x04C
    {true, "GOTO_IF_FALSE", {ARG_LABEL}, {}},                                                      // 0x04D
    {true, "TERMINATE_THIS_SCRIPT", {}, {}},                                                       // 0x04E
    {true, "START_NEW_SCRIPT",
        {ARG_LABEL, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM,
            ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM,
            ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM},
        {}},                                                                                                   // 0x04F
    {true, "GOSUB", {ARG_LABEL}, {}},                                                                          // 0x050
    {true, "RETURN", {}, {}},                                                                                  // 0x051
    {true, "LINE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                    // 0x052
    {true, "CREATE_PLAYER", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_PLAYER}},                // 0x053
    {false, "GET_PLAYER_COORDINATES", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_PLAYER}},                                                                                        // 0x054
    {false, "SET_PLAYER_COORDINATES", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_PLAYER}},              // 0x055
    {false, "IS_PLAYER_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x056
    {false, "IS_PLAYER_IN_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x057
    {true, "ADD_INT_VAR_TO_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                          // 0x058
    {true, "ADD_FLOAT_VAR_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                                  // 0x059
    {true, "ADD_INT_LVAR_TO_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                      // 0x05A
    {true, "ADD_FLOAT_LVAR_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                              // 0x05B
    {true, "ADD_INT_VAR_TO_INT_LVAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                        // 0x05C
    {true, "ADD_FLOAT_VAR_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                                // 0x05D
    {true, "ADD_INT_LVAR_TO_INT_VAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                        // 0x05E
    {true, "ADD_FLOAT_LVAR_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                                // 0x05F
    {true, "SUB_INT_VAR_FROM_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                        // 0x060
    {true, "SUB_FLOAT_VAR_FROM_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                                // 0x061
    {true, "SUB_INT_LVAR_FROM_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                    // 0x062
    {true, "SUB_FLOAT_LVAR_FROM_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                            // 0x063
    {true, "SUB_INT_VAR_FROM_INT_LVAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                      // 0x064
    {true, "SUB_FLOAT_VAR_FROM_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                              // 0x065
    {true, "SUB_INT_LVAR_FROM_INT_VAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                      // 0x066
    {true, "SUB_FLOAT_LVAR_FROM_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                              // 0x067
    {true, "MULT_INT_VAR_BY_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                         // 0x068
    {true, "MULT_FLOAT_VAR_BY_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                                 // 0x069
    {true, "MULT_INT_LVAR_BY_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                     // 0x06A
    {true, "MULT_FLOAT_LVAR_BY_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                             // 0x06B
    {true, "MULT_INT_VAR_BY_INT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                       // 0x06C
    {true, "MULT_FLOAT_VAR_BY_FLOAT_LVAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                               // 0x06D
    {true, "MULT_INT_LVAR_BY_INT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                       // 0x06E
    {true, "MULT_FLOAT_LVAR_BY_FLOAT_VAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                               // 0x06F
    {true, "DIV_INT_VAR_BY_INT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                          // 0x070
    {true, "DIV_FLOAT_VAR_BY_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                                  // 0x071
    {true, "DIV_INT_LVAR_BY_INT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                      // 0x072
    {true, "DIV_FLOAT_LVAR_BY_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                              // 0x073
    {true, "DIV_INT_VAR_BY_INT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                        // 0x074
    {true, "DIV_FLOAT_VAR_BY_FLOAT_LVAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                                // 0x075
    {true, "DIV_INT_LVAR_BY_INT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                        // 0x076
    {true, "DIV_FLOAT_LVAR_BY_FLOAT_VAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                                // 0x077
    {true, "ADD_TIMED_VAL_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                                // 0x078
    {true, "ADD_TIMED_VAL_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                              // 0x079
    {true, "ADD_TIMED_FLOAT_VAR_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                            // 0x07A
    {true, "ADD_TIMED_FLOAT_LVAR_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                        // 0x07B
    {true, "ADD_TIMED_FLOAT_VAR_TO_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                          // 0x07C
    {true, "ADD_TIMED_FLOAT_LVAR_TO_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                          // 0x07D
    {true, "SUB_TIMED_VAL_FROM_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_FLOAT_CONST}, {}},                              // 0x07E
    {true, "SUB_TIMED_VAL_FROM_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_FLOAT_CONST}, {}},                            // 0x07F
    {true, "SUB_TIMED_FLOAT_VAR_FROM_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                          // 0x080
    {true, "SUB_TIMED_FLOAT_LVAR_FROM_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                      // 0x081
    {true, "SUB_TIMED_FLOAT_VAR_FROM_FLOAT_LVAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                        // 0x082
    {true, "SUB_TIMED_FLOAT_LVAR_FROM_FLOAT_VAR", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                        // 0x083
    {true, "SET_VAR_INT_TO_VAR_INT", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                          // 0x084
    {true, "SET_LVAR_INT_TO_LVAR_INT", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                      // 0x085
    {true, "SET_VAR_FLOAT_TO_VAR_FLOAT", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                                  // 0x086
    {true, "SET_LVAR_FLOAT_TO_LVAR_FLOAT", {ARG_LVAR_FLOAT, ARG_LVAR_FLOAT}, {}},                              // 0x087
    {true, "SET_VAR_FLOAT_TO_LVAR_FLOAT", {ARG_VAR_FLOAT, ARG_LVAR_FLOAT}, {}},                                // 0x088
    {true, "SET_LVAR_FLOAT_TO_VAR_FLOAT", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                                // 0x089
    {true, "SET_VAR_INT_TO_LVAR_INT", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                        // 0x08A
    {true, "SET_LVAR_INT_TO_VAR_INT", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                        // 0x08B
    {true, "CSET_VAR_INT_TO_VAR_FLOAT", {ARG_VAR_INT, ARG_VAR_FLOAT}, {}},                                     // 0x08C
    {true, "CSET_VAR_FLOAT_TO_VAR_INT", {ARG_VAR_FLOAT, ARG_VAR_INT}, {}},                                     // 0x08D
    {true, "CSET_LVAR_INT_TO_VAR_FLOAT", {ARG_LVAR_INT, ARG_VAR_FLOAT}, {}},                                   // 0x08E
    {true, "CSET_LVAR_FLOAT_TO_VAR_INT", {ARG_LVAR_FLOAT, ARG_VAR_INT}, {}},                                   // 0x08F
    {true, "CSET_VAR_INT_TO_LVAR_FLOAT", {ARG_VAR_INT, ARG_LVAR_FLOAT}, {}},                                   // 0x090
    {true, "CSET_VAR_FLOAT_TO_LVAR_INT", {ARG_VAR_FLOAT, ARG_LVAR_INT}, {}},                                   // 0x091
    {true, "CSET_LVAR_INT_TO_LVAR_FLOAT", {ARG_LVAR_INT, ARG_LVAR_FLOAT}, {}},                                 // 0x092
    {true, "CSET_LVAR_FLOAT_TO_LVAR_INT", {ARG_LVAR_FLOAT, ARG_LVAR_INT}, {}},                                 // 0x093
    {true, "ABS_VAR_INT", {ARG_VAR_INT}, {}},                                                                  // 0x094
    {true, "ABS_LVAR_INT", {ARG_LVAR_INT}, {}},                                                                // 0x095
    {true, "ABS_VAR_FLOAT", {ARG_VAR_FLOAT}, {}},                                                              // 0x096
    {true, "ABS_LVAR_FLOAT", {ARG_LVAR_FLOAT}, {}},                                                            // 0x097
    {true, "GENERATE_RANDOM_FLOAT", {ARG_VAR_FLOAT}, {}},                                                      // 0x098
    {true, "GENERATE_RANDOM_INT", {ARG_VAR_INT}, {}},                                                          // 0x099
    {true, "CREATE_CHAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                                        // 0x09A
    {true, "DELETE_CHAR", {ARG_INT}, {TYPE_CHAR}},                                                              // 0x09B
    {false, "CHAR_WANDER_DIR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                // 0x09C
    {false, "CHAR_WANDER_RANGE", {ARG_INT}, {TYPE_CHAR}},                                                       // 0x09D
    {false, "CHAR_FOLLOW_PATH", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},   // 0x09E
    {false, "CHAR_SET_IDLE", {ARG_INT}, {TYPE_CHAR}},                                                           // 0x09F
    {true, "GET_CHAR_COORDINATES", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CHAR}},                                                                                           // 0x0A0
    {true, "SET_CHAR_COORDINATES", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},                    // 0x0A1
    {false, "IS_CHAR_STILL_ALIVE", {ARG_INT}, {TYPE_CHAR}},                                                     // 0x0A2
    {true, "IS_CHAR_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},  // 0x0A3
    {true, "IS_CHAR_IN_AREA_3D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                           // 0x0A4
    {true, "CREATE_CAR", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},                    // 0x0A5
    {true, "DELETE_CAR", {ARG_INT}, {TYPE_CAR}},                                                                // 0x0A6
    {true, "CAR_GOTO_COORDINATES", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},                     // 0x0A7
    {true, "CAR_WANDER_RANDOMLY", {ARG_INT}, {TYPE_CAR}},                                                       // 0x0A8
    {true, "CAR_SET_IDLE", {ARG_INT}, {TYPE_CAR}},                                                              // 0x0A9
    {true, "GET_CAR_COORDINATES", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CAR}},                                                                                            // 0x0AA
    {true, "SET_CAR_COORDINATES", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},                      // 0x0AB
    {false, "IS_CAR_STILL_ALIVE", {ARG_INT}, {TYPE_CAR}},                                                       // 0x0AC
    {true, "SET_CAR_CRUISE_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                           // 0x0AD
    {true, "SET_CAR_DRIVING_STYLE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x0AE
    {true, "SET_CAR_MISSION", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                  // 0x0AF
    {true, "IS_CAR_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},    // 0x0B0
    {true, "IS_CAR_IN_AREA_3D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                            // 0x0B1
    {false, "SPECIAL_0", {}, {}},                                                                               // 0x0B2
    {false, "SPECIAL_1", {}, {}},                                                                               // 0x0B3
    {false, "SPECIAL_2", {}, {}},                                                                               // 0x0B4
    {false, "SPECIAL_3", {}, {}},                                                                               // 0x0B5
    {false, "SPECIAL_4", {}, {}},                                                                               // 0x0B6
    {false, "SPECIAL_5", {}, {}},                                                                               // 0x0B7
    {false, "SPECIAL_6", {}, {}},                                                                               // 0x0B8
    {false, "SPECIAL_7", {}, {}},                                                                               // 0x0B9
    {true, "PRINT_BIG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                                                // 0x0BA
    {true, "PRINT", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                                                    // 0x0BB
    {true, "PRINT_NOW", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                                                // 0x0BC
    {false, "PRINT_SOON", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                                              // 0x0BD
    {true, "CLEAR_PRINTS", {}, {}},                                                                             // 0x0BE
    {true, "GET_TIME_OF_DAY", {ARG_RETURN_INT, ARG_RETURN_INT}, {}},                                            // 0x0BF
    {true, "SET_TIME_OF_DAY", {ARG_INT, ARG_INT}, {}},                                                          // 0x0C0
    {true, "GET_MINUTES_TO_TIME_OF_DAY", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {}},                               // 0x0C1
    {true, "IS_POINT_ON_SCREEN", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                             // 0x0C2
    {true, "DEBUG_ON", {}, {}},                                                                                 // 0x0C3
    {true, "DEBUG_OFF", {}, {}},                                                                                // 0x0C4
    {false, "RETURN_TRUE", {}, {}},                                                                             // 0x0C5
    {false, "RETURN_FALSE", {}, {}},                                                                            // 0x0C6
    {true, "VAR_INT",
        {ARG_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT,
            ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT,
            ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT,
            ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT},
        {}},  // 0x0C7
    {true, "VAR_FLOAT",
        {ARG_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT},
        {}},  // 0x0C8
    {true, "LVAR_INT",
        {ARG_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT,
            ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT,
            ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT,
            ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT, ARG_DECLARE_LVAR_INT,
            ARG_DECLARE_LVAR_INT},
        {}},  // 0x0C9
    {true, "LVAR_FLOAT",
        {ARG_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT,
            ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT,
            ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT,
            ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT, ARG_DECLARE_LVAR_FLOAT,
            ARG_DECLARE_LVAR_FLOAT},
        {}},                                                                                                   // 0x0CA
    {true, "{", {}, {}},                                                                                       // 0x0CB
    {true, "}", {}, {}},                                                                                       // 0x0CC
    {true, "REPEAT", {ARG_INT_CONST, ARG_VAR_INT}, {}},                                                        // 0x0CD
    {true, "ENDREPEAT", {}, {}},                                                                               // 0x0CE
    {true, "IF", {ARG_INT_CONST}, {}},                                                                         // 0x0CF
    {true, "IFNOT", {ARG_INT_CONST}, {}},                                                                      // 0x0D0
    {true, "ELSE", {}, {}},                                                                                    // 0x0D1
    {true, "ENDIF", {}, {}},                                                                                   // 0x0D2
    {true, "WHILE", {ARG_INT_CONST}, {}},                                                                      // 0x0D3
    {true, "WHILENOT", {ARG_INT_CONST}, {}},                                                                   // 0x0D4
    {true, "ENDWHILE", {}, {}},                                                                                // 0x0D5
    {true, "ANDOR", {ARG_INT_CONST}, {}},                                                                      // 0x0D6
    {true, "LAUNCH_MISSION", {ARG_LABEL}, {}},                                                                 // 0x0D7
    {true, "MISSION_HAS_FINISHED", {}, {}},                                                                    // 0x0D8
    {true, "STORE_CAR_CHAR_IS_IN", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_CAR}},                          // 0x0D9
    {false, "STORE_CAR_PLAYER_IS_IN", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER, TYPE_CAR}},                     // 0x0DA
    {true, "IS_CHAR_IN_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                                       // 0x0DB
    {false, "IS_PLAYER_IN_CAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CAR}},                                  // 0x0DC
    {true, "IS_CHAR_IN_MODEL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                               // 0x0DD
    {false, "IS_PLAYER_IN_MODEL", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                          // 0x0DE
    {true, "IS_CHAR_IN_ANY_CAR", {ARG_INT}, {TYPE_CHAR}},                                                      // 0x0DF
    {false, "IS_PLAYER_IN_ANY_CAR", {ARG_INT}, {TYPE_PLAYER}},                                                 // 0x0E0
    {true, "IS_BUTTON_PRESSED", {ARG_INT, ARG_INT}, {}},                                                       // 0x0E1
    {true, "GET_PAD_STATE", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {}},                                           // 0x0E2
    {false, "LOCATE_PLAYER_ANY_MEANS_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x0E3
    {false, "LOCATE_PLAYER_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x0E4
    {false, "LOCATE_PLAYER_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x0E5
    {false, "LOCATE_STOPPED_PLAYER_ANY_MEANS_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x0E6
    {false, "LOCATE_STOPPED_PLAYER_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x0E7
    {false, "LOCATE_STOPPED_PLAYER_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x0E8
    {false, "LOCATE_PLAYER_ANY_MEANS_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CHAR}},                                                                             // 0x0E9
    {false, "LOCATE_PLAYER_ON_FOOT_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CHAR}},                                                                             // 0x0EA
    {false, "LOCATE_PLAYER_IN_CAR_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CHAR}},                                                                             // 0x0EB
    {true, "LOCATE_CHAR_ANY_MEANS_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x0EC
    {true, "LOCATE_CHAR_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x0ED
    {true, "LOCATE_CHAR_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x0EE
    {true, "LOCATE_STOPPED_CHAR_ANY_MEANS_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x0EF
    {true, "LOCATE_STOPPED_CHAR_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x0F0
    {true, "LOCATE_STOPPED_CHAR_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x0F1
    {true, "LOCATE_CHAR_ANY_MEANS_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x0F2
    {true, "LOCATE_CHAR_ON_FOOT_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x0F3
    {true, "LOCATE_CHAR_IN_CAR_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x0F4
    {false, "LOCATE_PLAYER_ANY_MEANS_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x0F5
    {false, "LOCATE_PLAYER_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x0F6
    {false, "LOCATE_PLAYER_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x0F7
    {false, "LOCATE_STOPPED_PLAYER_ANY_MEANS_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x0F8
    {false, "LOCATE_STOPPED_PLAYER_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x0F9
    {false, "LOCATE_STOPPED_PLAYER_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x0FA
    {false, "LOCATE_PLAYER_ANY_MEANS_CHAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CHAR}},                                                                             // 0x0FB
    {false, "LOCATE_PLAYER_ON_FOOT_CHAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CHAR}},                                                                             // 0x0FC
    {false, "LOCATE_PLAYER_IN_CAR_CHAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CHAR}},                                                                             // 0x0FD
    {true, "LOCATE_CHAR_ANY_MEANS_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x0FE
    {true, "LOCATE_CHAR_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x0FF
    {true, "LOCATE_CHAR_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x100
    {true, "LOCATE_STOPPED_CHAR_ANY_MEANS_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x101
    {true, "LOCATE_STOPPED_CHAR_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x102
    {true, "LOCATE_STOPPED_CHAR_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x103
    {true, "LOCATE_CHAR_ANY_MEANS_CHAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x104
    {true, "LOCATE_CHAR_ON_FOOT_CHAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x105
    {true, "LOCATE_CHAR_IN_CAR_CHAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x106
    {true, "CREATE_OBJECT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT}},                // 0x107
    {true, "DELETE_OBJECT", {ARG_INT}, {TYPE_OBJECT}},                                                         // 0x108
    {true, "ADD_SCORE", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                                    // 0x109
    {true, "IS_SCORE_GREATER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                             // 0x10A
    {true, "STORE_SCORE", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                                           // 0x10B
    {false, "GIVE_REMOTE_CONTROLLED_CAR_TO_PLAYER", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_PLAYER}},                                                                                        // 0x10C
    {true, "ALTER_WANTED_LEVEL", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                           // 0x10D
    {true, "ALTER_WANTED_LEVEL_NO_DROP", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                   // 0x10E
    {true, "IS_WANTED_LEVEL_GREATER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                      // 0x10F
    {true, "CLEAR_WANTED_LEVEL", {ARG_INT}, {TYPE_PLAYER}},                                                    // 0x110
    {true, "SET_DEATHARREST_STATE", {ARG_INT}, {}},                                                            // 0x111
    {true, "HAS_DEATHARREST_BEEN_EXECUTED", {}, {}},                                                           // 0x112
    {false, "ADD_AMMO_TO_PLAYER", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x113
    {true, "ADD_AMMO_TO_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                      // 0x114
    {false, "ADD_AMMO_TO_CAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x115
    {false, "IS_PLAYER_STILL_ALIVE", {ARG_INT}, {TYPE_PLAYER}},                                                // 0x116
    {true, "IS_PLAYER_DEAD", {ARG_INT}, {TYPE_PLAYER}},                                                        // 0x117
    {true, "IS_CHAR_DEAD", {ARG_INT}, {TYPE_CHAR}},                                                            // 0x118
    {true, "IS_CAR_DEAD", {ARG_INT}, {TYPE_CAR}},                                                              // 0x119
    {false, "SET_CHAR_THREAT_SEARCH", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                        // 0x11A
    {false, "SET_CHAR_THREAT_REACTION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                      // 0x11B
    {false, "SET_CHAR_OBJ_NO_OBJ", {ARG_INT}, {TYPE_CHAR}},                                                    // 0x11C
    {false, "ORDER_DRIVER_OUT_OF_CAR", {ARG_INT}, {TYPE_CHAR}},                                                // 0x11D
    {false, "ORDER_CHAR_TO_DRIVE_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                             // 0x11E
    {false, "ADD_PATROL_POINT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},                      // 0x11F
    {false, "IS_PLAYER_IN_GANGZONE", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_GANG_ZONE}},                       // 0x120
    {false, "IS_PLAYER_IN_ZONE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_PLAYER}},                                    // 0x121
    {true, "IS_PLAYER_PRESSING_HORN", {ARG_INT}, {TYPE_PLAYER}},                                               // 0x122
    {false, "HAS_CHAR_SPOTTED_PLAYER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                          // 0x123
    {false, "ORDER_CHAR_TO_BACKDOOR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                              // 0x124
    {false, "ADD_CHAR_TO_GANG", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                              // 0x125
    {false, "IS_CHAR_OBJECTIVE_PASSED", {ARG_INT}, {TYPE_CHAR}},                                               // 0x126
    {false, "SET_CHAR_DRIVE_AGGRESSION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                     // 0x127
    {false, "SET_CHAR_MAX_DRIVESPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                     // 0x128
    {true, "CREATE_CHAR_INSIDE_CAR", {ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},                                            // 0x129
    {false, "WARP_PLAYER_FROM_CAR_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_PLAYER}},       // 0x12A
    {false, "MAKE_CHAR_DO_NOTHING", {ARG_INT}, {TYPE_CHAR}},                                                   // 0x12B
    {false, "SET_CHAR_INVINCIBLE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x12C
    {false, "SET_PLAYER_INVINCIBLE", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                       // 0x12D
    {false, "SET_CHAR_GRAPHIC_TYPE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                // 0x12E
    {false, "SET_PLAYER_GRAPHIC_TYPE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                            // 0x12F
    {false, "HAS_PLAYER_BEEN_ARRESTED", {ARG_INT}, {TYPE_PLAYER}},                                             // 0x130
    {false, "STOP_CHAR_DRIVING", {ARG_INT}, {TYPE_CHAR}},                                                      // 0x131
    {false, "KILL_CHAR", {ARG_INT}, {TYPE_CHAR}},                                                              // 0x132
    {false, "SET_FAVOURITE_CAR_MODEL_FOR_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                              // 0x133
    {false, "SET_CHAR_OCCUPATION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x134
    {false, "CHANGE_CAR_LOCK", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                // 0x135
    {false, "SHAKE_CAM_WITH_POINT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                           // 0x136
    {true, "IS_CAR_MODEL", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                    // 0x137
    {false, "IS_CAR_REMAP", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                   // 0x138
    {false, "HAS_CAR_JUST_SUNK", {ARG_INT}, {TYPE_CAR}},                                                       // 0x139
    {false, "SET_CAR_NO_COLLIDE", {ARG_INT}, {TYPE_CAR}},                                                      // 0x13A
    {false, "IS_CAR_DEAD_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                           // 0x13B
    {false, "IS_CAR_DEAD_IN_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},     // 0x13C
    {false, "IS_TRAILER_ATTACHED", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                                  // 0x13D
    {false, "IS_CAR_ON_TRAILER", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                                    // 0x13E
    {false, "HAS_CAR_GOT_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                             // 0x13F
    {false, "PARK", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_PARK}},                                                // 0x140
    {false, "HAS_PARK_FINISHED", {}, {}},                                                                      // 0x141
    {false, "KILL_ALL_PASSENGERS", {ARG_INT}, {TYPE_CAR}},                                                     // 0x142
    {false, "SET_CAR_BULLETPROOF", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x143
    {false, "SET_CAR_FLAMEPROOF", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                             // 0x144
    {false, "SET_CAR_ROCKETPROOF", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x145
    {false, "IS_CARBOMB_ACTIVE", {ARG_INT}, {TYPE_CAR}},                                                       // 0x146
    {false, "GIVE_CAR_ALARM", {ARG_INT}, {TYPE_CAR}},                                                          // 0x147
    {false, "PUT_CAR_ON_TRAILER", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                                   // 0x148
    {false, "IS_CAR_CRUSHED", {ARG_INT}, {TYPE_CAR}},                                                          // 0x149
    {false, "CREATE_GANG_CAR", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},                   // 0x14A
    {true, "CREATE_CAR_GENERATOR",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT,
            ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CAR_GENERATOR}},                                                                     // 0x14B
    {true, "SWITCH_CAR_GENERATOR", {ARG_INT, ARG_INT}, {TYPE_CAR_GENERATOR}},                         // 0x14C
    {false, "ADD_PAGER_MESSAGE", {ARG_TEXT_LABEL, ARG_INT_CONST, ARG_INT_CONST, ARG_INT_CONST}, {}},  // 0x14D
    {true, "DISPLAY_ONSCREEN_TIMER", {ARG_VAR_INT, ARG_INT}, {}},                                     // 0x14E
    {true, "CLEAR_ONSCREEN_TIMER", {ARG_VAR_INT}, {}},                                                // 0x14F
    {false, "DISPLAY_ONSCREEN_COUNTER", {ARG_VAR_INT, ARG_INT}, {}},                                  // 0x150
    {true, "CLEAR_ONSCREEN_COUNTER", {ARG_VAR_INT}, {}},                                              // 0x151
    {false, "SET_ZONE_CAR_INFO",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT,
            ARG_INT, ARG_INT},
        {}},                                                                           // 0x152
    {false, "IS_CHAR_IN_GANG_ZONE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_GANG_ZONE}},  // 0x153
    {true, "IS_CHAR_IN_ZONE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_CHAR}},                 // 0x154
    {false, "SET_CAR_DENSITY", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                // 0x155
    {false, "SET_PED_DENSITY", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                // 0x156
    {false, "POINT_CAMERA_AT_PLAYER", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},     // 0x157
    {true, "POINT_CAMERA_AT_CAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},            // 0x158
    {true, "POINT_CAMERA_AT_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},          // 0x159
    {true, "RESTORE_CAMERA", {}, {}},                                                  // 0x15A
    {true, "SHAKE_PAD", {ARG_INT, ARG_INT, ARG_INT}, {}},                              // 0x15B
    {false, "SET_ZONE_PED_INFO",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT,
            ARG_INT, ARG_INT},
        {}},                                                                          // 0x15C
    {true, "SET_TIME_SCALE", {ARG_FLOAT}, {}},                                        // 0x15D
    {false, "IS_CAR_IN_AIR", {ARG_INT}, {TYPE_CAR}},                                  // 0x15E
    {true, "SET_FIXED_CAMERA_POSITION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                          // 0x15F
    {true, "POINT_CAMERA_AT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},  // 0x160
    {true, "ADD_BLIP_FOR_CAR_OLD", {ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                   // 0x161
    {false, "ADD_BLIP_FOR_CHAR_OLD", {ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                  // 0x162
    {false, "ADD_BLIP_FOR_OBJECT_OLD", {ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                // 0x163
    {true, "REMOVE_BLIP", {ARG_INT}, {TYPE_BLIP}},                                    // 0x164
    {true, "CHANGE_BLIP_COLOUR", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                    // 0x165
    {false, "DIM_BLIP", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                             // 0x166
    {true, "ADD_BLIP_FOR_COORD_OLD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_BLIP}},                                                                        // 0x167
    {true, "CHANGE_BLIP_SCALE", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                               // 0x168
    {true, "SET_FADING_COLOUR", {ARG_INT, ARG_INT, ARG_INT}, {}},                               // 0x169
    {true, "DO_FADE", {ARG_INT, ARG_INT}, {}},                                                  // 0x16A
    {true, "GET_FADING_STATUS", {}, {}},                                                        // 0x16B
    {true, "ADD_HOSPITAL_RESTART", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},  // 0x16C
    {true, "ADD_POLICE_RESTART", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},    // 0x16D
    {true, "OVERRIDE_NEXT_RESTART", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},          // 0x16E
    {true, "DRAW_SHADOW",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                   // 0x16F
    {false, "GET_PLAYER_HEADING", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_PLAYER}},                                 // 0x170
    {false, "SET_PLAYER_HEADING", {ARG_INT, ARG_FLOAT}, {TYPE_PLAYER}},                                        // 0x171
    {true, "GET_CHAR_HEADING", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},                                      // 0x172
    {true, "SET_CHAR_HEADING", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                             // 0x173
    {true, "GET_CAR_HEADING", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                        // 0x174
    {true, "SET_CAR_HEADING", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                               // 0x175
    {true, "GET_OBJECT_HEADING", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},                                  // 0x176
    {true, "SET_OBJECT_HEADING", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT}},                                         // 0x177
    {false, "IS_PLAYER_TOUCHING_OBJECT", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_OBJECT}},                      // 0x178
    {true, "IS_CHAR_TOUCHING_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},                           // 0x179
    {false, "SET_PLAYER_AMMO", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                    // 0x17A
    {true, "SET_CHAR_AMMO", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                         // 0x17B
    {false, "SET_CAR_AMMO", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                          // 0x17C
    {false, "LOAD_CAMERA_SPLINE", {ARG_TEXT_LABEL}, {}},                                                       // 0x17D
    {false, "MOVE_CAMERA_ALONG_SPLINE", {ARG_INT}, {}},                                                        // 0x17E
    {false, "GET_CAMERA_POSITION_ALONG_SPLINE", {ARG_RETURN_FLOAT}, {}},                                       // 0x17F
    {true, "DECLARE_MISSION_FLAG", {ARG_VAR_INT}, {}},                                                         // 0x180
    {false, "DECLARE_MISSION_FLAG_FOR_CONTACT", {ARG_INT, ARG_VAR_INT}, {}},                                   // 0x181
    {false, "DECLARE_BASE_BRIEF_ID_FOR_CONTACT", {ARG_INT, ARG_INT}, {}},                                      // 0x182
    {false, "IS_PLAYER_HEALTH_GREATER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                    // 0x183
    {true, "IS_CHAR_HEALTH_GREATER", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                         // 0x184
    {true, "IS_CAR_HEALTH_GREATER", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                           // 0x185
    {true, "ADD_BLIP_FOR_CAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_BLIP}},                              // 0x186
    {true, "ADD_BLIP_FOR_CHAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_BLIP}},                            // 0x187
    {true, "ADD_BLIP_FOR_OBJECT", {ARG_INT, ARG_RETURN_INT}, {TYPE_OBJECT, TYPE_BLIP}},                        // 0x188
    {false, "ADD_BLIP_FOR_CONTACT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                                    // 0x189
    {true, "ADD_BLIP_FOR_COORD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                                    // 0x18A
    {true, "CHANGE_BLIP_DISPLAY", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                                            // 0x18B
    {true, "ADD_ONE_OFF_SOUND", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                               // 0x18C
    {true, "ADD_CONTINUOUS_SOUND", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_SOUND}},                 // 0x18D
    {true, "REMOVE_SOUND", {ARG_INT}, {TYPE_SOUND}},                                                           // 0x18E
    {true, "IS_CAR_STUCK_ON_ROOF", {ARG_INT}, {TYPE_CAR}},                                                     // 0x18F
    {true, "ADD_UPSIDEDOWN_CAR_CHECK", {ARG_INT}, {TYPE_CAR}},                                                 // 0x190
    {true, "REMOVE_UPSIDEDOWN_CAR_CHECK", {ARG_INT}, {TYPE_CAR}},                                              // 0x191
    {false, "SET_CHAR_OBJ_WAIT_ON_FOOT", {ARG_INT}, {TYPE_CHAR}},                                              // 0x192
    {false, "SET_CHAR_OBJ_FLEE_ON_FOOT_TILL_SAFE", {ARG_INT}, {TYPE_CHAR}},                                    // 0x193
    {false, "SET_CHAR_OBJ_GUARD_SPOT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},               // 0x194
    {false, "SET_CHAR_OBJ_GUARD_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},    // 0x195
    {false, "SET_CHAR_OBJ_WAIT_IN_CAR", {ARG_INT}, {TYPE_CHAR}},                                               // 0x196
    {false, "IS_PLAYER_IN_AREA_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x197
    {false, "IS_PLAYER_IN_AREA_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x198
    {false, "IS_PLAYER_STOPPED_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x199
    {false, "IS_PLAYER_STOPPED_IN_AREA_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x19A
    {false, "IS_PLAYER_STOPPED_IN_AREA_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x19B
    {false, "IS_PLAYER_IN_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x19C
    {false, "IS_PLAYER_IN_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x19D
    {false, "IS_PLAYER_STOPPED_IN_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x19E
    {false, "IS_PLAYER_STOPPED_IN_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x19F
    {false, "IS_PLAYER_STOPPED_IN_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},  // 0x1A0
    {true, "IS_CHAR_IN_AREA_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x1A1
    {true, "IS_CHAR_IN_AREA_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x1A2
    {true, "IS_CHAR_STOPPED_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x1A3
    {true, "IS_CHAR_STOPPED_IN_AREA_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x1A4
    {true, "IS_CHAR_STOPPED_IN_AREA_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x1A5
    {true, "IS_CHAR_IN_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x1A6
    {true, "IS_CHAR_IN_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x1A7
    {true, "IS_CHAR_STOPPED_IN_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x1A8
    {true, "IS_CHAR_STOPPED_IN_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x1A9
    {true, "IS_CHAR_STOPPED_IN_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x1AA
    {true, "IS_CAR_STOPPED_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                           // 0x1AB
    {true, "IS_CAR_STOPPED_IN_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},     // 0x1AC
    {true, "LOCATE_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},       // 0x1AD
    {true, "LOCATE_STOPPED_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                           // 0x1AE
    {true, "LOCATE_CAR_3D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                           // 0x1AF
    {true, "LOCATE_STOPPED_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},     // 0x1B0
    {false, "GIVE_WEAPON_TO_PLAYER", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                              // 0x1B1
    {true, "GIVE_WEAPON_TO_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x1B2
    {false, "GIVE_WEAPON_TO_CAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                    // 0x1B3
    {true, "SET_PLAYER_CONTROL", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                           // 0x1B4
    {true, "FORCE_WEATHER", {ARG_INT}, {}},                                                                    // 0x1B5
    {true, "FORCE_WEATHER_NOW", {ARG_INT}, {}},                                                                // 0x1B6
    {true, "RELEASE_WEATHER", {}, {}},                                                                         // 0x1B7
    {false, "SET_CURRENT_PLAYER_WEAPON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                   // 0x1B8
    {true, "SET_CURRENT_CHAR_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                        // 0x1B9
    {false, "SET_CURRENT_CAR_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                         // 0x1BA
    {true, "GET_OBJECT_COORDINATES", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_OBJECT}},                                                                                        // 0x1BB
    {true, "SET_OBJECT_COORDINATES", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},               // 0x1BC
    {true, "GET_GAME_TIMER", {ARG_RETURN_INT}, {}},                                                            // 0x1BD
    {false, "TURN_CHAR_TO_FACE_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},               // 0x1BE
    {false, "TURN_PLAYER_TO_FACE_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_PLAYER}},           // 0x1BF
    {true, "STORE_WANTED_LEVEL", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                                    // 0x1C0
    {true, "IS_CAR_STOPPED", {ARG_INT}, {TYPE_CAR}},                                                           // 0x1C1
    {true, "MARK_CHAR_AS_NO_LONGER_NEEDED", {ARG_INT}, {TYPE_CHAR}},                                           // 0x1C2
    {true, "MARK_CAR_AS_NO_LONGER_NEEDED", {ARG_INT}, {TYPE_CAR}},                                             // 0x1C3
    {true, "MARK_OBJECT_AS_NO_LONGER_NEEDED", {ARG_INT}, {TYPE_OBJECT}},                                       // 0x1C4
    {true, "DONT_REMOVE_CHAR", {ARG_INT}, {TYPE_CHAR}},                                                        // 0x1C5
    {false, "DONT_REMOVE_CAR", {ARG_INT}, {TYPE_CAR}},                                                         // 0x1C6
    {true, "DONT_REMOVE_OBJECT", {ARG_INT}, {TYPE_OBJECT}},                                                    // 0x1C7
    {true, "CREATE_CHAR_AS_PASSENGER", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},                          // 0x1C8
    {false, "SET_CHAR_OBJ_KILL_CHAR_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                     // 0x1C9
    {false, "SET_CHAR_OBJ_KILL_PLAYER_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                 // 0x1CA
    {false, "SET_CHAR_OBJ_KILL_CHAR_ANY_MEANS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                   // 0x1CB
    {false, "SET_CHAR_OBJ_KILL_PLAYER_ANY_MEANS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},               // 0x1CC
    {false, "SET_CHAR_OBJ_FLEE_CHAR_ON_FOOT_TILL_SAFE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},           // 0x1CD
    {false, "SET_CHAR_OBJ_FLEE_PLAYER_ON_FOOT_TILL_SAFE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},       // 0x1CE
    {false, "SET_CHAR_OBJ_FLEE_CHAR_ON_FOOT_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},              // 0x1CF
    {false, "SET_CHAR_OBJ_FLEE_PLAYER_ON_FOOT_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},          // 0x1D0
    {false, "SET_CHAR_OBJ_GOTO_CHAR_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                     // 0x1D1
    {false, "SET_CHAR_OBJ_GOTO_PLAYER_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                 // 0x1D2
    {false, "SET_CHAR_OBJ_LEAVE_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                              // 0x1D3
    {false, "SET_CHAR_OBJ_ENTER_CAR_AS_PASSENGER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                 // 0x1D4
    {false, "SET_CHAR_OBJ_ENTER_CAR_AS_DRIVER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                    // 0x1D5
    {false, "SET_CHAR_OBJ_FOLLOW_CAR_IN_CAR", {ARG_INT}, {TYPE_CHAR}},                                         // 0x1D6
    {false, "SET_CHAR_OBJ_FIRE_AT_OBJECT_FROM_VEHICLE", {ARG_INT}, {TYPE_CHAR}},                               // 0x1D7
    {false, "SET_CHAR_OBJ_DESTROY_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},                      // 0x1D8
    {false, "SET_CHAR_OBJ_DESTROY_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                            // 0x1D9
    {false, "SET_CHAR_OBJ_GOTO_AREA_ON_FOOT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR}},                                                                                          // 0x1DA
    {false, "SET_CHAR_OBJ_GOTO_AREA_IN_CAR", {ARG_INT}, {TYPE_CHAR}},                                          // 0x1DB
    {false, "SET_CHAR_OBJ_FOLLOW_CAR_ON_FOOT_WITH_OFFSET", {ARG_INT}, {TYPE_CHAR}},                            // 0x1DC
    {false, "SET_CHAR_OBJ_GUARD_ATTACK", {ARG_INT}, {TYPE_CHAR}},                                              // 0x1DD
    {false, "SET_CHAR_AS_LEADER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                                 // 0x1DE
    {false, "SET_PLAYER_AS_LEADER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                             // 0x1DF
    {false, "LEAVE_GROUP", {ARG_INT}, {TYPE_CHAR}},                                                            // 0x1E0
    {false, "SET_CHAR_OBJ_FOLLOW_ROUTE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                            // 0x1E1
    {false, "ADD_ROUTE_POINT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                // 0x1E2
    {true, "PRINT_WITH_NUMBER_BIG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},                          // 0x1E3
    {true, "PRINT_WITH_NUMBER", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},                              // 0x1E4
    {true, "PRINT_WITH_NUMBER_NOW", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},                          // 0x1E5
    {false, "PRINT_WITH_NUMBER_SOON", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},                        // 0x1E6
    {true, "SWITCH_ROADS_ON", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},         // 0x1E7
    {true, "SWITCH_ROADS_OFF", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},        // 0x1E8
    {true, "GET_NUMBER_OF_PASSENGERS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                 // 0x1E9
    {true, "GET_MAXIMUM_NUMBER_OF_PASSENGERS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                         // 0x1EA
    {true, "SET_CAR_DENSITY_MULTIPLIER", {ARG_FLOAT}, {}},                                                     // 0x1EB
    {true, "SET_CAR_HEAVY", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                   // 0x1EC
    {false, "CLEAR_CHAR_THREAT_SEARCH", {ARG_INT}, {TYPE_CHAR}},                                               // 0x1ED
    {false, "ACTIVATE_CRANE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                  // 0x1EE
    {false, "DEACTIVATE_CRANE", {ARG_FLOAT, ARG_FLOAT}, {}},  // 0x1EF
    {true, "SET_MAX_WANTED_LEVEL", {ARG_INT}, {}},            // 0x1F0
    {false, "SAVE_VAR_INT",
        {ARG_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT,
            ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT,
            ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT, ARG_DECLARE_VAR_INT,
            ARG_DECLARE_VAR_INT},
        {}},  // 0x1F1
    {false, "SAVE_VAR_FLOAT",
        {ARG_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT,
            ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT, ARG_DECLARE_VAR_FLOAT},
        {}},                                                                                             // 0x1F2
    {true, "IS_CAR_IN_AIR_PROPER", {ARG_INT}, {TYPE_CAR}},                                               // 0x1F3
    {true, "IS_CAR_UPSIDEDOWN", {ARG_INT}, {TYPE_CAR}},                                                  // 0x1F4
    {true, "GET_PLAYER_CHAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER, TYPE_CHAR}},                      // 0x1F5
    {true, "CANCEL_OVERRIDE_RESTART", {}, {}},                                                           // 0x1F6
    {true, "SET_POLICE_IGNORE_PLAYER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                               // 0x1F7
    {false, "ADD_PAGER_MESSAGE_WITH_NUMBER", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},  // 0x1F8
    {true, "START_KILL_FRENZY",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},   // 0x1F9
    {true, "READ_KILL_FRENZY_STATUS", {ARG_RETURN_INT}, {}},                                             // 0x1FA
    {true, "SQRT", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                                   // 0x1FB
    {false, "LOCATE_PLAYER_ANY_MEANS_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CAR}},                                                                        // 0x1FC
    {false, "LOCATE_PLAYER_ON_FOOT_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CAR}},                                                                        // 0x1FD
    {false, "LOCATE_PLAYER_IN_CAR_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CAR}},                                                                        // 0x1FE
    {false, "LOCATE_PLAYER_ANY_MEANS_CAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CAR}},                                                                        // 0x1FF
    {false, "LOCATE_PLAYER_ON_FOOT_CAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CAR}},                                                                        // 0x200
    {false, "LOCATE_PLAYER_IN_CAR_CAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER, TYPE_CAR}},                                                                        // 0x201
    {true, "LOCATE_CHAR_ANY_MEANS_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x202
    {true, "LOCATE_CHAR_ON_FOOT_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x203
    {true, "LOCATE_CHAR_IN_CAR_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x204
    {true, "LOCATE_CHAR_ANY_MEANS_CAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x205
    {true, "LOCATE_CHAR_ON_FOOT_CAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x206
    {true, "LOCATE_CHAR_IN_CAR_CAR_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x207
    {true, "GENERATE_RANDOM_FLOAT_IN_RANGE", {ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT}, {}},              // 0x208
    {true, "GENERATE_RANDOM_INT_IN_RANGE", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {}},                      // 0x209
    {true, "LOCK_CAR_DOORS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x20A
    {true, "EXPLODE_CAR", {ARG_INT}, {TYPE_CAR}},                                                        // 0x20B
    {true, "ADD_EXPLOSION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                             // 0x20C
    {true, "IS_CAR_UPRIGHT", {ARG_INT}, {TYPE_CAR}},                                                     // 0x20D
    {false, "TURN_CHAR_TO_FACE_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                       // 0x20E
    {false, "TURN_CHAR_TO_FACE_PLAYER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                   // 0x20F
    {false, "TURN_PLAYER_TO_FACE_CHAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CHAR}},                   // 0x210
    {false, "SET_CHAR_OBJ_GOTO_COORD_ON_FOOT", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},            // 0x211
    {false, "SET_CHAR_OBJ_GOTO_COORD_IN_CAR", {ARG_INT}, {TYPE_CHAR}},                                   // 0x212
    {true, "CREATE_PICKUP", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_PICKUP}},                                                                // 0x213
    {true, "HAS_PICKUP_BEEN_COLLECTED", {ARG_INT}, {TYPE_PICKUP}},                        // 0x214
    {true, "REMOVE_PICKUP", {ARG_INT}, {TYPE_PICKUP}},                                    // 0x215
    {true, "SET_TAXI_LIGHTS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                            // 0x216
    {true, "PRINT_BIG_Q", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                        // 0x217
    {false, "PRINT_WITH_NUMBER_BIG_Q", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},  // 0x218
    {false, "SET_GARAGE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT,
            ARG_RETURN_INT},
        {}},  // 0x219
    {false, "SET_GARAGE_WITH_CAR_MODEL",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT,
            ARG_RETURN_INT},
        {}},                                                                                                  // 0x21A
    {true, "SET_TARGET_CAR_FOR_MISSION_GARAGE", {ARG_TEXT_LABEL, ARG_INT}, {TYPE_UNIMPORTANT, TYPE_CAR}},     // 0x21B
    {false, "IS_CAR_IN_MISSION_GARAGE", {ARG_TEXT_LABEL}, {}},                                                // 0x21C
    {false, "SET_FREE_BOMBS", {ARG_INT}, {}},                                                                 // 0x21D
    {false, "SET_POWERPOINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                                  // 0x21E
    {false, "SET_ALL_TAXI_LIGHTS", {ARG_INT}, {}},                                                            // 0x21F
    {false, "IS_CAR_ARMED_WITH_ANY_BOMB", {ARG_INT}, {TYPE_CAR}},                                             // 0x220
    {true, "APPLY_BRAKES_TO_PLAYERS_CAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x221
    {false, "SET_PLAYER_HEALTH", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                          // 0x222
    {true, "SET_CHAR_HEALTH", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                               // 0x223
    {true, "SET_CAR_HEALTH", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                 // 0x224
    {false, "GET_PLAYER_HEALTH", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                                   // 0x225
    {true, "GET_CHAR_HEALTH", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                        // 0x226
    {true, "GET_CAR_HEALTH", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                          // 0x227
    {false, "IS_CAR_ARMED_WITH_BOMB", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                        // 0x228
    {true, "CHANGE_CAR_COLOUR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                     // 0x229
    {true, "SWITCH_PED_ROADS_ON", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},    // 0x22A
    {true, "SWITCH_PED_ROADS_OFF", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},   // 0x22B
    {false, "CHAR_LOOK_AT_CHAR_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                          // 0x22C
    {false, "CHAR_LOOK_AT_PLAYER_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                      // 0x22D
    {false, "PLAYER_LOOK_AT_CHAR_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CHAR}},                      // 0x22E
    {false, "STOP_CHAR_LOOKING", {ARG_INT}, {TYPE_CHAR}},                                                     // 0x22F
    {false, "STOP_PLAYER_LOOKING", {ARG_INT}, {TYPE_PLAYER}},                                                 // 0x230
    {false, "SET_SCRIPT_POLICE_HELI_TO_CHASE_CHAR", {ARG_INT}, {TYPE_CHAR}},                                  // 0x231
    {false, "SET_GANG_ATTITUDE", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                  // 0x232
    {false, "SET_GANG_GANG_ATTITUDE", {ARG_INT, ARG_INT, ARG_INT}, {}},                                       // 0x233
    {false, "SET_GANG_PLAYER_ATTITUDE", {ARG_INT, ARG_INT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_PLAYER}},                                                   // 0x234
    {false, "SET_GANG_PED_MODELS", {ARG_INT, ARG_INT, ARG_INT}, {}},                                          // 0x235
    {false, "SET_GANG_CAR_MODEL", {ARG_INT, ARG_INT}, {}},                                                    // 0x236
    {true, "SET_GANG_WEAPONS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                                     // 0x237
    {false, "SET_CHAR_OBJ_RUN_TO_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},  // 0x238
    {false, "SET_CHAR_OBJ_RUN_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},                       // 0x239
    {false, "IS_PLAYER_TOUCHING_OBJECT_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_OBJECT}},             // 0x23A
    {true, "IS_CHAR_TOUCHING_OBJECT_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},                  // 0x23B
    {true, "LOAD_SPECIAL_CHARACTER", {ARG_INT, ARG_TEXT_LABEL}, {}},                                          // 0x23C
    {true, "HAS_SPECIAL_CHARACTER_LOADED", {ARG_INT}, {}},                                                    // 0x23D
    {false, "FLASH_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                     // 0x23E
    {false, "FLASH_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                   // 0x23F
    {false, "FLASH_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                               // 0x240
    {true, "IS_PLAYER_IN_REMOTE_MODE", {ARG_INT}, {TYPE_PLAYER}},                                             // 0x241
    {true, "ARM_CAR_WITH_BOMB", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                              // 0x242
    {false, "SET_CHAR_PERSONALITY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                         // 0x243
    {true, "SET_CUTSCENE_OFFSET", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                     // 0x244
    {true, "SET_ANIM_GROUP_FOR_CHAR", {ARG_INT, ARG_TEXT_LABEL16}, {TYPE_CHAR}},                              // 0x245
    {false, "SET_ANIM_GROUP_FOR_PLAYER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                  // 0x246
    {true, "REQUEST_MODEL", {ARG_INT}, {}},                                                                   // 0x247
    {true, "HAS_MODEL_LOADED", {ARG_INT}, {}},                                                                // 0x248
    {true, "MARK_MODEL_AS_NO_LONGER_NEEDED", {ARG_INT}, {}},                                                  // 0x249
    {false, "GRAB_PHONE", {ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_PHONE}},                                                    // 0x24A
    {false, "SET_REPEATED_PHONE_MESSAGE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_PHONE}},                           // 0x24B
    {false, "SET_PHONE_MESSAGE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_PHONE}},                                    // 0x24C
    {false, "HAS_PHONE_DISPLAYED_MESSAGE", {ARG_INT}, {TYPE_PHONE}},                                          // 0x24D
    {false, "TURN_PHONE_OFF", {ARG_INT}, {TYPE_PHONE}},                                                       // 0x24E
    {true, "DRAW_CORONA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                  // 0x24F
    {false, "DRAW_LIGHT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT}, {}},                  // 0x250
    {false, "STORE_WEATHER", {}, {}},                                                                         // 0x251
    {false, "RESTORE_WEATHER", {}, {}},                                                                       // 0x252
    {true, "STORE_CLOCK", {}, {}},                                                                            // 0x253
    {true, "RESTORE_CLOCK", {}, {}},                                                                          // 0x254
    {false, "RESTART_CRITICAL_MISSION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                    // 0x255
    {true, "IS_PLAYER_PLAYING", {ARG_INT}, {TYPE_PLAYER}},                                                    // 0x256
    {false, "SET_COLL_OBJ_NO_OBJ", {ARG_INT}, {TYPE_COLL_OBJ}},                                               // 0x257
    {false, "SET_COLL_OBJ_WAIT_ON_FOOT", {ARG_INT}, {TYPE_COLL_OBJ}},                                         // 0x258
    {false, "SET_COLL_OBJ_FLEE_ON_FOOT_TILL_SAFE", {ARG_INT}, {TYPE_COLL_OBJ}},                               // 0x259
    {false, "SET_COLL_OBJ_GUARD_SPOT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_COLL_OBJ}},          // 0x25A
    {false, "SET_COLL_OBJ_GUARD_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x25B
    {false, "SET_COLL_OBJ_WAIT_IN_CAR", {ARG_INT}, {TYPE_COLL_OBJ}},                                          // 0x25C
    {false, "SET_COLL_OBJ_KILL_CHAR_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CHAR}},                // 0x25D
    {false, "SET_COLL_OBJ_KILL_PLAYER_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_PLAYER}},            // 0x25E
    {false, "SET_COLL_OBJ_KILL_CHAR_ANY_MEANS", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CHAR}},              // 0x25F
    {false, "SET_COLL_OBJ_KILL_PLAYER_ANY_MEANS", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_PLAYER}},          // 0x260
    {false, "SET_COLL_OBJ_FLEE_CHAR_ON_FOOT_TILL_SAFE", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CHAR}},      // 0x261
    {false, "SET_COLL_OBJ_FLEE_PLAYER_ON_FOOT_TILL_SAFE", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_PLAYER}},  // 0x262
    {false, "SET_COLL_OBJ_FLEE_CHAR_ON_FOOT_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CHAR}},         // 0x263
    {false, "SET_COLL_OBJ_FLEE_PLAYER_ON_FOOT_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_PLAYER}},     // 0x264
    {false, "SET_COLL_OBJ_GOTO_CHAR_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CHAR}},                // 0x265
    {false, "SET_COLL_OBJ_GOTO_PLAYER_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_PLAYER}},            // 0x266
    {false, "SET_COLL_OBJ_LEAVE_CAR", {ARG_INT}, {TYPE_COLL_OBJ}},                                            // 0x267
    {false, "SET_COLL_OBJ_ENTER_CAR_AS_PASSENGER", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CAR}},            // 0x268
    {false, "SET_COLL_OBJ_ENTER_CAR_AS_DRIVER", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CAR}},               // 0x269
    {false, "SET_COLL_OBJ_FOLLOW_CAR_IN_CAR", {ARG_INT}, {TYPE_COLL_OBJ}},                                    // 0x26A
    {false, "SET_COLL_OBJ_FIRE_AT_OBJECT_FROM_VEHICLE", {ARG_INT}, {TYPE_COLL_OBJ}},                          // 0x26B
    {false, "SET_COLL_OBJ_DESTROY_OBJECT", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_OBJECT}},                 // 0x26C
    {false, "SET_COLL_OBJ_DESTROY_CAR", {ARG_INT, ARG_INT}, {TYPE_COLL_OBJ, TYPE_CAR}},                       // 0x26D
    {false, "SET_COLL_OBJ_GOTO_AREA_ON_FOOT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x26E
    {false, "SET_COLL_OBJ_GOTO_AREA_IN_CAR", {ARG_INT}, {TYPE_COLL_OBJ}},                                     // 0x26F
    {false, "SET_COLL_OBJ_FOLLOW_CAR_ON_FOOT_WITH_OFFSET", {ARG_INT}, {TYPE_COLL_OBJ}},                       // 0x270
    {false, "SET_COLL_OBJ_GUARD_ATTACK", {ARG_INT}, {TYPE_COLL_OBJ}},                                         // 0x271
    {false, "SET_COLL_OBJ_FOLLOW_ROUTE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_COLL_OBJ}},                       // 0x272
    {false, "SET_COLL_OBJ_GOTO_COORD_ON_FOOT", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_COLL_OBJ}},             // 0x273
    {false, "SET_COLL_OBJ_GOTO_COORD_IN_CAR", {ARG_INT}, {TYPE_COLL_OBJ}},                                    // 0x274
    {false, "SET_COLL_OBJ_RUN_TO_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x275
    {false, "SET_COLL_OBJ_RUN_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_COLL_OBJ}},                   // 0x276
    {false, "ADD_PEDS_IN_AREA_TO_COLL", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_COLL_OBJ}},                               // 0x277
    {false, "ADD_PEDS_IN_VEHICLE_TO_COLL", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_COLL_OBJ}},             // 0x278
    {false, "CLEAR_COLL", {ARG_INT}, {TYPE_COLL_OBJ}},                                                        // 0x279
    {false, "IS_COLL_IN_CARS", {ARG_INT}, {TYPE_COLL_OBJ}},                                                   // 0x27A
    {false, "LOCATE_COLL_ANY_MEANS_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x27B
    {false, "LOCATE_COLL_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x27C
    {false, "LOCATE_COLL_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x27D
    {false, "LOCATE_STOPPED_COLL_ANY_MEANS_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x27E
    {false, "LOCATE_STOPPED_COLL_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x27F
    {false, "LOCATE_STOPPED_COLL_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x280
    {false, "LOCATE_COLL_ANY_MEANS_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_CHAR}},                                                                          // 0x281
    {false, "LOCATE_COLL_ON_FOOT_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_CHAR}},                                                                          // 0x282
    {false, "LOCATE_COLL_IN_CAR_CHAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_CHAR}},                                                                          // 0x283
    {false, "LOCATE_COLL_ANY_MEANS_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_CAR}},                                                                           // 0x284
    {false, "LOCATE_COLL_ON_FOOT_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_CAR}},                                                                           // 0x285
    {false, "LOCATE_COLL_IN_CAR_CAR_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_CAR}},                                                                           // 0x286
    {false, "LOCATE_COLL_ANY_MEANS_PLAYER_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_PLAYER}},                                                                        // 0x287
    {false, "LOCATE_COLL_ON_FOOT_PLAYER_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_PLAYER}},                                                                        // 0x288
    {false, "LOCATE_COLL_IN_CAR_PLAYER_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ, TYPE_PLAYER}},                                                                        // 0x289
    {false, "IS_COLL_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x28A
    {false, "IS_COLL_IN_AREA_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x28B
    {false, "IS_COLL_IN_AREA_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x28C
    {false, "IS_COLL_STOPPED_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x28D
    {false, "IS_COLL_STOPPED_IN_AREA_ON_FOOT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x28E
    {false, "IS_COLL_STOPPED_IN_AREA_IN_CAR_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x28F
    {false, "GET_NUMBER_OF_PEDS_IN_COLL", {ARG_INT, ARG_RETURN_INT}, {TYPE_COLL_OBJ}},                        // 0x290
    {false, "SET_CHAR_HEED_THREATS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                        // 0x291
    {false, "SET_PLAYER_HEED_THREATS", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                    // 0x292
    {true, "GET_CONTROLLER_MODE", {ARG_RETURN_INT}, {}},                                                      // 0x293
    {true, "SET_CAN_RESPRAY_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x294
    {false, "IS_TAXI", {ARG_INT}, {TYPE_CAR}},                                                                // 0x295
    {true, "UNLOAD_SPECIAL_CHARACTER", {ARG_INT}, {}},                                                        // 0x296
    {true, "RESET_NUM_OF_MODELS_KILLED_BY_PLAYER", {ARG_INT}, {TYPE_PLAYER}},                                 // 0x297
    {true, "GET_NUM_OF_MODELS_KILLED_BY_PLAYER", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},          // 0x298
    {true, "ACTIVATE_GARAGE", {ARG_TEXT_LABEL}, {}},                                                          // 0x299
    {false, "SWITCH_TAXI_TIMER", {ARG_INT}, {}},                                                              // 0x29A
    {true, "CREATE_OBJECT_NO_OFFSET", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT}},               // 0x29B
    {false, "IS_BOAT", {ARG_INT}, {TYPE_CAR}},                                                                // 0x29C
    {false, "SET_CHAR_OBJ_GOTO_AREA_ANY_MEANS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR}},                                                                                         // 0x29D
    {false, "SET_COLL_OBJ_GOTO_AREA_ANY_MEANS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_COLL_OBJ}},                                                                                     // 0x29E
    {false, "IS_PLAYER_STOPPED", {ARG_INT}, {TYPE_PLAYER}},                                                   // 0x29F
    {true, "IS_CHAR_STOPPED", {ARG_INT}, {TYPE_CHAR}},                                                        // 0x2A0
    {false, "MESSAGE_WAIT", {ARG_INT, ARG_INT}, {}},                                                          // 0x2A1
    {false, "ADD_PARTICLE_EFFECT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                  // 0x2A2
    {true, "SWITCH_WIDESCREEN", {ARG_INT}, {}},                                                               // 0x2A3
    {false, "ADD_SPRITE_BLIP_FOR_CAR", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_BLIP}},                                                             // 0x2A4
    {false, "ADD_SPRITE_BLIP_FOR_CHAR", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_BLIP}},                                                            // 0x2A5
    {false, "ADD_SPRITE_BLIP_FOR_OBJECT", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_BLIP}},                                                          // 0x2A6
    {true, "ADD_SPRITE_BLIP_FOR_CONTACT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                 // 0x2A7
    {true, "ADD_SPRITE_BLIP_FOR_COORD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                 // 0x2A8
    {true, "SET_CHAR_ONLY_DAMAGED_BY_PLAYER", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                               // 0x2A9
    {true, "SET_CAR_ONLY_DAMAGED_BY_PLAYER", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                 // 0x2AA
    {true, "SET_CHAR_PROOFS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},           // 0x2AB
    {true, "SET_CAR_PROOFS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},             // 0x2AC
    {false, "IS_PLAYER_IN_ANGLED_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                       // 0x2AD
    {false, "IS_PLAYER_IN_ANGLED_AREA_ON_FOOT_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},            // 0x2AE
    {false, "IS_PLAYER_IN_ANGLED_AREA_IN_CAR_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},            // 0x2AF
    {false, "IS_PLAYER_STOPPED_IN_ANGLED_AREA_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},            // 0x2B0
    {false, "IS_PLAYER_STOPPED_IN_ANGLED_AREA_ON_FOOT_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},            // 0x2B1
    {false, "IS_PLAYER_STOPPED_IN_ANGLED_AREA_IN_CAR_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_PLAYER}},            // 0x2B2
    {false, "IS_PLAYER_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},  // 0x2B3
    {false, "IS_PLAYER_IN_ANGLED_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},  // 0x2B4
    {false, "IS_PLAYER_IN_ANGLED_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},  // 0x2B5
    {false, "IS_PLAYER_STOPPED_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},  // 0x2B6
    {false, "IS_PLAYER_STOPPED_IN_ANGLED_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},  // 0x2B7
    {false, "IS_PLAYER_STOPPED_IN_ANGLED_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                // 0x2B8
    {true, "DEACTIVATE_GARAGE", {ARG_TEXT_LABEL}, {}},                                                 // 0x2B9
    {false, "GET_NUMBER_OF_CARS_COLLECTED_BY_GARAGE", {ARG_TEXT_LABEL, ARG_RETURN_INT}, {}},           // 0x2BA
    {false, "HAS_CAR_BEEN_TAKEN_TO_GARAGE", {ARG_TEXT_LABEL, ARG_INT}, {}},                            // 0x2BB
    {false, "SET_SWAT_REQUIRED", {ARG_INT}, {}},                                                       // 0x2BC
    {false, "SET_FBI_REQUIRED", {ARG_INT}, {}},                                                        // 0x2BD
    {false, "SET_ARMY_REQUIRED", {ARG_INT}, {}},                                                       // 0x2BE
    {true, "IS_CAR_IN_WATER", {ARG_INT}, {TYPE_CAR}},                                                  // 0x2BF
    {true, "GET_CLOSEST_CHAR_NODE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x2C0
    {true, "GET_CLOSEST_CAR_NODE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x2C1
    {true, "CAR_GOTO_COORDINATES_ACCURATE", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},   // 0x2C2
    {false, "START_PACMAN_RACE", {ARG_INT}, {}},                                                       // 0x2C3
    {false, "START_PACMAN_RECORD", {}, {}},                                                            // 0x2C4
    {false, "GET_NUMBER_OF_POWER_PILLS_EATEN", {ARG_RETURN_INT}, {}},                                  // 0x2C5
    {false, "CLEAR_PACMAN", {}, {}},                                                                   // 0x2C6
    {false, "START_PACMAN_SCRAMBLE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},       // 0x2C7
    {false, "GET_NUMBER_OF_POWER_PILLS_CARRIED", {ARG_RETURN_INT}, {}},                                // 0x2C8
    {false, "CLEAR_NUMBER_OF_POWER_PILLS_CARRIED", {}, {}},                                            // 0x2C9
    {true, "IS_CAR_ON_SCREEN", {ARG_INT}, {TYPE_CAR}},                                                 // 0x2CA
    {true, "IS_CHAR_ON_SCREEN", {ARG_INT}, {TYPE_CHAR}},                                               // 0x2CB
    {true, "IS_OBJECT_ON_SCREEN", {ARG_INT}, {TYPE_OBJECT}},                                           // 0x2CC
    {false, "GOSUB_FILE", {ARG_LABEL, ARG_LABEL}, {}},                                                 // 0x2CD
    {true, "GET_GROUND_Z_FOR_3D_COORD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT}, {}},      // 0x2CE
    {true, "START_SCRIPT_FIRE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_SCRIPT_FIRE}},                                                          // 0x2CF
    {true, "IS_SCRIPT_FIRE_EXTINGUISHED", {ARG_INT}, {TYPE_SCRIPT_FIRE}},                // 0x2D0
    {true, "REMOVE_SCRIPT_FIRE", {ARG_INT}, {TYPE_SCRIPT_FIRE}},                         // 0x2D1
    {false, "SET_COMEDY_CONTROLS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                      // 0x2D2
    {true, "BOAT_GOTO_COORDS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},  // 0x2D3
    {true, "BOAT_STOP", {ARG_INT}, {TYPE_CAR}},                                          // 0x2D4
    {false, "IS_PLAYER_SHOOTING_IN_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                  // 0x2D5
    {true, "IS_CHAR_SHOOTING_IN_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                    // 0x2D6
    {false, "IS_CURRENT_PLAYER_WEAPON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},              // 0x2D7
    {true, "IS_CURRENT_CHAR_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                   // 0x2D8
    {false, "CLEAR_NUMBER_OF_POWER_PILLS_EATEN", {}, {}},                                // 0x2D9
    {false, "ADD_POWER_PILL", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                    // 0x2DA
    {true, "SET_BOAT_CRUISE_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                   // 0x2DB
    {false, "GET_RANDOM_CHAR_IN_AREA",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                                                     // 0x2DC
    {true, "GET_RANDOM_CHAR_IN_ZONE", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},  // 0x2DD
    {false, "IS_PLAYER_IN_TAXI", {ARG_INT}, {TYPE_PLAYER}},                                    // 0x2DE
    {false, "IS_PLAYER_SHOOTING", {ARG_INT}, {TYPE_PLAYER}},                                   // 0x2DF
    {true, "IS_CHAR_SHOOTING", {ARG_INT}, {TYPE_CHAR}},                                        // 0x2E0
    {true, "CREATE_MONEY_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_PICKUP}},                                                                                    // 0x2E1
    {true, "SET_CHAR_ACCURACY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                             // 0x2E2
    {true, "GET_CAR_SPEED", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                         // 0x2E3
    {true, "LOAD_CUTSCENE", {ARG_TEXT_LABEL}, {}},                                                            // 0x2E4
    {false, "CREATE_CUTSCENE_OBJECT", {ARG_INT, ARG_RETURN_INT}, {TYPE_UNIMPORTANT, TYPE_OBJECT}},            // 0x2E5
    {false, "SET_CUTSCENE_ANIM", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_OBJECT}},                                   // 0x2E6
    {true, "START_CUTSCENE", {}, {}},                                                                         // 0x2E7
    {true, "GET_CUTSCENE_TIME", {ARG_RETURN_INT}, {}},                                                        // 0x2E8
    {true, "HAS_CUTSCENE_FINISHED", {}, {}},                                                                  // 0x2E9
    {true, "CLEAR_CUTSCENE", {}, {}},                                                                         // 0x2EA
    {true, "RESTORE_CAMERA_JUMPCUT", {}, {}},                                                                 // 0x2EB
    {true, "CREATE_COLLECTABLE1", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                     // 0x2EC
    {true, "SET_COLLECTABLE1_TOTAL", {ARG_INT}, {}},                                                          // 0x2ED
    {true, "IS_PROJECTILE_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x2EE
    {false, "DESTROY_PROJECTILES_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                  // 0x2EF
    {false, "DROP_MINE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                              // 0x2F0
    {false, "DROP_NAUTICAL_MINE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                     // 0x2F1
    {true, "IS_CHAR_MODEL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                 // 0x2F2
    {false, "LOAD_SPECIAL_MODEL", {ARG_INT, ARG_TEXT_LABEL}, {}},                                             // 0x2F3
    {false, "CREATE_CUTSCENE_HEAD", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_OBJECT}},                                                        // 0x2F4
    {false, "SET_CUTSCENE_HEAD_ANIM", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_OBJECT}},                              // 0x2F5
    {true, "SIN", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                                         // 0x2F6
    {true, "COS", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                                         // 0x2F7
    {true, "GET_CAR_FORWARD_X", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                     // 0x2F8
    {true, "GET_CAR_FORWARD_Y", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                     // 0x2F9
    {true, "CHANGE_GARAGE_TYPE", {ARG_TEXT_LABEL, ARG_INT}, {}},                                              // 0x2FA
    {false, "ACTIVATE_CRUSHER_CRANE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x2FB
    {false, "PRINT_WITH_2_NUMBERS", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                  // 0x2FC
    {true, "PRINT_WITH_2_NUMBERS_NOW", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},               // 0x2FD
    {false, "PRINT_WITH_2_NUMBERS_SOON", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},             // 0x2FE
    {true, "PRINT_WITH_3_NUMBERS", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},          // 0x2FF
    {false, "PRINT_WITH_3_NUMBERS_NOW", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},     // 0x300
    {false, "PRINT_WITH_3_NUMBERS_SOON", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},    // 0x301
    {true, "PRINT_WITH_4_NUMBERS", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x302
    {true, "PRINT_WITH_4_NUMBERS_NOW", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x303
    {false, "PRINT_WITH_4_NUMBERS_SOON", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x304
    {false, "PRINT_WITH_5_NUMBERS", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x305
    {false, "PRINT_WITH_5_NUMBERS_NOW", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x306
    {false, "PRINT_WITH_5_NUMBERS_SOON",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                   // 0x307
    {true, "PRINT_WITH_6_NUMBERS",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},          // 0x308
    {false, "PRINT_WITH_6_NUMBERS_NOW",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},          // 0x309
    {false, "PRINT_WITH_6_NUMBERS_SOON",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},          // 0x30A
    {false, "SET_CHAR_OBJ_FOLLOW_CHAR_IN_FORMATION", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},      // 0x30B
    {true, "PLAYER_MADE_PROGRESS", {ARG_INT_CONST}, {}},                                                        // 0x30C
    {true, "SET_PROGRESS_TOTAL", {ARG_INT_CONST}, {}},                                                          // 0x30D
    {false, "REGISTER_JUMP_DISTANCE", {ARG_FLOAT}, {}},                                                         // 0x30E
    {false, "REGISTER_JUMP_HEIGHT", {ARG_FLOAT}, {}},                                                           // 0x30F
    {false, "REGISTER_JUMP_FLIPS", {ARG_INT}, {}},                                                              // 0x310
    {false, "REGISTER_JUMP_SPINS", {ARG_INT}, {}},                                                              // 0x311
    {false, "REGISTER_JUMP_STUNT", {ARG_INT}, {}},                                                              // 0x312
    {false, "REGISTER_UNIQUE_JUMP_FOUND", {}, {}},                                                              // 0x313
    {false, "SET_UNIQUE_JUMPS_TOTAL", {ARG_INT}, {}},                                                           // 0x314
    {false, "REGISTER_PASSENGER_DROPPED_OFF_TAXI", {}, {}},                                                     // 0x315
    {false, "REGISTER_MONEY_MADE_TAXI", {ARG_INT}, {}},                                                         // 0x316
    {true, "REGISTER_MISSION_GIVEN", {}, {}},                                                                   // 0x317
    {true, "REGISTER_MISSION_PASSED", {ARG_TEXT_LABEL}, {}},                                                    // 0x318
    {false, "SET_CHAR_RUNNING", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                               // 0x319
    {true, "REMOVE_ALL_SCRIPT_FIRES", {}, {}},                                                                  // 0x31A
    {false, "IS_FIRST_CAR_COLOUR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                             // 0x31B
    {false, "IS_SECOND_CAR_COLOUR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x31C
    {true, "HAS_CHAR_BEEN_DAMAGED_BY_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                 // 0x31D
    {true, "HAS_CAR_BEEN_DAMAGED_BY_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x31E
    {false, "IS_CHAR_IN_CHARS_GROUP", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                              // 0x31F
    {false, "IS_CHAR_IN_PLAYERS_GROUP", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},                          // 0x320
    {true, "EXPLODE_CHAR_HEAD", {ARG_INT}, {TYPE_CHAR}},                                                        // 0x321
    {false, "EXPLODE_PLAYER_HEAD", {ARG_INT}, {TYPE_PLAYER}},                                                   // 0x322
    {true, "ANCHOR_BOAT", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                      // 0x323
    {false, "SET_ZONE_GROUP", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                                          // 0x324
    {true, "START_CAR_FIRE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_SCRIPT_FIRE}},                          // 0x325
    {true, "START_CHAR_FIRE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_SCRIPT_FIRE}},                        // 0x326
    {true, "GET_RANDOM_CAR_OF_TYPE_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x327
    {false, "GET_RANDOM_CAR_OF_TYPE_IN_ZONE", {ARG_TEXT_LABEL, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},                                                        // 0x328
    {false, "HAS_RESPRAY_HAPPENED", {ARG_TEXT_LABEL}, {}},                                                      // 0x329
    {true, "SET_CAMERA_ZOOM", {ARG_INT}, {}},                                                                   // 0x32A
    {true, "CREATE_PICKUP_WITH_AMMO", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_PICKUP}},                                                     // 0x32B
    {false, "SET_CAR_RAM_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},      // 0x32C
    {false, "SET_CAR_BLOCK_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},    // 0x32D
    {false, "SET_CHAR_OBJ_CATCH_TRAIN", {ARG_INT}, {TYPE_CHAR}},               // 0x32E
    {false, "SET_COLL_OBJ_CATCH_TRAIN", {ARG_INT}, {TYPE_COLL_OBJ}},           // 0x32F
    {true, "SET_PLAYER_NEVER_GETS_TIRED", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},  // 0x330
    {true, "SET_PLAYER_FAST_RELOAD", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},       // 0x331
    {true, "SET_CHAR_BLEEDING", {ARG_INT, ARG_INT}, {TYPE_CHAR}},              // 0x332
    {false, "SET_CAR_FUNNY_SUSPENSION", {ARG_INT, ARG_INT}, {TYPE_CAR}},       // 0x333
    {false, "SET_CAR_BIG_WHEELS", {ARG_INT, ARG_INT}, {TYPE_CAR}},             // 0x334
    {true, "SET_FREE_RESPRAYS", {ARG_INT}, {}},                                // 0x335
    {false, "SET_PLAYER_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},          // 0x336
    {true, "SET_CHAR_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},               // 0x337
    {true, "SET_CAR_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                 // 0x338
    {true, "IS_AREA_OCCUPIED",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                 // 0x339
    {false, "START_DRUG_RUN", {}, {}},                                                                       // 0x33A
    {false, "HAS_DRUG_RUN_BEEN_COMPLETED", {}, {}},                                                          // 0x33B
    {false, "HAS_DRUG_PLANE_BEEN_SHOT_DOWN", {}, {}},                                                        // 0x33C
    {false, "SAVE_PLAYER_FROM_FIRES", {ARG_INT}, {TYPE_PLAYER}},                                             // 0x33D
    {true, "DISPLAY_TEXT", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL}, {}},                                      // 0x33E
    {true, "SET_TEXT_SCALE", {ARG_FLOAT, ARG_FLOAT}, {}},                                                    // 0x33F
    {true, "SET_TEXT_COLOUR", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                                     // 0x340
    {true, "SET_TEXT_JUSTIFY", {ARG_INT}, {}},                                                               // 0x341
    {true, "SET_TEXT_CENTRE", {ARG_INT}, {}},                                                                // 0x342
    {true, "SET_TEXT_WRAPX", {ARG_FLOAT}, {}},                                                               // 0x343
    {true, "SET_TEXT_CENTRE_SIZE", {ARG_FLOAT}, {}},                                                         // 0x344
    {true, "SET_TEXT_BACKGROUND", {ARG_INT}, {}},                                                            // 0x345
    {false, "SET_TEXT_BACKGROUND_COLOUR", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                         // 0x346
    {false, "SET_TEXT_BACKGROUND_ONLY_TEXT", {ARG_INT}, {}},                                                 // 0x347
    {true, "SET_TEXT_PROPORTIONAL", {ARG_INT}, {}},                                                          // 0x348
    {true, "SET_TEXT_FONT", {ARG_INT}, {}},                                                                  // 0x349
    {false, "INDUSTRIAL_PASSED", {}, {}},                                                                    // 0x34A
    {false, "COMMERCIAL_PASSED", {}, {}},                                                                    // 0x34B
    {false, "SUBURBAN_PASSED", {}, {}},                                                                      // 0x34C
    {true, "ROTATE_OBJECT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_OBJECT}},                        // 0x34D
    {true, "SLIDE_OBJECT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},                                                                                      // 0x34E
    {true, "REMOVE_CHAR_ELEGANTLY", {ARG_INT}, {TYPE_CHAR}},                                                 // 0x34F
    {true, "SET_CHAR_STAY_IN_SAME_PLACE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x350
    {false, "IS_NASTY_GAME", {}, {}},                                                                        // 0x351
    {false, "UNDRESS_CHAR", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_CHAR}},                                         // 0x352
    {false, "DRESS_CHAR", {ARG_INT}, {TYPE_CHAR}},                                                           // 0x353
    {false, "START_CHASE_SCENE", {ARG_FLOAT}, {}},                                                           // 0x354
    {false, "STOP_CHASE_SCENE", {}, {}},                                                                     // 0x355
    {true, "IS_EXPLOSION_IN_AREA", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                 // 0x356
    {false, "IS_EXPLOSION_IN_ZONE", {ARG_INT, ARG_TEXT_LABEL}, {}},                                          // 0x357
    {false, "START_DRUG_DROP_OFF", {}, {}},                                                                  // 0x358
    {false, "HAS_DROP_OFF_PLANE_BEEN_SHOT_DOWN", {}, {}},                                                    // 0x359
    {false, "FIND_DROP_OFF_PLANE_COORDINATES", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x35A
    {false, "CREATE_FLOATING_PACKAGE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_PICKUP}},                                // 0x35B
    {true, "PLACE_OBJECT_RELATIVE_TO_CAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_OBJECT, TYPE_CAR}},                                                                            // 0x35C
    {true, "MAKE_OBJECT_TARGETTABLE", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                    // 0x35D
    {false, "ADD_ARMOUR_TO_PLAYER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                      // 0x35E
    {true, "ADD_ARMOUR_TO_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x35F
    {true, "OPEN_GARAGE", {ARG_TEXT_LABEL}, {}},                                                             // 0x360
    {true, "CLOSE_GARAGE", {ARG_TEXT_LABEL}, {}},                                                            // 0x361
    {true, "WARP_CHAR_FROM_CAR_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},          // 0x362
    {true, "SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {}},                                                                                                 // 0x363
    {true, "HAS_CHAR_SPOTTED_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                             // 0x364
    {false, "SET_CHAR_OBJ_HAIL_TAXI", {ARG_INT}, {TYPE_CHAR}},                                               // 0x365
    {true, "HAS_OBJECT_BEEN_DAMAGED", {ARG_INT}, {TYPE_OBJECT}},                                             // 0x366
    {false, "START_KILL_FRENZY_HEADSHOT",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},       // 0x367
    {false, "ACTIVATE_MILITARY_CRANE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                   // 0x368
    {false, "WARP_PLAYER_INTO_CAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CAR}},                              // 0x369
    {true, "WARP_CHAR_INTO_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                                   // 0x36A
    {false, "SWITCH_CAR_RADIO", {ARG_INT}, {}},                                                                // 0x36B
    {false, "SET_AUDIO_STREAM", {ARG_INT}, {}},                                                                // 0x36C
    {true, "PRINT_WITH_2_NUMBERS_BIG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},              // 0x36D
    {false, "PRINT_WITH_3_NUMBERS_BIG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},    // 0x36E
    {false, "PRINT_WITH_4_NUMBERS_BIG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                   // 0x36F
    {false, "PRINT_WITH_5_NUMBERS_BIG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                   // 0x370
    {false, "PRINT_WITH_6_NUMBERS_BIG",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},         // 0x371
    {false, "SET_CHAR_WAIT_STATE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x372
    {true, "SET_CAMERA_BEHIND_PLAYER", {}, {}},                                                                // 0x373
    {false, "SET_MOTION_BLUR", {ARG_INT}, {}},                                                                 // 0x374
    {false, "PRINT_STRING_IN_STRING", {ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                 // 0x375
    {true, "CREATE_RANDOM_CHAR", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},                                    // 0x376
    {false, "SET_CHAR_OBJ_STEAL_ANY_CAR", {ARG_INT}, {TYPE_CHAR}},                                             // 0x377
    {false, "SET_2_REPEATED_PHONE_MESSAGES", {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL}, {TYPE_PHONE}},         // 0x378
    {false, "SET_2_PHONE_MESSAGES", {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL}, {TYPE_PHONE}},                  // 0x379
    {false, "SET_3_REPEATED_PHONE_MESSAGES", {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},                                                                                         // 0x37A
    {false, "SET_3_PHONE_MESSAGES", {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL}, {TYPE_PHONE}},  // 0x37B
    {false, "SET_4_REPEATED_PHONE_MESSAGES", {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},                                                                                         // 0x37C
    {false, "SET_4_PHONE_MESSAGES", {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},                                                                                         // 0x37D
    {false, "IS_SNIPER_BULLET_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                   // 0x37E
    {false, "GIVE_PLAYER_DETONATOR", {}, {}},                                                                  // 0x37F
    {false, "SET_COLL_OBJ_STEAL_ANY_CAR", {ARG_INT}, {TYPE_COLL_OBJ}},                                         // 0x380
    {true, "SET_OBJECT_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},                  // 0x381
    {true, "SET_OBJECT_COLLISION", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                         // 0x382
    {false, "IS_ICECREAM_JINGLE_ON", {ARG_INT}, {TYPE_CAR}},                                                   // 0x383
    {true, "PRINT_STRING_IN_STRING_NOW", {ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},              // 0x384
    {false, "PRINT_STRING_IN_STRING_SOON", {ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},            // 0x385
    {false, "SET_5_REPEATED_PHONE_MESSAGES",
        {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},  // 0x386
    {false, "SET_5_PHONE_MESSAGES",
        {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},  // 0x387
    {false, "SET_6_REPEATED_PHONE_MESSAGES",
        {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},  // 0x388
    {false, "SET_6_PHONE_MESSAGES",
        {ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {TYPE_PHONE}},                                                                                          // 0x389
    {true, "IS_POINT_OBSCURED_BY_A_MISSION_ENTITY", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x38A
    {true, "LOAD_ALL_MODELS_NOW", {}, {}},                                                                      // 0x38B
    {true, "ADD_TO_OBJECT_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},                // 0x38C
    {true, "DRAW_SPRITE", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x38D
    {true, "DRAW_RECT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},  // 0x38E
    {true, "LOAD_SPRITE", {ARG_INT, ARG_TEXT_LABEL16}, {}},                                                     // 0x38F
    {true, "LOAD_TEXTURE_DICTIONARY", {ARG_TEXT_LABEL}, {}},                                                    // 0x390
    {true, "REMOVE_TEXTURE_DICTIONARY", {}, {}},                                                                // 0x391
    {true, "SET_OBJECT_DYNAMIC", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                            // 0x392
    {true, "SET_CHAR_ANIM_SPEED", {ARG_INT, ARG_TEXT_LABEL16, ARG_FLOAT}, {TYPE_CHAR}},                         // 0x393
    {true, "PLAY_MISSION_PASSED_TUNE", {ARG_INT}, {}},                                                          // 0x394
    {true, "CLEAR_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                            // 0x395
    {true, "FREEZE_ONSCREEN_TIMER", {ARG_INT}, {}},                                                             // 0x396
    {true, "SWITCH_CAR_SIREN", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                 // 0x397
    {false, "SWITCH_PED_ROADS_ON_ANGLED", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x398
    {false, "SWITCH_PED_ROADS_OFF_ANGLED",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                     // 0x399
    {false, "SWITCH_ROADS_ON_ANGLED", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x39A
    {false, "SWITCH_ROADS_OFF_ANGLED", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x39B
    {true, "SET_CAR_WATERTIGHT", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0x39C
    {false, "ADD_MOVING_PARTICLE_EFFECT",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT,
            ARG_INT, ARG_INT},
        {}},                                                                                                    // 0x39D
    {true, "SET_CHAR_CANT_BE_DRAGGED_OUT", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                    // 0x39E
    {true, "TURN_CAR_TO_FACE_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},                              // 0x39F
    {false, "IS_CRANE_LIFTING_CAR", {ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},                                                        // 0x3A0
    {true, "DRAW_SPHERE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                    // 0x3A1
    {true, "SET_CAR_STATUS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                   // 0x3A2
    {true, "IS_CHAR_MALE", {ARG_INT}, {TYPE_CHAR}},                                                             // 0x3A3
    {true, "SCRIPT_NAME", {ARG_TEXT_LABEL}, {}},                                                                // 0x3A4
    {false, "CHANGE_GARAGE_TYPE_WITH_CAR_MODEL", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                       // 0x3A5
    {false, "FIND_DRUG_PLANE_COORDINATES", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},         // 0x3A6
    {true, "SAVE_INT_TO_DEBUG_FILE", {ARG_INT}, {}},                                                            // 0x3A7
    {true, "SAVE_FLOAT_TO_DEBUG_FILE", {ARG_FLOAT}, {}},                                                        // 0x3A8
    {true, "SAVE_NEWLINE_TO_DEBUG_FILE", {}, {}},                                                               // 0x3A9
    {true, "POLICE_RADIO_MESSAGE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                      // 0x3AA
    {true, "SET_CAR_STRONG", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                   // 0x3AB
    {false, "REMOVE_ROUTE", {ARG_INT}, {}},                                                                     // 0x3AC
    {true, "SWITCH_RUBBISH", {ARG_INT}, {}},                                                                    // 0x3AD
    {false, "REMOVE_PARTICLE_EFFECTS_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x3AE
    {true, "SWITCH_STREAMING", {ARG_INT}, {}},                                                                  // 0x3AF
    {true, "IS_GARAGE_OPEN", {ARG_TEXT_LABEL}, {}},                                                             // 0x3B0
    {true, "IS_GARAGE_CLOSED", {ARG_TEXT_LABEL}, {}},                                                           // 0x3B1
    {false, "START_CATALINA_HELI", {}, {}},                                                                     // 0x3B2
    {false, "CATALINA_HELI_TAKE_OFF", {}, {}},                                                                  // 0x3B3
    {false, "REMOVE_CATALINA_HELI", {}, {}},                                                                    // 0x3B4
    {false, "HAS_CATALINA_HELI_BEEN_SHOT_DOWN", {}, {}},                                                        // 0x3B5
    {true, "SWAP_NEAREST_BUILDING_MODEL", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},  // 0x3B6
    {true, "SWITCH_WORLD_PROCESSING", {ARG_INT}, {}},                                                           // 0x3B7
    {false, "REMOVE_ALL_PLAYER_WEAPONS", {ARG_INT}, {TYPE_PLAYER}},                                             // 0x3B8
    {false, "GRAB_CATALINA_HELI", {ARG_RETURN_INT}, {TYPE_CAR}},                                                // 0x3B9
    {true, "CLEAR_AREA_OF_CARS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},       // 0x3BA
    {false, "SET_ROTATING_GARAGE_DOOR", {ARG_TEXT_LABEL}, {}},                                                  // 0x3BB
    {true, "ADD_SPHERE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_SPHERE}},                 // 0x3BC
    {true, "REMOVE_SPHERE", {ARG_INT}, {TYPE_SPHERE}},                                                          // 0x3BD
    {false, "CATALINA_HELI_FLY_AWAY", {}, {}},                                                                  // 0x3BE
    {true, "SET_EVERYONE_IGNORE_PLAYER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                    // 0x3BF
    {true, "STORE_CAR_CHAR_IS_IN_NO_SAVE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_CAR}},                   // 0x3C0
    {false, "STORE_CAR_PLAYER_IS_IN_NO_SAVE", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER, TYPE_CAR}},              // 0x3C1
    {false, "IS_PHONE_DISPLAYING_MESSAGE", {ARG_INT}, {TYPE_PHONE}},                                            // 0x3C2
    {true, "DISPLAY_ONSCREEN_TIMER_WITH_STRING", {ARG_VAR_INT, ARG_INT, ARG_TEXT_LABEL}, {}},                   // 0x3C3
    {true, "DISPLAY_ONSCREEN_COUNTER_WITH_STRING", {ARG_VAR_INT, ARG_INT, ARG_TEXT_LABEL}, {}},                 // 0x3C4
    {true, "CREATE_RANDOM_CAR_FOR_CAR_PARK", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                 // 0x3C5
    {false, "IS_COLLISION_IN_MEMORY", {ARG_INT}, {}},                                                           // 0x3C6
    {true, "SET_WANTED_MULTIPLIER", {ARG_FLOAT}, {}},                                                           // 0x3C7
    {true, "SET_CAMERA_IN_FRONT_OF_PLAYER", {}, {}},                                                            // 0x3C8
    {true, "IS_CAR_VISIBLY_DAMAGED", {ARG_INT}, {TYPE_CAR}},                                                    // 0x3C9
    {true, "DOES_OBJECT_EXIST", {ARG_INT}, {TYPE_OBJECT}},                                                      // 0x3CA
    {true, "LOAD_SCENE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                                // 0x3CB
    {true, "ADD_STUCK_CAR_CHECK", {ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},                                   // 0x3CC
    {true, "REMOVE_STUCK_CAR_CHECK", {ARG_INT}, {TYPE_CAR}},                                                    // 0x3CD
    {true, "IS_CAR_STUCK", {ARG_INT}, {TYPE_CAR}},                                                              // 0x3CE
    {true, "LOAD_MISSION_AUDIO", {ARG_INT, ARG_INT}, {}},                                                       // 0x3CF
    {true, "HAS_MISSION_AUDIO_LOADED", {ARG_INT}, {}},                                                          // 0x3D0
    {true, "PLAY_MISSION_AUDIO", {ARG_INT}, {}},                                                                // 0x3D1
    {true, "HAS_MISSION_AUDIO_FINISHED", {ARG_INT}, {}},                                                        // 0x3D2
    {true, "GET_CLOSEST_CAR_NODE_WITH_HEADING",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {}},                                                                               // 0x3D3
    {false, "HAS_IMPORT_GARAGE_SLOT_BEEN_FILLED", {ARG_TEXT_LABEL, ARG_INT}, {}},          // 0x3D4
    {true, "CLEAR_THIS_PRINT", {ARG_TEXT_LABEL}, {}},                                      // 0x3D5
    {true, "CLEAR_THIS_BIG_PRINT", {ARG_TEXT_LABEL}, {}},                                  // 0x3D6
    {true, "SET_MISSION_AUDIO_POSITION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x3D7
    {true, "ACTIVATE_SAVE_MENU", {}, {}},                                                  // 0x3D8
    {true, "HAS_SAVE_GAME_FINISHED", {}, {}},                                              // 0x3D9
    {false, "NO_SPECIAL_CAMERA_FOR_THIS_GARAGE", {ARG_TEXT_LABEL}, {}},                    // 0x3DA
    {false, "ADD_BLIP_FOR_PICKUP_OLD", {ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_PICKUP, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                     // 0x3DB
    {true, "ADD_BLIP_FOR_PICKUP", {ARG_INT, ARG_RETURN_INT}, {TYPE_PICKUP, TYPE_BLIP}},    // 0x3DC
    {false, "ADD_SPRITE_BLIP_FOR_PICKUP", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_PICKUP, TYPE_UNIMPORTANT, TYPE_BLIP}},                                       // 0x3DD
    {true, "SET_PED_DENSITY_MULTIPLIER", {ARG_FLOAT}, {}},                                 // 0x3DE
    {false, "FORCE_RANDOM_PED_TYPE", {ARG_INT}, {}},                                       // 0x3DF
    {true, "SET_TEXT_DRAW_BEFORE_FADE", {ARG_INT}, {}},                                    // 0x3E0
    {false, "GET_COLLECTABLE1S_COLLECTED", {ARG_RETURN_INT}, {}},                          // 0x3E1
    {false, "SET_CHAR_OBJ_LEAVE_ANY_CAR", {ARG_INT}, {TYPE_CHAR}},                         // 0x3E2
    {true, "SET_SPRITES_DRAW_BEFORE_FADE", {ARG_INT}, {}},                                 // 0x3E3
    {true, "SET_TEXT_RIGHT_JUSTIFY", {ARG_INT}, {}},                                       // 0x3E4
    {true, "PRINT_HELP", {ARG_TEXT_LABEL}, {}},                                            // 0x3E5
    {true, "CLEAR_HELP", {}, {}},                                                          // 0x3E6
    {true, "FLASH_HUD_OBJECT", {ARG_INT}, {}},                                             // 0x3E7
    {false, "FLASH_RADAR_BLIP", {ARG_INT}, {TYPE_BLIP}},                                   // 0x3E8
    {false, "IS_CHAR_IN_CONTROL", {ARG_INT}, {TYPE_CHAR}},                                 // 0x3E9
    {false, "SET_GENERATE_CARS_AROUND_CAMERA", {ARG_INT}, {}},                             // 0x3EA
    {true, "CLEAR_SMALL_PRINTS", {}, {}},                                                  // 0x3EB
    {false, "HAS_MILITARY_CRANE_COLLECTED_ALL_CARS", {}, {}},                              // 0x3EC
    {true, "SET_UPSIDEDOWN_CAR_NOT_DAMAGED", {ARG_INT, ARG_INT}, {TYPE_CAR}},              // 0x3ED
    {true, "CAN_PLAYER_START_MISSION", {ARG_INT}, {TYPE_PLAYER}},                          // 0x3EE
    {true, "MAKE_PLAYER_SAFE_FOR_CUTSCENE", {ARG_INT}, {TYPE_PLAYER}},                     // 0x3EF
    {true, "USE_TEXT_COMMANDS", {ARG_INT}, {}},                                            // 0x3F0
    {false, "SET_THREAT_FOR_PED_TYPE", {ARG_INT, ARG_INT}, {}},                            // 0x3F1
    {false, "CLEAR_THREAT_FOR_PED_TYPE", {ARG_INT, ARG_INT}, {}},                          // 0x3F2
    {true, "GET_CAR_COLOURS", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {TYPE_CAR}},      // 0x3F3
    {true, "SET_ALL_CARS_CAN_BE_DAMAGED", {ARG_INT}, {}},                                  // 0x3F4
    {true, "SET_CAR_CAN_BE_DAMAGED", {ARG_INT, ARG_INT}, {TYPE_CAR}},                      // 0x3F5
    {false, "MAKE_PLAYER_UNSAFE", {ARG_INT}, {TYPE_PLAYER}},                               // 0x3F6
    {false, "LOAD_COLLISION", {ARG_INT}, {}},                                              // 0x3F7
    {false, "GET_BODY_CAST_HEALTH", {ARG_RETURN_INT}, {}},                                 // 0x3F8
    {false, "SET_CHARS_CHATTING", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},    // 0x3F9
    {false, "MAKE_PLAYER_SAFE", {ARG_INT}, {TYPE_PLAYER}},                                 // 0x3FA
    {false, "SET_CAR_STAYS_IN_CURRENT_LEVEL", {ARG_INT, ARG_INT}, {TYPE_CAR}},             // 0x3FB
    {false, "SET_CHAR_STAYS_IN_CURRENT_LEVEL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},           // 0x3FC
    {true, "SET_DRUNK_INPUT_DELAY", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                    // 0x3FD
    {true, "SET_CHAR_MONEY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                             // 0x3FE
    {false, "INCREASE_CHAR_MONEY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                       // 0x3FF
    {true, "GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_OBJECT}},                                      // 0x400
    {false, "REGISTER_LIFE_SAVED", {}, {}},                  // 0x401
    {false, "REGISTER_CRIMINAL_CAUGHT", {}, {}},             // 0x402
    {false, "REGISTER_AMBULANCE_LEVEL", {ARG_INT}, {}},      // 0x403
    {false, "REGISTER_FIRE_EXTINGUISHED", {}, {}},           // 0x404
    {false, "TURN_PHONE_ON", {ARG_INT}, {TYPE_PHONE}},       // 0x405
    {false, "REGISTER_LONGEST_DODO_FLIGHT", {ARG_INT}, {}},  // 0x406
    {true, "GET_OFFSET_FROM_CAR_IN_WORLD_COORDS",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CAR}},                                                                                        // 0x407
    {false, "SET_TOTAL_NUMBER_OF_KILL_FRENZIES", {ARG_INT}, {}},                                            // 0x408
    {false, "BLOW_UP_RC_BUGGY", {}, {}},                                                                    // 0x409
    {false, "REMOVE_CAR_FROM_CHASE", {ARG_INT}, {}},                                                        // 0x40A
    {false, "IS_FRENCH_GAME", {}, {}},                                                                      // 0x40B
    {true, "IS_GERMAN_GAME", {}, {}},                                                                       // 0x40C
    {true, "CLEAR_MISSION_AUDIO", {ARG_INT}, {}},                                                           // 0x40D
    {false, "SET_FADE_IN_AFTER_NEXT_ARREST", {ARG_INT}, {}},                                                // 0x40E
    {false, "SET_FADE_IN_AFTER_NEXT_DEATH", {ARG_INT}, {}},                                                 // 0x40F
    {false, "SET_GANG_PED_MODEL_PREFERENCE", {ARG_INT, ARG_INT}, {}},                                       // 0x410
    {false, "SET_CHAR_USE_PEDNODE_SEEK", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x411
    {false, "SWITCH_VEHICLE_WEAPONS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                      // 0x412
    {false, "SET_GET_OUT_OF_JAIL_FREE", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x413
    {true, "SET_FREE_HEALTH_CARE", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                      // 0x414
    {false, "IS_CAR_DOOR_CLOSED", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                          // 0x415
    {true, "LOAD_AND_LAUNCH_MISSION", {ARG_LABEL}, {}},                                                     // 0x416
    {true, "LOAD_AND_LAUNCH_MISSION_INTERNAL", {ARG_INT}, {}},                                              // 0x417
    {true, "SET_OBJECT_DRAW_LAST", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                      // 0x418
    {false, "GET_AMMO_IN_PLAYER_WEAPON", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                // 0x419
    {true, "GET_AMMO_IN_CHAR_WEAPON", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                     // 0x41A
    {false, "REGISTER_KILL_FRENZY_PASSED", {}, {}},                                                         // 0x41B
    {false, "SET_CHAR_SAY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                               // 0x41C
    {true, "SET_NEAR_CLIP", {ARG_FLOAT}, {}},                                                               // 0x41D
    {true, "SET_RADIO_CHANNEL", {ARG_INT}, {}},                                                             // 0x41E
    {false, "OVERRIDE_HOSPITAL_LEVEL", {ARG_INT}, {}},                                                      // 0x41F
    {false, "OVERRIDE_POLICE_STATION_LEVEL", {ARG_INT}, {}},                                                // 0x420
    {false, "FORCE_RAIN", {ARG_INT}, {}},                                                                   // 0x421
    {false, "DOES_GARAGE_CONTAIN_CAR", {ARG_TEXT_LABEL, ARG_INT}, {TYPE_UNIMPORTANT, TYPE_CAR}},            // 0x422
    {true, "SET_CAR_TRACTION", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                           // 0x423
    {true, "ARE_MEASUREMENTS_IN_METRES", {}, {}},                                                           // 0x424
    {true, "CONVERT_METRES_TO_FEET", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                    // 0x425
    {false, "MARK_ROADS_BETWEEN_LEVELS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                // 0x426
    {false, "MARK_PED_ROADS_BETWEEN_LEVELS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                // 0x427
    {true, "SET_CAR_AVOID_LEVEL_TRANSITIONS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                              // 0x428
    {false, "SET_CHAR_AVOID_LEVEL_TRANSITIONS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                           // 0x429
    {false, "IS_THREAT_FOR_PED_TYPE", {ARG_INT, ARG_INT}, {}},                                              // 0x42A
    {true, "CLEAR_AREA_OF_CHARS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x42B
    {true, "SET_TOTAL_NUMBER_OF_MISSIONS", {ARG_INT}, {}},                                                  // 0x42C
    {true, "CONVERT_METRES_TO_FEET_INT", {ARG_INT, ARG_RETURN_INT}, {}},                                    // 0x42D
    {true, "REGISTER_FASTEST_TIME", {ARG_INT, ARG_INT}, {}},                                                // 0x42E
    {false, "REGISTER_HIGHEST_SCORE", {ARG_INT, ARG_INT}, {}},                                              // 0x42F
    {true, "WARP_CHAR_INTO_CAR_AS_PASSENGER", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},          // 0x430
    {true, "IS_CAR_PASSENGER_SEAT_FREE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x431
    {true, "GET_CHAR_IN_CAR_PASSENGER_SEAT", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_CHAR}},                                                           // 0x432
    {true, "SET_CHAR_IS_CHRIS_CRIMINAL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x433
    {true, "START_CREDITS", {}, {}},                                                                        // 0x434
    {true, "STOP_CREDITS", {}, {}},                                                                         // 0x435
    {true, "ARE_CREDITS_FINISHED", {}, {}},                                                                 // 0x436
    {false, "CREATE_SINGLE_PARTICLE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},        // 0x437
    {false, "SET_CHAR_IGNORE_LEVEL_TRANSITIONS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                          // 0x438
    {false, "GET_CHASE_CAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_UNIMPORTANT, TYPE_CAR}},                      // 0x439
    {false, "START_BOAT_FOAM_ANIMATION", {}, {}},                                                           // 0x43A
    {false, "UPDATE_BOAT_FOAM_ANIMATION", {ARG_INT}, {TYPE_OBJECT}},                                        // 0x43B
    {true, "SET_MUSIC_DOES_FADE", {ARG_INT}, {}},                                                           // 0x43C
    {false, "SET_INTRO_IS_PLAYING", {ARG_INT}, {}},                                                         // 0x43D
    {false, "SET_PLAYER_HOOKER", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CHAR}},                             // 0x43E
    {false, "PLAY_END_OF_GAME_TUNE", {}, {}},                                                               // 0x43F
    {false, "STOP_END_OF_GAME_TUNE", {}, {}},                                                               // 0x440
    {true, "GET_CAR_MODEL", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                         // 0x441
    {false, "IS_PLAYER_SITTING_IN_CAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CAR}},                       // 0x442
    {false, "IS_PLAYER_SITTING_IN_ANY_CAR", {ARG_INT}, {TYPE_PLAYER}},                                      // 0x443
    {false, "SET_SCRIPT_FIRE_AUDIO", {ARG_INT, ARG_INT}, {TYPE_SCRIPT_FIRE}},                               // 0x444
    {true, "ARE_ANY_CAR_CHEATS_ACTIVATED", {}, {}},                                                         // 0x445
    {true, "SET_CHAR_SUFFERS_CRITICAL_HITS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                              // 0x446
    {false, "IS_PLAYER_LIFTING_A_PHONE", {ARG_INT}, {TYPE_PLAYER}},                                         // 0x447
    {true, "IS_CHAR_SITTING_IN_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                            // 0x448
    {true, "IS_CHAR_SITTING_IN_ANY_CAR", {ARG_INT}, {TYPE_CHAR}},                                           // 0x449
    {false, "IS_PLAYER_ON_FOOT", {ARG_INT}, {TYPE_PLAYER}},                                                 // 0x44A
    {true, "IS_CHAR_ON_FOOT", {ARG_INT}, {TYPE_CHAR}},                                                      // 0x44B
    {false, "LOAD_COLLISION_WITH_SCREEN", {ARG_INT}, {}},                                                   // 0x44C
    {false, "LOAD_SPLASH_SCREEN", {ARG_TEXT_LABEL}, {}},                                                    // 0x44D
    {false, "SET_CAR_IGNORE_LEVEL_TRANSITIONS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                            // 0x44E
    {false, "MAKE_CRAIGS_CAR_A_BIT_STRONGER", {ARG_INT, ARG_INT}, {TYPE_CAR}},                              // 0x44F
    {false, "SET_JAMES_CAR_ON_PATH_TO_PLAYER", {ARG_INT}, {TYPE_CAR}},                                      // 0x450
    {false, "LOAD_END_OF_GAME_TUNE", {}, {}},                                                               // 0x451
    {false, "ENABLE_PLAYER_CONTROL_CAMERA", {}, {}},                                                        // 0x452
    {true, "SET_OBJECT_ROTATION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},               // 0x453
    {true, "GET_DEBUG_CAMERA_COORDINATES", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},     // 0x454
    {false, "GET_DEBUG_CAMERA_FRONT_VECTOR", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},   // 0x455
    {false, "IS_PLAYER_TARGETTING_ANY_CHAR", {ARG_INT}, {TYPE_PLAYER}},                                     // 0x456
    {true, "IS_PLAYER_TARGETTING_CHAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CHAR}},                      // 0x457
    {true, "IS_PLAYER_TARGETTING_OBJECT", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_OBJECT}},                  // 0x458
    {true, "TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME", {ARG_TEXT_LABEL}, {}},                                   // 0x459
    {true, "DISPLAY_TEXT_WITH_NUMBER", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_INT}, {}},                // 0x45A
    {true, "DISPLAY_TEXT_WITH_2_NUMBERS", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},    // 0x45B
    {true, "FAIL_CURRENT_MISSION", {}, {}},                                                                 // 0x45C
    {false, "GET_CLOSEST_OBJECT_OF_TYPE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_OBJECT}},                                                 // 0x45D
    {false, "PLACE_OBJECT_RELATIVE_TO_OBJECT", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_OBJECT, TYPE_OBJECT}},                                       // 0x45E
    {false, "SET_ALL_OCCUPANTS_OF_CAR_LEAVE_CAR", {ARG_INT}, {TYPE_CAR}},  // 0x45F
    {true, "SET_INTERPOLATION_PARAMETERS", {ARG_FLOAT, ARG_INT}, {}},      // 0x460
    {false, "GET_CLOSEST_CAR_NODE_WITH_HEADING_TOWARDS_POINT",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT},
        {}},  // 0x461
    {false, "GET_CLOSEST_CAR_NODE_WITH_HEADING_AWAY_POINT",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT},
        {}},                                                                                          // 0x462
    {true, "GET_DEBUG_CAMERA_POINT_AT", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x463
    {true, "ATTACH_CHAR_TO_CAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                       // 0x464
    {true, "DETACH_CHAR_FROM_CAR", {ARG_INT}, {TYPE_CHAR}},                                           // 0x465
    {true, "SET_CAR_STAY_IN_FAST_LANE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                              // 0x466
    {true, "CLEAR_CHAR_LAST_WEAPON_DAMAGE", {ARG_INT}, {TYPE_CHAR}},                                  // 0x467
    {true, "CLEAR_CAR_LAST_WEAPON_DAMAGE", {ARG_INT}, {TYPE_CAR}},                                    // 0x468
    {false, "GET_RANDOM_COP_IN_AREA",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},  // 0x469
    {false, "GET_RANDOM_COP_IN_ZONE", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                                       // 0x46A
    {false, "SET_CHAR_OBJ_FLEE_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                               // 0x46B
    {true, "GET_DRIVER_OF_CAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_CHAR}},                             // 0x46C
    {true, "GET_NUMBER_OF_FOLLOWERS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                 // 0x46D
    {true, "GIVE_REMOTE_CONTROLLED_MODEL_TO_PLAYER", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_PLAYER}},                                                                                        // 0x46E
    {false, "GET_CURRENT_PLAYER_WEAPON", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                            // 0x46F
    {true, "GET_CURRENT_CHAR_WEAPON", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                 // 0x470
    {true, "LOCATE_CHAR_ANY_MEANS_OBJECT_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                             // 0x471
    {true, "LOCATE_CHAR_ON_FOOT_OBJECT_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                             // 0x472
    {false, "LOCATE_CHAR_IN_CAR_OBJECT_2D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                             // 0x473
    {true, "LOCATE_CHAR_ANY_MEANS_OBJECT_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                             // 0x474
    {true, "LOCATE_CHAR_ON_FOOT_OBJECT_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                             // 0x475
    {false, "LOCATE_CHAR_IN_CAR_OBJECT_3D", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                             // 0x476
    {true, "SET_CAR_TEMP_ACTION", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                    // 0x477
    {false, "SET_CAR_HANDBRAKE_TURN_RIGHT", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x478
    {false, "SET_CAR_HANDBRAKE_STOP", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                         // 0x479
    {true, "IS_CHAR_ON_ANY_BIKE", {ARG_INT}, {TYPE_CHAR}},                                                     // 0x47A
    {false, "LOCATE_SNIPER_BULLET_2D", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},             // 0x47B
    {false, "LOCATE_SNIPER_BULLET_3D", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                                   // 0x47C
    {false, "GET_NUMBER_OF_SEATS_IN_MODEL", {ARG_INT, ARG_RETURN_INT}, {}},                                    // 0x47D
    {false, "IS_PLAYER_ON_ANY_BIKE", {ARG_INT}, {TYPE_PLAYER}},                                                // 0x47E
    {false, "IS_CHAR_LYING_DOWN", {ARG_INT}, {TYPE_CHAR}},                                                     // 0x47F
    {true, "CAN_CHAR_SEE_DEAD_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                         // 0x480
    {false, "SET_ENTER_CAR_RANGE_MULTIPLIER", {ARG_FLOAT}, {}},                                                // 0x481
    {false, "SET_THREAT_REACTION_RANGE_MULTIPLIER", {ARG_FLOAT}, {}},                                          // 0x482
    {false, "SET_CHAR_CEASE_ATTACK_TIMER", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x483
    {true, "GET_REMOTE_CONTROLLED_CAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER, TYPE_CAR}},                   // 0x484
    {true, "IS_PC_VERSION", {}, {}},                                                                           // 0x485
    {false, "REPLAY", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                       // 0x486
    {false, "IS_REPLAY_PLAYING", {}, {}},                                                                      // 0x487
    {true, "IS_MODEL_AVAILABLE", {ARG_INT}, {}},                                                               // 0x488
    {true, "SHUT_CHAR_UP", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                   // 0x489
    {true, "SET_ENABLE_RC_DETONATE", {ARG_INT}, {}},                                                           // 0x48A
    {true, "SET_CAR_RANDOM_ROUTE_SEED", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x48B
    {true, "IS_ANY_PICKUP_AT_COORDS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                  // 0x48C
    {false, "GET_FIRST_PICKUP_COORDS", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},            // 0x48D
    {false, "GET_NEXT_PICKUP_COORDS", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},             // 0x48E
    {true, "REMOVE_ALL_CHAR_WEAPONS", {ARG_INT}, {TYPE_CHAR}},                                                 // 0x48F
    {false, "HAS_PLAYER_GOT_WEAPON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                       // 0x490
    {true, "HAS_CHAR_GOT_WEAPON", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                            // 0x491
    {false, "IS_PLAYER_FACING_CHAR", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_PLAYER, TYPE_CHAR}},                 // 0x492
    {false, "SET_TANK_DETONATE_CARS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                         // 0x493
    {true, "GET_POSITION_OF_ANALOGUE_STICKS", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT},
        {}},                                                                                                   // 0x494
    {true, "IS_CAR_ON_FIRE", {ARG_INT}, {TYPE_CAR}},                                                           // 0x495
    {true, "IS_CAR_TYRE_BURST", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0x496
    {false, "SET_CAR_DRIVE_STRAIGHT_AHEAD", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x497
    {false, "SET_CAR_WAIT", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                   // 0x498
    {false, "IS_PLAYER_STANDING_ON_A_VEHICLE", {ARG_INT}, {TYPE_PLAYER}},                                      // 0x499
    {false, "IS_PLAYER_FOOT_DOWN", {ARG_INT}, {TYPE_PLAYER}},                                                  // 0x49A
    {false, "IS_CHAR_FOOT_DOWN", {ARG_INT}, {TYPE_CHAR}},                                                      // 0x49B
    {false, "INITIALISE_OBJECT_PATH", {ARG_INT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT_PATH}},                                               // 0x49C
    {false, "START_OBJECT_ON_PATH", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_OBJECT_PATH}},                      // 0x49D
    {false, "SET_OBJECT_PATH_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT_PATH}},                                // 0x49E
    {false, "SET_OBJECT_PATH_POSITION", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT_PATH}},                             // 0x49F
    {false, "GET_OBJECT_DISTANCE_ALONG_PATH", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_OBJECT_PATH}},                // 0x4A0
    {false, "CLEAR_OBJECT_PATH", {ARG_INT}, {TYPE_OBJECT_PATH}},                                               // 0x4A1
    {true, "HELI_GOTO_COORDS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},  // 0x4A2
    {true, "IS_INT_VAR_EQUAL_TO_CONSTANT", {ARG_VAR_INT, ARG_INT}, {}},                                        // 0x4A3
    {true, "IS_INT_LVAR_EQUAL_TO_CONSTANT", {ARG_LVAR_INT, ARG_INT}, {}},                                      // 0x4A4
    {true, "GET_DEAD_CHAR_PICKUP_COORDS", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CHAR}},                                                                                          // 0x4A5
    {true, "CREATE_PROTECTION_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_PICKUP}},                                                            // 0x4A6
    {true, "IS_CHAR_IN_ANY_BOAT", {ARG_INT}, {TYPE_CHAR}},                            // 0x4A7
    {false, "IS_PLAYER_IN_ANY_BOAT", {ARG_INT}, {TYPE_PLAYER}},                       // 0x4A8
    {true, "IS_CHAR_IN_ANY_HELI", {ARG_INT}, {TYPE_CHAR}},                            // 0x4A9
    {false, "IS_PLAYER_IN_ANY_HELI", {ARG_INT}, {TYPE_PLAYER}},                       // 0x4AA
    {true, "IS_CHAR_IN_ANY_PLANE", {ARG_INT}, {TYPE_CHAR}},                           // 0x4AB
    {false, "IS_PLAYER_IN_ANY_PLANE", {ARG_INT}, {TYPE_PLAYER}},                      // 0x4AC
    {true, "IS_CHAR_IN_WATER", {ARG_INT}, {TYPE_CHAR}},                               // 0x4AD
    {true, "SET_VAR_INT_TO_CONSTANT", {ARG_VAR_INT, ARG_INT}, {}},                    // 0x4AE
    {true, "SET_LVAR_INT_TO_CONSTANT", {ARG_LVAR_INT, ARG_INT}, {}},                  // 0x4AF
    {true, "IS_INT_VAR_GREATER_THAN_CONSTANT", {ARG_VAR_INT, ARG_INT}, {}},           // 0x4B0
    {true, "IS_INT_LVAR_GREATER_THAN_CONSTANT", {ARG_LVAR_INT, ARG_INT}, {}},         // 0x4B1
    {true, "IS_CONSTANT_GREATER_THAN_INT_VAR", {ARG_INT, ARG_VAR_INT}, {}},           // 0x4B2
    {true, "IS_CONSTANT_GREATER_THAN_INT_LVAR", {ARG_INT, ARG_LVAR_INT}, {}},         // 0x4B3
    {true, "IS_INT_VAR_GREATER_OR_EQUAL_TO_CONSTANT", {ARG_VAR_INT, ARG_INT}, {}},    // 0x4B4
    {true, "IS_INT_LVAR_GREATER_OR_EQUAL_TO_CONSTANT", {ARG_LVAR_INT, ARG_INT}, {}},  // 0x4B5
    {true, "IS_CONSTANT_GREATER_OR_EQUAL_TO_INT_VAR", {ARG_INT, ARG_VAR_INT}, {}},    // 0x4B6
    {true, "IS_CONSTANT_GREATER_OR_EQUAL_TO_INT_LVAR", {ARG_INT, ARG_LVAR_INT}, {}},  // 0x4B7
    {true, "GET_CHAR_WEAPON_IN_SLOT", {ARG_INT, ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT},
        {TYPE_CHAR}},                                                                 // 0x4B8
    {true, "GET_CLOSEST_STRAIGHT_ROAD",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {}},                                                                           // 0x4B9
    {true, "SET_CAR_FORWARD_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                 // 0x4BA
    {true, "SET_AREA_VISIBLE", {ARG_INT}, {}},                                         // 0x4BB
    {false, "SET_CUTSCENE_ANIM_TO_LOOP", {ARG_TEXT_LABEL}, {}},                        // 0x4BC
    {true, "MARK_CAR_AS_CONVOY_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                  // 0x4BD
    {false, "RESET_HAVOC_CAUSED_BY_PLAYER", {ARG_INT}, {TYPE_PLAYER}},                 // 0x4BE
    {false, "GET_HAVOC_CAUSED_BY_PLAYER", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},   // 0x4BF
    {true, "CREATE_SCRIPT_ROADBLOCK", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                           // 0x4C0
    {true, "CLEAR_ALL_SCRIPT_ROADBLOCKS", {}, {}},                                     // 0x4C1
    {false, "SET_CHAR_OBJ_WALK_TO_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},  // 0x4C2
    {false, "IS_PICKUP_IN_ZONE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_PICKUP}},            // 0x4C3
    {true, "GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CHAR}},                                                                     // 0x4C4
    {true, "HAS_CHAR_BEEN_PHOTOGRAPHED", {ARG_INT}, {TYPE_CHAR}},                         // 0x4C5
    {false, "SET_CHAR_OBJ_AIM_GUN_AT_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},  // 0x4C6
    {false, "SWITCH_SECURITY_CAMERA", {ARG_INT}, {}},                                     // 0x4C7
    {true, "IS_CHAR_IN_FLYING_VEHICLE", {ARG_INT}, {TYPE_CHAR}},                          // 0x4C8
    {false, "IS_PLAYER_IN_FLYING_VEHICLE", {ARG_INT}, {TYPE_PLAYER}},                     // 0x4C9
    {false, "HAS_SONY_CD_BEEN_READ", {ARG_INT}, {}},                                      // 0x4CA
    {false, "GET_NUMBER_OF_SONY_CDS_READ", {ARG_RETURN_INT}, {}},                         // 0x4CB
    {false, "ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_BLIP}},                                                                                        // 0x4CC
    {false, "ADD_SHORT_RANGE_BLIP_FOR_COORD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                                     // 0x4CD
    {true, "ADD_SHORT_RANGE_SPRITE_BLIP_FOR_COORD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                   // 0x4CE
    {false, "ADD_MONEY_SPENT_ON_CLOTHES", {ARG_INT}, {}},                                                       // 0x4CF
    {true, "SET_HELI_ORIENTATION", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                           // 0x4D0
    {true, "CLEAR_HELI_ORIENTATION", {ARG_INT}, {TYPE_CAR}},                                                    // 0x4D1
    {true, "PLANE_GOTO_COORDS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},  // 0x4D2
    {true, "GET_NTH_CLOSEST_CAR_NODE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x4D3
    {false, "GET_NTH_CLOSEST_CHAR_NODE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x4D4
    {true, "DRAW_WEAPONSHOP_CORONA",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},         // 0x4D5
    {true, "SET_ENABLE_RC_DETONATE_ON_CONTACT", {ARG_INT}, {}},                                                 // 0x4D6
    {true, "FREEZE_CHAR_POSITION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                            // 0x4D7
    {true, "SET_CHAR_DROWNS_IN_WATER", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                        // 0x4D8
    {true, "SET_OBJECT_RECORDS_COLLISIONS", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                 // 0x4D9
    {true, "HAS_OBJECT_COLLIDED_WITH_ANYTHING", {ARG_INT}, {TYPE_OBJECT}},                                      // 0x4DA
    {true, "REMOVE_RC_BUGGY", {}, {}},                                                                          // 0x4DB
    {false, "HAS_PHOTOGRAPH_BEEN_TAKEN", {}, {}},                                                               // 0x4DC
    {true, "GET_CHAR_ARMOUR", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                          // 0x4DD
    {false, "SET_CHAR_ARMOUR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                // 0x4DE
    {true, "SET_HELI_STABILISER", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                              // 0x4DF
    {true, "SET_CAR_STRAIGHT_LINE_DISTANCE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x4E0
    {true, "POP_CAR_BOOT", {ARG_INT}, {TYPE_CAR}},                                                              // 0x4E1
    {true, "SHUT_PLAYER_UP", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                                // 0x4E2
    {true, "SET_PLAYER_MOOD", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                      // 0x4E3
    {true, "REQUEST_COLLISION", {ARG_FLOAT, ARG_FLOAT}, {}},                                                    // 0x4E4
    {true, "LOCATE_OBJECT_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_OBJECT}},  // 0x4E5
    {true, "LOCATE_OBJECT_3D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},                                                                                         // 0x4E6
    {true, "IS_OBJECT_IN_WATER", {ARG_INT}, {TYPE_OBJECT}},                                                     // 0x4E7
    {false, "SET_CHAR_OBJ_STEAL_ANY_CAR_EVEN_MISSION_CAR", {ARG_INT}, {TYPE_CHAR}},                             // 0x4E8
    {false, "IS_OBJECT_IN_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},                                                                                         // 0x4E9
    {true, "IS_OBJECT_IN_AREA_3D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},                                                                                         // 0x4EA
    {true, "TASK_TOGGLE_DUCK", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                // 0x4EB
    {false, "SET_ZONE_CIVILIAN_CAR_INFO",
        {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT,
            ARG_INT, ARG_INT},
        {}},                                                                                                  // 0x4EC
    {true, "REQUEST_ANIMATION", {ARG_TEXT_LABEL16}, {}},                                                      // 0x4ED
    {true, "HAS_ANIMATION_LOADED", {ARG_TEXT_LABEL16}, {}},                                                   // 0x4EE
    {true, "REMOVE_ANIMATION", {ARG_TEXT_LABEL16}, {}},                                                       // 0x4EF
    {true, "IS_CHAR_WAITING_FOR_WORLD_COLLISION", {ARG_INT}, {TYPE_CHAR}},                                    // 0x4F0
    {true, "IS_CAR_WAITING_FOR_WORLD_COLLISION", {ARG_INT}, {TYPE_CAR}},                                      // 0x4F1
    {false, "IS_OBJECT_WAITING_FOR_WORLD_COLLISION", {ARG_INT}, {TYPE_OBJECT}},                               // 0x4F2
    {false, "SET_CHAR_SHUFFLE_INTO_DRIVERS_SEAT", {ARG_INT}, {TYPE_CHAR}},                                    // 0x4F3
    {true, "ATTACH_CHAR_TO_OBJECT", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                            // 0x4F4
    {false, "SET_CHAR_AS_PLAYER_FRIEND", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_PLAYER}},              // 0x4F5
    {false, "DISPLAY_NTH_ONSCREEN_COUNTER", {ARG_VAR_INT, ARG_INT, ARG_INT}, {}},                             // 0x4F6
    {true, "DISPLAY_NTH_ONSCREEN_COUNTER_WITH_STRING", {ARG_VAR_INT, ARG_INT, ARG_INT, ARG_TEXT_LABEL}, {}},  // 0x4F7
    {true, "ADD_SET_PIECE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT,
            ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                              // 0x4F8
    {true, "SET_EXTRA_COLOURS", {ARG_INT, ARG_INT}, {}},  // 0x4F9
    {true, "CLEAR_EXTRA_COLOURS", {ARG_INT}, {}},         // 0x4FA
    {false, "CLOSE_CAR_BOOT", {ARG_INT}, {TYPE_CAR}},     // 0x4FB
    {true, "GET_WHEELIE_STATS",
        {ARG_INT, ARG_RETURN_INT, ARG_RETURN_FLOAT, ARG_RETURN_INT, ARG_RETURN_FLOAT, ARG_RETURN_INT, ARG_RETURN_FLOAT},
        {TYPE_PLAYER}},                                                                     // 0x4FC
    {false, "DISARM_CHAR", {ARG_INT}, {TYPE_CHAR}},                                         // 0x4FD
    {true, "BURST_CAR_TYRE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                               // 0x4FE
    {false, "IS_CHAR_OBJ_NO_OBJ", {ARG_INT}, {TYPE_CHAR}},                                  // 0x4FF
    {true, "IS_PLAYER_WEARING", {ARG_INT, ARG_INT, ARG_TEXT_LABEL16}, {TYPE_PLAYER}},       // 0x500
    {true, "SET_PLAYER_CAN_DO_DRIVE_BY", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                // 0x501
    {false, "SET_CHAR_OBJ_SPRINT_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},  // 0x502
    {true, "CREATE_SWAT_ROPE", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                                       // 0x503
    {false, "SET_FIRST_PERSON_CONTROL_CAMERA", {}, {}},                                                        // 0x504
    {false, "GET_NEAREST_TYRE_TO_POINT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {TYPE_CAR}},         // 0x505
    {true, "SET_CAR_MODEL_COMPONENTS", {ARG_INT, ARG_INT, ARG_INT}, {}},                                       // 0x506
    {false, "SWITCH_LIFT_CAMERA", {ARG_INT}, {}},                                                              // 0x507
    {true, "CLOSE_ALL_CAR_DOORS", {ARG_INT}, {TYPE_CAR}},                                                      // 0x508
    {true, "GET_DISTANCE_BETWEEN_COORDS_2D", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT},
        {}},                                                                                                   // 0x509
    {true, "GET_DISTANCE_BETWEEN_COORDS_3D",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT}, {}},             // 0x50A
    {false, "POP_CAR_BOOT_USING_PHYSICS", {ARG_INT}, {TYPE_CAR}},                                              // 0x50B
    {false, "SET_FIRST_PERSON_WEAPON_CAMERA", {}, {}},                                                         // 0x50C
    {false, "IS_CHAR_LEAVING_VEHICLE_TO_DIE", {ARG_INT}, {TYPE_CHAR}},                                         // 0x50D
    {true, "SORT_OUT_OBJECT_COLLISION_WITH_CAR", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_CAR}},                 // 0x50E
    {true, "GET_MAX_WANTED_LEVEL", {ARG_RETURN_INT}, {}},                                                      // 0x50F
    {false, "IS_CHAR_WANDER_PATH_CLEAR", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},  // 0x510
    {false, "PRINT_HELP_WITH_NUMBER", {ARG_TEXT_LABEL, ARG_INT}, {}},                                          // 0x511
    {true, "PRINT_HELP_FOREVER", {ARG_TEXT_LABEL}, {}},                                                        // 0x512
    {true, "PRINT_HELP_FOREVER_WITH_NUMBER", {ARG_TEXT_LABEL, ARG_INT}, {}},                                   // 0x513
    {false, "SET_CHAR_CAN_BE_DAMAGED_BY_MEMBERS_OF_GANG", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},           // 0x514
    {true, "LOAD_AND_LAUNCH_MISSION_EXCLUSIVE", {ARG_LABEL}, {}},                                              // 0x515
    {false, "IS_MISSION_AUDIO_PLAYING", {ARG_INT}, {}},                                                        // 0x516
    {true, "CREATE_LOCKED_PROPERTY_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_PICKUP}},                // 0x517
    {true, "CREATE_FORSALE_PROPERTY_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_TEXT_LABEL, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_PICKUP}},                                                                               // 0x518
    {true, "FREEZE_CAR_POSITION", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x519
    {true, "HAS_CHAR_BEEN_DAMAGED_BY_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                 // 0x51A
    {true, "HAS_CHAR_BEEN_DAMAGED_BY_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                   // 0x51B
    {true, "HAS_CAR_BEEN_DAMAGED_BY_CHAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CHAR}},                   // 0x51C
    {true, "HAS_CAR_BEEN_DAMAGED_BY_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                     // 0x51D
    {true, "GET_RADIO_CHANNEL", {ARG_RETURN_INT}, {}},                                                   // 0x51E
    {false, "DISPLAY_TEXT_WITH_3_NUMBERS", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                             // 0x51F
    {false, "IS_CAR_DROWNING_IN_WATER", {ARG_INT}, {TYPE_CAR}},                                          // 0x520
    {false, "IS_CHAR_DROWNING_IN_WATER", {ARG_INT}, {TYPE_CHAR}},                                        // 0x521
    {false, "DISABLE_CUTSCENE_SHADOWS", {}, {}},                                                         // 0x522
    {false, "HAS_GLASS_BEEN_SHATTERED_NEARBY", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                   // 0x523
    {false, "ATTACH_CUTSCENE_OBJECT_TO_BONE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_OBJECT}},  // 0x524
    {false, "ATTACH_CUTSCENE_OBJECT_TO_COMPONENT", {ARG_INT, ARG_INT, ARG_TEXT_LABEL},
        {TYPE_OBJECT, TYPE_OBJECT}},                                                                     // 0x525
    {true, "SET_CHAR_STAY_IN_CAR_WHEN_JACKED", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                         // 0x526
    {false, "IS_MISSION_AUDIO_LOADING", {ARG_INT}, {}},                                                  // 0x527
    {false, "ADD_MONEY_SPENT_ON_WEAPONS", {ARG_INT}, {}},                                                // 0x528
    {false, "ADD_MONEY_SPENT_ON_PROPERTY", {ARG_INT}, {}},                                               // 0x529
    {false, "ADD_MONEY_SPENT_ON_AUTO_PAINTING", {ARG_INT}, {}},                                          // 0x52A
    {false, "SET_CHAR_ANSWERING_MOBILE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                               // 0x52B
    {true, "SET_PLAYER_DRUNKENNESS", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x52C
    {false, "GET_PLAYER_DRUNKENNESS", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                         // 0x52D
    {false, "SET_PLAYER_DRUG_LEVEL", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x52E
    {false, "GET_PLAYER_DRUG_LEVEL", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                          // 0x52F
    {false, "ADD_LOAN_SHARK_VISITS", {ARG_INT}, {}},                                                     // 0x530
    {false, "ADD_STORES_KNOCKED_OFF", {ARG_INT}, {}},                                                    // 0x531
    {false, "ADD_MOVIE_STUNTS", {ARG_INT}, {}},                                                          // 0x532
    {false, "ADD_NUMBER_OF_ASSASSINATIONS", {ARG_INT}, {}},                                              // 0x533
    {false, "ADD_PIZZAS_DELIVERED", {ARG_INT}, {}},                                                      // 0x534
    {false, "ADD_GARBAGE_PICKUPS", {ARG_INT}, {}},                                                       // 0x535
    {false, "ADD_ICE_CREAMS_SOLD", {ARG_INT}, {}},                                                       // 0x536
    {false, "SET_TOP_SHOOTING_RANGE_SCORE", {ARG_INT}, {}},                                              // 0x537
    {false, "ADD_SHOOTING_RANGE_RANK", {ARG_INT}, {}},                                                   // 0x538
    {false, "ADD_MONEY_SPENT_ON_GAMBLING", {ARG_INT}, {}},                                               // 0x539
    {false, "ADD_MONEY_WON_ON_GAMBLING", {ARG_INT}, {}},                                                 // 0x53A
    {false, "SET_LARGEST_GAMBLING_WIN", {ARG_INT}, {}},                                                  // 0x53B
    {false, "SET_CHAR_IN_PLAYERS_GROUP_CAN_FIGHT", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                     // 0x53C
    {false, "CLEAR_CHAR_WAIT_STATE", {ARG_INT}, {TYPE_CHAR}},                                            // 0x53D
    {true, "GET_RANDOM_CAR_OF_TYPE_IN_AREA_NO_SAVE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x53E
    {true, "SET_CAN_BURST_CAR_TYRES", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                          // 0x53F
    {false, "SET_PLAYER_AUTO_AIM", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                          // 0x540
    {true, "FIRE_HUNTER_GUN", {ARG_INT}, {TYPE_CAR}},                                                           // 0x541
    {false, "SET_PROPERTY_AS_OWNED", {ARG_INT}, {}},                                                            // 0x542
    {false, "ADD_BLOOD_RING_KILLS", {ARG_INT}, {}},                                                             // 0x543
    {false, "SET_LONGEST_TIME_IN_BLOOD_RING", {ARG_INT}, {}},                                                   // 0x544
    {false, "REMOVE_EVERYTHING_FOR_HUGE_CUTSCENE", {}, {}},                                                     // 0x545
    {false, "IS_PLAYER_TOUCHING_VEHICLE", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CAR}},                         // 0x546
    {true, "IS_CHAR_TOUCHING_VEHICLE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                              // 0x547
    {false, "CHECK_FOR_PED_MODEL_AROUND_PLAYER", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_PLAYER}},                                                                                         // 0x548
    {false, "CLEAR_CHAR_FOLLOW_PATH", {ARG_INT}, {TYPE_CHAR}},                                                  // 0x549
    {true, "SET_CHAR_CAN_BE_SHOT_IN_VEHICLE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                 // 0x54A
    {false, "ATTACH_CUTSCENE_OBJECT_TO_VEHICLE", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_CAR}},                  // 0x54B
    {true, "LOAD_MISSION_TEXT", {ARG_TEXT_LABEL}, {}},                                                          // 0x54C
    {false, "SET_TONIGHTS_EVENT", {ARG_INT}, {}},                                                               // 0x54D
    {true, "CLEAR_CHAR_LAST_DAMAGE_ENTITY", {ARG_INT}, {TYPE_CHAR}},                                            // 0x54E
    {true, "CLEAR_CAR_LAST_DAMAGE_ENTITY", {ARG_INT}, {TYPE_CAR}},                                              // 0x54F
    {true, "FREEZE_OBJECT_POSITION", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                        // 0x550
    {false, "SET_PLAYER_HAS_MET_DEBBIE_HARRY", {ARG_INT}, {}},                                                  // 0x551
    {false, "SET_RIOT_INTENSITY", {ARG_INT}, {}},                                                               // 0x552
    {false, "IS_CAR_IN_ANGLED_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                            // 0x553
    {false, "IS_CAR_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},                                                                                      // 0x554
    {true, "REMOVE_WEAPON_FROM_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x555
    {false, "SET_UP_TAXI_SHORTCUT",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},    // 0x556
    {false, "CLEAR_TAXI_SHORTCUT", {}, {}},                                                               // 0x557
    {false, "SET_CHAR_OBJ_GOTO_CAR_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                  // 0x558
    {false, "GET_CLOSEST_WATER_NODE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},     // 0x559
    {false, "ADD_PORN_LEAFLET_TO_RUBBISH", {ARG_INT}, {}},                                                // 0x55A
    {false, "CREATE_CLOTHES_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_PICKUP}},           // 0x55B
    {false, "CHANGE_BLIP_THRESHOLD", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_BLIP}},                       // 0x55C
    {true, "MAKE_PLAYER_FIRE_PROOF", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                  // 0x55D
    {true, "INCREASE_PLAYER_MAX_HEALTH", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                              // 0x55E
    {true, "INCREASE_PLAYER_MAX_ARMOUR", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                              // 0x55F
    {true, "CREATE_RANDOM_CHAR_AS_DRIVER", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_CHAR}},             // 0x560
    {true, "CREATE_RANDOM_CHAR_AS_PASSENGER", {ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_CHAR}},                                                         // 0x561
    {false, "SET_CHAR_IGNORE_THREATS_BEHIND_OBJECTS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                   // 0x562
    {true, "ENSURE_PLAYER_HAS_DRIVE_BY_WEAPON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                       // 0x563
    {true, "MAKE_HELI_COME_CRASHING_DOWN", {ARG_INT}, {TYPE_CAR}},                                        // 0x564
    {true, "ADD_EXPLOSION_NO_SOUND", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                     // 0x565
    {true, "SET_OBJECT_AREA_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                 // 0x566
    {false, "WAS_VEHICLE_EVER_POLICE", {ARG_INT}, {TYPE_CAR}},                                            // 0x567
    {true, "SET_CHAR_NEVER_TARGETTED", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x568
    {false, "LOAD_UNCOMPRESSED_ANIM", {ARG_TEXT_LABEL}, {}},                                              // 0x569
    {true, "WAS_CUTSCENE_SKIPPED", {}, {}},                                                               // 0x56A
    {false, "SET_CHAR_CROUCH_WHEN_THREATENED", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                          // 0x56B
    {true, "IS_CHAR_IN_ANY_POLICE_VEHICLE", {ARG_INT}, {TYPE_CHAR}},                                      // 0x56C
    {true, "DOES_CHAR_EXIST", {ARG_INT}, {TYPE_CHAR}},                                                    // 0x56D
    {true, "DOES_VEHICLE_EXIST", {ARG_INT}, {TYPE_CAR}},                                                  // 0x56E
    {false, "ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},                               // 0x56F
    {true, "ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_BLIP}},             // 0x570
    {false, "IS_CHAR_STUCK", {ARG_INT}, {TYPE_CHAR}},                                                     // 0x571
    {true, "SET_ALL_TAXIS_HAVE_NITRO", {ARG_INT}, {}},                                                    // 0x572
    {false, "SET_CHAR_STOP_SHOOT_DONT_SEEK_ENTITY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                     // 0x573
    {true, "FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION", {ARG_INT, ARG_INT}, {TYPE_CAR}},                // 0x574
    {true, "FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},              // 0x575
    {false, "FREEZE_OBJECT_POSITION_AND_DONT_LOAD_COLLISION", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},         // 0x576
    {false, "SET_FADE_AND_JUMPCUT_AFTER_RC_EXPLOSION", {ARG_INT}, {}},                                    // 0x577
    {false, "REGISTER_VIGILANTE_LEVEL", {ARG_INT}, {}},                                                   // 0x578
    {false, "CLEAR_ALL_CHAR_ANIMS", {ARG_INT}, {TYPE_CHAR}},                                              // 0x579
    {false, "SET_MAXIMUM_NUMBER_OF_CARS_IN_GARAGE", {ARG_TEXT_LABEL, ARG_INT}, {}},                       // 0x57A
    {false, "WANTED_STARS_ARE_FLASHING", {}, {}},                                                         // 0x57B
    {false, "SET_ALLOW_HURRICANES", {ARG_INT}, {}},                                                       // 0x57C
    {false, "PLAY_ANNOUNCEMENT", {ARG_INT}, {}},                                                          // 0x57D
    {true, "SET_PLAYER_IS_IN_STADIUM", {ARG_INT}, {}},                                                    // 0x57E
    {false, "GET_BUS_FARES_COLLECTED_BY_PLAYER", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},               // 0x57F
    {false, "SET_CHAR_OBJ_BUY_ICE_CREAM", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_CAR}},     // 0x580
    {true, "DISPLAY_RADAR", {ARG_INT}, {}},                                                               // 0x581
    {true, "REGISTER_BEST_POSITION", {ARG_INT, ARG_INT}, {}},                                             // 0x582
    {true, "IS_PLAYER_IN_INFO_ZONE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_PLAYER}},                           // 0x583
    {false, "CLEAR_CHAR_ICE_CREAM_PURCHASE", {ARG_INT}, {TYPE_CHAR}},                                     // 0x584
    {false, "IS_IN_CAR_FIRE_BUTTON_PRESSED", {}, {}},                                                     // 0x585
    {false, "HAS_CHAR_ATTEMPTED_ATTRACTOR", {ARG_INT}, {TYPE_CHAR, TYPE_ATTRACTOR}},                      // 0x586
    {true, "SET_LOAD_COLLISION_FOR_CAR_FLAG", {ARG_INT, ARG_INT}, {TYPE_CAR}},                            // 0x587
    {true, "SET_LOAD_COLLISION_FOR_CHAR_FLAG", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                          // 0x588
    {false, "SET_LOAD_COLLISION_FOR_OBJECT_FLAG", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                     // 0x589
    {true, "ADD_BIG_GUN_FLASH", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x58A
    {false, "HAS_CHAR_BOUGHT_ICE_CREAM", {ARG_INT}, {TYPE_CHAR}},                                         // 0x58B
    {true, "GET_PROGRESS_PERCENTAGE", {ARG_RETURN_FLOAT}, {}},                                            // 0x58C
    {false, "SET_SHORTCUT_PICKUP_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},               // 0x58D
    {false, "SET_SHORTCUT_DROPOFF_POINT_FOR_MISSION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x58E
    {false, "GET_RANDOM_ICE_CREAM_CUSTOMER_IN_AREA",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                                                                 // 0x58F
    {false, "GET_RANDOM_ICE_CREAM_CUSTOMER_IN_ZONE", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},              // 0x590
    {false, "UNLOCK_ALL_CAR_DOORS_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},             // 0x591
    {false, "SET_GANG_ATTACK_PLAYER_WITH_COPS", {ARG_INT, ARG_INT}, {}},                                   // 0x592
    {false, "SET_CHAR_FRIGHTENED_IN_JACKED_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                         // 0x593
    {true, "SET_VEHICLE_TO_FADE_IN", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                      // 0x594
    {true, "REGISTER_ODDJOB_MISSION_PASSED", {}, {}},                                                      // 0x595
    {true, "IS_PLAYER_IN_SHORTCUT_TAXI", {ARG_INT}, {TYPE_PLAYER}},                                        // 0x596
    {true, "IS_CHAR_DUCKING", {ARG_INT}, {TYPE_CHAR}},                                                     // 0x597
    {false, "CREATE_DUST_EFFECT_FOR_CUTSCENE_HELI", {ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_OBJECT}},         // 0x598
    {false, "REGISTER_FIRE_LEVEL", {ARG_INT}, {}},                                                         // 0x599
    {true, "IS_AUSTRALIAN_GAME", {}, {}},                                                                  // 0x59A
    {false, "DISARM_CAR_BOMB", {ARG_INT}, {TYPE_CAR}},                                                     // 0x59B
    {true, "SET_ONSCREEN_COUNTER_FLASH_WHEN_FIRST_DISPLAYED", {ARG_VAR_INT, ARG_INT}, {}},                 // 0x59C
    {true, "SHUFFLE_CARD_DECKS", {ARG_INT}, {}},                                                           // 0x59D
    {true, "FETCH_NEXT_CARD", {ARG_RETURN_INT}, {}},                                                       // 0x59E
    {true, "GET_OBJECT_VELOCITY", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_OBJECT}},                                                                                    // 0x59F
    {true, "IS_DEBUG_CAMERA_ON", {}, {}},                                                                  // 0x5A0
    {true, "ADD_TO_OBJECT_ROTATION_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},  // 0x5A1
    {true, "SET_OBJECT_ROTATION_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},     // 0x5A2
    {true, "IS_OBJECT_STATIC", {ARG_INT}, {TYPE_OBJECT}},                                                  // 0x5A3
    {true, "GET_ANGLE_BETWEEN_2D_VECTORS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT},
        {}},                                                                                               // 0x5A4
    {true, "DO_2D_RECTANGLES_COLLIDE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},     // 0x5A5
    {true, "GET_OBJECT_ROTATION_VELOCITY", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_OBJECT}},                                                                                    // 0x5A6
    {true, "ADD_VELOCITY_RELATIVE_TO_OBJECT_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_OBJECT}},                                                                                    // 0x5A7
    {true, "GET_OBJECT_SPEED", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},                                // 0x5A8
    {true, "SET_VAR_TEXT_LABEL", {ARG_VAR_TEXT_LABEL, ARG_TEXT_LABEL}, {}},                                // 0x5A9
    {true, "SET_LVAR_TEXT_LABEL", {ARG_LVAR_TEXT_LABEL, ARG_TEXT_LABEL}, {}},                              // 0x5AA
    {true, "VAR_TEXT_LABEL",
        {ARG_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL,
            ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL,
            ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL,
            ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL,
            ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL,
            ARG_DECLARE_VAR_TEXT_LABEL, ARG_DECLARE_VAR_TEXT_LABEL},
        {}},  // 0x5AB
    {true, "LVAR_TEXT_LABEL",
        {ARG_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL,
            ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL,
            ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL,
            ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL,
            ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL,
            ARG_DECLARE_LVAR_TEXT_LABEL, ARG_DECLARE_LVAR_TEXT_LABEL},
        {}},                                                                                            // 0x5AC
    {true, "IS_VAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL", {ARG_VAR_TEXT_LABEL, ARG_TEXT_LABEL}, {}},          // 0x5AD
    {true, "IS_LVAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL", {ARG_LVAR_TEXT_LABEL, ARG_TEXT_LABEL}, {}},        // 0x5AE
    {false, "DO_2D_LINES_INTERSECT",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x5AF
    {true, "GET_2D_LINES_INTERSECT_POINT",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT},
        {}},                                                                                             // 0x5B0
    {false, "IS_2D_POINT_IN_TRIANGLE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},   // 0x5B1
    {false, "IS_2D_POINT_IN_RECTANGLE_ON_LEFT_SIDE_OF_LINE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                         // 0x5B2
    {false, "IS_2D_POINT_ON_LEFT_SIDE_OF_2D_LINE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                             // 0x5B3
    {false, "CHAR_LOOK_AT_OBJECT_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},                 // 0x5B4
    {false, "APPLY_COLLISION_ON_OBJECT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},     // 0x5B5
    {true, "SAVE_STRING_TO_DEBUG_FILE", {ARG_STRING, ARG_STRING, ARG_STRING, ARG_STRING}, {}},           // 0x5B6
    {false, "TASK_PLAYER_ON_FOOT", {ARG_INT}, {TYPE_PLAYER}},                                            // 0x5B7
    {false, "TASK_PLAYER_IN_CAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CAR}},                          // 0x5B8
    {true, "TASK_PAUSE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                               // 0x5B9
    {true, "TASK_STAND_STILL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                         // 0x5BA
    {true, "TASK_FALL_AND_GET_UP", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                            // 0x5BB
    {true, "TASK_JUMP", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                // 0x5BC
    {true, "TASK_TIRED", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                               // 0x5BD
    {true, "TASK_DIE", {ARG_INT}, {TYPE_CHAR}},                                                          // 0x5BE
    {true, "TASK_LOOK_AT_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                    // 0x5BF
    {true, "TASK_LOOK_AT_VEHICLE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                  // 0x5C0
    {true, "TASK_SAY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                 // 0x5C1
    {true, "TASK_SHAKE_FIST", {ARG_INT}, {TYPE_CHAR}},                                                   // 0x5C2
    {true, "TASK_COWER", {ARG_INT}, {TYPE_CHAR}},                                                        // 0x5C3
    {true, "TASK_HANDS_UP", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                            // 0x5C4
    {true, "TASK_DUCK", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                // 0x5C5
    {false, "TASK_DETONATE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x5C6
    {true, "TASK_USE_ATM", {ARG_INT}, {TYPE_CHAR}},                                                      // 0x5C7
    {true, "TASK_SCRATCH_HEAD", {ARG_INT}, {TYPE_CHAR}},                                                 // 0x5C8
    {true, "TASK_LOOK_ABOUT", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                          // 0x5C9
    {true, "TASK_ENTER_CAR_AS_PASSENGER", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},  // 0x5CA
    {true, "TASK_ENTER_CAR_AS_DRIVER", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},              // 0x5CB
    {false, "TASK_STEAL_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                                // 0x5CC
    {true, "TASK_LEAVE_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                                 // 0x5CD
    {false, "TASK_LEAVE_CAR_AND_DIE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                        // 0x5CE
    {true, "TASK_LEAVE_CAR_AND_FLEE", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR, TYPE_CAR}},                                                                          // 0x5CF
    {false, "TASK_CAR_DRIVE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                                // 0x5D0
    {true, "TASK_CAR_DRIVE_TO_COORD",
        {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                                // 0x5D1
    {true, "TASK_CAR_DRIVE_WANDER", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},            // 0x5D2
    {true, "TASK_GO_STRAIGHT_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x5D3
    {true, "TASK_ACHIEVE_HEADING", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                         // 0x5D4
    {false, "SET_CHAR_IN_DISGUISE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                          // 0x5D5
    {true, "FLUSH_ROUTE", {}, {}},                                                                             // 0x5D6
    {true, "EXTEND_ROUTE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                             // 0x5D7
    {true, "TASK_FOLLOW_POINT_ROUTE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                               // 0x5D8
    {true, "TASK_GOTO_CHAR", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR, TYPE_CHAR}},                  // 0x5D9
    {true, "TASK_FLEE_POINT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x5DA
    {true, "TASK_FLEE_CHAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                  // 0x5DB
    {true, "TASK_SMART_FLEE_POINT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x5DC
    {true, "TASK_SMART_FLEE_CHAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},            // 0x5DD
    {true, "TASK_WANDER_STANDARD", {ARG_INT}, {TYPE_CHAR}},                                                    // 0x5DE
    {false, "TASK_WANDER_COP", {ARG_INT}, {TYPE_CHAR}},                                                        // 0x5DF
    {false, "TASK_WANDER_CRIMINAL", {ARG_INT}, {TYPE_CHAR}},                                                   // 0x5E0
    {false, "TASK_FOLLOW_LEADER_IN_FORMATION", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},  // 0x5E1
    {true, "TASK_KILL_CHAR_ON_FOOT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                              // 0x5E2
    {false, "START_ADDING_STUNT_POINTS", {}, {}},                                                              // 0x5E3
    {false, "ADD_STUNT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                         // 0x5E4
    {false, "START_PLAYING_STUNT", {ARG_FLOAT}, {}},                                                           // 0x5E5
    {false, "HAS_STUNT_ENDED", {}, {}},                                                                        // 0x5E6
    {false, "HAS_STUNT_FAILED", {}, {}},                                                                       // 0x5E7
    {false, "START_RECORDING_STUNT", {}, {}},                                                                  // 0x5E8
    {true, "START_RECORDING_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                             // 0x5E9
    {true, "STOP_RECORDING_CARS", {}, {}},                                                                     // 0x5EA
    {true, "START_PLAYBACK_RECORDED_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                     // 0x5EB
    {true, "STOP_PLAYBACK_RECORDED_CAR", {ARG_INT}, {TYPE_CAR}},                                               // 0x5EC
    {true, "PAUSE_PLAYBACK_RECORDED_CAR", {ARG_INT}, {TYPE_CAR}},                                              // 0x5ED
    {true, "UNPAUSE_PLAYBACK_RECORDED_CAR", {ARG_INT}, {TYPE_CAR}},                                            // 0x5EE
    {false, "SET_CAR_PROTECT_CAR_REAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                             // 0x5EF
    {false, "SET_CAR_PROTECT_CAR_FRONT", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                            // 0x5F0
    {true, "SET_CAR_ESCORT_CAR_LEFT", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                               // 0x5F1
    {true, "SET_CAR_ESCORT_CAR_RIGHT", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                              // 0x5F2
    {true, "SET_CAR_ESCORT_CAR_REAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                               // 0x5F3
    {true, "SET_CAR_ESCORT_CAR_FRONT", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                              // 0x5F4
    {true, "TASK_FOLLOW_PATH_NODES_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x5F5
    {true, "IS_CHAR_IN_ANGLED_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                          // 0x5F6
    {true, "IS_CHAR_IN_ANGLED_AREA_ON_FOOT_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},               // 0x5F7
    {true, "IS_CHAR_IN_ANGLED_AREA_IN_CAR_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},               // 0x5F8
    {false, "IS_CHAR_STOPPED_IN_ANGLED_AREA_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},               // 0x5F9
    {false, "IS_CHAR_STOPPED_IN_ANGLED_AREA_ON_FOOT_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},               // 0x5FA
    {false, "IS_CHAR_STOPPED_IN_ANGLED_AREA_IN_CAR_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},               // 0x5FB
    {true, "IS_CHAR_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},  // 0x5FC
    {true, "IS_CHAR_IN_ANGLED_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},  // 0x5FD
    {true, "IS_CHAR_IN_ANGLED_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},  // 0x5FE
    {false, "IS_CHAR_STOPPED_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},  // 0x5FF
    {false, "IS_CHAR_STOPPED_IN_ANGLED_AREA_ON_FOOT_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},  // 0x600
    {false, "IS_CHAR_STOPPED_IN_ANGLED_AREA_IN_CAR_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                    // 0x601
    {true, "IS_CHAR_IN_TAXI", {ARG_INT}, {TYPE_CHAR}},                                                   // 0x602
    {true, "TASK_GO_TO_COORD_ANY_MEANS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x603
    {true, "GET_HEADING_FROM_VECTOR_2D", {ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                  // 0x604
    {true, "TASK_PLAY_ANIM",
        {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                                                        // 0x605
    {true, "LOAD_PATH_NODES_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                     // 0x606
    {true, "RELEASE_PATH_NODES", {}, {}},                                                                    // 0x607
    {false, "HAVE_PATH_NODES_BEEN_LOADED", {}, {}},                                                          // 0x608
    {false, "LOAD_ALL_PATH_NODES_FOR_DEBUG", {}, {}},                                                        // 0x609
    {true, "LOAD_CHAR_DECISION_MAKER", {ARG_INT, ARG_RETURN_INT}, {TYPE_UNIMPORTANT, TYPE_DECISION_MAKER}},  // 0x60A
    {true, "SET_CHAR_DECISION_MAKER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_DECISION_MAKER}},                 // 0x60B
    {false, "CLEAR_ALL_DECISION_MAKERS", {}, {}},                                                            // 0x60C
    {true, "SET_TEXT_DROPSHADOW", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                        // 0x60D
    {true, "IS_PLAYBACK_GOING_ON_FOR_CAR", {ARG_INT}, {TYPE_CAR}},                                           // 0x60E
    {true, "SET_SENSE_RANGE", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                            // 0x60F
    {false, "SET_HEARING_RANGE", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                         // 0x610
    {true, "IS_CHAR_PLAYING_ANIM", {ARG_INT, ARG_TEXT_LABEL16}, {TYPE_CHAR}},                                // 0x611
    {true, "SET_CHAR_ANIM_PLAYING_FLAG", {ARG_INT, ARG_TEXT_LABEL16, ARG_INT}, {TYPE_CHAR}},                 // 0x612
    {true, "GET_CHAR_ANIM_CURRENT_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_RETURN_FLOAT}, {TYPE_CHAR}},        // 0x613
    {true, "SET_CHAR_ANIM_CURRENT_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_FLOAT}, {TYPE_CHAR}},               // 0x614
    {true, "OPEN_SEQUENCE_TASK", {ARG_RETURN_INT}, {TYPE_SEQUENCE_TASK}},                                    // 0x615
    {true, "CLOSE_SEQUENCE_TASK", {ARG_INT}, {TYPE_SEQUENCE_TASK}},                                          // 0x616
    {false, "SCRIPT_EVENT", {ARG_INT}, {TYPE_CHAR}},                                                         // 0x617
    {true, "PERFORM_SEQUENCE_TASK", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_SEQUENCE_TASK}},                    // 0x618
    {true, "SET_CHAR_COLLISION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x619
    {true, "GET_CHAR_ANIM_TOTAL_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_RETURN_FLOAT}, {TYPE_CHAR}},          // 0x61A
    {true, "CLEAR_SEQUENCE_TASK", {ARG_INT}, {TYPE_SEQUENCE_TASK}},                                          // 0x61B
    {false, "CLEAR_ALL_SEQUENCE_TASKS", {}, {}},                                                             // 0x61C
    {true, "ADD_ATTRACTOR", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_SEQUENCE_TASK,
            TYPE_ATTRACTOR}},                                                                // 0x61D
    {true, "CLEAR_ATTRACTOR", {ARG_INT}, {TYPE_ATTRACTOR}},                                  // 0x61E
    {false, "CLEAR_ALL_ATTRACTORS", {}, {}},                                                 // 0x61F
    {false, "TASK_PLAY_ANIM_FOR_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                                        // 0x620
    {true, "CREATE_CHAR_AT_ATTRACTOR", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_ATTRACTOR, TYPE_UNIMPORTANT, TYPE_CHAR}},  // 0x621
    {true, "TASK_LEAVE_CAR_IMMEDIATELY", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},         // 0x622
    {true, "INCREMENT_INT_STAT", {ARG_INT, ARG_INT}, {}},                                    // 0x623
    {true, "INCREMENT_FLOAT_STAT", {ARG_INT, ARG_FLOAT}, {}},                                // 0x624
    {true, "DECREMENT_INT_STAT", {ARG_INT, ARG_INT}, {}},                                    // 0x625
    {true, "DECREMENT_FLOAT_STAT", {ARG_INT, ARG_FLOAT}, {}},                                // 0x626
    {true, "REGISTER_INT_STAT", {ARG_INT, ARG_INT}, {}},                                     // 0x627
    {true, "REGISTER_FLOAT_STAT", {ARG_INT, ARG_FLOAT}, {}},                                 // 0x628
    {true, "SET_INT_STAT", {ARG_INT, ARG_INT}, {}},                                          // 0x629
    {true, "SET_FLOAT_STAT", {ARG_INT, ARG_FLOAT}, {}},                                      // 0x62A
    {false, "GET_ATTEMPTS_FOR_THIS_MISSION", {ARG_INT, ARG_RETURN_INT}, {}},                 // 0x62B
    {false, "REGISTER_THIS_MISSION_HAS_BEEN_ATTEMPTED", {ARG_INT}, {}},                      // 0x62C
    {false, "REGISTER_THIS_MISSION_HAS_BEEN_PASSED", {ARG_INT}, {}},                         // 0x62D
    {true, "GET_SCRIPT_TASK_STATUS", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},       // 0x62E
    {true, "CREATE_GROUP", {ARG_INT, ARG_RETURN_INT}, {TYPE_UNIMPORTANT, TYPE_GROUP}},       // 0x62F
    {true, "SET_GROUP_LEADER", {ARG_INT, ARG_INT}, {TYPE_GROUP, TYPE_CHAR}},                 // 0x630
    {true, "SET_GROUP_MEMBER", {ARG_INT, ARG_INT}, {TYPE_GROUP, TYPE_CHAR}},                 // 0x631
    {true, "REMOVE_GROUP", {ARG_INT}, {TYPE_GROUP}},                                         // 0x632
    {true, "TASK_LEAVE_ANY_CAR", {ARG_INT}, {TYPE_CHAR}},                                    // 0x633
    {true, "TASK_KILL_CHAR_ON_FOOT_WHILE_DUCKING", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR}},                                                             // 0x634
    {true, "TASK_AIM_GUN_AT_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},     // 0x635
    {false, "TASK_SIDE_STEP_AND_SHOOT",
        {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR, TYPE_CHAR}},  // 0x636
    {true, "TASK_GO_TO_COORD_WHILE_SHOOTING",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                                                             // 0x637
    {true, "TASK_STAY_IN_SAME_PLACE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                // 0x638
    {true, "TASK_TURN_CHAR_TO_FACE_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                 // 0x639
    {false, "OPEN_THREAT_LIST", {ARG_RETURN_INT}, {TYPE_THREAT_LIST}},                                 // 0x63A
    {false, "CLOSE_THREAT_LIST", {ARG_INT}, {TYPE_THREAT_LIST}},                                       // 0x63B
    {false, "SET_PEDMODEL_AS_THREAT", {ARG_INT, ARG_INT, ARG_INT}, {}},                                // 0x63C
    {false, "SET_CHAR_THREAT_LIST", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_THREAT_LIST}},                // 0x63D
    {false, "REMOVE_THREAT_LIST", {ARG_INT}, {TYPE_THREAT_LIST}},                                      // 0x63E
    {false, "PERFORM_SEQUENCE_TASK_REPEATEDLY", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_SEQUENCE_TASK}},  // 0x63F
    {false, "SET_PEDTYPE_AS_THREAT", {ARG_INT, ARG_INT, ARG_INT}, {}},                                 // 0x640
    {false, "CLEAR_CHAR_THREATS", {ARG_INT}, {TYPE_CHAR}},                                             // 0x641
    {true, "IS_CHAR_AT_SCRIPTED_ATTRACTOR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_ATTRACTOR}},          // 0x642
    {true, "SET_SEQUENCE_TO_REPEAT", {ARG_INT, ARG_INT}, {TYPE_SEQUENCE_TASK}},                        // 0x643
    {false, "CREATE_PED_GENERATOR",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_PED_GENERATOR}},                                   // 0x644
    {false, "SWITCH_PED_GENERATOR", {ARG_INT, ARG_INT}, {TYPE_PED_GENERATOR}},        // 0x645
    {true, "GET_SEQUENCE_PROGRESS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},          // 0x646
    {true, "CLEAR_LOOK_AT", {ARG_INT}, {TYPE_CHAR}},                                  // 0x647
    {true, "SET_FOLLOW_NODE_THRESHOLD_DISTANCE", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},  // 0x648
    {false, "SET_CHAR_ZONE_DISTANCE", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},    // 0x649
    {false, "ADD_PEDMODEL_AS_ATTRACTOR_USER", {ARG_INT, ARG_INT}, {TYPE_ATTRACTOR}},  // 0x64A
    {true, "CREATE_FX_SYSTEM", {ARG_TEXT_LABEL16, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_FX_SYSTEM}},                                        // 0x64B
    {true, "PLAY_FX_SYSTEM", {ARG_INT}, {TYPE_FX_SYSTEM}},           // 0x64C
    {false, "PAUSE_FX_SYSTEM", {ARG_INT}, {TYPE_FX_SYSTEM}},         // 0x64D
    {true, "STOP_FX_SYSTEM", {ARG_INT}, {TYPE_FX_SYSTEM}},           // 0x64E
    {true, "PLAY_AND_KILL_FX_SYSTEM", {ARG_INT}, {TYPE_FX_SYSTEM}},  // 0x64F
    {true, "KILL_FX_SYSTEM", {ARG_INT}, {TYPE_FX_SYSTEM}},           // 0x650
    {false, "CREATE_FX_SYSTEM_WITH_DIRECTION",
        {ARG_TEXT_LABEL16, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_FX_SYSTEM}},                                    // 0x651
    {true, "GET_INT_STAT", {ARG_INT, ARG_RETURN_INT}, {}},                                           // 0x652
    {true, "GET_FLOAT_STAT", {ARG_INT, ARG_RETURN_FLOAT}, {}},                                       // 0x653
    {true, "SET_OBJECT_RENDER_SCORCHED", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                         // 0x654
    {true, "TASK_LOOK_AT_OBJECT", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},            // 0x655
    {true, "LIMIT_ANGLE", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                        // 0x656
    {true, "OPEN_CAR_DOOR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                         // 0x657
    {false, "SET_GROUP_DEFAULT_LEADER_TASK", {ARG_INT, ARG_INT}, {TYPE_GROUP, TYPE_SEQUENCE_TASK}},  // 0x658
    {false, "SET_ATTRACTOR_PAIR",
        {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_ATTRACTOR, TYPE_SEQUENCE_TASK, TYPE_SEQUENCE_TASK, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_ATTRACTOR,
            TYPE_SEQUENCE_TASK, TYPE_SEQUENCE_TASK}},                                                   // 0x659
    {false, "PLACE_CHAR_AT_ATTRACTOR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_ATTRACTOR}},                // 0x65A
    {true, "GET_PICKUP_COORDINATES", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_PICKUP}},                                                                                 // 0x65B
    {true, "REMOVE_DECISION_MAKER", {ARG_INT}, {TYPE_DECISION_MAKER}},                                  // 0x65C
    {true, "VIEW_INTEGER_VARIABLE", {ARG_VAR_INT, ARG_TEXT_LABEL16}, {}},                               // 0x65D
    {true, "VIEW_FLOAT_VARIABLE", {ARG_VAR_FLOAT, ARG_TEXT_LABEL16}, {}},                               // 0x65E
    {true, "WATCH_INTEGER_VARIABLE", {ARG_VAR_INT, ARG_TEXT_LABEL16}, {}},                              // 0x65F
    {true, "WATCH_FLOAT_VARIABLE", {ARG_VAR_FLOAT, ARG_TEXT_LABEL16}, {}},                              // 0x660
    {true, "BREAKPOINT", {ARG_TEXT_LABEL16}, {}},                                                       // 0x661
    {true, "WRITE_DEBUG", {ARG_TEXT_LABEL16}, {}},                                                      // 0x662
    {true, "WRITE_DEBUG_WITH_INT", {ARG_TEXT_LABEL16, ARG_INT}, {}},                                    // 0x663
    {true, "WRITE_DEBUG_WITH_FLOAT", {ARG_TEXT_LABEL16, ARG_FLOAT}, {}},                                // 0x664
    {true, "GET_CHAR_MODEL", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                   // 0x665
    {false, "IS_CHAR_TOUCHING_ANY_OBJECT", {ARG_INT}, {TYPE_CHAR}},                                     // 0x666
    {true, "TASK_AIM_GUN_AT_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},  // 0x667
    {true, "TASK_SHOOT_AT_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},    // 0x668
    {true, "CREATE_FX_SYSTEM_ON_CHAR",
        {ARG_TEXT_LABEL16, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_FX_SYSTEM}},  // 0x669
    {true, "CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION",
        {ARG_TEXT_LABEL16, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT,
            ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_FX_SYSTEM}},  // 0x66A
    {true, "CREATE_FX_SYSTEM_ON_CAR",
        {ARG_TEXT_LABEL16, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_FX_SYSTEM}},  // 0x66B
    {true, "CREATE_FX_SYSTEM_ON_CAR_WITH_DIRECTION",
        {ARG_TEXT_LABEL16, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT,
            ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_FX_SYSTEM}},  // 0x66C
    {true, "CREATE_FX_SYSTEM_ON_OBJECT",
        {ARG_TEXT_LABEL16, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_FX_SYSTEM}},  // 0x66D
    {true, "CREATE_FX_SYSTEM_ON_OBJECT_WITH_DIRECTION",
        {ARG_TEXT_LABEL16, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT,
            ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_FX_SYSTEM}},                     // 0x66E
    {false, "ADD_QUEUED_DIALOGUE", {ARG_TEXT_LABEL16, ARG_INT}, {}},                                    // 0x66F
    {false, "IS_DIALOGUE_FINISHED", {ARG_TEXT_LABEL16}, {}},                                            // 0x670
    {false, "IS_DIALOGUE_PLAYING", {ARG_TEXT_LABEL16}, {}},                                             // 0x671
    {true, "TASK_DESTROY_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                              // 0x672
    {true, "TASK_DIVE_AND_GET_UP", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},              // 0x673
    {true, "CUSTOM_PLATE_FOR_NEXT_CAR", {ARG_INT, ARG_TEXT_LABEL16}, {}},                               // 0x674
    {false, "CREATE_PED_GENERATOR_AT_ATTRACTOR", {}, {}},                                               // 0x675
    {true, "TASK_SHUFFLE_TO_NEXT_CAR_SEAT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                 // 0x676
    {true, "TASK_CHAT_WITH_CHAR", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},        // 0x677
    {false, "GET_CHAR_AT_SCRIPTED_ATTRACTOR", {ARG_INT, ARG_RETURN_INT}, {TYPE_ATTRACTOR, TYPE_CHAR}},  // 0x678
    {true, "ATTACH_CAMERA_TO_VEHICLE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR}},  // 0x679
    {true, "ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_VEHICLE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x67A
    {true, "ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_CHAR",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},  // 0x67B
    {true, "ATTACH_CAMERA_TO_CHAR",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},  // 0x67C
    {true, "ATTACH_CAMERA_TO_CHAR_LOOK_AT_VEHICLE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x67D
    {true, "ATTACH_CAMERA_TO_CHAR_LOOK_AT_CHAR",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},                    // 0x67E
    {true, "FORCE_CAR_LIGHTS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                           // 0x67F
    {true, "ADD_PEDTYPE_AS_ATTRACTOR_USER", {ARG_INT, ARG_INT}, {TYPE_ATTRACTOR}},                        // 0x680
    {true, "ATTACH_OBJECT_TO_CAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_OBJECT, TYPE_CAR}},                                                                         // 0x681
    {true, "DETACH_OBJECT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_OBJECT}},          // 0x682
    {true, "ATTACH_CAR_TO_CAR", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CAR, TYPE_CAR}},                                                                            // 0x683
    {true, "DETACH_CAR", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CAR}},                // 0x684
    {true, "IS_OBJECT_ATTACHED", {ARG_INT}, {TYPE_OBJECT}},                                               // 0x685
    {true, "IS_VEHICLE_ATTACHED", {ARG_INT}, {TYPE_CAR}},                                                 // 0x686
    {true, "CLEAR_CHAR_TASKS", {ARG_INT}, {TYPE_CHAR}},                                                   // 0x687
    {true, "TASK_TOGGLE_PED_THREAT_SCANNER", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},          // 0x688
    {true, "POP_CAR_DOOR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                      // 0x689
    {true, "FIX_CAR_DOOR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0x68A
    {true, "TASK_EVERYONE_LEAVE_CAR", {ARG_INT}, {TYPE_CAR}},                                             // 0x68B
    {true, "IS_PLAYER_TARGETTING_ANYTHING", {ARG_INT}, {TYPE_PLAYER}},                                    // 0x68C
    {true, "GET_ACTIVE_CAMERA_COORDINATES", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},  // 0x68D
    {true, "GET_ACTIVE_CAMERA_POINT_AT", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},     // 0x68E
    {false, "GET_CLOSEST_BUYABLE_OBJECT_TO_PLAYER",
        {ARG_FLOAT, ARG_RETURN_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},           // 0x68F
    {false, "OPEN_FRIEND_LIST", {ARG_RETURN_INT}, {TYPE_FRIEND_LIST}},                                    // 0x690
    {false, "CLOSE_FRIEND_LIST", {ARG_INT}, {TYPE_FRIEND_LIST}},                                          // 0x691
    {false, "REMOVE_FRIEND_LIST", {ARG_INT}, {TYPE_FRIEND_LIST}},                                         // 0x692
    {false, "SET_PEDMODEL_AS_FRIEND", {ARG_INT, ARG_INT, ARG_INT}, {}},                                   // 0x693
    {false, "SET_PEDTYPE_AS_FRIEND", {ARG_INT, ARG_INT, ARG_INT}, {}},                                    // 0x694
    {false, "CLEAR_CHAR_FRIENDS", {ARG_INT}, {TYPE_CHAR}},                                                // 0x695
    {false, "SET_CHAR_FRIEND_LIST", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_FRIEND_LIST}},                   // 0x696
    {true, "POP_CAR_PANEL", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                     // 0x697
    {true, "FIX_CAR_PANEL", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                              // 0x698
    {true, "FIX_CAR_TYRE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0x699
    {true, "ATTACH_OBJECT_TO_OBJECT",
        {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_OBJECT, TYPE_OBJECT}},  // 0x69A
    {true, "ATTACH_OBJECT_TO_CHAR",
        {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_OBJECT, TYPE_CHAR}},  // 0x69B
    {false, "ATTACH_CAMERA_TO_OBJECT",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},  // 0x69C
    {false, "ATTACH_CAMERA_TO_OBJECT_LOOK_AT_VEHICLE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x69D
    {false, "ATTACH_CAMERA_TO_OBJECT_LOOK_AT_CHAR",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},  // 0x69E
    {true, "ATTACH_CAMERA_TO_OBJECT_LOOK_AT_OBJECT",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT}},  // 0x69F
    {false, "ATTACH_CAMERA_TO_CHAR_LOOK_AT_OBJECT",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT}},  // 0x6A0
    {false, "ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_OBJECT",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT}},                     // 0x6A1
    {true, "GET_CAR_SPEED_VECTOR", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CAR}},                                                                                        // 0x6A2
    {true, "GET_CAR_MASS", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                        // 0x6A3
    {false, "TASK_KILL_THREATS_ON_FOOT_WHILE_DUCKING", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},  // 0x6A4
    {true, "TASK_DIVE_FROM_ATTACHMENT_AND_GET_UP", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                        // 0x6A5
    {false, "TASK_PLAY_ANIM_WITH_VELOCITY_EXTRACTION",
        {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},  // 0x6A6
    {true, "ATTACH_CHAR_TO_BIKE",
        {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                               // 0x6A7
    {true, "TASK_GOTO_CHAR_OFFSET", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                              // 0x6A8
    {true, "TASK_LOOK_AT_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},           // 0x6A9
    {true, "IS_RECORDING_GOING_ON_FOR_CAR", {ARG_INT}, {TYPE_CAR}},                                           // 0x6AA
    {true, "HIDE_CHAR_WEAPON_FOR_SCRIPTED_CUTSCENE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                        // 0x6AB
    {true, "GET_CHAR_SPEED", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},                                       // 0x6AC
    {true, "SET_GROUP_DECISION_MAKER", {ARG_INT, ARG_INT}, {TYPE_GROUP, TYPE_DECISION_MAKER}},                // 0x6AD
    {true, "LOAD_GROUP_DECISION_MAKER", {ARG_INT, ARG_RETURN_INT}, {TYPE_UNIMPORTANT, TYPE_DECISION_MAKER}},  // 0x6AE
    {true, "DISABLE_PLAYER_SPRINT", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                       // 0x6AF
    {true, "TASK_SIT_DOWN", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                 // 0x6B0
    {true, "CREATE_SEARCHLIGHT",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_SEARCHLIGHT}},   // 0x6B1
    {true, "DELETE_SEARCHLIGHT", {ARG_INT}, {TYPE_SEARCHLIGHT}},      // 0x6B2
    {true, "DOES_SEARCHLIGHT_EXIST", {ARG_INT}, {TYPE_SEARCHLIGHT}},  // 0x6B3
    {true, "MOVE_SEARCHLIGHT_BETWEEN_COORDS",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_SEARCHLIGHT}},                                                                            // 0x6B4
    {true, "POINT_SEARCHLIGHT_AT_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_SEARCHLIGHT}},                                                                            // 0x6B5
    {true, "POINT_SEARCHLIGHT_AT_CHAR", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_SEARCHLIGHT, TYPE_CHAR}},  // 0x6B6
    {true, "IS_CHAR_IN_SEARCHLIGHT", {ARG_INT, ARG_INT}, {TYPE_SEARCHLIGHT, TYPE_CHAR}},                // 0x6B7
    {false, "SET_GROUP_DEFAULT_TASK", {ARG_INT, ARG_INT}, {TYPE_GROUP}},                                // 0x6B8
    {true, "HAS_CUTSCENE_LOADED", {}, {}},                                                              // 0x6B9
    {true, "TASK_TURN_CHAR_TO_FACE_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},    // 0x6BA
    {true, "TASK_DRIVE_POINT_ROUTE", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR, TYPE_CAR}},             // 0x6BB
    {true, "FIRE_SINGLE_BULLET", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                            // 0x6BC
    {true, "IS_LINE_OF_SIGHT_CLEAR",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                              // 0x6BD
    {true, "GET_CAR_ROLL", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                      // 0x6BE
    {true, "POINT_SEARCHLIGHT_AT_VEHICLE", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_SEARCHLIGHT, TYPE_CAR}},  // 0x6BF
    {true, "IS_VEHICLE_IN_SEARCHLIGHT", {ARG_INT, ARG_INT}, {TYPE_SEARCHLIGHT, TYPE_CAR}},                // 0x6C0
    {true, "CREATE_SEARCHLIGHT_ON_VEHICLE",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT,
            ARG_RETURN_INT},
        {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_SEARCHLIGHT}},  // 0x6C1
    {true, "TASK_GO_TO_COORD_WHILE_AIMING",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_FLOAT,
            ARG_FLOAT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                                                                     // 0x6C2
    {true, "GET_NUMBER_OF_FIRES_IN_RANGE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},  // 0x6C3
    {true, "ADD_BLIP_FOR_SEARCHLIGHT", {ARG_INT, ARG_RETURN_INT}, {TYPE_SEARCHLIGHT, TYPE_BLIP}},              // 0x6C4
    {true, "SKIP_TO_END_AND_STOP_PLAYBACK_RECORDED_CAR", {ARG_INT}, {TYPE_CAR}},                               // 0x6C5
    {false, "TASK_OPEN_DRIVER_DOOR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                               // 0x6C6
    {true, "TASK_CAR_TEMP_ACTION", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},               // 0x6C7
    {true, "SET_LA_RIOTS", {ARG_INT}, {}},                                                                     // 0x6C8
    {true, "REMOVE_CHAR_FROM_GROUP", {ARG_INT}, {TYPE_CHAR}},                                                  // 0x6C9
    {true, "ATTACH_SEARCHLIGHT_TO_SEARCHLIGHT_OBJECT",
        {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_SEARCHLIGHT, TYPE_OBJECT, TYPE_OBJECT, TYPE_OBJECT}},                                      // 0x6CA
    {false, "SET_VEHICLE_RECORDS_COLLISIONS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                           // 0x6CB
    {false, "DRAW_CROSS_IN_FRONT_OF_DEBUG_CAMERA", {ARG_FLOAT}, {}},                                     // 0x6CC
    {false, "DRAW_DEBUG_CUBE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                   // 0x6CD
    {false, "GET_CAR_LAST_ROUTE_COORDS",
        {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {TYPE_CAR}},  // 0x6CE
    {true, "DISPLAY_TIMER_BARS", {ARG_INT}, {}},                                                         // 0x6CF
    {true, "SWITCH_EMERGENCY_SERVICES", {ARG_INT}, {}},                                                  // 0x6D0
    {true, "SET_VAR_TEXT_LABEL16", {ARG_VAR_TEXT_LABEL16, ARG_TEXT_LABEL16}, {}},                        // 0x6D1
    {true, "SET_LVAR_TEXT_LABEL16", {ARG_LVAR_TEXT_LABEL16, ARG_TEXT_LABEL16}, {}},                      // 0x6D2
    {true, "VAR_TEXT_LABEL16",
        {ARG_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16,
            ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16,
            ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16,
            ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16,
            ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16,
            ARG_DECLARE_VAR_TEXT_LABEL16, ARG_DECLARE_VAR_TEXT_LABEL16},
        {}},  // 0x6D3
    {true, "LVAR_TEXT_LABEL16",
        {ARG_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16,
            ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16,
            ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16,
            ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16,
            ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16,
            ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16, ARG_DECLARE_LVAR_TEXT_LABEL16},
        {}},  // 0x6D4
    {true, "CREATE_CHECKPOINT",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHECKPOINT}},                                              // 0x6D5
    {true, "DELETE_CHECKPOINT", {ARG_INT}, {TYPE_CHECKPOINT}},                                                  // 0x6D6
    {true, "SWITCH_RANDOM_TRAINS", {ARG_INT}, {}},                                                              // 0x6D7
    {true, "CREATE_MISSION_TRAIN", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x6D8
    {true, "DELETE_MISSION_TRAINS", {}, {}},                                                                    // 0x6D9
    {true, "MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED", {}, {}},                                                  // 0x6DA
    {true, "DELETE_ALL_TRAINS", {}, {}},                                                                        // 0x6DB
    {true, "SET_TRAIN_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                                // 0x6DC
    {true, "SET_TRAIN_CRUISE_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                         // 0x6DD
    {true, "GET_TRAIN_CABOOSE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_CAR}},                               // 0x6DE
    {true, "DELETE_PLAYER", {ARG_INT}, {TYPE_PLAYER}},                                                          // 0x6DF
    {true, "SET_TWO_PLAYER_CAMERA_MODE", {ARG_INT}, {}},                                                        // 0x6E0
    {true, "TASK_CAR_MISSION", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR, TYPE_CAR}},                                                                       // 0x6E1
    {true, "TASK_GO_TO_OBJECT", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR, TYPE_OBJECT}},              // 0x6E2
    {true, "TASK_WEAPON_ROLL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                // 0x6E3
    {true, "TASK_CHAR_ARREST_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                                // 0x6E4
    {true, "GET_AVAILABLE_VEHICLE_MOD", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                        // 0x6E5
    {true, "GET_VEHICLE_MOD_TYPE", {ARG_INT, ARG_RETURN_INT}, {}},                                              // 0x6E6
    {true, "ADD_VEHICLE_MOD", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                  // 0x6E7
    {true, "REMOVE_VEHICLE_MOD", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0x6E8
    {true, "REQUEST_VEHICLE_MOD", {ARG_INT}, {}},                                                               // 0x6E9
    {true, "HAS_VEHICLE_MOD_LOADED", {ARG_INT}, {}},                                                            // 0x6EA
    {true, "MARK_VEHICLE_MOD_AS_NO_LONGER_NEEDED", {ARG_INT}, {}},                                              // 0x6EB
    {true, "GET_NUM_AVAILABLE_PAINTJOBS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                               // 0x6EC
    {true, "GIVE_VEHICLE_PAINTJOB", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x6ED
    {true, "IS_GROUP_MEMBER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_GROUP}},                                     // 0x6EE
    {true, "IS_GROUP_LEADER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_GROUP}},                                     // 0x6EF
    {true, "SET_GROUP_SEPARATION_RANGE", {ARG_INT, ARG_FLOAT}, {TYPE_GROUP}},                                   // 0x6F0
    {true, "LIMIT_TWO_PLAYER_DISTANCE", {ARG_FLOAT}, {}},                                                       // 0x6F1
    {false, "RELEASE_TWO_PLAYER_DISTANCE", {}, {}},                                                             // 0x6F2
    {true, "SET_PLAYER_PLAYER_TARGETTING", {ARG_INT}, {}},                                                      // 0x6F3
    {false, "CREATE_SCRIPT_GANG_ROADBLOCK", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x6F4
    {true, "GET_SCRIPT_FIRE_COORDS", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_SCRIPT_FIRE}},                                                                                    // 0x6F5
    {false, "CLEAR_TWO_PLAYER_CAMERA_MODE", {}, {}},                                                            // 0x6F6
    {false, "SET_PLAYER_PASSENGER_CAN_SHOOT", {ARG_INT}, {}},                                                   // 0x6F7
    {true, "GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT},
        {}},                                                                                                    // 0x6F8
    {false, "GET_HEIGHT_OF_CAR_WHEELS_FROM_GROUND",
        {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {TYPE_CAR}},         // 0x6F9
    {true, "SET_PLAYERS_CAN_BE_IN_SEPARATE_CARS", {ARG_INT}, {}},                                               // 0x6FA
    {false, "SWITCH_PLAYER_CROSSHAIR", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                      // 0x6FB
    {true, "DOES_CAR_HAVE_STUCK_CAR_CHECK", {ARG_INT}, {TYPE_CAR}},                                             // 0x6FC
    {true, "SET_PLAYBACK_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                             // 0x6FD
    {false, "GET_CAR_VALUE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                            // 0x6FE
    {true, "ARE_ANY_CHARS_NEAR_CHAR", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                       // 0x6FF
    {true, "SKIP_CUTSCENE_START", {}, {}},                                                                      // 0x700
    {true, "SKIP_CUTSCENE_END", {}, {}},                                                                        // 0x701
    {true, "GET_PERCENTAGE_TAGGED_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},  // 0x702
    {true, "SET_TAG_STATUS_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},                // 0x703
    {true, "CAR_GOTO_COORDINATES_RACING", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},              // 0x704
    {true, "START_PLAYBACK_RECORDED_CAR_USING_AI", {ARG_INT, ARG_INT}, {TYPE_CAR}},                             // 0x705
    {true, "SKIP_IN_PLAYBACK_RECORDED_CAR", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                  // 0x706
    {true, "SKIP_CUTSCENE_START_INTERNAL", {ARG_LABEL}, {}},                                                    // 0x707
    {true, "CLEAR_CHAR_DECISION_MAKER_EVENT_RESPONSE", {ARG_INT, ARG_INT}, {TYPE_DECISION_MAKER}},              // 0x708
    {true, "ADD_CHAR_DECISION_MAKER_EVENT_RESPONSE",
        {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_DECISION_MAKER}},  // 0x709
    {true, "TASK_PICK_UP_OBJECT",
        {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16,
            ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                           // 0x70A
    {true, "DROP_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                  // 0x70B
    {true, "EXPLODE_CAR_IN_CUTSCENE", {ARG_INT}, {TYPE_CAR}},                                                // 0x70C
    {true, "BUILD_PLAYER_MODEL", {ARG_INT}, {TYPE_PLAYER}},                                                  // 0x70D
    {true, "PLANE_ATTACK_PLAYER", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR, TYPE_PLAYER}},                   // 0x70E
    {true, "PLANE_FLY_IN_DIRECTION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},                // 0x70F
    {true, "PLANE_FOLLOW_ENTITY", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR, TYPE_CHAR, TYPE_CAR}},  // 0x710
    {false, "ALLOCATE_SCRIPT_TO_PED_GENERATOR", {ARG_LABEL, ARG_INT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_PED_GENERATOR}},                                                             // 0x711
    {false, "ALLOCATE_SCRIPT_TO_RANDOM_PED", {ARG_LABEL, ARG_INT, ARG_INT}, {}},                             // 0x712
    {true, "TASK_DRIVE_BY",
        {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR, TYPE_CAR}},                                                                      // 0x713
    {true, "SET_CAR_STAY_IN_SLOW_LANE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                        // 0x714
    {true, "TAKE_REMOTE_CONTROL_OF_CAR", {ARG_INT, ARG_INT}, {TYPE_PLAYER, TYPE_CAR}},                          // 0x715
    {true, "IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT}, {}},                           // 0x716
    {true, "START_SETTING_UP_CONVERSATION", {ARG_INT}, {TYPE_CHAR}},                                            // 0x717
    {false, "SET_UP_CONVERSATION_NODE", {ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL}, {}},                  // 0x718
    {true, "FINISH_SETTING_UP_CONVERSATION", {}, {}},                                                           // 0x719
    {true, "IS_CONVERSATION_AT_NODE", {ARG_INT, ARG_TEXT_LABEL}, {TYPE_CHAR}},                                  // 0x71A
    {false, "CLEAR_ALL_CONVERSATIONS", {}, {}},                                                                 // 0x71B
    {false, "GET_CHAR_LIGHTING", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},                                     // 0x71C
    {false, "SET_CLOSEST_OBJECT_OF_TYPE_RENDER_SCORCHED",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},                                    // 0x71D
    {true, "GET_OBJECT_HEALTH", {ARG_INT, ARG_RETURN_INT}, {TYPE_OBJECT}},                                      // 0x71E
    {true, "SET_OBJECT_HEALTH", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                             // 0x71F
    {false, "GET_VEHICLE_WHEEL_UPGRADE_CLASS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                          // 0x720
    {false, "GET_NUM_WHEELS_IN_UPGRADE_CLASS", {ARG_INT, ARG_RETURN_INT}, {}},                                  // 0x721
    {false, "GET_WHEEL_IN_UPGRADE_CLASS", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {}},                              // 0x722
    {true, "BREAK_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                                  // 0x723
    {true, "HELI_ATTACK_PLAYER", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR, TYPE_PLAYER}},                       // 0x724
    {false, "HELI_FLY_IN_DIRECTION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},                   // 0x725
    {true, "HELI_FOLLOW_ENTITY", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR, TYPE_CHAR, TYPE_CAR}},      // 0x726
    {true, "POLICE_HELI_CHASE_ENTITY", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT},
        {TYPE_CAR, TYPE_CHAR, TYPE_CAR}},                                                                       // 0x727
    {false, "SET_UP_CONVERSATION_END_NODE", {ARG_TEXT_LABEL}, {}},                                              // 0x728
    {true, "TASK_USE_MOBILE_PHONE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x729
    {true, "TASK_WARP_CHAR_INTO_CAR_AS_DRIVER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                     // 0x72A
    {true, "TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},         // 0x72B
    {true, "SWITCH_COPS_ON_BIKES", {ARG_INT}, {}},                                                              // 0x72C
    {true, "IS_FLAME_IN_ANGLED_AREA_2D", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                                    // 0x72D
    {true, "IS_FLAME_IN_ANGLED_AREA_3D",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},            // 0x72E
    {true, "ADD_STUCK_CAR_CHECK_WITH_WARP", {ARG_INT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CAR}},                                                                                            // 0x72F
    {true, "DAMAGE_CAR_PANEL", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                 // 0x730
    {true, "SET_CAR_ROLL", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                                   // 0x731
    {true, "SUPPRESS_CAR_MODEL", {ARG_INT}, {}},                                                                // 0x732
    {true, "DONT_SUPPRESS_CAR_MODEL", {ARG_INT}, {}},                                                           // 0x733
    {true, "DONT_SUPPRESS_ANY_CAR_MODELS", {}, {}},                                                             // 0x734
    {true, "IS_PS2_KEYBOARD_KEY_PRESSED", {ARG_INT}, {}},                                                       // 0x735
    {true, "IS_PS2_KEYBOARD_KEY_JUST_PRESSED", {ARG_INT}, {}},                                                  // 0x736
    {true, "IS_CHAR_HOLDING_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},                             // 0x737
    {false, "SET_ZONE_RADAR_COLOURS", {ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                // 0x738
    {false, "GIVE_LOWRIDER_SUSPENSION_TO_CAR", {}, {}},                                                         // 0x739
    {false, "DOES_CAR_HAVE_LOWRIDER_SUSPENSION", {}, {}},                                                       // 0x73A
    {true, "SET_CAR_CAN_GO_AGAINST_TRAFFIC", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x73B
    {true, "DAMAGE_CAR_DOOR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                                  // 0x73C
    {false, "GET_RANDOM_CAR_IN_SPHERE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x73D
    {true, "GET_RANDOM_CAR_IN_SPHERE_NO_SAVE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x73E
    {true, "GET_RANDOM_CHAR_IN_SPHERE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                                                           // 0x73F
    {false, "GET_COORDS_OF_CLOSEST_COLLECTABLE1",
        {ARG_INT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},    // 0x740
    {true, "HAS_CHAR_BEEN_ARRESTED", {ARG_INT}, {TYPE_CHAR}},                                        // 0x741
    {true, "SET_PLANE_THROTTLE", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                  // 0x742
    {true, "HELI_LAND_AT_COORDS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CAR}},                                                                                 // 0x743
    {false, "GET_STAT_CHANGE_AMOUNT", {ARG_INT, ARG_RETURN_FLOAT}, {}},                              // 0x744
    {true, "PLANE_STARTS_IN_AIR", {ARG_INT}, {TYPE_CAR}},                                            // 0x745
    {true, "SET_RELATIONSHIP", {ARG_INT, ARG_INT, ARG_INT}, {}},                                     // 0x746
    {true, "CLEAR_RELATIONSHIP", {ARG_INT, ARG_INT, ARG_INT}, {}},                                   // 0x747
    {false, "CLEAR_ALL_RELATIONSHIPS", {ARG_INT, ARG_INT}, {}},                                      // 0x748
    {true, "CLEAR_GROUP_DECISION_MAKER_EVENT_RESPONSE", {ARG_INT, ARG_INT}, {TYPE_DECISION_MAKER}},  // 0x749
    {true, "ADD_GROUP_DECISION_MAKER_EVENT_RESPONSE",
        {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_DECISION_MAKER}},  // 0x74A
    {true, "DRAW_SPRITE_WITH_ROTATION",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {}},                                                                                                   // 0x74B
    {true, "TASK_USE_ATTRACTOR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_ATTRACTOR}},                             // 0x74C
    {true, "TASK_SHOOT_AT_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                         // 0x74D
    {true, "SET_INFORM_RESPECTED_FRIENDS", {ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR}},                        // 0x74E
    {true, "IS_CHAR_RESPONDING_TO_EVENT", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                    // 0x74F
    {true, "SET_OBJECT_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                           // 0x750
    {true, "TASK_FLEE_CHAR_ANY_MEANS", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT},
        {TYPE_CHAR, TYPE_CHAR}},                                                                               // 0x751
    {true, "STOP_RECORDING_CAR", {ARG_INT}, {TYPE_CAR}},                                                       // 0x752
    {false, "SET_ALERTNESS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                               // 0x753
    {true, "FLUSH_PATROL_ROUTE", {}, {}},                                                                      // 0x754
    {true, "EXTEND_PATROL_ROUTE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16}, {}},  // 0x755
    {false, "TASK_GO_ON_PATROL", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_SEQUENCE_TASK, TYPE_UNIMPORTANT, TYPE_CAR}},      // 0x756
    {false, "GET_PATROL_ALERTNESS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                   // 0x757
    {false, "SET_CHAR_SPECIAL_EVENT", {ARG_INT}, {TYPE_CHAR}},                                                 // 0x758
    {false, "SET_ATTRACTOR_AS_COVER_NODE", {ARG_INT, ARG_INT}, {TYPE_ATTRACTOR}},                              // 0x759
    {true, "PLAY_OBJECT_ANIM", {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_OBJECT}},                                                                                        // 0x75A
    {true, "SET_RADAR_ZOOM", {ARG_INT}, {}},                                                                   // 0x75B
    {true, "DOES_BLIP_EXIST", {ARG_INT}, {TYPE_BLIP}},                                                         // 0x75C
    {true, "LOAD_PRICES", {ARG_TEXT_LABEL16}, {}},                                                             // 0x75D
    {true, "LOAD_SHOP", {ARG_TEXT_LABEL16}, {}},                                                               // 0x75E
    {true, "GET_NUMBER_OF_ITEMS_IN_SHOP", {ARG_RETURN_INT}, {}},                                               // 0x75F
    {true, "GET_ITEM_IN_SHOP", {ARG_INT, ARG_RETURN_INT}, {}},                                                 // 0x760
    {true, "GET_PRICE_OF_ITEM", {ARG_INT, ARG_RETURN_INT}, {}},                                                // 0x761
    {true, "TASK_DEAD", {ARG_INT}, {TYPE_CHAR}},                                                               // 0x762
    {true, "SET_CAR_AS_MISSION_CAR", {ARG_INT}, {TYPE_CAR}},                                                   // 0x763
    {false, "IS_SEARCHLIGHT_IN_ANGLED_AREA_2D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {TYPE_SEARCHLIGHT}},        // 0x764
    {false, "IS_SEARCHLIGHT_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_SEARCHLIGHT}},                                                                                   // 0x765
    {false, "SWITCH_SEARCHLIGHT_BULB", {ARG_INT, ARG_INT}, {TYPE_SEARCHLIGHT}},                                // 0x766
    {true, "SET_ZONE_POPULATION_TYPE", {ARG_TEXT_LABEL, ARG_INT}, {}},                                         // 0x767
    {false, "SET_ZONE_GANG_CAP", {ARG_TEXT_LABEL, ARG_INT}, {}},                                               // 0x768
    {false, "GET_ZONE_GANG_CAP", {ARG_TEXT_LABEL, ARG_RETURN_INT}, {}},                                        // 0x769
    {true, "SET_ZONE_DEALER_STRENGTH", {ARG_TEXT_LABEL, ARG_INT}, {}},                                         // 0x76A
    {true, "GET_ZONE_DEALER_STRENGTH", {ARG_TEXT_LABEL, ARG_RETURN_INT}, {}},                                  // 0x76B
    {true, "SET_ZONE_GANG_STRENGTH", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                                  // 0x76C
    {true, "GET_ZONE_GANG_STRENGTH", {ARG_TEXT_LABEL, ARG_INT, ARG_RETURN_INT}, {}},                           // 0x76D
    {false, "SET_NO_POLICE_DURING_LA_RIOTS", {ARG_INT}, {}},                                                   // 0x76E
    {true, "IS_MESSAGE_BEING_DISPLAYED", {}, {}},                                                              // 0x76F
    {true, "SET_CHAR_IS_TARGET_PRIORITY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                    // 0x770
    {true, "CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR", {ARG_INT, ARG_INT}, {}},                                        // 0x771
    {true, "TASK_GOTO_CAR", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR, TYPE_CAR}},                    // 0x772
    {false, "CLEAR_HELP_WITH_THIS_LABEL", {}, {}},                                                             // 0x773
    {false, "IS_SEARCHLIGHT_BULB_ON", {ARG_INT}, {TYPE_SEARCHLIGHT}},                                          // 0x774
    {false, "CREATE_OIL_PUDDLE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                       // 0x775
    {true, "REQUEST_IPL", {ARG_TEXT_LABEL16}, {}},                                                             // 0x776
    {true, "REMOVE_IPL", {ARG_TEXT_LABEL16}, {}},                                                              // 0x777
    {true, "REMOVE_IPL_DISCREETLY", {ARG_TEXT_LABEL16}, {}},                                                   // 0x778
    {false, "TASK_OPEN_PASSENGER_DOOR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                   // 0x779
    {true, "SET_CHAR_RELATIONSHIP", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                 // 0x77A
    {true, "CLEAR_CHAR_RELATIONSHIP", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                               // 0x77B
    {true, "CLEAR_ALL_CHAR_RELATIONSHIPS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x77C
    {true, "GET_CAR_PITCH", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                          // 0x77D
    {true, "GET_AREA_VISIBLE", {ARG_RETURN_INT}, {}},                                                          // 0x77E
    {false, "ADD_INT_TO_VAR_CONSOLE", {ARG_VAR_INT}, {}},                                                      // 0x77F
    {true, "HELI_KEEP_ENTITY_IN_VIEW", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CAR, TYPE_CHAR, TYPE_CAR}},                                                                      // 0x780
    {true, "GET_WEAPONTYPE_MODEL", {ARG_INT, ARG_RETURN_INT}, {}},                                             // 0x781
    {true, "GET_WEAPONTYPE_SLOT", {ARG_INT, ARG_RETURN_INT}, {}},                                              // 0x782
    {true, "GET_SHOPPING_EXTRA_INFO", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {}},                                 // 0x783
    {true, "GIVE_PLAYER_CLOTHES", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                        // 0x784
    {false, "GIVE_PLAYER_TATTOO", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x785
    {true, "GET_NUMBER_OF_FIRES_IN_AREA",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},               // 0x786
    {false, "SET_CHAR_TYRES_CAN_BE_BURST", {}, {}},                                                            // 0x787
    {true, "ATTACH_WINCH_TO_HELI", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x788
    {true, "RELEASE_ENTITY_FROM_WINCH", {ARG_INT}, {TYPE_CAR}},                                                // 0x789
    {true, "GET_TRAIN_CARRIAGE", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x78A
    {true, "GRAB_ENTITY_ON_WINCH", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT},
        {TYPE_CAR, TYPE_CAR, TYPE_CHAR, TYPE_OBJECT}},                                                         // 0x78B
    {true, "GET_NAME_OF_ITEM", {ARG_INT, ARG_VAR_TEXT_LABEL}, {}},                                             // 0x78C
    {false, "ADD_FLOAT_TO_VAR_CONSOLE", {ARG_VAR_FLOAT}, {}},                                                  // 0x78D
    {false, "TASK_DRAG_CHAR_FROM_CAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR, TYPE_CAR}},         // 0x78E
    {true, "TASK_CLIMB", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                                     // 0x78F
    {true, "BUY_ITEM", {ARG_INT}, {}},                                                                         // 0x790
    {false, "BUY_TATTOO", {ARG_INT, ARG_INT}, {}},                                                             // 0x791
    {true, "CLEAR_CHAR_TASKS_IMMEDIATELY", {ARG_INT}, {TYPE_CHAR}},                                            // 0x792
    {true, "STORE_CLOTHES_STATE", {}, {}},                                                                     // 0x793
    {true, "RESTORE_CLOTHES_STATE", {}, {}},                                                                   // 0x794
    {false, "DELETE_WINCH_FOR_HELI", {ARG_INT}, {TYPE_CAR}},                                                   // 0x795
    {true, "GET_ROPE_HEIGHT_FOR_OBJECT", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},                          // 0x796
    {true, "SET_ROPE_HEIGHT_FOR_OBJECT", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT}},                                 // 0x797
    {true, "GRAB_ENTITY_ON_ROPE_FOR_OBJECT", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT},
        {TYPE_OBJECT, TYPE_CAR, TYPE_CHAR, TYPE_OBJECT}},                                                      // 0x798
    {true, "RELEASE_ENTITY_FROM_ROPE_FOR_OBJECT", {ARG_INT}, {TYPE_OBJECT}},                                   // 0x799
    {false, "ATTACH_CAR_TO_ROPE_FOR_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_CAR}},                     // 0x79A
    {false, "ATTACH_CHAR_TO_ROPE_FOR_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_CHAR}},                   // 0x79B
    {false, "ATTACH_OBJECT_TO_ROPE_FOR_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_OBJECT}},               // 0x79C
    {true, "PLAYER_ENTERED_DOCK_CRANE", {}, {}},                                                               // 0x79D
    {true, "PLAYER_ENTERED_BUILDINGSITE_CRANE", {}, {}},                                                       // 0x79E
    {true, "PLAYER_LEFT_CRANE", {}, {}},                                                                       // 0x79F
    {true, "PERFORM_SEQUENCE_TASK_FROM_PROGRESS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_SEQUENCE_TASK}},                                                                      // 0x7A0
    {true, "SET_NEXT_DESIRED_MOVE_STATE", {ARG_INT}, {}},                                                      // 0x7A1
    {false, "SET_NEXT_EVENT_RESPONSE_SEQUENCE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_SEQUENCE_TASK}},          // 0x7A2
    {true, "TASK_GOTO_CHAR_AIMING", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR, TYPE_CHAR}},         // 0x7A3
    {true, "GET_SEQUENCE_PROGRESS_RECURSIVE", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {TYPE_CHAR}},         // 0x7A4
    {true, "TASK_KILL_CHAR_ON_FOOT_TIMED", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},               // 0x7A5
    {true, "GET_NEAREST_TAG_POSITION",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},          // 0x7A6
    {true, "TASK_JETPACK", {ARG_INT}, {TYPE_CHAR}},                                                            // 0x7A7
    {true, "SET_AREA51_SAM_SITE", {ARG_INT}, {}},                                                              // 0x7A8
    {true, "IS_CHAR_IN_ANY_SEARCHLIGHT", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_SEARCHLIGHT}},            // 0x7A9
    {false, "GET_SEARCHLIGHT_COORDS", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_SEARCHLIGHT}},                                                                                   // 0x7AA
    {true, "IS_TRAILER_ATTACHED_TO_CAB", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                            // 0x7AB
    {true, "DETACH_TRAILER_FROM_CAB", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                               // 0x7AC
    {false, "GET_TRAILER_ATTACHED_TO_CAB", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_CAR}},                   // 0x7AD
    {false, "GET_CAB_ATTACHED_TO_TRAILER", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_CAR}},                   // 0x7AE
    {true, "GET_PLAYER_GROUP", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER, TYPE_GROUP}},                          // 0x7AF
    {true, "GET_LOADED_SHOP", {ARG_VAR_TEXT_LABEL}, {}},                                                       // 0x7B0
    {true, "GET_BEAT_PROXIMITY", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {}},               // 0x7B1
    {false, "SET_BEAT_ZONE_SIZE", {ARG_INT, ARG_INT}, {}},                                                     // 0x7B2
    {true, "SET_GROUP_DEFAULT_TASK_ALLOCATOR", {ARG_INT, ARG_INT}, {TYPE_GROUP}},                              // 0x7B3
    {true, "SET_PLAYER_GROUP_RECRUITMENT", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                 // 0x7B4
    {false, "DISPLAY_TWO_ONSCREEN_COUNTERS", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                  // 0x7B5
    {false, "DISPLAY_TWO_ONSCREEN_COUNTERS_WITH_STRING", {ARG_VAR_INT, ARG_VAR_INT, ARG_TEXT_LABEL}, {}},      // 0x7B6
    {false, "DISPLAY_NTH_TWO_ONSCREEN_COUNTERS", {ARG_VAR_INT, ARG_VAR_INT, ARG_INT}, {}},                     // 0x7B7
    {false, "DISPLAY_NTH_TWO_ONSCREEN_COUNTERS_WITH_STRING", {ARG_VAR_INT, ARG_VAR_INT, ARG_INT, ARG_TEXT_LABEL},
        {}},                                                                                                   // 0x7B8
    {false, "TASK_KILL_CHAR_ON_FOOT_PATROL", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_SEQUENCE_TASK}},                                         // 0x7B9
    {false, "HELI_AIM_AHEAD_OF_TARGET_ENTITY", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                // 0x7BA
    {true, "ACTIVATE_HELI_SPEED_CHEAT", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x7BB
    {true, "TASK_SET_CHAR_DECISION_MAKER", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_DECISION_MAKER}},              // 0x7BC
    {true, "DELETE_MISSION_TRAIN", {ARG_INT}, {TYPE_CAR}},                                                     // 0x7BD
    {true, "MARK_MISSION_TRAIN_AS_NO_LONGER_NEEDED", {ARG_INT}, {TYPE_CAR}},                                   // 0x7BE
    {true, "SET_BLIP_ALWAYS_DISPLAY_ON_ZOOMED_RADAR", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                        // 0x7BF
    {true, "REQUEST_CAR_RECORDING", {ARG_INT}, {}},                                                            // 0x7C0
    {true, "HAS_CAR_RECORDING_BEEN_LOADED", {ARG_INT}, {}},                                                    // 0x7C1
    {false, "DISPLAY_PLAYBACK_RECORDED_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                  // 0x7C2
    {true, "GET_OBJECT_QUATERNION", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_OBJECT}},                                                                                        // 0x7C3
    {true, "SET_OBJECT_QUATERNION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},     // 0x7C4
    {true, "GET_VEHICLE_QUATERNION", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CAR}},                                                                                           // 0x7C5
    {true, "SET_VEHICLE_QUATERNION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},       // 0x7C6
    {true, "SET_MISSION_TRAIN_COORDINATES", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},           // 0x7C7
    {false, "DISPLAY_DEBUG_MESSAGE", {}, {}},                                                                  // 0x7C8
    {true, "TASK_COMPLEX_PICKUP_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_OBJECT}},                        // 0x7C9
    {false, "TASK_SIMPLE_PUTDOWN_OBJECT", {ARG_INT}, {TYPE_CHAR}},                                             // 0x7CA
    {true, "LISTEN_TO_PLAYER_GROUP_COMMANDS", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                // 0x7CB
    {true, "SET_PLAYER_ENTER_CAR_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                  // 0x7CC
    {true, "TASK_CHAR_SLIDE_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR}},                                                                                          // 0x7CD
    {false, "SET_BULLET_WHIZZ_BY_DISTANCE", {ARG_FLOAT}, {}},                                                  // 0x7CE
    {false, "SET_TWO_PLAYER_CAM_MODE_SEPARATE_CARS", {ARG_INT}, {}},                                           // 0x7CF
    {true, "GET_CURRENT_DAY_OF_WEEK", {ARG_RETURN_INT}, {}},                                                   // 0x7D0
    {false, "SET_CURRENT_DAY_OF_WEEK", {ARG_INT}, {}},                                                         // 0x7D1
    {false, "ACTIVATE_INTERIORS", {ARG_INT}, {}},                                                              // 0x7D2
    {true, "REGISTER_SCRIPT_BRAIN_FOR_CODE_USE", {ARG_INT, ARG_TEXT_LABEL}, {}},                               // 0x7D3
    {false, "REGISTER_OBJECT_SCRIPT_BRAIN_FOR_CODE_USE", {ARG_INT, ARG_TEXT_LABEL}, {}},                       // 0x7D4
    {true, "APPLY_FORCE_TO_CAR", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CAR}},                                                                                           // 0x7D5
    {true, "IS_INT_LVAR_EQUAL_TO_INT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                   // 0x7D6
    {true, "IS_FLOAT_LVAR_EQUAL_TO_FLOAT_VAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                           // 0x7D7
    {false, "IS_INT_LVAR_NOT_EQUAL_TO_INT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                              // 0x7D8
    {false, "IS_FLOAT_LVAR_NOT_EQUAL_TO_FLOAT_VAR", {ARG_LVAR_FLOAT, ARG_VAR_FLOAT}, {}},                      // 0x7D9
    {true, "ADD_TO_CAR_ROTATION_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},            // 0x7DA
    {true, "SET_CAR_ROTATION_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},               // 0x7DB
    {false, "GET_CAR_ROTATION_VELOCITY", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CAR}},                                                                                           // 0x7DC
    {true, "SET_CHAR_SHOOT_RATE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                            // 0x7DD
    {true, "IS_MODEL_IN_CDIMAGE", {ARG_INT}, {}},                                                              // 0x7DE
    {true, "REMOVE_OIL_PUDDLES_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                    // 0x7DF
    {true, "SET_BLIP_AS_FRIENDLY", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                                           // 0x7E0
    {true, "TASK_SWIM_TO_COORD", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},                     // 0x7E1
    {false, "TASK_GO_STRAIGHT_TO_COORD_WITHOUT_STOPPING", {}, {}},                                             // 0x7E2
    {false, "GET_BEAT_INFO_FOR_CURRENT_TRACK", {ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {}},          // 0x7E3
    {true, "GET_MODEL_DIMENSIONS",
        {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT},
        {}},                                                                                                    // 0x7E4
    {true, "COPY_CHAR_DECISION_MAKER", {ARG_INT, ARG_RETURN_INT}, {TYPE_DECISION_MAKER, TYPE_DECISION_MAKER}},  // 0x7E5
    {true, "COPY_GROUP_DECISION_MAKER", {ARG_INT, ARG_RETURN_INT},
        {TYPE_DECISION_MAKER, TYPE_DECISION_MAKER}},                                                            // 0x7E6
    {true, "TASK_DRIVE_POINT_ROUTE_ADVANCED", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_CAR}},                                                                                 // 0x7E7
    {true, "IS_RELATIONSHIP_SET", {ARG_INT, ARG_INT, ARG_INT}, {}},                                             // 0x7E8
    {false, "HAS_CHAR_SPOTTED_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},                                 // 0x7E9
    {false, "SET_ROPE_HEIGHT_FOR_HELI", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                      // 0x7EA
    {false, "GET_ROPE_HEIGHT_FOR_HELI", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                               // 0x7EB
    {false, "IS_CAR_LOWRIDER", {ARG_INT}, {}},                                                                  // 0x7EC
    {false, "IS_PERFORMANCE_CAR", {ARG_INT}, {}},                                                               // 0x7ED
    {true, "SET_CAR_ALWAYS_CREATE_SKIDS", {ARG_INT, ARG_INT}, {}},                                              // 0x7EE
    {true, "GET_CITY_FROM_COORDS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},                      // 0x7EF
    {true, "HAS_OBJECT_OF_TYPE_BEEN_SMASHED", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},       // 0x7F0
    {true, "IS_PLAYER_PERFORMING_WHEELIE", {ARG_INT}, {TYPE_PLAYER}},                                           // 0x7F1
    {true, "IS_PLAYER_PERFORMING_STOPPIE", {ARG_INT}, {TYPE_PLAYER}},                                           // 0x7F2
    {true, "SET_CHECKPOINT_COORDS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHECKPOINT}},             // 0x7F3
    {false, "SET_ONSCREEN_TIMER_DISPLAY", {ARG_VAR_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},      // 0x7F4
    {true, "CONTROL_CAR_HYDRAULICS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},        // 0x7F5
    {true, "GET_GROUP_SIZE", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {TYPE_GROUP}},                          // 0x7F6
    {true, "SET_OBJECT_COLLISION_DAMAGE_EFFECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                            // 0x7F7
    {true, "SET_CAR_FOLLOW_CAR", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR, TYPE_CAR}},                          // 0x7F8
    {true, "PLAYER_ENTERED_QUARRY_CRANE", {}, {}},                                                              // 0x7F9
    {true, "PLAYER_ENTERED_LAS_VEGAS_CRANE", {}, {}},                                                           // 0x7FA
    {true, "SWITCH_ENTRY_EXIT", {ARG_TEXT_LABEL, ARG_INT}, {}},                                                 // 0x7FB
    {true, "DISPLAY_TEXT_WITH_FLOAT", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_FLOAT, ARG_INT}, {}},          // 0x7FC
    {true, "DOES_GROUP_EXIST", {ARG_INT}, {TYPE_GROUP}},                                                        // 0x7FD
    {true, "GIVE_MELEE_ATTACK_TO_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                              // 0x7FE
    {true, "SET_CAR_HYDRAULICS", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0x7FF
    {true, "IS_2PLAYER_GAME_GOING_ON", {}, {}},                                                                 // 0x800
    {true, "GET_CAMERA_FOV", {ARG_RETURN_FLOAT}, {}},                                                           // 0x801
    {false, "SET_GLOBAL_PED_SEARCH_PARAMS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                             // 0x802
    {true, "DOES_CAR_HAVE_HYDRAULICS", {ARG_INT}, {TYPE_CAR}},                                                  // 0x803
    {true, "TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT,
            ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                                               // 0x804
    {false, "ALLOCATE_SCRIPT_TO_OBJECT", {ARG_LABEL, ARG_INT, ARG_INT}, {}},                        // 0x805
    {true, "GET_TOTAL_NUMBER_OF_PEDS_KILLED_BY_PLAYER", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},  // 0x806
    {false, "SET_TWO_PLAYER_CAM_MODE_SAME_CAR_SHOOTING", {ARG_INT}, {}},                            // 0x807
    {false, "SET_TWO_PLAYER_CAM_MODE_SAME_CAR_NO_SHOOTING", {ARG_INT}, {}},                         // 0x808
    {false, "SET_TWO_PLAYER_CAM_MODE_NOT_BOTH_IN_CAR", {ARG_INT}, {}},                              // 0x809
    {true, "GET_LEVEL_DESIGN_COORDS_FOR_OBJECT",
        {ARG_INT, ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},   // 0x80A
    {true, "SAVE_TEXT_LABEL_TO_DEBUG_FILE", {ARG_VAR_TEXT_LABEL16}, {}},                            // 0x80B
    {false, "GET_CHAR_BREATH", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                             // 0x80C
    {false, "SET_CHAR_BREATH", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                    // 0x80D
    {true, "GET_CHAR_HIGHEST_PRIORITY_EVENT", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},              // 0x80E
    {false, "ARE_PATHS_LOADED_FOR_CAR", {ARG_INT}, {TYPE_CAR}},                                     // 0x80F
    {true, "GET_PARKING_NODE_IN_AREA",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT,
            ARG_RETURN_FLOAT},
        {}},                                                                            // 0x810
    {true, "GET_CAR_CHAR_IS_USING", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_CAR}},  // 0x811
    {true, "TASK_PLAY_ANIM_NON_INTERRUPTABLE",
        {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                          // 0x812
    {false, "FORCE_NEXT_DIE_ANIM", {ARG_TEXT_LABEL16, ARG_TEXT_LABEL16}, {}},  // 0x813
    {true, "ADD_STUNT_JUMP",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT,
            ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                                    // 0x814
    {true, "SET_OBJECT_COORDINATES_AND_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_OBJECT}},   // 0x815
    {true, "SET_CHAR_KINDA_STAY_IN_SAME_PLACE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                               // 0x816
    {true, "TASK_FOLLOW_PATROL_ROUTE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                               // 0x817
    {true, "IS_CHAR_IN_AIR", {ARG_INT}, {TYPE_CHAR}},                                                           // 0x818
    {true, "GET_CHAR_HEIGHT_ABOVE_GROUND", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},                           // 0x819
    {true, "SET_CHAR_WEAPON_SKILL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                           // 0x81A
    {false, "ARE_PATHS_LOADED_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                      // 0x81B
    {true, "SET_TEXT_EDGE", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {}},                                 // 0x81C
    {true, "SET_CAR_ENGINE_BROKEN", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                            // 0x81D
    {true, "IS_THIS_MODEL_A_BOAT", {ARG_INT}, {}},                                                              // 0x81E
    {true, "IS_THIS_MODEL_A_PLANE", {ARG_INT}, {}},                                                             // 0x81F
    {true, "IS_THIS_MODEL_A_HELI", {ARG_INT}, {}},                                                              // 0x820
    {false, "IS_3D_COORD_IN_ZONE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL}, {}},                      // 0x821
    {true, "SET_FIRST_PERSON_IN_CAR_CAMERA_MODE", {ARG_INT}, {}},                                               // 0x822
    {true, "TASK_GREET_PARTNER", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},               // 0x823
    {false, "GET_CLOSEST_PICKUP_COORDS_TO_COORD",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},           // 0x824
    {true, "SET_HELI_BLADES_FULL_SPEED", {ARG_INT}, {TYPE_CAR}},                                                // 0x825
    {true, "DISPLAY_HUD", {ARG_INT}, {}},                                                                       // 0x826
    {true, "CONNECT_LODS", {ARG_INT, ARG_INT}, {TYPE_OBJECT, TYPE_OBJECT}},                                     // 0x827
    {true, "SET_MAX_FIRE_GENERATIONS", {ARG_INT}, {}},                                                          // 0x828
    {true, "TASK_DIE_NAMED_ANIM", {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                           // 0x829
    {true, "SET_PLAYER_DUCK_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                        // 0x82A
    {false, "FIND_NEAREST_MULTIBUILDING", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_MULTIBUILDING}},          // 0x82B
    {false, "SET_MULTIBUILDING_MODEL", {ARG_INT, ARG_INT}, {TYPE_MULTIBUILDING}},                               // 0x82C
    {false, "GET_NUMBER_MULTIBUILDING_MODELS", {ARG_INT, ARG_RETURN_INT}, {TYPE_MULTIBUILDING}},                // 0x82D
    {false, "GET_MULTIBUILDING_MODEL_INDEX", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_MULTIBUILDING}},         // 0x82E
    {false, "SET_CURRENT_BUYABLE_PROPERTY", {ARG_INT}, {TYPE_MULTIBUILDING}},                                   // 0x82F
    {true, "SET_POOL_TABLE_COORDS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},    // 0x830
    {true, "IS_AUDIO_BUILD", {}, {}},                                                                           // 0x831
    {false, "CLEAR_QUEUED_DIALOGUE", {ARG_TEXT_LABEL16}, {}},                                                   // 0x832
    {true, "HAS_OBJECT_BEEN_PHOTOGRAPHED", {ARG_INT}, {TYPE_OBJECT}},                                           // 0x833
    {true, "DO_CAMERA_BUMP", {ARG_FLOAT, ARG_FLOAT}, {}},                                                       // 0x834
    {true, "GET_CURRENT_DATE", {ARG_RETURN_INT, ARG_RETURN_INT}, {}},                                           // 0x835
    {true, "SET_OBJECT_ANIM_SPEED", {ARG_INT, ARG_TEXT_LABEL16, ARG_FLOAT}, {TYPE_OBJECT}},                     // 0x836
    {true, "IS_OBJECT_PLAYING_ANIM", {ARG_INT, ARG_TEXT_LABEL16}, {TYPE_OBJECT}},                               // 0x837
    {false, "SET_OBJECT_ANIM_PLAYING_FLAG", {ARG_INT, ARG_TEXT_LABEL16, ARG_INT}, {TYPE_OBJECT}},               // 0x838
    {true, "GET_OBJECT_ANIM_CURRENT_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},       // 0x839
    {true, "SET_OBJECT_ANIM_CURRENT_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_FLOAT}, {TYPE_OBJECT}},              // 0x83A
    {false, "GET_OBJECT_ANIM_TOTAL_TIME", {ARG_INT, ARG_TEXT_LABEL16, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},        // 0x83B
    {true, "SET_CHAR_VELOCITY", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},                       // 0x83C
    {true, "GET_CHAR_VELOCITY", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},  // 0x83D
    {true, "SET_CHAR_ROTATION", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},                       // 0x83E
    {true, "GET_CAR_UPRIGHT_VALUE", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                                   // 0x83F
    {true, "SET_VEHICLE_AREA_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                         // 0x840
    {true, "SELECT_WEAPONS_FOR_VEHICLE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x841
    {true, "GET_CITY_PLAYER_IS_IN", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                                  // 0x842
    {true, "GET_NAME_OF_ZONE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_VAR_TEXT_LABEL}, {}},                      // 0x843
    {true, "IS_VAR_TEXT_LABEL_EMPTY", {ARG_VAR_TEXT_LABEL}, {}},                                                // 0x844
    {true, "IS_LVAR_TEXT_LABEL_EMPTY", {ARG_LVAR_TEXT_LABEL}, {}},                                              // 0x845
    {true, "IS_VAR_TEXT_LABEL16_EMPTY", {ARG_VAR_TEXT_LABEL16}, {}},                                            // 0x846
    {true, "IS_LVAR_TEXT_LABEL16_EMPTY", {ARG_LVAR_TEXT_LABEL16}, {}},                                          // 0x847
    {true, "SWITCH", {ARG_INT}, {}},                                                                            // 0x848
    {true, "ENDSWITCH", {}, {}},                                                                                // 0x849
    {true, "CASE", {ARG_INT_CONST}, {}},                                                                        // 0x84A
    {true, "DEFAULT", {}, {}},                                                                                  // 0x84B
    {true, "BREAK", {}, {}},                                                                                    // 0x84C
    {true, "ACTIVATE_INTERIOR_PEDS", {ARG_INT}, {}},                                                            // 0x84D
    {true, "SET_VEHICLE_CAN_BE_TARGETTED", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                     // 0x84E
    {false, "GET_GROUP_LEADER", {ARG_INT, ARG_RETURN_INT}, {TYPE_GROUP, TYPE_CHAR}},                            // 0x84F
    {true, "TASK_FOLLOW_FOOTSTEPS", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                                // 0x850
    {true, "DAMAGE_CHAR", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},                                            // 0x851
    {true, "SET_CAR_CAN_BE_VISIBLY_DAMAGED", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0x852
    {true, "SET_HELI_REACHED_TARGET_DISTANCE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                 // 0x853
    {false, "BLOCK_NODES_IN_AREA", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                                    // 0x854
    {true, "GET_SOUND_LEVEL_AT_COORDS", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CHAR}},                                                                                           // 0x855
    {true, "SET_CHAR_ALLOWED_TO_DUCK", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                        // 0x856
    {false, "SET_WATER_CONFIGURATION", {ARG_INT}, {}},                                                          // 0x857
    {true, "SET_HEADING_FOR_ATTACHED_PLAYER", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {TYPE_PLAYER}},                  // 0x858
    {true, "TASK_WALK_ALONGSIDE_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                             // 0x859
    {true, "CREATE_EMERGENCY_SERVICES_CAR", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                    // 0x85A
    {true, "TASK_KINDA_STAY_IN_SAME_PLACE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x85B
    {false, "TASK_USE_ATTRACTOR_ADVANCED", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_ATTRACTOR}},           // 0x85C
    {false, "TASK_FOLLOW_PATH_NODES_TO_COORD_SHOOTING",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                               // 0x85D
    {true, "START_PLAYBACK_RECORDED_CAR_LOOPED", {ARG_INT, ARG_INT}, {TYPE_CAR}},                      // 0x85E
    {false, "START_PLAYBACK_RECORDED_CAR_USING_AI_LOOPED", {ARG_INT, ARG_INT}, {TYPE_CAR}},            // 0x85F
    {true, "SET_CHAR_AREA_VISIBLE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x860
    {true, "IS_ATTACHED_PLAYER_HEADING_ACHIEVED", {ARG_INT}, {TYPE_PLAYER}},                           // 0x861
    {false, "GET_MODEL_NAME_FOR_DEBUG_ONLY", {ARG_INT, ARG_VAR_TEXT_LABEL16}, {}},                     // 0x862
    {false, "TASK_USE_NEARBY_ENTRY_EXIT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},    // 0x863
    {true, "ENABLE_ENTRY_EXIT_PLAYER_GROUP_WARPING", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},  // 0x864
    {false, "FREEZE_STATE_OF_INTERIORS", {ARG_INT}, {}},                                               // 0x865
    {true, "GET_CLOSEST_STEALABLE_OBJECT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_OBJECT}},        // 0x866
    {true, "IS_PROCEDURAL_INTERIOR_ACTIVE", {ARG_INT}, {}},                                            // 0x867
    {true, "CLEAR_THIS_VIEW_INTEGER_VARIABLE", {ARG_VAR_INT}, {}},                                     // 0x868
    {true, "CLEAR_THIS_VIEW_FLOAT_VARIABLE", {ARG_VAR_FLOAT}, {}},                                     // 0x869
    {true, "CLEAR_ALL_VIEW_VARIABLES", {}, {}},                                                        // 0x86A
    {true, "CLEAR_THIS_INTEGER_WATCHPOINT", {ARG_VAR_INT}, {}},                                        // 0x86B
    {true, "CLEAR_THIS_FLOAT_WATCHPOINT", {ARG_VAR_FLOAT}, {}},                                        // 0x86C
    {true, "CLEAR_ALL_BREAKPOINTS", {}, {}},                                                           // 0x86D
    {true, "CLEAR_ALL_WATCHPOINTS", {}, {}},                                                           // 0x86E
    {false, "IS_THIS_MODEL_A_TRAIN", {ARG_INT}, {}},                                                   // 0x86F
    {false, "GET_VEHICLE_CHAR_IS_STANDING_ON", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_CAR}},      // 0x870
    {true, "SWITCH_START",
        {ARG_INT, ARG_INT, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT,
            ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL},
        {}},  // 0x871
    {true, "SWITCH_CONTINUED",
        {ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT,
            ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL, ARG_INT, ARG_LABEL},
        {}},                                                                                            // 0x872
    {true, "REMOVE_CAR_RECORDING", {ARG_INT}, {}},                                                      // 0x873
    {true, "SET_ZONE_POPULATION_RACE", {ARG_TEXT_LABEL, ARG_INT}, {}},                                  // 0x874
    {true, "SET_OBJECT_ONLY_DAMAGED_BY_PLAYER", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                     // 0x875
    {true, "CREATE_BIRDS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {}},                                                                                            // 0x876
    {false, "GET_VEHICLE_DIRT_LEVEL", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                         // 0x877
    {true, "SET_VEHICLE_DIRT_LEVEL", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                 // 0x878
    {true, "SET_GANG_WARS_ACTIVE", {ARG_INT}, {}},                                                      // 0x879
    {true, "IS_GANG_WAR_GOING_ON", {}, {}},                                                             // 0x87A
    {true, "GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP", {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_INT},
        {TYPE_PLAYER}},                                                                                 // 0x87B
    {true, "CLEAR_LOADED_SHOP", {}, {}},                                                                // 0x87C
    {true, "SET_GROUP_SEQUENCE", {ARG_INT, ARG_INT}, {TYPE_GROUP, TYPE_SEQUENCE_TASK}},                 // 0x87D
    {true, "SET_CHAR_DROPS_WEAPONS_WHEN_DEAD", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                        // 0x87E
    {true, "SET_CHAR_NEVER_LEAVES_GROUP", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                             // 0x87F
    {false, "DRAW_RECT_WITH_TITLE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL}, {}},  // 0x880
    {true, "SET_PLAYER_FIRE_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                // 0x881
    {false, "SET_ATTRACTOR_RADIUS", {ARG_INT, ARG_FLOAT}, {TYPE_ATTRACTOR}},                            // 0x882
    {true, "ATTACH_FX_SYSTEM_TO_CHAR_BONE", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_FX_SYSTEM, TYPE_CHAR}},  // 0x883
    {true, "REGISTER_ATTRACTOR_SCRIPT_BRAIN_FOR_CODE_USE", {ARG_INT, ARG_TEXT_LABEL}, {}},              // 0x884
    {false, "CONST_INT", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                             // 0x885
    {false, "CONST_FLOAT", {ARG_VAR_FLOAT, ARG_VAR_FLOAT}, {}},                                         // 0x886
    {true, "SET_HEADING_LIMIT_FOR_ATTACHED_CHAR", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},          // 0x887
    {true, "ADD_BLIP_FOR_DEAD_CHAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_BLIP}},                // 0x888
    {true, "GET_DEAD_CHAR_COORDINATES", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {TYPE_CHAR}},                                                                                   // 0x889
    {true, "TASK_PLAY_ANIM_WITH_FLAGS",
        {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT,
            ARG_INT},
        {TYPE_CHAR}},                                                                                 // 0x88A
    {true, "SET_VEHICLE_AIR_RESISTANCE_MULTIPLIER", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                // 0x88B
    {true, "SET_CAR_COORDINATES_NO_OFFSET", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CAR}},  // 0x88C
    {true, "SET_USES_COLLISION_OF_CLOSEST_OBJECT_OF_TYPE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},                          // 0x88D
    {true, "SET_TIME_ONE_DAY_FORWARD", {}, {}},                                                       // 0x88E
    {false, "SET_TIME_ONE_DAY_BACK", {}, {}},                                                         // 0x88F
    {true, "SET_TIMER_BEEP_COUNTDOWN_TIME", {ARG_VAR_INT, ARG_INT}, {}},                              // 0x890
    {false, "TASK_SIT_IN_RESTAURANT", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_CHAR}},                                                                                 // 0x891
    {false, "GET_RANDOM_ATTRACTOR_ON_CLOSEST_OBJECT_OF_TYPE", {}, {}},                                // 0x892
    {true, "ATTACH_TRAILER_TO_CAB", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                        // 0x893
    {false, "ADD_INTERESTING_ENTITY_FOR_CHAR", {ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR, TYPE_CHAR, TYPE_CAR, TYPE_OBJECT}},                                               // 0x894
    {false, "CLEAR_INTERESTING_ENTITIES_FOR_CHAR", {ARG_INT}, {TYPE_CHAR}},                           // 0x895
    {false, "GET_CLOSEST_ATTRACTOR", {}, {}},                                                         // 0x896
    {true, "IS_VEHICLE_TOUCHING_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_OBJECT}},                // 0x897
    {true, "ENABLE_CRANE_CONTROLS", {ARG_INT, ARG_INT, ARG_INT}, {}},                                 // 0x898
    {false, "ALLOCATE_SCRIPT_TO_ATTRACTOR", {}, {}},                                                  // 0x899
    {false, "GET_CLOSEST_ATTRACTOR_WITH_THIS_SCRIPT", {}, {}},                                        // 0x89A
    {true, "IS_PLAYER_IN_POSITION_FOR_CONVERSATION", {ARG_INT}, {TYPE_CHAR}},                         // 0x89B
    {true, "ENABLE_CONVERSATION", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x89C
    {false, "GET_CONVERSATION_STATUS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                       // 0x89D
    {true, "GET_RANDOM_CHAR_IN_SPHERE_ONLY_DRUGS_BUYERS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},         // 0x89E
    {true, "GET_PED_TYPE", {ARG_INT, ARG_RETURN_INT},
        {TYPE_CHAR, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                                                                   // 0x89F
    {true, "TASK_USE_CLOSEST_MAP_ATTRACTOR",
        {ARG_INT, ARG_FLOAT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL16}, {TYPE_CHAR}},      // 0x8A0
    {false, "GET_CHAR_MAP_ATTRACTOR_STATUS", {}, {}},                                                        // 0x8A1
    {true, "PLANE_ATTACK_PLAYER_USING_DOG_FIGHT", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR, TYPE_PLAYER}},   // 0x8A2
    {true, "CAN_TRIGGER_GANG_WAR_WHEN_ON_A_MISSION", {ARG_INT}, {}},                                         // 0x8A3
    {true, "CONTROL_MOVABLE_VEHICLE_PART", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                // 0x8A4
    {true, "WINCH_CAN_PICK_VEHICLE_UP", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                     // 0x8A5
    {true, "OPEN_CAR_DOOR_A_BIT", {ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                // 0x8A6
    {true, "IS_CAR_DOOR_FULLY_OPEN", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                        // 0x8A7
    {true, "SET_ALWAYS_DRAW_3D_MARKERS", {ARG_INT}, {}},                                                     // 0x8A8
    {true, "STREAM_SCRIPT", {ARG_INT}, {}},                                                                  // 0x8A9
    {false, "STREAM_SCRIPT_INTERNAL", {ARG_INT}, {}},                                                        // 0x8AA
    {true, "HAS_STREAMED_SCRIPT_LOADED", {ARG_INT}, {}},                                                     // 0x8AB
    {true, "SET_GANG_WARS_TRAINING_MISSION", {ARG_INT}, {}},                                                 // 0x8AC
    {true, "SET_CHAR_HAS_USED_ENTRY_EXIT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},         // 0x8AD
    {false, "DRAW_WINDOW_TEXT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_INT},
        {}},                                                                                                 // 0x8AE
    {true, "SET_CHAR_MAX_HEALTH", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                          // 0x8AF
    {false, "SET_CAR_PITCH", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                                              // 0x8B0
    {true, "SET_NIGHT_VISION", {ARG_INT}, {}},                                                               // 0x8B1
    {true, "SET_INFRARED_VISION", {ARG_INT}, {}},                                                            // 0x8B2
    {true, "SET_ZONE_FOR_GANG_WARS_TRAINING", {ARG_TEXT_LABEL}, {}},                                         // 0x8B3
    {true, "IS_GLOBAL_VAR_BIT_SET_CONST", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                 // 0x8B4
    {true, "IS_GLOBAL_VAR_BIT_SET_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                     // 0x8B5
    {true, "IS_GLOBAL_VAR_BIT_SET_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                   // 0x8B6
    {true, "IS_LOCAL_VAR_BIT_SET_CONST", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                                 // 0x8B7
    {true, "IS_LOCAL_VAR_BIT_SET_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                     // 0x8B8
    {true, "IS_LOCAL_VAR_BIT_SET_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                   // 0x8B9
    {true, "SET_GLOBAL_VAR_BIT_CONST", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                    // 0x8BA
    {true, "SET_GLOBAL_VAR_BIT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                        // 0x8BB
    {true, "SET_GLOBAL_VAR_BIT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                      // 0x8BC
    {true, "SET_LOCAL_VAR_BIT_CONST", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                                    // 0x8BD
    {true, "SET_LOCAL_VAR_BIT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                        // 0x8BE
    {true, "SET_LOCAL_VAR_BIT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                      // 0x8BF
    {true, "CLEAR_GLOBAL_VAR_BIT_CONST", {ARG_VAR_INT, ARG_INT_CONST}, {}},                                  // 0x8C0
    {true, "CLEAR_GLOBAL_VAR_BIT_VAR", {ARG_VAR_INT, ARG_VAR_INT}, {}},                                      // 0x8C1
    {true, "CLEAR_GLOBAL_VAR_BIT_LVAR", {ARG_VAR_INT, ARG_LVAR_INT}, {}},                                    // 0x8C2
    {true, "CLEAR_LOCAL_VAR_BIT_CONST", {ARG_LVAR_INT, ARG_INT_CONST}, {}},                                  // 0x8C3
    {true, "CLEAR_LOCAL_VAR_BIT_VAR", {ARG_LVAR_INT, ARG_VAR_INT}, {}},                                      // 0x8C4
    {true, "CLEAR_LOCAL_VAR_BIT_LVAR", {ARG_LVAR_INT, ARG_LVAR_INT}, {}},                                    // 0x8C5
    {true, "SET_CHAR_CAN_BE_KNOCKED_OFF_BIKE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                             // 0x8C6
    {true, "SET_CHAR_COORDINATES_DONT_WARP_GANG", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},  // 0x8C7
    {true, "ADD_PRICE_MODIFIER", {ARG_INT, ARG_INT}, {}},                                                    // 0x8C8
    {true, "REMOVE_PRICE_MODIFIER", {ARG_INT}, {}},                                                          // 0x8C9
    {true, "INIT_ZONE_POPULATION_SETTINGS", {}, {}},                                                         // 0x8CA
    {true, "EXPLODE_CAR_IN_CUTSCENE_SHAKE_AND_BITS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},      // 0x8CB
    {false, "PICK_UP_OBJECT_WITH_WINCH", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_OBJECT}},                       // 0x8CC
    {false, "PICK_UP_VEHICLE_WITH_WINCH", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                         // 0x8CD
    {false, "PICK_UP_CHAR_WITH_WINCH", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CHAR}},                           // 0x8CE
    {false, "STORE_CAR_IN_NEAREST_IMPOUNDING_GARAGE", {ARG_INT}, {TYPE_CAR}},                                // 0x8CF
    {true, "IS_SKIP_CUTSCENE_BUTTON_PRESSED", {}, {}},                                                       // 0x8D0
    {true, "GET_CUTSCENE_OFFSET", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},               // 0x8D1
    {true, "SET_OBJECT_SCALE", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT}},                                         // 0x8D2
    {true, "GET_CURRENT_POPULATION_ZONE_TYPE", {ARG_RETURN_INT}, {}},                                        // 0x8D3
    {true, "CREATE_MENU",
        {ARG_TEXT_LABEL, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {}},                                                                 // 0x8D4
    {false, "CONSTANT_INT", {ARG_VAR_INT, ARG_INT_CONST}, {}},               // 0x8D5
    {true, "SET_MENU_COLUMN_ORIENTATION", {ARG_INT, ARG_INT, ARG_INT}, {}},  // 0x8D6
    {true, "GET_MENU_ITEM_SELECTED", {ARG_INT, ARG_RETURN_INT}, {}},         // 0x8D7
    {true, "GET_MENU_ITEM_ACCEPTED", {ARG_INT, ARG_RETURN_INT}, {}},         // 0x8D8
    {true, "ACTIVATE_MENU_ITEM", {ARG_INT, ARG_INT, ARG_INT}, {}},           // 0x8D9
    {true, "DELETE_MENU", {ARG_INT}, {}},                                    // 0x8DA
    {true, "SET_MENU_COLUMN",
        {ARG_INT, ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL,
            ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL,
            ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {}},                                                                                                  // 0x8DB
    {true, "SET_BLIP_ENTRY_EXIT", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_BLIP}},                   // 0x8DC
    {true, "SWITCH_DEATH_PENALTIES", {ARG_INT}, {}},                                                          // 0x8DD
    {true, "SWITCH_ARREST_PENALTIES", {ARG_INT}, {}},                                                         // 0x8DE
    {true, "SET_EXTRA_HOSPITAL_RESTART_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},  // 0x8DF
    {true, "SET_EXTRA_POLICE_STATION_RESTART_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                  // 0x8E0
    {true, "FIND_NUMBER_TAGS_TAGGED", {ARG_RETURN_INT}, {}},                                                  // 0x8E1
    {true, "GET_TERRITORY_UNDER_CONTROL_PERCENTAGE", {ARG_RETURN_INT}, {}},                                   // 0x8E2
    {true, "IS_OBJECT_IN_ANGLED_AREA_2D", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},                                                                                       // 0x8E3
    {true, "IS_OBJECT_IN_ANGLED_AREA_3D",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_OBJECT}},                                                                                         // 0x8E4
    {true, "GET_RANDOM_CHAR_IN_SPHERE_NO_BRAIN", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CHAR}},                   // 0x8E5
    {true, "SET_PLANE_UNDERCARRIAGE_UP", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x8E6
    {true, "DISABLE_ALL_ENTRY_EXITS", {ARG_INT}, {}},                                                           // 0x8E7
    {true, "ATTACH_ANIMS_TO_MODEL", {ARG_INT, ARG_TEXT_LABEL}, {}},                                             // 0x8E8
    {true, "SET_OBJECT_AS_STEALABLE", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                       // 0x8E9
    {true, "SET_CREATE_RANDOM_GANG_MEMBERS", {ARG_INT}, {TYPE_OBJECT}},                                         // 0x8EA
    {true, "ADD_SPARKS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},      // 0x8EB
    {true, "GET_VEHICLE_CLASS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                         // 0x8EC
    {true, "CLEAR_CONVERSATION_FOR_CHAR", {ARG_INT}, {TYPE_CHAR}},                                              // 0x8ED
    {true, "SET_MENU_ITEM_WITH_NUMBER", {ARG_INT, ARG_INT, ARG_INT, ARG_TEXT_LABEL, ARG_INT}, {}},              // 0x8EE
    {true, "SET_MENU_ITEM_WITH_2_NUMBERS", {ARG_INT, ARG_INT, ARG_INT, ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},  // 0x8EF
    {true, "APPEND_TO_NEXT_CUTSCENE", {ARG_TEXT_LABEL, ARG_TEXT_LABEL}, {}},                                    // 0x8F0
    {true, "GET_NAME_OF_INFO_ZONE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_VAR_TEXT_LABEL}, {}},                 // 0x8F1
    {true, "VEHICLE_CAN_BE_TARGETTED_BY_HS_MISSILE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                           // 0x8F2
    {true, "SET_FREEBIES_IN_VEHICLE", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                          // 0x8F3
    {true, "SET_SCRIPT_LIMIT_TO_GANG_SIZE", {ARG_INT}, {}},                                                     // 0x8F4
    {true, "MAKE_PLAYER_GANG_DISAPPEAR", {}, {}},                                                               // 0x8F5
    {true, "MAKE_PLAYER_GANG_REAPPEAR", {}, {}},                                                                // 0x8F6
    {true, "GET_CLOTHES_ITEM", {ARG_INT, ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},              // 0x8F7
    {true, "SHOW_UPDATE_STATS", {ARG_INT}, {}},                                                                 // 0x8F8
    {true, "IS_VAR_TEXT_LABEL16_EQUAL_TO_TEXT_LABEL", {ARG_VAR_TEXT_LABEL16, ARG_TEXT_LABEL16}, {}},            // 0x8F9
    {true, "IS_LVAR_TEXT_LABEL16_EQUAL_TO_TEXT_LABEL", {ARG_LVAR_TEXT_LABEL16, ARG_TEXT_LABEL16}, {}},          // 0x8FA
    {true, "SET_COORD_BLIP_APPEARANCE", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                                       // 0x8FB
    {false, "GET_MENU_POSITION", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},                            // 0x8FC
    {true, "SET_HEATHAZE_EFFECT", {ARG_INT}, {}},                                                               // 0x8FD
    {true, "IS_HELP_MESSAGE_BEING_DISPLAYED", {}, {}},                                                          // 0x8FE
    {true, "HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                             // 0x8FF
    {true, "CLEAR_OBJECT_LAST_WEAPON_DAMAGE", {ARG_INT}, {TYPE_OBJECT}},                                        // 0x900
    {true, "SET_PLAYER_JUMP_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                        // 0x901
    {false, "SET_OBJECT_BEEN_PHOTOGRAPHED_FLAG", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                            // 0x902
    {false, "SET_CHAR_BEEN_PHOTOGRAPHED_FLAG", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                // 0x903
    {true, "GET_HUD_COLOUR", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {}},    // 0x904
    {true, "LOCK_DOOR", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                                                     // 0x905
    {true, "SET_OBJECT_MASS", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT}},                                             // 0x906
    {true, "GET_OBJECT_MASS", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},                                      // 0x907
    {true, "SET_OBJECT_TURN_MASS", {ARG_INT, ARG_FLOAT}, {TYPE_OBJECT}},                                        // 0x908
    {true, "GET_OBJECT_TURN_MASS", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_OBJECT}},                                 // 0x909
    {false, "IS_PLAYBACK_FOR_CAR_PAUSED", {ARG_INT}, {TYPE_CAR}},                                               // 0x90A
    {false, "TRIGGER_PED_BOUNCE", {ARG_INT}, {TYPE_CHAR}},                                                      // 0x90B
    {true, "SET_SPECIFIC_ZONE_TO_TRIGGER_GANG_WAR", {ARG_TEXT_LABEL}, {}},                                      // 0x90C
    {true, "CLEAR_SPECIFIC_ZONES_TO_TRIGGER_GANG_WAR", {}, {}},                                                 // 0x90D
    {true, "SET_ACTIVE_MENU_ITEM", {ARG_INT, ARG_INT}, {}},                                                     // 0x90E
    {true, "MARK_STREAMED_SCRIPT_AS_NO_LONGER_NEEDED", {ARG_INT}, {}},                                          // 0x90F
    {true, "REMOVE_STREAMED_SCRIPT", {ARG_INT}, {}},                                                            // 0x910
    {true, "REGISTER_STREAMED_SCRIPT", {ARG_LABEL}, {}},                                                        // 0x911
    {true, "SET_MESSAGE_FORMATTING", {ARG_INT, ARG_INT, ARG_INT}, {}},                                          // 0x912
    {true, "START_NEW_STREAMED_SCRIPT",
        {ARG_INT, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM,
            ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM,
            ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM, ARG_SCRIPT_PARAM},
        {}},                                                                // 0x913
    {true, "REGISTER_STREAMED_SCRIPT_INTERNAL", {ARG_INT}, {}},             // 0x914
    {true, "SET_WEATHER_TO_APPROPRIATE_TYPE_NOW", {}, {}},                  // 0x915
    {true, "WINCH_CAN_PICK_OBJECT_UP", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},  // 0x916
    {true, "SWITCH_AUDIO_ZONE", {ARG_TEXT_LABEL, ARG_INT}, {}},             // 0x917
    {true, "SET_CAR_ENGINE_ON", {ARG_INT, ARG_INT}, {TYPE_CAR}},            // 0x918
    {true, "SET_CAR_LIGHTS_ON", {ARG_INT, ARG_INT}, {TYPE_CAR}},            // 0x919
    {true, "GET_LATEST_CONSOLE_COMMAND", {ARG_VAR_TEXT_LABEL16}, {}},       // 0x91A
    {true, "RESET_LATEST_CONSOLE_COMMAND", {}, {}},                         // 0x91B
    {true, "GET_USER_OF_CLOSEST_MAP_ATTRACTOR",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_TEXT_LABEL16, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                                        // 0x91C
    {true, "SWITCH_ROADS_BACK_TO_ORIGINAL", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x91D
    {true, "SWITCH_PED_ROADS_BACK_TO_ORIGINAL", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {}},                                                                                                    // 0x91E
    {true, "GET_PLANE_UNDERCARRIAGE_POSITION", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                        // 0x91F
    {true, "CAMERA_SET_VECTOR_TRACK",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},              // 0x920
    {false, "CAMERA_SET_SHAKE_SIMULATION", {ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},                       // 0x921
    {true, "CAMERA_SET_LERP_FOV", {ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},                                // 0x922
    {true, "SWITCH_AMBIENT_PLANES", {ARG_INT}, {}},                                                             // 0x923
    {true, "SET_DARKNESS_EFFECT", {ARG_INT, ARG_INT}, {}},                                                      // 0x924
    {true, "CAMERA_RESET_NEW_SCRIPTABLES", {}, {}},                                                             // 0x925
    {true, "GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT", {ARG_INT, ARG_RETURN_INT}, {}},                        // 0x926
    {false, "ALLOCATE_STREAMED_SCRIPT_TO_PED_GENERATOR", {ARG_INT, ARG_INT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_PED_GENERATOR}},                                                                // 0x927
    {true, "ALLOCATE_STREAMED_SCRIPT_TO_RANDOM_PED", {ARG_INT, ARG_INT, ARG_INT}, {}},                          // 0x928
    {true, "ALLOCATE_STREAMED_SCRIPT_TO_OBJECT", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_INT}, {}},          // 0x929
    {false, "SET_PLAYER_CAN_BE_DAMAGED", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                    // 0x92A
    {true, "GET_GROUP_MEMBER", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_GROUP, TYPE_UNIMPORTANT, TYPE_CHAR}},  // 0x92B
    {false, "GET_PLAYERS_GANG_IN_CAR_ACTIVE", {ARG_INT}, {TYPE_PLAYER}},                                        // 0x92C
    {false, "SET_PLAYERS_GANG_IN_CAR_ACTIVE", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                               // 0x92D
    {true, "GET_WATER_HEIGHT_AT_COORDS", {ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_FLOAT}, {}},                // 0x92E
    {true, "CAMERA_PERSIST_TRACK", {ARG_INT}, {}},                                                              // 0x92F
    {true, "CAMERA_PERSIST_POS", {ARG_INT}, {}},                                                                // 0x930
    {true, "CAMERA_PERSIST_FOV", {ARG_INT}, {}},                                                                // 0x931
    {false, "CAMERA_IS_FOV_RUNNING", {}, {}},                                                                   // 0x932
    {true, "CAMERA_IS_VECTOR_MOVE_RUNNING", {}, {}},                                                            // 0x933
    {true, "CAMERA_IS_VECTOR_TRACK_RUNNING", {}, {}},                                                           // 0x934
    {false, "CAMERA_IS_SHAKE_RUNNING", {}, {}},                                                                 // 0x935
    {true, "CAMERA_SET_VECTOR_MOVE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},              // 0x936
    {true, "DRAW_WINDOW", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_INT}, {}},           // 0x937
    {false, "CLEAR_ALL_QUEUED_DIALOGUE", {}, {}},                                                               // 0x938
    {true, "ATTACH_CAR_TO_OBJECT", {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CAR, TYPE_OBJECT}},                                                                               // 0x939
    {true, "SET_GARAGE_RESPRAY_FREE", {ARG_TEXT_LABEL, ARG_INT}, {}},                                           // 0x93A
    {true, "SET_CHAR_BULLETPROOF_VEST", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                       // 0x93B
    {false, "SET_ONSCREEN_COUNTER_COLOUR", {ARG_INT, ARG_INT}, {}},                                             // 0x93C
    {true, "SET_CINEMA_CAMERA", {ARG_INT}, {}},                                                                 // 0x93D
    {true, "SET_CHAR_FIRE_DAMAGE_MULTIPLIER", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                               // 0x93E
    {false, "IS_FIRE_BUTTON_PRESSED", {ARG_INT}, {TYPE_PLAYER}},                                                // 0x93F
    {true, "SET_GROUP_FOLLOW_STATUS", {ARG_INT, ARG_INT}, {TYPE_GROUP}},                                        // 0x940
    {true, "SET_SEARCHLIGHT_CLIP_IF_COLLIDING", {ARG_INT, ARG_INT}, {TYPE_SEARCHLIGHT}},                        // 0x941
    {true, "HAS_PLAYER_BOUGHT_ITEM", {ARG_INT}, {}},                                                            // 0x942
    {false, "SET_CAMERA_BEHIND_CHAR", {ARG_INT}, {TYPE_CHAR}},                                                  // 0x943
    {true, "SET_CAMERA_IN_FRONT_OF_CHAR", {ARG_INT}, {TYPE_CHAR}},                                              // 0x944
    {true, "GET_PLAYER_MAX_ARMOUR", {ARG_INT, ARG_RETURN_INT}, {TYPE_PLAYER}},                                  // 0x945
    {true, "SET_CHAR_USES_UPPERBODY_DAMAGE_ANIMS_ONLY", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                       // 0x946
    {true, "SET_CHAR_SAY_CONTEXT", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                            // 0x947
    {true, "ADD_EXPLOSION_VARIABLE_SHAKE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_FLOAT}, {}},          // 0x948
    {true, "ATTACH_MISSION_AUDIO_TO_CHAR", {ARG_INT, ARG_INT}, {TYPE_UNIMPORTANT, TYPE_CHAR}},                  // 0x949
    {true, "UPDATE_PICKUP_MONEY_PER_DAY", {ARG_INT, ARG_INT}, {TYPE_PICKUP}},                                   // 0x94A
    {true, "GET_NAME_OF_ENTRY_EXIT_CHAR_USED", {ARG_INT, ARG_VAR_TEXT_LABEL16}, {TYPE_CHAR}},                   // 0x94B
    {true, "GET_POSITION_OF_ENTRY_EXIT_CHAR_USED",
        {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {TYPE_CHAR}},        // 0x94C
    {true, "IS_CHAR_TALKING", {ARG_INT}, {TYPE_CHAR}},                                                          // 0x94D
    {true, "DISABLE_CHAR_SPEECH", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                             // 0x94E
    {true, "ENABLE_CHAR_SPEECH", {ARG_INT}, {TYPE_CHAR}},                                                       // 0x94F
    {true, "SET_UP_SKIP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                                    // 0x950
    {true, "CLEAR_SKIP", {}, {}},                                                                               // 0x951
    {true, "PRELOAD_BEAT_TRACK", {ARG_INT}, {}},                                                                // 0x952
    {true, "GET_BEAT_TRACK_STATUS", {ARG_RETURN_INT}, {}},                                                      // 0x953
    {true, "PLAY_BEAT_TRACK", {}, {}},                                                                          // 0x954
    {true, "STOP_BEAT_TRACK", {}, {}},                                                                          // 0x955
    {true, "FIND_MAX_NUMBER_OF_GROUP_MEMBERS", {ARG_INT}, {}},                                                  // 0x956
    {true, "VEHICLE_DOES_PROVIDE_COVER", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                       // 0x957
    {true, "CREATE_SNAPSHOT_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},                    // 0x958
    {true, "CREATE_HORSESHOE_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},                   // 0x959
    {true, "CREATE_OYSTER_PICKUP", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT}, {}},                      // 0x95A
    {true, "HAS_OBJECT_BEEN_UPROOTED", {ARG_INT}, {TYPE_OBJECT}},                                               // 0x95B
    {true, "ADD_SMOKE_PARTICLE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT,
            ARG_FLOAT, ARG_FLOAT},
        {}},                                                                             // 0x95C
    {true, "IS_CHAR_STUCK_UNDER_CAR", {ARG_INT}, {TYPE_CHAR}},                           // 0x95D
    {true, "CONTROL_CAR_DOOR", {ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CAR}},      // 0x95E
    {true, "GET_DOOR_ANGLE_RATIO", {ARG_INT, ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},    // 0x95F
    {true, "SET_PLAYER_DISPLAY_VITAL_STATS_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},  // 0x960
    {true, "SET_CHAR_KEEP_TASK", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                       // 0x961
    {false, "DOES_CAR_HAVE_ROOF", {ARG_INT}, {TYPE_CAR}},                                // 0x962
    {false, "SET_BLIP_FADE", {ARG_INT, ARG_INT}, {TYPE_BLIP}},                           // 0x963
    {true, "CREATE_MENU_GRID",
        {ARG_TEXT_LABEL, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {}},                                                                                              // 0x964
    {true, "IS_CHAR_SWIMMING", {ARG_INT}, {TYPE_CHAR}},                                                   // 0x965
    {true, "GET_CHAR_SWIM_STATE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                // 0x966
    {true, "START_CHAR_FACIAL_TALK", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                    // 0x967
    {true, "STOP_CHAR_FACIAL_TALK", {ARG_INT}, {TYPE_CHAR}},                                              // 0x968
    {true, "IS_BIG_VEHICLE", {ARG_INT}, {TYPE_CAR}},                                                      // 0x969
    {true, "SWITCH_POLICE_HELIS", {ARG_INT}, {}},                                                         // 0x96A
    {true, "STORE_CAR_MOD_STATE", {}, {}},                                                                // 0x96B
    {true, "RESTORE_CAR_MOD_STATE", {}, {}},                                                              // 0x96C
    {true, "GET_CURRENT_CAR_MOD", {ARG_INT, ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                        // 0x96D
    {true, "IS_CAR_LOW_RIDER", {ARG_INT}, {TYPE_CAR}},                                                    // 0x96E
    {true, "IS_CAR_STREET_RACER", {ARG_INT}, {TYPE_CAR}},                                                 // 0x96F
    {true, "FORCE_DEATH_RESTART", {}, {}},                                                                // 0x970
    {true, "SYNC_WATER", {}, {}},                                                                         // 0x971
    {true, "SET_CHAR_COORDINATES_NO_OFFSET", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {TYPE_CHAR}},    // 0x972
    {true, "DOES_SCRIPT_FIRE_EXIST", {ARG_INT}, {TYPE_SCRIPT_FIRE}},                                      // 0x973
    {true, "RESET_STUFF_UPON_RESURRECTION", {}, {}},                                                      // 0x974
    {true, "IS_EMERGENCY_SERVICES_VEHICLE", {ARG_INT}, {TYPE_CAR}},                                       // 0x975
    {true, "KILL_FX_SYSTEM_NOW", {ARG_INT}, {TYPE_FX_SYSTEM}},                                            // 0x976
    {true, "IS_OBJECT_WITHIN_BRAIN_ACTIVATION_RANGE", {ARG_INT}, {TYPE_OBJECT}},                          // 0x977
    {true, "COPY_SHARED_CHAR_DECISION_MAKER", {ARG_INT, ARG_RETURN_INT},
        {TYPE_DECISION_MAKER, TYPE_DECISION_MAKER}},                                                      // 0x978
    {false, "LOAD_SHARED_CHAR_DECISION_MAKER", {ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_DECISION_MAKER}},                                                         // 0x979
    {true, "REPORT_MISSION_AUDIO_EVENT_AT_POSITION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT}, {}},     // 0x97A
    {true, "REPORT_MISSION_AUDIO_EVENT_AT_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                    // 0x97B
    {true, "ATTACH_MISSION_AUDIO_TO_OBJECT", {ARG_INT, ARG_INT}, {TYPE_UNIMPORTANT, TYPE_OBJECT}},        // 0x97C
    {true, "GET_NUM_CAR_COLOURS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                                 // 0x97D
    {false, "IS_POLICE_VEHICLE_IN_PURSUIT", {ARG_INT}, {}},                                               // 0x97E
    {false, "GET_CAR_COLOUR_FROM_MENU_INDEX",
        {ARG_INT, ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {TYPE_CAR}},  // 0x97F
    {true, "EXTINGUISH_FIRE_AT_POINT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                 // 0x980
    {true, "HAS_TRAIN_DERAILED", {ARG_INT}, {TYPE_CAR}},                                                  // 0x981
    {true, "SET_CHAR_FORCE_DIE_IN_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                 // 0x982
    {true, "SET_ONLY_CREATE_GANG_MEMBERS", {ARG_INT}, {}},                                                // 0x983
    {true, "GET_OBJECT_MODEL", {ARG_INT, ARG_RETURN_INT}, {TYPE_OBJECT}},                                 // 0x984
    {true, "SET_CHAR_USES_COLLISION_CLOSEST_OBJECT_OF_TYPE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                             // 0x985
    {true, "CLEAR_ALL_SCRIPT_FIRE_FLAGS", {}, {}},                                                   // 0x986
    {true, "GET_CAR_BLOCKING_CAR", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR, TYPE_CAR}},                 // 0x987
    {true, "GET_CURRENT_VEHICLE_PAINTJOB", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                   // 0x988
    {true, "SET_HELP_MESSAGE_BOX_SIZE", {ARG_INT}, {}},                                              // 0x989
    {true, "SET_GUNSHOT_SENSE_RANGE_FOR_RIOT2", {ARG_FLOAT}, {}},                                    // 0x98A
    {true, "STRING_CAT16", {ARG_VAR_TEXT_LABEL16, ARG_VAR_TEXT_LABEL16, ARG_VAR_TEXT_LABEL16}, {}},  // 0x98B
    {true, "STRING_CAT8", {ARG_VAR_TEXT_LABEL, ARG_VAR_TEXT_LABEL, ARG_VAR_TEXT_LABEL}, {}},         // 0x98C
    {true, "GET_CAR_MOVING_COMPONENT_OFFSET", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},              // 0x98D
    {true, "SET_NAMED_ENTRY_EXIT_FLAG", {ARG_TEXT_LABEL, ARG_INT, ARG_INT}, {}},                     // 0x98E
    {false, "RADIANS_TO_DEGREES", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                // 0x98F
    {false, "DEGREES_TO_RADIANS", {ARG_FLOAT, ARG_RETURN_FLOAT}, {}},                                // 0x990
    {true, "PAUSE_CURRENT_BEAT_TRACK", {ARG_INT}, {}},                                               // 0x991
    {true, "SET_PLAYER_CYCLE_WEAPON_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                     // 0x992
    {false, "SET_CHAR_AIR_RESISTANCE_MULTIPLIER", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                // 0x993
    {true, "MARK_ROAD_NODE_AS_DONT_WANDER", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                  // 0x994
    {true, "UNMARK_ALL_ROAD_NODES_AS_DONT_WANDER", {}, {}},                                          // 0x995
    {true, "SET_CHECKPOINT_HEADING", {ARG_INT, ARG_FLOAT}, {TYPE_CHECKPOINT}},                       // 0x996
    {true, "SET_MISSION_RESPECT_TOTAL", {ARG_INT_CONST}, {}},                                        // 0x997
    {true, "AWARD_PLAYER_MISSION_RESPECT", {ARG_INT_CONST}, {}},                                     // 0x998
    {false, "SET_PLAYER_FIRE_WITH_SHOULDER_BUTTON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},              // 0x999
    {true, "SET_CAR_COLLISION", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                     // 0x99A
    {true, "CHANGE_PLAYBACK_TO_USE_AI", {ARG_INT}, {TYPE_CAR}},                                      // 0x99B
    {true, "CAMERA_SET_SHAKE_SIMULATION_SIMPLE", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {}},               // 0x99C
    {true, "IS_NIGHT_VISION_ACTIVE", {}, {}},                                                        // 0x99D
    {true, "SET_CREATE_RANDOM_COPS", {ARG_INT}, {}},                                                 // 0x99E
    {true, "TASK_SET_IGNORE_WEAPON_RANGE_FLAG", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                    // 0x99F
    {true, "TASK_PICK_UP_SECOND_OBJECT",
        {ARG_INT, ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16,
            ARG_INT},
        {TYPE_CHAR, TYPE_OBJECT}},                                                                   // 0x9A0
    {true, "DROP_SECOND_OBJECT", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                   // 0x9A1
    {true, "REMOVE_OBJECT_ELEGANTLY", {ARG_INT}, {TYPE_OBJECT}},                                     // 0x9A2
    {true, "DRAW_CROSSHAIR", {ARG_INT}, {}},                                                         // 0x9A3
    {true, "SET_UP_CONVERSATION_NODE_WITH_SPEECH",
        {ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},            // 0x9A4
    {false, "SET_CCTV_EFFECT", {ARG_INT}, {}},                                                       // 0x9A5
    {true, "SHOW_BLIPS_ON_ALL_LEVELS", {ARG_INT}, {}},                                               // 0x9A6
    {true, "SET_CHAR_DRUGGED_UP", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                  // 0x9A7
    {true, "IS_CHAR_HEAD_MISSING", {ARG_INT}, {TYPE_CHAR}},                                          // 0x9A8
    {true, "GET_HASH_KEY", {ARG_TEXT_LABEL16, ARG_RETURN_INT}, {}},                                  // 0x9A9
    {true, "SET_UP_CONVERSATION_END_NODE_WITH_SPEECH", {ARG_TEXT_LABEL, ARG_INT}, {}},               // 0x9AA
    {true, "RANDOM_PASSENGER_SAY", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                  // 0x9AB
    {true, "HIDE_ALL_FRONTEND_BLIPS", {ARG_INT}, {}},                                                // 0x9AC
    {true, "SET_PLAYER_IN_CAR_CAMERA_MODE", {ARG_INT}, {}},                                          // 0x9AD
    {true, "IS_CHAR_IN_ANY_TRAIN", {ARG_INT}, {TYPE_CHAR}},                                          // 0x9AE
    {true, "SET_UP_SKIP_AFTER_MISSION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},           // 0x9AF
    {true, "SET_VEHICLE_IS_CONSIDERED_BY_PLAYER", {ARG_INT, ARG_INT}, {TYPE_CAR}},                   // 0x9B0
    {false, "GET_CPU_LEVEL", {ARG_RETURN_INT}, {}},                                                  // 0x9B1
    {true, "GET_RANDOM_CAR_MODEL_IN_MEMORY", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {}},         // 0x9B2
    {true, "GET_CAR_DOOR_LOCK_STATUS", {ARG_INT, ARG_RETURN_INT}, {TYPE_CAR}},                       // 0x9B3
    {true, "SET_CLOSEST_ENTRY_EXIT_FLAG", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT}, {}},  // 0x9B4
    {true, "SET_CHAR_SIGNAL_AFTER_KILL", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                           // 0x9B5
    {true, "SET_CHAR_WANTED_BY_POLICE", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                            // 0x9B6
    {true, "SET_ZONE_NO_COPS", {ARG_TEXT_LABEL, ARG_INT}, {}},                                       // 0x9B7
    {true, "ADD_BLOOD", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CHAR}},                          // 0x9B8
    {true, "DISPLAY_CAR_NAMES", {ARG_INT}, {}},                     // 0x9B9
    {true, "DISPLAY_ZONE_NAMES", {ARG_INT}, {}},                    // 0x9BA
    {true, "IS_CAR_DOOR_DAMAGED", {ARG_INT, ARG_INT}, {TYPE_CAR}},  // 0x9BB
    {true, "SET_CHAR_COORDINATES_DONT_WARP_GANG_NO_OFFSET", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_CHAR}},                                               // 0x9BC
    {true, "SET_MINIGAME_IN_PROGRESS", {ARG_INT}, {}},              // 0x9BD
    {true, "IS_MINIGAME_IN_PROGRESS", {}, {}},                      // 0x9BE
    {true, "SET_FORCE_RANDOM_CAR_MODEL", {ARG_INT}, {}},            // 0x9BF
    {true, "GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CAR}},                                            // 0x9C0
    {true, "ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS", {ARG_INT}, {}},  // 0x9C1
    {true, "FAIL_KILL_FRENZY", {}, {}},                            // 0x9C2
    {true, "IS_COP_VEHICLE_IN_AREA_3D_NO_SAVE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CAR}},                                                                                  // 0x9C3
    {true, "SET_PETROL_TANK_WEAKPOINT", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                 // 0x9C4
    {true, "IS_CHAR_USING_MAP_ATTRACTOR", {ARG_INT}, {TYPE_CHAR}},                                       // 0x9C5
    {false, "SET_ALL_CARS_IN_AREA_VISIBLE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {}},                                                                                             // 0x9C6
    {true, "SET_PLAYER_MODEL", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                       // 0x9C7
    {true, "ARE_SUBTITLES_SWITCHED_ON", {}, {}},                                                         // 0x9C8
    {true, "REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CAR}},         // 0x9C9
    {true, "SET_OBJECT_PROOFS", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_OBJECT}},  // 0x9CA
    {true, "IS_CAR_TOUCHING_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR, TYPE_CAR}},                             // 0x9CB
    {true, "DOES_OBJECT_HAVE_THIS_MODEL", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                            // 0x9CC
    {false, "IS_ITALIAN_GAME", {}, {}},                                                                  // 0x9CD
    {false, "IS_SPANISH_GAME", {}, {}},                                                                  // 0x9CE
    {true, "SET_TRAIN_FORCED_TO_SLOW_DOWN", {ARG_INT, ARG_INT}, {TYPE_CAR}},                             // 0x9CF
    {true, "IS_VEHICLE_ON_ALL_WHEELS", {ARG_INT}, {TYPE_CAR}},                                           // 0x9D0
    {true, "DOES_PICKUP_EXIST", {ARG_INT}, {TYPE_PICKUP}},                                               // 0x9D1
    {true, "ENABLE_AMBIENT_CRIME", {ARG_INT}, {}},                                                       // 0x9D2
    {false, "IS_AMBIENT_CRIME_ENABLED", {}, {}},                                                         // 0x9D3
    {true, "CLEAR_WANTED_LEVEL_IN_GARAGE", {}, {}},                                                      // 0x9D4
    {true, "SET_CHAR_SAY_CONTEXT_IMPORTANT", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_RETURN_INT},
        {TYPE_CHAR}},                                                                                    // 0x9D5
    {true, "SET_CHAR_SAY_SCRIPT", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_CHAR}},           // 0x9D6
    {true, "FORCE_INTERIOR_LIGHTING_FOR_PLAYER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                     // 0x9D7
    {true, "DISABLE_2ND_PAD_FOR_DEBUG", {ARG_INT}, {}},                                                  // 0x9D8
    {true, "USE_DETONATOR", {}, {}},                                                                     // 0x9D9
    {true, "IS_MONEY_PICKUP_AT_COORDS", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                          // 0x9DA
    {true, "SET_MENU_COLUMN_WIDTH", {ARG_INT, ARG_INT, ARG_INT}, {}},                                    // 0x9DB
    {false, "SET_CHAR_CAN_CLIMB_OUT_WATER", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                            // 0x9DC
    {true, "MAKE_ROOM_IN_PLAYER_GANG_FOR_MISSION_PEDS", {ARG_INT}, {}},                                  // 0x9DD
    {true, "IS_CHAR_GETTING_IN_TO_A_CAR", {ARG_INT}, {TYPE_CHAR}},                                       // 0x9DE
    {false, "RESTORE_PLAYER_AFTER_2P_GAME", {}, {}},                                                     // 0x9DF
    {true, "SET_UP_SKIP_FOR_SPECIFIC_VEHICLE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},             // 0x9E0
    {true, "GET_CAR_MODEL_VALUE", {ARG_INT, ARG_RETURN_INT}, {}},                                        // 0x9E1
    {true, "CREATE_CAR_GENERATOR_WITH_PLATE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT,
            ARG_INT, ARG_TEXT_LABEL16, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_UNIMPORTANT, TYPE_CAR_GENERATOR}},                                                             // 0x9E2
    {true, "FIND_TRAIN_DIRECTION", {ARG_INT}, {TYPE_CAR}},                                                      // 0x9E3
    {true, "SET_AIRCRAFT_CARRIER_SAM_SITE", {ARG_INT}, {}},                                                     // 0x9E4
    {true, "DRAW_LIGHT_WITH_RANGE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT},
        {}},                                                                                                    // 0x9E5
    {true, "ENABLE_BURGLARY_HOUSES", {ARG_INT}, {}},                                                            // 0x9E6
    {true, "IS_PLAYER_CONTROL_ON", {ARG_INT}, {TYPE_PLAYER}},                                                   // 0x9E7
    {true, "GET_CHAR_AREA_VISIBLE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR}},                                    // 0x9E8
    {true, "GIVE_NON_PLAYER_CAR_NITRO", {ARG_INT}, {TYPE_CAR}},                                                 // 0x9E9
    {false, "PLAYER_PUT_ON_GOGGLES", {ARG_INT}, {TYPE_PLAYER}},                                                 // 0x9EA
    {true, "PLAYER_TAKE_OFF_GOGGLES", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                       // 0x9EB
    {true, "ALLOW_FIXED_CAMERA_COLLISION", {ARG_INT}, {}},                                                      // 0x9EC
    {true, "HAS_CHAR_SPOTTED_CHAR_IN_FRONT", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                       // 0x9ED
    {true, "FORCE_BIG_MESSAGE_AND_COUNTER", {ARG_INT}, {}},                                                     // 0x9EE
    {true, "SET_VEHICLE_CAMERA_TWEAK", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                         // 0x9EF
    {true, "RESET_VEHICLE_CAMERA_TWEAK", {}, {}},                                                               // 0x9F0
    {true, "REPORT_MISSION_AUDIO_EVENT_AT_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                              // 0x9F1
    {true, "DOES_DECISION_MAKER_EXIST", {ARG_INT}, {TYPE_DECISION_MAKER}},                                      // 0x9F2
    {false, "GET_RANDOM_TRAIN_IN_SPHERE_NO_SAVE", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},  // 0x9F3
    {true, "IGNORE_HEIGHT_DIFFERENCE_FOLLOWING_NODES", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                        // 0x9F4
    {true, "SHUT_ALL_CHARS_UP", {ARG_INT}, {}},                                                                 // 0x9F5
    {true, "SET_CHAR_GET_OUT_UPSIDE_DOWN_CAR", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                // 0x9F6
    {true, "REPORT_MISSION_AUDIO_EVENT_AT_CAR", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                // 0x9F7
    {true, "DO_WEAPON_STUFF_AT_START_OF_2P_GAME", {}, {}},                                                      // 0x9F8
    {false, "SET_MENU_HEADER_ORIENTATION", {ARG_INT, ARG_INT, ARG_INT}, {}},                                    // 0x9F9
    {true, "HAS_GAME_JUST_RETURNED_FROM_FRONTEND", {}, {}},                                                     // 0x9FA
    {true, "GET_CURRENT_LANGUAGE", {ARG_RETURN_INT}, {}},                                                       // 0x9FB
    {true, "IS_OBJECT_INTERSECTING_WORLD", {ARG_INT}, {TYPE_OBJECT}},                                           // 0x9FC
    {true, "GET_STRING_WIDTH", {ARG_TEXT_LABEL, ARG_RETURN_INT}, {}},                                           // 0x9FD
    {true, "RESET_VEHICLE_HYDRAULICS", {ARG_INT}, {TYPE_CAR}},                                                  // 0x9FE
    {true, "SET_RESPAWN_POINT_FOR_DURATION_OF_MISSION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                 // 0x9FF
    {false, "IS_THIS_MODEL_A_BIKE", {ARG_INT}, {}},                                                             // 0xA00
    {true, "IS_THIS_MODEL_A_CAR", {ARG_INT}, {}},                                                               // 0xA01
    {true, "SWITCH_ON_GROUND_SEARCHLIGHT", {ARG_INT, ARG_INT}, {TYPE_SEARCHLIGHT}},                             // 0xA02
    {true, "IS_GANG_WAR_FIGHTING_GOING_ON", {}, {}},                                                            // 0xA03
    {false, "SET_VEHICLE_FIRING_RATE_MULTIPLIER", {ARG_INT, ARG_FLOAT}, {TYPE_CAR}},                            // 0xA04
    {false, "GET_VEHICLE_FIRING_RATE_MULTIPLIER", {ARG_INT, ARG_RETURN_FLOAT}, {TYPE_CAR}},                     // 0xA05
    {true, "IS_NEXT_STATION_ALLOWED", {ARG_INT}, {TYPE_CAR}},                                                   // 0xA06
    {true, "SKIP_TO_NEXT_ALLOWED_STATION", {ARG_INT}, {TYPE_CAR}},                                              // 0xA07
    {true, "GET_STRING_WIDTH_WITH_NUMBER", {ARG_TEXT_LABEL, ARG_INT, ARG_RETURN_INT}, {}},                      // 0xA08
    {true, "SHUT_CHAR_UP_FOR_SCRIPTED_SPEECH", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                // 0xA09
    {true, "ENABLE_DISABLED_ATTRACTORS_ON_OBJECT", {ARG_INT, ARG_INT}, {TYPE_OBJECT}},                          // 0xA0A
    {true, "LOAD_SCENE_IN_DIRECTION", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                        // 0xA0B
    {true, "IS_PLAYER_USING_JETPACK", {ARG_INT}, {TYPE_PLAYER}},                                                // 0xA0C
    {false, "BLOCK_VEHICLE_MODEL", {ARG_INT, ARG_INT}, {}},                                                     // 0xA0D
    {true, "CLEAR_THIS_PRINT_BIG_NOW", {ARG_INT}, {}},                                                          // 0xA0E
    {true, "HAS_LANGUAGE_CHANGED", {}, {}},                                                                     // 0xA0F
    {true, "INCREMENT_INT_STAT_NO_MESSAGE", {ARG_INT, ARG_INT}, {}},                                            // 0xA10
    {true, "SET_EXTRA_CAR_COLOURS", {ARG_INT, ARG_INT, ARG_INT}, {TYPE_CAR}},                                   // 0xA11
    {true, "GET_EXTRA_CAR_COLOURS", {ARG_INT, ARG_RETURN_INT, ARG_RETURN_INT}, {TYPE_CAR}},                     // 0xA12
    {true, "MANAGE_ALL_POPULATION", {}, {}},                                                                    // 0xA13
    {true, "SET_NO_RESPRAYS", {ARG_INT}, {}},                                                                   // 0xA14
    {true, "HAS_CAR_BEEN_RESPRAYED", {ARG_INT}, {TYPE_CAR}},                                                    // 0xA15
    {true, "ATTACH_MISSION_AUDIO_TO_CAR", {ARG_INT, ARG_INT}, {TYPE_UNIMPORTANT, TYPE_CAR}},                    // 0xA16
    {true, "SET_HAS_BEEN_OWNED_FOR_CAR_GENERATOR", {ARG_INT, ARG_INT}, {TYPE_CAR_GENERATOR}},                   // 0xA17
    {true, "SET_UP_CONVERSATION_NODE_WITH_SCRIPTED_SPEECH",
        {ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_TEXT_LABEL, ARG_INT, ARG_INT, ARG_INT}, {}},                       // 0xA18
    {true, "SET_AREA_NAME", {ARG_TEXT_LABEL}, {}},                                                              // 0xA19
    {true, "TASK_PLAY_ANIM_SECONDARY",
        {ARG_INT, ARG_TEXT_LABEL16, ARG_TEXT_LABEL16, ARG_FLOAT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT},
        {TYPE_CHAR}},                                                                                           // 0xA1A
    {true, "IS_CHAR_TOUCHING_CHAR", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                                // 0xA1B
    {true, "DISABLE_HELI_AUDIO", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                               // 0xA1C
    {true, "TASK_HAND_GESTURE", {ARG_INT, ARG_INT}, {TYPE_CHAR, TYPE_CHAR}},                                    // 0xA1D
    {true, "TAKE_PHOTO", {ARG_INT}, {}},                                                                        // 0xA1E
    {true, "INCREMENT_FLOAT_STAT_NO_MESSAGE", {ARG_INT, ARG_FLOAT}, {}},                                        // 0xA1F
    {true, "SET_PLAYER_GROUP_TO_FOLLOW_ALWAYS", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                             // 0xA20
    {true, "IMPROVE_CAR_BY_CHEATING", {ARG_INT, ARG_INT}, {TYPE_CAR}},                                          // 0xA21
    {true, "CHANGE_CAR_COLOUR_FROM_MENU", {ARG_INT, ARG_INT, ARG_INT, ARG_INT}, {TYPE_UNIMPORTANT, TYPE_CAR}},  // 0xA22
    {true, "HIGHLIGHT_MENU_ITEM", {ARG_INT, ARG_INT, ARG_INT}, {}},                                             // 0xA23
    {true, "SET_DISABLE_MILITARY_ZONES", {ARG_INT}, {}},                                                        // 0xA24
    {true, "SET_CAMERA_POSITION_UNFIXED", {ARG_FLOAT, ARG_FLOAT}, {}},                                          // 0xA25
    {true, "SET_RADIO_TO_PLAYERS_FAVOURITE_STATION", {}, {}},                                                   // 0xA26
    {true, "SET_DEATH_WEAPONS_PERSIST", {ARG_INT, ARG_INT}, {TYPE_CHAR}},                                       // 0xA27
    {true, "SET_SWIM_SPEED", {ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                                                // 0xA28
    {true, "IS_PLAYER_CLIMBING", {ARG_INT}, {TYPE_PLAYER}},                                                     // 0xA29
    {true, "IS_THIS_HELP_MESSAGE_BEING_DISPLAYED", {ARG_TEXT_LABEL}, {}},                                       // 0xA2A
    {true, "IS_WIDESCREEN_ON_IN_OPTIONS", {}, {}},                                                              // 0xA2B
    {true, "DRAW_SUBTITLES_BEFORE_FADE", {ARG_INT}, {}},                                                        // 0xA2C
    {true, "DRAW_ODDJOB_TITLE_BEFORE_FADE", {ARG_INT}, {}},                                                     // 0xA2D
    {true, "TASK_FOLLOW_PATH_NODES_TO_COORD_WITH_RADIUS",
        {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_INT, ARG_FLOAT}, {TYPE_CHAR}},                  // 0xA2E
    {true, "SET_PHOTO_CAMERA_EFFECT", {ARG_INT}, {}},                                                           // 0xA2F
    {true, "FIX_CAR", {ARG_INT}, {TYPE_CAR}},                                                                   // 0xA30
    {true, "SET_PLAYER_GROUP_TO_FOLLOW_NEVER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                              // 0xA31
    {true, "IS_CHAR_ATTACHED_TO_ANY_CAR", {ARG_INT}, {TYPE_CHAR}},                                              // 0xA32
    {true, "STORE_CAR_CHAR_IS_ATTACHED_TO_NO_SAVE", {ARG_INT, ARG_RETURN_INT}, {TYPE_CHAR, TYPE_CAR}},          // 0xA33
    {false, "SET_UP_SKIP_TO_BE_FINISHED_BY_SCRIPT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},          // 0xA34
    {true, "SET_UP_SKIP_FOR_VEHICLE_FINISHED_BY_SCRIPT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_CAR}},                    // 0xA35
    {true, "IS_SKIP_WAITING_FOR_SCRIPT_TO_FADE_IN", {}, {}},                                                    // 0xA36
    {true, "FORCE_ALL_VEHICLE_LIGHTS_OFF", {ARG_INT}, {}},                                                      // 0xA37
    {false, "SET_RENDER_PLAYER_WEAPON", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                                     // 0xA38
    {true, "GET_PLAYER_IN_CAR_CAMERA_MODE", {ARG_RETURN_INT}, {}},                                              // 0xA39
    {true, "IS_LAST_BUILDING_MODEL_SHOT_BY_PLAYER", {ARG_INT, ARG_INT}, {TYPE_PLAYER}},                         // 0xA3A
    {true, "CLEAR_LAST_BUILDING_MODEL_SHOT_BY_PLAYER", {ARG_INT}, {TYPE_PLAYER}},                               // 0xA3B
    {true, "SET_UP_CONVERSATION_END_NODE_WITH_SCRIPTED_SPEECH", {ARG_TEXT_LABEL, ARG_INT}, {}},                 // 0xA3C
    {true, "ACTIVATE_PIMP_CHEAT", {ARG_INT}, {}},                                                               // 0xA3D
    {true, "GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE",
        {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_RETURN_INT},
        {TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT, TYPE_UNIMPORTANT,
            TYPE_CHAR}},                                                                                       // 0xA3E
    {true, "SET_SCRIPT_COOP_GAME", {ARG_INT}, {}},                                                             // 0xA3F
    {true, "CREATE_USER_3D_MARKER", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_INT, ARG_RETURN_INT}, {}},           // 0xA40
    {true, "REMOVE_USER_3D_MARKER", {ARG_INT}, {}},                                                            // 0xA41
    {false, "REMOVE_ALLUSER_3D_MARKERS", {}, {}},                                                              // 0xA42
    {true, "GET_RID_OF_PLAYER_PROSTITUTE", {}, {}},                                                            // 0xA43
    {true, "DISPLAY_NON_MINIGAME_HELP_MESSAGES", {ARG_INT}, {}},                                               // 0xA44
    {true, "SET_RAILTRACK_RESISTANCE_MULT", {ARG_FLOAT}, {}},                                                  // 0xA45
    {true, "SWITCH_OBJECT_BRAINS", {ARG_INT, ARG_INT}, {}},                                                    // 0xA46
    {true, "FINISH_SETTING_UP_CONVERSATION_NO_SUBTITLES", {}, {}},                                             // 0xA47
    {true, "ALLOW_PAUSE_IN_WIDESCREEN", {ARG_INT}, {}},                                                        // 0xA48
    {true, "IS_XBOX_VERSION", {}, {}},                                                                         // 0xA49
    {true, "GET_PC_MOUSE_MOVEMENT", {ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},                                 // 0xA4A
    {true, "IS_PC_USING_JOYPAD", {}, {}},                                                                      // 0xA4B
    {true, "IS_MOUSE_USING_VERTICAL_INVERSION", {}, {}},                                                       // 0xA4C
    {true, "IS_JAPANESE_VERSION", {}, {}},                                                                     // 0xA4D
    {true, "IS_XBOX_PLAYER2_PRESSING_START", {}, {}},                                                          // 0xA4E
    {true, "FINISHED_WITH_XBOX_PLAYER2", {}, {}},                                                              // 0xA4F
    {true, "DO_DEBUG_STUFF", {}, {}},                                                                          // 0xA50
    {true, "IS_WIDGET_PRESSED", {ARG_INT}, {}},                                                                // 0xA51
    {true, "IS_WIDGET_RELEASED", {ARG_INT}, {}},                                                               // 0xA52
    {true, "IS_WIDGET_DOUBLETAPPED", {ARG_INT}, {}},                                                           // 0xA53
    {true, "IS_WIDGET_SWIPED", {ARG_INT}, {}},                                                                 // 0xA54
    {true, "IS_WIDGET_SWIPED_LEFT", {ARG_INT}, {}},                                                            // 0xA55
    {true, "IS_WIDGET_SWIPED_RIGHT", {ARG_INT}, {}},                                                           // 0xA56
    {true, "DO_MISSION_SKIP", {ARG_RETURN_INT, ARG_RETURN_INT}, {}},                                           // 0xA57
    {true, "GET_MISSION_NUM", {ARG_RETURN_INT}, {}},                                                           // 0xA58
    {true, "GET_MISSION_PAGE", {ARG_RETURN_INT}, {}},                                                          // 0xA59
    {true, "GET_WIDGET_VALUE", {ARG_INT, ARG_RETURN_FLOAT}, {}},                                               // 0xA5A
    {true, "GET_WIDGET_VALUE2", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT}, {}},                            // 0xA5B
    {true, "DISPLAY_TEXT_WIDGET", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL}, {}},                                 // 0xA5C
    {true, "GET_WIDGET_POSITION", {ARG_INT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT, ARG_RETURN_FLOAT},
        {}},                                                                                                   // 0xA5D
    {true, "SET_WIDGET_VALUE", {ARG_INT, ARG_FLOAT}, {}},                                                      // 0xA5E
    {true, "SET_SLIDER_RANGE", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {}},                                           // 0xA5F
    {true, "ADD_WIDGET_FLAG", {ARG_INT, ARG_INT}, {}},                                                         // 0xA60
    {true, "REMOVE_WIDGET_FLAG", {ARG_INT, ARG_INT}, {}},                                                      // 0xA61
    {true, "ADD_BUTTON_FLAG", {ARG_INT, ARG_INT}, {}},                                                         // 0xA62
    {true, "REMOVE_BUTTON_FLAG", {ARG_INT, ARG_INT}, {}},                                                      // 0xA63
    {true, "IS_TOUCH_ENABLED", {}, {}},                                                                        // 0xA64
    {true, "IS_FINALBUILD", {}, {}},                                                                           // 0xA65
    {true, "WRITE_LOG", {ARG_STRING, ARG_STRING, ARG_STRING, ARG_STRING}, {}},                                 // 0xA66
    {true, "WRITE_LOG_INT", {ARG_INT, ARG_INT, ARG_INT, ARG_STRING, ARG_STRING, ARG_STRING, ARG_STRING}, {}},  // 0xA67
    {true, "WRITE_LOG_FLOAT", {ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_STRING, ARG_STRING, ARG_STRING, ARG_STRING},
        {}},                                                                                                   // 0xA68
    {true, "CREATE_SHOP_WIDGET", {ARG_TEXT_LABEL}, {}},                                                        // 0xA69
    {true, "ADD_SHOP_ITEM", {ARG_TEXT_LABEL, ARG_INT}, {}},                                                    // 0xA6A
    {true, "DELETE_WIDGET", {ARG_INT}, {}},                                                                    // 0xA6B
    {true, "SET_EQUIPPED_ITEM", {ARG_INT, ARG_INT}, {}},                                                       // 0xA6C
    {true, "PRINT_HELP_FOREVER_CONDITIONAL", {ARG_TEXT_LABEL, ARG_INT}, {}},                                   // 0xA6D
    {true, "SET_WIDGET_TEXTURE", {ARG_INT, ARG_STRING, ARG_STRING, ARG_STRING, ARG_STRING}, {}},               // 0xA6E
    {true, "CHECKPOINT_SAVE", {ARG_INT}, {}},                                                                  // 0xA6F
    {true, "DISPLAY_TEXT_CLAMPED", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_FLOAT}, {}},                     // 0xA70
    {true, "DISPLAY_TEXT_WITH_NUMBER_CLAMPED", {ARG_FLOAT, ARG_FLOAT, ARG_TEXT_LABEL, ARG_INT, ARG_FLOAT},
        {}},                                                                                                   // 0xA71
    {true, "AUTO_SAVE", {}, {}},                                                                               // 0xA72
    {true, "SET_WIDGET_VALUE3", {ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT}, {}},                               // 0xA73
    {true, "IS_CHECKPOINT_RESUMING", {ARG_INT}, {}},                                                           // 0xA74
    {true, "SET_ACTIVE_MENU_ITEM_CAR_MODS", {ARG_INT, ARG_INT}, {}},                                           // 0xA75
    {true, "HID_IMPLEMENTS", {ARG_INT}, {}},                                                                   // 0xA76
    {true, "CHECKPOINT_SAVE_ODDJOB", {ARG_INT}, {}},                                                           // 0xA77
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_TOUCH", {ARG_TEXT_LABEL, ARG_INT}, {}},                             // 0xA78
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_HID", {ARG_TEXT_LABEL, ARG_INT}, {}},                               // 0xA79
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_CLASSIC", {ARG_TEXT_LABEL, ARG_INT}, {}},                     // 0xA7A
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_ADAPTED", {ARG_TEXT_LABEL, ARG_INT}, {}},                     // 0xA7B
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_HID_JOYPAD", {ARG_TEXT_LABEL, ARG_INT}, {}},                        // 0xA7C
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_HID_KEYBOARD", {ARG_TEXT_LABEL, ARG_INT}, {}},                      // 0xA7D
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_ANALOG", {ARG_TEXT_LABEL, ARG_INT}, {}},                      // 0xA7E
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_DIGITAL", {ARG_TEXT_LABEL, ARG_INT}, {}},                     // 0xA7F
    {true, "PRINT_HELP_FOREVER_CONDITIONAL_TOUCH_FLICK", {ARG_TEXT_LABEL, ARG_INT}, {}},                       // 0xA80
    {true, "PRINT_HELP_CONDITIONAL", {ARG_TEXT_LABEL, ARG_INT}, {}},                                           // 0xA81
    {true, "PRINT_HELP_CONDITIONAL_TOUCH", {ARG_TEXT_LABEL, ARG_INT}, {}},                                     // 0xA82
    {true, "PRINT_HELP_CONDITIONAL_HID", {ARG_TEXT_LABEL, ARG_INT}, {}},                                       // 0xA83
    {true, "PRINT_HELP_CONDITIONAL_TOUCH_CLASSIC", {ARG_TEXT_LABEL, ARG_INT}, {}},                             // 0xA84
    {true, "PRINT_HELP_CONDITIONAL_TOUCH_ADAPTED", {ARG_TEXT_LABEL, ARG_INT}, {}},                             // 0xA85
    {true, "PRINT_HELP_CONDITIONAL_HID_JOYPAD", {ARG_TEXT_LABEL, ARG_INT}, {}},                                // 0xA86
    {true, "PRINT_HELP_CONDITIONAL_HID_KEYBOARD", {ARG_TEXT_LABEL, ARG_INT}, {}},                              // 0xA87
    {true, "PRINT_HELP_CONDITIONAL_TOUCH_ANALOG", {ARG_TEXT_LABEL, ARG_INT}, {}},                              // 0xA88
    {true, "SET_WIDGET_VALUE2", {ARG_INT, ARG_FLOAT, ARG_FLOAT}, {}},                                          // 0xA89
    {true, "SET_WIDGET_INFO", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_FLOAT, ARG_TEXT_LABEL},
        {}},                                                                                                   // 0xA8A
    {true, "SET_WIDGET_INFO2", {ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_TEXT_LABEL, ARG_TEXT_LABEL},
        {}},                                                                                                   // 0xA8B
    {true, "LOAD_ALL_STREAMING_MODELS", {}, {}},                                                               // 0xA8C
    {true, "IS_HID_RELEASED", {ARG_INT}, {}},                                                                  // 0xA8D
    {true, "UNLOCK_ACHIEVEMENT", {ARG_INT}, {}},                                                               // 0xA8E
    {true, "RELEASE_WEAPON_LOCK", {}, {}},                                                                     // 0xA8F
    {true, "IS_HID_PRESSED", {ARG_INT}, {}},                                                                   // 0xA90
};

// 0x004976B0
tCommandAlternative g_aCommandAlternatives[NUM_COMMAND_ALTERNATIVES] = {
    {
        "SET",
        {COMMAND_SET_VAR_INT, COMMAND_SET_VAR_FLOAT, COMMAND_SET_LVAR_INT, COMMAND_SET_LVAR_FLOAT,
            COMMAND_SET_VAR_INT_TO_VAR_INT, COMMAND_SET_LVAR_INT_TO_LVAR_INT, COMMAND_SET_VAR_FLOAT_TO_VAR_FLOAT,
            COMMAND_SET_LVAR_FLOAT_TO_LVAR_FLOAT, COMMAND_SET_VAR_FLOAT_TO_LVAR_FLOAT,
            COMMAND_SET_LVAR_FLOAT_TO_VAR_FLOAT, COMMAND_SET_VAR_INT_TO_LVAR_INT, COMMAND_SET_LVAR_INT_TO_VAR_INT,
            COMMAND_SET_VAR_INT_TO_CONSTANT, COMMAND_SET_LVAR_INT_TO_CONSTANT, COMMAND_SET_VAR_TEXT_LABEL,
            COMMAND_SET_LVAR_TEXT_LABEL, COMMAND_SET_VAR_TEXT_LABEL16, COMMAND_SET_LVAR_TEXT_LABEL16, -1, -1},
    },
    {
        "CSET",
        {COMMAND_CSET_VAR_INT_TO_VAR_FLOAT, COMMAND_CSET_VAR_FLOAT_TO_VAR_INT, COMMAND_CSET_LVAR_INT_TO_VAR_FLOAT,
            COMMAND_CSET_LVAR_FLOAT_TO_VAR_INT, COMMAND_CSET_VAR_INT_TO_LVAR_FLOAT, COMMAND_CSET_VAR_FLOAT_TO_LVAR_INT,
            COMMAND_CSET_LVAR_INT_TO_LVAR_FLOAT, COMMAND_CSET_LVAR_FLOAT_TO_LVAR_INT, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1},
    },
    {
        "ADD_THING_TO_THING",
        {COMMAND_ADD_VAL_TO_INT_VAR, COMMAND_ADD_VAL_TO_FLOAT_VAR, COMMAND_ADD_VAL_TO_INT_LVAR,
            COMMAND_ADD_VAL_TO_FLOAT_LVAR, COMMAND_ADD_INT_VAR_TO_INT_VAR, COMMAND_ADD_FLOAT_VAR_TO_FLOAT_VAR,
            COMMAND_ADD_INT_LVAR_TO_INT_LVAR, COMMAND_ADD_FLOAT_LVAR_TO_FLOAT_LVAR, COMMAND_ADD_INT_VAR_TO_INT_LVAR,
            COMMAND_ADD_FLOAT_VAR_TO_FLOAT_LVAR, COMMAND_ADD_INT_LVAR_TO_INT_VAR, COMMAND_ADD_FLOAT_LVAR_TO_FLOAT_VAR,
            -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "SUB_THING_FROM_THING",
        {COMMAND_SUB_VAL_FROM_INT_VAR, COMMAND_SUB_VAL_FROM_FLOAT_VAR, COMMAND_SUB_VAL_FROM_INT_LVAR,
            COMMAND_SUB_VAL_FROM_FLOAT_LVAR, COMMAND_SUB_INT_VAR_FROM_INT_VAR, COMMAND_SUB_FLOAT_VAR_FROM_FLOAT_VAR,
            COMMAND_SUB_INT_LVAR_FROM_INT_LVAR, COMMAND_SUB_FLOAT_LVAR_FROM_FLOAT_LVAR,
            COMMAND_SUB_INT_VAR_FROM_INT_LVAR, COMMAND_SUB_FLOAT_VAR_FROM_FLOAT_LVAR, COMMAND_SUB_INT_LVAR_FROM_INT_VAR,
            COMMAND_SUB_FLOAT_LVAR_FROM_FLOAT_VAR, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "MULT_THING_BY_THING",
        {COMMAND_MULT_INT_VAR_BY_VAL, COMMAND_MULT_FLOAT_VAR_BY_VAL, COMMAND_MULT_INT_LVAR_BY_VAL,
            COMMAND_MULT_FLOAT_LVAR_BY_VAL, COMMAND_MULT_INT_VAR_BY_INT_VAR, COMMAND_MULT_FLOAT_VAR_BY_FLOAT_VAR,
            COMMAND_MULT_INT_LVAR_BY_INT_LVAR, COMMAND_MULT_FLOAT_LVAR_BY_FLOAT_LVAR, COMMAND_MULT_INT_VAR_BY_INT_LVAR,
            COMMAND_MULT_FLOAT_VAR_BY_FLOAT_LVAR, COMMAND_MULT_INT_LVAR_BY_INT_VAR,
            COMMAND_MULT_FLOAT_LVAR_BY_FLOAT_VAR, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "DIV_THING_BY_THING",
        {COMMAND_DIV_INT_VAR_BY_VAL, COMMAND_DIV_FLOAT_VAR_BY_VAL, COMMAND_DIV_INT_LVAR_BY_VAL,
            COMMAND_DIV_FLOAT_LVAR_BY_VAL, COMMAND_DIV_INT_VAR_BY_INT_VAR, COMMAND_DIV_FLOAT_VAR_BY_FLOAT_VAR,
            COMMAND_DIV_INT_LVAR_BY_INT_LVAR, COMMAND_DIV_FLOAT_LVAR_BY_FLOAT_LVAR, COMMAND_DIV_INT_VAR_BY_INT_LVAR,
            COMMAND_DIV_FLOAT_VAR_BY_FLOAT_LVAR, COMMAND_DIV_INT_LVAR_BY_INT_VAR, COMMAND_DIV_FLOAT_LVAR_BY_FLOAT_VAR,
            -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "IS_THING_EQUAL_TO_THING",
        {COMMAND_IS_INT_VAR_EQUAL_TO_NUMBER, COMMAND_IS_INT_LVAR_EQUAL_TO_NUMBER, COMMAND_IS_INT_VAR_EQUAL_TO_INT_VAR,
            COMMAND_IS_INT_LVAR_EQUAL_TO_INT_LVAR, COMMAND_IS_INT_VAR_EQUAL_TO_INT_LVAR,
            COMMAND_IS_FLOAT_VAR_EQUAL_TO_NUMBER, COMMAND_IS_FLOAT_LVAR_EQUAL_TO_NUMBER,
            COMMAND_IS_FLOAT_VAR_EQUAL_TO_FLOAT_VAR, COMMAND_IS_FLOAT_LVAR_EQUAL_TO_FLOAT_LVAR,
            COMMAND_IS_FLOAT_VAR_EQUAL_TO_FLOAT_LVAR, COMMAND_IS_INT_VAR_EQUAL_TO_CONSTANT,
            COMMAND_IS_INT_LVAR_EQUAL_TO_CONSTANT, COMMAND_IS_VAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL,
            COMMAND_IS_LVAR_TEXT_LABEL_EQUAL_TO_TEXT_LABEL, COMMAND_IS_INT_LVAR_EQUAL_TO_INT_VAR,
            COMMAND_IS_FLOAT_LVAR_EQUAL_TO_FLOAT_VAR, COMMAND_IS_VAR_TEXT_LABEL16_EQUAL_TO_TEXT_LABEL,
            COMMAND_IS_LVAR_TEXT_LABEL16_EQUAL_TO_TEXT_LABEL, -1, -1},
    },
    {
        "IS_THING_NOT_EQUAL_TO_THING",
        {COMMAND_IS_INT_VAR_NOT_EQUAL_TO_NUMBER, COMMAND_IS_INT_LVAR_NOT_EQUAL_TO_NUMBER,
            COMMAND_IS_INT_VAR_NOT_EQUAL_TO_INT_VAR, COMMAND_IS_INT_LVAR_NOT_EQUAL_TO_INT_LVAR,
            COMMAND_IS_INT_VAR_NOT_EQUAL_TO_INT_LVAR, COMMAND_IS_FLOAT_VAR_NOT_EQUAL_TO_NUMBER,
            COMMAND_IS_FLOAT_LVAR_NOT_EQUAL_TO_NUMBER, COMMAND_IS_FLOAT_VAR_NOT_EQUAL_TO_FLOAT_VAR,
            COMMAND_IS_FLOAT_LVAR_NOT_EQUAL_TO_FLOAT_LVAR, COMMAND_IS_FLOAT_VAR_NOT_EQUAL_TO_FLOAT_LVAR,
            COMMAND_IS_INT_LVAR_NOT_EQUAL_TO_INT_VAR, COMMAND_IS_FLOAT_LVAR_NOT_EQUAL_TO_FLOAT_VAR, -1, -1, -1, -1, -1,
            -1, -1, -1},
    },
    {
        "IS_THING_GREATER_THAN_THING",
        {COMMAND_IS_INT_VAR_GREATER_THAN_NUMBER, COMMAND_IS_INT_LVAR_GREATER_THAN_NUMBER,
            COMMAND_IS_NUMBER_GREATER_THAN_INT_VAR, COMMAND_IS_NUMBER_GREATER_THAN_INT_LVAR,
            COMMAND_IS_INT_VAR_GREATER_THAN_INT_VAR, COMMAND_IS_INT_LVAR_GREATER_THAN_INT_LVAR,
            COMMAND_IS_INT_VAR_GREATER_THAN_INT_LVAR, COMMAND_IS_INT_LVAR_GREATER_THAN_INT_VAR,
            COMMAND_IS_FLOAT_VAR_GREATER_THAN_NUMBER, COMMAND_IS_FLOAT_LVAR_GREATER_THAN_NUMBER,
            COMMAND_IS_NUMBER_GREATER_THAN_FLOAT_VAR, COMMAND_IS_NUMBER_GREATER_THAN_FLOAT_LVAR,
            COMMAND_IS_FLOAT_VAR_GREATER_THAN_FLOAT_VAR, COMMAND_IS_FLOAT_LVAR_GREATER_THAN_FLOAT_LVAR,
            COMMAND_IS_FLOAT_VAR_GREATER_THAN_FLOAT_LVAR, COMMAND_IS_FLOAT_LVAR_GREATER_THAN_FLOAT_VAR,
            COMMAND_IS_INT_VAR_GREATER_THAN_CONSTANT, COMMAND_IS_INT_LVAR_GREATER_THAN_CONSTANT,
            COMMAND_IS_CONSTANT_GREATER_THAN_INT_VAR, COMMAND_IS_CONSTANT_GREATER_THAN_INT_LVAR},
    },
    {
        "IS_THING_GREATER_OR_EQUAL_TO_THING",
        {COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_NUMBER, COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_NUMBER,
            COMMAND_IS_NUMBER_GREATER_OR_EQUAL_TO_INT_VAR, COMMAND_IS_NUMBER_GREATER_OR_EQUAL_TO_INT_LVAR,
            COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_INT_VAR, COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_INT_LVAR,
            COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_INT_LVAR, COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_INT_VAR,
            COMMAND_IS_FLOAT_VAR_GREATER_OR_EQUAL_TO_NUMBER, COMMAND_IS_FLOAT_LVAR_GREATER_OR_EQUAL_TO_NUMBER,
            COMMAND_IS_NUMBER_GREATER_OR_EQUAL_TO_FLOAT_VAR, COMMAND_IS_NUMBER_GREATER_OR_EQUAL_TO_FLOAT_LVAR,
            COMMAND_IS_FLOAT_VAR_GREATER_OR_EQUAL_TO_FLOAT_VAR, COMMAND_IS_FLOAT_LVAR_GREATER_OR_EQUAL_TO_FLOAT_LVAR,
            COMMAND_IS_FLOAT_VAR_GREATER_OR_EQUAL_TO_FLOAT_LVAR, COMMAND_IS_FLOAT_LVAR_GREATER_OR_EQUAL_TO_FLOAT_VAR,
            COMMAND_IS_INT_VAR_GREATER_OR_EQUAL_TO_CONSTANT, COMMAND_IS_INT_LVAR_GREATER_OR_EQUAL_TO_CONSTANT,
            COMMAND_IS_CONSTANT_GREATER_OR_EQUAL_TO_INT_VAR, COMMAND_IS_CONSTANT_GREATER_OR_EQUAL_TO_INT_LVAR},
    },
    {
        "ADD_THING_TO_THING_TIMED",
        {COMMAND_ADD_TIMED_VAL_TO_FLOAT_VAR, COMMAND_ADD_TIMED_VAL_TO_FLOAT_LVAR,
            COMMAND_ADD_TIMED_FLOAT_VAR_TO_FLOAT_VAR, COMMAND_ADD_TIMED_FLOAT_LVAR_TO_FLOAT_LVAR,
            COMMAND_ADD_TIMED_FLOAT_VAR_TO_FLOAT_LVAR, COMMAND_ADD_TIMED_FLOAT_LVAR_TO_FLOAT_VAR, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "SUB_THING_FROM_THING_TIMED",
        {COMMAND_SUB_TIMED_VAL_FROM_FLOAT_VAR, COMMAND_SUB_TIMED_VAL_FROM_FLOAT_LVAR,
            COMMAND_SUB_TIMED_FLOAT_VAR_FROM_FLOAT_VAR, COMMAND_SUB_TIMED_FLOAT_LVAR_FROM_FLOAT_LVAR,
            COMMAND_SUB_TIMED_FLOAT_VAR_FROM_FLOAT_LVAR, COMMAND_SUB_TIMED_FLOAT_LVAR_FROM_FLOAT_VAR, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "ABS",
        {COMMAND_ABS_VAR_INT, COMMAND_ABS_LVAR_INT, COMMAND_ABS_VAR_FLOAT, COMMAND_ABS_LVAR_FLOAT, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "IS_STRING_EMPTY",
        {COMMAND_IS_VAR_TEXT_LABEL_EMPTY, COMMAND_IS_LVAR_TEXT_LABEL_EMPTY, COMMAND_IS_VAR_TEXT_LABEL16_EMPTY,
            COMMAND_IS_LVAR_TEXT_LABEL16_EMPTY, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "IS_BIT_SET",
        {COMMAND_IS_GLOBAL_VAR_BIT_SET_CONST, COMMAND_IS_GLOBAL_VAR_BIT_SET_VAR, COMMAND_IS_GLOBAL_VAR_BIT_SET_LVAR,
            COMMAND_IS_LOCAL_VAR_BIT_SET_CONST, COMMAND_IS_LOCAL_VAR_BIT_SET_VAR, COMMAND_IS_LOCAL_VAR_BIT_SET_LVAR, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "SET_BIT",
        {COMMAND_SET_GLOBAL_VAR_BIT_CONST, COMMAND_SET_GLOBAL_VAR_BIT_VAR, COMMAND_SET_GLOBAL_VAR_BIT_LVAR,
            COMMAND_SET_LOCAL_VAR_BIT_CONST, COMMAND_SET_LOCAL_VAR_BIT_VAR, COMMAND_SET_LOCAL_VAR_BIT_LVAR, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "CLEAR_BIT",
        {COMMAND_CLEAR_GLOBAL_VAR_BIT_CONST, COMMAND_CLEAR_GLOBAL_VAR_BIT_VAR, COMMAND_CLEAR_GLOBAL_VAR_BIT_LVAR,
            COMMAND_CLEAR_LOCAL_VAR_BIT_CONST, COMMAND_CLEAR_LOCAL_VAR_BIT_VAR, COMMAND_CLEAR_LOCAL_VAR_BIT_LVAR, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    },
    {
        "STRING_CAT",
        {COMMAND_STRING_CAT16, COMMAND_STRING_CAT8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1},
    },
};

// 0x0049B368
FILE* g_pScriptCompileInfoFile;
// 0x0049B36C
int g_nNumLabelRefs;
// 0x0049B370
int g_aLabelRefCodeOffset[MAX_LABEL_REFS];
// 0x004FCDF0
int g_aLabelRefLOC[MAX_LABEL_REFS];
// 0x0055E870
char g_aLabelRefName[MAX_LABEL_REFS + 1][NAME_SIZE];

// 0x00407440 ?WriteIntegerArgument@@YAXH@Z
void WriteIntegerArgument(int nValue)
{
    if (nValue >= INT8_MIN && nValue < INT8_MAX + 1)
    {
        WriteArgumentDataType(DATA_TYPE_INT8, true);
        int nOffset = g_nCodeOffset;
        g_pCompiledCode[nOffset] = nValue;
        g_nCodeOffset++;
        g_nStatInt8sWritten++;
    }
    else if (nValue >= INT16_MIN && nValue < INT16_MAX + 1)
    {
        WriteArgumentDataType(DATA_TYPE_INT16, true);
        int nOffset = g_nCodeOffset;
        int16_t nSwapped = nValue;
        if (Config_TargetMode == SAN_ANDREAS_XBOX360)
        {
            nSwapped = (nSwapped << 8) | (nSwapped >> 8);
        }
        *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nSwapped;
        g_nCodeOffset += CODE_SIZE_INT16;
        g_nStatInt16sWritten++;
    }
    else
    {
        WriteArgumentDataType(DATA_TYPE_INT32, true);
        WriteInt32(nValue);
    }
}

// 0x004074F0 ?WriteFloat@@YAXI@Z
void WriteFloat(float fValue)
{
    int nOffset = g_nCodeOffset;
    float fSwapped = fValue;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        *reinterpret_cast<uint32_t*>(&fSwapped) =
            static_cast<int>(*reinterpret_cast<uint32_t*>(&fSwapped) >> 24) |
            ((static_cast<int>(*reinterpret_cast<uint32_t*>(&fSwapped)) >> 8) & 0xFF00) |
            ((static_cast<int>(*reinterpret_cast<uint32_t*>(&fSwapped)) << 8) & 0xFF0000) |
            (static_cast<int>(*reinterpret_cast<uint32_t*>(&fSwapped)) << 24);
    }
    *reinterpret_cast<float*>(&g_pCompiledCode[nOffset]) = fSwapped;
    g_nCodeOffset += CODE_SIZE_FLOAT;
    g_nStatFloatsWritten++;
}

// 0x00407560 ?WriteInt16@@YAXF@Z
void WriteInt16(int16_t nValue)
{
    int nOffset = g_nCodeOffset;
    int16_t nSwapped = nValue;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nSwapped = (nValue << 8) | (nValue >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nSwapped;
    g_nCodeOffset += CODE_SIZE_INT16;
    g_nStatInt16sWritten++;
}

// 0x004075B0 ?WriteInt32@@YAXI@Z
void WriteInt32(uint32_t nValue)
{
    if (static_cast<int>(nValue) >= INT8_MIN && static_cast<int>(nValue) < INT8_MAX + 1)
    {
        g_nStatJumpsInt8++;
    }
    else if (static_cast<int>(nValue) >= INT16_MIN && static_cast<int>(nValue) < INT16_MAX + 1)
    {
        g_nStatJumpsInt16++;
    }
    else
    {
        g_nStatJumpsInt32++;
    }
    int nOffset = g_nCodeOffset;
    uint32_t nSwapped = nValue;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nSwapped = static_cast<int>(nValue >> 24) | ((static_cast<int>(nValue) >> 8) & 0xFF00) |
                   ((static_cast<int>(nValue) << 8) & 0xFF0000) | (static_cast<int>(nValue) << 24);
    }
    *reinterpret_cast<uint32_t*>(&g_pCompiledCode[nOffset]) = nSwapped;
    g_nCodeOffset += CODE_SIZE_INT32;
    g_nStatInt32sWritten++;
}

// 0x00407640 ?WriteArgumentDataType@@YAXED@Z
void WriteArgumentDataType(uint8_t nType, bool bCountStats)
{
    int nOffset = g_nCodeOffset;
    g_pCompiledCode[nOffset] = nType;
    g_nCodeOffset++;
    if (bCountStats)
    {
        switch (static_cast<int8_t>(nType))
        {
            case DATA_TYPE_END_OF_ARGS:
                g_nStatArgEndOfArgs++;
                break;
            case DATA_TYPE_INT32:
                g_nStatArgInt32++;
                break;
            case DATA_TYPE_GLOBAL_VAR:
                g_nStatArgGlobalVar++;
                break;
            case DATA_TYPE_LOCAL_VAR:
                g_nStatArgLocalVar++;
                break;
            case DATA_TYPE_INT8:
                g_nStatArgInt8++;
                break;
            case DATA_TYPE_INT16:
                g_nStatArgInt16++;
                break;
            case DATA_TYPE_FLOAT:
                g_nStatArgFloat++;
                break;
            case DATA_TYPE_GLOBAL_ARRAY:
                g_nStatArgGlobalArray++;
                break;
            case DATA_TYPE_LOCAL_ARRAY:
                g_nStatArgLocalArray++;
                break;
        }
    }
    g_nStatInt8sWritten++;
}

// 0x00407720 ?WriteKeywordFiles@@YAHD@Z
int WriteKeywordFiles(bool bMacFormat)
{
    char szMessage[128];
    char szPrefix[12];
    char szSuffix[12];
    FILE* pFile;

    if (bMacFormat)
    {
        strcpy(szPrefix, "<string>");
        strcpy(szSuffix, "</string>");
        pFile = fopen("c2.mac", "w");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open c2.mac file for writing - check it is writable");
            PrintMessage(szMessage);
            return -1;
        }
    }
    else
    {
        szPrefix[0] = '\0';
        szSuffix[0] = '\0';
        pFile = fopen("c2.kwd", "w");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open c2.kwd file for writing - check it is writable");
            PrintMessage(szMessage);
            return -1;
        }
    }

    fprintf(pFile, "%sMISSION_START%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sMISSION_END%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sIS_STRING_EMPTY%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sIS_BIT_SET%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sSET_BIT%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sCLEAR_BIT%s\n", szPrefix, szSuffix);
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aCommandInfo); i++)
    {
        if (g_aCommandInfo[i].bHandledByGame)
        {
            fprintf(pFile, "%s%s%s\n", szPrefix, g_aCommandInfo[i].szName, szSuffix);
        }
    }
    fclose(pFile);

    if (bMacFormat)
    {
        pFile = fopen("c0.mac", "w");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open c0.mac file for writing - check it is writable");
            PrintMessage(szMessage);
            return -1;
        }
    }
    else
    {
        pFile = fopen("c0.kwd", "w");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open c0.kwd file for writing - check it is writable");
            PrintMessage(szMessage);
            return -1;
        }
    }

    fprintf(pFile, "%sTRUE%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sFALSE%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sON%s\n", szPrefix, szSuffix);
    fprintf(pFile, "%sOFF%s\n", szPrefix, szSuffix);
    WriteKeywordListFromArray(szPrefix, g_aDefaultModels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCameraModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPads, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPadSticks, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPedTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszColours, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszBlipDisplayTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCameraSwitchModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFadeTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszShadowTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszWeaponTypes, szSuffix, pFile);
    WriteKeywordListFromArray(szPrefix, g_aSoundNames, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszWeatherTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszKillFrenzyStates, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarLockStates, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszExplosionTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPickupTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszGarageTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarBombTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarColours, szSuffix, pFile);
    WriteKeywordListFromArray(szPrefix, g_aObjectModels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszGangs, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCoronaTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFlareTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszRadarSprites, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszDrivingModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarMissions, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFormations, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszWaitStates, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCamZooms, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFonts, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszEntityStatus, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszLevels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCounterDisplayTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszMotionBlurTypes, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apHudFlashItems, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarDoors, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarPanels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszRadioStations, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFacingDirections, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszReplayCamModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarWheels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszTempActions, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszMoods, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszSetPieces, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszTimerDirections, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apBones, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszAnnouncementTracks, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPedMoveStates, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apStats, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszTaskStatus, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apDuckingTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFollowRouteModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszSplineModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarLightOverrides, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszDefaultTaskAllocators, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCheckpointTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszVehicleUpgrades, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszDriveByTypes, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apHoldOrientateTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPedBodyParts, szSuffix, pFile);
    if (Config_TargetMode == SAN_ANDREAS)
    {
        WriteKeywordList(szPrefix, g_apszKeyboardKeysPS2, szSuffix, pFile);
    }
    else if (Config_TargetMode == SAN_ANDREAS_PC || Config_TargetMode == SAN_ANDREAS_XBOX ||
             Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        WriteKeywordList(szPrefix, g_apszKeyboardKeysPC, szSuffix, pFile);
    }
    WriteKeywordList(szPrefix, g_apszRadarModes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszAcquaintanceTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszStatIncrements, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPopcycleZones, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarPlateDesigns, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszWinchTypes, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apDecisionMakerPedTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszDecisionMakerMissionPedTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszDecisionMakerGroupTypes, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apMeleeCombos, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszWeaponSkills, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apBuyableTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszSelectedWeaponTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszInteriorTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszPopRaces, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszBirdTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszConversationStates, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszFontOrientations, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCarClasses, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszKnockOffBikeTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszClothesTextures, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszCoordBlipAppearances, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszHudColours, szSuffix, pFile);
    if (Config_TargetMode == SAN_ANDREAS || Config_TargetMode == SAN_ANDREAS_XBOX)
    {
        WriteKeywordList(szPrefix, g_apszGlobalSpeechContexts, szSuffix, pFile);
    }
    else if (Config_TargetMode == SAN_ANDREAS_PC || Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        WriteKeywordList(szPrefix, g_apszGlobalSpeechContextsPC, szSuffix, pFile);
    }
    WriteKeywordList(szPrefix, g_apszCutsceneTrackStates, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszDoorDamageStates, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszSwirlTypes, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszSwimStates, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apEntryExitFlags, szSuffix, pFile);
    WriteKeywordListFromPairs(szPrefix, g_apZoomLevels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszProcLevels, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszLanguages, szSuffix, pFile);
    WriteKeywordList(szPrefix, g_apszHandSignalAnims, szSuffix, pFile);
    WriteKeywordList(szPrefix, (const char**)g_apszWidgetIds, szSuffix, pFile);
    WriteKeywordList(szPrefix, (const char**)g_apszHIDMappings, szSuffix, pFile);
    fclose(pFile);
    return 0;
}

// 0x00408120 ?WriteKeywordList@@YAXPBDPAPBD0PAU_iobuf@@@Z
void WriteKeywordList(const char* pszPrefix, const char** apszNames, const char* pszSuffix, FILE* pFile)
{
    for (uint16_t i = 0; strcmp(apszNames[i], "") != 0; i++)
    {
        fprintf(pFile, "%s%s%s\n", pszPrefix, apszNames[i], pszSuffix);
    }
}

// 0x004081C0 ?WriteKeywordListFromArray@@YAXPBD00PAU_iobuf@@@Z
void WriteKeywordListFromArray(const char* pszPrefix, const tNamedId* aNamedIds, const char* pszSuffix, FILE* pFile)
{
    for (uint16_t i = 0; strcmp(aNamedIds[i].szName, "") != 0; i++)
    {
        fprintf(pFile, "%s%s%s\n", pszPrefix, aNamedIds[i].szName, pszSuffix);
    }
}

// 0x00408260 ?WriteKeywordListFromPairs@@YAXPBDPAPBD0PAU_iobuf@@@Z
void WriteKeywordListFromPairs(const char* pszPrefix, const char** apPairs, const char* pszSuffix, FILE* pFile)
{
    for (uint16_t i = 0; strcmp(apPairs[i * NAME_VALUE_PAIR_SIZE], "") != 0; i++)
    {
        fprintf(pFile, "%s%s%s\n", pszPrefix, apPairs[i * NAME_VALUE_PAIR_SIZE], pszSuffix);
    }
}

// 0x00408300 ?WriteCompiledScript@@YAHXZ
int WriteCompiledScript()
{
    char szMessage[128];
    char szPath[1025];
    WIN32_FIND_DATAA findData;
    FILE* pFile;
    uint32_t nEndOffset;
    uint32_t i;

    int16_t nLen = strlen(Config_InputFileName);
    Config_InputFileName[nLen] = 'm';
    Config_InputFileName[nLen + 1] = '\0';
    pFile = fopen(Config_InputFileName, "wb");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open output file %s - check it is writable", Config_InputFileName);
        PrintMessage(szMessage);
        Config_InputFileName[nLen] = '\0';
        return -1;
    }
    g_pScriptCompileInfoFile = fopen("ScriptCompileInfo.txt", "wb");
    CalculateSubscriptSizes();
    WriteMissionScriptHeader();
    WriteStreamedScriptHeader();
    nEndOffset = g_nCodeOffset;
    if (g_nNumStreamedScripts > 0)
    {
        nEndOffset = g_aStreamedScripts[0].nStartOffset;
    }
    for (i = 0; i < nEndOffset; i++)
    {
        fputc(g_pCompiledCode[i], pFile);
    }
    fclose(g_pScriptCompileInfoFile);
    fclose(pFile);
    Config_InputFileName[nLen] = '\0';

    strcpy(szPath, Config_TargetPath);
    strcat(szPath, "\\streamed\\*.scm");
    HANDLE hFind = FindFirstFileA(szPath, &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            strcpy(szPath, Config_TargetPath);
            strcat(szPath, "\\streamed\\");
            strcat(szPath, findData.cFileName);
            DeleteFileA(szPath);
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }

    for (uint16_t nScript = 0; nScript < g_nNumStreamedScripts; nScript++)
    {
        strcpy(szPath, Config_TargetPath);
        strcat(szPath, "\\streamed\\");
        CreateDirectoryA(szPath, nullptr);
        strcat(szPath, g_aStreamedScripts[nScript].szName);
        strcat(szPath, "m");
        uint16_t nPathLen = strlen(szPath);
        for (uint16_t j = 0; j < nPathLen; j++)
        {
            if (szPath[j] >= 'A' && szPath[j] <= 'Z')
            {
                szPath[j] += 'a' - 'A';
            }
        }
        pFile = fopen(szPath, "wb");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open output file %s - check it is writable", szPath);
            PrintMessage(szMessage);
            return -1;
        }
        if (nScript == g_nNumStreamedScripts - 1)
        {
            nEndOffset = g_nCodeOffset;
        }
        else
        {
            nEndOffset = g_aStreamedScripts[nScript + 1].nStartOffset;
        }
        for (; i < nEndOffset; i++)
        {
            fputc(g_pCompiledCode[i], pFile);
        }
        fclose(pFile);
    }

    if (g_nNumStreamedScripts > 0)
    {
        strcpy(szPath, Config_TargetPath);
        strcat(szPath, "\\streamed\\");
        strcat(szPath, "aaa.scm");
        uint16_t nPathLen = strlen(szPath);
        for (uint16_t j = 0; j < nPathLen; j++)
        {
            if (szPath[j] >= 'A' && szPath[j] <= 'Z')
            {
                szPath[j] += 'a' - 'A';
            }
        }
        pFile = fopen(szPath, "wb");
        if (pFile == nullptr)
        {
            sprintf(szMessage, "**** Couldn't open output file streamed\\%s - check it is writable", szPath);
            PrintMessage(szMessage);
            return -1;
        }
        fwrite(&g_nGlobalVarSpaceSize, sizeof(g_nGlobalVarSpaceSize), 1, pFile);
        fwrite(&g_nCompileId, sizeof(g_nCompileId), 1, pFile);
        fclose(pFile);
    }
    return 0;
}

// Not in sc.exe as separate functions: both were inlined into CompileCommand (their only caller)
// and dropped by the linker. Their `return -1` exits are what share CompileCommand's error call.
static int HandleScriptParamArgument(int loc, uint16_t nCmd, int16_t nArg, int nScriptLabelLOC)
{
    if (g_aScopeOfLine[nScriptLabelLOC] == NO_SCOPE)
    {
        ReportCompilationError("**** NO SCOPE FOR DESTINATION LINE", nScriptLabelLOC);
        return -1;
    }
    switch (GetArgTypeAtLOC(loc, nCmd, nArg, true))
    {
        case CARG_INT:
            if (LocalVariables::GetType(g_aScopeOfLine[nScriptLabelLOC], nArg - 1) != VAR_TYPE_INT)
            {
                ReportCompilationError("**** ARGUMENT TYPE MISMATCH (INT)", loc);
                return -1;
            }
            {
                int nValue = GetArgIntValueAtLOC(loc, nCmd, nArg, true);
                fprintf(pMainText, "%d ", nValue);
                WriteIntegerArgument(nValue);
            }
            break;
        case CARG_FLOAT:
            if (LocalVariables::GetType(g_aScopeOfLine[nScriptLabelLOC], nArg - 1) != VAR_TYPE_FLOAT)
            {
                ReportCompilationError("**** ARGUMENT TYPE MISMATCH (FLOAT)", loc);
                return -1;
            }
            fprintf(pMainText, "%f ", GetArgFloatValueAtLOC(loc, nCmd, nArg, true));
            WriteArgumentDataType(DATA_TYPE_FLOAT, true);
            WriteFloat(GetArgFloatValueAtLOC(loc, nCmd, nArg, true));
            break;
        case CARG_NAME:
            return HandleArrayArgument(loc, nCmd, nArg, VAR_KIND_SCRIPT_PARAM, nScriptLabelLOC);
        default:
            ReportCompilationError("**** SOMETHING WENT WRONG HERE", loc);
            return -1;
    }
    return 0;
}

static int HandleNumberArgument(int loc, uint16_t nCmd, int16_t nArg)
{
    uint8_t nExpectedType = g_aCommandInfo[GetCommandAtLOC(loc, nCmd, true)].aArgTypes[nArg];
    switch (GetArgTypeAtLOC(loc, nCmd, nArg, true))
    {
        case CARG_INT:
            if (nExpectedType == ARG_FLOAT || nExpectedType == ARG_RETURN_FLOAT)
            {
                ReportCompilationError("**** ARGUMENT TYPE MISMATCH - EXPECTED A FLOAT", loc);
                return -1;
            }
            if (nExpectedType == ARG_RETURN_INT)
            {
                ReportCompilationError("**** CANNOT USE A NUMBER AS INPUT VARIABLE", loc);
                return -1;
            }
            {
                int nValue = GetArgIntValueAtLOC(loc, nCmd, nArg, true);
                fprintf(pMainText, "%d ", nValue);
                WriteIntegerArgument(nValue);
            }
            break;
        case CARG_FLOAT:
            if (nExpectedType == ARG_INT || nExpectedType == ARG_RETURN_INT)
            {
                ReportCompilationError("**** ARGUMENT TYPE MISMATCH - EXPECTED AN INT", loc);
                return -1;
            }
            if (nExpectedType == ARG_RETURN_FLOAT)
            {
                ReportCompilationError("**** CANNOT USE A NUMBER AS INPUT VARIABLE", loc);
                return -1;
            }
            fprintf(pMainText, "%f ", GetArgFloatValueAtLOC(loc, nCmd, nArg, true));
            WriteArgumentDataType(DATA_TYPE_FLOAT, true);
            WriteFloat(GetArgFloatValueAtLOC(loc, nCmd, nArg, true));
            break;
        case CARG_NAME:
            return HandleArrayArgument(loc, nCmd, nArg, VAR_KIND_ANY, 0);
        default:
            ReportCompilationError("**** SOMETHING WENT WRONG HERE", loc);
            return -1;
    }
    return 0;
}

// 0x00408870 ?CompileCommand@@YAHHG@Z
int CompileCommand(int loc, uint16_t nCmd)
{
    char szMessage[128];
    char szValue[12];
    int nScriptLabelLOC;

    eScriptCommands nCommand = GetCommandAtLOC(loc, nCmd, true);
    char* pszName = g_aCommandInfo[nCommand].szName;
    if (!g_aCommandInfo[nCommand].bHandledByGame)
    {
        sprintf(szMessage, "Error during compilation - %s not handled by game code", pszName);
        ReportCompilationError(szMessage, loc);
    }
    else
    {
        g_aCommandUsageCount[nCommand]++;
        if (GetNotFlagAtLOC(loc, nCmd, true))
        {
            fprintf(pMainText, "%d NOT %s ", g_nCodeOffset, pszName);
            nCommand = static_cast<eScriptCommands>(nCommand | COMMAND_NOT_FLAG);
        }
        else
        {
            fprintf(pMainText, "%d %s ", g_nCodeOffset, pszName);
        }
        g_nStatCommandsWritten++;
        int nOffset = g_nCodeOffset;
        int16_t nSwapped = nCommand;
        if (Config_TargetMode == SAN_ANDREAS_XBOX360)
        {
            nSwapped = (nCommand << 8) | (nCommand >> 8);
        }
        *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nSwapped;
        g_nCodeOffset += CODE_SIZE_COMMAND;
        g_nStatInt16sWritten++;

        if (nCommand == COMMAND_SET_COLLECTABLE1_TOTAL)
        {
            if (!g_bCollectable1TotalSet)
            {
                if (GetArgTypeAtLOC(loc, nCmd, 0, true) == CARG_INT && GetArgIntValueAtLOC(loc, nCmd, 0, true) == 0)
                {
                    SetArgIntValueAtLOC(loc, nCmd, 0, g_nTotalCollectables, false);
                    sprintf(szValue, "%d", g_nTotalCollectables);
                    SetArgStringAtLOC(loc, nCmd, 0, szValue, false);
                    g_bCollectable1TotalSet = true;
                }
                else
                {
                    ReportCompilationError("**** SET_COLLECTABLE1_TOTAL should be followed by a 0", loc);
                }
            }
            else
            {
                ReportCompilationError("**** Should only have one SET_COLLECTABLE1_TOTAL in the script", loc);
            }
        }
        else if (nCommand == COMMAND_SET_PROGRESS_TOTAL)
        {
            if (!g_bProgressTotalSet)
            {
                if (GetArgTypeAtLOC(loc, nCmd, 0, true) == CARG_INT && GetArgIntValueAtLOC(loc, nCmd, 0, true) == 0)
                {
                    SetArgIntValueAtLOC(loc, nCmd, 0, g_nTotalProgress, false);
                    sprintf(szValue, "%d", g_nTotalProgress);
                    SetArgStringAtLOC(loc, nCmd, 0, szValue, false);
                    g_bProgressTotalSet = true;
                }
                else
                {
                    ReportCompilationError("**** SET_PROGRESS_TOTAL should be followed by a 0", loc);
                }
            }
            else
            {
                ReportCompilationError("**** Should only have one SET_PROGRESS_TOTAL in the script", loc);
            }
        }
        else if (nCommand == COMMAND_SET_MISSION_RESPECT_TOTAL)
        {
            if (!g_bMissionRespectTotalSet)
            {
                if (GetArgTypeAtLOC(loc, nCmd, 0, true) == CARG_INT && GetArgIntValueAtLOC(loc, nCmd, 0, true) == 0)
                {
                    SetArgIntValueAtLOC(loc, nCmd, 0, g_nTotalMissionRespect, false);
                    sprintf(szValue, "%d", g_nTotalMissionRespect);
                    SetArgStringAtLOC(loc, nCmd, 0, szValue, false);
                    g_bMissionRespectTotalSet = true;
                }
                else
                {
                    ReportCompilationError("**** SET_RESPECT_TOTAL should be followed by a 0", loc);
                }
            }
            else
            {
                ReportCompilationError("**** Should only have one SET_RESPECT_TOTAL in the script", loc);
            }
        }
        else if (nCommand == COMMAND_SET_TOTAL_NUMBER_OF_MISSIONS)
        {
            if (!g_bTotalNumberOfMissionsSet)
            {
                if (GetArgTypeAtLOC(loc, nCmd, 0, true) == CARG_INT && GetArgIntValueAtLOC(loc, nCmd, 0, true) == 0)
                {
                    SetArgIntValueAtLOC(loc, nCmd, 0, g_nTotalMissionsPassed, false);
                    sprintf(szValue, "%d", g_nTotalMissionsPassed);
                    SetArgStringAtLOC(loc, nCmd, 0, szValue, false);
                    g_bTotalNumberOfMissionsSet = true;
                }
                else
                {
                    ReportCompilationError("**** SET_TOTAL_NUMBER_OF_MISSIONS should be followed by a 0", loc);
                }
            }
            else
            {
                ReportCompilationError("**** Should only have one SET_TOTAL_NUMBER_OF_MISSIONS in the script", loc);
            }
        }
        else if (nCommand == COMMAND_START_NEW_STREAMED_SCRIPT)
        {
            if (GetArgTypeAtLOC(loc, nCmd, 0, true) != CARG_INT)
            {
                ReportCompilationError(
                    "**** FIRST PARAMETER OF START_NEW_STREAMED_SCRIPT SHOULD BE AN INTEGER NOW", loc);
                return -1;
            }
            int nScript = GetArgIntValueAtLOC(loc, nCmd, 0, true);
            if (nScript >= 0 && nScript < g_nNumStreamedScripts)
            {
                nScriptLabelLOC = g_aStreamedScripts[nScript].nStartLOC;
                if (nScriptLabelLOC < 0)
                {
                    ReportCompilationError("****PROBLEM WITH FIRST LINE OF STREAMED SCRIPT", loc);
                    return -1;
                }
                while (GetCommandAtLOC(nScriptLabelLOC, 0, false) == COMMAND_NOP)
                {
                    nScriptLabelLOC++;
                }
                if (GetCommandAtLOC(nScriptLabelLOC, 0, true) != COMMAND_START_SCOPE)
                {
                    ReportCompilationError("**** EXPECTED { AFTER LABEL", nScriptLabelLOC);
                    return -1;
                }
                nScriptLabelLOC++;
            }
            else
            {
                ReportCompilationError("**** FIRST PARAMETER OF START_NEW_STREAMED_SCRIPT IS OUT OF RANGE", loc);
                return -1;
            }
        }

        int16_t nArg = 0;
        bool bWriteScriptParams = true;
        while (g_aCommandInfo[GetCommandAtLOC(loc, nCmd, true)].aArgTypes[nArg] != ARG_NONE && nArg < MAX_ARGS)
        {
            if (CheckArgumentEntityType(loc, nCmd, nArg) == -1 || UpdateVariableUsageRecord(loc, nCmd, nArg) == -1)
            {
                return -1;
            }
            switch (g_aCommandInfo[GetCommandAtLOC(loc, nCmd, true)].aArgTypes[nArg])
            {
                case ARG_INT_CONST:
                {
                    int nValue = GetArgIntValueAtLOC(loc, nCmd, nArg, true);
                    fprintf(pMainText, "%d ", nValue);
                    WriteIntegerArgument(nValue);
                    break;
                }
                case ARG_FLOAT_CONST:
                {
                    float fValue = GetArgFloatValueAtLOC(loc, nCmd, nArg, true);
                    fprintf(pMainText, "%f ", fValue);
                    WriteArgumentDataType(DATA_TYPE_FLOAT, true);
                    float fSwapped = fValue;
                    int nOffset = g_nCodeOffset;
                    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
                    {
                        *reinterpret_cast<int*>(&fSwapped) =
                            static_cast<uint32_t>(*reinterpret_cast<int*>(&fSwapped)) >> 24 |
                            ((*reinterpret_cast<int*>(&fSwapped) >> 8) & 0xFF00) |
                            ((*reinterpret_cast<int*>(&fSwapped) << 8) & 0xFF0000) |
                            (*reinterpret_cast<int*>(&fSwapped) << 24);
                    }
                    *reinterpret_cast<float*>(&g_pCompiledCode[nOffset]) = fSwapped;
                    g_nCodeOffset += CODE_SIZE_FLOAT;
                    g_nStatFloatsWritten++;
                    break;
                }
                case ARG_VAR_INT:
                case ARG_VAR_FLOAT:
                case ARG_VAR_TEXT_LABEL:
                case ARG_VAR_TEXT_LABEL16:
                    if (HandleArrayArgument(loc, nCmd, nArg, VAR_KIND_GLOBAL, 0) < 0)
                    {
                        return -1;
                    }
                    break;
                case ARG_LVAR_INT:
                case ARG_LVAR_FLOAT:
                case ARG_LVAR_TEXT_LABEL:
                case ARG_LVAR_TEXT_LABEL16:
                    if (HandleArrayArgument(loc, nCmd, nArg, VAR_KIND_LOCAL, 0) < 0)
                    {
                        return -1;
                    }
                    break;
                case ARG_LABEL:
                    if (g_nNumLabelRefs < ARRAY_SIZE(g_aLabelRefLOC))
                    {
                        g_aLabelRefLOC[g_nNumLabelRefs] = loc;
                        g_aLabelRefCodeOffset[g_nNumLabelRefs] = g_nCodeOffset;
                        strcpy(g_aLabelRefName[g_nNumLabelRefs], GetArgStringAtLOC(loc, nCmd, nArg, true));
                        g_nNumLabelRefs++;
                        char* pszLabel = GetArgStringAtLOC(loc, nCmd, nArg, true);
                        nScriptLabelLOC = GetValueFromMap(pszLabel, &g_mapLabels, false);
                        if (nScriptLabelLOC >= 0)
                        {
                            WriteArgumentDataType(DATA_TYPE_INT32, true);
                            WriteInt32(nScriptLabelLOC);
                            fprintf(pMainText, "%s ", GetArgStringAtLOC(loc, nCmd, nArg, true));
                            break;
                        }
                        ReportCompilationError("**** THIS LABEL DOESN'T EXIST!", loc);
                    }
                    else
                    {
                        ReportCompilationError("**** TOO MANY LABEL REFERENCES (hassle Obbe)", loc);
                    }
                    return -1;
                case ARG_SCRIPT_PARAM:
                    if (bWriteScriptParams)
                    {
                        if (GetArgTypeAtLOC(loc, nCmd, nArg, false) < 0)
                        {
                            WriteArgumentDataType(DATA_TYPE_END_OF_ARGS, true);
                            bWriteScriptParams = false;
                        }
                        else
                        {
                            if (HandleScriptParamArgument(loc, nCmd, nArg, nScriptLabelLOC) < 0)
                            {
                                return -1;
                            }
                        }
                    }
                    break;
                case ARG_INT:
                case ARG_FLOAT:
                case ARG_RETURN_INT:
                case ARG_RETURN_FLOAT:
                    if (HandleNumberArgument(loc, nCmd, nArg) < 0)
                    {
                        return -1;
                    }
                    break;
                case ARG_TEXT_LABEL:
                case ARG_TEXT_LABEL16:
                    if (HandleTextStringArgument(loc, nCmd, nArg) < 0)
                    {
                        return -1;
                    }
                    break;
                case ARG_STRING:
                    if (HandleStringArgument(loc, nCmd, nArg) < 0)
                    {
                        return -1;
                    }
                    break;
                default:
                    sprintf(szMessage, "**** ARGUMENT TYPE NOT IMPLEMENTED (kick Obbe) (%d) Com:%d Args:%d",
                        g_aCommandInfo[GetCommandAtLOC(loc, nCmd, true)].aArgTypes[nArg],
                        GetCommandAtLOC(loc, nCmd, true), nArg);
                    ReportCompilationError(szMessage, loc);
                    return -1;
            }
            nArg++;
        }
        fprintf(pMainText, "\n");
        return 0;
    }
    return -1;
}

// 0x00409370 ?WriteVariableIndex@@YAXF@Z
void WriteVariableIndex(int16_t nIndex)
{
    int nOffset = g_nCodeOffset;
    int16_t nSwapped = nIndex;
    if (Config_TargetMode == SAN_ANDREAS_XBOX360)
    {
        nSwapped = (nIndex << 8) | (nIndex >> 8);
    }
    *reinterpret_cast<int16_t*>(&g_pCompiledCode[nOffset]) = nSwapped;
    g_nCodeOffset += CODE_SIZE_INT16;
    g_nStatVarIndicesWritten++;
}

// 0x004093C0 ?WriteCommandUsageFile@@YAHXZ
int WriteCommandUsageFile()
{
    char szMessage[128];
    for (int i = 0; i < ARRAY_SIZE(g_aSortedCommandUsageCount); i++)
    {
        g_aSortedCommandUsageCount[i] = 0;
    }
    for (int i = 0; i < ARRAY_SIZE(g_aSortedCommandIds); i++)
    {
        g_aSortedCommandIds[i] = 0;
    }
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aCommandUsageCount); i++)
    {
        uint32_t nCount = g_aCommandUsageCount[i];
        bool bInserted = false;
        if (nCount > 0)
        {
            for (uint16_t j = 0; j < ARRAY_SIZE(g_aSortedCommandUsageCount); j++)
            {
                if (bInserted)
                {
                    break;
                }
                if (nCount > static_cast<uint32_t>(g_aSortedCommandUsageCount[j]))
                {
                    for (uint16_t k = ARRAY_SIZE(g_aSortedCommandIds) - 1; k > j; k--)
                    {
                        g_aSortedCommandIds[k] = g_aSortedCommandIds[k - 1];
                        g_aSortedCommandUsageCount[k] = g_aSortedCommandUsageCount[k - 1];
                    }
                    g_aSortedCommandIds[j] = i;
                    g_aSortedCommandUsageCount[j] = nCount;
                    bInserted = true;
                }
            }
        }
        else
        {
            for (uint16_t j = 0; j < ARRAY_SIZE(g_aSortedCommandUsageCount); j++)
            {
                if (bInserted)
                {
                    break;
                }
                if (g_aSortedCommandUsageCount[j] == 0)
                {
                    for (uint16_t k = ARRAY_SIZE(g_aSortedCommandIds) - 1; k > j; k--)
                    {
                        g_aSortedCommandIds[k] = g_aSortedCommandIds[k - 1];
                        g_aSortedCommandUsageCount[k] = g_aSortedCommandUsageCount[k - 1];
                    }
                    g_aSortedCommandIds[j] = i;
                    g_aSortedCommandUsageCount[j] = nCount;
                    bInserted = true;
                }
            }
        }
    }
    FILE* pFile = fopen("command_usage.txt", "w");
    if (pFile == nullptr)
    {
        sprintf(szMessage, "**** Couldn't open command usage file - check it is writable");
        PrintMessage(szMessage);
        return -1;
    }
    for (uint16_t i = 0; i < ARRAY_SIZE(g_aSortedCommandIds); i++)
    {
        fprintf(
            pFile, "%d\t%s\t\t\t%d\n", i, g_aCommandInfo[g_aSortedCommandIds[i]].szName, g_aSortedCommandUsageCount[i]);
    }
    fclose(pFile);
    return 0;
}
