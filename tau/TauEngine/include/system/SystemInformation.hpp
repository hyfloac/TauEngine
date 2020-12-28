#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>

namespace tau { namespace si {

enum class OSType
{
    Unknown = 0,
    Windows,
    WindowsXP,
    WindowsVista,
    Windows7,
    Windows8,
    Windows8_1,
    Windows10,
    MacOS,
    OSX10_8,
    OSX10_9,
    OSX10_10,
    OSX10_11,
    MacOS10_12,
    MacOS10_13,
    MacOS10_14,
    MacOS10_15,
    MacOS11,
    Unix,
    Linux,
    RHELBased,
    DebianBased,
    GentooBased,
    Gentoo,
    Debian,
    Ubuntu,
    Fedora,
    ArchLinux,
    RedHatLinux,
    CentOS,
    OpenSUSE,
    AmazonLinux2,
    OracleLinux,
    ChromeOS,
    ChromiumOS
};

enum class ProcessorType
{
    Unknown = 0,
    x86,
    IA64,
    AMD64,
    ARM,
    ARM64
};

[[nodiscard]] inline bool isWindows(const OSType os) noexcept
{
    switch(os)
    {
        case OSType::Windows:
        case OSType::WindowsXP:
        case OSType::WindowsVista:
        case OSType::Windows7:
        case OSType::Windows8:
        case OSType::Windows8_1:
        case OSType::Windows10: return true;
        default:                return false;
    }
}

[[nodiscard]] inline bool isMacOS(const OSType os) noexcept
{
    switch(os)
    {
        case OSType::MacOS:
        case OSType::OSX10_8:
        case OSType::OSX10_9:
        case OSType::OSX10_10:
        case OSType::OSX10_11:
        case OSType::MacOS10_12:
        case OSType::MacOS10_13:
        case OSType::MacOS10_14:
        case OSType::MacOS10_15:
        case OSType::MacOS11: return true;
        default:              return false;
    }
}

[[nodiscard]] inline bool isUnix(const OSType os) noexcept
{
    switch(os)
    {
        case OSType::MacOS:
        case OSType::OSX10_8:
        case OSType::OSX10_9:
        case OSType::OSX10_10:
        case OSType::OSX10_11:
        case OSType::MacOS10_12:
        case OSType::MacOS10_13:
        case OSType::MacOS10_14:
        case OSType::MacOS10_15:
        case OSType::MacOS11:
        case OSType::Unix:
        case OSType::Linux:
        case OSType::RHELBased:
        case OSType::DebianBased:
        case OSType::GentooBased:
        case OSType::Gentoo:
        case OSType::Debian:
        case OSType::Ubuntu:
        case OSType::Fedora:
        case OSType::ArchLinux:
        case OSType::RedHatLinux:
        case OSType::CentOS:
        case OSType::OpenSUSE:
        case OSType::AmazonLinux2:
        case OSType::OracleLinux:
        case OSType::ChromeOS:
        case OSType::ChromiumOS: return true;
        default:                 return false;
    }
}

[[nodiscard]] inline bool isLinux(const OSType os) noexcept
{
    switch(os)
    {
        case OSType::Linux:
        case OSType::RHELBased:
        case OSType::DebianBased:
        case OSType::GentooBased:
        case OSType::Gentoo:
        case OSType::Debian:
        case OSType::Ubuntu:
        case OSType::Fedora:
        case OSType::ArchLinux:
        case OSType::RedHatLinux:
        case OSType::CentOS:
        case OSType::OpenSUSE:
        case OSType::AmazonLinux2:
        case OSType::OracleLinux:
        case OSType::ChromeOS:
        case OSType::ChromiumOS: return true;
        default:                 return false;
    }
}

[[nodiscard]] inline bool isIntel(const ProcessorType processor) noexcept
{
    switch(processor)
    {
        case ProcessorType::x86:
        case ProcessorType::IA64:
        case ProcessorType::AMD64: return true;
        default:                   return false;
    }
}

[[nodiscard]] inline bool isARM(const ProcessorType processor) noexcept
{
    switch(processor)
    {
        case ProcessorType::ARM:
        case ProcessorType::ARM64: return true;
        default:                   return false;
    }
}

[[nodiscard]] inline bool is32Bit(const ProcessorType processor) noexcept
{
    switch(processor)
    {
        case ProcessorType::x86:
        case ProcessorType::ARM: return true;
        default:                 return false;
    }
}

[[nodiscard]] inline bool is64Bit(const ProcessorType processor) noexcept
{
    switch(processor)
    {
        case ProcessorType::IA64:
        case ProcessorType::AMD64: 
        case ProcessorType::ARM64: return true;
        default:                   return false;
    }
}

class SystemInformation final
{
    DEFAULT_DESTRUCT(SystemInformation);
    DEFAULT_CM_PU(SystemInformation);
public:
    static SystemInformation& GetSystemInformation() noexcept;
private:
    OSType _os;
    ProcessorType _processor;
    uSys _pageSize;
private:
    SystemInformation() noexcept
        : _os(OSType::Unknown)
        , _processor(ProcessorType::Unknown)
        , _pageSize(0)
    { init(); }
public:
    [[nodiscard]] OSType os() const noexcept { return _os; }
    [[nodiscard]] ProcessorType processor() const noexcept { return _processor; }
    [[nodiscard]] uSys pageSize() const noexcept { return _pageSize; }
private:
    void init() noexcept;
};

} }
