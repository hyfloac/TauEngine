#pragma once

#include "NumTypes.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <intrin0.h>
#include <winnt.h>
#pragma warning(pop)

template<typename _T>
_T atomicIncrement(volatile _T* t) noexcept = delete;

template<typename _T>
_T atomicDecrement(volatile _T* t) noexcept = delete;

template<>
inline i16 atomicIncrement<i16>(volatile i16* const t) noexcept
{ return _InterlockedIncrement16(t); }

template<>
inline i32 atomicIncrement<i32>(volatile i32* const t) noexcept
{
    static_assert(sizeof(i32) == sizeof(long));
    return _InterlockedIncrement(reinterpret_cast<volatile long*>(t));
}

template<>
inline i64 atomicIncrement<i64>(volatile i64* const t) noexcept
{ return _InterlockedIncrement64(t); }

template<>
inline u16 atomicIncrement<u16>(volatile u16* const t) noexcept
{ return _InterlockedIncrement16(reinterpret_cast<volatile i16*>(t)); }

template<>
inline u32 atomicIncrement<u32>(volatile u32* const t) noexcept
{
    static_assert(sizeof(u32) == sizeof(long));
    return _InterlockedIncrement(reinterpret_cast<volatile long*>(t));
}

template<>
inline u64 atomicIncrement<u64>(volatile u64* const t) noexcept
{ return _InterlockedIncrement64(reinterpret_cast<volatile i64*>(t)); }

template<>
inline i16 atomicDecrement<i16>(volatile i16* const t) noexcept
{ return _InterlockedDecrement16(t); }

template<>
inline i32 atomicDecrement<i32>(volatile i32* const t) noexcept
{
    static_assert(sizeof(i32) == sizeof(long));
    return _InterlockedDecrement(reinterpret_cast<volatile long*>(t));
}

template<>
inline i64 atomicDecrement<i64>(volatile i64* const t) noexcept
{ return _InterlockedDecrement64(t); }

template<>
inline u16 atomicDecrement<u16>(volatile u16* const t) noexcept
{ return _InterlockedDecrement16(reinterpret_cast<volatile i16*>(t)); }

template<>
inline u32 atomicDecrement<u32>(volatile u32* const t) noexcept
{
    static_assert(sizeof(u32) == sizeof(long));
    return _InterlockedDecrement(reinterpret_cast<volatile long*>(t));
}

template<>
inline u64 atomicDecrement<u64>(volatile u64* const t) noexcept
{ return _InterlockedDecrement64(reinterpret_cast<volatile i64*>(t)); }
#endif
