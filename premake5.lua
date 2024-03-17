#!lua

workspace "TauEngine"
    configurations {
        "Debug",
        "Release",
        "Production",
        "OptimizedDebug"
    }
    platforms { "Win64" }

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"
        _arch = "x64"

    filter "configurations:Production"  
        defines { "TAU_PRODUCTION" }

    filter {}

    targetdir "%{wks.location}/bin/%{cfg.longname}-%{_arch}"

    project "TauEngine"
        kind "StaticLib"
        language "C++"
        location "tau/TauEngine"
        files { "**.h", "**.hpp", "**.c", "**.cpp" }
        defines {
            "GLEW_NO_GLU",
            "FMT_HEADER_ONLY",
            "SPD_LOG_FMT_EXTERNAL",
            "_CRT_SECURE_NO_WARNINGS",
            "_HAS_EXCEPTIONS=0"
        }
        includedirs {
            "%{prj.location}/include",
            "%{wks.location}/tau/TauUtils/include",
            "%{wks.location}/libs/FreeImage/Dist/x64",
            "%{wks.location}/libs/SPDLog/include",
            "%{wks.location}/libs/GLEW/include",
            "%{wks.location}/libs/freetype-2.10.0/include",
            "%{wks.location}/libs/fmt/include",
            "%{wks.location}/utils/ResourceLib/include",
            "%{wks.location}/libs/glm",
            "%{wks.location}/libs/OpenVR/headers"
        }

    project "TauEditor"
        kind "ConsoleApp"
        language "C++"
        location "tau/TauEditor"
        files { "**.h", "**.hpp", "**.c", "**.cpp" }
        defines {
            "GLEW_NO_GLU",
            "FMT_HEADER_ONLY",
            "SPD_LOG_FMT_EXTERNAL",
            "_CRT_SECURE_NO_WARNINGS",
            "_HAS_EXCEPTIONS=0",
            "GLM_FORCE_DEPTH_ZERO_TO_ONE",
            "GLM_FORCE_LEFT_HANDED"
        }
        includedirs {
            "%{prj.location}/include",
            "%{wks.location}/tau/TauEngine/include",
            "%{wks.location}/tau/TauUtils/include",
            "%{wks.location}/libs/GLEW/include",
            "%{wks.location}/libs/SPDLog/include",
            "%{wks.location}/libs/freetype-2.10.0/include",
            "%{wks.location}/libs/fmt/include",
            "%{wks.location}/utils/ResourceLib/include",
            "%{wks.location}/libs/glm",
            "%{wks.location}/libs/OpenVR/headers"
        }
        libdirs {
            "%{cfg.outdir}",
            "%{wks.location}/libs/OpenVR/lib/win64",
            "%{wks.location}/libs/FreeImage/Dist/%{_arch}"
        }
        links {
            "TauEngine.lib",
            "opengl32.lib",
            "GLEW.lib",
            "d3d9.lib",
            "d3d10.lib",
            "d3d11.lib",
            "d3dcompiler.lib",
            "dxgi.lib",
            "dxguid.lib",
            "TauUtils.lib",
            "ResourceLib.lib",
            "freetype.lib",
            "openvr_api.lib",
            "FreeImage.lib"
        }



