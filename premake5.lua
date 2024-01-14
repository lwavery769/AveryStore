workspace "StoreGame"
	architecture "x64"
	startproject "StoreGame"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Engine/vendor/imgui"
project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Engine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%(prj.name)/vendor/spdlog/**.h"
		--"%{prj.name}/vendor/glm/glm/**.hpp",
		--"%{prj.name}/vendor/glm/glm/**.inl"
	}
	defines
	{
		"AL_PLATFORM_WINDOWS",
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/imgui"
		
	}
	libdirs{"%(ProjectDir)vendor/glfw"}
	links 
	{ 
		"glfw3_mt.lib",
		--"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		--cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		--postbuildcommands{("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "on"

project "StoreGame"
	location "StoreGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/glad.c"
	}

	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src",
		"Engine/vendor",
		"Engine/vendor/imgui"
		--"%{IncludeDir.glm}"
	}
	libdirs{"Engine/vendor/glfw"}
	links 
	{ 
		"glfw3_mt.lib",
		"ImGui",
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "on"
		systemversion "latest"

		defines
		{
			"AL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "on"
		staticruntime "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "on"
		staticruntime "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "on"