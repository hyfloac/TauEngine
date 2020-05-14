#pragma once

#include "Objects.hpp"
#include "TauAllocator.hpp"
#include "PageAllocator.hpp"

/**
 * An allocator which only allocates a fixed size block.
 *
 *   Unlike the FixedBlockArenaAllocator it is possible to free
 * the blocks once you are done using them.
 *
 *   You should be quite liberal with the number of reserved
 * pages. Reserving more pages doesn't have any effect on the
 * memory consumption of the application, it only affects the
 * available memory space of your program, ie if you reserve a
 * gigabyte of data your computers memory usage is not going to
 * spike up suddenly, but 1 gigabyte of memory will be reserved for
 * future use, and your program will not be able to allocate from
 * that block any longer. The pages will only be truly allocated
 * when they are first used, and the allocator only commits them
 * when needed. A page _should_ be 4 KiB, but that isn't
 * necessarily guaranteed. The formal way to get the page size is
 * to call `PageAllocator::pageSize()`.
 *
 *   That being said, don't go too large as it may end up
 * consuming the entire potential address space of your
 * program.
 */
template<AllocationTracking _AllocTracking = AllocationTracking::None>
class FixedBlockAllocator final : public TauAllocator
{
    DELETE_CONSTRUCT(FixedBlockAllocator);
    DEFAULT_DESTRUCT(FixedBlockAllocator);
    DELETE_COPY(FixedBlockAllocator);
public:
    FixedBlockAllocator(uSys blockSize, uSys numReservePages) noexcept = delete;

    [[nodiscard]] inline uSys blockSize() const noexcept { return 0; }
    [[nodiscard]] inline iSys allocationDifference() const noexcept { return 0; }
    [[nodiscard]] inline uSys multipleDeleteCount() const noexcept { return 0; }

    [[nodiscard]] inline void* allocate() noexcept { return nullptr; }
    [[nodiscard]] inline void* allocate(uSys size) noexcept override { return nullptr; }
    inline void deallocate(void* obj) noexcept override { }
};

/**
 *   This allocator does not perform any allocation tracking.
 * If you do need to keep track of every allocation you should
 * specify a different AllocationTracking.
 */
template<>
class FixedBlockAllocator<AllocationTracking::None> final : public TauAllocator
{
    DELETE_COPY(FixedBlockAllocator);
private:
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    uSys _numReservedPages;
    void** _firstFree;
    void** _lastFree;
public:
    inline FixedBlockAllocator(const uSys blockSize, const uSys numReservedPages = 1024) noexcept
        : _pages(PageAllocator::reserve(numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) : blockSize)
        , _allocIndex(0)
        , _numReservedPages(numReservedPages)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
    { }

    inline ~FixedBlockAllocator() noexcept
    { PageAllocator::free(_pages); }

    [[nodiscard]] inline uSys blockSize() const noexcept { return _blockSize; }

    [[nodiscard]] inline void* allocate() noexcept
    {
        if(_firstFree)
        {
            void* const ret = _firstFree;
            if(_firstFree == _lastFree)
            {
                _firstFree = nullptr;
                _lastFree = nullptr;
            }
            else
            {
                _firstFree = reinterpret_cast<void**>(*_firstFree);
            }
            return ret;
        }

        if(!assertSize())
        { return nullptr; }

        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        return ret;
    }

    [[nodiscard]] inline void* allocate(uSys) noexcept override { return allocate(); }

    inline void deallocate(void* obj) noexcept override
    {
        if(!obj)
        { return; }

        if(reinterpret_cast<u8*>(_pages) + (_allocIndex - _blockSize) == obj)
        {
            --_allocIndex;
            attemptRelease();
            return;
        }

        if(_lastFree)
        {
            *_lastFree = obj;
            _lastFree = reinterpret_cast<void**>(obj);
            return;
        }

        _firstFree = reinterpret_cast<void**>(obj);
        _lastFree = reinterpret_cast<void**>(obj);
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _committedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void)PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_committedPages;
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are two empty pages, release the last page.
        const uSys pageBytes = (_committedPages - 1) * PageAllocator::pageSize();
        if(pageBytes - _blockSize <= PageAllocator::pageSize())
        {
            (void)PageAllocator::decommitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            --_committedPages;
        }
    }
};

/**
 *   This allocator keeps track of how many allocations there
 * are compared to how many deallocations there are. If these
 * two don't match you likely have a bug in your code
 * somewhere.
 */
template<>
class FixedBlockAllocator<AllocationTracking::Count> final : public TauAllocator
{
    DELETE_COPY(FixedBlockAllocator);
private:
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    uSys _numReservedPages;
    void** _firstFree;
    void** _lastFree;
    iSys _allocationDifference;
public:
    inline FixedBlockAllocator(const uSys blockSize, const uSys numReservedPages = 1024) noexcept
        : _pages(PageAllocator::reserve(numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) : blockSize)
        , _allocIndex(0)
        , _numReservedPages(numReservedPages)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
        , _allocationDifference(0)
    { }

    inline ~FixedBlockAllocator() noexcept
    { PageAllocator::free(_pages); }

    [[nodiscard]] inline uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] inline iSys allocationDifference() const noexcept { return _allocationDifference; }

    [[nodiscard]] inline void* allocate() noexcept
    {
        if(_firstFree)
        {
            void* const ret = _firstFree;
            if(_firstFree == _lastFree)
            {
                _firstFree = nullptr;
                _lastFree = nullptr;
            }
            else
            {
                _firstFree = reinterpret_cast<void**>(*_firstFree);
            }

            ++_allocationDifference;

            return ret;
        }

        if(!assertSize())
        { return nullptr; }

        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        ++_allocationDifference;
        return ret;
    }

    [[nodiscard]] inline void* allocate(uSys) noexcept override { return allocate(); }

    inline void deallocate(void* obj) noexcept override
    {
        if(!obj)
        { return; }

        --_allocationDifference;

        if(reinterpret_cast<u8*>(_pages) + (_allocIndex - _blockSize) == obj)
        {
            --_allocIndex;
            attemptRelease();
            return;
        }

        if(_lastFree)
        {
            *_lastFree = obj;
            _lastFree = reinterpret_cast<void**>(obj);
            return;
        }

        _firstFree = reinterpret_cast<void**>(obj);
        _lastFree = reinterpret_cast<void**>(obj);
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _committedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void)PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_committedPages;
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are two empty pages, release the last page.
        const uSys pageBytes = (_committedPages - 1) * PageAllocator::pageSize();
        if(pageBytes - _blockSize <= PageAllocator::pageSize())
        {
            (void)PageAllocator::decommitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            --_committedPages;
        }
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
template<>
class FixedBlockAllocator<AllocationTracking::DoubleDeleteCount> final : public TauAllocator
{
    DELETE_COPY(FixedBlockAllocator);
private:
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    uSys _numReservedPages;
    void** _firstFree;
    void** _lastFree;
    iSys _allocationDifference;
    uSys _doubleDeleteCount;
    uSys _multipleDeleteCount;
public:
    inline FixedBlockAllocator(const uSys blockSize, const uSys numReservedPages = 1024) noexcept
        : _pages(PageAllocator::reserve(numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) + sizeof(uSys) : blockSize + sizeof(uSys))
        , _allocIndex(0)
        , _numReservedPages(numReservedPages)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
        , _allocationDifference(0)
        , _doubleDeleteCount(0)
        , _multipleDeleteCount(0)
    { }

    inline ~FixedBlockAllocator() noexcept
    { PageAllocator::free(_pages); }

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
        if(_firstFree)
        {
            void* const ret = _firstFree;
            if(_firstFree == _lastFree)
            {
                _firstFree = nullptr;
                _lastFree = nullptr;
            }
            else
            {
                _firstFree = reinterpret_cast<void**>(*_firstFree);
            }

            ++_allocationDifference;

            // Reset multiple delete count.
            uSys* const deleteCount = reinterpret_cast<uSys*>(ret) - 1;
            *deleteCount = 0;

            return ret;
        }

        if(!assertSize())
        { return nullptr; }

        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex + sizeof(uSys);
        _allocIndex += _blockSize;
        ++_allocationDifference;
        return ret;
    }

    [[nodiscard]] inline void* allocate(uSys) noexcept override { return allocate(); }

    inline void deallocate(void* obj) noexcept override
    {
        if(!obj)
        { return; }

        --_allocationDifference;
        uSys* const deallocationCount = reinterpret_cast<uSys*>(obj) - 1;
        ++(*deallocationCount);
        if(*deallocationCount > 1)
        {
            ++_multipleDeleteCount;
            if(*deallocationCount == 2)
            { ++_doubleDeleteCount; }
            return;
        }

        if(reinterpret_cast<u8*>(_pages) + (_allocIndex - _blockSize) == obj)
        {
            --_allocIndex;
            attemptRelease();
            return;
        }

        if(_lastFree)
        {
            *_lastFree = obj;
            _lastFree = reinterpret_cast<void**>(obj);
            return;
        }

        _firstFree = reinterpret_cast<void**>(obj);
        _lastFree = reinterpret_cast<void**>(obj);
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _committedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void)PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_committedPages;
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are two empty pages, release the last page.
        const uSys pageBytes = (_committedPages - 1) * PageAllocator::pageSize();
        if(pageBytes - _blockSize <= PageAllocator::pageSize())
        {
            (void)PageAllocator::decommitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            --_committedPages;
        }
    }
};

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
template<AllocationTracking _AllocTracking = AllocationTracking::None>
class FixedBlockArenaAllocator final : public TauAllocator
{
    DELETE_CONSTRUCT(FixedBlockArenaAllocator);
    DEFAULT_DESTRUCT(FixedBlockArenaAllocator);
    DELETE_COPY(FixedBlockArenaAllocator);
public:
    FixedBlockArenaAllocator(uSys blockSize, uSys numReservePages = 1024) noexcept = delete;

    [[nodiscard]] inline uSys blockSize() const noexcept { return 0; }
    [[nodiscard]] inline iSys allocationDifference() const noexcept { return 0; }
    [[nodiscard]] inline uSys multipleDeleteCount() const noexcept { return 0; }

    [[nodiscard]] inline void* allocate() noexcept { return nullptr; }
    [[nodiscard]] inline void* allocate(uSys size) noexcept override { return nullptr; }
    inline void deallocate(void* obj) noexcept override { }
};

/**
 *   This allocator does not perform any allocation tracking.
 * If you do need to keep track of every allocation you should
 * specify a different AllocationTracking.
 */
template<>
class FixedBlockArenaAllocator<AllocationTracking::None> final : public TauAllocator
{
    DELETE_COPY(FixedBlockArenaAllocator);
private:
    void* _pages;
    uSys _allocatedPages;
    uSys _blockSize;
    uSys _allocIndex;
    uSys _numReservedPages;
public:
    inline FixedBlockArenaAllocator(const uSys blockSize, const uSys numReservedPages = 1024) noexcept
        : _pages(PageAllocator::reserve(numReservedPages))
        , _allocatedPages(0)
        , _blockSize(blockSize)
        , _allocIndex(0)
        , _numReservedPages(numReservedPages)
    { }

    inline ~FixedBlockArenaAllocator() noexcept
    { PageAllocator::free(_pages); }

    [[nodiscard]] inline uSys blockSize() const noexcept { return _blockSize; }

    [[nodiscard]] inline void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        return ret;
    }

    [[nodiscard]] inline void* allocate(uSys) noexcept override { return allocate(); }

    inline void deallocate(void*) noexcept override { }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _allocatedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_allocatedPages == _numReservedPages)
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
template<>
class FixedBlockArenaAllocator<AllocationTracking::Count> final : public TauAllocator
{
    DELETE_COPY(FixedBlockArenaAllocator);
private:
    void* _pages;
    uSys _allocatedPages;
    uSys _blockSize;
    uSys _allocIndex;
    uSys _numReservedPages;
    iSys _allocationDifference;
public:
    inline FixedBlockArenaAllocator(const uSys blockSize, const uSys numReservedPages = 1024) noexcept
        : _pages(PageAllocator::reserve(numReservedPages))
        , _allocatedPages(0)
        , _blockSize(blockSize)
        , _allocIndex(0)
        , _numReservedPages(numReservedPages)
        , _allocationDifference(0)
    { }

    inline ~FixedBlockArenaAllocator() noexcept
    { PageAllocator::free(_pages); }

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

    [[nodiscard]] inline void* allocate(uSys) noexcept override { return allocate(); }

    inline void deallocate(void* const obj) noexcept override
    {
        if(!obj)
        { return; }
        --_allocationDifference;
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _allocatedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_allocatedPages == _numReservedPages)
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
template<>
class FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount> final : public TauAllocator
{
    DELETE_COPY(FixedBlockArenaAllocator);
private:
    void* _pages;
    uSys _allocatedPages;
    uSys _blockSize;
    uSys _allocIndex;
    uSys _numReservedPages;
    iSys _allocationDifference;
    uSys _doubleDeleteCount;
    uSys _multipleDeleteCount;
public:
    inline FixedBlockArenaAllocator(const uSys blockSize, const uSys numReservedPages = 1024) noexcept
        : _pages(PageAllocator::reserve(numReservedPages))
        , _allocatedPages(0)
        , _blockSize(blockSize + sizeof(uSys))
        , _allocIndex(0)
        , _numReservedPages(numReservedPages)
        , _allocationDifference(0)
        , _doubleDeleteCount(0)
        , _multipleDeleteCount(0)
    { }

    inline ~FixedBlockArenaAllocator() noexcept
    { PageAllocator::free(_pages); }

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

    [[nodiscard]] inline void* allocate(uSys) noexcept override { return allocate(); }

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
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _allocatedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_allocatedPages == _numReservedPages)
            { return false; }
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_pages) + pageBytes);
            ++_allocatedPages;
        }

        return true;
    }
};
