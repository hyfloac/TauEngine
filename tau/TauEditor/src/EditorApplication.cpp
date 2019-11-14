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
#include <shlobj.h>
#include <texture/FITextureLoader.hpp>
#include <RenderingPipeline.hpp>
#include <ResourceLoader.hpp>

static void setupGameFolders() noexcept;
static bool setupDebugCallback(TauEditorApplication* tea) noexcept;

TauEditorApplication::TauEditorApplication() noexcept
    : Application(32), _config { false, 800, 600 },
      _window(null), _logger(null), _renderer(null), _gameState(State::Game)
{ }

TauEditorApplication::~TauEditorApplication() noexcept
{
    delete _renderer;
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
    _window->createWindow();
    _window->showWindow();
    if(!_window->createContext()) { return false; }

    _window->renderingContext()->activateContext();

    _window->setEventHandler(::onWindowEvent);
    Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
    Mouse::setVisible(false);

    setupDebugCallback(this);

    _window->renderingContext()->setVSync(_config.vsync);

    bool async = false;

    for(int i = 0; i < argCount; ++i)
    {
        if(strcmp(args[i], "-async") == 0)
        {
            async = true;
        }
    }

    TextureLoader::setMissingTexture(TextureLoader::generateMissingTexture());

    _renderer = new TERenderer(*_window, _gameState, async);

    return true;
}

void TauEditorApplication::finalize() noexcept
{
    stopDebugOutput();
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
    ResourceLoader::update();
    _renderer->update(fixedDelta);
}

void TauEditorApplication::renderFPS(const u32 ups, const u32 fps) noexcept
{
    char buf[256];
    snprintf(buf, 256, "Tau Editor: UPS / FPS: %u / %u", ups, fps);
    _window->setTitle(buf);
}

void TauEditorApplication::runMessageLoop() noexcept
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
            TAU_RENDER_1(_renderer->renderingPipeline(), vsync, {
                context.setVSync(vsync);
            });
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
    TAU_RENDER_S(_renderer->renderingPipeline(), {
        context.updateViewport(0, 0, window.width(), window.height());
        self->_renderer->camera()->setProjection(window.height(), window.width(), 0.0f, 0.0f);
        Mouse::mousePos(window.width() >> 1, window.height() >> 1);
    });
    // _renderer->renderingPipeline().addCtxCtrl([](RenderingPipeline&, Window& window, void*)
    // { window.renderingContext()->updateViewport(0, 0, window.width(), window.height()); }, nullptr, true);
    // _renderer->camera()->setProjection(e.newHeight(), e.newWidth(), 0.0f, 0.0f);
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

void GLAPIENTRY openGLDebugErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept
{
    UNUSED2(length, userParam);
    UNUSED2(id, message);

    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    const TauEditorApplication* tea = reinterpret_cast<const TauEditorApplication*>(userParam);

#define STR_CASE(__ENUM, __STR) case __ENUM: str = __STR; break
#define DEFAULT_CASE default: str = "Unknown"; break

    tea->logger()->error("--OpenGL Error Callback--");
    tea->logger()->error("Message: {0}", message);
    const char* str;
    switch(source)
    {
        STR_CASE(GL_DEBUG_SOURCE_API, "API");
        STR_CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System");
        STR_CASE(GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler");
        STR_CASE(GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party");
        STR_CASE(GL_DEBUG_SOURCE_APPLICATION, "Application");
        STR_CASE(GL_DEBUG_SOURCE_OTHER, "Other");
        DEFAULT_CASE;
    }
    tea->logger()->error("Source [0x{0:X}] {1}", source, str);

    switch(type)
    {
        STR_CASE(GL_DEBUG_TYPE_ERROR, "Error");
        STR_CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behavior");
        STR_CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behavior");
        STR_CASE(GL_DEBUG_TYPE_PORTABILITY, "Portability");
        STR_CASE(GL_DEBUG_TYPE_PERFORMANCE, "Performance");
        STR_CASE(GL_DEBUG_TYPE_OTHER, "Other");
        STR_CASE(GL_DEBUG_TYPE_MARKER, "Marker");
        DEFAULT_CASE;
    }
    tea->logger()->error("Type: [0x{0:X}] {1}", type, str);
    tea->logger()->error("Id: 0x{0:X} // {0:d}", id);

    switch(severity)
    {
        STR_CASE(GL_DEBUG_SEVERITY_MEDIUM, "Medium");
        STR_CASE(GL_DEBUG_SEVERITY_HIGH, "High");
        STR_CASE(GL_DEBUG_SEVERITY_LOW, "Low");
        STR_CASE(GL_DEBUG_SEVERITY_NOTIFICATION, "Notification");
        DEFAULT_CASE;
    }
    tea->logger()->error("Severity: [0x{0:X}] {1}", severity, str);
    tea->logger()->error("-------------------------");
#if _DEBUG && 0
    if(severity != GL_DEBUG_SEVERITY_NOTIFICATION && renderData && renderData->rp)
    {
        FILE* fileData = fopen("CommandBuffer.txt", "w");
        renderData->rp->dumpCommandBufferToFile(fileData);
        fclose(fileData);

        // char* fileName = new char[1024];
        // GetFullPathNameA("CommandBuffer.txt", 1024, fileName, NULL);
        // fprintf(stderr, "Command Dump: %s\n", fileName);
        // delete[] fileName;
        __debugbreak();
    }
#else
    // getchar();
#endif

#undef DEFAULT_CASE
#undef STR_CASE
}

static bool setupDebugCallback(TauEditorApplication* tea) noexcept
{
#ifndef TAU_PRODUCTION
    setupDebugMessageCallback(openGLDebugErrorCallback, tea, true);
    return true;
#else
    return false;
#endif
}
