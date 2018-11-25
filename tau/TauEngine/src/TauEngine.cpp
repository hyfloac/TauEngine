#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)
#include <TauEngine.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>

static std::shared_ptr<spdlog::logger> engineLogger;

bool tauInit() noexcept
{
    static bool _initializationComplete = false;

    if(!_initializationComplete)
    {
        _initializationComplete = true;
        spdlog::set_pattern("[%^%H:%M:%S:%e%$] [%^%n%$] [%^%l%$]: %v");
        engineLogger = spdlog::stdout_color_mt("TauEngine");
        engineLogger->set_level(spdlog::level::trace);
        engineLogger->set_pattern("[%^%H:%M:%S:%e%$] [%^%n%$] [%^%l%$]: %v");
    }

    return true;
}

std::shared_ptr<spdlog::logger> getEngineLogger() noexcept
{
    return engineLogger;
}


volatile static bool shouldExit = false;

bool tauShouldExit() noexcept
{
    return shouldExit;
}

#ifdef _WIN32
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 8
#endif

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
