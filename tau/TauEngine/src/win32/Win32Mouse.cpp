#if defined(_WIN32)
#include "system/Mouse.hpp"
#include "system/Window.hpp"
#include <Windows.h>

bool Mouse::isButtonDown(const Mouse::Button button) noexcept
{
    return GetKeyState(Mouse::convertToSysVButton(button)) & 0x8000;
}

Mouse::Pos Mouse::mousePos() noexcept
{
    POINT p;
    GetCursorPos(&p);
    return { static_cast<u32>(p.x), static_cast<u32>(p.y) };
}

void Mouse::mousePos(const Pos pos) noexcept
{
    SetCursorPos(pos.x, pos.y);
}

void Mouse::mousePos(const u32 x, const u32 y) noexcept
{
    SetCursorPos(x, y);
}

Mouse::Pos Mouse::mousePos(const Window& window) noexcept
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(window._windowContainer.windowHandle, &p);
    return { static_cast<u32>(p.x), static_cast<u32>(p.y) };
}

void Mouse::mousePos(const Window& window, const Pos pos) noexcept
{
    POINT p = { static_cast<LONG>(pos.x), static_cast<LONG>(pos.y) };
    ClientToScreen(window._windowContainer.windowHandle, &p);
    SetCursorPos(p.x, p.y);
}

void Mouse::mousePos(const Window& window, const u32 x, const u32 y) noexcept
{
    POINT p = { static_cast<LONG>(x), static_cast<LONG>(y) };
    ClientToScreen(window._windowContainer.windowHandle, &p);
    SetCursorPos(p.x, p.y);
}

void Mouse::setVisible(bool visible) noexcept
{
    ShowCursor(visible);
}


int Mouse::convertToSysVButton(const Mouse::Button key) noexcept
{
    switch(key)
    {
        case  Mouse::Button::LeftButton:    return VK_LBUTTON;
        case  Mouse::Button::RightButton:   return VK_RBUTTON;
        case  Mouse::Button::MiddleButton:  return VK_MBUTTON;
        case  Mouse::Button::XButton1:      return VK_XBUTTON1;
        case  Mouse::Button::XButton2:      return VK_XBUTTON2;
        default: break;
    }
    return -1;
}

Mouse::Button Mouse::convertFromSysVButton(const int key) noexcept
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
