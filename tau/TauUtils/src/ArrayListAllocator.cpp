#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#include "ArrayList.hpp"

static bool initialized = false;
static DWORD _pageSize = 0;

void ArrayListAllocator::init() noexcept
{
    if(!initialized)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        _pageSize = sysInfo.dwPageSize;

        initialized = true;
    }
}

void* ArrayListAllocator::reserve(std::size_t numPages) noexcept
{
    init();
    return VirtualAlloc(NULL, numPages * _pageSize, MEM_RESERVE, PAGE_NOACCESS);
}

void* ArrayListAllocator::commitPage(void* page) noexcept
{
    return VirtualAlloc(page, _pageSize, MEM_COMMIT, PAGE_READWRITE);
}

void ArrayListAllocator::decommitPage(void* page) noexcept
{
    VirtualFree(page, 1, MEM_DECOMMIT);
}

void ArrayListAllocator::decommitPages(void* page, std::size_t pageCount) noexcept
{
    VirtualFree(page, pageCount * _pageSize, MEM_DECOMMIT);
}

void ArrayListAllocator::free(void* page) noexcept
{
    VirtualFree(page, 0, MEM_RELEASE);
}

std::size_t ArrayListAllocator::pageSize() noexcept
{
    return _pageSize;
}
