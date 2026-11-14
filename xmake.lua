set_project("sa_script_compiler_decomp")

set_plat("windows")
set_arch("x86")
set_defaultmode("release")

-- Select the originally used compiler. 
-- Comment these lines if don't need the matching build.
set_config("vs", "2017")
set_config("vs_toolset", "14.16")
set_config("vs_sdkver", "10.0.26100.0")
--

set_toolchains("msvc")

local cxflags = {
    "/O2", "/Oi", "/Oy-", "/GS", "/Gy", "/EHsc", "/arch:SSE2", "/fp:precise", "/Zi", "/W3", "/FS", "/Fdbuild\\sa_script_compiler.pdb",
}

target("sc")
    set_kind("binary")
    set_basename("sc_decomp")
    add_files("res/sc.rc")
    add_includedirs("src")
    add_files("src/*.cpp")
    set_languages("cxx14")
    set_runtimes("MT")
    add_defines("SERGEANUR_PATCH") -- https://github.com/Sergeanur/UndefinifiedGrove/tree/master/patch
    add_cxflags("/O2", "/Oi", "/Oy-", "/GS", "/Gy", "/EHsc", "/arch:SSE2", "/fp:precise", "/Zi", "/W3", "/FS",
                {force = true})
    add_defines("WIN32", "NDEBUG", "_WINDOWS", "_MBCS", "_CRT_SECURE_NO_WARNINGS")
    add_ldflags("/SUBSYSTEM:WINDOWS", "/DEBUG", "/OPT:REF", "/OPT:ICF", "/DYNAMICBASE:NO", "/FIXED", "/NXCOMPAT",
                "/SAFESEH", {force = true})
    add_syslinks("user32", "gdi32", "comdlg32", "advapi32", "kernel32")
