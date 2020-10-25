#pragma once

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

typedef float  single;
typedef float  f32;
typedef double f64;

#if defined(_MSVC_LANG) && 1
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

constexpr uSys operator "" _uz(const unsigned long long n) noexcept
{ return n; }

constexpr iSys operator "" _iz(const unsigned long long n) noexcept
{ return n; }

template<typename _T>
struct IntMaxMin final
{
    static constexpr _T Min = 0;
    static constexpr _T Max = 0;
};

template<>
struct IntMaxMin<i8> final
{
    static constexpr i8 Min = INT8_MIN;
    static constexpr i8 Max = INT8_MAX;
};

template<>
struct IntMaxMin<i16> final
{
    static constexpr i16 Min = INT16_MIN;
    static constexpr i16 Max = INT16_MAX;
};

template<>
struct IntMaxMin<i32> final
{
    static constexpr i32 Min = INT32_MIN;
    static constexpr i32 Max = INT32_MAX;
};

template<>
struct IntMaxMin<i64> final
{
    static constexpr i64 Min = INT64_MIN;
    static constexpr i64 Max = INT64_MAX;
};

template<>
struct IntMaxMin<u8> final
{
    static constexpr u8 Min = 0;
    static constexpr u8 Max = UINT8_MAX;
};

template<>
struct IntMaxMin<u16> final
{
    static constexpr u16 Min = 0;
    static constexpr u16 Max = UINT16_MAX;
};

template<>
struct IntMaxMin<u32> final
{
    static constexpr u32 Min = 0;
    static constexpr u32 Max = UINT32_MAX;
};

template<>
struct IntMaxMin<u64> final
{
    static constexpr u64 Min = 0;
    static constexpr u64 Max = UINT64_MAX;
};
