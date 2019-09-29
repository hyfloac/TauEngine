#define APP_IMPL
// #include <TauEngine.hpp>
#include <Application.hpp>

Application::Application(const u32 targetUPS) noexcept
    : _targetUPS(targetUPS)
{
    // _instance = this;
}

Application::~Application() noexcept = default;

void Application::startGameLoop() const noexcept
{
    // const update_f updateBox = [](const float fixedDelta) { Application::_instance->update(fixedDelta); };
    // const render_f renderBox = [](const float delta) { Application::_instance->render(delta); };
    // const renderFPS_f fpsBox = [](const u32 ups, const u32 fps) { Application::_instance->renderFPS(ups, fps); };
    //
    // tauGameLoop(this->_targetUPS, updateBox, renderBox, fpsBox);
}
