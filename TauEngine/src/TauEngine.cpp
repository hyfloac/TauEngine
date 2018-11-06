#include <TauEngine.hpp>
#include <Utils.hpp>
#include <GL/glew.h>
#include <NumTypes.hpp>

bool tauInit() noexcept
{
    static bool _initializationComplete = false;

    if(!_initializationComplete)
    {
        _initializationComplete = true;
        // const GLenum success = glewInit();
        // if(success != GLEW_OK)
        // {
        //     return false;
        // }
    }

    return true;
}

#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 8
#endif

#ifdef _WIN32
#include <Windows.h>

void tauRunMessageLoop() noexcept
{
    MSG msg;
    u32 cnt = 0;
    while(cnt++ < NUM_MESSAGES_TO_READ && PeekMessageA(&msg, null, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

volatile static bool shouldExit = false;

bool tauShouldExit() noexcept
{
    return shouldExit;
}

void tauExit(int code) noexcept
{
    shouldExit = true;
    PostQuitMessage(code);
}
#else
void tauRunMessageLoop() noexcept
{
}
#endif
