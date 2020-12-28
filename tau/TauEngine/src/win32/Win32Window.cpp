/**
 * @file
 * 
 * The Win32 implementation of @link Window @endlink.
 */
#ifdef _WIN32
#pragma warning(push, 0)
#include <windowsx.h>
#pragma warning(pop)

#include "system/Keyboard.hpp"
#include "TauEngine.hpp"
#include "system/Window.hpp"
#include "events/WindowEvent.hpp"
#include "Timings.hpp"

#include <TUMaths.hpp>
#include <EnumBitFields.hpp>

/**
 * A basic AVL tree.
 *
 *   This is used to help speed up the search time of windows
 * from the WindowProc. Given that this is one of the most
 * called functions in the engine this is quite important to
 * have fast. That being said, the linear search wasn't
 * particularly slow, especially for a single window, but this
 * will have a faster lookup for multiple windows. Insertion
 * may be slow, but you're probably not inserting windows
 * every frame.
 */
struct WindowNode final
{
    DEFAULT_CONSTRUCT_PU(WindowNode);
    DEFAULT_DESTRUCT(WindowNode);
    DEFAULT_CM_PU(WindowNode);
public:
    static i32 height(const WindowNode* const node) noexcept
    {
        if(!node) { return 0; }
        return maxT(height(node->left), height(node->right)) + 1;
    }

    static WindowNode* rotateRight(WindowNode* const y) noexcept
    {
        WindowNode* x = y->left;
        WindowNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    static WindowNode* rotateLeft(WindowNode* const x) noexcept
    {
        WindowNode* y = x->left;
        WindowNode* T2 = y->right;

        y->left = x;
        x->right = T2;

        return y;
    }

    static WindowNode* insert(WindowNode* const root, WindowNode* const node) noexcept
    {
        if(node->key() < root->key())
        { root->left = insert(root->left, node); }
        else if(node->key() > root->key())
        { root->right = insert(root->right, node); }
        else
        { return root; }

        const i32 balance = root->getBalance();

        if(balance > 1 && node->key() < root->left->key())
        { return rotateRight(root); }

        if(balance < -1 && node->key() > root->right->key())
        { return rotateLeft(root); }

        if(balance > 1 && node->key() > root->left->key())
        {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if(balance < -1 && node->key() < root->right->key())
        {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    static WindowNode* remove(WindowNode* root, HWND nodeKey) noexcept
    {
        if(root->key() == nodeKey)
        {
            root->window = nullptr;
            return nullptr;
        }

        if(nodeKey < root->key())
        { root->left = remove(root->left, nodeKey); }
        else if(nodeKey > root->key())
        { root->right = remove(root->right, nodeKey); }
        else
        {
            if(!root->left || !root->right)
            {
                WindowNode* tmp = root->left ? root->left : root->right;
                if(tmp)
                {
                    *root = *tmp;
                }
                else
                {
                    tmp = root;
                    root = nullptr;
                }

                tmp->window = nullptr;
            }
            else
            {
                WindowNode* tmp = root->right->minValueNode();

                root->window = tmp->window;
                root->right = remove(root->right, tmp->key());
            }
        }

        if(!root)
        { return root; }

        const i32 balance = root->getBalance();

        if(balance > 1 && root->left->getBalance() >= 0)
        { return rotateRight(root); }

        if(balance < -1 && root->right->getBalance() <= 0)
        { return rotateLeft(root); }

        if(balance > 1 && root->left->getBalance() < 0)
        {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if(balance < -1 && root->right->getBalance() > 0)
        {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }
public:
    Window* window;
    WindowNode* left;
    WindowNode* right;
public:
    WindowNode(Window* window) noexcept
        : window(window), left(null), right(null)
    { }

    [[nodiscard]] HWND key() const noexcept { return window->_windowContainer.windowHandle; }

    [[nodiscard]] i32 getBalance() const noexcept { return height(left) - height(right); }

    [[nodiscard]] WindowNode* minValueNode() noexcept
    {
        WindowNode* current = this;

        while(current->left)
        { current = current->left; }

        return current;
    }

    [[nodiscard]] WindowNode* find(HWND findKey) noexcept
    {
        if(key() == findKey)
        {
            return this;
        }

        if(key() > findKey)
        {
            if(right)
            {
                return right->find(findKey);
            }
            return nullptr;
        }
        else
        {
            if(left)
            {
                return left->find(findKey);
            }
            return nullptr;
        }
    }
};

#ifndef MAX_WINDOW_COUNT
/**
 * The maximum number of windows that are being stored.
 * 
 *   This is used with @link windowHandles @endlink to be able to 
 * retrieve a @link Window @endlink from an @link HWND @endlink.
 * 
 *   This can be overriden by defining `MAX_WINDOW_COUNT`
 * in the pre processor before building the library.
 */
 #define MAX_WINDOW_COUNT 16
#endif

/**
 *   When we get a message for a window we only receive the
 * handle, thus we need to store the reference to the actual
 * window somewhere. That somewhere is in this tree.
 */
static WindowNode windowHandles[MAX_WINDOW_COUNT];

static WindowNode* root = nullptr;

static uSys windowCount() noexcept
{
    uSys ret = 0;

    for(auto& windowHandle : windowHandles)
    {
        if(windowHandle.window)
        {
            ++ret;
        }
    }

    return ret;
}

/**
 * Subtracts a window to @link windowHandles @endlink.
 * 
 * @param[in] systemWindowContainer
 *    The @link Window @endlink to put into @link windowHandles @endlink.
 * @return 
 *      Returns true if the `systemWindowContainer` was 
 *    successfully added to @link windowHandles @endlink.
 */
static bool addWindow(Window& systemWindowContainer) noexcept
{
    if(windowCount() == 0)
    {
        windowHandles[0].window = &systemWindowContainer;
        windowHandles[0].left = nullptr;
        windowHandles[0].right = nullptr;

        root = &windowHandles[0];

        return true;
    }

    for(auto& windowHandle : windowHandles)
    {
        if(!windowHandle.window)
        {
            windowHandle.window = &systemWindowContainer;
            windowHandle.left = nullptr;
            windowHandle.right = nullptr;

            root = WindowNode::insert(root, &windowHandle);

            return true;
        }
    }

    return false;
}

/**
 *   Removes a window from {@link windowHandles @endlink}. After the 
 * window is removed, all other pointers are shifted down if
 * necessary.
 * 
 * @param[in] systemWindowContainer
 *    The @link Window @endlink to remove from @link windowHandles @endlink.
 */
static void removeWindow(NotNull<const Window> systemWindowContainer) noexcept
{
    if(root)
    {
        root = WindowNode::remove(root, systemWindowContainer->_windowContainer.windowHandle);
    }
}

/**
 * Retrieves a @link Window @endlink from @link windowHandles @endlink.
 * 
 * @param[in] handle
 *    The handle of the @link Window @endlink to retrieve.
 * @return 
 *      A pointer to the @link Window @endlink containing the 
 *    @link HWND @endlink `handle`. returns null if no window is 
 *    currently holding the referenced handle.
 */
static Nullable Window* getWindowFromHandle(HWND handle) noexcept
{
    if(root)
    {
        WindowNode* const node = root->find(handle);
        if(node)
        {
            return node->window;
        }
    }

    return nullptr;
}

/**
 *   Returns a @link Mouse::Flags @endlink bit field represented by the 
 * @link WPARAM @endlink.
 * 
 *   If `TRUST_RAW_MOUSE_PARAM` is defined as `true` then this 
 * is just a simple cast, otherwise we manually go through 
 * each flag and binary OR in the equivalent 
 * @link Mouse::Flags @endlink.
 * 
 * @param[in] wParam
 *      The @link WPARAM @endlink representing a bit mask of flags 
 *    for use with mouse events.
 * @return
 *    An equivalent @link Mouse::Flags @endlink enum bit mask.
 */
static Mouse::Flags mouseFlagsFromWParam(const WPARAM wParam) noexcept
{
#ifdef TRUST_RAW_MOUSE_PARAM
    return static_cast<MouseFlags>(wParam);
#else
    Mouse::Flags out = static_cast<Mouse::Flags>(0);

    if(wParam & MK_LBUTTON)
    {
        out |= Mouse::Flags::LeftButtonDown;
    }
    if(wParam & MK_RBUTTON)
    {
        out |= Mouse::Flags::RightButtonDown;
    }
    if(wParam & MK_SHIFT)
    {
        out |= Mouse::Flags::ShiftKeyDown;
    }
    if(wParam & MK_CONTROL)
    {
        out |= Mouse::Flags::CtrlKeyDown;
    }
    if(wParam & MK_MBUTTON)
    {
        out |= Mouse::Flags::MiddleButtonDown;
    }

    const UINT xButton = GET_XBUTTON_WPARAM(wParam);
    if(xButton == XBUTTON1)
    {
        out |= Mouse::Flags::XButton1Down;
    }                              
    if(xButton == XBUTTON2)        
    {                              
        out |= Mouse::Flags::XButton2Down;
    }

    return out;
#endif
}

/**
 *   Returns a {@link Mouse::Event @endlink} based on the message type. 
 * `wParam` is also needed for identifying which X button was
 * pressed.
 * 
 * @param[in] uMsg
 *      The message type, this is the primary component in 
 *    creating the {@link Mouse::Event @endlink}.
 * @param[in] wParam
 *    Used to determine which X button may have been pressed.
 * @return
 *    A {@link Mouse::Event @endlink} representing which event was fired.
 */
static Mouse::Event mouseEventFromMsg(const UINT uMsg, const WPARAM wParam) noexcept
{
    const UINT xButton = GET_XBUTTON_WPARAM(wParam);
    switch(uMsg)
    {
        case WM_LBUTTONDOWN: return Mouse::Event::LeftButtonDown;
        case WM_LBUTTONUP:   return Mouse::Event::LeftButtonUp;
        case WM_RBUTTONDOWN: return Mouse::Event::RightButtonDown;
        case WM_RBUTTONUP:   return Mouse::Event::RightButtonUp;
        case WM_MBUTTONDOWN: return Mouse::Event::MiddleButtonDown;
        case WM_MBUTTONUP:   return Mouse::Event::MiddleButtonUp;
        case WM_XBUTTONDOWN:
            if(xButton == XBUTTON1) { return Mouse::Event::XButton1Down; }
            if(xButton == XBUTTON2) { return Mouse::Event::XButton2Down; }
            break;
        case WM_XBUTTONUP:
            if(xButton == XBUTTON1) { return Mouse::Event::XButton1Up; }
            if(xButton == XBUTTON2) { return Mouse::Event::XButton2Up; }
            break;
        default: break;
    }
    return static_cast<Mouse::Event>(0xFF);
}

static void callWindowResizeHandler(Window& window, const LPARAM) noexcept
{
    if(!window._eventHandler) { return; }

    RECT clientArea;
    GetClientRect(window._windowContainer.windowHandle, &clientArea);
    const u32 cWidth = clientArea.right;
    const u32 cHeight = clientArea.bottom;

    WindowResizeEvent evt(window, window._cWidth, window._cHeight, cWidth, cHeight);
    window._eventHandler(window._userContainer, evt);

    RECT windowArea;
    GetWindowRect(window._windowContainer.windowHandle, &windowArea);
    window._width = windowArea.right;
    window._height = windowArea.bottom;

    window._cWidth  = cWidth;
    window._cHeight = cHeight;
}

LRESULT CALLBACK WindowProc(HWND windowHandle, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) noexcept
{
    Window* window = getWindowFromHandle(windowHandle);

    switch(uMsg)
    {
        case WM_CREATE:
            window = reinterpret_cast<Window*>(reinterpret_cast<CREATESTRUCTA*>(lParam)->lpCreateParams);
            UNUSED(window);
            break;
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
            // if(window) { window->closeWindow(); }
            if(!root) { tauExit(); }
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            if(window && window->_eventHandler)
            {
                WindowMouseClickEvent evt(*window, mouseFlagsFromWParam(wParam), static_cast<u32>(GET_X_LPARAM(lParam)), static_cast<u32>(GET_Y_LPARAM(lParam)), mouseEventFromMsg(uMsg, wParam));
                window->_eventHandler(window->_userContainer, evt);
            }
            break;
        case WM_MOUSEMOVE:
            if(window && window->_eventHandler)
            {
                WindowMouseMoveEvent evt(*window, mouseFlagsFromWParam(wParam), static_cast<u32>(GET_X_LPARAM(lParam)), static_cast<u32>(GET_Y_LPARAM(lParam)));
                window->_eventHandler(window->_userContainer, evt);
            }
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if(window && window->_eventHandler)
            {
                if(lParam & (1 << 30))
                {
                    WindowKeyEvent evt(*window, Keyboard::Event::Held, Keyboard::keyboardFlags(), Keyboard::convertFromSysVKey(static_cast<int>(wParam)));
                    window->_eventHandler(window->_userContainer, evt);
                }
                else
                {
                    WindowKeyEvent evt(*window, Keyboard::Event::Pressed, Keyboard::keyboardFlags(), Keyboard::convertFromSysVKey(static_cast<int>(wParam)));
                    window->_eventHandler(window->_userContainer, evt);
                }
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if(window && window->_eventHandler)
            {
                WindowKeyEvent evt(*window, Keyboard::Event::Released, Keyboard::keyboardFlags(), Keyboard::convertFromSysVKey(static_cast<int>(wParam)));
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
                const u32 xPos = static_cast<u32>(GET_X_LPARAM(lParam));
                const u32 yPos = static_cast<u32>(GET_Y_LPARAM(lParam));
                if(window->_eventHandler)
                {
                    WindowMoveEvent evt(*window, window->_xPos, window->_yPos, xPos, yPos);
                    window->_eventHandler(window->_userContainer, evt);
                }
                window->_xPos = xPos;
                window->_yPos = yPos;
            }
            break;
        default: return DefWindowProcA(windowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

Window::Window(const u32 width, const u32 height, const WDynString& title, void* userContainer, const Window* parent) noexcept
    : _width(width)
    , _height(height)
    , _cWidth(0)
    , _cHeight(0)
    , _xPos(0)
    , _yPos(0)
    , _title(title)
    , _windowContainer({ { }, nullptr, nullptr })
    , _userContainer(userContainer)
    , _parent(parent)
    , _renderingMode(RenderingMode::getGlobalMode())
    , _windowState(WindowState::NEITHER)
    , _eventHandler(null)
{ }

Window::Window(const u32 width, const u32 height, WDynString&& title, void* userContainer, const Window* parent) noexcept
    : _width(width)
    , _height(height)
    , _cWidth(0)
    , _cHeight(0)
    , _xPos(0)
    , _yPos(0)
    , _title(::std::move(title))
    , _windowContainer({ { }, nullptr, nullptr })
    , _userContainer(userContainer)
    , _parent(parent)
    , _renderingMode(RenderingMode::getGlobalMode())
    , _windowState(WindowState::NEITHER)
    , _eventHandler(null)
{ }

Window::~Window() noexcept
{ closeWindow(); }

void Window::resize(const u32 width, const u32 height) noexcept
{
    PERF();
    _width = width;
    _height = height;
    SetWindowPos(_windowContainer.windowHandle, nullptr, 0, 0, static_cast<int>(width), static_cast<int>(height), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void Window::move(const u32 xPos, const u32 yPos) noexcept
{
    PERF();
    _xPos = xPos;
    _yPos = yPos;
    SetWindowPos(_windowContainer.windowHandle, nullptr, static_cast<int>(xPos), static_cast<int>(yPos), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void Window::moveAndResize(const u32 xPos, const u32 yPos, const u32 width, const u32 height) noexcept
{
    PERF();
    _xPos = xPos;
    _yPos = yPos;
    _width = width;
    _height = height;
    SetWindowPos(_windowContainer.windowHandle, nullptr, xPos, yPos, width, height, SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void Window::setTitle(const WDynString& title) noexcept
{
    PERF();
    _title = title;
    SetWindowTextW(_windowContainer.windowHandle, title);
}

bool Window::createWindow() noexcept
{
    PERF();
    addWindow(*this);

    WNDCLASSEXW* windowClass = &_windowContainer.windowClass;

    windowClass->cbSize = sizeof(WNDCLASSEXW);
    windowClass->style = CS_DBLCLKS;
    windowClass->lpfnWndProc = WindowProc;
    windowClass->cbClsExtra = 0;
    windowClass->cbWndExtra = 0;
    windowClass->hInstance = GetModuleHandleW(nullptr);
    windowClass->hIcon = nullptr;
    windowClass->hCursor = nullptr;
    windowClass->hbrBackground = nullptr;
    windowClass->lpszMenuName = nullptr;
    windowClass->lpszClassName = L"TauWndCls";
    windowClass->hIconSm = nullptr;
    
    RegisterClassExW(windowClass);

    HWND parent = nullptr;

    if(_parent)
    {
        parent = _parent->_windowContainer.windowHandle;
    }

    _windowContainer.windowHandle = CreateWindowExW(
        0, 
        windowClass->lpszClassName, 
        _title, 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT,
        static_cast<i32>(_width), 
        static_cast<i32>(_height),
        parent, 
        nullptr, 
        windowClass->hInstance, 
        this);

    _windowContainer.hdc = GetDC(_windowContainer.windowHandle);

    RECT clientArea;
    GetClientRect(_windowContainer.windowHandle, &clientArea);
    _cWidth = clientArea.right;
    _cHeight = clientArea.bottom;

    return _windowContainer.windowHandle;
}

void Window::closeWindow() const noexcept
{
    PERF();
    removeWindow(this);
    if(_windowContainer.windowHandle)
    {
        DestroyWindow(_windowContainer.windowHandle);
    }
}

void Window::showWindow() const noexcept
{
    PERF();
    ShowWindow(_windowContainer.windowHandle, SW_SHOWNA);
    UpdateWindow(_windowContainer.windowHandle);

    SetActiveWindow(_windowContainer.windowHandle);
}

void Window::hideWindow() const noexcept
{
    PERF();
    ShowWindow(_windowContainer.windowHandle, SW_HIDE);
}

void Window::setBorderlessFullscreen() noexcept
{
    HMONITOR currentMonitor = MonitorFromWindow(_windowContainer.windowHandle, MONITOR_DEFAULTTONEAREST);
    
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);

    GetMonitorInfoW(currentMonitor, &monitorInfo);

    const RECT screen = monitorInfo.rcMonitor;

    SetWindowLongW(_windowContainer.windowHandle, GWL_STYLE, WS_POPUP);
    SetWindowPos(_windowContainer.windowHandle, nullptr, screen.left, screen.top, screen.right, screen.bottom, SWP_SHOWWINDOW);

    _xPos = screen.left;
    _yPos = screen.top;
    _width = screen.right;
    _height = screen.bottom;
}

void Window::setBorderlessWorkspace() noexcept
{
    HMONITOR currentMonitor = MonitorFromWindow(_windowContainer.windowHandle, MONITOR_DEFAULTTONEAREST);

    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);

    GetMonitorInfoW(currentMonitor, &monitorInfo);

    const RECT screen = monitorInfo.rcWork;

    SetWindowLongW(_windowContainer.windowHandle, GWL_STYLE, WS_POPUP);
    SetWindowPos(_windowContainer.windowHandle, nullptr, screen.left, screen.top, screen.right, screen.bottom, SWP_SHOWWINDOW);

    _xPos = screen.left;
    _yPos = screen.top;
    _width = screen.right;
    _height = screen.bottom;
}

void Window::setWindowed(const u32 width, const u32 height) noexcept
{
    SetWindowLongW(_windowContainer.windowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    SetWindowPos(_windowContainer.windowHandle, nullptr, CW_USEDEFAULT, CW_USEDEFAULT, width, height, SWP_SHOWWINDOW);
}

static BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam);

void Window::setAsDesktopBackground() noexcept
{
    if(_parent)
    { return; }

    HWND progMan = FindWindowW(L"Progman", nullptr);
    DWORD_PTR result;
    SendMessageTimeoutW(progMan, 0x052C, 0, 0, SMTO_NORMAL, 1000, &result);

    HWND workerW;
    EnumWindows(&enumWindowsProc, reinterpret_cast<LPARAM>(&workerW));

    SetParent(_windowContainer.windowHandle, workerW);
}

void Window::removeFromDesktopBackground() noexcept
{
    if(_parent)
    { return; }

    SetParent(_windowContainer.windowHandle, null);
}

static BOOL CALLBACK enumWindowsProc(HWND topHandle, const LPARAM lParam)
{
    HWND defView = FindWindowExW(topHandle, nullptr, L"SHELLDLL_DefView", nullptr);

    if(defView)
    {
        HWND* workerW = reinterpret_cast<HWND*>(lParam);
        *workerW = FindWindowExW(nullptr, topHandle, L"WorkerW", nullptr);
    }

    return true;
}
#endif
