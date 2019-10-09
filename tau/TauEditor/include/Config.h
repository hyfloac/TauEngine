#pragma once

#include <NumTypes.hpp>

#pragma pack(push, 1)
struct Config final
{
    bool vsync;
    u32 windowWidth;
    u32 windowHeight;
};
#pragma pack(pop)
