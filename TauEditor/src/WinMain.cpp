#ifdef _WIN32

#define NO_STDIO_REDIRECT
#include <Utils.hpp>
#include <Windows.h>
#include <Window.hpp>
#include <cstdio>

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int __old_Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    printf("Starting Tau Engine\n");
    OutputDebugStringA("Starting Tau Engine\n");

    Window window(800, 600, "Test Window");
    window.createWindow();
    window.showWindow();

    MSG msg;
    while(GetMessageA(&msg, null, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

#endif
