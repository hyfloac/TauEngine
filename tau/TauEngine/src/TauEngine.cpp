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

volatile static bool should_exit = false;

bool tauShouldExit() noexcept
{
    return should_exit;
}

#ifdef _WIN32
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 8
#endif

static void runMessageLoop() noexcept
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
    should_exit = true;
    PostQuitMessage(code);
}
#else
static void runMessageLoop() noexcept
{
}

void tauExit(int code) noexcept
{
    should_exit = true;
}
#endif

void tauGameLoop(u32 targetUPS, update_f updateF, render_f renderF, u8 printFPSInterval) noexcept
{
    const float MS_PER_UPDATE = 1000.0f / targetUPS;
    u64 lastTime = currentTimeMillis();
    float lag = 0.0f;

    u64 counterTime = lastTime;
    u32 fps = 0;
    u32 ups = 0;

    const u64 _timeInterval = printFPSInterval * 1000;

    while(!should_exit)
    {
        const u64 currentTime = currentTimeMillis();
        const u64 elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += static_cast<float>(elapsed);

        runMessageLoop();

        while(lag >= MS_PER_UPDATE)
        {
            updateF(); 
            ++ups;
            lag -= MS_PER_UPDATE;
        }

        renderF(lag / MS_PER_UPDATE); 
        ++fps;

        const u64 currentCounterTime = currentTimeMillis();

        if(printFPSInterval && currentCounterTime - counterTime >= _timeInterval)
        {
            counterTime = currentCounterTime;
            engineLogger->debug("UPS {}", ups / printFPSInterval);
            engineLogger->debug("FPS {}", fps / printFPSInterval);

            ups = 0;
            fps = 0;
        }
    }
}
