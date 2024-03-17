#pragma once

#pragma warning(push, 0)
#include <cstring>
#pragma warning(pop)

#include "NumTypes.hpp"
#include "Objects.hpp"

template<typename _T>
class ConstRefDynArray;

template<typename _T>
class DynArrayIterator final
{
    DEFAULT_DESTRUCT(DynArrayIterator);
    DEFAULT_CM_PU(DynArrayIterator);
private:
    _T* _arr;
    uSys _length;
    uSys _index;
public:
    DynArrayIterator(_T* const arr, const uSys length, const uSys index) noexcept
        : _arr(arr)
        , _length(length)
        , _index(index)
    { }

    DynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    DynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] DynArrayIterator<_T> operator++(int) noexcept
    {
        const DynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] DynArrayIterator<_T> operator--(int) noexcept
    {
        const DynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] _T& operator*() noexcept
    { return _arr[_index]; }

    [[nodiscard]] const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] bool operator ==(const DynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] bool operator !=(const DynArrayIterator<_T>& other) const noexcept
    { return _index != other._index; }
};

template<typename _T>
class ConstDynArrayIterator final
{
    DEFAULT_DESTRUCT(ConstDynArrayIterator);
    DEFAULT_CM_PU(ConstDynArrayIterator);
private:
    _T* _arr;
    uSys _length;
    uSys _index;
public:
    ConstDynArrayIterator(_T* const arr, const uSys length, const uSys index) noexcept
        : _arr(arr)
        , _length(length)
        , _index(index)
    { }

    ConstDynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    ConstDynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] ConstDynArrayIterator<_T> operator++(int) noexcept
    {
        const ConstDynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] ConstDynArrayIterator<_T> operator--(int) noexcept
    {
        const ConstDynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] bool operator ==(const ConstDynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] bool operator !=(const ConstDynArrayIterator<_T>& other) const noexcept
    { return _index != other._index; }
};

template<typename _T>
class RefDynArrayIterator final
{
private:
    _T* _arr;
    uSys* _refCount;
    uSys _length;
    uSys _index;
public:
    RefDynArrayIterator(_T* const arr, uSys* const refCount, const uSys length, const uSys index) noexcept
        : _arr(arr)
        , _refCount(refCount)
        , _length(length)
        , _index(index)
    { ++(*_refCount); }

    ~RefDynArrayIterator() noexcept
    {
        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    RefDynArrayIterator(const RefDynArrayIterator& copy) noexcept
        : _arr(copy._arr)
        , _refCount(copy._refCount)
        , _length(copy._length)
        , _index(copy._index)
    { ++(*_refCount); }

    RefDynArrayIterator(RefDynArrayIterator&& move) noexcept
        : _arr(move._arr)
        , _refCount(move._refCount)
        , _length(move._length)
        , _index(move._index)
    {
        move._arr = null;
        move._refCount = null;
    }

    RefDynArrayIterator& operator=(const RefDynArrayIterator& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }

        _arr = copy._arr;
        _refCount = copy._refCount;
        _length = copy._length;
        _index = copy._index;

        ++(*_refCount);

        return *this;
    }

    RefDynArrayIterator& operator=(RefDynArrayIterator&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }

        _arr = move._arr;
        _refCount = move._refCount;
        _length = move._length;
        _index = move._index;

        move._arr = null;
        move._refCount = null;

        return *this;
    }

    RefDynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    RefDynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] RefDynArrayIterator<_T> operator++(int) noexcept
    {
        const RefDynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] RefDynArrayIterator<_T> operator--(int) noexcept
    {
        const RefDynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] _T& operator*() noexcept
    { return _arr[_index]; }

    [[nodiscard]] const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] bool operator ==(const RefDynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] bool operator !=(const RefDynArrayIterator<_T>& other) const noexcept
    { return _index != other._index; }
};

template<typename _T>
class ConstRefDynArrayIterator final
{
private:
    _T* _arr;
    uSys* _refCount;
    uSys _length;
    uSys _index;
public:
    ConstRefDynArrayIterator(_T* const arr, uSys* refCount, const uSys length, const uSys index) noexcept
        : _arr(arr)
        , _refCount(refCount)
        , _length(length)
        , _index(index)
    { ++(*_refCount); }

    ~ConstRefDynArrayIterator() noexcept
    {
        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    ConstRefDynArrayIterator(const ConstRefDynArrayIterator<_T>& copy) noexcept
        : _arr(copy._arr)
        , _refCount(copy._refCount)
        , _length(copy._length)
        , _index(copy._index)
    { ++(*_refCount); }

    ConstRefDynArrayIterator(ConstRefDynArrayIterator<_T>&& move) noexcept
        : _arr(move._arr)
        , _refCount(move._refCount)
        , _length(move._length)
        , _index(move._index)
    {
        move._arr = null;
        move._refCount = null;
    }

    ConstRefDynArrayIterator<_T>& operator=(const ConstRefDynArrayIterator<_T>& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }

        _arr = copy._arr;
        _refCount = copy._refCount;
        _length = copy._length;
        _index = copy._index;

        ++(*_refCount);

        return *this;
    }

    ConstRefDynArrayIterator<_T>& operator=(ConstRefDynArrayIterator<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }

        _arr = move._arr;
        _refCount = move._refCount;
        _length = move._length;
        _index = move._index;

        move._arr = null;
        move._refCount = null;

        return *this;
    }

    ConstRefDynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    ConstRefDynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] ConstRefDynArrayIterator<_T> operator++(int) noexcept
    {
        const ConstRefDynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] ConstRefDynArrayIterator<_T> operator--(int) noexcept
    {
        const ConstRefDynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] bool operator ==(const ConstRefDynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] bool operator !=(const ConstRefDynArrayIterator<_T>& other) const noexcept
    { return _index != other._index; }
};

template<typename _T>
class DynArray final
{
private:
    _T* _arr;
    uSys _size;
public:
    explicit DynArray(const uSys size = 0)
        : _arr(new(::std::nothrow) _T[size])
        , _size(size)
    { }

    ~DynArray()
    { delete[] _arr; }

    DynArray(const DynArray<_T>& copy)
        : _arr(new(::std::nothrow) _T[copy._size])
        , _size(copy._size)
    { ::std::memcpy(_arr, copy._arr, copy._size * sizeof(_T)); }

    DynArray(DynArray<_T>&& move) noexcept
        : _arr(move._arr)
        , _size(move._size)
    { move._arr = nullptr; }

    DynArray<_T>& operator =(const DynArray<_T>& copy)
    {
        if(this == &copy)
        { return *this; }

        delete[] _arr;

        _arr = new(::std::nothrow) _T[copy._size];
        _size = copy._size;

        ::std::memcpy(_arr, copy._arr, copy._size * sizeof(_T));

        return *this;
    }

    DynArray<_T>& operator =(DynArray<_T>&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        delete[] _arr;

        _arr = move._arr;
        _size = move._size;

        move._arr = nullptr;

        return *this;
    }

    [[nodiscard]] operator const _T*() const noexcept { return _arr; }
    [[nodiscard]] operator       _T*()       noexcept { return _arr; }

    [[nodiscard]] const _T* arr() const noexcept { return _arr; }
    [[nodiscard]]       _T* arr()       noexcept { return _arr; }

    [[nodiscard]] uSys size()   const noexcept { return _size; }
    [[nodiscard]] uSys length() const noexcept { return _size; }
    [[nodiscard]] uSys count()  const noexcept { return _size; }

    [[nodiscard]] operator uSys() const { return _size; }

    [[nodiscard]] DynArrayIterator<_T> begin() noexcept { return DynArrayIterator<_T>(_arr, _size, 0); }
    [[nodiscard]] DynArrayIterator<_T> end() noexcept { return DynArrayIterator<_T>(_arr, _size, _size); }

    [[nodiscard]] ConstDynArrayIterator<_T> begin() const noexcept { return ConstDynArrayIterator<_T>(_arr, _size, 0); }
    [[nodiscard]] ConstDynArrayIterator<_T> end() const noexcept { return ConstDynArrayIterator<_T>(_arr, _size, _size); }
};

template<typename _T>
class RefDynArray final
{
private:
    _T* _arr;
    uSys _size;
    uSys* _refCount;
public:
    explicit RefDynArray(const uSys size = 0) noexcept
        : _arr(new(::std::nothrow) _T[size])
        , _size(size)
        , _refCount(new(::std::nothrow) uSys(1))
    { }

    ~RefDynArray() noexcept
    {
        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    RefDynArray(const RefDynArray<_T>& copy) noexcept
        : _arr(copy._arr)
        , _size(copy._size)
        , _refCount(copy._refCount)
    { ++(*_refCount); }

    RefDynArray(RefDynArray<_T>&& move) noexcept
        : _arr(move._arr)
        , _size(move._size)
        , _refCount(move._refCount)
    {
        move._arr = nullptr;
        move._refCount = nullptr;
    }

    RefDynArray<_T>& operator =(const RefDynArray<_T>& copy) noexcept
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

    RefDynArray<_T>& operator =(RefDynArray<_T>&& move) noexcept
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

        move._arr = null;
        move._refCount = null;

        return *this;
    }

    [[nodiscard]] operator const _T* () const noexcept { return _arr; }
    [[nodiscard]] operator       _T* ()       noexcept { return _arr; }

    [[nodiscard]] const _T* arr() const noexcept { return _arr; }
    [[nodiscard]]       _T* arr()       noexcept { return _arr; }

    [[nodiscard]] uSys size()   const noexcept { return _size; }
    [[nodiscard]] uSys length() const noexcept { return _size; }
    [[nodiscard]] uSys count()  const noexcept { return _size; }

    [[nodiscard]] operator uSys() const { return _size; }

    [[nodiscard]] RefDynArrayIterator<_T> begin() noexcept { return RefDynArrayIterator<_T>(_arr, _refCount, _size, 0); }
    [[nodiscard]] RefDynArrayIterator<_T> end() noexcept { return RefDynArrayIterator<_T>(_arr, _refCount, _size, _size); }

    [[nodiscard]] ConstRefDynArrayIterator<_T> begin() const noexcept { return ConstRefDynArrayIterator<_T>(_arr, _refCount, _size, 0); }
    [[nodiscard]] ConstRefDynArrayIterator<_T> end() const noexcept { return ConstRefDynArrayIterator<_T>(_arr, _refCount, _size, _size); }
private:
    friend class ConstRefDynArray<_T>;
};

template<typename _T>
class ConstRefDynArray final
{
private:
    _T* _arr;
    uSys _size;
    uSys* _refCount;
public:
    ConstRefDynArray(const RefDynArray<_T>& copy) noexcept
        : _arr(copy._arr)
        , _size(copy._size)
        , _refCount(copy._refCount)
    { ++(*_refCount); }

    ConstRefDynArray(RefDynArray<_T>&& move) noexcept
        : _arr(move._arr)
        , _size(move._size)
        , _refCount(move._refCount)
    {
        move._arr = nullptr;
        move._refCount = nullptr;
    }

    ~ConstRefDynArray() noexcept
    {
        if(_refCount && --(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    ConstRefDynArray(const ConstRefDynArray<_T>& copy) noexcept
        : _arr(copy._arr)
        , _size(copy._size)
        , _refCount(copy._refCount)
    { ++(*_refCount); }

    ConstRefDynArray(ConstRefDynArray<_T>&& move) noexcept
        : _arr(move._arr)
        , _size(move._size)
        , _refCount(move._refCount)
    {
        move._arr = nullptr;
        move._refCount = nullptr;
    }

    ConstRefDynArray<_T>& operator =(const ConstRefDynArray<_T>& copy) noexcept
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

    ConstRefDynArray<_T>& operator =(ConstRefDynArray<_T>&& move) noexcept
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

        move._arr = null;
        move._refCount = null;

        return *this;
    }

    ConstRefDynArray<_T>& operator =(const RefDynArray<_T>& copy) noexcept
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

    ConstRefDynArray<_T>& operator =(RefDynArray<_T>&& move) noexcept
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

        move._arr = null;
        move._refCount = null;

        return *this;
    }

    [[nodiscard]] operator const _T* () const noexcept { return _arr; }

    [[nodiscard]] const _T* arr() const noexcept { return _arr; }

    [[nodiscard]] uSys size()   const noexcept { return _size; }
    [[nodiscard]] uSys length() const noexcept { return _size; }
    [[nodiscard]] uSys count()  const noexcept { return _size; }

    [[nodiscard]] operator uSys() const { return _size; }

    [[nodiscard]] ConstRefDynArrayIterator<_T> begin() const noexcept { return ConstRefDynArrayIterator<_T>(_arr, _refCount, _size, 0); }
    [[nodiscard]] ConstRefDynArrayIterator<_T> end() const noexcept { return ConstRefDynArrayIterator<_T>(_arr, _refCount, _size, _size); }
};
