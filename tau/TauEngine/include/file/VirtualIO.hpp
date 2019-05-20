#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <DLL.hpp>

enum VirtualFileFlags
{
    WRITABLE   = 1 << 0,
    APPENDING  = 1 << 1,
    COMPRESSED = 1 << 2,
    OPEN       = 1 << 3,
};

typedef struct VirtualFile
{
    VirtualFileFlags flags;
    std::vector<u8> data;
    u32 index;
    u32 fileID;
    const char* name;
} VFILE;

TAU_DLL VFILE* vfopen(const char* virtualFileName, const char* virtualMode) noexcept;

TAU_DLL i32 vfclose(VFILE* virtualFile) noexcept;

TAU_DLL i32 vfputc(i32 c, VFILE* virtualFile) noexcept;

TAU_DLL i32 vfputs(const char* str, VFILE* virtualFile) noexcept;

TAU_DLL i32 vfprintf(VFILE* virtualFile, const char* format, ...) noexcept;

TAU_DLL i32 vfprintf(VFILE* virtualFile, const u32 bufferLen, const char* format, ...) noexcept;

i32 vfgetc(VFILE* virtualFile) noexcept;

char* vfgets(char* buffer, i32 count, VFILE* virtualFile) noexcept;

size_t vfread(void* ptr, size_t sizeOfElements, size_t numberOfElements, VFILE* virtualFile) noexcept;

size_t vfwrite(const void* ptr, size_t sizeOfElements, size_t numberOfElements, VFILE* virtualFile) noexcept;

void vfstore(VFILE* virtualFile) noexcept;

TAU_DLL bool vfdelete(VFILE* virtualFile) noexcept;
