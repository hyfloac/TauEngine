#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>

flags_e class KeyboardFlags : u16
{
    KF_SHIFT       = 1 << 0,
    KF_CTRL        = 1 << 1,
    KF_ALT         = 1 << 2,
    KF_LEFT_SHIFT  = 1 << 3,
    KF_RIGHT_SHIFT = 1 << 4,
    KF_LEFT_CTRL   = 1 << 5,
    KF_RIGHT_CTRL  = 1 << 6,
    KF_LEFT_ALT    = 1 << 7,
    KF_RIGHT_ALT   = 1 << 8,
};

enum class KeyboardEvent : u8
{
    KE_KEY_PRESSED = 0,
    KE_KEY_RELEASED,
    KE_KEY_HELD
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
    F1 , F2 , F3 , F4 , // The F keys, and yes there are 24 of them.
    F5 , F6 , F7 , F8 ,
    F9 , F10, F11, F12,
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
    Ctrl,  LCtrl,  RCtrl,
    Alt,   LAlt,   RAlt,
    Win,   LWin,   RWin,
    BackSpace,
    Enter,
    Up, Down, Left, Right,
    PrintScreen, ScrollLock, PauseBreak,
    Insert, Delete,
    Home, End,
    PageUp, PageDown
};
