#pragma once

#ifdef TAU_NUMTYPES_USE_CSTDLIB
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
#else
  typedef signed char      i8;
  typedef signed short     i16;
  typedef signed int       i32;
  typedef signed long long i64;
  
  typedef unsigned char      u8;
  typedef unsigned short     u16;
  typedef unsigned int       u32;
  typedef unsigned long long u64;

  #define INT8_MIN         (-127i8 - 1)
  #define INT16_MIN        (-32767i16 - 1)
  #define INT32_MIN        (-2147483647i32 - 1)
  #define INT64_MIN        (-9223372036854775807i64 - 1)
  #define INT8_MAX         127i8
  #define INT16_MAX        32767i16
  #define INT32_MAX        2147483647i32
  #define INT64_MAX        9223372036854775807i64
  #define UINT8_MAX        0xffui8
  #define UINT16_MAX       0xffffui16
  #define UINT32_MAX       0xffffffffui32
  #define UINT64_MAX       0xffffffffffffffffui64
#endif

// Boolean bit masks
typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#if !defined(__cpp_char8_t)
  enum char8_t : unsigned char { };
#endif

typedef char8_t  c8;
typedef char16_t c16;
typedef char32_t c32;

typedef float  single;
typedef float  f32;
typedef double f64;

#if defined(_MSVC_LANG) || defined(TAU_NUMTYPES_USE_CSTDLIB) || 1
  #if defined(_WIN64) || (defined(_M_X64) && _M_X64 == 100)
    typedef i64 iSys;
    typedef u64 uSys;

    typedef i64 iPtr;
    typedef u64 uPtr;
  #else
    typedef i32 iSys;
    typedef u32 uSys;

    typedef i32 iPtr;
    typedef u32 uPtr;
  #endif
#else
  typedef ::std::ptrdiff_t iSys;
  typedef ::std::size_t    uSys;

  typedef ::std::intptr_t  iPtr;
  typedef ::std::uintptr_t uPtr;
#endif

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
