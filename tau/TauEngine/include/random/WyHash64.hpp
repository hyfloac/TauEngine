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
 *   Despite its name it is not an arithmetic type. Instead,
 * it is used to interface with x86 saturated multiply. Even
 * if the compiler optimization isn't available WyHash64
 * will still work, albeit with worse randomness.
 */
#if !__is_identifier(__uint128_t) && !defined(__uint128_t)
  #define __uint128_t u64
#endif

class WyHash64 final
{
    DEFAULT_DESTRUCT(WyHash64);
    DEFAULT_CM_PU(WyHash64);
public:
    using Seed_t = u64;
    using Rand_t = u64;
public:
    WyHash64() noexcept
        : m_Seed(time(nullptr))
    { }

    WyHash64(const Seed_t seed) noexcept
        : m_Seed(seed)
    { }

    [[nodiscard]] Seed_t Seed() const noexcept { return m_Seed; }
    [[nodiscard]] Seed_t& Seed() noexcept { return m_Seed; }

    [[nodiscard]] Seed_t seed() const noexcept { return m_Seed; }
    void seed(const Seed_t seed) noexcept { m_Seed = seed; }

    [[nodiscard]] Rand_t Rand() noexcept
    {
        m_Seed += 0x60bee2bee120fc15;
        const __uint128_t tmp0 = static_cast<__uint128_t>(m_Seed * 0xa3b195354a39b70d);
        const u64 m0 = (tmp0 >> 64) ^ tmp0;
        const __uint128_t tmp1 = static_cast<__uint128_t>(m0) * 0x1b03738712fad5c9;
        const u64 m1 = (tmp1 >> 64) ^ tmp1;
        return m1;
    }

    [[nodiscard]] Rand_t rand() noexcept { return Rand(); }
private:
    Seed_t m_Seed;
};