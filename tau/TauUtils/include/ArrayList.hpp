#pragma once

#include "NumTypes.hpp"
#include "Objects.hpp"
#include <cstring>

class ArrayListAllocator final
{
    DELETE_CONSTRUCT(ArrayListAllocator);
    DELETE_COPY(ArrayListAllocator);
    DELETE_DESTRUCT(ArrayListAllocator);
private:
    static void init() noexcept;
public:
    static void* reserve(std::size_t numPages) noexcept;

    static void* commitPage(void* page) noexcept;

    static void decommitPage(void* page) noexcept;

    static void decommitPages(void* page, std::size_t pageCount) noexcept;

    static void free(void* page) noexcept;

    static std::size_t pageSize() noexcept;
};

template<typename _T, std::size_t _NumReservePages = 1024>
class ArrayList final
{
private:
    i64* _refCount;
    _T* _arr;
    std::size_t _elementCount;
    std::size_t _dataSize;
    std::size_t _committedPages;
public:
    ArrayList() noexcept
        : _refCount(reinterpret_cast<i64*>(ArrayListAllocator::reserve(_NumReservePages))), _arr(nullptr),
          _elementCount(0), _dataSize(sizeof(i64)), _committedPages(1)
    {
        (void) ArrayListAllocator::commitPage(_refCount);
        *_refCount = 1;
        _arr = reinterpret_cast<_T*>(_refCount + 1);
    }

    ~ArrayList() noexcept
    {
        if(--(*_refCount) <= 0)
        {
            ArrayListAllocator::free(_refCount);
        }
    }

    ArrayList(const ArrayList<_T, _NumReservePages>& copy) noexcept
        : _refCount(copy._refCount), _arr(copy._arr),
          _elementCount(copy._elementCount), _dataSize(copy._dataSize),
          _committedPages(copy._committedPages)
    { ++(*_refCount); }

    ArrayList(ArrayList<_T, _NumReservePages>&& move) noexcept
        : _refCount(move._refCount), _arr(move._arr),
          _elementCount(move._elementCount), _dataSize(move._dataSize),
          _committedPages(move._committedPages)
    { ++(*_refCount); }

    ArrayList<_T, _NumReservePages>& operator =(const ArrayList<_T, _NumReservePages>& copy) noexcept
    {
        _refCount = copy._refCount;
        _arr = copy._arr;
        _elementCount = copy._elementCount;
        _dataSize = copy._dataSize;
        _committedPages = copy._committedPages;

        ++(*_refCount);
        return *this;
    }

    ArrayList<_T, _NumReservePages>& operator =(ArrayList<_T, _NumReservePages>&& move) noexcept
    {
        _refCount = move._refCount;
        _arr = move._arr;
        _elementCount = move._elementCount;
        _dataSize = move._dataSize;
        _committedPages = move._committedPages;

        ++(*_refCount);
        return *this;
    }

    [[nodiscard]] std::size_t count() const noexcept { return _elementCount; }

    void add(const _T& val) noexcept
    {
        assertSize();
        _arr[_elementCount++] = val;
        _dataSize += sizeof(_T);
    }

    template<typename... _Args>
    _T& emplace(_Args... args) noexcept
    {
        assertSize();
        void* const placement = reinterpret_cast<u8*>(_arr) + _elementCount * sizeof(_T);
        _T* ret = new(placement) _T(args...);
        ++_elementCount;
        _dataSize += sizeof(_T);


        return *ret;
    }

    void remove(const std::size_t index) noexcept
    {
        if(index + 1 > _elementCount)
        { return; }

        --_elementCount;
        if(index == _elementCount)
        { return; }

        (void) std::memcpy(reinterpret_cast<void*>(_arr + index), reinterpret_cast<void*>(_arr + _elementCount), sizeof(_T));
        _dataSize -= sizeof(_T);
    }

    [[nodiscard]] _T* get(const std::size_t index) noexcept
    {
        if(index >= _elementCount)
        {
            return nullptr;
        }

        return _arr[index];
    }

    [[nodiscard]] const _T* get(const std::size_t index) const noexcept
    {
        if(index >= _elementCount)
        {
            return nullptr;
        }

        return &_arr[index];
    }

    [[nodiscard]] _T& operator[](const std::size_t index) noexcept
    {
        return _arr[index];
    }

    [[nodiscard]] const _T& operator[](const std::size_t index) const noexcept
    {
        return _arr[index];
    }
private:
    void assertSize() noexcept
    {
        const std::size_t pageBytes = _committedPages * ArrayListAllocator::pageSize();
        if(_dataSize + sizeof(_T) >= pageBytes)
        {
            (void) ArrayListAllocator::commitPage(reinterpret_cast<u8*>(_refCount) + pageBytes);
            ++_committedPages;
        }
    }
};
