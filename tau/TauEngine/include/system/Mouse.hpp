/**
 * @file
 * 
 * Contains several things for use with mouse events.
 */
#pragma once
#include <NumTypes.hpp>

// /**
//  * A series of flags for use with Mouse Events.
//  * 
//  * These value are equivalent to the Win32 API values.
//  */
// enum class MouseFlags : u8
// {
//     /** The left mouse button is currently pressed.                  */ MF_LEFT_BUTTON_DOWN   = 1 << 0,
//     /** The right mouse button is currently pressed.                 */ MF_RIGHT_BUTTON_DOWN  = 1 << 1,
//     /** The shift key is current pressed.                            */ MF_SHIFT_KEY_DOWN     = 1 << 2,
//     /** The ctrl key is currently pressed.                           */ MF_CTRL_KEY_DOWN      = 1 << 3,
//     /** The middle mouse button (scroll wheel) is currently pressed. */ MF_MIDDLE_BUTTON_DOWN = 1 << 4,
//     /** The first side mouse button is currently pressed.            */ MF_X_BUTTON_1_DOWN    = 1 << 5,
//     /** The second side mouse button is currently pressed.           */ MF_X_BUTTON_2_DOWN    = 1 << 6
// };
//
// enum class MouseEvent : u8
// {
//     /** The left mouse button has just been pressed.                   */ ME_LEFT_BUTTON_DOWN = 0,
//     /** The left mouse button has just been released.                  */ ME_LEFT_BUTTON_UP,
//     /** The middle mouse button (scroll wheel) has just been pressed.  */ ME_MIDDLE_BUTTON_DOWN,
//     /** The middle mouse button (scroll wheel) has just been released. */ ME_MIDDLE_BUTTON_UP,
//     /** The right mouse button has just been pressed.                  */ ME_RIGHT_BUTTON_DOWN,
//     /** The right mouse button has just been released.                 */ ME_RIGHT_BUTTON_UP,
//     /** The first side mouse button has just been pressed.             */ ME_X_BUTTON_1_DOWN,
//     /** The first side mouse button has just been released.            */ ME_X_BUTTON_1_UP,
//     /** The second side mouse button has just been pressed.            */ ME_X_BUTTON_2_DOWN,
//     /** The second side mouse button has just been released.           */ ME_X_BUTTON_2_UP,
//     ME_ERROR = 0xFF
// };

class Mouse final
{
public:
    /**
     * A series of flags for use with Mouse Events.
     *
     * These value are equivalent to the Win32 API values.
     */
    enum class Flags : u8
    {
        /** The left mouse button is currently pressed.                  */ LeftButtonDown   = 1 << 0,
        /** The right mouse button is currently pressed.                 */ RightButtonDown  = 1 << 1,
        /** The shift key is current pressed.                            */ ShiftKeyDown     = 1 << 2,
        /** The ctrl key is currently pressed.                           */ CtrlKeyDown      = 1 << 3,
        /** The middle mouse button (scroll wheel) is currently pressed. */ MiddleButtonDown = 1 << 4,
        /** The first side mouse button is currently pressed.            */ XButton1Down     = 1 << 5,
        /** The second side mouse button is currently pressed.           */ XButton2Down     = 1 << 6
    };

    enum class Event : u8
    {
        /** The left mouse button has just been pressed.                   */ LeftButtonDown = 0,
        /** The left mouse button has just been released.                  */ LeftButtonUp,
        /** The right mouse button has just been pressed.                  */ RightButtonDown,
        /** The right mouse button has just been released.                 */ RightButtonUp,
        /** The middle mouse button (scroll wheel) has just been pressed.  */ MiddleButtonDown,
        /** The middle mouse button (scroll wheel) has just been released. */ MiddleButtonUp,
        /** The first side mouse button has just been pressed.             */ XButton1Down,
        /** The first side mouse button has just been released.            */ XButton1Up,
        /** The second side mouse button has just been pressed.            */ XButton2Down,
        /** The second side mouse button has just been released.           */ XButton2Up,
        Error = 0xFF
    };

    enum class Button : u8
    {
        LeftButton,
        RightButton,
        MiddleButton,
        XButton1,
        XButton2
    };

    struct Pos final
    {
        u32 x;
        u32 y;
    };
public:
    [[nodiscard]] static bool isButtonDown(Button button) noexcept;

    [[nodiscard]] static Pos mousePos() noexcept;

    [[nodiscard]] static int convertToSysVButton(Button key) noexcept;

    [[nodiscard]] static Button convertFromSysVButton(int key) noexcept;
};
