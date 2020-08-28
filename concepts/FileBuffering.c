#include <Windows.h>
#include <stdio.h>

int main(int argCount, char* args[])
{
    HANDLE file = CreateFileW(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, NULL);

    if(!file)
    {
        (void) wprintf(L"File failed to open.\n");
        DWORD error = GetLastError();
        WCHAR* buf = (WCHAR*) malloc(sizeof(WCHAR) * 65536);
        (void) FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM , NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 65536, NULL);
        (void) wprintf(L"Error: %s\n", buf);
        free(buf);
    }

    void* page = VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    const char* str = "Hello World!\n";
    (void) memcpy(page, str, strlen(str));

    DWORD bytesWritten;
    (void) WriteFile(file, page, 4096, &bytesWritten, NULL);

    VirtualFree(page, 4096, MEM_RELEASE);

    // SetFileValidData(file, strlen(str));
    // LONG posHigh = 0;
    // const DWORD newFilePointer = SetFilePointer(file, strlen(str), &posHigh, FILE_BEGIN);
    // if(newFilePointer == INVALID_SET_FILE_POINTER)
    // {
    //     (void) wprintf(L"SetFilePointer failed.\n");
    //     DWORD error = GetLastError();
    //     WCHAR* buf = (WCHAR*) malloc(sizeof(WCHAR) * 65536);
    //     (void) FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM , NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 65536, NULL);
    //     (void) wprintf(L"Error: (%u/0x%08x) %s\n", error, error, buf);
    //     free(buf);
    // }
    // SetEndOfFile(file);

    FILE_END_OF_FILE_INFO feof;
    feof.EndOfFile.QuadPart = strlen(str);
    const BOOL setFileInfoRet = SetFileInformationByHandle(file, FileEndOfFileInfo, &feof, sizeof(feof));
    if(!setFileInfoRet)
    {
        (void) wprintf(L"Failed to set file size.\n");
        DWORD error = GetLastError();
        WCHAR* buf = (WCHAR*) malloc(sizeof(WCHAR) * 65536);
        (void) FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM , NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 65536, NULL);
        (void) wprintf(L"Error: %s\n", buf);
        free(buf);
    }

    CloseHandle(file);

    return 0;
}
