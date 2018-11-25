#include <FastRand.hpp>

static u32 s_seed;

void seedFastRand(u32 seed) noexcept
{
    s_seed = seed;
}

u32 fastRand() noexcept
{
    s_seed = 214013 * s_seed + 2531011;
    return s_seed;
}
