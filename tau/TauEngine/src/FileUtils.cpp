#include <TauEngine.hpp>
#include <Safeties.hpp>
#include <file/FileUtils.hpp>

NonNull const char* getFileModeStr(FileMode fileMode) noexcept
{
    switch(fileMode)
    {
        case READ:               return "r";
        case WRITE:              return "w";
        case APPEND:             return "a";
        case READ_WRITE:         return "r+";
        case CLEAR_WRITE:        return "w+";
        case READ_APPEND:        return "a+";

        case READ_BINARY:        return "rb";
        case WRITE_BINARY:       return "wb";
        case APPEND_BINARY:      return "ab";
        case READ_WRITE_BINARY:  return "rb+";
        case CLEAR_WRITE_BINARY: return "wb+";
        case READ_APPEND_BINARY: return "ab+";
        default:                 return "";
    }
}

FileMode getFileMode(const char* fileMode) noexcept
{
    Ensure(fileMode != null);

    if(fileMode && fileMode[0])
    {
        if(fileMode[1])
        {
            if(fileMode[2])
            {
                if((fileMode[1] == 'b' && fileMode[2] == '+') || 
                   (fileMode[1] == '+' && fileMode[2] == 'b'))
                {
                    switch(fileMode[0])
                    {
                        case 'r': return READ_WRITE_BINARY;
                        case 'w': return CLEAR_WRITE_BINARY;
                        case 'a': return READ_APPEND_BINARY;
                        default: break;
                    }
                }
            }
            else
            {
                if(fileMode[1] == '+')
                {
                    switch(fileMode[0])
                    {
                        case 'r': return READ_WRITE;
                        case 'w': return CLEAR_WRITE;
                        case 'a': return READ_APPEND;
                        default: break;
                    }
                }
                else if(fileMode[1] == 'b')
                {
                    switch(fileMode[0])
                    {
                    case 'r': return READ_BINARY;
                    case 'w': return WRITE_BINARY;
                    case 'a': return APPEND_BINARY;
                    default: break;
                    }
                }
            }
        }
        else
        {
            switch(fileMode[0])
            {
                case 'r': return READ;
                case 'w': return WRITE;
                case 'a': return APPEND;
                default: break;
            }
        }
    }

    return static_cast<FileMode>(0);
}

#ifdef _WIN32
#include <Windows.h>

void loadResourceFile(int name, int type, int* size, const char** data) noexcept
{
    static const HMODULE handle = GetModuleHandleA(NULL);
    const HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
    const HGLOBAL rcData = LoadResource(handle, rc);
    *size = SizeofResource(handle, rc);
    *data = static_cast<const char*>(LockResource(rcData));
}
#endif
