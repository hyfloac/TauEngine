#if defined(_WIN32)
#include "system/Keyboard.hpp"
#include <Windows.h>
#include <EnumBitFields.hpp>

bool Keyboard::isKeyPressed(Keyboard::Key key) noexcept
{
    if(key == Key::Win)
    {
        return GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000;
    }
    const int keyCode = convertToSysVKey(key);
    return keyCode == -1 ? false : GetKeyState(keyCode) & 0x8000;
}

Keyboard::LockState Keyboard::isKeyLocked(Keyboard::Key key) noexcept
{
    switch(key)
    {
        case Key::NumLock:
        case Key::CapsLock:
        case Key::ScrollLock: return (GetKeyState(convertToSysVKey(key)) & 0x0001) ? Keyboard::LockState::Locked : Keyboard::LockState::NotLocked;
        default: return Keyboard::LockState::NotValidForLocking;
    }
}

bool Keyboard::isKeyActive(Keyboard::Key key) noexcept
{
    const int keyCode = convertToSysVKey(key);
    switch(key)
    {
        case Key::NumLock:
        case Key::CapsLock:
        case Key::ScrollLock: return GetKeyState(keyCode) & 0x0001;
        default: break;
    }
    if(key == Key::Win)
    {
        return GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000;
    }
    return keyCode == -1 ? false : GetKeyState(keyCode) & 0x8000;
}

Keyboard::Flags Keyboard::keyboardFlags() noexcept
{
    Keyboard::Flags ret = static_cast<Keyboard::Flags>(0);

    if(GetKeyState(VK_SHIFT)    & 0x8000) { ret |= Keyboard::Flags::Shift; }
    if(GetKeyState(VK_CONTROL)  & 0x8000) { ret |= Keyboard::Flags::Ctrl;  }
    if(GetKeyState(VK_MENU)     & 0x8000) { ret |= Keyboard::Flags::Alt;   }

    if(GetKeyState(VK_LSHIFT)   & 0x8000) { ret |= Keyboard::Flags::LeftShift;  }
    if(GetKeyState(VK_RSHIFT)   & 0x8000) { ret |= Keyboard::Flags::RightShift; }
    if(GetKeyState(VK_LCONTROL) & 0x8000) { ret |= Keyboard::Flags::LeftCtrl;   }
    if(GetKeyState(VK_RCONTROL) & 0x8000) { ret |= Keyboard::Flags::RightCtrl;  }
    if(GetKeyState(VK_LMENU)    & 0x8000) { ret |= Keyboard::Flags::LeftAlt;    }
    if(GetKeyState(VK_RMENU)    & 0x8000) { ret |= Keyboard::Flags::RightAlt;   }

    return ret;
}

int Keyboard::convertToSysVKey(Keyboard::Key key) noexcept
{
    constexpr int ka = static_cast<int>(Keyboard::Key::A);
    const int k = static_cast<int>(key) - ka;

    constexpr int VK_A = 0x41;
    constexpr int VK_0 = 0x30;
    constexpr int VK_N0 = VK_NUMPAD0;
    constexpr int _VK_F1 = VK_F1;

    if(key >= Keyboard::Key::A && key <= Keyboard::Key::Z)
    {
        return k + VK_A;
    }
    if(key >= Keyboard::Key::K0 && key <= Keyboard::Key::K9)
    {
        return k + VK_0;
    }
    if(key >= Keyboard::Key::N0 && key <= Keyboard::Key::N9)
    {
        return k + VK_N0;
    }
    if(key >= Keyboard::Key::F1 && key <= Keyboard::Key::F24)
    {
        return k + _VK_F1;
    }

    switch(key)
    {
        case Keyboard::Key::Space: return VK_SPACE;
        case Keyboard::Key::Minus: return VK_OEM_MINUS;
        case Keyboard::Key::Equals: return VK_OEM_PLUS;
        case Keyboard::Key::Comma: return VK_OEM_COMMA;
        case Keyboard::Key::Dot: return VK_OEM_PERIOD;
        case Keyboard::Key::ForwardSlash: return VK_OEM_2;
        case Keyboard::Key::LBracket: return VK_OEM_4;
        case Keyboard::Key::RBracket: return VK_OEM_6;
        case Keyboard::Key::SemiColon: return VK_OEM_1;
        case Keyboard::Key::Quote: return VK_OEM_7;
        case Keyboard::Key::NDivide: return VK_DIVIDE;
        case Keyboard::Key::NSubtract: return VK_SUBTRACT;
        case Keyboard::Key::NMultiply: return VK_MULTIPLY;
        case Keyboard::Key::NAdd: return VK_ADD;
        case Keyboard::Key::NDot: return VK_DECIMAL;
        case Keyboard::Key::Enter: return VK_RETURN;
        case Keyboard::Key::NumLock: return VK_NUMLOCK;
        case Keyboard::Key::BackTick: return VK_OEM_3;
        case Keyboard::Key::Esc: return VK_ESCAPE;
        case Keyboard::Key::Tab: return VK_TAB;
        case Keyboard::Key::CapsLock: return VK_CAPITAL;
        case Keyboard::Key::Shift: return VK_SHIFT;
        case Keyboard::Key::LShift: return VK_LSHIFT;
        case Keyboard::Key::RShift: return VK_RSHIFT;
        case Keyboard::Key::Ctrl: return VK_CONTROL;
        case Keyboard::Key::LCtrl: return VK_LCONTROL;
        case Keyboard::Key::RCtrl: return VK_RCONTROL;
        case Keyboard::Key::Alt: return VK_MENU;
        case Keyboard::Key::LAlt: return VK_LMENU;
        case Keyboard::Key::RAlt: return VK_RMENU;
        case Keyboard::Key::LWin: return VK_LWIN;
        case Keyboard::Key::RWin: return VK_RWIN;
        case Keyboard::Key::BackSpace: return VK_BACK;
        case Keyboard::Key::Up: return VK_UP;
        case Keyboard::Key::Down: return VK_DOWN;
        case Keyboard::Key::Left: return VK_LEFT;
        case Keyboard::Key::Right: return VK_RIGHT;
        case Keyboard::Key::PrintScreen: return VK_SNAPSHOT;
        case Keyboard::Key::ScrollLock: return VK_SCROLL;
        case Keyboard::Key::PauseBreak: return VK_PAUSE;
        case Keyboard::Key::Insert: return VK_INSERT;
        case Keyboard::Key::Delete: return VK_DELETE;
        case Keyboard::Key::Home: return VK_HOME;
        case Keyboard::Key::End: return VK_END;
        case Keyboard::Key::PageUp: return VK_PRIOR;
        case Keyboard::Key::PageDown: return VK_NEXT;
        default: return -1;
    }
}

Keyboard::Key Keyboard::convertFromSysVKey(int key) noexcept
{
    constexpr int ka = static_cast<int>(Keyboard::Key::A);
    const int k = key + ka;

    constexpr int VK_A = 0x41;
    constexpr int VK_Z = 0x5A;
    constexpr int VK_0 = 0x30;
    constexpr int VK_9 = 0x39;
    constexpr int VK_N0 = VK_NUMPAD0;
    constexpr int VK_N9 = VK_NUMPAD9;
    constexpr int _VK_F1 = VK_F1;
    constexpr int _VK_F24 = VK_F24;

    if(key >= VK_A && key <= VK_Z)
    {
        return static_cast<Keyboard::Key>(k - VK_A);
    }
    if(key >= VK_0 && key <= VK_9)
    {
        return static_cast<Keyboard::Key>(k - VK_0);
    }
    if(key >= VK_N0 && key <= VK_N9)
    {
        return static_cast<Keyboard::Key>(k - VK_N0);
    }
    if(key >= _VK_F1 && key <= _VK_F24)
    {
        return static_cast<Keyboard::Key>(key - _VK_F1 + static_cast<int>(Keyboard::Key::F1));
    }

    switch(key)
    {
        case VK_SPACE: return Keyboard::Key::Space;
        case VK_OEM_MINUS: return Keyboard::Key::Minus;
        case VK_OEM_PLUS: return Keyboard::Key::Equals;
        case VK_OEM_COMMA: return Keyboard::Key::Comma;
        case VK_OEM_PERIOD: return Keyboard::Key::Dot;
        case VK_OEM_2: return Keyboard::Key::ForwardSlash;
        case VK_OEM_4: return Keyboard::Key::LBracket;
        case VK_OEM_6: return Keyboard::Key::RBracket;
        case VK_OEM_1: return Keyboard::Key::SemiColon;
        case VK_OEM_7: return Keyboard::Key::Quote;
        case VK_DIVIDE: return Keyboard::Key::NDivide;
        case VK_SUBTRACT: return Keyboard::Key::NSubtract;
        case VK_MULTIPLY: return Keyboard::Key::NMultiply;
        case VK_ADD: return Keyboard::Key::NAdd;
        case VK_DECIMAL: return Keyboard::Key::NDot;
        case VK_RETURN: return Keyboard::Key::Enter;
        case VK_NUMLOCK: return Keyboard::Key::NumLock;
        case VK_OEM_3: return Keyboard::Key::BackTick;
        case VK_ESCAPE: return Keyboard::Key::Esc;
        case VK_TAB: return Keyboard::Key::Tab;
        case VK_CAPITAL: return Keyboard::Key::CapsLock;
        case VK_SHIFT: return Keyboard::Key::Shift;
        case VK_LSHIFT: return Keyboard::Key::LShift;
        case VK_RSHIFT: return Keyboard::Key::RShift;
        case VK_CONTROL: return Keyboard::Key::Ctrl;
        case VK_LCONTROL: return Keyboard::Key::LCtrl;
        case VK_RCONTROL: return Keyboard::Key::RCtrl;
        case VK_MENU: return Keyboard::Key::Alt;
        case VK_LMENU: return Keyboard::Key::LAlt;
        case VK_RMENU: return Keyboard::Key::RAlt;
        case VK_LWIN: return Keyboard::Key::LWin;
        case VK_RWIN: return Keyboard::Key::RWin;
        case VK_BACK: return Keyboard::Key::BackSpace;
        case VK_UP: return Keyboard::Key::Up;
        case VK_DOWN: return Keyboard::Key::Down;
        case VK_LEFT: return Keyboard::Key::Left;
        case VK_RIGHT: return Keyboard::Key::Right;
        case VK_SNAPSHOT: return Keyboard::Key::PrintScreen;
        case VK_SCROLL: return Keyboard::Key::ScrollLock;
        case VK_PAUSE: return Keyboard::Key::PauseBreak;
        case VK_INSERT: return Keyboard::Key::Insert;
        case VK_DELETE: return Keyboard::Key::Delete;
        case VK_HOME: return Keyboard::Key::Home;
        case VK_END: return Keyboard::Key::End;
        case VK_PRIOR: return Keyboard::Key::PageUp;
        case VK_NEXT: return Keyboard::Key::PageDown;
        default: return Keyboard::Key::Unknown;
    }
}
#endif
