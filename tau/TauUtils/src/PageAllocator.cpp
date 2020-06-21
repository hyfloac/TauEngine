#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#include "allocator/PageAllocator.hpp"

static bool initialized = false;
static DWORD _pageSize = 0;

void PageAllocator::init() noexcept
{
    if(!initialized)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        _pageSize = sysInfo.dwPageSize;

        initialized = true;
    }
}

void* PageAllocator::reserve(const uSys numPages) noexcept
{
    return VirtualAlloc(NULL, numPages * _pageSize, MEM_RESERVE, PAGE_NOACCESS);
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

uSys PageAllocator::pageSize() noexcept
{
    return _pageSize;
}
