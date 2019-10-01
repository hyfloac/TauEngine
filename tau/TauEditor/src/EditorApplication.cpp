#include "EditorApplication.hpp"
#include <VFS.hpp>
#include <Win32File.hpp>
#include <Utils.hpp>
#include "gl/GLUtils.hpp"

TauEditorApplication::TauEditorApplication() noexcept
    : Application(32), _window(null), _renderer(null)
{ }

TauEditorApplication::~TauEditorApplication() noexcept
{
    stopDebugOutput();
    delete _window;
}

bool TauEditorApplication::init(int argCount, char* args[]) noexcept
{
    UNUSED(argCount);
    UNUSED(args);

    _logger = spdlog::stdout_color_mt("TauEditor2");
    _logger->set_level(spdlog::level::trace);
    _logger->set_pattern("%^[%H:%M:%S:%e] [%n] [%l]%$: %v");

    VFS::Instance().mount("TERes", "E:/TauEngine/tau/TauEditor/resources", Win32FileLoader::Instance());

    _window = new Window(800, 600, "Tau Editor");
#if !defined(TAU_PRODUCTION)
    ContextSettings& contextSettings = _window->contextSettings();
    contextSettings.debug = true;
#endif
    _window->createWindow();
    _window->showWindow();

    if(!_window->createContext())
    {
        return false;
    }

    _window->makeContextCurrent();


    _renderer = new TERenderer(*_window);

    return true;
}

void TauEditorApplication::render(const float delta) noexcept
{
    UNUSED(delta);
    RenderingPipeline& rp = _renderer->renderingPipeline();

    rp.pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rp.pushRenderText(&_renderer->textHandler(), "Hello World!", 0.0f, static_cast<float>(_window->height() - 15), 0.35f, Vector3f(0.4f, 0.4f, 0.2f), _renderer->ortho());
    rp.pushFinishRender();
}

void TauEditorApplication::update(const float fixedDelta) noexcept
{
    UNUSED(fixedDelta);
}

void TauEditorApplication::renderFPS(const u32 ups, const u32 fps) noexcept
{
    char buf[256];
    snprintf(buf, 256, "Tau Editor: UPS / FPS: %u / %u", ups, fps);
    _window->setTitle(buf);
}

#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 64
#endif

void TauEditorApplication::runMessageLoop() noexcept
{
    MSG msg;
    u32 cnt = 0;
    while(cnt++ < NUM_MESSAGES_TO_READ && PeekMessageA(&msg, null, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

Application* startGame() noexcept
{
    return new TauEditorApplication;
}
