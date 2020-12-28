#include "system/SystemInformation.hpp"

#ifdef _WIN32

#include <Windows.h>
#include <VersionHelpers.h>

namespace tau { namespace si {

SystemInformation& SystemInformation::GetSystemInformation() noexcept
{
    static SystemInformation instance;
    return instance;
}

void SystemInformation::init() noexcept
{
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);

        switch(systemInfo.wProcessorArchitecture)
        {
            case PROCESSOR_ARCHITECTURE_AMD64:   _processor = ProcessorType::AMD64; break;
            case PROCESSOR_ARCHITECTURE_ARM:     _processor = ProcessorType::ARM; break;
            case PROCESSOR_ARCHITECTURE_ARM64:   _processor = ProcessorType::ARM64; break;
            case PROCESSOR_ARCHITECTURE_IA64:    _processor = ProcessorType::IA64; break;
            case PROCESSOR_ARCHITECTURE_INTEL:   _processor = ProcessorType::x86; break;
            case PROCESSOR_ARCHITECTURE_UNKNOWN: 
            default:                             _processor = ProcessorType::Unknown; break;
        }

        _pageSize = systemInfo.dwPageSize;
    }

    {
        if(IsWindows10OrGreater())
        { _os = OSType::Windows10; }
        else if(IsWindows8Point1OrGreater())
        { _os = OSType::Windows8_1; }
        else if(IsWindows8OrGreater())
        { _os = OSType::Windows8; }
        else if(IsWindows7OrGreater())
        { _os = OSType::Windows7; }
        else if(IsWindowsVistaOrGreater())
        { _os = OSType::WindowsVista; }
        else if(IsWindowsXPOrGreater())
        { _os = OSType::WindowsXP; }
        else
        { _os = OSType::Windows; }
    }
}

} }

#endif
