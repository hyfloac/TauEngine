#pragma once

#include <NumTypes.hpp>
#include <_SysWindowContainer.hpp>
#include <DLL.hpp>

class Window;

/**
 * Handles a resize of the window. This should only be used to reset the graphics.
 * 
 * Do not call Window#setSize(const u32, const u32) noexcept.
 */
typedef void(*onWindowResized_t)(const u32 width, const u32 height, Window* window);

enum WindowState : u8
{
    MINIMIZED,
    MAXIMIZED,
    NEITHER
};

class TAU_DLL Window
{
private:
    u32 _width;
    u32 _height;
    const char* _title;
    _SysWindowContainer _windowContainer;

    WindowState _windowState;
    bool _isResizing;

    onWindowResized_t _windowResizeHandler;
public:
    Window(u32 width, u32 height, const char* title) noexcept;
    ~Window() noexcept;

    inline u32 width()  const noexcept { return _width;  }
    inline u32 height() const noexcept { return _height; }
    inline const char* title() const noexcept { return _title; }

    void resize(const u32 width, const u32 height) noexcept;

    void setTitle(const char* title) noexcept;

    inline void setOnUserResizeWindow(onWindowResized_t windowResizeHandler) { _windowResizeHandler = windowResizeHandler; }

    bool createWindow() noexcept;
    void closeWindow() noexcept;
    void showWindow() const noexcept;

public:
#ifdef _WIN32
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    friend void removeWindow(Window*);
    friend Window* getWindowFromHandle(HWND);
#endif
};
