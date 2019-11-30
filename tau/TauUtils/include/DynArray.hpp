#pragma once

#pragma warning(push, 0)
#include <cstring>
#include <cstddef>
#pragma warning(pop)

template<typename _T>
class DynArray final
{
private:
    _T* _arr;
    ::std::size_t _size;
public:
    explicit DynArray(::std::size_t size)
        : _arr(new(::std::nothrow) _T[size]), _size(size)
    { }

    ~DynArray()
    { delete[] _arr; }

    DynArray(const DynArray<_T>& copy)
        : _arr(new(::std::nothrow) _T[copy._size]), _size(copy._size)
    {
        memcpy(_arr, copy._arr, copy._size);
    }

    DynArray(DynArray<_T>&& move) noexcept
        : _arr(move._arr), _size(move._size)
    {
        move._arr = nullptr;
        move._size = 0;
    }

    DynArray<_T>& operator =(const DynArray<_T>& copy)
    {
        delete[] _arr;

        _arr = new(::std::nothrow) _T[copy._size];
        _size = copy._size;

        memcpy(_arr, copy._arr, copy._size);

        return *this;
    }

    DynArray<_T>& operator =(DynArray<_T>&& move) noexcept
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

    [[nodiscard]] ::std::size_t size()   const noexcept { return _size; }
    [[nodiscard]] ::std::size_t length() const noexcept { return _size; }
    [[nodiscard]] ::std::size_t count()  const noexcept { return _size; }

    [[nodiscard]] operator ::std::size_t() const { return _size; }
};

template<typename _T>
class RefDynArray final
{
private:
    _T* _arr;
    ::std::size_t _size;
    ::std::size_t* _refCount;
public:
    explicit RefDynArray(::std::size_t size)
        : _arr(new(::std::nothrow) _T[size]), _size(size), _refCount(new(::std::nothrow) ::std::size_t(1))
    { }

    ~RefDynArray()
    {
        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    inline RefDynArray(const RefDynArray<_T>& copy) noexcept
        : _arr(copy._arr), _size(copy._size), _refCount(copy._refCount)
    { ++(*_refCount); }

    inline RefDynArray(RefDynArray<_T>&& move) noexcept
        : _arr(move._arr), _size(move._size), _refCount(move._refCount)
    { ++(*_refCount); }

    inline RefDynArray<_T>& operator =(const RefDynArray<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }

        _arr = copy._arr;
        _size = copy._size;
        _refCount = copy._refCount;
        ++(*_refCount);

        return *this;
    }

    inline RefDynArray<_T>& operator =(RefDynArray<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }

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

    [[nodiscard]] ::std::size_t size()   const noexcept { return _size; }
    [[nodiscard]] ::std::size_t length() const noexcept { return _size; }
    [[nodiscard]] ::std::size_t count()  const noexcept { return _size; }

    [[nodiscard]] operator ::std::size_t() const { return _size; }
};
