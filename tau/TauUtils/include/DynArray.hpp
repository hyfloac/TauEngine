#pragma once

#include <cstring>

template<typename _T>
class DynArray
{
private:
    _T* _arr;
    size_t _size;
public:
    explicit DynArray(size_t size)
        : _arr(new _T[size]), _size(size)
    { }

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

    ~DynArray()
    {
        delete[] _arr;
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

    operator const _T*() const noexcept { return _arr; }
    operator       _T*()       noexcept { return _arr; }

    size_t size()   const noexcept { return _size; }
    size_t length() const noexcept { return _size; }
    size_t count()  const noexcept { return _size; }

    operator size_t() const { return _size; }
};
