// ReSharper disable CppClangTidyClangDiagnosticUninitialized
#pragma once

#include "Objects.hpp"
#include "TauAllocator.hpp"
#include "PageAllocator.hpp"
#include "TUMaths.hpp"

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
    DELETE_CM(FixedBlockAllocator);
public:
    FixedBlockAllocator(uSys blockSize, PageCountVal numReservePages, uSys allocPages) noexcept = delete;
    FixedBlockAllocator(uSys blockSize, uSys maxElements, uSys allocPages) noexcept = delete;
    
    [[nodiscard]] const void* head() const noexcept { return nullptr; }
    [[nodiscard]] uSys reservedPages() const noexcept { return 0; }
    [[nodiscard]] uSys committedPages() const noexcept { return 0; }
    [[nodiscard]] uSys allocIndex() const noexcept { return 0; }
    [[nodiscard]] uSys blockSize() const noexcept { return 0; }
    [[nodiscard]] iSys allocationDifference() const noexcept { return 0; }
    [[nodiscard]] uSys multipleDeleteCount() const noexcept { return 0; }

    [[nodiscard]] void* allocate() noexcept { return nullptr; }
    [[nodiscard]] void* allocate(uSys size) noexcept override { return nullptr; }
    void deallocate(void* obj) noexcept override { }
};

/**
 *   This allocator does not perform any allocation tracking.
 * If you do need to keep track of every allocation you should
 * specify a different AllocationTracking.
 */
template<>
class FixedBlockAllocator<AllocationTracking::None> final : public TauAllocator
{
    DELETE_CM(FixedBlockAllocator);
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    void** _firstFree;
    void** _lastFree;
public:
    FixedBlockAllocator(const uSys blockSize, const PageCountVal numReservedPages = static_cast<PageCountVal>(1024), const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) : blockSize)
        , _allocIndex(0)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
    { }

    FixedBlockAllocator(const uSys blockSize, const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo((maxElements * blockSize) / PageAllocator::pageSize() + 1, _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) : blockSize)
        , _allocIndex(0)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
    { }

    ~FixedBlockAllocator() noexcept override
    { PageAllocator::free(_pages); }

    [[nodiscard]] const void* head() const noexcept { return _pages; }
    [[nodiscard]] uSys reservedPages() const noexcept { return _numReservedPages; }
    [[nodiscard]] uSys committedPages() const noexcept { return _committedPages; }
    [[nodiscard]] uSys allocIndex() const noexcept { return _allocIndex; }
    [[nodiscard]] uSys blockSize() const noexcept { return _blockSize; }

    [[nodiscard]] void* allocate() noexcept
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

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void* obj) noexcept override
    {
        if(!obj)
        { return; }

        // if(reinterpret_cast<u8*>(_pages) + (_allocIndex - _blockSize) == obj)
        // {
        //     --_allocIndex;
        //     attemptRelease();
        //     return;
        // }

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
            (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are `_allocPages` empty pages, release the block of pages.
        const uSys pageBytes = (_committedPages - _allocPages) * PageAllocator::pageSize();
        if(pageBytes - _blockSize <= (PageAllocator::pageSize() * _allocPages))
        {
            (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages -= _allocPages;
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
    DELETE_CM(FixedBlockAllocator);
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    void** _firstFree;
    void** _lastFree;
    iSys _allocationDifference;
public:
    FixedBlockAllocator(const uSys blockSize, const PageCountVal numReservedPages = static_cast<PageCountVal>(1024), const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) : blockSize)
        , _allocIndex(0)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
        , _allocationDifference(0)
    { }

    FixedBlockAllocator(const uSys blockSize, const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo((maxElements* blockSize) / PageAllocator::pageSize() + 1, _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) : blockSize)
        , _allocIndex(0)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
        , _allocationDifference(0)
    { }

    ~FixedBlockAllocator() noexcept override
    { PageAllocator::free(_pages); }
    
    [[nodiscard]] const void* head() const noexcept { return _pages; }
    [[nodiscard]] uSys reservedPages() const noexcept { return _numReservedPages; }
    [[nodiscard]] uSys committedPages() const noexcept { return _committedPages; }
    [[nodiscard]] uSys allocIndex() const noexcept { return _allocIndex; }
    [[nodiscard]] uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] iSys allocationDifference() const noexcept { return _allocationDifference; }

    [[nodiscard]] void* allocate() noexcept
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

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void* obj) noexcept override
    {
        if(!obj)
        { return; }

        --_allocationDifference;

        // if(reinterpret_cast<u8*>(_pages) + (_allocIndex - _blockSize) == obj)
        // {
        //     --_allocIndex;
        //     attemptRelease();
        //     return;
        // }

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
            (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are `_allocPages` empty pages, release the block of pages.
        const uSys pageBytes = (_committedPages - _allocPages) * PageAllocator::pageSize();
        if(pageBytes - _blockSize <= (PageAllocator::pageSize() * _allocPages))
        {
            (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages -= _allocPages;
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
    DELETE_CM(FixedBlockAllocator);
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    void** _firstFree;
    void** _lastFree;
    iSys _allocationDifference;
    uSys _doubleDeleteCount;
    uSys _multipleDeleteCount;
public:
    FixedBlockAllocator(const uSys blockSize, const PageCountVal numReservedPages = static_cast<PageCountVal>(1024), const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) + sizeof(uSys) : blockSize + sizeof(uSys))
        , _allocIndex(0)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
        , _allocationDifference(0)
        , _doubleDeleteCount(0)
        , _multipleDeleteCount(0)
    { }

    FixedBlockAllocator(const uSys blockSize, const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo((maxElements* blockSize) / PageAllocator::pageSize() + 1, _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize < sizeof(void*) ? sizeof(void*) + sizeof(uSys) : blockSize + sizeof(uSys))
        , _allocIndex(0)
        , _firstFree(nullptr)
        , _lastFree(nullptr)
        , _allocationDifference(0)
        , _doubleDeleteCount(0)
        , _multipleDeleteCount(0)
    { }

    ~FixedBlockAllocator() noexcept override
    { PageAllocator::free(_pages); }
    
    [[nodiscard]] const void* head() const noexcept { return _pages; }
    [[nodiscard]] uSys reservedPages() const noexcept { return _numReservedPages; }
    [[nodiscard]] uSys committedPages() const noexcept { return _committedPages; }
    [[nodiscard]] uSys allocIndex() const noexcept { return _allocIndex; }
    [[nodiscard]] uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] iSys allocationDifference() const noexcept { return _allocationDifference; }

    /**
     * Returns the number of times objects were deleted twice.
     *
     *   This value does not track deallocations more than 2,
     * (ie triple deleting an object only counts increments
     * this value once).
     */
    [[nodiscard]] uSys doubleDeleteCount() const noexcept { return _doubleDeleteCount; }

    /**
     * Returns the number of times objects were more than once.
     *
     *   This value tracks any deallocations more than 1. (ie
     * a triple delete will increment this twice).
     */
    [[nodiscard]] uSys multipleDeleteCount() const noexcept { return _multipleDeleteCount; }

    [[nodiscard]] void* allocate() noexcept
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

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void* obj) noexcept override
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

        // if(reinterpret_cast<u8*>(_pages) + (_allocIndex - _blockSize) == obj)
        // {
        //     --_allocIndex;
        //     attemptRelease();
        //     return;
        // }

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
            (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are `_allocPages` empty pages, release the block of pages.
        const uSys pageBytes = (_committedPages - _allocPages) * PageAllocator::pageSize();
        if(pageBytes - _blockSize <= (PageAllocator::pageSize() * _allocPages))
        {
            (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages -= _allocPages;
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
    DELETE_CM(FixedBlockArenaAllocator);
public:
    FixedBlockArenaAllocator(uSys blockSize, PageCountVal numReservePages, uSys allocPages) noexcept = delete;
    FixedBlockArenaAllocator(uSys blockSize, uSys maxElements, uSys allocPages) noexcept = delete;
    
    [[nodiscard]] const void* head() const noexcept { return nullptr; }
    [[nodiscard]] uSys reservedPages() const noexcept { return 0; }
    [[nodiscard]] uSys committedPages() const noexcept { return 0; }
    [[nodiscard]] uSys allocIndex() const noexcept { return 0; }
    [[nodiscard]] uSys blockSize() const noexcept { return 0; }
    [[nodiscard]] iSys allocationDifference() const noexcept { return 0; }
    [[nodiscard]] uSys multipleDeleteCount() const noexcept { return 0; }

    [[nodiscard]] void* allocate() noexcept { return nullptr; }
    [[nodiscard]] void* allocate(uSys size) noexcept override { return nullptr; }
    void deallocate(void* obj) noexcept override { }
};

/**
 *   This allocator does not perform any allocation tracking.
 * If you do need to keep track of every allocation you should
 * specify a different AllocationTracking.
 */
template<>
class FixedBlockArenaAllocator<AllocationTracking::None> final : public TauAllocator
{
    DELETE_CM(FixedBlockArenaAllocator);
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
public:
    FixedBlockArenaAllocator(const uSys blockSize, const PageCountVal numReservedPages = static_cast<PageCountVal>(1024), const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize)
        , _allocIndex(0)
    { }

    FixedBlockArenaAllocator(const uSys blockSize, const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo((maxElements* blockSize) / PageAllocator::pageSize() + 1, _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize)
        , _allocIndex(0)
    { }

    ~FixedBlockArenaAllocator() noexcept override
    { PageAllocator::free(_pages); }
    
    [[nodiscard]] const void* head() const noexcept { return _pages; }
    [[nodiscard]] uSys reservedPages() const noexcept { return _numReservedPages; }
    [[nodiscard]] uSys committedPages() const noexcept { return _committedPages; }
    [[nodiscard]] uSys allocIndex() const noexcept { return _allocIndex; }
    [[nodiscard]] uSys blockSize() const noexcept { return _blockSize; }

    [[nodiscard]] void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        return ret;
    }

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void*) noexcept override { }

    void reset(const bool releasePages = false) noexcept
    {
        _allocIndex = 0;
        if(releasePages)
        {
            PageAllocator::decommitPages(reinterpret_cast<u8*>(_pages) + _allocPages * PageAllocator::pageSize(), _committedPages - _allocPages);
            _committedPages = _allocPages;
        }
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _committedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
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
    DELETE_CM(FixedBlockArenaAllocator);
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    iSys _allocationDifference;
public:
    FixedBlockArenaAllocator(const uSys blockSize, const PageCountVal numReservedPages = static_cast<PageCountVal>(1024), const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize)
        , _allocIndex(0)
        , _allocationDifference(0)
    { }

    FixedBlockArenaAllocator(const uSys blockSize, const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo((maxElements* blockSize) / PageAllocator::pageSize() + 1, _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize)
        , _allocIndex(0)
        , _allocationDifference(0)
    { }

    ~FixedBlockArenaAllocator() noexcept override
    { PageAllocator::free(_pages); }
    
    [[nodiscard]] const void* head() const noexcept { return _pages; }
    [[nodiscard]] uSys reservedPages() const noexcept { return _numReservedPages; }
    [[nodiscard]] uSys committedPages() const noexcept { return _committedPages; }
    [[nodiscard]] uSys allocIndex() const noexcept { return _allocIndex; }
    [[nodiscard]] uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] iSys allocationDifference() const noexcept { return _allocationDifference; }

    [[nodiscard]] void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += _blockSize;
        ++_allocationDifference;
        return ret;
    }

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void* const obj) noexcept override
    {
        if(!obj)
        { return; }
        --_allocationDifference;
    }

    void reset(const bool releasePages = false) noexcept
    {
        _allocIndex = 0;
        _allocationDifference = 0;
        if(releasePages)
        {
            PageAllocator::decommitPages(reinterpret_cast<u8*>(_pages) + _allocPages * PageAllocator::pageSize(), _committedPages - _allocPages);
            _committedPages = _allocPages;
        }
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _committedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
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
    DELETE_CM(FixedBlockArenaAllocator);
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _blockSize;
    uSys _allocIndex;
    iSys _allocationDifference;
    uSys _doubleDeleteCount;
    uSys _multipleDeleteCount;
public:
    FixedBlockArenaAllocator(const uSys blockSize, const PageCountVal numReservedPages = static_cast<PageCountVal>(1024), const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize + sizeof(uSys))
        , _allocIndex(0)
        , _allocationDifference(0)
        , _doubleDeleteCount(0)
        , _multipleDeleteCount(0)
    { }

    FixedBlockArenaAllocator(const uSys blockSize, const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo((maxElements* blockSize) / PageAllocator::pageSize() + 1, _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _blockSize(blockSize + sizeof(uSys))
        , _allocIndex(0)
        , _allocationDifference(0)
        , _doubleDeleteCount(0)
        , _multipleDeleteCount(0)
    { }

    ~FixedBlockArenaAllocator() noexcept override
    { PageAllocator::free(_pages); }
    
    [[nodiscard]] const void* head() const noexcept { return _pages; }
    [[nodiscard]] uSys reservedPages() const noexcept { return _numReservedPages; }
    [[nodiscard]] uSys committedPages() const noexcept { return _committedPages; }
    [[nodiscard]] uSys allocIndex() const noexcept { return _allocIndex; }
    [[nodiscard]] uSys blockSize() const noexcept { return _blockSize; }

    /**
     *   Returns the difference in the number of allocations vs
     * the number of deallocations.
     */
    [[nodiscard]] iSys allocationDifference() const noexcept { return _allocationDifference; }

    /**
     * Returns the number of times objects were deleted twice.
     *
     *   This value does not track deallocations more than 2,
     * (ie triple deleting an object only counts increments
     * this value once).
     */
    [[nodiscard]] uSys doubleDeleteCount() const noexcept { return _doubleDeleteCount; }

    /**
     * Returns the number of times objects were more than once.
     *
     *   This value tracks any deallocations more than 1. (ie
     * a triple delete will increment this twice).
     */
    [[nodiscard]] uSys multipleDeleteCount() const noexcept { return _multipleDeleteCount; }

    [[nodiscard]] void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex + sizeof(uSys);
        _allocIndex += _blockSize;
        ++_allocationDifference;
        return ret;
    }

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void* const obj) noexcept override
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

    void reset(const bool releasePages = false) noexcept
    {
        _allocIndex = 0;
        _allocationDifference = 0;
        _multipleDeleteCount = 0;
        _doubleDeleteCount = 0;
        if(releasePages)
        {
            PageAllocator::decommitPages(reinterpret_cast<u8*>(_pages) + _allocPages * PageAllocator::pageSize(), _committedPages - _allocPages);
            _committedPages = _allocPages;
        }
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        const uSys pageBytes = _committedPages * PageAllocator::pageSize();
        if(_allocIndex + _blockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
        }

        return true;
    }
};
