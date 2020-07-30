#include <Windows.h>
#include <stdint.h>
#include <stdio.h>

#define MIN(_X, _Y) (_X < _Y ? _X : _Y)

typedef uint64_t u64;
typedef uint8_t byte;

typedef struct 
{
    HANDLE handle;
    void* buffer;
    u64 fileSize;
    u64 blockSize;
    u64 bufferOffset;
    u64 bufferReadCount;
    u64 readOffset;
} File;

File* openFileW(const wchar_t* path);
File* openFile(const char* path);
void closeFile(File* file);
u64 readFile(File* file, byte* data, u64 size);
int readChar(File* file);

void dumpFile(File* file);
void dumpFileSingle(File* file);

int main(const int argCount, char* args[])
{
    if(argCount < 2)
    {
        return 1;
    }

    BOOL readSingle = FALSE;

    if(argCount > 2)
    {
        if(strcmp(args[2], "single") == 0)
        {
            readSingle = TRUE;
        }
    }

    File* file = openFile(args[1]);

    if(readSingle)
    {
        dumpFileSingle(file);
    }
    else
    {
        dumpFile(file);
    }

    closeFile(file);

    return 0;
}

void dumpFile(File* file)
{
    while(TRUE)
    {
        byte buf[256];
        u64 read = readFile(file, buf, 256);
        if(read == 0)
        {
            break;
        }

        printf("%.*s", (int) read, (const char*) buf);

        if(read != 256)
        {
            break;
        }
    }
}

void dumpFileSingle(File* file)
{
    while(TRUE)
    {
        int c = readChar(file);
        if(c == -1)
        {
            break;
        }

        fputc(c, stdout);
    }
}

File* openFileW(const wchar_t* path)
{
    // Open file
    const HANDLE handle = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, NULL);
    if(handle == INVALID_HANDLE_VALUE)
    { 
        return NULL; 
    }

    // Get file size
    DWORD fileSizeHigh;
    DWORD fileSizeLow = GetFileSize(handle, &fileSizeHigh);

    // Allocate file struct
    File* const ret = malloc(sizeof(File));

    ret->handle = handle;
    // Allocate buffer aligned to the 4096 byte boundary
    ret->buffer = VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    // Compute the file size as a 64 bit number
    ret->fileSize = ((u64) fileSizeHigh << 32) | ((u64) fileSizeLow);
    ret->blockSize = 4096;
    ret->bufferOffset = 0;
    ret->bufferReadCount = 0;
    ret->readOffset = 0;

    return ret;
}

File* openFile(const char* path)
{
    // Convert to wide string
    const int wBufSize = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    wchar_t* const wPath = malloc(sizeof(wchar_t) * wBufSize);
    MultiByteToWideChar(CP_UTF8, 0, path, -1, wPath, wBufSize);

    // Open file
    File* const ret = openFileW(wPath);

    // Free wide string
    free(wPath);

    return ret;
}

void closeFile(File* file)
{
    // Close file
    CloseHandle(file->handle);
    // Free buffer
    VirtualFree(file->buffer, file->blockSize, MEM_RELEASE);
    // Free file structure
    free(file);
}

u64 readFile(File* file, byte* const data, u64 size)
{
    if(size == 0)
    {
        return 0;
    }

    // Initial read of a new file
    if(file->bufferReadCount == 0)
    {
        // Read into buffer
        DWORD readCount;
        const BOOL read = ReadFile(file->handle, file->buffer, file->blockSize, &readCount, NULL);
        if(!read)
        {
            return 0;
        }

        file->bufferReadCount = readCount;

        if(file->bufferReadCount == 0)
        {
            return 0;
        }
    }

    u64 writeOffset = 0;

    while(TRUE)
    {
        // Number of bytes to the end of buffer
        const u64 countToEnd = file->bufferReadCount - file->bufferOffset;
        // The maximum number of bytes to copy
        const u64 copyCount = MIN(countToEnd, size);
        // Copy the buffered data into the user buffer
        memcpy(data + writeOffset, file->buffer + file->bufferOffset, copyCount);

        // Update the write offset into the user buffer
        writeOffset += copyCount;
        // Decrease the number of bytes that still need to be read to fill the user buffer
        size -= copyCount;
        // Update the offset into the cache buffer
        file->bufferOffset += copyCount;

        // Reached end of buffer
        if(file->bufferOffset == file->bufferReadCount)
        {
            // Reached end of file
            if(file->bufferReadCount != file->blockSize)
            {
                break;
            }

            // Read into buffer
            DWORD readCount;
            const BOOL read = ReadFile(file->handle, file->buffer, file->blockSize, &readCount, NULL);
            if(!read)
            {
                return 0;
            }

            file->bufferReadCount = readCount;

            if(file->bufferReadCount == 0)
            {
                return 0;
            }

            // Update the offset into the file
            file->readOffset += file->bufferReadCount;
            // Reset the offset into the buffer
            file->bufferOffset = 0;
        }

        // Finished filling the buffer
        if(size == 0)
        {
            break;
        }
    }

    return writeOffset;
}

int readChar(File* file)
{
    // Initial read of a new file
    if(file->bufferReadCount == 0)
    {
        // Read into buffer
        DWORD readCount;
        const BOOL read = ReadFile(file->handle, file->buffer, file->blockSize, &readCount, NULL);
        if(!read)
        {
            return -1;
        }

        file->bufferReadCount = readCount;

        if(file->bufferReadCount == 0)
        {
            return -1;
        }
    }
    // Reached end of buffer
    else if(file->bufferReadCount == file->bufferOffset)
    {
        // Read into buffer
        DWORD readCount;
        const BOOL read = ReadFile(file->handle, file->buffer, file->blockSize, &readCount, NULL);
        if(!read)
        {
            return -1;
        }

        file->bufferReadCount = readCount;

        if(file->bufferReadCount == 0)
        {
            return -1;
        }
        
        // Update the offset into the file
        file->readOffset += file->bufferReadCount;
        // Reset the offset into the buffer
        file->bufferOffset = 0;
    }

    // Get character
    const char ret = ((const char*) file->buffer)[file->bufferOffset];

    // Update the offset into the cache buffer
    ++file->bufferOffset;

    return ret;
}
