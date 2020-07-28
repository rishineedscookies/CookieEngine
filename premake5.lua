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

project "CookieEngine"
    location "CookieEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ckpch.h"
    pchsource "CookieEngine/src/ckpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "CK_PLATFORM_WINDOWS",
            "CK_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "CK_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CK_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CK_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

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
        "CookieEngine/src"
    }

    links
    {
        "CookieEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "CK_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CK_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CK_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CK_DIST"
        optimize "On"