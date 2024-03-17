#pragma once

#if defined(ENUM_BITFIELDS_IN_DEV)
#include "EnumBitFields.hpp"
#endif

template<typename _T>
inline _T operator ~(const _T a) noexcept
{ return static_cast<_T>(~static_cast<u64>(a)); }

template<typename _T>
inline _T operator |(const _T a, const _T b) noexcept
{ return static_cast<_T>(static_cast<u64>(a) | static_cast<u64>(b)); }

template<typename _T>
inline _T operator &(const _T a, const _T b) noexcept
{ return static_cast<_T>(static_cast<u64>(a) & static_cast<u64>(b)); }

template<typename _T>
inline _T operator ^(const _T a, const _T b) noexcept
{ return static_cast<_T>(static_cast<u64>(a) ^ static_cast<u64>(b)); }

template<typename _T> 
inline _T& operator |=(_T& a, const _T& b) noexcept
{
    a = a | b;
    return a;
}

template<typename _T> 
inline _T& operator &=(_T& a, const _T& b) noexcept
{
    a = a & b;
    return a;
}

template<typename _T> 
inline _T& operator ^=(_T& a, const _T& b) noexcept
{
    a = a ^ b;
    return a;
}

template<typename _Int, typename _Enum> 
inline _Int operator |(const _Int a, const _Enum b) noexcept
{ return a | static_cast<_Int>(b); }

template<typename _Int, typename _Enum> 
inline _Int operator &(const _Int a, const _Enum b) noexcept
{ return a & static_cast<_Int>(b); }

template<typename _Int, typename _Enum> 
inline _Int operator ^(const _Int a, const _Enum b) noexcept
{ return a ^ static_cast<_Int>(b); }

template<typename _Int, typename _Enum> 
inline _Int& operator |=(_Int& a, const _Enum& b) noexcept
{
    a = a | b;
    return a;
}

template<typename _Int, typename _Enum> 
inline _Int& operator &=(_Int& a, const _Enum& b) noexcept
{
    a = a & b;
    return a;
}

template<typename _Int, typename _Enum> 
inline _Int& operator ^=(_Int& a, const _Enum& b) noexcept
{
    a = a ^ b;
    return a;
}

template<typename _T> 
inline bool  hasFlag(const _T in, const _T flag) noexcept
{ return (static_cast<u64>(in) & static_cast<u64>(flag)) != 0; }

template<typename _T> 
inline _T setFlag(const _T& in, const _T flag) noexcept
{ return in | flag; }

template<typename _T> 
inline _T& setFlag(_T& in, const _T flag) noexcept
{ return in |= flag; }

template<typename _T> 
inline _T unsetFlag(const _T& in, const _T flag) noexcept
{ return in & ~flag; }

template<typename _T> 
inline _T& unsetFlag(_T& in, const _T flag) noexcept
{ return in &= ~flag; }

template<typename _Int, typename _Enum>
bool hasFlag(const _Int in, const _Enum flag) noexcept
{ return (in & static_cast<_Int>(flag)) != 0; }

template<typename _Int, typename _Enum>
inline _Int setFlag(const _Int& in, const _Enum flag) noexcept
{ return in | static_cast<_Int>(flag); }

template<typename _Int, typename _Enum>
inline _Int& setFlag(_Int& in, const _Enum flag) noexcept
{ return in |= static_cast<_Int>(flag); }

template<typename _Int, typename _Enum>
inline _Int unsetFlag(const _Int& in, const _Enum flag) noexcept
{ return in & ~static_cast<_Int>(flag); }

template<typename _Int, typename _Enum>
inline _Int& unsetFlag(_Int& in, const _Enum flag) noexcept
{ return in &= ~static_cast<_Int>(flag); }

template<typename _T>
inline _T setFlag(const _T& in, const _T flag, const bool enable) noexcept
{
    if(enable)
    { return setFlag(in, flag); }
    else
    { return unsetFlag(in, flag); }
}

template<typename _T>
inline _T setFlag(_T& in, const _T flag, const bool enable) noexcept
{
    if(enable)
    { return setFlag(in, flag); }
    else
    { return unsetFlag(in, flag); }
}

template<typename _Int, typename _Enum>
inline _Int setFlag(const _Int& in, const _Enum flag, const bool enable) noexcept
{
    if(enable)
    { return setFlag(in, flag); }
    else
    { return unsetFlag(in, flag); }
}

template<typename _Int, typename _Enum>
inline _Int setFlag(_Int& in, const _Enum flag, const bool enable) noexcept
{
    if(enable)
    { return setFlag(in, flag); }
    else
    { return unsetFlag(in, flag); }
}
