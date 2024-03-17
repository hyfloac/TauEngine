#include "allocator/PageAllocator.hpp"

#ifdef _WIN32

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

bool PageAllocator::_initialized = false;
uSys PageAllocator::_pageSize = 0;

void PageAllocator::init() noexcept
{
    if(!_initialized)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        _pageSize = sysInfo.dwPageSize;

        _initialized = true;
    }
}

void* PageAllocator::reserve(const uSys numPages) noexcept
{
    return VirtualAlloc(nullptr, numPages * _pageSize, MEM_RESERVE, PAGE_NOACCESS);
}

void* PageAllocator::alloc(const uSys numPages) noexcept
{
    return VirtualAlloc(nullptr, numPages * _pageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void* PageAllocator::commitPage(void* const page) noexcept
{
    return VirtualAlloc(page, _pageSize, MEM_COMMIT, PAGE_READWRITE);
}

void* PageAllocator::commitPages(void* const page, const uSys pageCount) noexcept
{
    return VirtualAlloc(page, pageCount * _pageSize, MEM_COMMIT, PAGE_READWRITE);
}

void PageAllocator::decommitPage(void* const page) noexcept
{
    VirtualFree(page, 1, MEM_DECOMMIT);
}

void PageAllocator::decommitPages(void* const page, const uSys pageCount) noexcept
{
    VirtualFree(page, pageCount * _pageSize, MEM_DECOMMIT);
}

void PageAllocator::free(void* const page) noexcept
{
    VirtualFree(page, 0, MEM_RELEASE);
}

void PageAllocator::setReadWrite(void* const page, const uSys pageCount) noexcept
{
    DWORD oldProtect;
    VirtualProtect(page, pageCount * _pageSize, PAGE_READWRITE, &oldProtect);
}

void PageAllocator::setReadOnly(void* const page, const uSys pageCount) noexcept
{
    DWORD oldProtect;
    VirtualProtect(page, pageCount * _pageSize, PAGE_READONLY, &oldProtect);
}

void PageAllocator::setExecute(void* const page, const uSys pageCount) noexcept
{
    DWORD oldProtect;
    VirtualProtect(page, pageCount * _pageSize, PAGE_EXECUTE_READ, &oldProtect);
}

uSys PageAllocator::pageSize() noexcept
{
    /*   Screw it, I'm tired of dealing with problems of this value
       not being initialized. */
    if(!_initialized)
    { init(); }
    return _pageSize;
}

#endif
