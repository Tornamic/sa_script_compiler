# sa_script_compiler

Matching decompilation of `sc.exe`, the original GTA:SA mission script compiler. Partially matching for now: 188 functions in total, 169 matching, 31 partially matching. The logic is identical to the original compiler.

R* and Rockstar Games are trademarks of their respective owner. This project is unofficial and is not affiliated with or endorsed by Rockstar Games. It is provided for educational and research purposes only.

## Requirements for matching `sc.exe`

To get a matching binary, you will need:

* Visual Studio 2017 Build Tools with the **v141 x86/x64 C++ toolset** (`C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\`)
* Windows SDK **10.0.26100.0** (installed with VS 2026 under `D:\Windows Kits\10\`, override with `SA_WINSDK_VERSION`)
* xmake

Then run:

```bash
xmake build sc
```

If you don't want to go through the hassle of installing VS 2017 and WinSDK, you can just build it with your current compiler. The resulting binary won't match the original `sc.exe`, and the built `sc_decomp.exe` may not produce the identical .scm files because of the uninitialized char buffer for generating string types, but it will still work.

```bash
xmake build sc
```

## Usage

To build the original GTA:SA main.scm with the decomp, clone this repo https://github.com/Sergeanur/UndefinifiedGrove and follow the scripts building instruction with the original R* compiler, after you can replace the original sc.exe with the decompiled one.

## Toolchain (recovered from sc.exe)

| Item | Value |
|---|---|
| Compiler / linker | MSVC 19.16.27045 / link 14.16.27045 (VS 2017 15.9, v141), x86 (we use 19.16.27054, same 15.9 branch) |
| Objects | 17 C++ TUs (16 here; the 17th is probably an empty precompiled-header `stdafx.cpp`) |
| Configuration | Release, PDB `H:\Projects\3Master\Projects\Gameface\Plugins\GTABase\Tools\sa_script_compiler\Release\sa_script_compiler.pdb`, built 2021-09-03 |
| cl | `/O2 /Oi /Oy- /GS /Gy /EHsc /MT /arch:SSE2 /fp:precise /Zi`, no `/GL`, no `/sdl`, no `/guard:cf`, MBCS |
| link | `/SUBSYSTEM:WINDOWS /DEBUG /OPT:REF /OPT:ICF /DYNAMICBASE /NXCOMPAT /SAFESEH` |