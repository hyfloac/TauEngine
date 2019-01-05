#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>

flags KeyboardFlags : u16
{
    KF_SHIFT       = 1 << 0,
    KF_ALT         = 1 << 1,
    KF_CTRL        = 1 << 2,
    KF_LEFT_SHIFT  = 1 << 3,
    KF_RIGHT_SHIFT = 1 << 4,
    KF_LEFT_ALT    = 1 << 5,
    KF_RIGHT_ALT   = 1 << 6,
    KF_LEFT_CTRL   = 1 << 7,
    KF_RIGHT_CTRL  = 1 << 8,
};

enum class KeyboardEvent : u8
{
    KE_KEY_PRESSED = 0,
    KE_KEY_RELEASED,
    KE_KEY_HELD
};
