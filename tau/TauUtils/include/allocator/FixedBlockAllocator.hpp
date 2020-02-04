#pragma once

#include "Objects.hpp"
#include "TauAllocator.hpp"
#include "PageAllocator.hpp"

class FixedBlockAllocator final
{ };

/**
 *   An allocator where you can't deallocate any objects, but
 * instead delete the entire block in once finished.
 *
 *   It is still important to call the destructor of each
 * object. When the allocator is released all of the memory is
 * released, but no destructors are called.
 *
 *   You should be quite liberal with the number of reserved
 * pages. Reserving more pages doesn't have any effect on the
 * memory consumption of the application. The pages will only
 * be truly allocated when they are first used, and the
 * allocator only commits them when needed. A page _should_ be
 * 4 KiB, but that isn't necessarily guaranteed. The formal
 * way to get the page size is to call `PageAllocator::pageSize()`.
 * That being said, don't go too large as it may end up
 * consuming the entire potential address space of your
 * program.
 */
template<AllocationTracking _AllocTracking = AllocationTracking::None, uSys _NumReservePages = 1024>
class FixedBlockArenaAllocator final : public TauAllocator
{
    DELETE_CONSTRUCT(FixedBlockArenaAllocator);
    DEFAULT_DESTRUCT(FixedBlockArenaAllocator);
    DELETE_COPY(FixedBlockArenaAllocator);
public:
    FixedBlockArenaAllocator(uSys blockSize) noexcept = delete;

    [[nodiscard]] inline uSys blockSize() const noexcept { return 0; }
    [[nodiscard]] inline iSys allocationDifference() const noexcept { return 0; }
    [[nodiscard]] inline uSys multipleDeleteCount() const noexcept { return 0; }

    [[nodiscard]] inline void* allocate() noexcept { return nullptr; }
    [[nodiscard]] inline void* allocate(uSys size) noexcept { return nullptr; }
    inline void deallocate(void* obj) noexcept { }
};

/**
 *   This allocator does not perform any allocation tracking.
 * If you do need to keep track of every allocation you should
 * specify a different AllocationTracking.
 */
template<uSys _NumReservePages>
class FixedBlockArenaAllocator<AllocationTracking::None, _NumReservePages> final : public TauAllocator
{
    DELETE_COPY(FixedBlockArenaAllocator);
private:
    void* _pages;
    uSys _allocatedPages;
    uSys _blockSize;
    uSys _allocIndex;
public:
    inline FixedBlockArenaAllocator(const uSys blockSize) noexcept
        : _pages(PageAllocator::reserve(_NumReservePages)),
          _allocatedPages(0), _blockSize(blockSize), _allocIndex(0)
    { }

    inline ~FixedBlockArenaAllocator() noexcept
    { PageAllocator::decommitPages(_pages, _allocatedPages); }

    [[nodiscard]] inline uSys blockSize() const noexcept { return _blockSize; }

    [[nodiscard]] inline void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        return ret;
    }

    [[nodiscard]] inline void* allocate(const uSys size) noexcept override { return allocate(); }

    inline void deallocate(void* obj) noexcept override { }
private:
    bool assertSize() noexcept
    {
        const uSys pageBytes = _allocatedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_allocatedPages == _NumReservePages)
            { return false; }
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_allocatedPages;
        }

        return true;
    }
};

/**
 *   This allocator keeps track of how many allocations there
 * are compared to how many deallocations there are. If these
 * two don't match you likely have a bug in your code
 * somewhere.
 */
template<uSys _NumReservePages>
class FixedBlockArenaAllocator<AllocationTracking::Count, _NumReservePages> final : public TauAllocator
{
    DELETE_COPY(FixedBlockArenaAllocator);
private:
    void* _pages;
    uSys _allocatedPages;
    uSys _blockSize;
    uSys _allocIndex;
    iSys _allocationDifference;
public:
    inline FixedBlockArenaAllocator(const uSys blockSize) noexcept
        : _pages(PageAllocator::reserve(_NumReservePages)),
          _allocatedPages(0), _blockSize(blockSize), _allocIndex(0),
    _allocationDifference(0)
    { }

    inline ~FixedBlockArenaAllocator() noexcept
    { PageAllocator::decommitPages(_pages, _allocatedPages); }

    [[nodiscard]] inline uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] inline iSys allocationDifference() const noexcept { return _allocationDifference; }

    [[nodiscard]] inline void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        ++_allocationDifference;
        return ret;
    }

    [[nodiscard]] inline void* allocate(const uSys size) noexcept override { return allocate(); }

    inline void deallocate(void* obj) noexcept override
    {
        if(!obj)
        { return; }
        --_allocationDifference;
    }
private:
    bool assertSize() noexcept
    {
        const uSys pageBytes = _allocatedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_allocatedPages == _NumReservePages)
            { return false; }
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_allocatedPages;
        }

        return true;
    }
};

/**
 *   This allocator keeps track of how many allocations there
 * are compared to how many deallocations there are. If these
 * two don't match you likely have a bug in your code
 * somewhere.
 *
 *   This allocator also keeps track of any double deletes
 * that may occur.
 */
template<uSys _NumReservePages>
class FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount, _NumReservePages> final : public TauAllocator
{
    DELETE_COPY(FixedBlockArenaAllocator);
private:
    void* _pages;
    uSys _allocatedPages;
    uSys _blockSize;
    uSys _allocIndex;
    iSys _allocationDifference;
    uSys _doubleDeleteCount;
    uSys _multipleDeleteCount;
public:
    inline FixedBlockArenaAllocator(const uSys blockSize) noexcept
        : _pages(PageAllocator::reserve(_NumReservePages)),
          _allocatedPages(0), _blockSize(blockSize + sizeof(uSys)), _allocIndex(0),
          _allocationDifference(0), _doubleDeleteCount(0), _multipleDeleteCount(0)
    { }

    inline ~FixedBlockArenaAllocator() noexcept
    { PageAllocator::decommitPages(_pages, _allocatedPages); }

    [[nodiscard]] inline uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] inline iSys allocationDifference() const noexcept { return _allocationDifference; }

    /**
     * Returns the number of times objects were deleted twice.
     *
     *   This value does not track deallocations more than 2,
     * (ie triple deleting an object only counts increments
     * this value once).
     */
    [[nodiscard]] inline uSys doubleDeleteCount() const noexcept { return _doubleDeleteCount; }

    /**
     * Returns the number of times objects were more than once.
     *
     *   This value tracks any deallocations more than 1. (ie
     * a triple delete will increment this twice).
     */
    [[nodiscard]] inline uSys multipleDeleteCount() const noexcept { return _multipleDeleteCount; }

    [[nodiscard]] inline void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex + sizeof(uSys);
        _allocIndex += _blockSize;
        ++_allocationDifference;
        return ret;
    }

    [[nodiscard]] inline void* allocate(const uSys size) noexcept override { return allocate(); }

    inline void deallocate(void* const obj) noexcept override
    {
        if(!obj)
        { return; }

        --_allocationDifference;
        uSys* const deallocationCount = reinterpret_cast<uSys*>(reinterpret_cast<u8*>(obj) - sizeof(uSys));
        ++(*deallocationCount);
        if(*deallocationCount > 1)
        {
            ++_multipleDeleteCount;
            if(*deallocationCount == 2)
            { ++_doubleDeleteCount; }
        }
    }
private:
    bool assertSize() noexcept
    {
        const uSys pageBytes = _allocatedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_allocatedPages == _NumReservePages)
            { return false; }
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_allocatedPages;
        }

        return true;
    }
};

