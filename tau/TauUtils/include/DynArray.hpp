#pragma once

#include <cstring>
#include <cstddef>
#include "NumTypes.hpp"

template<typename _T>
class DynArray final
{
private:
    _T* _arr;
    size_t _size;
public:
    explicit DynArray(size_t size)
        : _arr(new _T[size]), _size(size)
    { }

    ~DynArray()
    { delete[] _arr; }

    DynArray(const DynArray<_T>& copy)
        : _arr(new _T[copy._size]), _size(copy._size)
    {
        memcpy(_arr, copy._arr, copy._size);
    }

    DynArray(DynArray<_T>&& move) noexcept
        : _arr(move._arr), _size(move._size)
    {
        move._arr = nullptr;
        move._size = 0;
    }

    DynArray& operator =(const DynArray<_T>& copy)
    {
        delete[] _arr;

        _arr = new _T[copy._size];
        _size = copy._size;

        memcpy(_arr, copy._arr, copy._size);

        return *this;
    }

    DynArray& operator =(DynArray<_T>&& move) noexcept
    {
        delete[] _arr;

        _arr = move._arr;
        _size = move._size;

        move._arr = nullptr;
        move._size = 0;

        return *this;
    }

    [[nodiscard]] operator const _T*() const noexcept { return _arr; }
    [[nodiscard]] operator       _T*()       noexcept { return _arr; }

    [[nodiscard]] const _T* arr() const noexcept { return _arr; }
    [[nodiscard]]       _T* arr()       noexcept { return _arr; }

    [[nodiscard]] size_t size()   const noexcept { return _size; }
    [[nodiscard]] size_t length() const noexcept { return _size; }
    [[nodiscard]] size_t count()  const noexcept { return _size; }

    [[nodiscard]] operator size_t() const { return _size; }
};

template<typename _T>
class RefDynArray final
{
private:
    _T* _arr;
    size_t _size;
    i32* _refCount;
public:
    explicit RefDynArray(size_t size)
        : _arr(new _T[size]), _size(size), _refCount(new i32(1))
    { }

    ~RefDynArray()
    {
        if(--(*_refCount) <= 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    RefDynArray(const RefDynArray<_T>& copy)
        : _arr(copy._arr), _size(copy._size), _refCount(copy._refCount)
    { ++(*_refCount); }

    RefDynArray(RefDynArray<_T>&& move) noexcept
        : _arr(move._arr), _size(move._size), _refCount(move._refCount)
    { ++(*_refCount); }

    RefDynArray& operator =(const RefDynArray<_T>& copy)
    {
        // if(--(*_refCount))
        // {
        //     delete[] _arr;
        //     delete _refCount;
        // }

        _arr = copy._arr;
        _size = copy._size;
        _refCount = copy._refCount;

        ++(*_refCount);

        return *this;
    }

    RefDynArray& operator =(RefDynArray<_T>&& move) noexcept
    {
        // if(--(*_refCount))
        // {
        //     delete[] _arr;
        //     delete _refCount;
        // }

        _arr = move._arr;
        _size = move._size;
        _refCount = move._refCount;

        ++(*_refCount);

        return *this;
    }

    [[nodiscard]] operator const _T* () const noexcept { return _arr; }
    [[nodiscard]] operator       _T* ()       noexcept { return _arr; }

    [[nodiscard]] const _T* arr() const noexcept { return _arr; }
    [[nodiscard]]       _T* arr()       noexcept { return _arr; }

    [[nodiscard]] size_t size()   const noexcept { return _size; }
    [[nodiscard]] size_t length() const noexcept { return _size; }
    [[nodiscard]] size_t count()  const noexcept { return _size; }

    [[nodiscard]] operator size_t() const { return _size; }
};
