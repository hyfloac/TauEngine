#include "EditorApplication.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <VFS.hpp>
#include <Win32File.hpp>
#include <Utils.hpp>
#include <gl/GLUtils.hpp>
#include <events/Event.hpp>
#include "events/WindowEvent.hpp"
#include "GL/glew.h"
#include "GL/wglew.h"

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

    _window = new Window(800, 600, "Tau Editor", this);
#if !defined(TAU_PRODUCTION)
    ContextSettings& contextSettings = _window->contextSettings();
    contextSettings.debug = true;
#endif
    _window->createWindow();
    _window->showWindow();
    if(!_window->createContext()) { return false; }
    _window->makeContextCurrent();
    _window->setEventHandler(::onWindowEvent);

    if(WGLEW_EXT_swap_control)
    {
        constexpr bool vsync = false;
        if(vsync)
        {
            wglSwapIntervalEXT(1);
        }
        else
        {
            wglSwapIntervalEXT(0);
        }
    }

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    enableGLBlend();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _renderer = new TERenderer(*_window);

    _renderer->renderingPipeline().pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _renderer->renderingPipeline().pushFinishRender();
    _renderer->renderingPipeline().pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _renderer->renderingPipeline().pushFinishRender();

    return true;
}

void TauEditorApplication::finalize() noexcept
{
}

void TauEditorApplication::render(const float delta) noexcept
{
    // UNUSED(delta);
    _renderer->render(delta);
}

void TauEditorApplication::update(const float fixedDelta) noexcept
{
    // UNUSED(fixedDelta);
    _renderer->update(fixedDelta);
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

void TauEditorApplication::onWindowEvent(WindowEvent& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowAsciiKeyEvent>(this, &TauEditorApplication::onCharPress);
    dispatcher.dispatch<WindowKeyEvent>(this, &TauEditorApplication::onKeyPress);
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool TauEditorApplication::onCharPress(WindowAsciiKeyEvent& e) noexcept
{
    switch(e.c())
    {
        // case 'p':
        // {
        //     _renderer->addTextSpeed(0.5f);
        //     break;
        // }
        // case 'o':
        // {
        //     _renderer->addTextSpeed(-0.5f);
        //     break;
        // }
        // case 'l':
        // {
        //     _renderer->addTextScale(0.05f);
        //     break;
        // }
        // case 'k':
        // {
        //     _renderer->addTextScale(-0.05f);
        //     break;
        // }
        // case 'e':
        // {
        //     _renderer->randomReset();
        //     break;
        // }
        // case 'r':
        // {
        //     _renderer->screenReset();
        //     break;
        // }
        case 'v':
        {
            static bool vsync = false;

            if(WGLEW_EXT_swap_control)
            {
                _renderer->renderingPipeline().takeControlOfContext();
                vsync = !vsync;
                if(vsync)
                {
                    wglSwapIntervalEXT(1);
                }
                else
                {
                    wglSwapIntervalEXT(0);
                }
                _renderer->renderingPipeline().returnControlOfContext();
            }
        }
        default: return false;
    }

    return false;
}


// ReSharper disable once CppMemberFunctionMayBeStatic
bool TauEditorApplication::onKeyPress(WindowKeyEvent& e) noexcept
{
    if(e.event() == KeyboardEvent::KE_KEY_PRESSED)
    {
        switch(e.key())
        {
            case 0x1B:
            {
                tauExit(0);
                break;
            }
            default: return false;
        }
    }

    return false;
}

Application* startGame() noexcept
{
    return new TauEditorApplication;
}

static void onWindowEvent(void* param, WindowEvent& e) noexcept
{
    auto* app = reinterpret_cast<TauEditorApplication*>(param);
    app->onWindowEvent(e);
}
