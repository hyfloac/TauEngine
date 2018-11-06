#ifdef _WIN32
#include <Window.hpp>
#include <Windowsx.h>
#include <TauEngine.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <EnumBitFields.hpp>

#ifndef MAX_WINDOW_COUNT
 #define MAX_WINDOW_COUNT 16
#endif

static const char*    CLASS_NAME   =  "TauEngineWindowClass";
static const wchar_t* CLASS_NAME_W = L"TauEngineWindowClass";

/**
 *   When we get a message for a window we only receive the
 * handle. Thus we need to store the reference to the actual
 * window somewhere. That somewhere is in this array.
 */
static Window* windowHandles[MAX_WINDOW_COUNT];
static u32 currentWindowHandleIndex = 0;

static bool addWindow(Window* systemWindowContainer)
{
    if(currentWindowHandleIndex < MAX_WINDOW_COUNT)
    {
        windowHandles[currentWindowHandleIndex++] = systemWindowContainer;
        return true;
    }
    return false;
}

static void removeWindow(Window* systemWindowContainer)
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

static Window* getWindowFromHandle(HWND handle)
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

static MouseFlags mouseFlagsFromWParam(WPARAM wParam)
{
#if TRUST_RAW_MOUSE_PARAM
    return static_cast<MouseFlags>(wParam);
#else
    auto out = static_cast<MouseFlags>(0);

    if(wParam & MK_LBUTTON)
    {
        out |= MouseFlags::MF_LEFT_BUTTON_DOWN;
    }
    else if(wParam & MK_RBUTTON)
    {
        out |= MouseFlags::MF_RIGHT_BUTTON_DOWN;
    }
    else if(wParam & MK_SHIFT)
    {
        out |= MouseFlags::MF_SHIFT_KEY_DOWN;
    }
    else if(wParam & MK_CONTROL)
    {
        out |= MouseFlags::MF_CTRL_KEY_DOWN;
    }
    else if(wParam & MK_MBUTTON)
    {
        out |= MouseFlags::MF_MIDDLE_BUTTON_DOWN;
    }

    const UINT xButton = GET_XBUTTON_WPARAM(wParam);
    if(xButton == XBUTTON1)
    {
        out |= MouseFlags::MF_X_BUTTON_1_DOWN;
    }
    else if(xButton == XBUTTON2)
    {
        out |= MouseFlags::MF_X_BUTTON_2_DOWN;
    }

    return out;
#endif
}

static MouseEvent mouseEventFromMsg(UINT uMsg, WPARAM wParam)
{
    const UINT xButton = GET_XBUTTON_WPARAM(wParam);
    switch(uMsg)
    {
        case WM_LBUTTONDOWN: return ME_LEFT_BUTTON_DOWN;
        case WM_LBUTTONUP:   return ME_LEFT_BUTTON_UP;
        case WM_MBUTTONDOWN: return ME_MIDDLE_BUTTON_DOWN;
        case WM_MBUTTONUP:   return ME_MIDDLE_BUTTON_UP;
        case WM_RBUTTONDOWN: return ME_RIGHT_BUTTON_DOWN;
        case WM_RBUTTONUP:   return ME_RIGHT_BUTTON_UP;
        case WM_XBUTTONDOWN:
            if(xButton == XBUTTON1) { return ME_X_BUTTON_1_DOWN; }
            if(xButton == XBUTTON2) { return ME_X_BUTTON_2_DOWN; }
            break;
        case WM_XBUTTONUP:
            if(xButton == XBUTTON1) { return ME_X_BUTTON_1_UP; }
            if(xButton == XBUTTON2) { return ME_X_BUTTON_2_UP; }
            break;
        default: break;
    }
    return static_cast<MouseEvent>(0xFF);
}

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = getWindowFromHandle(windowHandle);
    const u32 xPos = static_cast<u32>(GET_X_LPARAM(lParam));
    const u32 yPos = static_cast<u32>(GET_Y_LPARAM(lParam));

    switch(uMsg)
    {
        case WM_ENTERSIZEMOVE:
            if(window)
            {
                window->_isResizing = true;
            }
            break;
        case WM_EXITSIZEMOVE:
            if(window)
            {
                window->_isResizing = false;
                CALL_WINDOW_RESIZE_HANDLER:
                if(window->_windowResizeHandler)
                {
                    const u32 width = LOWORD(lParam);
                    const u32 height = HIWORD(lParam);
                    window->_windowResizeHandler(width, height, window);
                }
            }
            break;
        case WM_SIZE:
            if(window)
            {
                if(wParam == SIZE_MINIMIZED)
                {
                    window->_windowState = MINIMIZED;
                } 
                else if(wParam == SIZE_MAXIMIZED)
                {
                    window->_windowState = MAXIMIZED;
                    goto CALL_WINDOW_RESIZE_HANDLER;
                }
                else if(wParam == SIZE_RESTORED)
                {
                    if(window->_windowState == MINIMIZED || window->_windowState == MAXIMIZED)
                    {
                        window->_windowState = NEITHER;
                        goto CALL_WINDOW_RESIZE_HANDLER;
                    }
                    goto CALL_WINDOW_RESIZE_HANDLER;
                }
            }
            break;
        case WM_CLOSE:
            if(window)
            {
                window->closeWindow();
            }
            break;
        case WM_DESTROY:
            if(window)
            {
                window->closeWindow();
            }
            if(currentWindowHandleIndex == 0)
            {
                tauExit(0);
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            if(window)
            {
                window->_mouseEventHandler(mouseEventFromMsg(uMsg, wParam), mouseFlagsFromWParam(wParam), xPos, yPos, window);
                return 1;
            }
        default: return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(u32 width, u32 height, const char* title, const void* userContainer, const Window* parent) noexcept
    : _width(width), 
      _height(height), 
      _title(title),
      _windowContainer({ { }, null, null, null }),
      _userContainer(userContainer),
      _parent(parent),
      _windowState(NEITHER),
      _isResizing(false),
      _windowResizeHandler(null),
      _mouseEventHandler(null)
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
}

void Window::setTitle(const char* title) noexcept
{
    this->_title = title;
    SetWindowTextA(_windowContainer.windowHandle, title);
}

bool Window::createWindow() noexcept
{
    addWindow(this);

    WNDCLASS* windowClass = &this->_windowContainer.windowClass;

    windowClass->lpfnWndProc = WindowProc;
    windowClass->hInstance = GetModuleHandleA(null);
    windowClass->lpszClassName = CLASS_NAME;
    windowClass->style = CS_DBLCLKS;

    RegisterClass(windowClass);

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
                                                          this->_width, this->_height,
                                                          parent,
                                                          null,
                                                          windowClass->hInstance,
                                                          (LPVOID) null);

    this->_windowContainer.hdc = GetDC(this->_windowContainer.windowHandle);

    return this->_windowContainer.windowHandle;
}

void Window::closeWindow() noexcept
{
    removeWindow(this);
    if(this->_windowContainer.windowHandle)
    {
        DestroyWindow(_windowContainer.windowHandle);
    }
}

void Window::showWindow() const noexcept
{
    ShowWindow(this->_windowContainer.windowHandle, SW_SHOWNA);
    UpdateWindow(this->_windowContainer.windowHandle);
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
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pixelFormat = ChoosePixelFormat(this->_windowContainer.hdc, &pfd);

    if(pixelFormat == 0)
    {
        return false;
    }

    const BOOL res = SetPixelFormat(this->_windowContainer.hdc, pixelFormat, &pfd);

    if(!res) 
    {
        return false;
    }

    const HGLRC tempContext = wglCreateContext(this->_windowContainer.hdc);  // NOLINT(misc-misplaced-const)
    wglMakeCurrent(this->_windowContainer.hdc, tempContext);

    const GLenum glewSuccess = glewInit();
    if(glewSuccess != GLEW_OK)
    {
        return false;
    }

    static const int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
        this->_windowContainer.renderingContext = wglCreateContextAttribsARB(this->_windowContainer.hdc, null, attribs);
        wglMakeCurrent(null, null);
        wglDeleteContext(tempContext);
        wglMakeCurrent(this->_windowContainer.hdc, this->_windowContainer.renderingContext);
    }
    else
    {
        this->_windowContainer.renderingContext = tempContext;
    }

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
#endif
