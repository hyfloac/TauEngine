#include "Application.hpp"
#include "TauEngine.hpp"
#include "Timings.hpp"

Application::Application(const u32 targetUPS) noexcept
    : _targetUPS(targetUPS)
{ }

Application::~Application() noexcept = default;

void Application::startGameLoop() noexcept
{
    const float Mu_PER_UPDATE = 1000000.0f / _targetUPS;
    u64 lastTime = microTime();
    float lag = 0.0f;

    u64 counterTime = lastTime;
    u32 fps = 0;
    u32 ups = 0;

    while(!tauShouldExit())
    {
        const u64 currentTime = microTime();
        const u64 elapsed = currentTime - lastTime;
        lastTime = currentTime;
        lag += static_cast<float>(elapsed);

        while(lag >= Mu_PER_UPDATE)
        {
            runMessageLoop();

            update(Mu_PER_UPDATE);
            ++ups;
            lag -= Mu_PER_UPDATE;
        }

        if(elapsed != 0)
        {
            render(static_cast<float>(elapsed));
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

    Exception* ex = tauGetException();
    if(ex)
    {
        onException(*ex);
    }
}
