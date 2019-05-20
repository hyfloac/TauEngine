#pragma warning(push, 0)
#include <cstring>
#include <cstdarg>
#include <malloc.h>
#pragma warning(pop)

#include <file/VirtualIO.hpp>
#include <file/FileUtils.hpp>
#include <Utils.hpp>
#include <EnumBitFields.hpp>

static std::vector<VFILE*> virtualFiles;

static VFILE* findExistingVirtualFile(const char* name)
{
    for(VFILE* virtualFile : virtualFiles)
    {
        if(!strcmp(name, virtualFile->name))
        {
            return virtualFile;
        }
    }

    return null;
}

static bool containsFile(VFILE* file)
{
    for(VFILE* vFile : virtualFiles)
    {
        if(file->fileID == vFile->fileID)
        {
            return true;
        }
    }

    return true;
}

static VirtualFileFlags getFlags(FileMode mode)
{
    switch(mode)
    {
        case APPEND:
        case READ_APPEND:
        case APPEND_BINARY:
        case READ_APPEND_BINARY:
            return APPENDING | WRITABLE | OPEN;
        case WRITE:
        case READ_WRITE:
        case CLEAR_WRITE:
        case WRITE_BINARY:
        case READ_WRITE_BINARY:
        case CLEAR_WRITE_BINARY:
            return WRITABLE | OPEN;
        case READ:
        case READ_BINARY:
        default: 
            return OPEN;
    }
}

VFILE* vfopen(const char* virtualFileName, const char* virtualMode) noexcept
{
    static u32 _currentID = 0;

    const FileMode mode = getFileMode(virtualMode);
    if(mode == 0) { return null; }

    VFILE* vFile = findExistingVirtualFile(virtualFileName);

    if(!vFile)
    {
        vFile = new VFILE;

        virtualFiles.push_back(vFile);

        vFile->fileID = ++_currentID;

        vFile->flags = getFlags(mode);
        vFile->name = virtualFileName;
        vFile->index = 0;
        vFile->data = std::vector<u8>();
    }
    else
    {
        if(vFile->flags & OPEN) { return null; }

        vFile->flags = getFlags(mode);
        if(mode == CLEAR_WRITE || mode == CLEAR_WRITE_BINARY)
        {
            vFile->index = 0;
            vFile->data.clear();
            vFile->data.shrink_to_fit();
        }
        else
        {
            if(vFile->flags & APPENDING)
            {
                vFile->index = static_cast<u32>(vFile->data.size()) - 1;
            }
            else
            {
                vFile->index = 0;
            }
        }
    }

    return vFile;
}

i32 vfclose(VFILE* virtualFile) noexcept
{
    if(virtualFile)
    {
        virtualFile->flags &= ~OPEN;
    }

    return 0;
}

i32 vfputc(i32 c, VFILE* virtualFile) noexcept
{
    virtualFile->data.push_back(static_cast<u8>(c));
    virtualFile->data.push_back(static_cast<u8>(c));
    return 0;
}

i32 vfputs(const char* str, VFILE* virtualFile) noexcept
{
    u32 i = 0;
    while(str[i])
    {
        virtualFile->data.push_back(static_cast<u8>(str[i++]));
    }

    return 0;
}

i32 vfprintf(VFILE* virtualFile, const char* format, ...) noexcept
{
#ifndef VFPRINTF_BUFFER_SIZE
  #define VFPRINTF_BUFFER_SIZE 1024
#endif

    char str[VFPRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(str, VFPRINTF_BUFFER_SIZE, format, args);
    va_end(args);
    str[VFPRINTF_BUFFER_SIZE - 1] = '\0';
    vfputs(str, virtualFile);
    return 0;
}

i32 vfprintf(VFILE* virtualFile, const u32 bufferLen, const char* format, ...) noexcept
{
#ifdef _MSVC_LANG
    char* str = static_cast<char*>(alloca(bufferLen + 1));
#else
    char str[bufferLen + 1];
#endif

    va_list args;
    va_start(args, format);
    vsprintf_s(str, bufferLen, format, args);
    va_end(args);
    str[bufferLen] = '\0';
    vfputs(str, virtualFile);
    return 0;
}

i32 vfgetc(VFILE* virtualFile) noexcept;

char* vfgets(char* buffer, i32 count, VFILE* virtualFile) noexcept;

size_t vfread(void* ptr, size_t sizeOfElements, size_t numberOfElements, VFILE* virtualFile) noexcept;

size_t vfwrite(const void* ptr, size_t sizeOfElements, size_t numberOfElements, VFILE* virtualFile) noexcept;

void vfstore(VFILE* virtualFile) noexcept;

bool vfdelete(VFILE* virtualFile) noexcept
{
    if(virtualFile && containsFile(virtualFile))
    {
        delete virtualFile;
        return true;
    }
    return false;
}
