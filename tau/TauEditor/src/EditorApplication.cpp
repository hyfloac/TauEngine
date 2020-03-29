#define APP_MAIN
#include "Application.hpp"

#include "EditorApplication.hpp"
#include <TauEngine.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <VFS.hpp>
#include <Win32File.hpp>
#include <Utils.hpp>
#include <gl/GLUtils.hpp>
#include <events/Event.hpp>
#include <events/WindowEvent.hpp>
#include <GL/glew.h>
#include <ShlObj.h>
#include <texture/FITextureLoader.hpp>
#include <ResourceLoader.hpp>
#include <ResourceSelector.hpp>
#include <Timings.hpp>

#include <system/SystemInterface.hpp>
#include <graphics/DepthStencilState.hpp>

#include "dx/dx10/DX10RasterizerState.hpp"

static void setupGameFolders() noexcept;
static bool setupDebugCallback(TauEditorApplication* tea) noexcept;

TauEditorApplication::TauEditorApplication() noexcept
    : Application(32), _config { false, 800, 600 },
      _window(null), _logger(null), _renderer(null), _gameState(State::Game), _globals(nullptr)
{ }

TauEditorApplication::~TauEditorApplication() noexcept
{
    delete _renderer;
    delete _window;
    delete _globals;
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

    ResourceSelectorLoader::setCacheDirectory("|TERes/cache");

    TimingsWriter::begin("TauEditor::Initialization", "|TERes/perfInit.json");
    PERF();

    RenderingMode::getGlobalMode().setDebugMode(true);
    // RenderingMode::getGlobalMode().setMode(RenderingMode::OpenGL4_3);
    RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX10);

    _window = new Window(_config.windowWidth, _config.windowHeight, "Tau Editor", this);
    _window->createWindow();
    _window->showWindow();

    _graphicsInterface = SystemInterface::get()->createGraphicsInterface(RenderingMode::getGlobalMode());

    const DepthStencilArgs dsArgs(tau::rec);
    const NullableRef<IDepthStencilState> dsState = _graphicsInterface->createDepthStencilState().buildTauRef(dsArgs, null);

    const RasterizerArgs rsArgs(tau::rec);
    const NullableRef<IRasterizerState> rsState = _graphicsInterface->createRasterizerState().buildTauRef(rsArgs, null);

    RenderingContextArgs rcArgs;
    rcArgs.window = _window;
    rcArgs.vsync = _config.vsync || true;

    _renderingContext = _graphicsInterface->createRenderingContext().buildTauRef(rcArgs, null);
    _renderingContext->activateContext();
    _renderingContext->setDefaultDepthStencilState(dsState);
    _renderingContext->resetDepthStencilState();
    _renderingContext->setDefaultRasterizerState(rsState);
    _renderingContext->resetRasterizerState();

    _window->setEventHandler(::onWindowEvent);
    Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
    Mouse::setVisible(false);

    if(RenderingMode::getGlobalMode().isOpenGL())
    {
        setupDebugCallback(this);
        filterDebugOutput(GLDebugSeverity::Notification, false);
    }

    _renderingContext->setVSync(_config.vsync);

    TextureLoader::setMissingTexture(TextureLoader::generateMissingTexture(*_renderingContext));

    _globals = new Globals(*_window, *_graphicsInterface, *_renderingContext, _gr, _gameState);

    if(_renderingContext->mode().isOpenGL())
    {
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

        enableGLDepthTest();

        enableGLCullFace();
        glCullFace(GL_BACK);

        glFrontFace(GL_CW);

        setGLBlend(true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    _renderingContext->updateViewport(0, 0, _window->width(), _window->height());

    _renderer = new TERenderer(*_globals);

    TimingsWriter::end();
    TimingsWriter::begin("TauEditor::Runtime", "|TERes/perfRuntime.json");
    return true;
}

void TauEditorApplication::finalize() noexcept
{
    TimingsWriter::end();
    if(RenderingMode::getGlobalMode().isOpenGL())
    {
        stopDebugOutput();
    }
}

void TauEditorApplication::onException(ExceptionData& ex) noexcept
{
#if defined(_DEBUG)
    _logger->trace("Exception Thrown in {}@{} \"{}\"", ex.file, ex.line, ex.func);
#endif
    ExceptionDispatcher dispatcher(*ex.ex);
    dispatcher.dispatch<IncorrectContextException>(this, &TauEditorApplication::onIncorrectContext);
}

void TauEditorApplication::update(const float fixedDelta) noexcept
{
    PERF();
    ResourceLoader::update();
    _renderer->update(fixedDelta);
}

void TauEditorApplication::render(const DeltaTime& delta) noexcept
{
    PERF();
    _renderer->render(delta);
    _renderingContext->swapFrame();
}

void TauEditorApplication::renderFPS(const u32 ups, const u32 fps) noexcept
{
    PERF();
    char buf[64];
    snprintf(buf, 64, "Tau Editor: UPS / FPS: %u / %u", ups, fps);
    _window->setTitle(buf);
}

void TauEditorApplication::runMessageLoop() noexcept
{
    PERF();
#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 256
#endif

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
    PERF();
    if(VFS::Instance().fileExists(CONFIG_PATH))
    {
        CPPRef<IFile> configFile = VFS::Instance().openFile(CONFIG_PATH, FileProps::Read);
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

void TauEditorApplication::writeConfig() const noexcept
{
    PERF();
    CPPRef<IFile> file = VFS::Instance().openFile(CONFIG_PATH, FileProps::WriteNew);
    if(file)
    {
        file->writeType(_config);
    }
}

void TauEditorApplication::onWindowEvent(WindowEvent& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowAsciiKeyEvent>(this, &TauEditorApplication::onCharPress);
    dispatcher.dispatch<WindowKeyEvent>(this, &TauEditorApplication::onKeyPress);
    dispatcher.dispatch<WindowResizeEvent>(this, &TauEditorApplication::onWindowResize);

    _renderer->onEvent(e);
}

bool TauEditorApplication::onCharPress(WindowAsciiKeyEvent& e) const noexcept
{
    switch(e.c())
    {
        case 'v':
        {
            static bool vsync = _config.vsync;

            vsync = !vsync;
            _globals->rc.setVSync(vsync);
        }
        default: return false;
    }
}

bool TauEditorApplication::onKeyPress(WindowKeyEvent& e) noexcept
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

bool TauEditorApplication::onWindowResize(WindowResizeEvent& e) const noexcept
{
    UNUSED(e);
    _globals->rc.updateViewport(0, 0, e.newWidth(), e.newHeight());
    _renderer->camera()->setProjection(e.newWidth(), e.newHeight(), 0.0f, 0.0f);
    _renderer->camera3D().setProjection(static_cast<float>(e.newWidth()), static_cast<float>(e.newHeight()), 90, 0.0001f, 1000.0f);
    // Mouse::mousePos(e.newWidth() >> 1, e.newHeight() >> 1);
    return false;
}

void TauEditorApplication::onIncorrectContext(IncorrectContextException& ex) const noexcept
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
    VFS::Instance().mountDynamic("TERes", "D:/TauEngine/tau/TauEditor/resources", Win32FileLoader::Instance());
}

static bool setupDebugCallback(TauEditorApplication* tea) noexcept
{
#ifndef TAU_PRODUCTION
    setupDefaultDebugMessageCallback(tea->logger(), true);
    return true;
#else
    return false;
#endif
}
