#pragma once

#pragma warning(push, 0)
#include <cstring>
#include <utility>
#pragma warning(pop)

#include "NumTypes.hpp"
#include "allocator/PageAllocator.hpp"
#include "allocator/TauAllocator.hpp"

namespace _ArrayListUtils
{
/**
 * The control block for an array list.
 *
 * @element refCount
 *     The number of references to this arraylist. because
 *   there is reference counting you cannot implicitly copy the
 *   entire array. This is also used with the iterators to
 *   ensure the data is not freed before iteration is complete.
 * @element elementCount
 *     The number of elements in the array.
 * @element dataSize
 *     The total size in bytes of usage. This is of the data
 *   array as well as the control block. This is because they
 *   are contained in the same block of memory to decrease the
 *   number of required allocations.
 * @element numReservedPages
 *     The total number of pages that have been reserved. The
 *   allocator takes advantage of VirtualAlloc to reduce memory
 *   and computation overhead. This is done by reserving a
 *   number pages from system memory. Reserving pages doesn't
 *   actually consume any memory, it simply reserves a
 *   contiguous series of addresses in your program. This
 *   allows us to expand the array by commiting pages, instead
 *   of the classic reallocate and copy strategy. This means
 *   we can be more conservative with the number of allocations
 *   we perform, and eliminate the need for unnecessary data
 *   copying.
 * @element committedPages
 *     This is the number of reserved pages that have been
 *    committed. A committed page is one where the address
 *    space correlates to a real block of RAM.
 */
struct _CtrlBlockData
{
    uSys refCount;
    uSys elementCount;
    uSys dataSize;
    uSys committedPages;
};

struct ControlBlock final
{
    union
    {
        _CtrlBlockData data;
        char _alignment[64];
    };
};
}

template<typename _T>
class ArrayListIterator final
{
public:
    using ControlBlock = _ArrayListUtils::ControlBlock;
private:
    ControlBlock* _ctrlBlock;
    _T* _arr;
    uSys _index;
public:
    inline ArrayListIterator(ControlBlock* const ctrlBlock, _T* const arr, const uSys index) noexcept
        : _ctrlBlock(ctrlBlock)
        , _arr(arr)
        , _index(index)
    { ++_ctrlBlock->data.refCount; }

    inline ~ArrayListIterator() noexcept
    {
        if(_ctrlBlock && --_ctrlBlock->data.refCount == 0)
        { PageAllocator::free(_ctrlBlock); }
    }

    inline ArrayListIterator(const ArrayListIterator<_T>& copy) noexcept
        : _ctrlBlock(copy._ctrlBlock)
        , _arr(copy._arr)
        , _index(copy._index)
    { ++_ctrlBlock->data.refCount; }

    inline ArrayListIterator(ArrayListIterator<_T>&& move) noexcept
        : _ctrlBlock(move._ctrlBlock)
        , _arr(move._arr)
        , _index(move._index)
    { move._ctrlBlock = nullptr; }

    inline ArrayListIterator<_T>& operator =(const ArrayListIterator<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        _ctrlBlock = copy._ctrlBlock;
        _arr = copy._arr;
        _index = copy._index;

        ++_ctrlBlock->data.refCount;

        return *this;
    }

    inline ArrayListIterator<_T>& operator =(ArrayListIterator<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        _ctrlBlock = move._ctrlBlock;
        _arr = move._arr;
        _index = move._index;

        move._ctrlBlock = nullptr;

        return *this;
    }

    inline ArrayListIterator<_T>& operator++() noexcept
    {
        if(_index < _ctrlBlock->data.elementCount)
        { ++_index; }
        return *this;
    }

    inline ArrayListIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline ArrayListIterator<_T> operator++(int) noexcept
    {
        const ArrayListIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline ArrayListIterator<_T> operator--(int) noexcept
    {
        const ArrayListIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline uSys index() const noexcept { return _index; }

    [[nodiscard]] inline       _T& operator*()       noexcept { return _arr[_index]; }
    [[nodiscard]] inline const _T& operator*() const noexcept { return _arr[_index]; }

    [[nodiscard]] inline bool operator ==(const ArrayListIterator<_T> & other) const noexcept { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const ArrayListIterator<_T> & other) const noexcept { return _index != other._index; }
};

template<typename _T>
class ConstArrayListIterator final
{
public:
    using ControlBlock = _ArrayListUtils::ControlBlock;
private:
    ControlBlock* _ctrlBlock;
    const _T* _arr;
    uSys _index;
public:
    inline ConstArrayListIterator(ControlBlock* const ctrlBlock, const _T* const arr, const uSys index) noexcept
        : _ctrlBlock(ctrlBlock)
        , _arr(arr)
        , _index(index)
    { ++_ctrlBlock->data.refCount; }

    inline ~ConstArrayListIterator() noexcept
    {
        if(_ctrlBlock && --_ctrlBlock->data.refCount == 0)
        { PageAllocator::free(_ctrlBlock); }
    }

    inline ConstArrayListIterator(const ConstArrayListIterator<_T>& copy) noexcept
        : _ctrlBlock(copy._ctrlBlock)
        , _arr(copy._arr)
        , _index(copy._index)
    { ++_ctrlBlock->data.refCount; }

    inline ConstArrayListIterator(ConstArrayListIterator<_T>&& move) noexcept
        : _ctrlBlock(move._ctrlBlock)
        , _arr(move._arr)
        , _index(move._index)
    { move._ctrlBlock = nullptr; }

    inline ConstArrayListIterator<_T>& operator =(const ConstArrayListIterator<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--_ctrlBlock->data.refCount == 0)
        { PageAllocator::free(_ctrlBlock); }

        _ctrlBlock = copy._ctrlBlock;
        _arr = copy._arr;
        _index = copy._index;

        ++_ctrlBlock->data.refCount;

        return *this;
    }

    inline ConstArrayListIterator<_T>& operator =(ConstArrayListIterator<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--_ctrlBlock->data.refCount == 0)
        { PageAllocator::free(_ctrlBlock); }

        _ctrlBlock = move._ctrlBlock;
        _arr = move._arr;
        _index = move._index;

        move._ctrlBlock = nullptr;

        return *this;
    }

    inline ConstArrayListIterator<_T>& operator++() noexcept
    {
        if(_index < _ctrlBlock->data.elementCount)
        { ++_index; }
        return *this;
    }

    inline ConstArrayListIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline ConstArrayListIterator<_T> operator++(int) noexcept
    {
        const ArrayListIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline ConstArrayListIterator<_T> operator--(int) noexcept
    {
        const ArrayListIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline uSys index() const noexcept { return _index; }

    [[nodiscard]] inline const _T& operator*() const noexcept { return _arr[_index]; }

    [[nodiscard]] inline bool operator ==(const ConstArrayListIterator<_T>& other) const noexcept { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const ConstArrayListIterator<_T>& other) const noexcept { return _index != other._index; }
};

enum class ALMoveMethod
{
    MemCopy = 0,
    MoveConstruct
};

template<typename _T, ALMoveMethod _MoveMethod = ALMoveMethod::MemCopy>
class ArrayList final { };

template<typename _T>
class ArrayList<_T, ALMoveMethod::MemCopy> final
{
public:
    using ControlBlock = _ArrayListUtils::ControlBlock;
private:
    ControlBlock* _ctrlBlock;
    _T* _arr;
public:
    ArrayList(const uSys maxElements) noexcept
        : _ctrlBlock(reinterpret_cast<ControlBlock*>(PageAllocator::reserve((maxElements * sizeof(_T)) / PageAllocator::pageSize() + 1)))
        , _arr(reinterpret_cast<_T*>(_ctrlBlock + 1))
    {
        (void) PageAllocator::commitPage(_ctrlBlock);
        _ctrlBlock->data.refCount = 1;
        _ctrlBlock->data.elementCount = 0;
        _ctrlBlock->data.dataSize = sizeof(ControlBlock);
        _ctrlBlock->data.committedPages = 1;
    }

    ~ArrayList() noexcept
    {
        if(_ctrlBlock && --_ctrlBlock->data.refCount == 0)
        {
            for(uSys i = 0; i < _ctrlBlock->data.elementCount; ++i)
            {
                _arr[i].~_T();
            }

            PageAllocator::free(_ctrlBlock);
        }
    }

    ArrayList(const ArrayList<_T, ALMoveMethod::MemCopy>& copy) noexcept
        : _ctrlBlock(copy._ctrlBlock)
        , _arr(copy._arr)
    { ++_ctrlBlock->data.refCount; }

    ArrayList(ArrayList<_T, ALMoveMethod::MemCopy>&& move) noexcept
        : _ctrlBlock(move._ctrlBlock)
        , _arr(move._arr)
    { move._ctrlBlock = nullptr; }

    ArrayList<_T, ALMoveMethod::MemCopy>& operator=(const ArrayList<_T, ALMoveMethod::MemCopy>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        _ctrlBlock = copy._ctrlBlock;
        _arr = copy._arr;

        ++_ctrlBlock->data.refCount;

        return *this;
    }

    ArrayList<_T, ALMoveMethod::MemCopy>& operator=(ArrayList<_T, ALMoveMethod::MemCopy>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        _ctrlBlock = move._ctrlBlock;
        _arr = move._arr;

        move._ctrlBlock = nullptr;

        return *this;
    }
    
    [[nodiscard]]       _T* arr()       noexcept { return _arr; }
    [[nodiscard]] const _T* arr() const noexcept { return _arr; }
    [[nodiscard]] uSys  count() const noexcept { return _ctrlBlock->data.elementCount; }
    [[nodiscard]] uSys   size() const noexcept { return _ctrlBlock->data.elementCount; }
    [[nodiscard]] uSys length() const noexcept { return _ctrlBlock->data.elementCount; }

    void add(const _T& val) noexcept
    {
        assertSize();
        _arr[_ctrlBlock->data.elementCount++] = val;
        _ctrlBlock->data.dataSize += sizeof(_T);
    }

    template<typename... _Args>
    _T& emplace(_Args&&... args) noexcept
    {
        assertSize();
        void* const placement = _arr + _ctrlBlock->data.elementCount;
        _T* const ret = new(placement) _T(_TauAllocatorUtils::_forward<_Args>(args)...);
        ++_ctrlBlock->data.elementCount;
        _ctrlBlock->data.dataSize += sizeof(_T);

        return *ret;
    }

    [[nodiscard]] _T* placement(uSys* index = nullptr) noexcept
    {
        assertSize();
        _T* const placementBlock = _arr + _ctrlBlock->data.elementCount;
        if(index)
        { *index = _ctrlBlock->data.elementCount; }
        ++_ctrlBlock->data.elementCount;
        _ctrlBlock->data.dataSize += sizeof(_T);
        return placementBlock;
    }

    void removeFast(const uSys index) noexcept
    {
        if(index + 1 > _ctrlBlock->data.elementCount)
        { return; }

        _arr[index].~_T();

        --_ctrlBlock->data.elementCount;
        if(index == _ctrlBlock->data.elementCount)
        { return; }

        (void) std::memcpy(_arr + index, _arr + _ctrlBlock->data.elementCount, sizeof(_T));
        _ctrlBlock->data.dataSize -= sizeof(_T);

        attemptRelease();
    }

    void remove(const uSys index) noexcept
    {
        if(index + 1 > _ctrlBlock->data.elementCount)
        { return; }

        _arr[index].~_T();

        --_ctrlBlock->data.elementCount;
        if(index == _ctrlBlock->data.elementCount)
        { return; }

        (void) std::memmove(_arr + index, _arr + index + 1, (_ctrlBlock->data.elementCount - index) * sizeof(_T));
        _ctrlBlock->data.dataSize -= sizeof(_T);

        attemptRelease();
    }

    void clear(const bool releasePages = true) noexcept
    {
        for(uSys i = 0; i < _ctrlBlock->data.elementCount; ++i)
        {
            _arr[i].~_T();
        }

        _ctrlBlock->data.dataSize = sizeof(ControlBlock);
        _ctrlBlock->data.elementCount = 0;

        if(releasePages)
        {
            PageAllocator::decommitPages(_ctrlBlock + PageAllocator::pageSize(), _ctrlBlock->data.committedPages - 1);
        }
    }

    [[nodiscard]] _T* at(const uSys index) noexcept
    {
        if(index >= _ctrlBlock->data.elementCount)
        { return nullptr; }

        return _arr[index];
    }

    [[nodiscard]] const _T* at(const uSys index) const noexcept
    {
        if(index >= _ctrlBlock->data.elementCount)
        { return nullptr; }

        return &_arr[index];
    }

    [[nodiscard]]       _T* get(const uSys index)       noexcept { return at(index); }
    [[nodiscard]] const _T* get(const uSys index) const noexcept { return at(index); }

    [[nodiscard]]       _T& operator[](const uSys index)       noexcept { return _arr[index]; }
    [[nodiscard]] const _T& operator[](const uSys index) const noexcept { return _arr[index]; }

    [[nodiscard]] inline ArrayListIterator<_T> begin() noexcept { return ArrayListIterator<_T>(_ctrlBlock, _arr, 0);                        }
    [[nodiscard]] inline ArrayListIterator<_T>   end() noexcept { return ArrayListIterator<_T>(_ctrlBlock, _arr, _ctrlBlock->data.elementCount); }

    [[nodiscard]] inline ConstArrayListIterator<_T> begin() const noexcept { return ConstArrayListIterator<_T>(_ctrlBlock, _arr, 0);                        }
    [[nodiscard]] inline ConstArrayListIterator<_T>   end() const noexcept { return ConstArrayListIterator<_T>(_ctrlBlock, _arr, _ctrlBlock->data.elementCount); }
private:
    void assertSize() noexcept
    {
        const uSys pageBytes = _ctrlBlock->data.committedPages * PageAllocator::pageSize();
        if(_ctrlBlock->data.dataSize + sizeof(_T) >= pageBytes)
        {
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_ctrlBlock) + pageBytes);
            ++_ctrlBlock->data.committedPages;
        }
    }

    void attemptRelease() noexcept
    {
        // If there are two empty pages, release the last page.
        const uSys pageBytes = (_ctrlBlock->data.committedPages - 1) * PageAllocator::pageSize();
        if(pageBytes - _ctrlBlock->data.dataSize <= PageAllocator::pageSize())
        {
            (void) PageAllocator::decommitPage(reinterpret_cast<u8*>(_ctrlBlock) + pageBytes);
            --_ctrlBlock->data.committedPages;
        }
    }
};

template<typename _T>
class ArrayList<_T, ALMoveMethod::MoveConstruct> final
{
public:
    using ControlBlock = _ArrayListUtils::ControlBlock;
private:
    ControlBlock* _ctrlBlock;
    _T* _arr;
public:
    ArrayList(const uSys maxElements) noexcept
        : _ctrlBlock(reinterpret_cast<ControlBlock*>(PageAllocator::reserve(maxElements / PageAllocator::pageSize() + 1)))
        , _arr(reinterpret_cast<_T*>(_ctrlBlock + 1))
    {
        (void)PageAllocator::commitPage(_ctrlBlock);
        _ctrlBlock->data.refCount = 1;
        _ctrlBlock->data.elementCount = 0;
        _ctrlBlock->data.dataSize = sizeof(ControlBlock);
        _ctrlBlock->data.committedPages = 1;
    }

    ~ArrayList() noexcept
    {
        if(_ctrlBlock && --_ctrlBlock->data.refCount == 0)
        {
            for(uSys i = 0; i < _ctrlBlock->data.elementCount; ++i)
            {
                _arr[i].~_T();
            }

            PageAllocator::free(_ctrlBlock);
        }
    }

    ArrayList(const ArrayList<_T, ALMoveMethod::MoveConstruct>& copy) noexcept
        : _ctrlBlock(copy._ctrlBlock)
        , _arr(copy._arr)
    { ++_ctrlBlock->data.refCount; }

    ArrayList(ArrayList<_T, ALMoveMethod::MoveConstruct>&& move) noexcept
        : _ctrlBlock(move._ctrlBlock)
        , _arr(move._arr)
    { move._ctrlBlock = nullptr; }

    ArrayList<_T, ALMoveMethod::MoveConstruct>& operator=(const ArrayList<_T, ALMoveMethod::MoveConstruct>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        _ctrlBlock = copy._ctrlBlock;
        _arr = copy._arr;

        ++_ctrlBlock->data.refCount;

        return *this;
    }

    ArrayList<_T, ALMoveMethod::MoveConstruct>& operator=(ArrayList<_T, ALMoveMethod::MoveConstruct>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        _ctrlBlock = move._ctrlBlock;
        _arr = move._arr;

        move._ctrlBlock = nullptr;

        return *this;
    }

    [[nodiscard]]       _T* arr()       noexcept { return _arr; }
    [[nodiscard]] const _T* arr() const noexcept { return _arr; }
    [[nodiscard]] uSys  count() const noexcept { return _ctrlBlock->data.elementCount; }
    [[nodiscard]] uSys   size() const noexcept { return _ctrlBlock->data.elementCount; }
    [[nodiscard]] uSys length() const noexcept { return _ctrlBlock->data.elementCount; }

    void add(const _T& val) noexcept
    {
        assertSize();
        _arr[_ctrlBlock->data.elementCount++] = val;
        _ctrlBlock->data.dataSize += sizeof(_T);
    }

    template<typename... _Args>
    _T& emplace(_Args&&... args) noexcept
    {
        assertSize();
        void* const placement = _arr + _ctrlBlock->data.elementCount;
        _T* ret = new(placement) _T(_TauAllocatorUtils::_forward<_Args>(args)...);
        ++_ctrlBlock->data.elementCount;
        _ctrlBlock->data.dataSize += sizeof(_T);

        return *ret;
    }

    void removeFast(const uSys index) noexcept
    {
        if(index + 1 > _ctrlBlock->data.elementCount)
        { return; }

        _arr[index].~_T();

        --_ctrlBlock->data.elementCount;
        if(index == _ctrlBlock->data.elementCount)
        { return; }

        _arr[index] = ::std::move(_arr[_ctrlBlock->data.elementCount - 1]);
        _ctrlBlock->data.dataSize -= sizeof(_T);

        attemptRelease();
    }

    void remove(const uSys index) noexcept
    {
        if(index + 1 > _ctrlBlock->data.elementCount)
        { return; }

        _arr[index].~_T();

        --_ctrlBlock->data.elementCount;
        if(index == _ctrlBlock->data.elementCount)
        { return; }

        const uSys count = _ctrlBlock->data.elementCount - index - 1;

        for(uSys i = 0; i < count; ++i)
        {
            _arr[index + i] = ::std::move(_arr[index + i + 1]);
        }
        _ctrlBlock->data.dataSize -= sizeof(_T);

        attemptRelease();
    }

    void clear(const bool releasePages = true) noexcept
    {
        for(uSys i = 0; i < _ctrlBlock->data.elementCount; ++i)
        {
            _arr[i].~_T();
        }

        if(releasePages)
        {
            PageAllocator::decommitPages(_ctrlBlock + PageAllocator::pageSize(), _ctrlBlock->data.committedPages - 1);
        }
    }

    [[nodiscard]] _T* at(const uSys index) noexcept
    {
        if(index >= _ctrlBlock->data.elementCount)
        { return nullptr; }

        return _arr[index];
    }

    [[nodiscard]] const _T* at(const uSys index) const noexcept
    {
        if(index >= _ctrlBlock->data.elementCount)
        { return nullptr; }

        return &_arr[index];
    }

    [[nodiscard]]       _T* get(const uSys index)       noexcept { return at(index); }
    [[nodiscard]] const _T* get(const uSys index) const noexcept { return at(index); }

    [[nodiscard]]       _T& operator[](const uSys index)       noexcept { return _arr[index]; }
    [[nodiscard]] const _T& operator[](const uSys index) const noexcept { return _arr[index]; }

    [[nodiscard]] inline ArrayListIterator<_T> begin() noexcept { return ArrayListIterator<_T>(_ctrlBlock, _arr, 0);                        }
    [[nodiscard]] inline ArrayListIterator<_T>   end() noexcept { return ArrayListIterator<_T>(_ctrlBlock, _arr, _ctrlBlock->data.elementCount); }

    [[nodiscard]] inline ConstArrayListIterator<_T> begin() const noexcept { return ConstArrayListIterator<_T>(_ctrlBlock, _arr, 0);                        }
    [[nodiscard]] inline ConstArrayListIterator<_T>   end() const noexcept { return ConstArrayListIterator<_T>(_ctrlBlock, _arr, _ctrlBlock->data.elementCount); }
private:
    void assertSize() noexcept
    {
        const uSys pageBytes = _ctrlBlock->data.committedPages * PageAllocator::pageSize();
        if(_ctrlBlock->data.dataSize + sizeof(_T) >= pageBytes)
        {
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_ctrlBlock) + pageBytes);
            ++_ctrlBlock->data.committedPages;
        }
    }

    void attemptRelease() noexcept
    {
        // If there are two empty pages, release the last page.
        const uSys pageBytes = (_ctrlBlock->data.committedPages - 1) * PageAllocator::pageSize();
        if(pageBytes - _ctrlBlock->data.dataSize <= PageAllocator::pageSize())
        {
            (void) PageAllocator::decommitPage(reinterpret_cast<u8*>(_ctrlBlock) + pageBytes);
            --_ctrlBlock->data.committedPages;
        }
    }
};
