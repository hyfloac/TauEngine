#pragma once

#include "NumTypes.hpp"
#include "PageAllocator.hpp"
#include <cstring>

template<typename _T, uSys _NumReservePages = 1024>
class ArrayList final
{
private:
    struct ControlBlock final
    {
        iSys refCount;
        uSys elementCount;
        uSys dataSize;
        uSys committedPages;
    };
private:
    ControlBlock* _ctrlBlock;
    _T* _arr;
public:
    ArrayList() noexcept
        : _ctrlBlock(reinterpret_cast<ControlBlock*>(PageAllocator::reserve(_NumReservePages))),
          _arr(reinterpret_cast<_T*>(_ctrlBlock + 1))
    {
        (void) PageAllocator::commitPage(_ctrlBlock);
        _ctrlBlock->refCount = 1;
        _ctrlBlock->elementCount = 0;
        _ctrlBlock->dataSize = sizeof(ControlBlock);
        _ctrlBlock->committedPages = 1;
    }

    ~ArrayList() noexcept
    {
        if(--(_ctrlBlock->refCount) <= 0)
        { PageAllocator::free(_ctrlBlock->refCount); }
    }

    ArrayList(const ArrayList<_T, _NumReservePages>& copy) noexcept
        : _ctrlBlock(copy._ctrlBlock), _arr(copy._arr)
    { ++(_ctrlBlock->refCount); }

    ArrayList(ArrayList<_T, _NumReservePages>&& move) noexcept
        : _ctrlBlock(move._ctrlBlock), _arr(move._arr)
    { ++(_ctrlBlock->refCount); }

    ArrayList<_T, _NumReservePages>& operator =(const ArrayList<_T, _NumReservePages>& copy) noexcept
    {
        _ctrlBlock = copy._ctrlBlock;
        _arr = copy._arr;

        ++(_ctrlBlock->refCount);
        return *this;
    }

    ArrayList<_T, _NumReservePages>& operator =(ArrayList<_T, _NumReservePages>&& move) noexcept
    {
        _ctrlBlock = move._ctrlBlock;
        _arr = move._arr;

        ++(_ctrlBlock->refCount);
        return *this;
    }

    [[nodiscard]] uSys count() const noexcept { return _ctrlBlock->elementCount; }

    void add(const _T& val) noexcept
    {
        assertSize();
        _arr[_ctrlBlock->elementCount++] = val;
        _ctrlBlock->dataSize += sizeof(_T);
    }

    template<typename... _Args>
    _T& emplace(_Args... args) noexcept
    {
        assertSize();
        void* const placement = reinterpret_cast<u8*>(_arr) + _ctrlBlock->elementCount * sizeof(_T);
        _T* ret = new(placement) _T(args...);
        ++_ctrlBlock->elementCount;
        _ctrlBlock->dataSize += sizeof(_T);

        return *ret;
    }

    void remove(const uSys index) noexcept
    {
        if(index + 1 > _ctrlBlock->elementCount)
        { return; }

        --_ctrlBlock->elementCount;
        if(index == _ctrlBlock->elementCount)
        { return; }

        (void) std::memcpy(reinterpret_cast<void*>(_arr + index), reinterpret_cast<void*>(_arr + _ctrlBlock->elementCount), sizeof(_T));
        _ctrlBlock->dataSize -= sizeof(_T);
    }

    [[nodiscard]] _T* get(const uSys index) noexcept
    {
        if(index >= _ctrlBlock->elementCount)
        { return nullptr; }

        return _arr[index];
    }

    [[nodiscard]] const _T* get(const uSys index) const noexcept
    {
        if(index >= _ctrlBlock->elementCount)
        { return nullptr; }

        return &_arr[index];
    }

    [[nodiscard]] _T& operator[](const uSys index) noexcept
    {
        return _arr[index];
    }

    [[nodiscard]] const _T& operator[](const uSys index) const noexcept
    {
        return _arr[index];
    }
private:
    void assertSize() noexcept
    {
        const uSys pageBytes = _ctrlBlock->committedPages * PageAllocator::pageSize();
        if(_ctrlBlock->dataSize + sizeof(_T) >= pageBytes)
        {
            (void) PageAllocator::commitPage(reinterpret_cast<u8*>(_ctrlBlock->refCount) + pageBytes);
            ++_ctrlBlock->committedPages;
        }
    }
};
