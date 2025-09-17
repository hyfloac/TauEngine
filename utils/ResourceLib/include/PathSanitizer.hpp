#pragma once

#include <String.hpp>

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
public:
    static void setSettings(const PathSanitizerSettings& settings) noexcept;

    [[nodiscard]] static const PathSanitizerSettings& settings() noexcept
    { return s_Settings; }

    static C8DynString sanitizePath(const C8DynStringView& path) noexcept;
    static C8DynString sanitizeSubPath(const C8DynStringView& path) noexcept;
private:
    static PathSanitizerSettings s_Settings;
};
