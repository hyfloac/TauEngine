#pragma warning(push, 0)
#include <cstdio>
#include <cstdlib>
#pragma warning(pop)
#include <NumTypes.hpp>
#include <file/FileHandling.hpp>

char* readFile(const char* filePath) noexcept
{
    FILE* file;
    fopen_s(&file, filePath, "rb");
    fseek(file, 0, SEEK_END);

    const u32 length = ftell(file);
    char* data = reinterpret_cast<char*>(malloc(sizeof(char) * (length + 1)));

    if(!data)
    {
        fclose(file);
        return nullptr;
    }

    fseek(file, 0, SEEK_SET);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    return data;
}

#ifdef _WIN32
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

char* readFileFast(const char* filePath) noexcept
{
    HANDLE fileHandle = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(fileHandle == INVALID_HANDLE_VALUE) { return nullptr; }

    BY_HANDLE_FILE_INFORMATION fileInfo = { };

    BOOL retValue = GetFileInformationByHandle(fileHandle, &fileInfo);

    if(!retValue)
    {
        CloseHandle(fileHandle);
        return nullptr;
    }
    
    const u64 length = (static_cast<u64>(fileInfo.nFileSizeHigh) << 32) | fileInfo.nFileSizeLow;
    char* data = reinterpret_cast<char*>(malloc(sizeof(char) * (length + 1)));

    if(!data)
    {
        CloseHandle(fileHandle);
        return nullptr;
    }

    DWORD bytesRead = 0;
    retValue = ReadFile(fileHandle, data, static_cast<DWORD>(length), &bytesRead, NULL);

    if(!retValue)
    {
        DWORD errorCode = GetLastError();

        if(errorCode == ERROR_IO_PENDING)
        {
            do
            {
                errorCode = GetLastError();
                SwitchToThread();
            } while(errorCode == ERROR_IO_PENDING);
        }
        else
        {
            CloseHandle(fileHandle);
            delete[] data;
            return nullptr;
        }
    }

    CloseHandle(fileHandle);

    data[length] = '\0';

    return data;
}
#endif

void freeFileData(const char* fileData) noexcept
{
    free(const_cast<void*>(reinterpret_cast<const void*>(fileData)));
}
