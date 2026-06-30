/**
 * @file
 */
#pragma once

#include <NumTypes.hpp>

namespace tau::graphics::driver::gl {

// TaGL
constexpr u32 GLDriverMagic = 0x5461474C;

struct GLDriverHeader
{
    u32 Magic = GLDriverMagic;
};

}
