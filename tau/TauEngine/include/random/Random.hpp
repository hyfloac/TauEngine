#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>

template<typename RngT>
class Random final
{
    DEFAULT_DESTRUCT(Random);
    DEFAULT_CM_PU(Random);
public:
    using Seed_t = RngT::Seed_t;
    using Rand_t = RngT::Rand_t;
public:
    Random(RngT& rng) noexcept
        : m_Rng(rng)
    { }

    [[nodiscard]] Seed_t Seed() const noexcept { return m_Rng.Seed(); }
    [[nodiscard]] Seed_t& Seed() noexcept { return m_Rng.Seed(); }

    [[nodiscard]] Seed_t seed() const noexcept { return m_Rng.seed(); }
    void seed(const Seed_t seed) const noexcept { m_Rng.seed(seed); }

    [[nodiscard]] u64 Rand() noexcept { return m_Rng.Rand(); }
    [[nodiscard]] u64 rand() noexcept { return Rand(); }

    [[nodiscard]] u8  randomU8 () noexcept { return static_cast<u8> (m_Rng.Rand()); }
    [[nodiscard]] i8  randomI8 () noexcept { return static_cast<i8> (m_Rng.Rand()); }
    [[nodiscard]] u16 randomU16() noexcept { return static_cast<u16>(m_Rng.Rand()); }
    [[nodiscard]] i16 randomI16() noexcept { return static_cast<i16>(m_Rng.Rand()); }
    [[nodiscard]] u32 randomU32() noexcept { return static_cast<u32>(m_Rng.Rand()); }
    [[nodiscard]] i32 randomI32() noexcept { return static_cast<i32>(m_Rng.Rand()); }
    [[nodiscard]] u64 randomU64() noexcept { return static_cast<u64>(m_Rng.Rand()); }
    [[nodiscard]] i64 randomI64() noexcept { return static_cast<i64>(m_Rng.Rand()); }

    [[nodiscard]] u8  randomU8 (const u8  maxInclusive) noexcept { return static_cast<u8> (m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] i8  randomI8 (const i8  maxInclusive) noexcept { return static_cast<i8> (m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] u16 randomU16(const u16 maxInclusive) noexcept { return static_cast<u16>(m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] i16 randomI16(const i16 maxInclusive) noexcept { return static_cast<i16>(m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] u32 randomU32(const u32 maxInclusive) noexcept { return static_cast<u32>(m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] i32 randomI32(const i32 maxInclusive) noexcept { return static_cast<i32>(m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] u64 randomU64(const u64 maxInclusive) noexcept { return static_cast<u64>(m_Rng.Rand() % (maxInclusive + 1)); }
    [[nodiscard]] i64 randomI64(const i64 maxInclusive) noexcept { return static_cast<i64>(m_Rng.Rand() % (maxInclusive + 1)); }

    [[nodiscard]] u8  randomU8E (const u8  maxExclusive) noexcept { return static_cast<u8> (m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] i8  randomI8E (const i8  maxExclusive) noexcept { return static_cast<i8> (m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] u16 randomU16E(const u16 maxExclusive) noexcept { return static_cast<u16>(m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] i16 randomI16E(const i16 maxExclusive) noexcept { return static_cast<i16>(m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] u32 randomU32E(const u32 maxExclusive) noexcept { return static_cast<u32>(m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] i32 randomI32E(const i32 maxExclusive) noexcept { return static_cast<i32>(m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] u64 randomU64E(const u64 maxExclusive) noexcept { return static_cast<u64>(m_Rng.Rand() % maxExclusive); }
    [[nodiscard]] i64 randomI64E(const i64 maxExclusive) noexcept { return static_cast<i64>(m_Rng.Rand() % maxExclusive); }

    [[nodiscard]] u8  randomU8 (const u8  minInclusive, const u8  maxInclusive) noexcept { return static_cast<u8> (m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] i8  randomI8 (const i8  minInclusive, const i8  maxInclusive) noexcept { return static_cast<i8> (m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] u16 randomU16(const u16 minInclusive, const u16 maxInclusive) noexcept { return static_cast<u16>(m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] i16 randomI16(const i16 minInclusive, const i16 maxInclusive) noexcept { return static_cast<i16>(m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] u32 randomU32(const u32 minInclusive, const u32 maxInclusive) noexcept { return static_cast<u32>(m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] i32 randomI32(const i32 minInclusive, const i32 maxInclusive) noexcept { return static_cast<i32>(m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] u64 randomU64(const u64 minInclusive, const u64 maxInclusive) noexcept { return static_cast<u64>(m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }
    [[nodiscard]] i64 randomI64(const i64 minInclusive, const i64 maxInclusive) noexcept { return static_cast<i64>(m_Rng.Rand() % (maxInclusive - minInclusive + 1) + minInclusive); }

    [[nodiscard]] u8  randomU8E (const u8  minInclusive, const u8  maxExclusive) noexcept { return static_cast<u8> (m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] i8  randomI8E (const i8  minInclusive, const i8  maxExclusive) noexcept { return static_cast<i8> (m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] u16 randomU16E(const u16 minInclusive, const u16 maxExclusive) noexcept { return static_cast<u16>(m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] i16 randomI16E(const i16 minInclusive, const i16 maxExclusive) noexcept { return static_cast<i16>(m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] u32 randomU32E(const u32 minInclusive, const u32 maxExclusive) noexcept { return static_cast<u32>(m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] i32 randomI32E(const i32 minInclusive, const i32 maxExclusive) noexcept { return static_cast<i32>(m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] u64 randomU64E(const u64 minInclusive, const u64 maxExclusive) noexcept { return static_cast<u64>(m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }
    [[nodiscard]] i64 randomI64E(const i64 minInclusive, const i64 maxExclusive) noexcept { return static_cast<i64>(m_Rng.Rand() % (maxExclusive - minInclusive) + minInclusive); }

    [[nodiscard]] f32 randomF32() noexcept { return static_cast<f32>(m_Rng.Rand()) / static_cast<f32>(RngT::MaxValue()); }
    [[nodiscard]] f64 randomF64() noexcept { return static_cast<f64>(m_Rng.Rand()) / static_cast<f64>(RngT::MaxValue()); }

    [[nodiscard]] f32 randomF32(const f32 maxInclusive) noexcept { return randomF32() * maxInclusive; }
    [[nodiscard]] f64 randomF64(const f64 maxInclusive) noexcept { return randomF64() * maxInclusive; }

    [[nodiscard]] f32 randomF32(const f32 minInclusive, const f32 maxInclusive) noexcept { return randomF32() * (maxInclusive - minInclusive) + minInclusive; }
    [[nodiscard]] f64 randomF64(const f64 minInclusive, const f64 maxInclusive) noexcept { return randomF64() * (maxInclusive - minInclusive) + minInclusive; }
private:
    RngT& m_Rng;
};
