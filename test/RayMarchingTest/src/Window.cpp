#include "Window.hpp"

static Window* globalWindow;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(hWnd != globalWindow->_hWnd && uMsg != WM_CREATE)
    { return DefWindowProcW(hWnd, uMsg, wParam, lParam); }

    switch(uMsg)
    {
        case WM_CREATE:
        {
            CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Window* const window = reinterpret_cast<Window*>(create->lpCreateParams);

            window->_hDC = GetDC(hWnd);

            RECT windowArea;
            (void) GetWindowRect(hWnd, &windowArea);

            RECT clientArea = { 0, 0, globalWindow->_cWidth, globalWindow->_cHeight };
            (void) AdjustWindowRect(&clientArea, WS_OVERLAPPEDWINDOW, false);

            window->_x = windowArea.left;
            window->_y = windowArea.top;
            window->_width = clientArea.right - clientArea.left;
            window->_height = clientArea.bottom - clientArea.top;

            (void) SetWindowPos(hWnd, NULL, 0, 0, window->_width, window->_height, SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOOWNERZORDER | SWP_NOZORDER);
            break;
        }
        case WM_CLOSE:
        {
            globalWindow->_shouldClose = true;
            globalWindow->close();
            break;
        }
        case WM_WINDOWPOSCHANGED:
        {
            WINDOWPOS* wndPos = reinterpret_cast<WINDOWPOS*>(lParam);
            globalWindow->_x = wndPos->x;
            globalWindow->_y = wndPos->y;

            if(globalWindow->_width != wndPos->cx || globalWindow->_height != wndPos->cy)
            {
                globalWindow->_width = wndPos->cx;
                globalWindow->_height = wndPos->cy;

                RECT clientArea;
                GetClientRect(hWnd, &clientArea);

                globalWindow->_cWidth = clientArea.right;
                globalWindow->_cHeight = clientArea.bottom;

                if(globalWindow->_onResize)
                {
                    globalWindow->_onResize(clientArea.right, clientArea.bottom, globalWindow->_userParam);
                }
            }

            break;
        }
        default: return DefWindowProcW(hWnd, uMsg, wParam, lParam);       
    }

    return 0;
}

void Window::create() noexcept
{
    globalWindow = this;

    _windowClass.cbSize = sizeof(WNDCLASSEXW);
    _windowClass.style = CS_DBLCLKS;
    _windowClass.lpfnWndProc = WindowProc;
    _windowClass.cbClsExtra = 0;
    _windowClass.cbWndExtra = 0;
    _windowClass.hInstance = GetModuleHandleW(NULL);
    _windowClass.hIcon = NULL;
    _windowClass.hCursor = NULL;
    _windowClass.hbrBackground = NULL;
    _windowClass.lpszMenuName = NULL;
    _windowClass.lpszClassName = L"RayMarchWndCls";
    _windowClass.hIconSm = NULL;

    RegisterClassExW(&_windowClass);

    _hWnd = CreateWindowExW(
        0,
        _windowClass.lpszClassName,
        L"Ray Marching Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        _windowClass.hInstance,
        this);
}

void Window::close() noexcept
{
    if(_hWnd)
    {
        DestroyWindow(_hWnd);
    }
}

void Window::show() noexcept
{
    ShowWindow(_hWnd, SW_SHOWNA);
    UpdateWindow(_hWnd);

    SetActiveWindow(_hWnd);
}

void Window::hide() noexcept
{
    ShowWindow(_hWnd, SW_HIDE);
}

static BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam);

static HWND _workerW = NULL;

void Window::setAsDesktopBackground() noexcept
{
    HWND progMan = FindWindowW(L"Progman", NULL);
    DWORD_PTR result;
    SendMessageTimeoutW(progMan, 0x052C, 0, 0, SMTO_NORMAL, 1000, &result);
    
    EnumWindows(&enumWindowsProc, 0);
    
    SetParent(_hWnd, _workerW);

    RECT screen;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &screen, 0);

    SetWindowLongW(_hWnd, GWL_STYLE, WS_POPUP);
    SetWindowPos(_hWnd, NULL, 0, 0, screen.right, screen.bottom, SWP_SHOWWINDOW);
}

void Window::removeFromDesktopBackground() noexcept
{
    SetParent(_hWnd, NULL);
}

static BOOL CALLBACK enumWindowsProc(HWND topHandle, LPARAM lParam)
{
    HWND defView = FindWindowExW(topHandle, NULL, L"SHELLDLL_DefView", NULL);

    if(defView)
    {
        _workerW = FindWindowExW(NULL, topHandle, L"WorkerW", NULL);
    }

    return true;
}
