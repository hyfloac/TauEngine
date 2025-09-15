#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <ctime>

class FastRand16 final
{
    DEFAULT_DESTRUCT(FastRand16);
    DEFAULT_CM_PU(FastRand16);
public:
    using Seed_t = u32;
    using Rand_t = u16;

    static Rand_t MaxValue() noexcept { return 0x7FFF; }
public:
    FastRand16() noexcept
        : m_Seed(time(nullptr))
    { }

    FastRand16(const Seed_t seed) noexcept
        : m_Seed(seed)
    { }

    [[nodiscard]] Seed_t Seed() const noexcept { return m_Seed; }
    [[nodiscard]] Seed_t& Seed() noexcept { return m_Seed; }

    [[nodiscard]] Seed_t seed() const noexcept { return m_Seed; }
    void seed(const Seed_t seed) noexcept { m_Seed = seed; }

    [[nodiscard]] Rand_t Rand() noexcept
    {
        m_Seed = 214013 * m_Seed + 2531011;
        return (m_Seed >> 16) & 0x7FFF;
    }

    [[nodiscard]] Rand_t rand() noexcept { return Rand(); }
private:
    Seed_t m_Seed;
};

class FastRand32 final
{
    DEFAULT_DESTRUCT(FastRand32);
    DEFAULT_CM_PU(FastRand32);
public:
    using Seed_t = u32;
    using Rand_t = u32;

    static Rand_t MaxValue() noexcept { return IntMaxMin<Rand_t>::Max; }
public:
    FastRand32() noexcept
        : m_Seed(time(nullptr))
    { }

    FastRand32(const Seed_t seed) noexcept
        : m_Seed(seed)
    { }

    [[nodiscard]] Seed_t Seed() const noexcept { return m_Seed; }
    [[nodiscard]] Seed_t& Seed() noexcept { return m_Seed; }

    [[nodiscard]] Seed_t seed() const noexcept { return m_Seed; }
    void seed(const Seed_t seed) noexcept { m_Seed = seed; }

    [[nodiscard]] Rand_t Rand() noexcept
    {
        m_Seed = 214013 * m_Seed + 2531011;
        return m_Seed;
    }

    [[nodiscard]] Rand_t rand() noexcept { return Rand(); }
private:
    Seed_t m_Seed;
};

class FastRand64 final
{
    DEFAULT_DESTRUCT(FastRand64);
    DEFAULT_CM_PU(FastRand64);
public:
    using Seed_t = u64;
    using Rand_t = u64;

    static Rand_t MaxValue() noexcept { return IntMaxMin<Rand_t>::Max; }
public:
    FastRand64() noexcept
        : m_Seed(time(nullptr))
    { }

    FastRand64(const Seed_t seed) noexcept
        : m_Seed(seed)
    { }

    [[nodiscard]] Seed_t Seed() const noexcept { return m_Seed; }
    [[nodiscard]] Seed_t& Seed() noexcept { return m_Seed; }

    [[nodiscard]] Seed_t seed() const noexcept { return m_Seed; }
    void seed(const Seed_t seed) noexcept { m_Seed = seed; }

    [[nodiscard]] Rand_t Rand() noexcept
    {
        m_Seed = 214013 * m_Seed + 2531011;
        return m_Seed;
    }

    [[nodiscard]] Rand_t rand() noexcept { return Rand(); }
private:
    Seed_t m_Seed;
};
