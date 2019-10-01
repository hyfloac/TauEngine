#pragma warning(push, 0)
#include <climits>
#include <cstdio>
#pragma warning(pop)
#include <TUID.hpp>
#include <FastRand.hpp>

// static inline u32 rotL32(u32 n, u32 c)
// {
//     const u32 mask = (CHAR_BIT * sizeof(n) - 1);  // assumes width is a power of 2.
//
//     c &= mask;
//     const u32 negC = static_cast<u32>(-static_cast<i32>(c));
//     return (n << c) | (n >> (negC & mask));
// }

static inline u32 rotR32(u32 n, u32 c)
{
    const u32 mask = (CHAR_BIT * sizeof(n) - 1);

    c &= mask;
    const u32 negC = static_cast<u32>(-static_cast<i32>(c));
    return (n >> c) | (n << (negC & mask));
}

TUID TUID::generate() noexcept 
{
    static u32 s_incremental = 0;

    const u32 inc = s_incremental++;

    u64 b0 = fastRand();
    u32 b1 = fastRand();
    u64 b2 = fastRand();
    u32 b3 = fastRand();

    b0 ^= b1 >> 8;
    b0 ^= b2 << 14;

    b0 &= 0xFFFF00FF;
    b0 |= inc & 0x0000FF00;
    b3 = rotR32(b3, 9);

    b2 &= 0xFF00FF00;
    b2 |= inc & 0x00FF00FF;
    b3 ^= b2 ^ (b1 >> 3);

    b1 &= 0x00FFFFFF;
    b1 |= inc & 0xFF000000;

    return TUID((b0 << 32) | b1, (b2 << 32) | b3);
}

TUID::TUID(u64 highBits, u64 lowBits) noexcept
    : _highBits(highBits), _lowBits(lowBits)
{ }

void TUID::toString(char str[38]) const noexcept
{
    const u32 b0 = _highBits >> 32;
    const u32 b1 = _highBits & 0xFFFFFFFF;
    const u32 b2 = _lowBits >> 32;
    const u32 b3 = _lowBits & 0xFFFFFFFF;
    sprintf(str, "%.4X-%.8X-%.6X-%.4X-%.4X-%.6X",
        b0 >> 16, 
        (b0 << 16) | (b1 >> 16),
        ((b1 & 0xFFFF) << 8) | (b2 >> 24),
        (b2 & 0x00FFFF00) >> 8,
        ((b2 & 0xFF) << 8) | (b3 >> 24),
        b3 & 0x00FFFFFF);
    str[38] = '\0';
}

