#pragma once

#include <NumTypes.hpp>

class Keyboard final
{
public:
    enum class Flags : u16
    {
        Shift = 1 << 0,
        Ctrl = 1 << 1,
        Alt = 1 << 2,
        LeftShift = 1 << 3,
        RightShift = 1 << 4,
        LeftCtrl = 1 << 5,
        RightCtrl = 1 << 6,
        LeftAlt = 1 << 7,
        RightAlt = 1 << 8,
    };

    enum class Event : u8
    {
        Pressed = 0,
        Released,
        Held
    };

    enum class Key : u16
    {
        Unknown = 0,
        A, B, C, D,
        E, F, G, H,
        I, J, K, L,
        M, N, O, P,
        Q, R, S, T,
        U, V, W, X,
        Y, Z,
        Space,
        K0, K1, K2, K3, // Top row numbers
        K4, K5, K6, K7,
        K8, K9,
        N0, N1, N2, N3, // Num Pad
        N4, N5, N6, N7,
        N8, N9,
        F1, F2, F3, F4, // The F keys, and yes there are 24 of them.
        F5, F6, F7, F8,
        F9, F10, F11, F12,
        F13, F14, F15, F16,
        F17, F18, F19, F20,
        F21, F22, F23, F24,
        Minus,
        Equals,
        Comma,
        Dot,
        ForwardSlash,
        LBracket,
        RBracket,
        BackSlash,
        SemiColon,
        Quote,
        NDivide,   // The num pad /,*,-,+,. keys
        NSubtract,
        NMultiply,
        NAdd,
        NDot,
        NumLock,
        BackTick,
        Esc,
        Tab,
        CapsLock,
        Shift, LShift, RShift,
        Ctrl, LCtrl, RCtrl,
        Alt, LAlt, RAlt,
        Win, LWin, RWin,
        BackSpace,
        Enter,
        Up, Down, Left, Right,
        PrintScreen, ScrollLock, PauseBreak,
        Insert, Delete,
        Home, End,
        PageUp, PageDown
    };

    enum class LockState
    {
        Locked,
        NotLocked,
        NotValidForLocking
    };
public:
    /**
     * Returns whether or not a key is actively pressed.
     */
    [[nodiscard]] static bool isKeyPressed(Key key) noexcept;

    /**
     *   Returns whether a toggleable key (like CapsLock) is
     * currently in the active state.
     */
    [[nodiscard]] static LockState isKeyLocked(Key key) noexcept;

    /**
     *   If the key is a toggleable key returns if its in
     * the active state, if it is not a toggleable key,
     * returns if it is currently pressed.
     */
    [[nodiscard]] static bool isKeyActive(Key key) noexcept;

    [[nodiscard]] static Keyboard::Flags keyboardFlags() noexcept;

    [[nodiscard]] static int convertToSysVKey(Key key) noexcept;

    [[nodiscard]] static Key convertFromSysVKey(int key) noexcept;
};
