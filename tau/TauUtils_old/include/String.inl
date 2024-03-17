#pragma once

#if defined(STRING_IN_DEV) || 1
#include "String.hpp"
#endif

#include <cstring>
#include <cwctype>
#include <locale>
#include "TUMaths.hpp"

template<>
inline char toLower<char>(const char c) noexcept
{
    if(c >= 'A' && c <= 'Z')
    {
        return static_cast<char>((c - 'A') + 'a');
    }
    return c;
}

template<>
inline wchar_t toLower<wchar_t>(const wchar_t c) noexcept
{ return static_cast<wchar_t>(::std::towlower(c)); }

template<>
inline c8 toLower<c8>(const c8 c) noexcept
{ return ::std::tolower(c, ::std::locale()); }

template<>
inline c16 toLower<c16>(const c16 c) noexcept
{ return ::std::tolower(c, ::std::locale()); }

template<>
inline c32 toLower<c32>(const c32 c) noexcept
{ return ::std::tolower(c, ::std::locale()); }

template<>
inline char toUpper<char>(const char c) noexcept
{
    if(c >= 'a' && c <= 'z')
    {
        return static_cast<char>((c - 'a') + 'A');
    }
    return c;
}

template<>
inline wchar_t toUpper<wchar_t>(const wchar_t c) noexcept
{ return static_cast<wchar_t>(::std::towupper(c)); }

template<>
inline c8 toUpper<c8>(const c8 c) noexcept
{ return ::std::toupper(c, ::std::locale()); }

template<>
inline c16 toUpper<c16>(const c16 c) noexcept
{ return ::std::toupper(c, ::std::locale()); }

template<>
inline c32 toUpper<c32>(const c32 c) noexcept
{ return ::std::toupper(c, ::std::locale()); }

template<typename _C>
inline uSys toLower(const _C* str, _C* store) noexcept
{
    uSys i = 0;
    if(store)
    {
        for(; str[i] != '\0'; ++i)
        {
            store[i] = toLower(str[i]);
        }
    }
    else
    {
        for(; str[i] != '\0'; ++i);
    }

    return i;
}

template<typename _C>
inline uSys toUpper(const _C* str, _C* store) noexcept
{
    uSys i = 0;
    if(store)
    {
        for(; str[i] != '\0'; ++i)
        {
            store[i] = toUpper(str[i]);
        }
    }
    else
    {
        for(; str[i] != '\0'; ++i);
    }

    return i;
}

template<typename _C>
inline uSys findHashCode(const _C* str) noexcept
{
    uSys hash = 0;
    for(uSys i = 0; str[i]; ++i)
    {
        hash = 31u * hash + static_cast<uSys>(str[i]);
    }
    return hash;
}

template<typename _C>
inline uSys findHashCode(const _C* str, const uSys len) noexcept
{
    uSys hash = 0;
    for(uSys i = 0; i < len; ++i)
    {
        hash = 31u * hash + static_cast<uSys>(str[i]);
    }
    return hash;
}

template<typename _C>
inline uSys strLength(const _C* const str) noexcept
{
    if(!str)
    { return 0; }

    uSys i = 0;
    // ReSharper disable once CppPossiblyErroneousEmptyStatements
    for(; str[i]; ++i);
    return i - 1;
}

template<>
inline uSys strLength<char>(const char* const str) noexcept
{ return ::std::strlen(str); }

template<>
inline uSys strLength<wchar_t>(const wchar_t* const str) noexcept
{ return ::std::wcslen(str); }

template<typename _C>
inline i32 strCompare(const _C* const lhs, const _C* const rhs) noexcept
{
    while(*lhs != '\0' && *rhs != '\0')
    {
        if(*lhs != *rhs)
        {
            if(*lhs < *rhs)
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
        ++lhs;
        ++rhs;
    }

    if(*lhs == '\0' && *rhs == '\0')
    {
        return 0;
    }
    else if(*lhs == '\0')
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

template<>
inline i32 strCompare(const char* const lhs, const char* const rhs) noexcept
{ return ::std::strcmp(lhs, rhs); }

template<>
inline i32 strCompare(const wchar_t* const lhs, const wchar_t* const rhs) noexcept
{ return ::std::wcscmp(lhs, rhs); }

template<typename _C>
inline i32 strCompare(const _C* const lhs, const _C* const rhs, const uSys length) noexcept
{
    uSys i = 0;
    while(*lhs != '\0' && *rhs != '\0' && i < length)
    {
        if(*lhs != *rhs)
        {
            if(*lhs < *rhs)
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
        ++lhs;
        ++rhs;
        ++i;
    }

    if(*lhs == *rhs)
    {
        return 0;
    }
    else if(*lhs == '\0')
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

template<>
inline i32 strCompare(const char* const lhs, const char* const rhs, const uSys length) noexcept
{ return ::std::strncmp(lhs, rhs, length); }

template<>
inline i32 strCompare(const wchar_t* const lhs, const wchar_t* const rhs, const uSys length) noexcept
{ return ::std::wcsncmp(lhs, rhs, length); }

template<typename _C, uSys _Len>
inline constexpr uSys cexpr::strlen(const _C(&str)[_Len]) noexcept
{ return _Len - 1; }

template<typename _C, uSys _Len>
inline constexpr bool cexpr::streq(const _C(&lhs)[_Len], const _C(&rhs)[_Len])
{
    while(*lhs != '\0' && *rhs != '\0')
    {
        if(*lhs != *rhs)
        { return false; }
        ++lhs;
        ++rhs;
    }

    return *lhs == '\0' && *rhs == '\0';
}

template<typename _C, uSys _Len>
inline constexpr bool cexpr::streqIgnoreCase(const _C(&lhs)[_Len], const _C(&rhs)[_Len])
{
    while(*lhs != '\0' && *rhs != '\0')
    {
        if(toLower(*lhs) != toLower(*rhs))
        { return false; }
        ++lhs;
        ++rhs;
    }

    return *lhs == '\0' && *rhs == '\0';
}

template<typename _C, uSys _Len>
inline constexpr uSys cexpr::findHashCode(const _C(&str)[_Len]) noexcept
{
    uSys hash = 0;
    for(uSys i = 0; str[i]; ++i)
    {
        hash = 31u * hash + static_cast<uSys>(str[i]);
    }
    return hash;
}

template<typename _C>
inline StringIteratorT<_C>::StringIteratorT(const _C* const string, const uSys length, const uSys index) noexcept
    : _string(string)
    , _length(length)
    , _index(index)
{ }

template<typename _C>
inline StringIteratorT<_C>& StringIteratorT<_C>::operator++() noexcept
{
    if(_index < _length - 1)
    { ++_index; }
    return *this;
}

template<typename _C>
inline StringIteratorT<_C>& StringIteratorT<_C>::operator--() noexcept
{
    if(_index > 0)
    { --_index; }
    return *this;
}

template<typename _C>
inline StringIteratorT<_C> StringIteratorT<_C>::operator++(int) noexcept
{
    const StringIteratorT copy(*this);
    operator++();
    return copy;
}

template<typename _C>
inline StringIteratorT<_C> StringIteratorT<_C>::operator--(int) noexcept
{
    const StringIteratorT copy(*this);
    operator--();
    return copy;
}

template<typename _C>
inline _C StringIteratorT<_C>::operator*() const noexcept
{ return _string[_index]; }

template<typename _C>
inline bool StringIteratorT<_C>::operator==(const StringIteratorT<_C>& other) const noexcept
{ return _index == other._index; }

template<typename _C>
inline bool StringIteratorT<_C>::operator!=(const StringIteratorT<_C>& other) const noexcept
{ return _index != other._index; }

template<typename _C>
template<uSys _Len>
inline constexpr ConstExprStringT<_C>::ConstExprStringT(const _C(&str)[_Len]) noexcept
    : _string(str)
    , _length(_Len - 1)
    , _hash(cexpr::findHashCode(str))
{ }

template<typename _C>
template<uSys _Len>
inline constexpr ConstExprStringT<_C>& ConstExprStringT<_C>::operator=(const _C(&str)[_Len]) noexcept
{
    _string = str;
    _length = _Len;
    _hash = cexpr::findHashCode(str);

    return *this;
}

template<typename _C>
template<uSys _Len>
inline constexpr bool ConstExprStringT<_C>::equals(const _C(&str)[_Len]) const noexcept
{
    if(_string == str) { return true; }
    if(_length != _Len) { return false; }
    return strCompare(_string, str) == 0;
}

template<typename _C>
inline bool ConstExprStringT<_C>::equals(const ConstExprStringT<_C>& other) const noexcept
{
    if(_string == other._string) { return true; }
    if(_length != other._length) { return false; }
    if(_hash != other._hash) { return false; }
    return strCompare(_string, other.c_str()) == 0;
}

template<typename _C>
inline bool ConstExprStringT<_C>::equals(const DynStringT<_C>& other) const noexcept
{
    if(_length != other._length || _hash != other._hash)
    { return false; }
    return strCompare(_string, other.c_str()) == 0;
}

template<typename _C>
inline bool ConstExprStringT<_C>::equals(const DynStringViewT<_C>& other) const noexcept
{
    if(_length != other._length || _hash != other._hash)
    { return false; }
    return strCompare(_string, other.c_str(), _length) == 0;
}

template<typename _C>
inline bool ConstExprStringT<_C>::equals(const _C* const str) const noexcept
{
    if(_string == str) { return true; }
    return strCompare(_string, str) == 0;
}

template<typename _C>
inline i32 ConstExprStringT<_C>::compareTo(const ConstExprStringT<_C>& other) const noexcept
{ return strCompare(_string, other._string()); }

template<typename _C>
inline i32 ConstExprStringT<_C>::compareTo(const DynStringT<_C>& other) const noexcept
{ return strCompare(_string, other.c_str()); }

template<typename _C>
inline i32 ConstExprStringT<_C>::compareTo(const DynStringViewT<_C>& other) const noexcept
{ return strCompare(_string, other._string, _length); }

template<typename _C>
inline i32 ConstExprStringT<_C>::compareTo(const _C* const str) const noexcept
{ return strCompare(_string, str); }

template<typename _C>
inline _C ConstExprStringT<_C>::operator[](const uSys index) const noexcept
{ return _string[index]; }

template<typename _C>
inline _C ConstExprStringT<_C>::at(const uSys index) const noexcept
{
    if(index >= _length) { return _C{ '\0' }; }
    return _string[index];
}

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::passControl(const _C* const str) noexcept
{
    const uSys len = strLength(str);
    if(len < 16)
    {
        DynStringT<_C> tmp(str);
        delete str;
        return tmp;
    }
    return DynStringT<_C>(str, len);
}

template<typename _C>
inline DynStringT<_C>::DynStringT(const _C* const string, const uSys length) noexcept
    : _largeString { new(::std::nothrow) uSys(1), string }
    , _length(length)
    , _hash(findHashCode(string))
{ }

template<typename _C>
inline DynStringT<_C>::DynStringT(const uSys length) noexcept
    : _stackString { }
    , _length(length)
    , _hash(0)
{ }

template<typename _C>
DynStringT<_C>::DynStringT(const uSys length, const _C* const string) noexcept
    : _largeString { null, null }
    , _length(length)
    , _hash(0)
{
    ::std::memcpy(_stackString, string, length * sizeof(_C));
    _stackString[length] = '\0';
    _hash = findHashCode(_stackString);
}

template<typename _C>
inline DynStringT<_C>::DynStringT() noexcept
    : _largeString { null, null }
    , _length(0)
    , _hash(0)
{ }

template<typename _C>
inline DynStringT<_C>::DynStringT(const _C* const string) noexcept
    : _largeString { null, null }
    , _length(strLength(string))
    , _hash(findHashCode(string))
{
    if(_length >= 16)
    {
        void* const placement = new(::std::nothrow) u8[sizeof(uSys) + (_length + 1) * sizeof(_C)];

        _largeString.refCount = new(placement) uSys(1);
        _C* const str = new(reinterpret_cast<u8* const>(placement) + sizeof(uSys)) _C[_length + 1];

        ::std::memcpy(str, string, (_length + 1) * sizeof(_C));
        _largeString.string = str;
    }
    else
    {
        ::std::memcpy(_stackString, string, (_length + 1) * sizeof(_C));
    }
}

template<typename _C>
inline DynStringT<_C>::~DynStringT() noexcept
{
    if(_length >= 16 && _largeString.string && --(*_largeString.refCount) == 0)
    {
        delete _largeString.refCount;
        // Was this allocated as a single block.
        if(reinterpret_cast<uPtr>(_largeString.refCount) != reinterpret_cast<uPtr>(_largeString.string) - static_cast<uPtr>(sizeof(uSys)))
        {
            delete[] _largeString.string;
        }
    }
}

template<typename _C>
inline DynStringT<_C>::DynStringT(const DynStringT<_C>& copy) noexcept
    : _length(copy._length)
    , _hash(copy._hash)
{
    if(_length >= 16)
    {
        _largeString.refCount = copy._largeString.refCount;
        _largeString.string = copy._largeString.string;
        ++(*_largeString.refCount);
    }
    else
    {
        ::std::memcpy(_stackString, copy._stackString, 16 * sizeof(_C));
    }
}

template<typename _C>
inline DynStringT<_C>::DynStringT(DynStringT<_C>&& move) noexcept
    : _length(move._length)
    , _hash(move._hash)
{
    if(_length >= 16)
    {
        _largeString.refCount = move._largeString.refCount;
        _largeString.string = move._largeString.string;
        move._largeString.string = null;
    }
    else
    {
        ::std::memcpy(_stackString, move._stackString, 16 * sizeof(_C));
    }
}

template<typename _C>
inline DynStringT<_C>& DynStringT<_C>::operator=(const DynStringT<_C>& copy) noexcept  // NOLINT(cert-oop54-cpp)
{
    if(this == &copy)
    { return *this; }

    if(copy._length >= 16)
    {
        if(_length >= 16 && --(*_largeString.refCount) == 0)
        {
            delete _largeString.refCount;
            // Was this allocated as a single block.
            if(reinterpret_cast<uPtr>(_largeString.refCount) != reinterpret_cast<uPtr>(_largeString.string) - static_cast<uPtr>(sizeof(uSys)))
            {
                delete[] _largeString.string;
            }
        }

        _largeString.refCount = copy._largeString.refCount;
        _largeString.string = copy._largeString.string;
        ++(*_largeString.refCount);
    }
    else
    {
        ::std::memcpy(_stackString, copy._stackString, 16 * sizeof(_C));
    }

    _length = copy._length;
    _hash = copy._hash;

    return *this;
}

template<typename _C>
inline DynStringT<_C>& DynStringT<_C>::operator=(DynStringT<_C>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(move._length >= 16)
    {
        if(_length >= 16 && --(*_largeString.refCount) == 0)
        {
            delete _largeString.refCount;
            // Was this allocated as a single block.
            if(reinterpret_cast<uPtr>(*_largeString.refCount) != reinterpret_cast<uPtr>(_largeString.string) - static_cast<uPtr>(sizeof(uSys)))
            {
                delete[] _largeString.string;
            }
        }

        _largeString.refCount = move._largeString.refCount;
        _largeString.string = move._largeString.string;
        move._largeString.string = null;
    }
    else
    {
        ::std::memcpy(_stackString, move._stackString, 16 * sizeof(_C));
    }

    _length = move._length;
    _hash = move._hash;

    return *this;
}

template<typename _C>
inline DynStringT<_C>& DynStringT<_C>::operator=(const _C* const string) noexcept
{
    if(!string)
    {
        return *this;
    }

    /**
     * _largeString.string and _stackString store the same address.
     */
    if(string == _largeString.string)
    {
        return *this;
    }

    const uSys length = strLength(string);

    if(_length >= 16 && length >= 16 && *_largeString.refCount == 1)
    {
        _length = length;

        // Was this allocated as a single block.
        if(reinterpret_cast<iPtr>(_largeString.refCount) != reinterpret_cast<iPtr>(_largeString.string) - sizeof(uSys))
        {
            delete[] _largeString.string;

            _C* const str = new(::std::nothrow) _C[_length + 1];
            ::std::memcpy(str, string, (_length + 1) * sizeof(_C));
            _largeString.string = str;
        }
        else
        {
            delete _largeString.refCount;

            void* const placement = new(::std::nothrow) u8[sizeof(uSys) + (_length + 1) * sizeof(_C)];

            _largeString.refCount = new(placement) uSys(1);
            _C* const str = new(reinterpret_cast<u8*>(placement) + sizeof(uSys)) _C[_length + 1];

            ::std::memcpy(str, string, (_length + 1) * sizeof(_C));
            _largeString.string = str;
        }
    }
    else
    {
        if(_length >= 16 && --(*_largeString.refCount) == 0)
        {
            delete _largeString.refCount;
            // Was this allocated as a single block.
            if(reinterpret_cast<iPtr>(_largeString.refCount) != reinterpret_cast<iPtr>(_largeString.string) - static_cast<iPtr>(sizeof(uSys)))
            {
                delete[] _largeString.string;
            }
        }

        _length = length;
        if(_length >= 16)
        {
            void* const placement = new(::std::nothrow) u8[sizeof(uSys) + (_length + 1) * sizeof(_C)];

            _largeString.refCount = new(placement) uSys(1);
            _C* const str = new(reinterpret_cast<u8*>(placement) + sizeof(uSys)) _C[_length + 1];

            ::std::memcpy(str, string, (_length + 1) * sizeof(_C));
            _largeString.string = str;
        }
        else
        {
            ::std::memcpy(_stackString, string, (_length + 1) * sizeof(_C));
        }
    }

    return *this;
}

template<typename _C>
inline bool DynStringT<_C>::equals(const ConstExprStringT<_C>& other) const noexcept
{
    if(_length != other._length ) { return false; }
    if(_hash != other._hash) { return false; }
    return strCompare(c_str(), other._string) == 0;
}

template<typename _C>
inline bool DynStringT<_C>::equals(const DynStringT<_C>& other) const noexcept
{
    if(this == &other) { return true; }
    if(_length == other._length && _hash == other._hash)
    {
        if(_length >= 16)
        {
            if(_largeString.string == other._largeString.string)
            {
                return true;
            }
            return strCompare(_largeString.string, other._largeString.string) == 0;
        }
        else
        {
            return strCompare(_stackString, other._stackString) == 0;
        }
    }
    return false;
}

template<typename _C>
inline bool DynStringT<_C>::equals(const DynStringViewT<_C>& other) const noexcept
{
    if(_length != other._length ) { return false; }
    if(_hash != other._hash) { return false; }
    return strCompare(c_str(), other._string, _length) == 0;
}

template<typename _C>
inline bool DynStringT<_C>::equals(const _C* const str) const noexcept
{ return strCompare(c_str(), str) == 0; }

template<typename _C>
inline i32 DynStringT<_C>::compareTo(const ConstExprStringT<_C>& other) const noexcept
{ return strCompare(c_str(), other._string()); }

template<typename _C>
inline i32 DynStringT<_C>::compareTo(const DynStringT<_C>& other) const noexcept
{ return strCompare(c_str(), other.c_str()); }

template<typename _C>
inline i32 DynStringT<_C>::compareTo(const DynStringViewT<_C>& other) const noexcept
{ return strCompare(c_str(), other._string, _length); }

template<typename _C>
inline i32 DynStringT<_C>::compareTo(const _C* const str) const noexcept
{ return strCompare(c_str(), str); }

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::concat(const ConstExprStringT<_C>& other) const noexcept
{ return _concat(other._length, other._string); }

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::concat(const DynStringT<_C>& other) const noexcept
{ return _concat(other._length, other.c_str()); }

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::concat(const DynStringViewT<_C>& other) const noexcept
{ return _concat(other._length, other._string); }

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::concat(const _C* const other) const noexcept
{ return _concat(strLength(other), other); }

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::_concat(const uSys len, const _C* const str) const noexcept
{
    const uSys newLen = _length + len;
    if(newLen >= 16)
    {
        _C* const newStr = new(::std::nothrow) _C[newLen + 1];
        newStr[newLen] = '\0';
        ::std::memcpy(newStr, c_str(), _length * sizeof(_C));
        ::std::memcpy(newStr + _length, str, len * sizeof(_C));
        return DynStringT<_C>(newStr, newLen);
    }
    else
    {
        DynStringT<_C> tmp(newLen);
        tmp._stackString[newLen] = '\0';
        ::std::memcpy(tmp._stackString, _stackString, _length * sizeof(_C));
        ::std::memcpy(tmp._stackString + _length, str, len * sizeof(_C));
        tmp._hash = findHashCode(tmp._stackString);
        return tmp;
    }
}

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::subString(const uSys begin, const uSys end) const noexcept
{
    if(begin >= end || end > _length)
    { return DynStringT<_C>(); }

    const uSys length = end - begin;

    if(length >= 16)
    {
        _C* const sub = new(::std::nothrow) _C[length + 1];
        sub[length] = '\0';
        ::std::memcpy(sub, c_str() + begin, length * sizeof(_C));
        return DynStringT<_C>(sub, length);
    }

    return DynStringT<_C>(length, c_str() + begin);
}

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::subStringLen(const uSys begin, const uSys length) const noexcept
{
    if(begin + length > _length)
    {
        return DynStringT<_C>("");
    }

    if(length >= 16)
    {
        _C* const sub = new(::std::nothrow) _C[length + 1];
        sub[length] = '\0';
        ::std::memcpy(sub, c_str() + begin, length * sizeof(_C));
        return DynStringT<_C>(sub, length);
    }

    return DynStringT<_C>(length, c_str() + begin);
}

template<typename _C>
inline DynStringT<_C> DynStringT<_C>::subString(const uSys from) const noexcept
{
    if(from > _length)
    {
        return DynStringT<_C>("");
    }

    const uSys length = _length - from;

    if(length >= 16)
    {
        _C* const sub = new(::std::nothrow) _C[length + 1];
        sub[length] = '\0';
        ::std::memcpy(sub, c_str() + from, length * sizeof(_C));
        return DynStringT<_C>(sub, length);
    }

    return DynStringT<_C>(length, c_str() + from);
}

template<typename _C>
inline _C DynStringT<_C>::operator[](const uSys index) const noexcept
{ return c_str()[index]; }

template<typename _C>
inline _C DynStringT<_C>::at(const uSys index) const noexcept
{
    if(index >= _length) { return '\0'; }
    return c_str()[index];
}

template<typename _C>
inline DynStringViewT<_C>::DynStringViewT(const uSys begin, const uSys length, const DynStringT<_C>& str) noexcept
    : _string(str.c_str() + begin)
    , _refCount(null)
    , _length(length)
    , _hash(findHashCode(_string, length))
{
    if(str._length >= 16)
    {
        _refCount = str._largeString.refCount;
        ++(*_refCount);
    }
}

template<typename _C>
inline DynStringViewT<_C>::DynStringViewT(const uSys begin, const uSys length, const DynStringViewT<_C>& str) noexcept
    : _string(str.c_str() + begin)
    , _refCount(str._refCount)
    , _length(length)
    , _hash(findHashCode(_string, length))
{
    if(_refCount) 
    { ++(*_refCount); }
}

template<typename _C>
inline DynStringViewT<_C>::DynStringViewT(const DynStringT<_C>& str, const uSys begin, const uSys end) noexcept
    : _string(str.c_str() + begin)
    , _refCount(null)
    , _length(end - begin)
    , _hash(findHashCode(_string, _length))
{
    if(str._length >= 16)
    {
        _refCount = str._largeString.refCount;
        ++(*_refCount);
    }
}

template<typename _C>
inline DynStringViewT<_C>::DynStringViewT(const DynStringViewT<_C>& str, const uSys begin, const uSys end) noexcept
    : _string(str.c_str() + begin)
    , _refCount(str._refCount)
    , _length(end - begin)
    , _hash(findHashCode(_string, _length))
{
    if(_refCount) 
    { ++(*_refCount); }
}

template<typename _C>
inline DynStringViewT<_C>::~DynStringViewT() noexcept
{
    if(_refCount && --(*_refCount) == 0)
    {
        delete _refCount;
        // Was this allocated as a single block.
        if(reinterpret_cast<iPtr>(_refCount) != reinterpret_cast<iPtr>(_string) - static_cast<iPtr>(sizeof(uSys)))
        {
            delete[] _string;
        }
    }
}

template<typename _C>
inline DynStringViewT<_C>::DynStringViewT(const DynStringViewT<_C>& copy) noexcept
    : _string(copy._string)
    , _refCount(copy._refCount)
    , _length(copy._length)
    , _hash(copy._hash)
{
    if(_refCount) 
    { ++(*_refCount); }
}

template<typename _C>
inline DynStringViewT<_C>::DynStringViewT(DynStringViewT<_C>&& move) noexcept
    : _string(move._string)
    , _refCount(move._refCount)
    , _length(move._length)
    , _hash(move._hash)
{ move._refCount = null; }

template<typename _C>
inline DynStringViewT<_C>& DynStringViewT<_C>::operator=(const DynStringViewT<_C>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    if(_refCount && --(*_refCount) == 0)
    {
        delete _refCount;
        // Was this allocated as a single block.
        if(reinterpret_cast<iPtr>(_refCount) != reinterpret_cast<iPtr>(_string) - sizeof(uSys))
        {
            delete[] _string;
        }
    }

    _string = copy._string;
    _refCount = copy._refCount;
    _length = copy._length;
    _hash = copy._hash;

    if(_refCount) { ++(*_refCount); }

    return *this;
}

template<typename _C>
inline DynStringViewT<_C>& DynStringViewT<_C>::operator=(DynStringViewT<_C>&& move) noexcept
{
    if(this == &move)
    { return *this; }

    if(_refCount && --(*_refCount) == 0)
    {
        delete _refCount;
        // Was this allocated as a single block.
        if(reinterpret_cast<iPtr>(_refCount) != reinterpret_cast<iPtr>(_string) - sizeof(uSys))
        {
            delete[] _string;
        }
    }

    _string = move._string;
    _refCount = move._refCount;
    _length = move._length;
    _hash = move._hash;

    move._refCount = null;

    return *this;
}

template<typename _C>
inline DynStringViewT<_C> DynStringViewT<_C>::create(const DynStringT<_C>& str, const uSys begin, const uSys length) noexcept
{ return DynStringViewT(begin, length, str); }

template<typename _C>
inline DynStringViewT<_C> DynStringViewT<_C>::create(const DynStringViewT<_C>& str, const uSys begin, const uSys length) noexcept
{ return DynStringViewT(begin, length, str); }

template<typename _C>
inline DynStringViewT<_C>& DynStringViewT<_C>::reset(const DynStringT<_C>& str, const uSys begin, const uSys end) noexcept
{
    if(end > str._length)
    { return *this; }

    if(_refCount && --(*_refCount) == 0)
    {
        delete _refCount;
        // Was this allocated as a single block.
        if(reinterpret_cast<iPtr>(_refCount) != reinterpret_cast<iPtr>(_string) - sizeof(uSys))
        {
            delete[] _string;
        }
    }

    _string = str.c_str() + begin;
    if(str._length >= 16)
    {
        _refCount = str._largeString.refCount;
        ++(*_refCount);
    }
    else
    {
        _refCount = null;
    }
    _length = end - begin;
    _hash = findHashCode(_string, _length);

    return *this;
}

template<typename _C>
inline DynStringViewT<_C>& DynStringViewT<_C>::resetLen(const DynStringT<_C>& str, const uSys begin, const uSys length) noexcept
{
    if(begin + length > str._length)
    { return *this; }

    if(_refCount && --(*_refCount) == 0)
    {
        delete _refCount;
        // Was this allocated as a single block.
        if(reinterpret_cast<iPtr>(_refCount) != reinterpret_cast<iPtr>(_string) - sizeof(uSys))
        {
            delete[] _string;
        }
    }

    _string = str.c_str() + begin;
    if(str._length >= 16)
    {
        _refCount = str._largeString.refCount;
        ++(*_refCount);
    }
    else
    {
        _refCount = null;
    }
    _length = length;
    _hash = findHashCode(_string, _length);

    return *this;
}

template<typename _C>
inline bool DynStringViewT<_C>::equals(const DynStringT<_C>& other) const noexcept
{
    if(_length == other._length && _hash == other._hash)
    {
        return strCompare(_string, other.c_str()) == 0;
    }
    return false;
}

template<typename _C>
inline bool DynStringViewT<_C>::equals(const DynStringViewT<_C>& other) const noexcept
{
    if(this == &other) { return true; }
    if(_length == other._length && _hash == other._hash)
    {
        return strCompare(_string, other._string, _length) == 0;
    }
    return false;
}

template<typename _C>
inline bool DynStringViewT<_C>::equals(const _C* str) const noexcept
{
    return strCompare(_string, str, _length) == 0;
}

template<typename _C>
inline i32 DynStringViewT<_C>::compareTo(const DynStringViewT<_C>& other) const noexcept
{
    if(this == &other) { return 0; }
    return strCompare(_string, other._string, minT(_length, other._length));
}

template<typename _C>
inline i32 DynStringViewT<_C>::compareTo(const DynStringT<_C>& other) const noexcept
{
    return strCompare(_string, other.c_str(), minT(_length, other._length));
}

template<typename _C>
inline i32 DynStringViewT<_C>::compareTo(const _C* str) const noexcept
{
    return strCompare(_string, str, _length);
}

template<typename _C>
inline DynStringT<_C> DynStringViewT<_C>::concat(const DynStringT<_C>& other) const noexcept
{ return _concat(other._length, other._length >= 16 ? other._largeString.string : other._stackString); }

template<typename _C>
inline DynStringT<_C> DynStringViewT<_C>::concat(const DynStringViewT<_C>& other) const noexcept
{ return _concat(other._length, other._string); }

template<typename _C>
inline DynStringT<_C> DynStringViewT<_C>::concat(const _C* const other) const noexcept
{
    const uSys otherLen = strlen(other);
    return _concat(otherLen, other);
}

template<typename _C>
inline DynStringT<_C> DynStringViewT<_C>::_concat(const uSys len, const _C* const str) noexcept
{
    const uSys newLen = _length + len;
    if(newLen >= 16)
    {
        _C* const newStr = new(::std::nothrow) _C[newLen + 1];
        newStr[newLen] = '\0';
        ::std::memcpy(newStr, _string, _length * sizeof(_C));
        ::std::memcpy(newStr + _length, str, len * sizeof(_C));
        return DynStringT<_C>(newStr, newLen);
    }
    else
    {
        DynStringT<_C> tmp(newLen);
        tmp._stackString[newLen] = '\0';
        ::std::memcpy(tmp._stackString, _string, _length * sizeof(_C));
        ::std::memcpy(tmp._stackString + _length, str, len * sizeof(_C));
        tmp._hash = findHashCode(tmp._stackString);
        return tmp;
    }
}

template<typename _C>
inline DynStringT<_C> DynStringViewT<_C>::subString(const uSys begin, const uSys end) const noexcept
{
    if(begin >= end || end > _length)
    { return DynStringT<_C>(); }

    const uSys length = end - begin;

    if(length >= 16)
    {
        _C* const sub = new(::std::nothrow) _C[length + 1];
        sub[length] = '\0';
        ::std::memcpy(sub, c_str() + begin, length * sizeof(_C));
        return DynStringT<_C>(sub, length);
    }

    return DynStringT<_C>(length, c_str() + begin);
}

template<typename _C>
inline DynString DynStringViewT<_C>::subStringLen(const uSys begin, const uSys length) const noexcept
{
    if(begin + length > _length)
    { return DynStringT<_C>(""); }

    if(length >= 16)
    {
        _C* const sub = new(::std::nothrow) _C[length + 1];
        sub[length] = '\0';
        ::std::memcpy(sub, c_str() + begin, length * sizeof(_C));
        return DynStringT<_C>(sub, length);
    }

    return DynStringT<_C>(length, c_str() + begin);
}

template<typename _C>
inline DynString DynStringViewT<_C>::subString(const uSys from) const noexcept
{
    if(from > _length)
    { return DynStringT<_C>(""); }

    const uSys length = _length - from;

    if(length >= 16)
    {
        _C* const sub = new(::std::nothrow) _C[length + 1];
        sub[length] = '\0';
        ::std::memcpy(sub, c_str() + from, length * sizeof(_C));
        return DynStringT<_C>(sub, length);
    }

    return DynStringT<_C>(length, c_str() + from);
}

template<typename _C>
inline _C DynStringViewT<_C>::operator[](const uSys index) const noexcept
{ return _string[index]; }

template<typename _C>
inline _C DynStringViewT<_C>::at(const uSys index) const noexcept
{
    if(index >= _length) { return '\0'; }
    return _string[index];
}

template<typename _C>
inline StringBuilderT<_C>::StringBuilderT() noexcept
    : _string(new(::std::nothrow) _C[64])
    , _length(0)
    , _size(64)
{ }

template<typename _C>
inline StringBuilderT<_C>::StringBuilderT(const uSys initialSize) noexcept
    : _string(new(::std::nothrow) _C[maxT(initialSize, 1)])
    , _length(0)
    , _size(maxT(initialSize, 1))
{ }

template<typename _C>
inline StringBuilderT<_C>::StringBuilderT(const _C* const string) noexcept
    : _string(null)
    , _length(strLength(string))
    , _size(_length + 64)
{
    _C* const str = new(::std::nothrow) _C[_size];

    if(string)
    { ::std::memcpy(str, string, (_length + 1) * sizeof(_C)); }

    _string = str;
}

template<typename _C>
inline StringBuilderT<_C>::~StringBuilderT() noexcept
{ delete[] _string; }

template<typename _C>
inline StringBuilderT<_C>::StringBuilderT(const StringBuilderT<_C>& copy) noexcept
    : _string(new(::std::nothrow) _C[copy._size])
    , _length(copy._length)
    , _size(copy._size)
{ ::std::memcpy(_string, copy._string, _size); }

template<typename _C>
inline StringBuilderT<_C>::StringBuilderT(StringBuilderT<_C>&& move) noexcept
    : _string(move._string)
    , _length(move._length)
    , _size(move._size)
{ move._string = null; }

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::operator=(const StringBuilderT<_C>& copy) noexcept
{
    if(this == &copy)
    { return *this; }

    delete[] _string;

    _string = new(::std::nothrow) _C[copy._size];
    _length = copy._length;
    _size = copy._size;

    ::std::memcpy(_string, copy._string, _size * sizeof(_C));

    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::operator=(StringBuilderT<_C>&& move) noexcept
{
    if(this == &move)
    { return *this; } 

    delete[] _string;

    _string = move._string;
    _length = move._length;
    _size = move._size;

    move._string = null;

    return *this;
}

template<typename _C>
inline bool StringBuilderT<_C>::equals(const StringBuilderT<_C>& other) const noexcept
{
    if(_string == other._string) { return true; }
    if(_length == other._length)
    {
        return strCompare(_string, other._string) == 0;
    }
    return false;
}

template<typename _C>
inline i32 StringBuilderT<_C>::compareTo(const StringBuilderT<_C>& other) const noexcept
{
    if(_string == other._string) { return 0; }
    return strCompare(_string, other._string);
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::append(const ConstExprStringT<_C>& string) noexcept
{
    append(string._string, string._length);
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::append(const DynStringT<_C>& string) noexcept
{
    append(string.c_str(), string._length);
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::append(const DynStringViewT<_C>& string) noexcept
{
    append(string._string, string._length);
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::append(const StringBuilderT<_C>& string) noexcept
{
    append(string._string, string._length);
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::append(const _C* const string) noexcept
{
    append(string, strLength(string));
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::append(const _C c) noexcept
{
    _C string[2] = { c,_C{ '\0' } };
    append(string, 1);
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::reset() noexcept
{
    _length = 0;
    _string[0] = '\0';
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::reset(const uSys newSize) noexcept
{
    if(newSize == 0) 
    { return *this; }
    delete[] _string;
    _string = new(::std::nothrow) _C[newSize];
    _size = newSize;
    _length = 0;
    _string[0] = '\0';
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::resetIf(const uSys maxSize, const uSys newSize) noexcept
{
    if(newSize == 0) { return *this; }
    if(maxSize <= newSize) { return *this; }
    if(_size >= maxSize)
    {
        delete[] _string;
        _string = new(::std::nothrow) _C[newSize];
        _size = newSize;
    }
    _length = 0;
    _string[0] = '\0';
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::backspace() noexcept
{
    if(_length > 0)
    {
        --_length;
        _string[_length] = '\0';
    }
    return *this;
}

template<typename _C>
inline StringBuilderT<_C>& StringBuilderT<_C>::backspace(const uSys count) noexcept
{
    if(_length > 0)
    {
        const uSys newLen = _length - count;
        if(newLen > _length)
        { return *this; }

        _length = newLen;
        _string[_length] = '\0';
    }
    return *this;
}

template<typename _C>
inline _C StringBuilderT<_C>::operator[](const uSys index) const noexcept
{ return _string[index]; }

template<typename _C>
inline _C StringBuilderT<_C>::at(const uSys index) const noexcept
{
    if(index >= _length) { return '\0'; }
    return _string[index];
}

template<typename _C>
inline void StringBuilderT<_C>::append(const _C* const string, const uSys length) noexcept
{
    const uSys newLen = _length + length;
    if(newLen >= _size)
    {
        const uSys newSize = newLen + (newLen >> 1);
        _C* const newStr = new(::std::nothrow) _C[newSize];
        ::std::memcpy(newStr, _string, (_length + 1) * sizeof(_C));
        _string = newStr;
        _size = newSize;
    }
    ::std::memcpy(_string + _length, string, length * sizeof(_C));
    _length += length;
    _string[_length] = '\0';
}

template<typename _CTo, typename _CFrom>
DynStringT<_CTo> StringCast(const DynStringT<_CFrom>&) noexcept
{ return DynStringT<_CTo>(); }

template<>
inline DynStringT<char> StringCast(const DynStringT<char>& string) noexcept
{ return string; }

template<>
inline DynStringT<wchar_t> StringCast(const DynStringT<wchar_t> & string) noexcept
{ return string; }

template<>
inline DynStringT<c8> StringCast(const DynStringT<c8> & string) noexcept
{ return string; }

template<>
inline DynStringT<c16> StringCast(const DynStringT<c16> & string) noexcept
{ return string; }

template<>
inline DynStringT<c32> StringCast(const DynStringT<c32> & string) noexcept
{ return string; }

template<>
inline DynStringT<wchar_t> StringCast<wchar_t, char>(const DynStringT<char>& string) noexcept
{
    const char* rawStr = string.c_str();

    ::std::mbstate_t state;
    const uSys len = ::std::mbsrtowcs(null, &rawStr, 0, &state) + 1;

    wchar_t* const newStr = new(::std::nothrow) wchar_t[len];
    ::std::mbsrtowcs(newStr, &rawStr, len, &state);

    return DynStringT<wchar_t>::passControl(newStr);
}

template<>
inline DynStringT<char> StringCast<char, wchar_t>(const DynStringT<wchar_t>& string) noexcept
{
    const wchar_t* rawStr = string.c_str();

    ::std::mbstate_t state;
    const uSys len = ::std::wcsrtombs(null, &rawStr, 0, &state) + 1;

    char* const newStr = new(::std::nothrow) char[len];
    ::std::wcsrtombs(newStr, &rawStr, len, &state);

    return DynStringT<char>::passControl(newStr);
}

namespace std
{
template<typename _C>
struct hash<ConstExprStringT<_C>>
{
    inline uSys operator()(const ConstExprStringT<_C>& str) const noexcept
    { return str.hashCode(); }
};

template<typename _C>
struct hash<DynStringT<_C>>
{
    inline uSys operator()(const DynStringT<_C>& str) const noexcept
    { return str.hashCode(); }
};

template<typename _C>
struct hash<DynStringViewT<_C>>
{
    inline uSys operator()(const DynStringViewT<_C>& str) const noexcept
    { return str.hashCode(); }
};
}
