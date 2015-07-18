function Torque6Editor()
    project "Torque6Editor"
        targetname "Torque6Editor"
        language "C++"
        kind "WindowedApp"
        debugdir (EDITOR_DIR)

        includedirs {
            SRC_DIR,
            path.join(LIB_DIR, "assimp/include"),
            path.join(LIB_DIR, "bgfx/include"),
            path.join(LIB_DIR, "bgfx/common"),
            path.join(LIB_DIR, "bgfx/common/imgui"),
            path.join(LIB_DIR, "bgfx/common/nanovg"),
            path.join(LIB_DIR, "openal/win32"),
        }

        files {
            path.join(EDITOR_DIR, "src/**.cpp"),
            path.join(EDITOR_DIR, "src/**.h"),
        }

        links {
            "Torque6"
        }

        defines {
            "TORQUE_PLUGIN"
        }

        configuration "Debug"
            targetname "Torque6Editor_DEBUG"
            defines     { "TORQUE_DEBUG", "TORQUE_ENABLE_PROFILER" }
            flags       { "Symbols" }

        configuration "Release"
            defines     {  }

        configuration "vs*"
            defines     { "_CRT_SECURE_NO_WARNINGS" }

        configuration "windows"
            flags { "WinMain" }
            targetdir   "../bin/windows"
            links { "ole32",
                    "wxmsw30ud_adv",
                    "wxmsw30ud_aui",
                    "wxmsw30ud_core",
                    "wxmsw30ud_propgrid",
                    "wxmsw30ud_stc",
                    "wxbase30ud",
                    "wxbase30ud_net",
                    "wxscintillad",
                    "wxtiffd",
                    "wxjpegd",
                    "wxpngd",
                    "wxzlibd",
                    "wxregexud",
                    "wxexpatd",
                    "winmm",
                    "comctl32",
                    "rpcrt4",
                    "wsock32",
                    "wininet", 
                  }
            includedirs { "$(wxwin)/lib/vc_lib/mswud",
                          "$(wxwin)/include",
                          "$(wxwin)/src/stc/scintilla/include",
                          "$(wxwin)/src/stc/scintilla/lexlib",
                          "$(wxwin)/src/stc/scintilla/src",
                        }
            libdirs { "$(wxwin)/lib/vc_lib" }

        configuration "linux"
            targetdir   "../bin/linux"
            links       { "dl" }
            linkoptions { "-rdynamic" }

        configuration "bsd"
            targetdir   "../bin/bsd"

        configuration "linux or bsd"
            defines     {  }
            links       { "m" }
            linkoptions { "-rdynamic" }

        configuration "macosx"
            targetdir   "../bin/darwin"
            defines     {  }
            links       { "CoreServices.framework" }

        configuration { "macosx", "gmake" }
            buildoptions { "-mmacosx-version-min=10.4" }
            linkoptions  { "-mmacosx-version-min=10.4" }
end
