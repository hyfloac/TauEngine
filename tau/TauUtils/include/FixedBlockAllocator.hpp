#pragma once

#include "Objects.hpp"
#include "PageAllocator.hpp"

enum class AllocationTracking
{
    /**
     * Doesn't perform any allocation tracking.
     */
    None,
    /**
     *   Tracks every time an object is allocated and deallocated.
     * This doesn't track the individual allocations of any given
     * object, but instead keeps track of the number of
     * allocations. Performing a double delete is not tracked.
     */
    Count,
    /**
     *   Tracks every time an object is allocated and deallocated.
     * This also tracks any time an object is double deleted. A
     * double deletion doesn't count towards the deallocation
     * count.
     */
    DoubleDeleteCount
};

class FixedBlockAllocator final
{ };

/**
 *   An allocator where you can't deallocate any objects,
 * but instead delete the entire block in once finished.
 */
template<typename _T, AllocationTracking _AllocTracking = AllocationTracking::None, uSys _NumReservePages = 1024>
class FixedBlockCycleAllocator final
{
    DELETE_COPY(FixedBlockCycleAllocator);
private:
    void* _pages;
    uSys _allocIndex;
public:
    void* allocate() noexcept
    {
        return nullptr;
    }

    void* allocate(const uSys size) noexcept { return allocate(); }

    void deallocate(void* obj) noexcept { }
};
