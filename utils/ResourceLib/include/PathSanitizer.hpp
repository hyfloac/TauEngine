#pragma once

#include "String.hpp"

struct PathSanitizerSettings final
{
    bool blockUNCPath;
    bool blockLocalDevicePath;
    bool blockLocalDeviceDriveAbsolutePath;
    bool blockRootLocalDevicePath;
    bool blockRootLocalDeviceDriveAbsolutePath;
    bool blockRootLocalDeviceDriveRelativePath;
    bool blockDriveAbsolutePath;
    bool blockDriveRelativePath;
    bool blockRootedPath;
    bool blockDevices;
    bool blockNonLetterDrive;
    bool lockSettings;
};

class PathSanitizer final
{
private:
    static PathSanitizerSettings _settings;
public:
    static void setSettings(const PathSanitizerSettings& settings) noexcept;

    [[nodiscard]] static const PathSanitizerSettings& settings() noexcept
    { return _settings; }

    static WDynString sanitizePath(const WDynString& path) noexcept;
    static WDynString sanitizeSubPath(const WDynString& path) noexcept;
};
