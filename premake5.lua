workspace "CookieEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "CookieEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "CookieEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "CookieEngine/vendor/imgui"
IncludeDir["MathFu"] = "CookieEngine/vendor/MathFu/include"
IncludeDir["Vectorial"] = "CookieEngine/vendor/vectorial/include"
IncludeDir["stb_image"] = "CookieEngine/vendor/stb_image"
IncludeDir["assimp"] = "CookieEngine/vendor/assimp-master/include"

include "CookieEngine/vendor/GLFW"
include "CookieEngine/vendor/Glad"
include "CookieEngine/vendor/imgui"
include "CookieEngine/vendor/assimp-master"

project "CookieEngine"
    location "CookieEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ckpch.h"
    pchsource "CookieEngine/src/ckpch.cpp"

    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/MathFu/include/**.h",
        "%{prj.name}/vendor/vectorial/include/**.h"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.MathFu}",
        "%{IncludeDir.Vectorial}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.assimp}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "assimp"
    }

    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "CK_PLATFORM_WINDOWS",
            "CK_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "CK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CK_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "CK_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "CookieEngine/vendor/spdlog/include",
        "CookieEngine/src",
        "CookieEngine/vendor",
        "%{IncludeDir.MathFu}",
        "%{IncludeDir.Vectorial}"
    }

    links
    {
        "CookieEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "CK_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CK_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "CK_DIST"
        runtime "Release"
        optimize "on"