/**
 * @file
 * 
 * The windows implementation of {@link Window}.
 */
#ifdef _WIN32
#pragma warning(push, 0)
#include <windowsx.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma warning(pop)

#include <TauEngine.hpp>
#include <system/Window.hpp>
#include <EnumBitFields.hpp>

#ifndef MAX_WINDOW_COUNT
/**
 * The maximum number of windows that are being stored.
 * 
 *   This is used with {@link windowHandles} to be able to 
 * retrieve a {@link Window} from an {@link HWND}.
 * 
 *   This can be overriden by defining `MAX_WINDOW_COUNT`
 * in the pre processor before building the DLL.
 */
 #define MAX_WINDOW_COUNT 16
#endif

/**
 *   This is the name of a Windows class. This is used for 
 * interfacing with the OS.
 */
static const char* CLASS_NAME = "TauEngineWindowClass";

/**
 *   When we get a message for a window we only receive the
 * handle, thus we need to store the reference to the actual
 * window somewhere. That somewhere is in this array.
 */
static Window* windowHandles[MAX_WINDOW_COUNT];

/**
 * This is the current index for the {@link windowHandles} array.
 * It serves to let us easily insert the next window.
 */
static u32 currentWindowHandleIndex = 0;

/**
 * Adds a window to {@link windowHandles}.
 * 
 * @param[in] systemWindowContainer
 *    The {@link Window} to put into {@link windowHandles}.
 * @return 
 *      Returns true if the `systemWindowContainer` was 
 *    successfully added to {@link windowHandles}.
 */
static bool addWindow(NonNull Window* systemWindowContainer) noexcept
{
    if(currentWindowHandleIndex < MAX_WINDOW_COUNT)
    {
        windowHandles[currentWindowHandleIndex++] = systemWindowContainer;
        return true;
    }
    return false;
}

/**
 *   Removes a window from {@link windowHandles}. After the 
 * window is removed, all other pointers are shifted down if
 * necessary.
 * 
 * @param[in] systemWindowContainer
 *    The {@link Window} to remove from {@link windowHandles}.
 */
static void removeWindow(NotNull<const Window> systemWindowContainer) noexcept
{
    for(u32 i = 0; i < currentWindowHandleIndex; ++i)
    {
        if(systemWindowContainer->_windowContainer.windowHandle == windowHandles[i]->_windowContainer.windowHandle)
        {
            memcpy(windowHandles + i, windowHandles + i + 1, --currentWindowHandleIndex - i);
#if _DEBUG
            //   If in debug mode zero out the location, this is useful
            // for debugging. In practice, this is actually useless for 
            // the system and requires a memory write.
            windowHandles[currentWindowHandleIndex] = null;
#endif
            return;
        }
    }
}

/**
 * Retrieves a {@link Window} from {@link windowHandles}.
 * 
 * @param[in] handle
 *    The handle of the {@link Window} to retrieve.
 * @return 
 *      A pointer to the {@link Window} containing the 
 *    {@link HWND} `handle`. returns null if no window is 
 *    currently holding the referenced handle.
 */
static Nullable Window* getWindowFromHandle(HWND handle) noexcept
{
    for(u32 i = 0; i < currentWindowHandleIndex; ++i)
    {
        if(windowHandles[i]->_windowContainer.windowHandle == handle)
        {
            return windowHandles[i];
        }
    }

    return null;
}

/**
 *   Returns a {@link MouseFlags} bit field represented by the 
 * {@link WPARAM}.
 * 
 *   If `TRUST_RAW_MOUSE_PARAM` is defined as `true` then this 
 * is just a simple cast, otherwise we manually go through 
 * each flag and binary OR in the equivalent 
 * {@link MouseFlags}.
 * 
 * @param[in] wParam
 *      The {@link WPARAM} representing a bit mask of flags 
 *    for use with mouse events.
 * @return
 *    An equivalent {@link MouseFlags} enum bit mask.
 */
static MouseFlags mouseFlagsFromWParam(WPARAM wParam) noexcept
{
#ifdef TRUST_RAW_MOUSE_PARAM
    return static_cast<MouseFlags>(wParam);
#else
    MouseFlags out = static_cast<MouseFlags>(0);

    if(wParam & MK_LBUTTON)
    {
        out |= MouseFlags::MF_LEFT_BUTTON_DOWN;
    }
    if(wParam & MK_RBUTTON)
    {
        out |= MouseFlags::MF_RIGHT_BUTTON_DOWN;
    }
    if(wParam & MK_SHIFT)
    {
        out |= MouseFlags::MF_SHIFT_KEY_DOWN;
    }
    if(wParam & MK_CONTROL)
    {
        out |= MouseFlags::MF_CTRL_KEY_DOWN;
    }
    if(wParam & MK_MBUTTON)
    {
        out |= MouseFlags::MF_MIDDLE_BUTTON_DOWN;
    }

    const UINT xButton = GET_XBUTTON_WPARAM(wParam);
    if(xButton == XBUTTON1)
    {
        out |= MouseFlags::MF_X_BUTTON_1_DOWN;
    }
    if(xButton == XBUTTON2)
    {
        out |= MouseFlags::MF_X_BUTTON_2_DOWN;
    }

    return out;
#endif
}

/**
 *   Returns a {@link MouseEvent} based on the message type. 
 * `wParam` is also needed for identifying which X button was
 * pressed.
 * 
 * @param[in] uMsg
 *      The message type, this is the primary component in 
 *    creating the {@link MouseEvent}.
 * @param[in] wParam
 *    Used to determine which X button may have been pressed.
 * @return
 *    A {@link MouseEvent} representing which event was fired.
 */
static MouseEvent mouseEventFromMsg(UINT uMsg, WPARAM wParam) noexcept
{
    const UINT xButton = GET_XBUTTON_WPARAM(wParam);
    switch(uMsg)
    {
        case WM_LBUTTONDOWN: return MouseEvent::ME_LEFT_BUTTON_DOWN;
        case WM_LBUTTONUP:   return MouseEvent::ME_LEFT_BUTTON_UP;
        case WM_MBUTTONDOWN: return MouseEvent::ME_MIDDLE_BUTTON_DOWN;
        case WM_MBUTTONUP:   return MouseEvent::ME_MIDDLE_BUTTON_UP;
        case WM_RBUTTONDOWN: return MouseEvent::ME_RIGHT_BUTTON_DOWN;
        case WM_RBUTTONUP:   return MouseEvent::ME_RIGHT_BUTTON_UP;
        case WM_XBUTTONDOWN:
            if(xButton == XBUTTON1) { return MouseEvent::ME_X_BUTTON_1_DOWN; }
            if(xButton == XBUTTON2) { return MouseEvent::ME_X_BUTTON_2_DOWN; }
            break;
        case WM_XBUTTONUP:
            if(xButton == XBUTTON1) { return MouseEvent::ME_X_BUTTON_1_UP; }
            if(xButton == XBUTTON2) { return MouseEvent::ME_X_BUTTON_2_UP; }
            break;
        default: break;
    }
    return static_cast<MouseEvent>(0xFF);
}

static KeyboardFlags getKeyboardFlags() noexcept
{
    KeyboardFlags ret = static_cast<KeyboardFlags>(0);

    if(GetKeyState(VK_SHIFT)    & 0x8000) { ret |= KeyboardFlags::KF_SHIFT;       }
    if(GetKeyState(VK_CONTROL)  & 0x8000) { ret |= KeyboardFlags::KF_CTRL;        }
    if(GetKeyState(VK_MENU)     & 0x8000) { ret |= KeyboardFlags::KF_ALT;         }

    if(GetKeyState(VK_LSHIFT)   & 0x8000) { ret |= KeyboardFlags::KF_LEFT_SHIFT;  }
    if(GetKeyState(VK_RSHIFT)   & 0x8000) { ret |= KeyboardFlags::KF_RIGHT_SHIFT; }
    if(GetKeyState(VK_LCONTROL) & 0x8000) { ret |= KeyboardFlags::KF_LEFT_CTRL;   }
    if(GetKeyState(VK_RCONTROL) & 0x8000) { ret |= KeyboardFlags::KF_RIGHT_CTRL;  }
    if(GetKeyState(VK_LMENU)    & 0x8000) { ret |= KeyboardFlags::KF_LEFT_ALT;    }
    if(GetKeyState(VK_RMENU)    & 0x8000) { ret |= KeyboardFlags::KF_RIGHT_ALT;   }

    return ret;
}

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* window = getWindowFromHandle(windowHandle);
    const u32 xPos = static_cast<u32>(GET_X_LPARAM(lParam));
    const u32 yPos = static_cast<u32>(GET_Y_LPARAM(lParam));

    switch(uMsg)
    {
        case WM_ACTIVATE:
            if(window && window->_windowActiveHandler)
            {
                window->_windowActiveHandler(wParam != WA_INACTIVE, window);
            }
            break;
        case WM_SIZE:
            if(window)
            {
                if(wParam == SIZE_MINIMIZED)
                {
                    window->_windowState = WindowState::MINIMIZED;
                } 
                else if(wParam == SIZE_MAXIMIZED)
                {
                    window->_windowState = WindowState::MAXIMIZED; 
                    CALL_WINDOW_RESIZE_HANDLER:
                    if(window->_windowResizeHandler)
                    {
                        const u32 width = LOWORD(lParam);
                        const u32 height = HIWORD(lParam);
                        window->_width = width;
                        window->_height = height;
                        window->_windowResizeHandler(width, height, window);
                    }
                }
                else if(wParam == SIZE_RESTORED)
                {
                    if(window->_windowState == WindowState::MINIMIZED || window->_windowState == WindowState::MAXIMIZED)
                    {
                        window->_windowState = WindowState::NEITHER;
                    }
                    goto CALL_WINDOW_RESIZE_HANDLER;
                }
            }
            break;
        case WM_CLOSE:
            if(window) { window->closeWindow(); }
            break;
        case WM_DESTROY:
            if(window) { window->closeWindow(); }
            if(currentWindowHandleIndex == 0) { tauExit(0); }
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            if(window && window->_mouseEventHandler)
            {
                window->_mouseEventHandler(mouseEventFromMsg(uMsg, wParam), mouseFlagsFromWParam(wParam), xPos, yPos, window);
            }
            break;
        case WM_MOUSEMOVE:
            if(window && window->_mouseMoveHandler)
            {
                window->_mouseMoveHandler(mouseFlagsFromWParam(wParam), xPos, yPos, window);
            }
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if(window && window->_keyEventHandler)
            {
                if(lParam & (1 << 30))
                {
                    window->_keyEventHandler(KeyboardEvent::KE_KEY_HELD, getKeyboardFlags(), wParam, window);
                }
                else
                {
                    window->_keyEventHandler(KeyboardEvent::KE_KEY_PRESSED, getKeyboardFlags(), wParam, window);
                }
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (window && window->_keyEventHandler)
            {
                window->_keyEventHandler(KeyboardEvent::KE_KEY_RELEASED, getKeyboardFlags(), wParam, window);
            }
            break;
        case WM_CHAR:
            if(window && window->_asciiKeyEventHandler)
            {
                window->_asciiKeyEventHandler((wchar_t) wParam, (char) wParam, window);
            }
            break;
        case WM_MOVE:
            if(window)
            {
                window->_xPos = xPos;
                window->_yPos = yPos;
            }
            break;
        default: return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(u32 width, u32 height, Nullable const char* title, Nullable const void* userContainer, Nullable const Window* parent) noexcept
    : _width(width), 
      _height(height), 
      _xPos(0),
      _yPos(0),
      _title(title),
      _windowContainer({ { }, null, null, null }),
      _userContainer(userContainer),
      _parent(parent),
      _contextSettings({ 3, 1, { { false, false, true, false, 0,0,0,0 } } }),
      _windowState(WindowState::NEITHER),
      _windowResizeHandler(null),
      _mouseEventHandler(null),
      _mouseMoveHandler(null),
      _keyEventHandler(null),
      _asciiKeyEventHandler(null),
      _windowActiveHandler(null)
{ }

Window::~Window() noexcept
{
    this->closeWindow();
}

void Window::resize(const u32 width, const u32 height) noexcept
{
    this->_width = width;
    this->_height = height;
    SetWindowPos(_windowContainer.windowHandle, null, 0, 0, static_cast<int>(width), static_cast<int>(height), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    updateViewPort(0, 0);
}

void Window::setTitle(const char* title) noexcept
{
    this->_title = title;
    SetWindowTextA(_windowContainer.windowHandle, title);
}

bool Window::createWindow() noexcept
{
    addWindow(this);

    WNDCLASSA* windowClass = &this->_windowContainer.windowClass;

    windowClass->lpfnWndProc = WindowProc;
    windowClass->hInstance = GetModuleHandleA(null);
    windowClass->lpszClassName = CLASS_NAME;
    windowClass->style = CS_DBLCLKS;

    RegisterClassA(windowClass);

    HWND parent = null;

    if(this->_parent)
    {
        parent = this->_parent->_windowContainer.windowHandle;
    }

    this->_windowContainer.windowHandle = CreateWindowExA(0,
                                                          windowClass->lpszClassName,
                                                          this->_title,
                                                          WS_OVERLAPPEDWINDOW,
                                                          CW_USEDEFAULT, CW_USEDEFAULT, 
                                                          static_cast<i32>(this->_width), static_cast<i32>(this->_height),
                                                          parent,
                                                          null,
                                                          windowClass->hInstance,
                                                          (LPVOID) null);

    this->_windowContainer.hdc = GetDC(this->_windowContainer.windowHandle);

    RECT clientArea;
    GetClientRect(this->_windowContainer.windowHandle, &clientArea);
    this->_width = clientArea.right;
    this->_height = clientArea.bottom;

    return this->_windowContainer.windowHandle;
}

void Window::closeWindow() const noexcept
{
    removeWindow(this);
    if(this->_windowContainer.renderingContext)
    {
        unloadCurrentContext();
        wglDeleteContext(this->_windowContainer.renderingContext);      // TODO: Fix crash
    }
    if(this->_windowContainer.windowHandle)
    {
        DestroyWindow(_windowContainer.windowHandle);
    }
}

void Window::showWindow() const noexcept
{
    ShowWindow(this->_windowContainer.windowHandle, SW_SHOWNA);
    UpdateWindow(this->_windowContainer.windowHandle);

    SetActiveWindow(this->_windowContainer.windowHandle);
}

void Window::hideWindow() const noexcept
{
    ShowWindow(this->_windowContainer.windowHandle, SW_HIDE);
}

bool Window::createContext() noexcept
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pixelFormat = ChoosePixelFormat(this->_windowContainer.hdc, &pfd);

    if(pixelFormat == 0) { return false; }

    const BOOL res = SetPixelFormat(this->_windowContainer.hdc, pixelFormat, &pfd);

    if(!res) { return false; }

    const HGLRC tempContext = wglCreateContext(this->_windowContainer.hdc);  // NOLINT(misc-misplaced-const)
    wglMakeCurrent(this->_windowContainer.hdc, tempContext);

    if(glewInit() != GLEW_OK) { return false; }

    if(WGLEW_ARB_create_context)
    {
        const int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, this->_contextSettings.majorVersion,
            WGL_CONTEXT_MINOR_VERSION_ARB, this->_contextSettings.minorVersion,
            WGL_CONTEXT_FLAGS_ARB,        (this->_contextSettings.debug             ? WGL_CONTEXT_DEBUG_BIT_ARB                 : 0) |
                                          (this->_contextSettings.forwardCompatible ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    : 0),
            WGL_CONTEXT_PROFILE_MASK_ARB, (this->_contextSettings.coreProfile       ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB          : 0) |
                                          (this->_contextSettings.compatProfile     ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : 0),
            0
        };

        this->_windowContainer.renderingContext = wglCreateContextAttribsARB(this->_windowContainer.hdc, null, attribs);
        wglMakeCurrent(null, null);
        wglDeleteContext(tempContext);
        wglMakeCurrent(this->_windowContainer.hdc, this->_windowContainer.renderingContext);
    }
    else
    {
        this->_windowContainer.renderingContext = tempContext;
    }

    updateViewPort(0, 0, this->_width, this->_height);

    return this->_windowContainer.renderingContext;
}

void Window::makeContextCurrent() const noexcept
{
    if(!this->_windowContainer.hdc || !this->_windowContainer.renderingContext)
    {
        return;
    }
    wglMakeCurrent(this->_windowContainer.hdc, this->_windowContainer.renderingContext);
}

void Window::unloadCurrentContext() noexcept
{
    wglMakeCurrent(null, null);
}

void Window::swapBuffers() const noexcept
{
    if(this->_windowContainer.hdc)
    {
        SwapBuffers(this->_windowContainer.hdc);
    }
}

void Window::updateViewPort(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) const noexcept
{
    glViewport(x, y, width, height);
}

void Window::updateViewPort(u32 x, u32 y, float minZ, float maxZ) const noexcept
{
    glViewport(x, y, this->_width, this->_height);
}
#endif
