#pragma once

#include <NumTypes.hpp>
#include <_SysWindowContainer.hpp>
#include <DLL.hpp>
#include <Utils.hpp>
#include <Mouse.hpp>

class Window;

/**
 * Handles a resize of the window. This should only be used to reset the graphics.
 * 
 * Do not call Window#setSize(const u32, const u32) noexcept.
 */
typedef void(*onWindowResized_f)(const u32 width, const u32 height, Window* window);

/**
 * Handles a mouse event.
 * 
 * @param mouseEvent
 *      Specifies what event is going to be handled.
 * @param mouseFlags
 *      Specifies which flags are currently set.
 * @param window
 *      A pointer to the window for which this event was triggered.
 */
typedef void(*onMouseEvent_f)(const MouseEvent mouseEvent, const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, Window* window);

enum WindowState : u8
{
    MINIMIZED = 0,
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
    const void* _userContainer;
    const Window* _parent;

    WindowState _windowState;
    bool _isResizing;

    onWindowResized_f _windowResizeHandler;
    onMouseEvent_f _mouseEventHandler;
public:
    static void unloadCurrentContext() noexcept;
public:
#pragma region Constructor
    Window(u32 width, u32 height, const char* title, const void* userContainer = null, const Window* parent = null) noexcept;
    Window(const Window& copy) noexcept = default;
    Window(Window&& move) noexcept = default;
#pragma endregion

    ~Window() noexcept;

#pragma region Assignment Operator
    Window& operator =(const Window& copy) noexcept = default;
    Window& operator =(Window&& move) noexcept = default;
#pragma endregion

#pragma region Getters
    inline u32 width()  const noexcept { return _width;  }
    inline u32 height() const noexcept { return _height; }
    inline const char* title() const noexcept { return _title; }
    inline const void* userContainer() const noexcept { return _userContainer; }
    inline const Window* parent() const noexcept { return _parent; }
#pragma endregion

    void resize(const u32 width, const u32 height) noexcept;

    void setTitle(const char* title) noexcept;

    inline void setResizeWindowHandler(onWindowResized_f windowResizeHandler) { _windowResizeHandler = windowResizeHandler; }
    inline void setMouseEventHandler(onMouseEvent_f mouseEventHandler) { _mouseEventHandler = mouseEventHandler; }

    bool createWindow() noexcept;
    void closeWindow() noexcept;
    void showWindow() const noexcept;
    void hideWindow() const noexcept;

    bool createContext() noexcept;

    void makeContextCurrent() const noexcept;

    void swapBuffers() const noexcept;
public:
#ifdef _WIN32
    friend static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    friend static void removeWindow(Window*);
    friend static Window* getWindowFromHandle(HWND);
#endif
};
