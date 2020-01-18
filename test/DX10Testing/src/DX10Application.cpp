#define APP_MAIN
#include <Application.hpp>

#include "DX10Application.hpp"
#include <TauEngine.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <VFS.hpp>
#include <Win32File.hpp>
#include <Utils.hpp>
#include <ShlObj.h>
#include <texture/FITextureLoader.hpp>
#include <RenderingPipeline.hpp>
#include <ResourceLoader.hpp>
#include <Timings.hpp>

static void setupGameFolders() noexcept;

DX10Application::DX10Application() noexcept
    : Application(32), _config { false, 800, 600 },
      _window(null), _logger(null)
{ }

DX10Application::~DX10Application() noexcept
{
    delete _window;
}

bool DX10Application::init(int argCount, char* args[]) noexcept
{
    _logger = spdlog::stdout_color_mt("DX10 Test");
    _logger->set_level(spdlog::level::trace);
    _logger->set_pattern("%^[%H:%M:%S:%e] [%n] [%l]%$: %v");

    setupGameFolders();
    setupConfig();

    TimingsWriter::begin("DX10Test::Initialization", "|TERes/perfInit.json");

    RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX10);

    _window = new(std::nothrow) Window(_config.windowWidth, _config.windowHeight, "DX10 Test", this);
    _window->createWindow();
    _window->showWindow();
    if(!_window->createContext()) { return false; }
    _window->renderingContext()->activateContext();
    _window->setEventHandler(::onWindowEvent);
    Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
    // Mouse::setVisible(false);

    _window->renderingContext()->setVSync(_config.vsync);

    bool async = false;

    for(int i = 0; i < argCount; ++i)
    {
        if(strcmp(args[i], "-async") == 0)
        {
            async = true;
        }
    }

    UNUSED(async);

    // TextureLoader::setMissingTexture(TextureLoader::generateMissingTexture(*_window->renderingContext()));

    TimingsWriter::end();
    TimingsWriter::begin("DX10Test::Runtime", "|TERes/perfRuntime.json");
    return true;
}

void DX10Application::finalize() noexcept
{
    TimingsWriter::end();
}

void DX10Application::onException(Exception& ex) noexcept
{
    ExceptionDispatcher dispatcher(ex);
    dispatcher.dispatch<IncorrectContextException>(this, &DX10Application::onIncorrectContext);
}

void DX10Application::update(float fixedDelta) noexcept
{
    UNUSED(fixedDelta);
    ResourceLoader::update();
}

void DX10Application::render(const DeltaTime& delta) noexcept
{
    UNUSED(delta);
    auto& ctx = *_window->renderingContext();
    ctx.beginFrame();
    ctx.clearScreen(true, true, false, RGBAColor { 255, 0, 0, 255 });
    ctx.endFrame();
    ctx.swapFrame();
}

void DX10Application::renderFPS(u32 ups, u32 fps) noexcept
{
    char buf[256];
    snprintf(buf, 256, "DX10 Test: UPS / FPS: %u / %u", ups, fps);
    _window->setTitle(buf);
}

void DX10Application::runMessageLoop() noexcept
{
#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 64
#endif

    MSG msg;
    u32 cnt = 0;
    while(cnt++ < NUM_MESSAGES_TO_READ && PeekMessageA(&msg, null, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void DX10Application::setupConfig() noexcept
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

void DX10Application::writeConfig() noexcept
{
    Ref<IFile> file = VFS::Instance().openFile(CONFIG_PATH, FileProps::WriteNew);
    if(file)
    {
        file->writeBytes(reinterpret_cast<u8*>(&_config), sizeof(_config));
    }
}

void DX10Application::onWindowEvent(WindowEvent& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowAsciiKeyEvent>(this, &DX10Application::onCharPress);
    dispatcher.dispatch<WindowKeyEvent>(this, &DX10Application::onKeyPress);
    dispatcher.dispatch<WindowResizeEvent>(this, &DX10Application::onWindowResize);
}

bool DX10Application::onCharPress(WindowAsciiKeyEvent& e) const noexcept
{
    UNUSED(e);
    return false;
}

bool DX10Application::onKeyPress(WindowKeyEvent& e) noexcept
{
    if(e.event() == Keyboard::Event::Pressed)
    {
        switch(e.key())
        {
            case Keyboard::Key::Esc:
            {
                tauExit(0);
                break;
            }
            default: return false;
        }
    }

    return false;
}

bool DX10Application::onWindowResize(WindowResizeEvent& e) const noexcept
{
    _window->renderingContext()->updateViewport(0, 0, e.newWidth(), e.newHeight());
    Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
    return false;
}

void DX10Application::onIncorrectContext(IncorrectContextException& ex) const noexcept
{
    UNUSED(ex);
    _logger->trace("Incorrect Context Detected, Exiting.");
}

Application* startGame() noexcept
{
    return new(std::nothrow) DX10Application;
}

static void onWindowEvent(void* const param, WindowEvent& e) noexcept
{
    auto* app = reinterpret_cast<DX10Application*>(param);
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
            folder = folder.concat(subPath);
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
    // if(!setupWinFolder(CSIDL_MYDOCUMENTS, "\\My Games\\DX10Test", "game"))
    // {
    //     VFS::Instance().mount("game", "C:\\DX10Test", Win32FileLoader::Instance());
    // }
    if(!setupWinFolder(CSIDL_LOCAL_APPDATA, "/DX10Test", "game"))
    {
        VFS::Instance().mountDynamic("game", "C:/DX10Test", Win32FileLoader::Instance());
        if(!Win32FileLoader::Instance()->fileExists("C:/DX10Test"))
        {
            if(!Win32FileLoader::Instance()->createFolder("C:/DX10Test"))
            {
            }
        }
    }
    VFS::Instance().mountDynamic("res", "E:/TauEngine/test/DX10Test/resources", Win32FileLoader::Instance());
}
