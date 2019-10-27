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
#include <Safeties.hpp>
#include <RenderingMode.hpp>
#include "events/WindowEvent.hpp"
#include "system/RenderingContext.hpp"

class Window;

enum class WindowState : u8
{
    MINIMIZED = 0,
    MAXIMIZED,
    NEITHER
};

#ifdef _WIN32
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) noexcept;
static void removeWindow(NotNull<const Window>) noexcept;
static Nullable Window* getWindowFromHandle(HWND) noexcept;
static void callWindowResizeHandler(Window& window, const LPARAM lParam) noexcept;
#endif

class TAU_DLL Window
{
public:
    typedef void(*onEvent_f)(void*, WindowEvent&);
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
    DynString _title;
    /**
     * A system dependent container of information.
     * 
     * See {@link _SysWindowContainer @endlink}.
     */
    _SysWindowContainer _windowContainer;
    IRenderingContext* _context;
    // SharedRenderingContextsContainer _contexts;
    /**
     *   A pointer to a user dependent structure. This can be
     * anything, it is up to the developer.
     */
    void* _userContainer;
    /**
     * The parent window. This cannot be modified.
     */
    const Window* _parent;
    RenderingMode& _renderingMode;
    /**
     * The current window state.
     * 
     * See {@link WindowState @endlink}.
     */
    WindowState _windowState;
    onEvent_f _eventHandler;
public:
    Window(u32 width, u32 height, DynString title, Nullable void* userContainer = null, Nullable const Window* parent = null) noexcept;

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
    [[nodiscard]] IRenderingContext* renderingContext() const noexcept { return _context; }
    [[nodiscard]] inline const void* userContainer() const noexcept { return _userContainer; }
    [[nodiscard]] inline const Window* parent() const noexcept { return _parent; }
    [[nodiscard]] inline RenderingMode& renderingMode() const noexcept { return _renderingMode; }
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

    void setTitle(const DynString& title) noexcept;

    inline void setEventHandler(Nullable onEvent_f eventHandler) noexcept { _eventHandler = eventHandler;  }

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
     *   Swaps the current frame buffer. Only needed if not 
     * rendering in double buffered mode
     */
    void swapBuffers() const noexcept;
public:
#ifdef _WIN32
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) noexcept;
    friend void removeWindow(NotNull<const Window>) noexcept;
    friend Nullable Window* getWindowFromHandle(HWND) noexcept;
    friend void callWindowResizeHandler(Window& window, const LPARAM lParam) noexcept;
#endif
};
