#pragma once

#include "NumTypes.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <intrin0.h>
// #include <winnt.h>
#pragma warning(pop)

template<typename _T>
_T atomicIncrement(volatile _T* t) noexcept = delete;

template<typename _T>
_T atomicDecrement(volatile _T* t) noexcept = delete;

template<typename _T>
_T atomicExchange(volatile _T* t, _T value) noexcept = delete;

template<>
inline i16 atomicIncrement<i16>(volatile i16* const t) noexcept
{ return _InterlockedIncrement16(t); }

template<>
inline i32 atomicIncrement<i32>(volatile i32* const t) noexcept
{
    static_assert(sizeof(i32) == sizeof(long), "NumTypes i32 [int] does not match the size of long.");
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
    static_assert(sizeof(u32) == sizeof(long), "NumTypes u32 [unsigned int] does not match the size of long.");
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
    static_assert(sizeof(i32) == sizeof(long), "NumTypes i32 [int] does not match the size of long.");
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
    static_assert(sizeof(u32) == sizeof(long), "NumTypes u32 [unsigned int] does not match the size of long.");
    return _InterlockedDecrement(reinterpret_cast<volatile long*>(t));
}

template<>
inline u64 atomicDecrement<u64>(volatile u64* const t) noexcept
{ return _InterlockedDecrement64(reinterpret_cast<volatile i64*>(t)); }

template<>
inline i8 atomicExchange<i8>(volatile i8* const t, const i8 value) noexcept
{ return _InterlockedExchange8(reinterpret_cast<volatile char*>(t), value); }

template<>
inline i16 atomicExchange<i16>(volatile i16* const t, const i16 value) noexcept
{ return _InterlockedExchange16(t, value); }

template<>
inline i32 atomicExchange<i32>(volatile i32* const t, const i32 value) noexcept
{
    static_assert(sizeof(i32) == sizeof(long), "NumTypes i32 [int] does not match the size of long.");
    return _InterlockedExchange(reinterpret_cast<volatile long*>(t), value);
}

template<>
inline i64 atomicExchange<i64>(volatile i64* const t, const i64 value) noexcept
{ return _InterlockedExchange64(t, value); }

template<>
inline u8 atomicExchange<u8>(volatile u8* const t, const u8 value) noexcept
{ return _InterlockedExchange8(reinterpret_cast<volatile char*>(t), value); }

template<>
inline u16 atomicExchange<u16>(volatile u16* const t, const u16 value) noexcept
{ return _InterlockedExchange16(reinterpret_cast<volatile i16*>(t), value); }

template<>
inline u32 atomicExchange<u32>(volatile u32* const t, const u32 value) noexcept
{
    static_assert(sizeof(u32) == sizeof(long), "NumTypes u32 [unsigned int] does not match the size of long.");
    return _InterlockedExchange(reinterpret_cast<volatile long*>(t), value);
}

template<>
inline u64 atomicExchange<u64>(volatile u64* const t, const u64 value) noexcept
{ return _InterlockedExchange64(reinterpret_cast<volatile i64*>(t), value); }

#endif
