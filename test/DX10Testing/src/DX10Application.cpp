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
#include <model/VertexArray.hpp>

static void setupGameFolders() noexcept;

DX10Application::DX10Application() noexcept
    : Application(32), _config { false, 800, 600 },
      _window(null), _logger(null), _r(13), _g(23), _b(127), _rr(1), _gg(1), _bb(-1)
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
    // Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
    // Mouse::setVisible(false);

    ctx().setVSync(_config.vsync || true);

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


    // Ref<IInputLayoutBuilder> ilBuilder = ctx().createInputLayout(2);
    // ilBuilder->setLayoutDescriptor(0, ShaderDataType::Vector2Float, ShaderSemantic::Position);
    // ilBuilder->setLayoutDescriptor(1, ShaderDataType::Vector4Float, ShaderSemantic::Color);
    // const Ref<IInputLayout> inputLayout = Ref<IInputLayout>(ilBuilder->build());

    Ref<IShaderBuilder> shaderBuilder = ctx().createShader();

    shaderBuilder->type(IShader::Type::Vertex);
    shaderBuilder->file(VFS::Instance().openFile("|res/shader/TestVertexShader.cso", FileProps::Read));
    // shaderBuilder->inputLayout(inputLayout);
    auto vertexShader = Ref<IShader>(shaderBuilder->build());

    shaderBuilder->type(IShader::Type::Pixel);
    shaderBuilder->file(VFS::Instance().openFile("|res/shader/TestPixelShader.cso", FileProps::Read));
    auto pixelShader = Ref<IShader>(shaderBuilder->build());

    _shader = IShaderProgram::create(ctx());
    _shader->setVertexShader(ctx(), vertexShader);
    _shader->setPixelShader(ctx(), pixelShader);
    _shader->link(ctx());
    float positions[2 * 3] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };

    Ref<IBufferBuilder> bufBuilder = ctx().createBuffer(1);
    bufBuilder->type(EBuffer::Type::ArrayBuffer);
    bufBuilder->usage(EBuffer::UsageType::StaticDraw);
    bufBuilder->bufferSize(sizeof(positions));
    bufBuilder->initialBuffer(positions);
    bufBuilder->descriptor().addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
    const Ref<IBuffer> posBuffer = Ref<IBuffer>(bufBuilder->build(null));

    float colors[4 * 3] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    bufBuilder->bufferSize(sizeof(colors));
    bufBuilder->initialBuffer(colors);
    bufBuilder->descriptor().reset(1);
    bufBuilder->descriptor().addDescriptor(ShaderSemantic::Color, ShaderDataType::Vector4Float);
    const Ref<IBuffer> colorBuffer = Ref<IBuffer>(bufBuilder->build(null));

    Ref<IVertexArrayBuilder> vaBuilder = ctx().createVertexArray(2);

    vaBuilder->setVertexBuffer(0, posBuffer);
    vaBuilder->setVertexBuffer(1, colorBuffer);
    // vaBuilder->inputLayout(inputLayout);
    vaBuilder->shader(vertexShader);
    vaBuilder->drawCount(3);
    vaBuilder->drawType(DrawType::SeparatedTriangles);
    _va = Ref<IVertexArray>(vaBuilder->build());

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
#define UPDATE_COLOR0(_COL, _INC) do {\
        (_COL) += (_INC);\
        if((_COL) == 255 || (_COL) == 0) { (_INC) *= -1; } \
    }while(0);
#define UPDATE_COLOR(_COL) UPDATE_COLOR0(_##_COL, _##_COL##_COL)
    UPDATE_COLOR(r);
    UPDATE_COLOR(g);
    UPDATE_COLOR(b);
}

void DX10Application::render(const DeltaTime& delta) noexcept
{
    UNUSED(delta);
    _logger->info("Render: DeltaTimeMS: {}", delta.mSeconds());
    ctx().beginFrame();
      ctx().clearScreen(true, true, false, RGBAColor { _r, _g, _b, 255 });
      _shader->bind(ctx());
        _va->preDraw(ctx());
        _va->draw(ctx());
        _va->postDraw(ctx());
      _shader->unbind(ctx());
    ctx().endFrame();
    ctx().swapFrame();
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
    if(!setupWinFolder(CSIDL_LOCAL_APPDATA, "/DX10Testing", "game"))
    {
        VFS::Instance().mountDynamic("game", "C:/DX10Testing", Win32FileLoader::Instance());
        if(!Win32FileLoader::Instance()->fileExists("C:/DX10Testing"))
        {
            if(!Win32FileLoader::Instance()->createFolder("C:/DX10Testing"))
            {
            }
        }
    }
    VFS::Instance().mountDynamic("res", "E:/TauEngine/test/DX10Testing/resources", Win32FileLoader::Instance());
}
