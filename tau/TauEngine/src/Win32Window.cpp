/**
 * @file
 * 
 * The Win32 implementation of {@link Window @endlink}.
 */
#ifdef _WIN32
#pragma warning(push, 0)
#include <windowsx.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <cstdio>
#pragma warning(pop)

#include <TauEngine.hpp>
#include <system/Window.hpp>
#include <EnumBitFields.hpp>
#include <events/WindowEvent.hpp>
#include <utility>

#ifndef MAX_WINDOW_COUNT
/**
 * The maximum number of windows that are being stored.
 * 
 *   This is used with {@link windowHandles @endlink} to be able to 
 * retrieve a {@link Window @endlink} from an {@link HWND @endlink}.
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
// static constexpr const char* CLASS_NAME = "TauEngineWindowClass";

/**
 *   When we get a message for a window we only receive the
 * handle, thus we need to store the reference to the actual
 * window somewhere. That somewhere is in this array.
 */
static Window* windowHandles[MAX_WINDOW_COUNT];

/**
 * This is the current index for the {@link windowHandles @endlink} array.
 * It serves to let us easily insert the next window.
 */
static u32 currentWindowHandleIndex = 0;

/**
 * Subtracts a window to {@link windowHandles @endlink}.
 * 
 * @param[in] systemWindowContainer
 *    The {@link Window @endlink} to put into {@link windowHandles @endlink}.
 * @return 
 *      Returns true if the `systemWindowContainer` was 
 *    successfully added to {@link windowHandles @endlink}.
 */
static bool addWindow(Window& systemWindowContainer) noexcept
{
    if(currentWindowHandleIndex < MAX_WINDOW_COUNT)
    {
        windowHandles[currentWindowHandleIndex++] = &systemWindowContainer;
        return true;
    }
    return false;
}

/**
 *   Removes a window from {@link windowHandles @endlink}. After the 
 * window is removed, all other pointers are shifted down if
 * necessary.
 * 
 * @param[in] systemWindowContainer
 *    The {@link Window @endlink} to remove from {@link windowHandles @endlink}.
 */
static void removeWindow(NotNull<const Window> systemWindowContainer) noexcept
{
    for(u32 i = 0; i < currentWindowHandleIndex; ++i)
    {
        if(systemWindowContainer->_windowContainer.windowHandle == windowHandles[i]->_windowContainer.windowHandle)
        {
            memcpy(windowHandles + i, windowHandles + i + 1, --currentWindowHandleIndex - i);
#if !defined(TAU_PRODUCTION)
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
 * Retrieves a {@link Window @endlink} from {@link windowHandles @endlink}.
 * 
 * @param[in] handle
 *    The handle of the {@link Window @endlink} to retrieve.
 * @return 
 *      A pointer to the {@link Window @endlink} containing the 
 *    {@link HWND @endlink} `handle`. returns null if no window is 
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
 *   Returns a {@link MouseFlags @endlink} bit field represented by the 
 * {@link WPARAM @endlink}.
 * 
 *   If `TRUST_RAW_MOUSE_PARAM` is defined as `true` then this 
 * is just a simple cast, otherwise we manually go through 
 * each flag and binary OR in the equivalent 
 * {@link MouseFlags @endlink}.
 * 
 * @param[in] wParam
 *      The {@link WPARAM @endlink} representing a bit mask of flags 
 *    for use with mouse events.
 * @return
 *    An equivalent {@link MouseFlags @endlink} enum bit mask.
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
 *   Returns a {@link MouseEvent @endlink} based on the message type. 
 * `wParam` is also needed for identifying which X button was
 * pressed.
 * 
 * @param[in] uMsg
 *      The message type, this is the primary component in 
 *    creating the {@link MouseEvent @endlink}.
 * @param[in] wParam
 *    Used to determine which X button may have been pressed.
 * @return
 *    A {@link MouseEvent @endlink} representing which event was fired.
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

static void callWindowResizeHandler(Window& window, const LPARAM lParam) noexcept
{
    if(!window._eventHandler) { return; }

    const u32 width  = LOWORD(lParam);
    const u32 height = HIWORD(lParam);

    WindowResizeEvent evt(window, window._width, window._height, width, height);
    window._eventHandler(window._userContainer, evt);

    window._width  = width;
    window._height = height;
}

static Key convertFromWin32VKey(int key) noexcept;

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* window = getWindowFromHandle(windowHandle);
    const u32 xPos = static_cast<u32>(GET_X_LPARAM(lParam));
    const u32 yPos = static_cast<u32>(GET_Y_LPARAM(lParam));

    switch(uMsg)
    {
        case WM_ACTIVATE:
            if(window && window->_eventHandler)
            {
                WindowActiveEvent evt(*window, wParam != WA_INACTIVE);
                window->_eventHandler(window->_userContainer, evt);
            }
            break;
        case WM_SIZE:
            if(window)
            {
                if(wParam == SIZE_MINIMIZED)
                { window->_windowState = WindowState::MINIMIZED; } 
                else if(wParam == SIZE_MAXIMIZED)
                {
                    window->_windowState = WindowState::MAXIMIZED;
                    callWindowResizeHandler(*window, lParam);
                }
                else if(wParam == SIZE_RESTORED)
                {
                    if(window->_windowState == WindowState::MINIMIZED || window->_windowState == WindowState::MAXIMIZED)
                    { window->_windowState = WindowState::NEITHER; }
                    callWindowResizeHandler(*window, lParam);
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
            if(window && window->_eventHandler)
            {
                if(lParam & (1 << 30))
                {
                    WindowKeyEvent evt(*window, KeyboardEvent::KE_KEY_HELD, getKeyboardFlags(), convertFromWin32VKey(wParam));
                    window->_eventHandler(window->_userContainer, evt);
                }
                else
                {
                    WindowKeyEvent evt(*window, KeyboardEvent::KE_KEY_PRESSED, getKeyboardFlags(), convertFromWin32VKey(wParam));
                    window->_eventHandler(window->_userContainer, evt);
                }
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if(window && window->_eventHandler)
            {
                WindowKeyEvent evt(*window, KeyboardEvent::KE_KEY_RELEASED, getKeyboardFlags(), convertFromWin32VKey(wParam));
                window->_eventHandler(window->_userContainer, evt);
            }
            break;
        case WM_CHAR:
            if(window && window->_eventHandler)
            {
                WindowAsciiKeyEvent evt(*window, static_cast<wchar_t>(wParam), static_cast<char>(wParam));
                window->_eventHandler(window->_userContainer, evt);
            }
            break;
        case WM_MOVE:
            if(window)
            {
                window->_xPos = xPos;
                window->_yPos = yPos;
            }
            break;
        default: return DefWindowProcA(windowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(u32 width, u32 height, DynString title, Nullable void* userContainer, Nullable const Window* parent) noexcept
    : _width(width), _height(height), _xPos(0), _yPos(0),
      _title(std::move(title)),
      _windowContainer({ { }, null, null, null }),
      _userContainer(userContainer),
      _parent(parent),
      _contextSettings({ 3, 1, { { false, false, true, false, 0,0,0,0 } } }),
      _renderingMode(RenderingMode::getGlobalMode()),
      _windowState(WindowState::NEITHER),
      _eventHandler(null), _mouseEventHandler(null), _mouseMoveHandler(null)
{ }

Window::~Window() noexcept
{
    this->closeWindow();
}

void Window::resize(const u32 width, const u32 height) noexcept
{
    this->_width = width;
    this->_height = height;
    SetWindowPos(_windowContainer.windowHandle, null, 0, 0, static_cast<int>(width), static_cast<int>(height), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSENDCHANGING);
    updateViewPort(0, 0, width, height);
}

void Window::move(const u32 xPos, const u32 yPos) noexcept
{
    this->_xPos = xPos;
    this->_yPos = yPos;
    SetWindowPos(_windowContainer.windowHandle, null, static_cast<int>(xPos), static_cast<int>(yPos), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSENDCHANGING);
}

void Window::moveAndResize(const u32 xPos, const u32 yPos, const u32 width, const u32 height) noexcept
{
    this->_xPos = xPos;
    this->_yPos = yPos;
    this->_width = width;
    this->_height = height;
    SetWindowPos(_windowContainer.windowHandle, null, xPos, yPos, width, height, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSENDCHANGING);
    updateViewPort(0, 0, width, height);
}

void Window::setTitle(const DynString& title) noexcept
{
    this->_title = title;
    SetWindowTextA(_windowContainer.windowHandle, title);
}

bool Window::createWindow() noexcept
{
    addWindow(*this);

    WNDCLASSA* windowClass = &this->_windowContainer.windowClass;

    windowClass->lpfnWndProc = WindowProc;
    windowClass->hInstance = GetModuleHandleA(null);
    windowClass->lpszClassName = "TauEngineWindowClass";
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
                                                          static_cast<LPVOID>(null));

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
        wglDeleteContext(this->_windowContainer.renderingContext);
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

        const HGLRC attribContext = wglCreateContextAttribsARB(this->_windowContainer.hdc, null, attribs);  // NOLINT(misc-misplaced-const)

        if(!attribContext)
        {
            const ContextSettings& cs = this->_contextSettings;

            const DWORD errorCode     = GetLastError();
            const DWORD errorCodeLow  = errorCode & 0xFFFF;
            const DWORD errorCodeHigh = errorCode >> 16;

            fputs("Error creating context with attributes.\n", stderr);
             
            LPVOID lpMsgBuf;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           null,
                           errorCodeLow,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           reinterpret_cast<LPSTR>(&lpMsgBuf),
                           0, null);

            fprintf(stderr, "Error string from system (may not be accurate): %s\n", reinterpret_cast<LPSTR>(lpMsgBuf));
            LocalFree(lpMsgBuf);

            fprintf(stderr, "Error: %lu (0x%08lX)\n", errorCode, errorCode);
            fprintf(stderr, "Error Low: %lu (0x%04lX)\n", errorCodeLow, errorCodeLow);
            fprintf(stderr, "Error High: %lu [%lu] (0x%04lX)\n", errorCodeHigh, errorCodeHigh << 16, errorCodeHigh);

            fputs("Requested OpenGL\n", stderr);
            fprintf(stderr, "  Version: %u.%u%s\n", cs.majorVersion, cs.minorVersion, cs.forwardCompatible ? " Forward Compatible" : "");
            if(cs.coreProfile)
            {
                fputs("  Core Profile\n", stderr);
            }
            if(cs.compatProfile)
            {
                fputs("  Compatibility Profile\n", stderr);
            }
            if(cs.debug)
            {
                fputs("  Debug Context\n", stderr);
            }

            if(errorCodeLow == ERROR_INVALID_VERSION_ARB)
            {
                fputs("Error Code Enum: ERROR_INVALID_VERSION_ARB\n", stderr);
                fputs("Invalid OpenGL Version Requested\n", stderr);

                if(cs.majorVersion < 1)
                {
                    fputs("The minimum requestable OpenGL major version is 1.\n", stderr);
                }
                else if(cs.majorVersion == 1 && cs.minorVersion > 5)
                {
                    fputs("The maximum requestable OpenGL minor version with major version 1 is 5.\n", stderr);
                }
                else if(cs.majorVersion == 2 && cs.minorVersion > 1)
                {
                    fputs("The maximum requestable OpenGL minor version with major version 2 is 1.\n", stderr);
                }
                else if(cs.majorVersion > 4)
                {
                    fputs("At the time of compilation the maximum requestable OpenGL major version is 4.\n", stderr);
                }

                if(cs.majorVersion < 3 && cs.majorVersion > 0 && cs.forwardCompatible)
                {
                    fputs("Forward Compatible profiles are not compatible with OpenGL major version less than 3.\n", stderr);
                }
            }
            else if(errorCodeLow == ERROR_INVALID_PROFILE_ARB)
            {
                fputs("Error Code Enum: ERROR_INVALID_PROFILE_ARB\n", stderr);
                fputs("Invalid OpenGL Profile Requested\n", stderr);

                if(!cs.coreProfile && !cs.compatProfile)
                {
                    fputs("Neither Core Profile nor Compatibility Profile was requested. This is an invalid request.\n", stderr);
                }
                else if(cs.coreProfile && cs.compatProfile)
                {
                    fputs("Core Profile and Compatibility Profile was requested. This is an invalid request.\n", stderr);
                }

                const u32 otherFlags = attribs[7] & ~(WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
                if(otherFlags != 0)
                {
                    fputs("Flags other than `WGL_CONTEXT_CORE_PROFILE_BIT_ARB` and `WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB` were set.\n", stderr);
                    fprintf(stderr, "Flags: 0x%x\n", attribs[7]);
                    fprintf(stderr, "Other Flags Flags: 0x%08x\n", otherFlags);
                }
            }
            else if(errorCodeLow == ERROR_INVALID_OPERATION)
            {
                fputs("Error Code Enum: ERROR_INVALID_OPERATION\n", stderr);
                fprintf(stderr, "hSharedContext is neither null nor a valid context.\n");
            }
            else if(errorCodeLow == ERROR_DC_NOT_FOUND)
            {
                fputs("Error Code Enum: ERROR_DC_NOT_FOUND\n", stderr);
                fprintf(stderr, "Invalid Device Context\n");
            }
            else if(errorCodeLow == ERROR_INVALID_PIXEL_FORMAT)
            {
                fputs("Error Code Enum: ERROR_INVALID_PIXEL_FORMAT\n", stderr);
                fprintf(stderr, "Invalid Pixel Format\n");
            }
            else if(errorCodeLow == ERROR_NO_SYSTEM_RESOURCES)
            {
                fputs("Error Code Enum: ERROR_NO_SYSTEM_RESOURCES\n", stderr);
                fprintf(stderr, "Insufficient System Resources\n");
            }
            else if(errorCodeLow == ERROR_INVALID_PARAMETER)
            {
                fputs("Error Code Enum: ERROR_INVALID_PARAMETER\n", stderr);
                fprintf(stderr, "Unrecognized Attribute In Attribute List\n");
            }
            else
            {
                fprintf(stderr, "Error code did not match any expected errors.\n");
            }

            return false;
        }

        wglMakeCurrent(null, null);
        wglDeleteContext(tempContext);
        wglMakeCurrent(this->_windowContainer.hdc, this->_windowContainer.renderingContext);

        this->_windowContainer.renderingContext = attribContext;
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

void Window::getMousePos(i32& x, i32& y) const noexcept
{
    POINT p;
    GetCursorPos(&p);

    ScreenToClient(this->_windowContainer.windowHandle, &p);

    x = p.x;
    y = p.y;
}

void Window::setMousePos(i32 x, i32 y) const noexcept
{
    POINT p { x, y };
    ClientToScreen(this->_windowContainer.windowHandle, &p);
    SetCursorPos(p.x, p.y);
}

static int convertToWin32VKey(Key key) noexcept
{
    constexpr int ka = static_cast<int>(Key::A);
    const int k = static_cast<int>(key) - ka;

    constexpr int VK_A = 0x41;
    constexpr int VK_0 = 0x30;
    constexpr int VK_N0 = VK_NUMPAD0;
    constexpr int _VK_F1 = VK_F1;

    if(key >= Key::A && key <= Key::Z)
    { return k + VK_A; }
    if(key >= Key::K0 && key <= Key::K9)
    { return k + VK_0; }
    if(key >= Key::N0 && key <= Key::N9)
    { return k + VK_N0; }
    if(key >= Key::F1 && key <= Key::F24)
    { return k + _VK_F1; }

    switch(key)
    {
        case Key::Space: return VK_SPACE;
        case Key::Minus: return VK_OEM_MINUS;
        case Key::Equals: return VK_OEM_PLUS;
        case Key::Comma: return VK_OEM_COMMA;
        case Key::Dot: return VK_OEM_PERIOD;
        case Key::ForwardSlash: return VK_OEM_2;
        case Key::LBracket: return VK_OEM_4;
        case Key::RBracket: return VK_OEM_6;
        case Key::SemiColon: return VK_OEM_1;
        case Key::Quote: return VK_OEM_7;
        case Key::NDivide: return VK_DIVIDE;
        case Key::NSubtract: return VK_SUBTRACT;
        case Key::NMultiply: return VK_MULTIPLY;
        case Key::NAdd: return VK_ADD;
        case Key::NDot: return VK_DECIMAL;
        case Key::Enter: return VK_RETURN;
        case Key::NumLock: return VK_NUMLOCK;
        case Key::BackTick: return VK_OEM_3;
        case Key::Esc: return VK_ESCAPE;
        case Key::Tab: return VK_TAB;
        case Key::CapsLock: return VK_CAPITAL;
        case Key::Shift: return VK_SHIFT;
        case Key::LShift: return VK_LSHIFT;
        case Key::RShift: return VK_RSHIFT;
        case Key::Ctrl: return VK_CONTROL;
        case Key::LCtrl: return VK_LCONTROL;
        case Key::RCtrl: return VK_RCONTROL;
        case Key::Alt: return VK_MENU;
        case Key::LAlt: return VK_LMENU;
        case Key::RAlt: return VK_RMENU;
        case Key::LWin: return VK_LWIN;
        case Key::RWin: return VK_RWIN;
        case Key::BackSpace: return VK_BACK;
        case Key::Up: return VK_UP;
        case Key::Down: return VK_DOWN;
        case Key::Left: return VK_LEFT;
        case Key::Right: return VK_RIGHT;
        case Key::PrintScreen: return VK_SNAPSHOT;
        case Key::ScrollLock: return VK_SCROLL;
        case Key::PauseBreak: return VK_PAUSE;
        case Key::Insert: return VK_INSERT;
        case Key::Delete: return VK_DELETE;
        case Key::Home: return VK_HOME;
        case Key::End: return VK_END;
        case Key::PageUp: return VK_PRIOR;
        case Key::PageDown: return VK_NEXT;
        default: return -1;
    }
}

static Key convertFromWin32VKey(int key) noexcept
{
    constexpr int ka = static_cast<int>(Key::A);
    const int k = key + ka;

    constexpr int VK_A = 0x41;
    constexpr int VK_Z = 0x5A;
    constexpr int VK_0 = 0x30;
    constexpr int VK_9 = 0x39;
    constexpr int VK_N0 = VK_NUMPAD0;
    constexpr int VK_N9 = VK_NUMPAD9;
    constexpr int _VK_F1 = VK_F1;
    constexpr int _VK_F24 = VK_F24;

    if(key >= VK_A && key <= VK_Z)
    {
        return static_cast<Key>(k - VK_A);
    }
    if(key >= VK_0 && key <= VK_9)
    {
        return static_cast<Key>(k - VK_0);
    }
    if(key >= VK_N0 && key <= VK_N9)
    {
        return static_cast<Key>(k - VK_N0);
    }
    if(key >= _VK_F1 && key <= _VK_F24)
    {
        return static_cast<Key>(k - _VK_F1);
    }

    switch(key)
    {
        case VK_SPACE: return Key::Space;
        case VK_OEM_MINUS: return Key::Minus;
        case VK_OEM_PLUS: return Key::Equals;
        case VK_OEM_COMMA: return Key::Comma;
        case VK_OEM_PERIOD: return Key::Dot;
        case VK_OEM_2: return Key::ForwardSlash;
        case VK_OEM_4: return Key::LBracket;
        case VK_OEM_6: return Key::RBracket;
        case VK_OEM_1: return Key::SemiColon;
        case VK_OEM_7: return Key::Quote;
        case VK_DIVIDE: return Key::NDivide;
        case VK_SUBTRACT: return Key::NSubtract;
        case VK_MULTIPLY: return Key::NMultiply;
        case VK_ADD: return Key::NAdd;
        case VK_DECIMAL: return Key::NDot;
        case VK_RETURN: return Key::Enter;
        case VK_NUMLOCK: return Key::NumLock;
        case VK_OEM_3: return Key::BackTick;
        case VK_ESCAPE: return Key::Esc;
        case VK_TAB: return Key::Tab;
        case VK_CAPITAL: return Key::CapsLock;
        case VK_SHIFT: return Key::Shift;
        case VK_LSHIFT: return Key::LShift;
        case VK_RSHIFT: return Key::RShift;
        case VK_CONTROL: return Key::Ctrl;
        case VK_LCONTROL: return Key::LCtrl;
        case VK_RCONTROL: return Key::RCtrl;
        case VK_MENU: return Key::Alt;
        case VK_LMENU: return Key::LAlt;
        case VK_RMENU: return Key::RAlt;
        case VK_LWIN: return Key::LWin;
        case VK_RWIN: return Key::RWin;
        case VK_BACK: return Key::BackSpace;
        case VK_UP: return Key::Up;
        case VK_DOWN: return Key::Down;
        case VK_LEFT: return Key::Left;
        case VK_RIGHT: return Key::Right;
        case VK_SNAPSHOT: return Key::PrintScreen;
        case VK_SCROLL: return Key::ScrollLock;
        case VK_PAUSE: return Key::PauseBreak;
        case VK_INSERT: return Key::Insert;
        case VK_DELETE: return Key::Delete;
        case VK_HOME: return Key::Home;
        case VK_END: return Key::End;
        case VK_PRIOR: return Key::PageUp;
        case VK_NEXT: return Key::PageDown;
        default: return Key::Unknown;
    }
}

bool Window::isKeyPressed(Key key) const noexcept
{
    if(key == Key::Win)
    {
        return GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000;
    }
    const int keyCode = convertToWin32VKey(key);
    return keyCode == -1 ? false : GetKeyState(keyCode) & 0x8000;
}

i32 Window::isKeyLocked(Key key) const noexcept
{
    switch(key)
    {
        case Key::NumLock:
        case Key::CapsLock:
        case Key::ScrollLock: return (GetKeyState(convertToWin32VKey(key)) & 0x0001) ? 1 : 0;
        default: return -1;
    }
}

bool Window::isKeyActive(Key key) const noexcept
{
    const int keyCode = convertToWin32VKey(key);
    switch(key)
    {
        case Key::NumLock:
        case Key::CapsLock:
        case Key::ScrollLock: return GetKeyState(keyCode) & 0x0001;
        default: break;
    }
    if(key == Key::Win)
    {
        return GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000;
    }
    return keyCode == -1 ? false : GetKeyState(keyCode) & 0x8000;
}
#endif
