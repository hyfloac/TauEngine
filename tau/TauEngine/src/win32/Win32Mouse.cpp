#if defined(_WIN32)
#include "system/Mouse.hpp"
#include <Windows.h>

bool Mouse::isButtonDown(Mouse::Button button) noexcept
{
    return GetKeyState(Mouse::convertToSysVButton(button)) & 0x8000;
}

Mouse::Pos Mouse::mousePos() noexcept
{
    POINT p;
    GetCursorPos(&p);
    return { static_cast<u32>(p.x), static_cast<u32>(p.y) };
}

int Mouse::convertToSysVButton(Mouse::Button key) noexcept
{
    switch(key)
    {
        case  Mouse::Button::LeftButton:  return VK_LBUTTON;
        case  Mouse::Button::RightButton:  return VK_RBUTTON;
        case  Mouse::Button::MiddleButton:  return VK_MBUTTON;
        case  Mouse::Button::XButton1: return VK_XBUTTON1;
        case  Mouse::Button::XButton2: return VK_XBUTTON2;
        default: break;
    }
    return -1;
}

Mouse::Button Mouse::convertFromSysVButton(int key) noexcept
{
    switch(key)
    {
        case VK_LBUTTON:  return Mouse::Button::LeftButton;
        case VK_RBUTTON:  return Mouse::Button::RightButton;
        case VK_MBUTTON:  return Mouse::Button::MiddleButton;
        case VK_XBUTTON1: return Mouse::Button::XButton1;
        case VK_XBUTTON2: return Mouse::Button::XButton2;
        default: break;
    }
    return static_cast<Mouse::Button>(0xFF);
}
#endif
