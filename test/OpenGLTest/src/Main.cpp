#include <cstdio>
#include <Windows.h>
#include <NumTypes.hpp>
#include <GL/glew.h>

#include "GL/wglew.h"

static LRESULT CALLBACK WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

static void messageLoop() noexcept;

static bool shouldClose = false;

static int width = 1024;
static int height = 720;

static int cWidth = 0;
static int cHeight = 0;

int main(int argCount, char* args[])
{
    WNDCLASSA windowClass = { };
    memset(&windowClass, 0, sizeof(windowClass));

    windowClass.style = CS_DBLCLKS;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = GetModuleHandleA(NULL);
    windowClass.lpszClassName = "OpenGLGTestWindowClass";

    RegisterClassA(&windowClass);

    HWND window = CreateWindowExA(0, windowClass.lpszClassName, "OpenGL Test",
                                  WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                  width, height, NULL, NULL, windowClass.hInstance, NULL);

    if(!window)
    { return -1; }

    RECT clientArea;
    GetClientRect(window, &clientArea);
    cWidth = clientArea.right;
    cHeight = clientArea.bottom;

    HDC hdc = GetDC(window);

    PIXELFORMATDESCRIPTOR pfd{ };
    memset(&pfd, 0, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if(!pixelFormat) { return -2; }

    const BOOL res = SetPixelFormat(hdc, pixelFormat, &pfd);
    if(!res) { return -3; }

    HGLRC tmpContext = wglCreateContext(hdc);
    if(!tmpContext)
    { return -4; }

    wglMakeCurrent(hdc, tmpContext);

    {
        const GLenum glewError = glewInit();
        if(glewError != GLEW_OK)
        {
            fprintf(stderr, "Glew Error: %s\n", reinterpret_cast<const char*>(glewGetErrorString(glewError)));
            return -5;
        }
    }

    HGLRC attribContext;

    if(WGLEW_ARB_create_context)
    {
        const int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_FLAGS_ARB,        WGL_CONTEXT_DEBUG_BIT_ARB | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0
        };

        // ReSharper disable once CppLocalVariableMayBeConst
        attribContext = wglCreateContextAttribsARB(hdc, NULL, attribs);

        if(!attribContext)
        { return -6; }

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tmpContext);
    }
    else
    { return -7; }

    wglMakeCurrent(hdc, attribContext);

    {
        const GLenum glewError = glewInit();
        if(glewError != GLEW_OK)
        {
            fprintf(stderr, "Glew Error: %s\n", reinterpret_cast<const char*>(glewGetErrorString(glewError)));
            return -8;
        }
    }

    ShowWindow(window, SW_SHOWNA);
    UpdateWindow(window);
    SetActiveWindow(window);

    glClearDepth(1.0f);
    glClearStencil(0);
    glClearColor(0.5f, 0.7f, 0.1f, 1.0f);
    while(!shouldClose)
    {
        messageLoop();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SwapBuffers(hdc);
    }

    return 0;
}

static void messageLoop() noexcept
{
    MSG msg;
    u32 cnt = 0;
    while(cnt++ < 256 && PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch(uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            shouldClose = true;
            break;
        case WM_SIZE:
            width  = LOWORD(lParam);
            height = HIWORD(lParam);

            RECT clientArea;
            GetClientRect(hWnd, &clientArea);
            cWidth = clientArea.right;
            cHeight = clientArea.bottom;
            break;
        default: return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
