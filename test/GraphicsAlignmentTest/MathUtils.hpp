#pragma once

#include <cstdint>
#include <cstddef>
#include <Windows.h>

typedef ::std::int32_t i32;
typedef ::std::int64_t i64;
typedef ::std::uint32_t u16;
typedef ::std::uint32_t u32;
typedef ::std::uint64_t u64;
typedef ::std::size_t uSys;

template<typename _T>
[[nodiscard]] constexpr inline _T _alignTo(const _T val, const _T alignment) noexcept
{
    if(alignment == 1)
    { return val; }
    return (val + alignment) & ~(alignment - 1);
}

[[nodiscard]] inline u32 _ctz(const u32 v) noexcept
{
    unsigned long trailingZero = 0;
    _BitScanForward(&trailingZero, v);
    return trailingZero;
}

[[nodiscard]] inline u32 _clz(const u32 v) noexcept
{
    unsigned long leadingZero = 0;
    _BitScanReverse(&leadingZero, v);
    return 31 - leadingZero;
}

[[nodiscard]] inline u64 _ctz(const u64 v) noexcept
{
    unsigned long trailingZero = 0;
    _BitScanForward64(&trailingZero, v);
    return trailingZero;
}

[[nodiscard]] inline u64 _clz(const u64 v) noexcept
{
    unsigned long leadingZero = 0;
    _BitScanReverse64(&leadingZero, v);
    return 63 - leadingZero;
}
