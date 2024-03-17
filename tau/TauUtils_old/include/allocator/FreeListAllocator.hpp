#pragma once

#include "Objects.hpp"
#include "TauAllocator.hpp"
#include "PageAllocator.hpp"
#include "ReferenceCountingPointer.hpp"
#include "TUMaths.hpp"

/**
 *   This allocator is intended for use with free lists of
 * reference counting pointers.
 *
 *   More specifically, when you have a set of reference
 * counting pointers (of potentially different types, or
 * pointing to different types) that all need to be released at
 * the same time, you can use this allocator to add each
 * pointer to an array and release them all at the same time,
 * even if they are different types.
 *
 *   This is done by taking advantage of the fact that the
 * reference counting pointers are always the same size, and
 * that they inherit from a non-template base class. Thus we
 * can cast the raw memory to an array of the base class (sort
 * of, the stride would be wrong if we did this literally,
 * instead we treat it as an array of bytes, moving down with a
 * stride equal to the size of a reference counting pointer,
 * and then casting that pointer to the base class) and
 * virtually invoking the destructor.
 */
class FreeListAllocator final : public TauAllocator
{
    DELETE_CM(FreeListAllocator);
private:
    /**
     *   The size of each pointer type should be the same, the only
     * difference is the size of the object it points to.
     */
    static constexpr uSys BlockSize = sizeof(ReferenceCountingPointer<void*>);

    /*
     * Assert ref pointers of different types are the same size.
     */
    static_assert(sizeof(ReferenceCountingPointer<void*>) == sizeof(StrongReferenceCountingPointer<void*>), "Reference Counting Pointer type sizes do not match.");
    static_assert(sizeof(ReferenceCountingPointer<void*>) == sizeof(WeakReferenceCountingPointer<void*>), "Reference Counting Pointer type sizes do not match.");

    /*
     *   Assert ref pointers of different types to different types
     * are the same size.
     */
    static_assert(sizeof(ReferenceCountingPointer<u64>) == sizeof(ReferenceCountingPointer<u32>), "Reference Counting Pointer type sizes do not match.");
    static_assert(sizeof(StrongReferenceCountingPointer<u64>) == sizeof(StrongReferenceCountingPointer<u32>), "Reference Counting Pointer type sizes do not match.");
    static_assert(sizeof(WeakReferenceCountingPointer<u64>) == sizeof(WeakReferenceCountingPointer<u32>), "Reference Counting Pointer type sizes do not match.");
private:
    uSys _allocPages;
    uSys _numReservedPages;
    void* _pages;
    uSys _committedPages;
    uSys _allocIndex;
public:
    FreeListAllocator(const PageCountVal numReservedPages = PageCountVal { 1024 }, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>(numReservedPages), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _allocIndex(0)
    { }

    FreeListAllocator(const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _numReservedPages(_alignTo(static_cast<uSys>((maxElements * BlockSize) / PageAllocator::pageSize() + 1), _allocPages))
        , _pages(PageAllocator::reserve(_numReservedPages))
        , _committedPages(0)
        , _allocIndex(0)
    { }

    ~FreeListAllocator() noexcept override
    {
        /*
         * Invoke the destructor of each pointer using its vtable.
         */
        for(uSys i = 0; i < _allocIndex; i += BlockSize)
        {
            ReferenceCountingPointerBase* const base = reinterpret_cast<ReferenceCountingPointerBase* const>(reinterpret_cast<u8*>(_pages) + i);
            base->~ReferenceCountingPointerBase();
        }
        PageAllocator::free(_pages);
    }

    [[nodiscard]] uSys blockSize() const noexcept { return BlockSize; }

    [[nodiscard]] void* allocate() noexcept
    {
        if(!assertSize())
        { return nullptr; }
        void* const ret = reinterpret_cast<u8*>(_pages) + _allocIndex;
        _allocIndex += BlockSize;
        return ret;
    }

    [[nodiscard]] void* allocate(uSys) noexcept override { return allocate(); }

    void deallocate(void*) noexcept override { }

    void reset(const bool releasePages = false) noexcept
    {
        /*
         * Invoke the destructor of each pointer using its vtable.
         */
        for(uSys i = 0; i < _allocIndex; i += BlockSize)
        {
            ReferenceCountingPointerBase* const base = reinterpret_cast<ReferenceCountingPointerBase* const>(reinterpret_cast<u8*>(_pages) + i);
            base->~ReferenceCountingPointerBase();
        }

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
        if(_allocIndex + BlockSize > pageBytes)
        {
            if(_committedPages == _numReservedPages)
            { return false; }
            (void)PageAllocator::commitPages(reinterpret_cast<u8*>(_pages) + pageBytes, _allocPages);
            _committedPages += _allocPages;
        }

        return true;
    }
};
