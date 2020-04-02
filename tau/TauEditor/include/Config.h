#pragma once

#include <NumTypes.hpp>

#pragma pack(push, 1)
struct Config final
{
    bool vsync;
    bool useVR;
    u32 windowWidth;
    u32 windowHeight;
};
#pragma pack(pop)
