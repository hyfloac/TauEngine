#pragma once

#pragma warning(push, 0)
#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
#else
#include <stdint.h>
#include <stddef.h>
#endif
#pragma warning(pop)

#include <CVersion.hpp>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#if !defined(__CPP_20) || 0
// typedef char char8_t;
#endif

typedef char8_t  c8;
typedef char16_t c16;
typedef char32_t c32;
typedef wchar_t  cUnicode;

typedef float    single;
typedef float    f32;
typedef double   f64;

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
