#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdint>
#include <cwchar>
#include <cstddef>
#include <cstring>

typedef ::std::ptrdiff_t iSys;
typedef ::std::size_t uSys;

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

    static wchar_t* sanitizePath(const uSys length, const wchar_t* path) noexcept;
};

int main(int argCount, char* args[])
{
    if(argCount < 2)
    {
        printf("Usage: %s [file]\n", args[0]);
        return 1;
    }

    PathSanitizerSettings settings;
    settings.blockUNCPath = false;
    settings.blockLocalDevicePath = true;
    settings.blockLocalDeviceDriveAbsolutePath = false;
    settings.blockRootLocalDevicePath = false;
    settings.blockRootLocalDeviceDriveAbsolutePath = false;
    settings.blockRootLocalDeviceDriveRelativePath = false;
    settings.blockDriveAbsolutePath = false;
    settings.blockDriveRelativePath = false;
    settings.blockRootedPath = false;
    settings.blockDevices = false;
    settings.blockNonLetterDrive = false;
    settings.lockSettings = false;

    PathSanitizer::setSettings(settings);

    const char* path = args[1];

    ::std::mbstate_t state;
    const uSys wPathLen = ::std::mbsrtowcs(nullptr, &path, 0, &state) + 1;
    wchar_t* const wPath = new(::std::nothrow) wchar_t[wPathLen];
    ::std::mbsrtowcs(wPath, &path, wPathLen, &state);

    wprintf(L"Original Path: %s\n", wPath);

    wchar_t* const buf = PathSanitizer::sanitizePath(wPathLen - 1, wPath);

    if(!buf)
    {
        wprintf(L"Path did not validate.\n");
        return -1;
    }

    wprintf(L"Cleaned path: %s\n", buf);

    delete[] wPath;
    delete[] buf;

    return 0;
}

PathSanitizerSettings PathSanitizer::_settings;

static bool isValidCharset(uSys length, const wchar_t* path) noexcept;
static wchar_t* fixPathSeparator(uSys length, const wchar_t* path) noexcept;
static bool validatePathPrefix(uSys length, wchar_t* path) noexcept;
static bool containsWin32Device(uSys length, const wchar_t* path) noexcept;
static bool ensureWin32DriveIsLetter(uSys length, const wchar_t* path) noexcept;
static bool cleanDotDirs(uSys pathLength, wchar_t* path) noexcept;

void PathSanitizer::setSettings(const PathSanitizerSettings& settings) noexcept
{
    if(!_settings.lockSettings)
    {
        _settings = settings;
    }
}

wchar_t* PathSanitizer::sanitizePath(const uSys length, const wchar_t* path) noexcept
{
    if(!isValidCharset(length, path))
    {
        return nullptr;
    }

    wchar_t* const newPath = fixPathSeparator(length, path);

    if(!validatePathPrefix(length, newPath))
    {
        delete[] newPath;
        return nullptr;
    }

    if(!cleanDotDirs(length, newPath))
    {
        delete[] newPath;
        return nullptr;
    }

    const uSys pathLength = ::std::wcslen(newPath);

    if(_settings.blockDevices && containsWin32Device(pathLength, newPath))
    {
        delete[] newPath;
        return nullptr;
    }

    return newPath;
}

static bool isValidCharset(const uSys length, const wchar_t* const path) noexcept
{
    uSys colonCount = 0;

    for(uSys i = 0; i < length; ++i)
    {
        if(path[i] >= 0 && path[i] <= 31)
        {
            continue;
        }

        switch(path[i])
        {
            case L'<':
            case L'>':
            case L'\"':
            case L'|':
            // case L'?':
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
                                if(!iswalpha(path[4]))
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
                                if(!iswalpha(path[4]))
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
                        if(!iswalpha(path[4]))
                        {
                            return false;
                        }
                    }
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
         *   Technically a device does not have to at the end of a
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

/**
 * Ensures that the drive letter is indeed a letter.
 */
static bool ensureWin32DriveIsLetter(const uSys length, const wchar_t* const path) noexcept
{
    if(length < 2) // To short to be a Drive Absolute or Drive Relative path
    {
        return true;
    }

    if(path[1] == L':') // Is a Drive Absolute or Drive Relative path
    {
        /**
         *   Drive letter is not a drive letter, probably not an issue,
         * but it's better to be safe than sorry. There is some minor
         * potential that this could alias to DosDevices object
         * directory.
         */
        if(iswalpha(path[0]))
        {
            return true;
        }

        return false;
    }

    return true;
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
