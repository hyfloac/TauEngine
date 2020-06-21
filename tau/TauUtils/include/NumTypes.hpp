#pragma once

#ifndef NUM_TYPES_HPP
#define NUM_TYPES_HPP

#pragma warning(push, 0)
#include <cstddef>
#include <cstdint>
#pragma warning(pop)

typedef ::std::int8_t   i8;
typedef ::std::int16_t  i16;
typedef ::std::int32_t  i32;
typedef ::std::int64_t  i64;

typedef ::std::uint8_t  u8;
typedef ::std::uint16_t u16;
typedef ::std::uint32_t u32;
typedef ::std::uint64_t u64;

#if !defined(__cpp_char8_t)
enum char8_t : unsigned char { };
#endif

typedef char8_t  c8;
typedef char16_t c16;
typedef char32_t c32;
typedef wchar_t  cUnicode;

typedef float  single;
typedef float  f32;
typedef double f64;

#ifdef _MSVC_LANG
  #if defined(_WIN64) || (defined(_M_X64) && _M_X64 == 100)
    typedef i64 iSys;
    typedef u64 uSys;
  #else
    typedef i32 iSys;
    typedef u32 uSys;
  #endif
#else
  typedef ::std::ptrdiff_t iSys;
  typedef ::std::size_t    uSys;
#endif

typedef ::std::intptr_t  iPtr;
typedef ::std::uintptr_t uPtr;

template<typename _T>
struct IntMaxMin final
{
    static _T Min() noexcept { return 0; }
    static _T Max() noexcept { return 0; }
};

template<>
struct IntMaxMin<i8> final
{
    static i8 Min() noexcept { return INT8_MIN; }
    static i8 Max() noexcept { return INT8_MAX; }
};

template<>
struct IntMaxMin<i16> final
{
    static i16 Min() noexcept { return INT16_MIN; }
    static i16 Max() noexcept { return INT16_MAX; }
};

template<>
struct IntMaxMin<i32> final
{
    static i32 Min() noexcept { return INT32_MIN; }
    static i32 Max() noexcept { return INT32_MAX; }
};

template<>
struct IntMaxMin<i64> final
{
    static i64 Min() noexcept { return INT64_MIN; }
    static i64 Max() noexcept { return INT64_MAX; }
};

template<>
struct IntMaxMin<u8> final
{
    static u8 Min() noexcept { return 0; }
    static u8 Max() noexcept { return UINT8_MAX; }
};

template<>
struct IntMaxMin<u16> final
{
    static u16 Min() noexcept { return 0; }
    static u16 Max() noexcept { return UINT16_MAX; }
};

template<>
struct IntMaxMin<u32> final
{
    static u32 Min() noexcept { return 0; }
    static u32 Max() noexcept { return UINT32_MAX; }
};

template<>
struct IntMaxMin<u64> final
{
    static u64 Min() noexcept { return 0; }
    static u64 Max() noexcept { return UINT64_MAX; }
};

#endif
