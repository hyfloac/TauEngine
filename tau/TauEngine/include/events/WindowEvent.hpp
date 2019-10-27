#pragma once

#include "events/Event.hpp"
#include "system/Keyboard.hpp"
#include "system/Mouse.hpp"
#include <NumTypes.hpp>
#include <Objects.hpp>

class Window;

class WindowEvent : public Event
{
    DEFAULT_DESTRUCT_VI(WindowEvent);
    EVENT_IMPL(WindowEvent);
protected:
    Window& _window;
protected:
    inline WindowEvent(Window& window) noexcept
        : _window(window)
    { }
public:
    [[nodiscard]] Window& window() noexcept { return  _window; }
    [[nodiscard]] const Window& window() const noexcept { return  _window; }
};

class WindowResizeEvent final : public WindowEvent
{
    DEFAULT_DESTRUCT(WindowResizeEvent);
    EVENT_IMPL(WindowResizeEvent);
private:
    u32 _oldWidth;
    u32 _oldHeight;
    u32 _newWidth;
    u32 _newHeight;
public:
    WindowResizeEvent(Window& window, const u32 oldWidth, const u32 oldHeight, const u32 newWidth, const u32 newHeight) noexcept
        : WindowEvent(window),
          _oldWidth(oldWidth), _oldHeight(oldHeight),
          _newWidth(newWidth), _newHeight(newHeight)
    { }

    [[nodiscard]] u32 oldWidth() const noexcept { return _oldWidth; }
    [[nodiscard]] u32 oldHeight() const noexcept { return _oldHeight; }
    [[nodiscard]] u32 newWidth() const noexcept { return _newWidth; }
    [[nodiscard]] u32 newHeight() const noexcept { return _newHeight; }
};

class WindowMoveEvent final : public WindowEvent
{
    DEFAULT_DESTRUCT(WindowMoveEvent);
    EVENT_IMPL(WindowMoveEvent);
private:
    u32 _oldX;
    u32 _oldY;
    u32 _newX;
    u32 _newY;
public:
    WindowMoveEvent(Window& window, const u32 oldX, const u32 oldY, const u32 newX, const u32 newY) noexcept
        : WindowEvent(window),
        _oldX(oldX), _oldY(oldY),
        _newX(newX), _newY(newY)
    { }

    [[nodiscard]] u32 oldX() const noexcept { return _oldX; }
    [[nodiscard]] u32 oldY() const noexcept { return _oldY; }
    [[nodiscard]] u32 newX() const noexcept { return _newX; }
    [[nodiscard]] u32 newY() const noexcept { return _newY; }
};

class WindowActiveEvent final : public WindowEvent
{
    DEFAULT_DESTRUCT(WindowActiveEvent);
    EVENT_IMPL(WindowActiveEvent);
private:
    bool _active;
public:
    WindowActiveEvent(Window& window, const bool active) noexcept
        : WindowEvent(window), _active(active)
    { }

    [[nodiscard]] bool active() const noexcept { return _active; }
};

class WindowKeyEvent final : public WindowEvent
{
    DEFAULT_DESTRUCT(WindowKeyEvent);
    EVENT_IMPL(WindowKeyEvent);
    EVENT_INTERCEPTABLE(true);
private:
    Keyboard::Event _event;
    Keyboard::Flags _flags;
    Keyboard::Key _key;
public:
    WindowKeyEvent(Window& window, const Keyboard::Event event, const Keyboard::Flags flags, const Keyboard::Key key) noexcept
        : WindowEvent(window), _event(event), _flags(flags), _key(key)
    { }

    [[nodiscard]] Keyboard::Event event() const noexcept { return _event; }
    [[nodiscard]] Keyboard::Flags flags() const noexcept { return _flags; }
    [[nodiscard]] Keyboard::Key key() const noexcept { return _key; }
};

class WindowAsciiKeyEvent final : public WindowEvent
{
    DEFAULT_DESTRUCT(WindowAsciiKeyEvent);
    EVENT_IMPL(WindowAsciiKeyEvent);
    EVENT_INTERCEPTABLE(true);
private:
    wchar_t _wideChar;
    char _c;
public:
    WindowAsciiKeyEvent(Window& window, const wchar_t wideChar, const char c) noexcept
        : WindowEvent(window), _wideChar(wideChar), _c(c)
    { }

    [[nodiscard]] wchar_t wideChar() const noexcept { return _wideChar; }
    [[nodiscard]] char c() const noexcept { return _c; }
};

class WindowMouseEvent : public WindowEvent
{
    DEFAULT_DESTRUCT_VI(WindowMouseEvent);
    EVENT_IMPL(WindowMouseEvent);
protected:
    Mouse::Flags _flags;
    u32 _x;
    u32 _y;
protected:
    WindowMouseEvent(Window& window, const Mouse::Flags flags, const u32 x, const u32 y) noexcept
        : WindowEvent(window), _flags(flags), _x(x), _y(y)
    { }
public:
    [[nodiscard]] Mouse::Flags flags() const noexcept { return _flags; }
    [[nodiscard]] u32 x() const noexcept { return _x; }
    [[nodiscard]] u32 y() const noexcept { return _y; }
};

class WindowMouseMoveEvent : public WindowMouseEvent
{
    DEFAULT_DESTRUCT(WindowMouseMoveEvent);
    EVENT_IMPL(WindowMouseMoveEvent);
    EVENT_INTERCEPTABLE(true);
public:
    WindowMouseMoveEvent(Window& window, const Mouse::Flags flags, const u32 x, const u32 y) noexcept
        : WindowMouseEvent(window, flags, x, y)
    { }
};

class WindowMouseClickEvent : public WindowMouseEvent
{
    DEFAULT_DESTRUCT(WindowMouseClickEvent);
    EVENT_IMPL(WindowMouseClickEvent);
    EVENT_INTERCEPTABLE(true);
private:
    Mouse::Event _mouseEvent;
public:
    WindowMouseClickEvent(Window& window, const Mouse::Flags flags, const u32 x, const u32 y, Mouse::Event mouseEvent) noexcept
        : WindowMouseEvent(window, flags, x, y), _mouseEvent(mouseEvent)
    { }

    [[nodiscard]] Mouse::Event mouseEvent() const noexcept { return _mouseEvent; }
};