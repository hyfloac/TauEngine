/**
 * @file
 * 
 * Contains several things for use with mouse events.
 */
#pragma once
#include <NumTypes.hpp>

/* [Flags] */
/**
 * A series of flags for use with Mouse Events.
 * 
 * These value are equivalent to the Win32 API values.
 */
enum MouseFlags : u8
{
    /** The left mouse button is currently pressed.                  */ MF_LEFT_BUTTON_DOWN   = 1 << 0,
    /** The right mouse button is currently pressed.                 */ MF_RIGHT_BUTTON_DOWN  = 1 << 1,
    /** The shift key is current pressed.                            */ MF_SHIFT_KEY_DOWN     = 1 << 2,
    /** The ctrl key is currently pressed.                           */ MF_CTRL_KEY_DOWN      = 1 << 3,
    /** The middle mouse button (scroll wheel) is currently pressed. */ MF_MIDDLE_BUTTON_DOWN = 1 << 4,
    /** The first side mouse button is currently pressed.            */ MF_X_BUTTON_1_DOWN    = 1 << 5,
    /** The second side mouse button is currently pressed.           */ MF_X_BUTTON_2_DOWN    = 1 << 6
};

enum MouseEvent : u8
{
    /** The left mouse button has just been pressed.                   */ ME_LEFT_BUTTON_DOWN = 0,
    /** The left mouse button has just been released.                  */ ME_LEFT_BUTTON_UP,
    /** The middle mouse button (scroll wheel) has just been pressed.  */ ME_MIDDLE_BUTTON_DOWN,
    /** The middle mouse button (scroll wheel) has just been released. */ ME_MIDDLE_BUTTON_UP,
    /** The right mouse button has just been pressed.                  */ ME_RIGHT_BUTTON_DOWN,
    /** The right mouse button has just been released.                 */ ME_RIGHT_BUTTON_UP,
    /** The first side mouse button has just been pressed.             */ ME_X_BUTTON_1_DOWN,
    /** The first side mouse button has just been released.            */ ME_X_BUTTON_1_UP,
    /** The second side mouse button has just been pressed.            */ ME_X_BUTTON_2_DOWN,
    /** The second side mouse button has just been released.           */ ME_X_BUTTON_2_UP,
    ME_ERROR = 0xFF
};
