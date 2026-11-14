#pragma once

// Resource IDs of sc.exe (extracted by tools/extract_resources.py)

#define IDR_MAINMENU 101        // menu, WNDCLASS::lpszMenuName
#define IDI_MAINICON 105        // icon, WNDCLASS::hIcon

#define IDC_OUTPUT_LISTBOX 200  // child list box that shows every compiler message

// Menu commands (WM_COMMAND, handled by HandleMenuCommand)
#define ID_FILE_LOAD 40001           // File > Load: pick the main .sc file
#define ID_FILE_EXIT 40002           // File > Exit
#define ID_RUN_PARSER 40003          // not in the menu any more, still handled: compiles like ID_RUN_MULTIFILE
#define ID_RUN_MULTIFILE 40005       // Run > Run MultiFile Parser (also used for -f on the command line)
#define ID_TARGET_SAN_ANDREAS 40008  // Target > San Andreas (PS2)
#define ID_TARGET_SA_PC 40009        // Target > San Andreas PC
#define ID_TARGET_SA_XBOX360 40010   // Target > San Andreas Xbox 360
#define ID_TARGET_SA_XBOX 40011      // Target > San Andreas Xbox
