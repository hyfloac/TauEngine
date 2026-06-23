/**
 * @file
 */
#include "PathSanitizer.hpp"

#ifndef _WIN32
#include <cstdlib>
#include <limits.h>
#include <cstdio>
#endif

PathSanitizerSettings PathSanitizer::s_Settings;

static bool IsValidCharset(const C8DynStringView& path) noexcept;
static C8DynString FixPathSeparator(const C8DynStringView& path, const bool targetWindows) noexcept;
static u32 ValidatePathPrefix(const C8DynStringView& path) noexcept;
static bool ContainsWin32Device(const C8DynStringView& path) noexcept;
static C8DynString CleanDotDirs(const C8DynStringView& pathView) noexcept;

void PathSanitizer::setSettings(const PathSanitizerSettings& settings) noexcept
{
    if(!s_Settings.lockSettings)
    {
        s_Settings = settings;
    }
}

C8DynString PathSanitizer::sanitizePath(const C8DynStringView& path) noexcept
{
    if(!IsValidCharset(path))
    {
        return { };
    }

    // We want to target windows initially for the devices.
    C8DynString newPath = FixPathSeparator(path, true);

    const u32 validateStatus = ValidatePathPrefix(newPath);

    if(validateStatus == 0)
    {
        return { };
    }
    else if(validateStatus == 1)
    {
        if(newPath.Length() < 3)
        {
            return { };
        }

        char8_t* bypassFixPath = new char8_t[newPath.Length() + 1];
        (void) ::std::memcpy(bypassFixPath, newPath, newPath.Length() * sizeof(char8_t));
        bypassFixPath[newPath.Length()] = '\0';

        if(bypassFixPath[0] == u8'\xEF')
        {
            if(newPath.Length() < 5 || bypassFixPath[1] != u8'\xBB' || bypassFixPath[2] != u8'\xBF')
            {
                delete[] bypassFixPath;
                return { };
            }

            bypassFixPath[4] = u8'\\';
        }
        else
        {
            bypassFixPath[1] = u8'\\';
        }

        newPath = C8DynString::PassControl(bypassFixPath, [](c8* const ptr) { delete[] ptr; });
    }

    newPath = CleanDotDirs(newPath);

#if defined(_WIN32)
    const WDynString widePath = StringCast<wchar_t>(newPath);

    const uSys fullPathLen = GetFullPathNameW(widePath.String(), 0, nullptr, nullptr);

    wchar_t* const fullPathWideRaw = new(::std::nothrow) wchar_t[fullPathLen + 1];
    fullPathWideRaw[fullPathLen] = u8'\0';
    if(GetFullPathNameW(widePath.String(), static_cast<DWORD>(fullPathLen), fullPathWideRaw, nullptr) != fullPathLen)
    {
        delete[] fullPathWideRaw;
        return { };
    }

    WDynString fullPathWide = WDynString::PassControl(fullPathWideRaw, [](wchar_t* const ptr) { delete[] ptr; });
    C8DynString fullPath = StringCast<char8_t>(fullPathWide);

    if(s_Settings.blockDevices && ContainsWin32Device(fullPath))
    {
        return { };
    }

    return fullPath;
#else
    char fullPath[PATH_MAX + 1];
    realpath(reinterpret_cast<const char*>(newPath.String()), fullPath);

    if(s_Settings.blockDevices && ContainsWin32Device(newPath))
    {
        return { };
    }

    return FixPathSeparator(newPath, false);
#endif
}

C8DynString PathSanitizer::sanitizeSubPath(const C8DynStringView& path) noexcept
{
    if(!IsValidCharset(path))
    {
        return { };
    }

#ifdef _WIN32
    return FixPathSeparator(path, true);
#else
    return FixPathSeparator(path, false);
#endif
}

static bool IsValidCharset(const C8DynStringView& path) noexcept
{
    uSys colonCount = 0;

    const auto begin = path.begin();
    const auto end = path.end();
    for(auto iter = begin; iter != end; ++iter)
    {
        const c32 c = *iter;

        if(c <= 31)
        { continue; }

        //   The first characters are banned on Windows, then we have the set
        // of direction reversal characters, which may or may not be allowed,
        // but are notorious for creating unsafe paths. Then we have a bunch of
        // various Unicode slashes, that can be used to trick people.
        switch(c)
        {
            case U'<':
            case U'>':
            case U'\"':
            case U'|':
            case U'?':
            case U'*':
            case U'\u200E': // Left-to-Right Mark (LRM)
            case U'\u200F': // Right-to-Left Mark (RLM)
            case U'\u202A': // Left-to-Right Embedding (LRE)
            case U'\u202B': // Right-to-Left Embedding (RLE)
            case U'\u202C': // Pop Directional Formatting (PDF)
            case U'\u202D': // Left-to-Right Override (LRO)
            case U'\u202E': // Right-to-Left Override (RLO)
            case U'\u2066': // Left-to-Right Isolate (LRI)
            case U'\u2067': // Right-to-Left Isolate (RLI)
            case U'\u2068': // First Strong Isolate (FSI)
            case U'\u2069': // Pop Directional Isolate (PDI)
            case U'\u1735': // Philippine Single Punctuation
            case U'\u1736': // Philippine Double Punctuation
            case U'\u2044': // Fraction Slash
            case U'\u2215': // Division Slash
            case U'\u2216': // Set Minus
            case U'\u2571': // Box Drawings Light Diagonal Upper Right to Lower Left
            case U'\u2572': // Box Drawings Light Diagonal Upper Left to Lower Right
            case U'\u27CB': // Mathematical Rising Diagonal
            case U'\u27CD': // Mathematical Falling Diagonal
            case U'\u29F5': // Reverse Solidus Operator
            case U'\u29F8': // Big Solidus
            case U'\u29F9': // Big Reverse Solidus
            case U'\U0001D10D': // Musical Symbol Repeated Figure-1
                return false;
            default: break;
        }

        if(c == U':')
        {
            ++colonCount;
            if(colonCount > 1)
            {
                return false;
            }
        }

        if(c == U'.') // Ensure file doesn't end with a '.'
        {
            auto tmpIter = iter;
            if(++tmpIter == end || *tmpIter == U'\\' || *tmpIter == U'/')
            {
                if(iter != begin)
                {
                    tmpIter = iter;
                    --tmpIter;
                    if(*tmpIter != u8'\\' && *tmpIter != u8'/')
                    {
                        if(*tmpIter == u8'.')
                        {
                            if(tmpIter != begin)
                            {
                                --tmpIter;
                                if(*tmpIter != u8'\\' && *tmpIter != u8'/')
                                {
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

static C8DynString FixPathSeparator(const C8DynStringView& path, const bool targetWindows) noexcept
{
    char8_t* const newPath = new(::std::nothrow) char8_t[path.Length() + 1];
    (void) ::std::memcpy(newPath, path, path.Length() * sizeof(char8_t));
    newPath[path.Length()] = u8'\0';

    if(targetWindows)
    {
        for(uSys i = 0; i < path.Length(); ++i)
        {
            if(newPath[i] == u8'/')
            {
                newPath[i] = u8'\\';
            }
        }
    }
    else
    {
        for(uSys i = 0; i < path.Length(); ++i)
        {
            if(newPath[i] == u8'\\')
            {
                newPath[i] = u8'/';
            }
        }
    }

    return C8DynString::PassControl(newPath, [](char8_t* const ptr) noexcept { delete[] ptr; });
}

/**
 * Validates that the beginning of a path is valid.
 *
 *   This includes checks like drive specification, and UNC
 * paths.
 *
 * @returns 0 on failure, 1 on pass, and 2 if a bypass is being performed.
 */
static u32 ValidatePathPrefix(const C8DynStringView& path) noexcept
{
    auto begin = path.begin();
    const auto end = path.end();

    if(begin == end)
    {
        return 0;
    }

    if(*begin == 0xFEFF)
    {
        ++begin;
    }

    if(begin == end)
    {
        return 0;
    }

    auto iter = begin;

    if(*iter == U'\\') // Rooted, UNC Absolute, Local Device, Root Local Device
    {
        ++iter;
        if(iter != end)
        {
            if(*iter == U'\\') // UNC Absolute, Local Device, Root Local Device
            {
                ++iter;
                if(iter != end)
                {
                    if(*iter == U'.') // Local Device
                    {
                        if(PathSanitizer::settings().blockLocalDevicePath)
                        {
                            return 0;
                        }

                        auto tmpIter = iter;

                        if(++tmpIter == end || *tmpIter != u8'\\') // Malformed
                        {
                            return 0;
                        }

                        tmpIter = begin;
                        ++tmpIter; // \\.\
                        ++tmpIter; // \\.\X
                        ++tmpIter; // \\.\X:

                        if(tmpIter != end && *tmpIter == U':') // Drive Specification
                        {
                            if(PathSanitizer::settings().blockLocalDeviceDriveAbsolutePath)
                            {
                                return 0;
                            }

                            if(PathSanitizer::settings().blockNonLetterDrive)
                            {
                                if(iswalpha(path[2]))
                                {
                                    return 0;
                                }
                            }

                            ++tmpIter; // \\.\X:X

                            if(tmpIter == end || *tmpIter != U'\\') // Malformed
                            {
                                return 0;
                            }
                        }
                    }
                    else if(*iter == U'?') // Root Local Device
                    {
                        if(PathSanitizer::settings().blockRootLocalDevicePath)
                        {
                            return 0;
                        }

                        auto tmpIter = iter;

                        if(++tmpIter == end || *tmpIter != u8'\\') // Malformed
                        {
                            return 0;
                        }

                        tmpIter = begin;
                        ++tmpIter; // \\.\
                        ++tmpIter; // \\.\X
                        const c32 driveLetter = *tmpIter;
                        ++tmpIter; // \\.\X:

                        if(tmpIter != end && *tmpIter == U':') // Drive Specification
                        {
                            ++tmpIter; // \\.\X:X

                            if(tmpIter != end && *tmpIter == U'\\') // Drive Absolute
                            {
                                if(PathSanitizer::settings().blockRootLocalDeviceDriveAbsolutePath)
                                {
                                    return 0;
                                }
                            }
                            else if(PathSanitizer::settings().blockRootLocalDeviceDriveRelativePath) // Drive Relative
                            {
                                return 0;
                            }

                            if(PathSanitizer::settings().blockNonLetterDrive)
                            {
                                if(iswalpha(driveLetter))
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else // UNC Absolute
                    {
                        if(PathSanitizer::settings().blockUNCPath)
                        {
                            return 0;
                        }

                        // Drive specification is not allowed in a UNC absolute path.
                        for(; iter != end; ++iter)
                        {
                            if(*iter == U':')
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else if(*iter == U'?') // Root Local Device Bypass
            {
                if(PathSanitizer::settings().blockRootLocalDevicePath)
                {
                    return 0;
                }

                ++iter; // \??

                if(iter == end) // Malformed
                {
                    return 0;
                }

                if(*iter != U'?') // Malformed
                {
                    return 0;
                }

                ++iter; // \??\

                if(iter == end) // Malformed
                {
                    return 0;
                }

                if(*iter == '\\') // Fix bypass to proper syntax.
                {
                    return 2;
                }
                else // Malformed
                {
                    return 0;
                }
            }
            else // Rooted
            {
                if(PathSanitizer::settings().blockRootedPath)
                {
                    return 0;
                }

                // Drive specification is not allowed in a rooted path.
                for(; iter != end; ++iter)
                {
                    if(*iter == U':')
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(++iter != end)
        {
            if(*iter == U':') // Drive Absolute, Drive Relative
            {
                if(++iter != end)
                {
                    if(*iter == U'\\') // Drive Absolute
                    {
                        if(PathSanitizer::settings().blockDriveAbsolutePath)
                        {
                            return 0;
                        }
                    }
                    else // Drive Relative
                    {
                        if(PathSanitizer::settings().blockDriveRelativePath)
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

/**
 * Identifies a Win32 device anywhere in the path.
 */
static bool ContainsWin32Device(const C8DynStringView& path) noexcept
{
    bool possibleDeviceEnd = true;

    auto begin = path.begin();
    const auto end = path.end();

    if(*begin == 0xFEFF)
    {
        ++begin;
    }

    auto iter = end;
    --iter;

    // for(iSys i = length - 1; i >= 0; --i)
    for(; iter != begin; --iter)
    {
        if(possibleDeviceEnd)
        {
            /*
             *   Technically devices don't have to end with [1-9], they can
             * end in any wide character digit, including U+00B2, U+00B3,
             * U+00B9, or Superscript 2, Superscript 3, Superscript 1
             * respectively.
             *
             * Source: https://googleprojectzero.blogspot.com/2016/02/the-definitive-guide-on-win32-to-nt.html
             *
             *   Devices can also technically be multiple characters if you
             * have an expansion card. Should this be the case you should
             * prefix the path with `\\.\`, but we'll just reject it either
             * way to be safe.
             */
            while(iter != begin && iswdigit(*iter)) // LPT[1-9], COM[1-9]
            {
                --iter;
            }

            // if(i < 3) // Not enough characters for a device.
            // {
            //     return false;
            // }

            switch(*iter)
            {
                case u8'n':
                case u8'N': // PRN, CON
                {
                    --iter;
                    if(iter == begin)
                    {
                        break;
                    }

                    switch(*iter)
                    {
                        case u8'r':
                        case u8'R': // PRN
                        {
                            --iter;
                            if(iter == begin)
                            {
                                break;
                            }
                            if(*iter == u8'P' || *iter == u8'p')
                            {
                                auto tmpIter = iter;

                                if(iter == begin || *(--tmpIter) == u8'\\')
                                {
                                    return true;
                                }
                                --iter;
                            }
                            break;
                        }
                        case u8'o':
                        case u8'O': // CON
                        {
                            --iter;
                            if(iter == begin)
                            {
                                break;
                            }
                            if(*iter == u8'C' || *iter == u8'c')
                            {
                                auto tmpIter = iter;

                                if(iter == begin || *(--tmpIter) == u8'\\')
                                {
                                    return true;
                                }
                                --iter;
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case u8'x':
                case u8'X': // AUX
                {
                    --iter;
                    if(iter == begin)
                    {
                        break;
                    }
                    if(*iter == u8'U' || *iter == u8'u')
                    {
                        --iter;
                        if(iter == begin)
                        {
                            break;
                        }
                        if(*iter == u8'A' || *iter == u8'a')
                        {
                            auto tmpIter = iter;

                            if(iter == begin || *(--tmpIter) == u8'\\')
                            {
                                return true;
                            }
                            --iter;
                        }
                    }
                    break;
                }
                case u8'l':
                case u8'L': // NUL
                {
                    --iter;
                    if(iter == begin)
                    {
                        break;
                    }
                    if(*iter == u8'U' || *iter == u8'u')
                    {
                        --iter;
                        if(iter == begin)
                        {
                            break;
                        }
                        if(*iter == u8'N' || *iter == u8'n')
                        {
                            auto tmpIter = iter;

                            if(iter == begin || *(--tmpIter) == u8'\\')
                            {
                                return true;
                            }
                            --iter;
                        }
                    }
                    break;
                }
                case u8'$': // CONIN$, CONOUT$
                {
                    // if(i < 5)  // Not enough characters for a device.
                    // {
                    //     return false;
                    // }

                    --iter;
                    if(iter == begin)
                    {
                        break;
                    }
                    switch(*iter)
                    {
                        case u8'n':
                        case u8'N': // CONIN$
                        {
                            --iter;
                            if(iter == begin)
                            {
                                break;
                            }
                            if(*iter == u8'I' || *iter == u8'i')
                            {
                                --iter;
                                if(iter == begin)
                                {
                                    break;
                                }
                                if(*iter == u8'N' || *iter == u8'n')
                                {
                                    --iter;
                                    if(iter == begin)
                                    {
                                        break;
                                    }
                                    if(*iter == u8'O' || *iter == u8'o')
                                    {
                                        --iter;
                                        if(iter == begin)
                                        {
                                            break;
                                        }
                                        if(*iter == u8'C' || *iter == u8'c')
                                        {
                                            auto tmpIter = iter;

                                            if(iter == begin || *(--tmpIter) == u8'\\')
                                            {
                                                return true;
                                            }
                                            --iter;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case u8't':
                        case u8'T': // CONOUT$
                        {
                            // if(i < 5)  // Not enough characters for a device.
                            // {
                            //     return false;
                            // }

                            --iter;
                            if(iter == begin)
                            {
                                break;
                            }
                            if(*iter == u8'U' || *iter == u8'u')
                            {
                                --iter;
                                if(iter == begin)
                                {
                                    break;
                                }
                                if(*iter == u8'O' || *iter == u8'o')
                                {
                                    --iter;
                                    if(iter == begin)
                                    {
                                        break;
                                    }
                                    if(*iter == u8'N' || *iter == u8'n')
                                    {
                                        --iter;
                                        if(iter == begin)
                                        {
                                            break;
                                        }
                                        if(*iter == u8'O' || *iter == u8'o')
                                        {
                                            --iter;
                                            if(iter == begin)
                                            {
                                                break;
                                            }
                                            if(*iter == u8'C' || *iter == u8'c')
                                            {
                                                auto tmpIter = iter;

                                                if(iter == begin || *(--tmpIter) == u8'\\')
                                                {
                                                    return true;
                                                }
                                                --iter;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        /**
         *   Technically a device does not have to be at the end of a
         * path, they are allowed to have arbitrary characters
         * (except '\') after the device, as long as the these
         * characters are prefixed by a '.' or ':', this delimiter is
         * allowed to be prefixed by a space. For safety I'm choosing
         * to consider all whitespace (' ', '\t', '\r', '\n') a valid
         * space, even if that isn't actually true.
         *
         * Example:
         *   COM1       - Valid
         *   COM1a      - Invalid
         *   COM1\      - Invalid
         *   COM1 a     - Invalid
         *   COM1:a     - Valid
         *   COM1  :  a - Valid
         *   COM1.  a   - Valid
         */
        switch(*iter)
        {
            case u8' ':
            case u8'\t':
            case u8'\r':
            case u8'\n':
            case u8'.':
            case u8':':
                possibleDeviceEnd = true;
                break;
            default:
                possibleDeviceEnd = false;
                break;
        }
    }

    return false;
}

static C8DynString CleanDotDirs(const C8DynStringView& pathView) noexcept
{
    // Because we only care about ascii \ and . for this function, we can operate on code units.
    const uSys pathLength = pathView.Length();
    char8_t* path = new(::std::nothrow) char8_t[pathLength + 1];
    ::std::memcpy(path, pathView.String(), pathLength);
    path[pathLength] = u8'\0';

    uSys insert = 0;
    uSys i = 0;

    if(pathLength >= 2 && path[0] == u8'\\' && path[1] == u8'\\')
    {
        if(pathLength >= 4 && (path[2] == '?' || path[2] == '.'))
        {
            i = 4;
            insert = 4;
        }
        else
        {
            i = 2;
            insert = 2;
        }
    }

    for(; i < pathLength; ++i)
    {
        if(path[i] == '?') // Ensure there are no '?' in the path.
        {
            return { };
        }

        if(path[i] == u8'.')
        {
            if(i > 0)
            {
                if(path[i - 1] == u8'\\')
                {
                    if(i < pathLength)
                    {
                        if(path[i + 1] == u8'\\')
                        {
                            i += 2;
                        }
                        else if(path[i + 1] == u8'.')
                        {
                            if(i + 2 < pathLength)
                            {
                                if(path[i + 2] == u8'\\')
                                {
                                    iSys j = insert - 2;
                                    while(j >= 0 && path[j] != u8'\\')
                                    {
                                        --j;
                                        --insert;
                                    }
                                    i += 2;
                                    insert -= 1;
                                    continue;
                                }
                            }
                            else
                            {
                                iSys j = insert - 2;
                                while(j >= 0 && path[j] != u8'\\')
                                {
                                    --j;
                                    --insert;
                                }
                                i += 2;
                                insert -= 1;
                                continue;
                            }
                        }
                    }
                    else
                    {
                        return { };
                    }
                }
            }
            else
            {
                continue;
            }
        }

        path[insert++] = path[i];
    }

    path[insert] = u8'\0';
    return C8DynString::PassControl(path, [](c8* const ptr) { delete[] ptr; });
}
