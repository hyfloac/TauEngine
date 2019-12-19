#pragma once

#include <NumTypes.hpp>
#include "DLL.hpp"
#include "Timings.hpp"

class Exception;

class TAU_DLL Application
{
private:
    u32 _targetUPS;
protected:
    Application(u32 targetUPS) noexcept;
public:
    virtual ~Application() noexcept;

    Application(const Application& copy) noexcept = delete;
    Application(Application&& move) noexcept = delete;

    Application& operator =(const Application& copy) noexcept = delete;
    Application& operator =(Application&& move) noexcept = delete;

    virtual bool init(int argCount, char* args[]) noexcept = 0;

    virtual void finalize() noexcept = 0;

    virtual void onException(Exception& ex) noexcept { }
protected:
    virtual void update(float fixedDelta) noexcept = 0;

    virtual void render(const DeltaTime& delta) noexcept = 0;

    virtual void renderFPS(u32 ups, u32 fps) noexcept = 0;

    virtual void runMessageLoop() noexcept = 0;
public:
    void startGameLoop() noexcept;
};

#if defined(APP_MAIN)
Application* startGame() noexcept;

void tauMain() noexcept;
i32 tauExitCode() noexcept;

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

    return tauExitCode();
}
#endif
