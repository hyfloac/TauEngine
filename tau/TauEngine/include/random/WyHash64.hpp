#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <ctime>

#ifndef __is_identifier
  #define __is_identifier(_X) 0
#endif

/**
 * WyHash64 requires the __uint128_t type.
 *
 *   Despite its name it is not an arithmetic type. Instead
 * it is used to interface with x86 saturated multiply. Even
 * if the compiler optimization isn't available WyHash64
 * will still work, albeit with worse randomness.
 */
#if !__is_identifier(__uint128_t)
  #define __uint128_t u64
#endif

class WyHash64 final
{
    DEFAULT_DESTRUCT(WyHash64);
    DEFAULT_CM(WyHash64);
private:
    u64 _seed;
public:
    WyHash64() noexcept
        : _seed(time(NULL))
    { }

    WyHash64(const u64 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] u64 seed() const noexcept { return _seed; }
    void seed(u64 seed) noexcept { _seed = seed; }

    [[nodiscard]] u64 rand() noexcept
    {
        _seed += 0x60bee2bee120fc15;
        const __uint128_t tmp0 = (__uint128_t) _seed * 0xa3b195354a39b70d;
        const u64 m0 = (tmp0 >> 64) ^ tmp0;
        const __uint128_t tmp1 = (__uint128_t) m0 * 0x1b03738712fad5c9;
        const u64 m1 = (tmp1 >> 64) ^ tmp1;
        return m1;
    }
};