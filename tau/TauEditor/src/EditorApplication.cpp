#define APP_MAIN
#include "Application.hpp"

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
#include <shlobj.h>

static void setupGameFolders() noexcept;

TauEditorApplication::TauEditorApplication() noexcept
    : Application(32), _window(null), _renderer(null), _config{ false, 800, 600 }
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

    setupGameFolders();
    setupConfig();

    _window = new Window(_config.windowWidth, _config.windowHeight, "Tau Editor", this);
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
        if(_config.vsync)
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

    return true;
}

void TauEditorApplication::finalize() noexcept
{
}

void TauEditorApplication::onException(Exception& ex) noexcept
{
    ExceptionDispatcher dispatcher(ex);
    dispatcher.dispatch<IncorrectContextException>(this, &TauEditorApplication::onIncorrectContext);
}

void TauEditorApplication::render(const float delta) noexcept
{
    _renderer->render(delta);
}

void TauEditorApplication::update(const float fixedDelta) noexcept
{
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

void TauEditorApplication::setupConfig() noexcept
{
    if(VFS::Instance().fileExists(CONFIG_PATH))
    {
        Ref<IFile> configFile = VFS::Instance().openFile(CONFIG_PATH, FileProps::Read);
        Config tmp = { false, 0, 0 };
        const i32 read = configFile->readBytes(reinterpret_cast<u8*>(&tmp), sizeof(tmp));
        if(read != sizeof(tmp))
        {
            writeConfig();
        }
        else
        {
            _config = tmp;
        }
    }
    else
    {
        writeConfig();
    }
}

void TauEditorApplication::writeConfig() noexcept
{
    Ref<IFile> file = VFS::Instance().openFile(CONFIG_PATH, FileProps::WriteNew);
    if(file)
    {
        file->writeBytes(reinterpret_cast<u8*>(&_config), sizeof(_config));
    }
}

void TauEditorApplication::onWindowEvent(WindowEvent& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowAsciiKeyEvent>(this, &TauEditorApplication::onCharPress);
    dispatcher.dispatch<WindowKeyEvent>(this, &TauEditorApplication::onKeyPress);
}

bool TauEditorApplication::onCharPress(WindowAsciiKeyEvent& e) noexcept
{
    switch(e.c())
    {
        case 'v':
        {
            static bool vsync = _config.vsync;

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
}

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


void TauEditorApplication::onIncorrectContext(IncorrectContextException& ex)
{
    UNUSED(ex);
    _logger->trace("Incorrect Context Detected, Exiting.");
}

Application* startGame() noexcept
{
    return new(std::nothrow) TauEditorApplication;
}

static void onWindowEvent(void* param, WindowEvent& e) noexcept
{
    auto* app = reinterpret_cast<TauEditorApplication*>(param);
    app->onWindowEvent(e);
}

static bool setupWinFolder(int dir, const char* subPath, const char* mountPoint) noexcept
{
    CHAR folderPath[MAX_PATH];
    const HRESULT result = SHGetFolderPathA(NULL, dir, NULL, SHGFP_TYPE_CURRENT, folderPath);

    if(SUCCEEDED(result))
    {
        DynString folder(folderPath);
        if(subPath)
        {
            folder.append(subPath);
        }
        if(!Win32FileLoader::Instance()->fileExists(folder))
        {
            if(!Win32FileLoader::Instance()->createFolder(folder))
            {
                return false;
            }
        }

        VFS::Instance().mountDynamic(mountPoint, folder, Win32FileLoader::Instance());
        return true;
    }

    return false;
}

static void setupGameFolders() noexcept
{
    // if(!setupWinFolder(CSIDL_MYDOCUMENTS, "\\My Games\\TauEditor", "game"))
    // {
    //     VFS::Instance().mount("game", "C:\\TauEditor", Win32FileLoader::Instance());
    // }
    if(!setupWinFolder(CSIDL_LOCAL_APPDATA, "/TauEditor", "game"))
    {
        VFS::Instance().mountDynamic("game", "C:/TauEditor", Win32FileLoader::Instance());
        if(!Win32FileLoader::Instance()->fileExists("C:/TauEditor"))
        {
            if(!Win32FileLoader::Instance()->createFolder("C:/TauEditor"))
            {
            }
        }
    }
    VFS::Instance().mountDynamic("TERes", "E:/TauEngine/tau/TauEditor/resources", Win32FileLoader::Instance());
}
