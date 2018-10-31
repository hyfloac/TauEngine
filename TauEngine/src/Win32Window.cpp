#ifdef _WIN32
#include <Window.hpp>
#include <Utils.hpp>
#include <cstdio>

#ifndef MAX_WINDOW_COUNT
 #define MAX_WINDOW_COUNT 16
#endif

static const char*    CLASS_NAME   =  "TauEngineWindowClass";
static const wchar_t* CLASS_NAME_W = L"TauEngineWindowClass";

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
        case WM_QUIT:
            if(window)
            {
                window->closeWindow();
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(windowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(u32 width, u32 height, const char* title) noexcept
    : _width(width), 
      _height(height), 
      _title(title), 
      _windowContainer({ { }, null }),
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

    this->_windowContainer.windowHandle = CreateWindowExA(0,
                                                          windowClass->lpszClassName,
                                                          this->_title,
                                                          WS_OVERLAPPEDWINDOW,
                                                          CW_USEDEFAULT, CW_USEDEFAULT, 
                                                          CW_USEDEFAULT, CW_USEDEFAULT,
                                                          null,
                                                          null,
                                                          windowClass->hInstance,
                                                          null);

    return this->_windowContainer.windowHandle;
}

void Window::closeWindow() noexcept
{
    if(this->_windowContainer.windowHandle)
    {
        removeWindow(this);
        DestroyWindow(_windowContainer.windowHandle);
    }
}

void Window::showWindow() const noexcept
{
    ShowWindow(this->_windowContainer.windowHandle, SW_SHOWNA);
    UpdateWindow(this->_windowContainer.windowHandle);
}
#endif
