#pragma once

#include <events/Event.hpp>
#include <NumTypes.hpp>
#include <system/Keyboard.hpp>

class Window;

class WindowEvent : public Event
{
protected:
    Window* _window;
protected:
    inline WindowEvent(Window* window) noexcept
        : _window(window)
    { }
public:
    inline virtual ~WindowEvent() noexcept = default;

    [[nodiscard]] Window* window() noexcept { return  _window; }
    [[nodiscard]] const Window* window() const noexcept { return  _window; }

    EVENT_IMPL(WindowEvent);
};

class WindowResizeEvent final : public WindowEvent
{
private:
    u32 _oldWidth;
    u32 _oldHeight;
    u32 _newWidth;
    u32 _newHeight;
public:
    WindowResizeEvent(Window* const window, const u32 oldWidth, const u32 oldHeight, const u32 newWidth, const u32 newHeight) noexcept
        : WindowEvent(window),
          _oldWidth(oldWidth), _oldHeight(oldHeight),
          _newWidth(newWidth), _newHeight(newHeight)
    { }

    inline ~WindowResizeEvent() noexcept = default;

    [[nodiscard]] u32 oldWidth() const noexcept { return _oldWidth; }
    [[nodiscard]] u32 oldHeight() const noexcept { return _oldHeight; }
    [[nodiscard]] u32 newWidth() const noexcept { return _newWidth; }
    [[nodiscard]] u32 newHeight() const noexcept { return _newHeight; }

    EVENT_IMPL(WindowResizeEvent);
};

class WindowActiveEvent final : public WindowEvent
{
private:
    bool _active;
public:
    WindowActiveEvent(Window* const window, const bool active) noexcept
        : WindowEvent(window), _active(active)
    { }

    inline ~WindowActiveEvent() noexcept = default;

    [[nodiscard]] bool active() const noexcept { return _active; }

    EVENT_IMPL(WindowActiveEvent);
};

class WindowKeyEvent final : public WindowEvent
{
private:
    KeyboardEvent _event;
    KeyboardFlags _flags;
    u64 _key;
public:
    WindowKeyEvent(Window* const window, const KeyboardEvent event, const KeyboardFlags flags, const u64 key) noexcept
        : WindowEvent(window), _event(event), _flags(flags), _key(key)
    { }

    [[nodiscard]] KeyboardEvent event() const noexcept { return _event; }
    [[nodiscard]] KeyboardFlags flags() const noexcept { return _flags; }
    [[nodiscard]] u64 key() const noexcept { return _key; }

    EVENT_IMPL_INTERCEPTABLE(WindowKeyEvent);
};

class WindowAsciiKeyEvent final : public WindowEvent
{
private:
    wchar_t _wideChar;
    char _c;
public:
    WindowAsciiKeyEvent(Window* const window, const wchar_t wideChar, const char c) noexcept
        : WindowEvent(window), _wideChar(wideChar), _c(c)
    { }

    inline ~WindowAsciiKeyEvent() noexcept = default;

    [[nodiscard]] wchar_t wideChar() const noexcept { return _wideChar; }
    [[nodiscard]] char c() const noexcept { return _c; }

    EVENT_IMPL_INTERCEPTABLE(WindowAsciiKeyEvent);
};
