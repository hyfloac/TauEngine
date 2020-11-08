/**
 * @file
 */
#include "PathSanitizer.hpp"

PathSanitizerSettings PathSanitizer::_settings;

static bool isValidCharset(uSys length, const wchar_t* path) noexcept;
static wchar_t* fixPathSeparator(uSys length, const wchar_t* path) noexcept;
static bool validatePathPrefix(uSys length, wchar_t* path) noexcept;
static bool containsWin32Device(uSys length, const wchar_t* path) noexcept;
static bool cleanDotDirs(uSys pathLength, wchar_t* path) noexcept;

void PathSanitizer::setSettings(const PathSanitizerSettings& settings) noexcept
{
    if(!_settings.lockSettings)
    {
        _settings = settings;
    }
}

WDynString PathSanitizer::sanitizePath(const WDynString& path) noexcept
{
    if(!isValidCharset(path.length(), path.c_str()))
    {
        return { };
    }

    wchar_t* const newPath = fixPathSeparator(path.length(), path.c_str());

    if(!validatePathPrefix(path.length(), newPath))
    {
        delete[] newPath;
        return { };
    }

    if(!cleanDotDirs(path.length(), newPath))
    {
        delete[] newPath;
        return { };
    }

    const uSys pathLength = ::std::wcslen(newPath);

    const uSys fullPathLen = GetFullPathNameW(newPath, 0, nullptr, nullptr);

    wchar_t* const fullPath = new(::std::nothrow) wchar_t[fullPathLen + 1];
    fullPath[fullPathLen] = L'\0';
    if(GetFullPathNameW(newPath, static_cast<DWORD>(fullPathLen), fullPath, nullptr) != fullPathLen)
    {
        delete[] newPath;
        delete[] fullPath;
        return { };
    }

    delete[] newPath;

    if(_settings.blockDevices && containsWin32Device(pathLength, fullPath))
    {
        delete[] fullPath;
        return { };
    }

    return WDynString::passControl(fullPath);
}

WDynString PathSanitizer::sanitizeSubPath(const WDynString& path) noexcept
{
    if(!isValidCharset(path.length(), path.c_str()))
    {
        return { };
    }

    wchar_t* const newPath = fixPathSeparator(path.length(), path.c_str());

    return newPath;
}

static bool isValidCharset(const uSys length, const wchar_t* const path) noexcept
{
    uSys colonCount = 0;

    for(uSys i = 0; i < length; ++i)
    {
        if(path[i] <= 31)
        { continue; }

        switch(path[i])
        {
            case L'<':
            case L'>':
            case L'\"':
            case L'|':
            case L'?':
            case L'*': return false;
            default: break;
        }

        if(path[i] == L':')
        {
            ++colonCount;
            if(colonCount > 1)
            {
                return false;
            }
        }

        if(path[i] == L'.') // Ensure file doesn't end with a '.'
        {
            if(i + 1 == length || path[i + 1] == L'\\' || path[i + 1] == L'/')
            {
                if(i > 0)
                {
                    if(path[i - 1] != L'\\' && path[i - 1] != L'/')
                    {
                        if(path[i - 1] == L'.')
                        {
                            if(i > 1 && path[i - 2] != L'\\' && path[i - 2] != L'/')
                            {
                                return false;
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

static wchar_t* fixPathSeparator(const uSys length, const wchar_t* const path) noexcept
{
    wchar_t* const newPath = new(::std::nothrow) wchar_t[length + 1];
    (void) ::std::memcpy(newPath, path, (length + 1) * sizeof(wchar_t));

    for(uSys i = 0; i < length; ++i)
    {
        if(newPath[i] == L'/')
        {
            newPath[i] = L'\\';
        }
    }

    return newPath;
}

/**
 * Validates that the beginning of a path is valid.
 *
 *   This includes checks like drive specification, and UNC
 * paths.
 */
static bool validatePathPrefix(const uSys length, wchar_t* const path) noexcept
{
    if(length < 1)
    { return false; }

    if(path[0] == L'\\') // Rooted, UNC Absolute, Local Device, Root Local Device
    {
        if(length >= 2)
        {
            if(path[1] == L'\\') // UNC Absolute, Local Device, Root Local Device
            {
                if(length >= 3)
                {
                    if(path[2] == L'.') // Local Device
                    {
                        if(PathSanitizer::settings().blockLocalDevicePath)
                        {
                            return false;
                        }

                        if(length < 3 || path[3] != L'\\') // Malformed
                        {
                            return false;
                        }

                        if(length >= 6 && path[5] == L':') // Drive Specification
                        {
                            if(PathSanitizer::settings().blockLocalDeviceDriveAbsolutePath)
                            {
                                return false;
                            }

                            if(PathSanitizer::settings().blockNonLetterDrive)
                            {
                                if(iswalpha(path[2]))
                                {
                                    return false;
                                }
                            }

                            if(length < 7 || path[6] != L'\\') // Malformed
                            {
                                return false;
                            }
                        }
                    }
                    else if(path[2] == L'?') // Root Local Device
                    {
                        if(PathSanitizer::settings().blockRootLocalDevicePath)
                        {
                            return false;
                        }

                        if(length < 3 || path[3] != L'\\') // Malformed
                        {
                            return false;
                        }

                        if(length >= 6 && path[5] == L':') // Drive Specification
                        {
                            if(length >= 7 && path[6] == L'\\') // Drive Absolute
                            {
                                if(PathSanitizer::settings().blockRootLocalDeviceDriveAbsolutePath)
                                {
                                    return false;
                                }
                            }
                            else if(PathSanitizer::settings().blockRootLocalDeviceDriveRelativePath) // Drive Relative
                            {
                                return false;
                            }

                            if(PathSanitizer::settings().blockNonLetterDrive)
                            {
                                if(iswalpha(path[2]))
                                {
                                    return false;
                                }
                            }
                        }
                    }
                    else // UNC Absolute
                    {
                        if(PathSanitizer::settings().blockUNCPath)
                        {
                            return false;
                        }

                        // Drive specification is not allowed in a UNC absolute path.
                        for(uSys i = 1; i < length; ++i)
                        {
                            if(path[i] == L':')
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            else if(path[1] == L'?') // Root Local Device Bypass
            {
                if(PathSanitizer::settings().blockRootLocalDevicePath)
                {
                    return false;
                }

                if(length >= 4 && path[2] == '?' && path[3] == '\\') // Fix bypass to proper syntax.
                {
                    path[1] = '\\';
                }
                else // Malformed
                {
                    return false;
                }
            }
            else // Rooted
            {
                if(PathSanitizer::settings().blockRootedPath)
                {
                    return false;
                }

                // Drive specification is not allowed in a rooted path.
                for(uSys i = 1; i < length; ++i)
                {
                    if(path[i] == L':')
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(length >= 2)
        {
            if(path[1] == L':') // Drive Absolute, Drive Relative
            {
                if(length >= 3)
                {
                    if(path[2] == L'\\') // Drive Absolute
                    {
                        if(PathSanitizer::settings().blockDriveAbsolutePath)
                        {
                            return false;
                        }
                    }
                    else // Drive Relative
                    {
                        if(PathSanitizer::settings().blockDriveRelativePath)
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

/**
 * Identifies a Win32 device anywhere in the path.
 */
static bool containsWin32Device(const uSys length, const wchar_t* const path) noexcept
{
    bool possibleDeviceEnd = true;

    for(iSys i = length - 1; i >= 0; --i)
    {
        if(possibleDeviceEnd)
        {
            /*
             *   Technically devices don't have to end with [1-9], they can
             * end in an wide character digit, including U+00B2, U+00B3,
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
            while(i >= 0 && iswdigit(path[i])) // LPT[1-9], COM[1-9]
            {
                --i;
            }

            if(i < 3) // Not enough characters for a device.
            {
                return false;
            }

            switch(path[i])
            {
                case L'n':
                case L'N': // PRN, CON
                {
                    --i;
                    switch(path[i])
                    {
                        case L'r':
                        case L'R': // PRN
                        {
                            --i;
                            if(path[i] == L'P' || path[i] == L'p')
                            {
                                if(i == 0 || path[i - 1] == L'\\')
                                {
                                    return true;
                                }
                                --i;
                            }
                            break;
                        }
                        case L'o':
                        case L'O': // CON
                        {
                            --i;
                            if(path[i] == L'C' || path[i] == L'c')
                            {
                                if(i == 0 || path[i - 1] == L'\\')
                                {
                                    return true;
                                }
                                --i;
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case L'x':
                case L'X': // AUX
                {
                    --i;
                    if(path[i] == L'U' || path[i] == L'u')
                    {
                        --i;
                        if(path[i] == L'A' || path[i] == L'a')
                        {
                            if(i == 0 || path[i - 1] == L'\\')
                            {
                                return true;
                            }
                            --i;
                        }
                    }
                    break;
                }
                case L'l':
                case L'L': // NUL
                {
                    --i;
                    if(path[i] == L'U' || path[i] == L'u')
                    {
                        --i;
                        if(path[i] == L'N' || path[i] == L'n')
                        {
                            if(i == 0 || path[i - 1] == L'\\')
                            {
                                return true;
                            }
                            --i;
                        }
                    }
                    break;
                }
                case L'$': // CONIN$, CONOUT$
                {
                    if(i < 5)  // Not enough characters for a device.
                    {
                        return false;
                    }

                    --i;
                    switch(path[i])
                    {
                        case L'n':
                        case L'N': // CONIN$
                        {
                            --i;
                            if(path[i] == L'I' || path[i] == L'i')
                            {
                                --i;
                                if(path[i] == L'N' || path[i] == L'n')
                                {
                                    --i;
                                    if(path[i] == L'O' || path[i] == L'o')
                                    {
                                        --i;
                                        if(path[i] == L'C' || path[i] == L'c')
                                        {
                                            if(i == 0 || path[i - 1] == L'\\')
                                            {
                                                return true;
                                            }
                                            --i;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case L't':
                        case L'T': // CONOUT$
                        {
                            if(i < 5)  // Not enough characters for a device.
                            {
                                return false;
                            }

                            --i;
                            if(path[i] == L'U' || path[i] == L'u')
                            {
                                --i;
                                if(path[i] == L'O' || path[i] == L'o')
                                {
                                    --i;
                                    if(path[i] == L'N' || path[i] == L'n')
                                    {
                                        --i;
                                        if(path[i] == L'O' || path[i] == L'o')
                                        {
                                            --i;
                                            if(path[i] == L'C' || path[i] == L'c')
                                            {
                                                if(i == 0 || path[i - 1] == L'\\')
                                                {
                                                    return true;
                                                }
                                                --i;
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
         * to consider all whitespace (' ', '\t', '\r', '\n) a valid
         * space, even if that isn't actually true.
         *
         * Example:
         *   COM1       - Valid
         *   COM1a      - Invalid
         *   COM1\      - Invalid
         *   COM1 a     - Invalid
         *   COM1:a     - valid
         *   COM1  :  a - valid
         *   COM1.  a   - valid
         */
        switch(path[i])
        {
            case L' ':
            case L'\t':
            case L'\r':
            case L'\n':
            case L'.':
            case L':':
                possibleDeviceEnd = true;
                break;
            default:
                possibleDeviceEnd = false;
                break;
        }
    }

    return false;
}

static bool cleanDotDirs(const uSys pathLength, wchar_t* const path) noexcept
{
    uSys insert = 0;
    uSys i = 0;

    if(pathLength >= 2 && path[0] == L'\\' && path[1] == L'\\')
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
            return false;
        }

        if(path[i] == L'.')
        {
            if(i > 0)
            {
                if(path[i - 1] == L'\\')
                {
                    if(i < pathLength)
                    {
                        if(path[i + 1] == L'\\')
                        {
                            i += 2;
                        }
                        else if(path[i + 1] == L'.')
                        {
                            if(i + 2 < pathLength)
                            {
                                if(path[i + 2] == L'\\')
                                {
                                    iSys j = insert - 2;
                                    while(j >= 0 && path[j] != L'\\')
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
                                while(j >= 0 && path[j] != L'\\')
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
                        return false;
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

    path[insert] = L'\0';
    return true;
}
