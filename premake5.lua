workspace "Referencer"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "Referencer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory) // keby si zabudol :D
IncludeDir = {}
IncludeDir["GLFW"] = "Referencer/vendor/GLFW/include"
IncludeDir["Glad"] = "Referencer/vendor/Glad/include"
IncludeDir["ImGui"] = "Referencer/vendor/imGui"
--IncludeDir["libCURL"] = "Referencer/vendor/curl"
--IncludeDir["assimp"] = "Referencer/vendor/assimp/include/"
--IncludeDir["assimp"] = "Referencer/vendor/assimp_build/include"
IncludeDir["GLM"] = "Referencer/vendor/glm/"

include "Referencer/vendor/GLFW"
include "Referencer/vendor/Glad"
include "Referencer/vendor/imGui"
--include "Referencer/vendor/curl"
--include "Referencer/vendor/assimp_build" 
include "Referencer/vendor/glm"

project "Referencer"
	location "Referencer"
	kind "WindowedApp"
	language "C++"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rfpch.h"
	pchsource "Referencer/src/rfpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"Referencer/vendor/assimp/include", --%{prj.name}
		"%{IncludeDir.GLM}",
		"Referencer/vendor/toml/toml11-master", -- %{prj.name}
		"Referencer/vendor/STB", -- %{prj.name}
		"D:/dev/Referencer/Referencer/vendor/curl/debug/include", -- %{prj.name}
	}
	--"Referencer/vendor/assimp/lib/Release"
	libdirs { "Referencer/vendor/assimp/lib/Debug", "D:/dev/Referencer/Referencer/vendor/curl/debug/lib"}
	
	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"GLM",
		-- add to respective configurations
		"assimp-vc143-mtd.lib",
		"zlibstaticd.lib",
		--"assimp-vc143-mt.lib",
		--"zlibstatic.lib",
		"libcurl_a_debug.lib",

		-- needed for curl windows -> add to windows 
		"Ws2_32.lib",
		"Wldap32.lib",
		"Normaliz.lib",
		"Crypt32.lib",
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		icon "icon.ico"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "RF_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RF_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "RF_DIST"
		runtime "Release"
		optimize "On"
