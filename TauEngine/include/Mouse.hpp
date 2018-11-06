#pragma once
#include <NumTypes.hpp>

/* [Flags] */
enum MouseFlags : u8
{
    MF_LEFT_BUTTON_DOWN   = 1 << 0, /** The left mouse button is currently pressed.                  */
    MF_RIGHT_BUTTON_DOWN  = 1 << 1, /** The right mouse button is currently pressed.                 */
    MF_SHIFT_KEY_DOWN     = 1 << 2, /** The shift key is current pressed.                            */
    MF_CTRL_KEY_DOWN      = 1 << 3, /** The ctrl key is currently pressed.                           */
    MF_MIDDLE_BUTTON_DOWN = 1 << 4, /** The middle mouse button (scroll wheel) is currently pressed. */
    MF_X_BUTTON_1_DOWN    = 1 << 5, /** The first side mouse button is currently pressed.            */
    MF_X_BUTTON_2_DOWN    = 1 << 6  /** The second side mouse button is currently pressed.           */
};

enum MouseEvent : u8
{
    ME_LEFT_BUTTON_DOWN = 0, /** The left mouse button has just been pressed.                   */
    ME_LEFT_BUTTON_UP,       /** The left mouse button has just been released.                  */
    ME_MIDDLE_BUTTON_DOWN,   /** The middle mouse button (scroll wheel) has just been pressed.  */
    ME_MIDDLE_BUTTON_UP,     /** The middle mouse button (scroll wheel) has just been released. */
    ME_RIGHT_BUTTON_DOWN,    /** The right mouse button has just been pressed.                  */
    ME_RIGHT_BUTTON_UP,      /** The right mouse button has just been released.                 */
    ME_X_BUTTON_1_DOWN,      /** The first side mouse button has just been pressed.             */
    ME_X_BUTTON_1_UP,        /** The first side mouse button has just been released.            */
    ME_X_BUTTON_2_DOWN,      /** The second side mouse button has just been pressed.            */
    ME_X_BUTTON_2_UP,        /** The second side mouse button has just been released.           */
    ME_ERROR = 0xFF
};
