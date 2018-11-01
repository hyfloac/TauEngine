#include <TauEngine.hpp>
#ifdef _WIN32
#include <Window.hpp>
#include <Utils.hpp>
#include <cstdio>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>

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

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = getWindowFromHandle(windowHandle);

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
        default: return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(u32 width, u32 height, const char* title, const Window* parent) noexcept
    : _width(width), 
      _height(height), 
      _title(title),
      _windowContainer({ { }, null, null, null }),
      _parent(parent),
      _windowState(NEITHER),
      _isResizing(false),
      _windowResizeHandler(null)
{ }

Window::~Window() noexcept
{
    this->closeWindow();
}

void Window::resize(const u32 width, const u32 height) noexcept
{
    this->_width = width;
    this->_height = height;
    SetWindowPos(_windowContainer.windowHandle, null, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
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
                                                          null);

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
