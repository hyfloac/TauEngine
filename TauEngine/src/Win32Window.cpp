#ifdef _WIN32
#include "Window.hpp"
#include "Utils.hpp"
#include <cstdio>

#define MAX_WINDOW_COUNT 16

static const char* CLASS_NAME = "TauEngineWindow";

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
        if(systemWindowContainer->__windowContainer()->windowHandle == windowHandles[i]->__windowContainer()->windowHandle)
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
        if(windowHandles[i]->__windowContainer()->windowHandle == handle)
        {
            return windowHandles[i];
        }
    }

    return null;
}

LRESULT CALLBACK WindowProc(CONST HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = getWindowFromHandle(hwnd);

    switch(uMsg)
    {
        case WM_SIZE:
            // u32 width = LOWORD(lParam);
            // u32 height = HIWORD(lParam);

            // window = getWindowFromHandle(hwnd);
            printf("[Window Proc] [WM_SIZE]: %d\n", wParam);
            char buffer[512];
            sprintf(buffer, "[Window Proc] [WM_SIZE]: %d\n", wParam);
            OutputDebugStringA(buffer);
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
        default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(u32 width, u32 height, const char* title) noexcept
    : _width(width), _height(height), _title(title), _windowContainer({ { }, null })
{
}

Window::~Window() noexcept
{
    closeWindow();
}

void Window::setSize(u32 width, u32 height) noexcept
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

void Window::createWindow() noexcept
{
    addWindow(this);

    this->_windowContainer.windowClass.lpfnWndProc = WindowProc;
    this->_windowContainer.windowClass.hInstance = GetModuleHandleA(NULL);
    this->_windowContainer.windowClass.lpszClassName = CLASS_NAME;

    RegisterClass(&this->_windowContainer.windowClass);

    this->_windowContainer.windowHandle = 
    CreateWindowExA(0,
                    this->_windowContainer.windowClass.lpszClassName,
                    this->_title,
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                    NULL,
                    NULL,
                    this->_windowContainer.windowClass.hInstance,
                    NULL);
}

void Window::closeWindow() noexcept
{
    if(_windowContainer.windowHandle)
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
