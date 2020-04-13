#pragma once

#include <NumTypes.hpp>
#include "DLL.hpp"
#include "Timings.hpp"
#include "TauEngine.hpp"

struct ExceptionData;

class TAU_DLL Application
{
private:
    u32 _targetUPS;
protected:
    Application(u32 targetUPS) noexcept
        : _targetUPS(targetUPS)
    { }
public:
    virtual ~Application() noexcept = default;

    Application(const Application& copy) noexcept = delete;
    Application(Application&& move) noexcept = delete;

    Application& operator =(const Application& copy) noexcept = delete;
    Application& operator =(Application&& move) noexcept = delete;

    virtual bool init(int argCount, char* args[]) noexcept = 0;

    virtual void finalize() noexcept = 0;

    virtual void onException(ExceptionData& ex) noexcept { }
protected:
    virtual void update(float fixedDelta) noexcept { }

    virtual void render(const DeltaTime& delta) noexcept { }

    virtual void renderFPS(u32 ups, u32 fps) noexcept { }

    virtual void runMessageLoop() noexcept { }
public:
    void startGameLoop() noexcept
    {
        const float Mu_PER_UPDATE = 1000000.0f / _targetUPS;
        u64 lastTime = microTime();
        float lag = 0.0f;

        u64 counterTime = lastTime;
        u32 fps = 0;
        u32 ups = 0;

        DeltaTime deltaTime;

        while(!tauShouldExit())
        {
            const u64 currentTime = microTime();
            const u64 elapsed = currentTime - lastTime;
            lastTime = currentTime;
            lag += static_cast<float>(elapsed);

            while(lag >= Mu_PER_UPDATE)
            {
                deltaTime.onUpdate();
                runMessageLoop();

                update(Mu_PER_UPDATE);
                ++ups;
                lag -= Mu_PER_UPDATE;
            }

            if(elapsed != 0)
            {
                deltaTime.setDeltaMicro(elapsed);
                render(deltaTime);
                ++fps;
            }

            if(currentTime - counterTime >= 1000000)
            {
                counterTime = currentTime;

                renderFPS(ups, fps);

                ups = 0;
                fps = 0;
            }
        }

        ExceptionData& ex = tauGetException();
        if(ex.ex)
        {
            onException(ex);
        }
    }
};

#ifdef APP_MAIN
Application* startGame() noexcept;

int main(int argCount, char* args[]) noexcept
{
#ifndef TAU_DLL_LINK
    tauMain();
#endif

    Application* app = startGame();

    if(!app) { return -2; }

    if(!app->init(argCount, args))
    {
        delete app;
        return -1;
    }
    app->startGameLoop();
    app->finalize();

    delete app;

#ifndef TAU_DLL_LINK
    tauFinalize();
#endif

    return tauExitCode();
}
#endif
