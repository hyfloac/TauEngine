#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)
#include <TauEngine.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <Timings.hpp>

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


void tauGameLoop(u32 targetUPS, update_f updateF, render_f renderF) noexcept
{
#ifndef PRINT_FPS
  #define PRINT_FPS 0 || _DEBUG
#endif

    const float MS_PER_UPDATE = 1000.0f / targetUPS;
    u64 lastTime = currentTimeMillis();
    float lag = 0.0f;

#if PRINT_FPS
    u64 counterTime = lastTime;
    u32 fps = 0;
    u32 ups = 0;
#endif

#if PRINT_FPS
#define PSC(__COUNTER, __FUNC, ...) __FUNC(__VA_ARGS__); ++(__COUNTER);
#else
#define PSC(__COUNTER, __FUNC, ...) __FUNC(__VA_ARGS__);
#endif


    while (!tauShouldExit())
    {
        const u64 currentTime = currentTimeMillis();
        const u64 elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += static_cast<float>(elapsed);

        tauRunMessageLoop();

        while (lag >= MS_PER_UPDATE)
        {
            PSC(ups, updateF);
            lag -= MS_PER_UPDATE;
        }

        PSC(fps, renderF, (lag / MS_PER_UPDATE));

#if PRINT_FPS
#ifndef PRINT_FPS_INTERVAL
  #define PRINT_FPS_INTERVAL 3
#endif
        constexpr u64 _timeInterval = PRINT_FPS_INTERVAL * 1000;

        const u64 currentCounterTime = currentTimeMillis();

        if ((currentCounterTime - counterTime) >= _timeInterval)
        {
            counterTime = currentCounterTime;
            engineLogger->debug("UPS {}", ups / PRINT_FPS_INTERVAL);
            engineLogger->debug("FPS {}", fps / PRINT_FPS_INTERVAL);

            ups = 0;
            fps = 0;
        }
#endif
    }
}
