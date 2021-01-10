#include "TauEngine.hpp"
#include <NumTypes.hpp>
#include <Utils.hpp>

#include "allocator/PageAllocator.hpp"
#include "Timings.hpp"
#include "system/Window.hpp"
#include "maths/Maths.hpp"
#include "system/SystemInterface.hpp"

#include "dx/dx12/DX12GraphicsInterface.hpp"

bool tauInit() noexcept
{
    static bool _initializationComplete = false;

    if(!_initializationComplete)
    {
        _initializationComplete = true;
        PageAllocator::init();

        SystemInterface::registerGraphicsInterface(RenderingMode::DirectX12, new(::std::nothrow) DX12GraphicsInterfaceBuilder);
    }

    return true;
}

void initProgramStartTimes() noexcept;

void tauMain() noexcept
{
    tauInit();
    initSinTable();
    initProgramStartTimes();
}

void tauFinalize() noexcept
{
}

static ExceptionData exData = { null, 0, "", "" };

#if !defined(TAU_PRODUCTION)
void tauThrowException(Exception& e, const uSys line, const char* const file, const char* const func) noexcept
{
    exData.ex = &e;
    exData.line = line;
    exData.file = file;
    exData.func = func;
    tauExit(-1);
    DEBUG_BREAK;
}
#else
void tauThrowException(Exception& e) noexcept
{
    exData.ex = &e;
    tauExit(-1);
}
#endif

ExceptionData& tauGetException() noexcept
{ return exData; }

volatile static bool should_exit = false;
volatile static i32 exit_code = 0;

bool tauShouldExit() noexcept
{
    return should_exit;
}

void tauExit(const i32 code) noexcept
{
    should_exit = true;
    exit_code = code;
}

#ifdef _WIN32
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 128
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
#else
static void runMessageLoop() noexcept
{
}
#endif

i32 tauExitCode() noexcept
{
    return exit_code;
}

void tauGameLoop(const u32 targetUPS, const update_f updateF, const render_f renderF, const renderFPS_f renderFPS) noexcept
{
    const float Mu_PER_UPDATE = 1000000.0f / static_cast<float>(targetUPS);
    u64 lastTime = microTime();
    float lag = 0.0f;

    u64 counterTime = lastTime;
    u32 fps = 0;
    u32 ups = 0;

    while(!should_exit)
    {
        const u64 currentTime = microTime();
        const u64 elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += static_cast<float>(elapsed);

        while(lag >= Mu_PER_UPDATE)
        {
            runMessageLoop();

            updateF(Mu_PER_UPDATE);
            ++ups;
            lag -= Mu_PER_UPDATE;
        }

        if(elapsed != 0)
        {
            renderF(static_cast<float>(elapsed));
            ++fps;
        }

        if(currentTime - counterTime >= 1000000)
        {
            counterTime = currentTime;

            if(renderFPS) { renderFPS(ups, fps); }

            ups = 0;
            fps = 0;
        }
    }
}
