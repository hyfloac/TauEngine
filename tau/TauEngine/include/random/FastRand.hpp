#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <ctime>

class FastRand16 final
{
    DEFAULT_DESTRUCT(FastRand16);
    DEFAULT_COPY(FastRand16);
public:
    static u32 MaxValue() noexcept { return 0x7FFF; }
private:
    u32 _seed;
public:
    inline FastRand16() noexcept
        : _seed(time(NULL))
    { }

    inline FastRand16(const u32 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] inline u32 seed() const noexcept { return _seed; }
    inline void seed(u32 seed) noexcept { _seed = seed; }

    [[nodiscard]] inline u32 rand() noexcept
    {
        _seed = (214013 * _seed + 2531011);
        return (_seed >> 16) & 0x7FFF;
    }
};

class FastRand32 final
{
    DEFAULT_DESTRUCT(FastRand32);
    DEFAULT_COPY(FastRand32);
public:
    static u32 MaxValue() noexcept { return IntMaxMin<u32>::Max(); }
private:
    u32 _seed;
public:
    inline FastRand32() noexcept
        : _seed(time(NULL))
    { }

    inline FastRand32(const u32 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] inline u32 seed() const noexcept { return _seed; }
    inline void seed(u32 seed) noexcept { _seed = seed; }

    [[nodiscard]] inline u32 rand() noexcept
    {
        _seed = (214013 * _seed + 2531011);
        return _seed;
    }
};

class FastRand64 final
{
    DEFAULT_DESTRUCT(FastRand64);
    DEFAULT_COPY(FastRand64);
public:
    static u32 MaxValue() noexcept { return IntMaxMin<u64>::Max(); }
private:
    u64 _seed;
public:
    inline FastRand64() noexcept
        : _seed(time(NULL))
    { }

    inline FastRand64(const u64 seed) noexcept
        : _seed(seed)
    { }

    [[nodiscard]] inline u64 seed() const noexcept { return _seed; }
    inline void seed(u64 seed) noexcept { _seed = seed; }

    [[nodiscard]] inline u64 rand() noexcept
    {
        _seed = (214013 * _seed + 2531011);
        return _seed;
    }
};
