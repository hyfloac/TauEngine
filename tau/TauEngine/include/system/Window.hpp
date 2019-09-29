/**
 * @file
 * 
 * Describes an abstract window.
 * 
 *   This class should be fairly OS-independent. Any OS 
 * dependent stuff should either be put in {@link _SysWindowContainer @endlink}
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
#include <Safeties.hpp>
#include <RenderingMode.hpp>

class Window;
class Event;

/**
 * Handles a resize of the window. This should only be used to reset the graphics.
 * 
 * Do not call {@link Window::resize(const u32,const u32) @endlink}.
 */
// typedef void (* onWindowResized_f)(const u32 width, const u32 height, NonNull Window* window);

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
typedef void (* onMouseEvent_f)(const MouseEvent mouseEvent, const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, NonNull Window* window);

typedef void (* onMouseMove_f)(const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, NonNull Window* window);

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
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) noexcept;
static void removeWindow(NotNull<const Window>) noexcept;
static Nullable Window* getWindowFromHandle(HWND) noexcept;
#endif

class TAU_DLL Window
{
public:
    typedef void(*onEvent_f)(Event&);
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
     *   The current X position of the window. This can change
     * by either being manually resized, or by being
     * programatically resized.
     */
    u32 _xPos;
    /**
     *   The current Y position of the window. This can change
     * by either being manually resized, or by being
     * programatically resized.
     */
    u32 _yPos;
    /**
     *   The title of the window. The string can change, the 
     * contents cannot.
     */
    const char* _title;
    /**
     * A system dependent container of information.
     * 
     * See {@link _SysWindowContainer @endlink}.
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
    RenderingMode& _renderingMode;

    /**
     * The current window state.
     * 
     * See {@link WindowState @endlink}.
     */
    WindowState _windowState;

    onEvent_f _eventHandler;

    onMouseEvent_f    _mouseEventHandler;
    onMouseMove_f     _mouseMoveHandler;
public:
    /**
     * Unloads the current context. 
     * 
     * Not much use for this except when exiting.
     */
    static void unloadCurrentContext() noexcept;
public:
    Window(u32 width, u32 height, Nullable const char* title, Nullable const void* userContainer = null, Nullable const Window* parent = null) noexcept;

    ~Window() noexcept;

#pragma region Copy and Move Operations
    Window(const Window& copy) noexcept = delete;
    Window(Window&& move) noexcept = delete;

    Window& operator =(const Window& copy) noexcept = delete;
    Window& operator =(Window&& move) noexcept = delete;
#pragma endregion

#pragma region Getters
    [[nodiscard]] inline u32 width()  const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }
    [[nodiscard]] inline u32 xPos()   const noexcept { return _xPos; }
    [[nodiscard]] inline u32 yPos()   const noexcept { return _yPos; }
    [[nodiscard]] inline const char* title() const noexcept { return _title; }
    [[nodiscard]] inline const void* userContainer() const noexcept { return _userContainer; }
    [[nodiscard]] inline const Window* parent() const noexcept { return _parent; }
    [[nodiscard]] inline ContextSettings& contextSettings() noexcept { return _contextSettings; }
    [[nodiscard]] inline RenderingMode& renderingMode() noexcept { return _renderingMode; }
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

    /**
     * Moves the window.
     *
     * @param[in] xPos
     *      The new x position of the window.
     * @param[in] yPos
     *      The new y position of the window.
     */
    void move(const u32 xPos, const u32 yPos) noexcept;

    /**
     * Moves and resizes the window.
     *
     * @param[in] xPos
     *      The new x position of the window.
     * @param[in] yPos
     *      The new y position of the window.
     * @param[in] width
     *      The new width of the window.
     * @param[in] height
     *      The new height of the window.
     */
    void moveAndResize(const u32 xPos, const u32 yPos, const u32 width, const u32 height) noexcept;

    void setTitle(const char* title) noexcept;

    inline void setEventHandler(Nullable onEvent_f eventHandler) noexcept { _eventHandler = eventHandler;  }

    inline void setMouseEventHandler(Nullable onMouseEvent_f mouseEventHandler)          noexcept { _mouseEventHandler    = mouseEventHandler;    }
    inline void setMouseMoveHandler(Nullable onMouseMove_f mouseMoveHandler)             noexcept { _mouseMoveHandler     = mouseMoveHandler;     }

    bool createWindow()      noexcept;
    void closeWindow() const noexcept;
    void showWindow()  const noexcept;
    void hideWindow()  const noexcept;

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

    void updateViewPort(u32 x, u32 y, u32 width, u32 height, float minZ = 0.0f, float maxZ = 1.0f) const noexcept;
    void updateViewPort(u32 x, u32 y, float minZ = 0.0f, float maxZ = 1.0f) const noexcept;

    void getMousePos(i32& x, i32& y) const noexcept;
    void setMousePos(i32 x, i32 y) const noexcept;
public:
#ifdef _MSVC_LANG
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) noexcept;
    friend void removeWindow(NotNull<const Window>) noexcept;
    friend Nullable Window* getWindowFromHandle(HWND) noexcept;
#endif
};
