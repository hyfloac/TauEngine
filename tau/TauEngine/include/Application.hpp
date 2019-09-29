#pragma once

#include <NumTypes.hpp>
#include <DLL.hpp>

class TAU_DLL Application
{
private:
    static Application* _instance;
private:
    u32 _targetUPS;
protected:
    Application(const u32 targetUPS) noexcept;
public:
    Application(const Application& copy) noexcept = delete;
    Application(Application&& move) noexcept = delete;

    virtual ~Application() noexcept;

    Application& operator =(const Application& copy) noexcept = delete;
    Application& operator =(Application&& move) noexcept = delete;

    virtual void init(int argCount, char* args[]) noexcept = 0;

    virtual void update(const float fixedDelta) noexcept = 0;

    virtual void render(const float delta) noexcept = 0;

    virtual void renderFPS(const u32 ups, const u32 fps) noexcept = 0;

    void startGameLoop() const noexcept;
};

#ifndef APP_IMPL
Application* startGame() noexcept;

int main(int argCount, char* args[]) noexcept
{
    tauMain();

    Application* app = startGame();

    app->init(argCount, args);
    app->startGameLoop();

    return 0;
}
#endif
