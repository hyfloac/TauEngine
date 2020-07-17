#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <ctime>

class FastRand16 final
{
    DEFAULT_DESTRUCT(FastRand16);
    DEFAULT_CM(FastRand16);
public:
    static u32 MaxValue() noexcept { return 0x7FFF; }
private:
    u32 _seed;
public:
    FastRand16() noexcept
        : _seed(time(NULL))
    { }

    FastRand16(const u32 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] u32 seed() const noexcept { return _seed; }
    void seed(const u32 seed) noexcept { _seed = seed; }

    [[nodiscard]] u32 rand() noexcept
    {
        _seed = 214013 * _seed + 2531011;
        return (_seed >> 16) & 0x7FFF;
    }
};

class FastRand32 final
{
    DEFAULT_DESTRUCT(FastRand32);
    DEFAULT_CM(FastRand32);
public:
    static u32 MaxValue() noexcept { return IntMaxMin<u32>::Max; }
private:
    u32 _seed;
public:
    FastRand32() noexcept
        : _seed(time(NULL))
    { }

    FastRand32(const u32 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] u32 seed() const noexcept { return _seed; }
    void seed(const u32 seed) noexcept { _seed = seed; }

    [[nodiscard]] u32 rand() noexcept
    {
        _seed = 214013 * _seed + 2531011;
        return _seed;
    }
};

class FastRand64 final
{
    DEFAULT_DESTRUCT(FastRand64);
    DEFAULT_CM(FastRand64);
public:
    static u64 MaxValue() noexcept { return IntMaxMin<u64>::Max; }
private:
    u64 _seed;
public:
    FastRand64() noexcept
        : _seed(time(NULL))
    { }

    FastRand64(const u64 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] u64 seed() const noexcept { return _seed; }
    void seed(const u64 seed) noexcept { _seed = seed; }

    [[nodiscard]] u64 rand() noexcept
    {
        _seed = 214013 * _seed + 2531011;
        return _seed;
    }
};
