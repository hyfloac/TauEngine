#pragma once

#pragma warning(push, 0)
#include <cstring>
#pragma warning(pop)

#include "NumTypes.hpp"
#include "Objects.hpp"

template<typename _T>
class DynArrayIterator final
{
    DEFAULT_DESTRUCT(DynArrayIterator);
    DEFAULT_COPY(DynArrayIterator);
private:
    _T* _arr;
    uSys _length;
    uSys _index;
public:
    inline DynArrayIterator(_T* const arr, const uSys length, const uSys index)
        : _arr(arr), _length(length), _index(index)
    { }

    inline DynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    inline DynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline DynArrayIterator<_T> operator++(int) noexcept
    {
        const DynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline DynArrayIterator<_T> operator--(int) noexcept
    {
        const DynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline _T& operator*() noexcept
    { return _arr[_index]; }

    [[nodiscard]] inline const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] inline bool operator ==(const DynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const DynArrayIterator<_T>& other) const noexcept
    { return _index != other._index; }
};

template<typename _T>
class ConstDynArrayIterator final
{
    DEFAULT_DESTRUCT(ConstDynArrayIterator);
    DEFAULT_COPY(ConstDynArrayIterator);
private:
    _T* _arr;
    uSys _length;
    uSys _index;
public:
    inline ConstDynArrayIterator(_T* const arr, const uSys length, const uSys index)
        : _arr(arr), _length(length), _index(index)
    { }

    inline ConstDynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    inline ConstDynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline ConstDynArrayIterator<_T> operator++(int) noexcept
    {
        const ConstDynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline ConstDynArrayIterator<_T> operator--(int) noexcept
    {
        const ConstDynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] inline bool operator ==(const ConstDynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const ConstDynArrayIterator<_T>& other) const noexcept
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
    inline RefDynArrayIterator(_T* const arr, uSys* refCount, const uSys length, const uSys index)
        : _arr(arr), _refCount(refCount), _length(length), _index(index)
    { ++(*_refCount); }

    inline ~RefDynArrayIterator() noexcept
    {
        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    RefDynArrayIterator(const RefDynArrayIterator& copy)
        : _arr(copy._arr), _refCount(copy._refCount), _length(copy._length), _index(copy._index)
    { ++(*_refCount); }

    RefDynArrayIterator(RefDynArrayIterator&& move) noexcept
        : _arr(move._arr), _refCount(move._refCount), _length(move._length), _index(move._index)
    { ++(*_refCount); }

    RefDynArrayIterator& operator=(const RefDynArrayIterator& copy)
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

        ++(*_refCount);

        return *this;
    }

    inline RefDynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    inline RefDynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline RefDynArrayIterator<_T> operator++(int) noexcept
    {
        const RefDynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline RefDynArrayIterator<_T> operator--(int) noexcept
    {
        const RefDynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline _T& operator*() noexcept
    { return _arr[_index]; }

    [[nodiscard]] inline const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] inline bool operator ==(const RefDynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const RefDynArrayIterator<_T>& other) const noexcept
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
    inline ConstRefDynArrayIterator(_T* const arr, uSys* refCount, const uSys length, const uSys index)
        : _arr(arr), _refCount(refCount), _length(length), _index(index)
    { ++(*_refCount); }

    inline ~ConstRefDynArrayIterator() noexcept
    {
        if(--(*_refCount) == 0)
        {
            delete[] _arr;
            delete _refCount;
        }
    }

    ConstRefDynArrayIterator(const ConstRefDynArrayIterator<_T>& copy)
        : _arr(copy._arr), _refCount(copy._refCount), _length(copy._length), _index(copy._index)
    { ++(*_refCount); }

    ConstRefDynArrayIterator(ConstRefDynArrayIterator<_T>&& move) noexcept
        : _arr(move._arr), _refCount(move._refCount), _length(move._length), _index(move._index)
    { ++(*_refCount); }

    ConstRefDynArrayIterator<_T>& operator=(const ConstRefDynArrayIterator<_T>& copy)
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

        ++(*_refCount);

        return *this;
    }

    inline ConstRefDynArrayIterator<_T>& operator++() noexcept
    {
        if(_index < _length)
        { ++_index; }
        return *this;
    }

    inline ConstRefDynArrayIterator<_T>& operator--() noexcept
    {
        if(_index > 0)
        { --_index; }
        return *this;
    }

    [[nodiscard]] inline ConstRefDynArrayIterator<_T> operator++(int) noexcept
    {
        const ConstRefDynArrayIterator<_T> copy(*this);
        this->operator++();
        return copy;
    }

    [[nodiscard]] inline ConstRefDynArrayIterator<_T> operator--(int) noexcept
    {
        const ConstRefDynArrayIterator<_T> copy(*this);
        this->operator--();
        return copy;
    }

    [[nodiscard]] inline const _T& operator*() const noexcept
    { return _arr[_index]; }
    
    [[nodiscard]] inline bool operator ==(const ConstRefDynArrayIterator<_T>& other) const noexcept
    { return _index == other._index; }
    [[nodiscard]] inline bool operator !=(const ConstRefDynArrayIterator<_T>& other) const noexcept
    { return _index != other._index; }
};

template<typename _T>
class DynArray final
{
private:
    _T* _arr;
    uSys _size;
public:
    explicit DynArray(uSys size)
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

    [[nodiscard]] uSys size()   const noexcept { return _size; }
    [[nodiscard]] uSys length() const noexcept { return _size; }
    [[nodiscard]] uSys count()  const noexcept { return _size; }

    [[nodiscard]] operator uSys() const { return _size; }

    [[nodiscard]] inline DynArrayIterator<_T> begin() noexcept { return DynArrayIterator<_T>(_arr, _size, 0); }
    [[nodiscard]] inline DynArrayIterator<_T> end() noexcept { return DynArrayIterator<_T>(_arr, _size, _size); }

    [[nodiscard]] inline ConstDynArrayIterator<_T> begin() const noexcept { return ConstDynArrayIterator<_T>(_arr, _size, 0); }
    [[nodiscard]] inline ConstDynArrayIterator<_T> end() const noexcept { return ConstDynArrayIterator<_T>(_arr, _size, _size); }
};

template<typename _T>
class RefDynArray final
{
private:
    _T* _arr;
    uSys _size;
    uSys* _refCount;
public:
    explicit RefDynArray(uSys size)
        : _arr(new(::std::nothrow) _T[size]), _size(size), _refCount(new(::std::nothrow) uSys(1))
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

    [[nodiscard]] uSys size()   const noexcept { return _size; }
    [[nodiscard]] uSys length() const noexcept { return _size; }
    [[nodiscard]] uSys count()  const noexcept { return _size; }

    [[nodiscard]] operator uSys() const { return _size; }

    [[nodiscard]] inline RefDynArrayIterator<_T> begin() noexcept { return RefDynArrayIterator<_T>(_arr, _refCount, _size, 0); }
    [[nodiscard]] inline RefDynArrayIterator<_T> end() noexcept { return RefDynArrayIterator<_T>(_arr, _refCount, _size, _size); }

    [[nodiscard]] inline ConstRefDynArrayIterator<_T> begin() const noexcept { return ConstRefDynArrayIterator<_T>(_arr, _refCount, _size, 0); }
    [[nodiscard]] inline ConstRefDynArrayIterator<_T> end() const noexcept { return ConstRefDynArrayIterator<_T>(_arr, _refCount, _size, _size); }
};
