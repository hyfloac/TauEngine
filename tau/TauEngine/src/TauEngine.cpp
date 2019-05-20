#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include <TauEngine.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <Timings.hpp>
#include <system/Window.hpp>

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

void setEngineLoggerLevel(spdlog::level::level_enum level) noexcept
{
    engineLogger->set_level(level);
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
  #define NUM_MESSAGES_TO_READ 24
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

void tauExit(i32 code) noexcept
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

void tauGameLoop(u32 targetUPS, update_f updateF, render_f renderF, u8 printFPSInterval, Window* window) noexcept
{
    const float MS_PER_UPDATE = 1000.0f / targetUPS;
    u64 lastTime = currentTimeMillis();
    float lag = 0.0f;

    u64 counterTime = lastTime;
    u32 fps = 0;
    u32 ups = 0;

    const char* originalTitle = "";

    if(!window)
    {
        printFPSInterval = 0;
    }
    else
    {
        originalTitle = window->title();
    }

    // " - FPS/UPS: 10000/10000"
    const u32 titleLen = strlen(originalTitle) + 24;
    char* newTitle = new char[titleLen];
    newTitle[titleLen - 1] = '\0';

    const u64 _timeInterval = printFPSInterval * 1000;

    while(!should_exit)
    {
        const u64 currentTime = currentTimeMillis();
        const u64 elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += static_cast<float>(elapsed);

        while(lag >= MS_PER_UPDATE)
        {
            runMessageLoop();

            updateF(MS_PER_UPDATE); 
            ++ups;
            lag -= MS_PER_UPDATE;
        }

        renderF(lag / MS_PER_UPDATE); 
        ++fps;

        const u64 currentCounterTime = currentTimeMillis();

        if(printFPSInterval && currentCounterTime - counterTime >= _timeInterval)
        {
            counterTime = currentCounterTime;
            // engineLogger->debug("UPS {}", ups / printFPSInterval);
            // engineLogger->debug("FPS {}", fps / printFPSInterval);

            snprintf(newTitle, titleLen, "%s - UPS/FPS: %u/%u", originalTitle, ups / printFPSInterval, fps / printFPSInterval);

            window->setTitle(newTitle);

            ups = 0;
            fps = 0;
        }
    }

    delete[] newTitle;
}
