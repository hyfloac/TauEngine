/**
 * @file
 * 
 * Describes an abstract window.
 * 
 *   This class should be fairly OS-independent. Any OS 
 * dependent stuff should either be put in {@link _SysWindowContainer}
 * or be in the implementation file.
 * 
 * OS      | Implementation  
 * ------- | --------------
 * Windows | Win32Window.cpp
 */
#pragma once

#include <NumTypes.hpp>
#include <system/_SysWindowContainer.hpp>
#include <DLL.hpp>
#include <Utils.hpp>
#include <system/Mouse.hpp>
#include <system/Keyboard.hpp>
#include <Safeties.hpp>

class Window;

/**
 * Handles a resize of the window. This should only be used to reset the graphics.
 * 
 * Do not call {@link Window#resize(const u32,const u32)}.
 */
typedef void(*onWindowResized_f)(const u32 width, const u32 height, NonNull Window* window);

/**
 * Handles a mouse event.
 * 
 * @param[in] mouseEvent
 *      Specifies what event is going to be handled.
 * @param[in] mouseFlags
 *      Specifies which flags are currently set.
 * @param[in] xPos
 *      The current x position of the mouse.
 * @param[in] yPos
 *      The current y position of the mouse.
 * @param[in] window
 *      A pointer to the window for which this event was triggered.
 */
typedef void(*onMouseEvent_f)(const MouseEvent mouseEvent, const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, NonNull Window* window);

typedef void(*onMouseMove_f)(const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, NonNull Window* window);

typedef void(*onKeyEvent_f)(const KeyboardEvent keyboardEvent, const KeyboardFlags keyboardFlags, const u64 key, const wchar_t unicodeChar, const char asciiChar, NonNull Window* window);

enum class WindowState : u8
{
    MINIMIZED = 0,
    MAXIMIZED,
    NEITHER
};

struct ContextSettings
{
    u8 majorVersion;
    u8 minorVersion;
    union
    {
        struct
        {
            bool debug : 1;
            bool forwardCompatible : 1;
            bool coreProfile : 1;
            bool compatProfile : 1;
            u8 u0 : 1, u1 : 1, u2 : 1, u3 : 1;
        };
        u8 _compressed;
    };
};

#ifdef _WIN32
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static void removeWindow(NotNull<Window>);
static Nullable Window* getWindowFromHandle(HWND);
#endif

class TAU_DLL Window
{
private:
    /**
     *   The current width of the window. This can change by 
     * either being manually resized, or by being 
     * programatically resized.
     */
    u32 _width;
    /**
     *   The current height of the window. This can change by
     * either being manually resized, or by being
     * programatically resized.
     */
    u32 _height;
    /**
     *   The title of the window. The string can change, the 
     * contents cannot.
     */
    const char* _title;
    /**
     * A system dependent container of information.
     * 
     * See {@link _SysWindowContainer}.
     */
    _SysWindowContainer _windowContainer;
    /**
     *   A pointer to a user dependent structure. This can be
     * anything, it is up to the developer.
     */
    const void* _userContainer;
    /**
     * The parent window. This cannot be modified.
     */
    const Window* _parent;
    ContextSettings _contextSettings;

    /**
     * The current window state.
     * 
     * See {@link WindowState}.
     */
    WindowState _windowState;
    /**
     *   Whether or not the window is currently being resized. 
     * This reduces lag by not updating any visuals until the 
     * window is finished being resized.
     */
    bool _isResizing;

    onWindowResized_f _windowResizeHandler;
    onMouseEvent_f _mouseEventHandler;
    onMouseMove_f _mouseMoveHandler;
    onKeyEvent_f _keyEventHandler;
public:
    /**
     * Unloads the current context. 
     * 
     * Not much use for this except when exiting.
     */
    static void unloadCurrentContext() noexcept;
public:
#pragma region Constructor
    Window(u32 width, u32 height, Nullable const char* title, Nullable const void* userContainer = null, Nullable const Window* parent = null) noexcept;
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
    inline ContextSettings& contextSettings() noexcept { return _contextSettings; }
#pragma endregion

    /**
     * Resizes the window.
     * 
     * @param[in] width
     *      The new width of the window.
     * @param[in] height
     *      The new height of the window.
     */
    void resize(const u32 width, const u32 height) noexcept;

    void setTitle(const char* title) noexcept;

    inline void setResizeWindowHandler(Nullable onWindowResized_f windowResizeHandler) noexcept { _windowResizeHandler = windowResizeHandler; }
    inline void setMouseEventHandler(Nullable onMouseEvent_f mouseEventHandler) noexcept { _mouseEventHandler = mouseEventHandler; }
    inline void setMouseMoveHandler(Nullable onMouseMove_f mouseMoveHandler) noexcept { _mouseMoveHandler = mouseMoveHandler; }
    inline void setKeyEventHandler(Nullable onKeyEvent_f keyEventHandler) noexcept { _keyEventHandler = keyEventHandler; }

    bool createWindow() noexcept;
    void closeWindow() noexcept;
    void showWindow() const noexcept;
    void hideWindow() const noexcept;

    /**
     * Creates a context for the window.
     * 
     * @return
     *    Returns true if the context was successfully created.
     */
    bool createContext() noexcept;

    /**
     * Makes the context current so that it can be rendered to.
     */
    void makeContextCurrent() const noexcept;

    /**
     *   Swaps the current frame buffer. Only needed if not 
     * rendering in double buffered mode
     */
    void swapBuffers() const noexcept;
public:
#ifdef _WIN32
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    friend void removeWindow(NotNull<Window>);
    friend Nullable Window* getWindowFromHandle(HWND);
#endif
};
