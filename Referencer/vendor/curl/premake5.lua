project "LibCURL"
    kind "StaticLib"
    language "C"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs { "./include" }
    libdirs { "./lib" } 

    files
    {
        "include/*.h",
    }


    links { "curl" }

    filter "system:linux"
        pic "On"
        systemversion "latest"
        staticruntime "On"
        defines
        {
            "_GLM_X11"
        }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        defines
        {
            "_WINDOWS",
            "WIN32"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
