#define APP_MAIN
#include <Application.hpp>

#include "DX10Application.hpp"
#include <TauEngine.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <VFS.hpp>
#include <Win32File.hpp>
#include <Utils.hpp>
#include <ShlObj.h>
#include <ResourceLoader.hpp>
#include <Timings.hpp>
#include <model/VertexArray.hpp>
#include <shader/IShader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ResourceSelector.hpp>
#include <texture/FITextureLoader.hpp>
#include <shader/bundle/ShaderBundleParser.hpp>
#include <shader/bundle/PrintShaderBundleVisitor.hpp>
#include <shader/bundle/ast/ExprAST.hpp>
#include <maths/glmExt/GlmMatrixTransformExt.hpp>
#include <graphics/DepthStencilState.hpp>

#include <gl/GLUtils.hpp>
#include <system/SystemInterface.hpp>

#include "dx/dx10/DX10RasterizerState.hpp"

static bool setupDebugCallback(DX10Application* tea) noexcept;

template<>
class UniformAccessor<DX10Application::Uniforms> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const DX10Application::Uniforms& t) noexcept
    {
        const float r = static_cast<float>(t.color.r) / 255.0f;
        const float g = static_cast<float>(t.color.g) / 255.0f;
        const float b = static_cast<float>(t.color.b) / 255.0f;
        const float a = static_cast<float>(t.color.a) / 255.0f;

        buffer->beginModification(context);
        buffer->modifyBuffer(sizeof(float) * 0, r);
        buffer->modifyBuffer(sizeof(float) * 1, g);
        buffer->modifyBuffer(sizeof(float) * 2, b);
        buffer->modifyBuffer(sizeof(float) * 3, a);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<DX10Application::Matrices> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    static constexpr uSys MATRIX_SIZE = sizeof(float) * 4 * 4;

    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE * 3; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const DX10Application::Matrices& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(MATRIX_SIZE * 0, MATRIX_SIZE, glm::value_ptr(t.projection));
        buffer->modifyBuffer(MATRIX_SIZE * 1, MATRIX_SIZE, glm::value_ptr(t.viewMatrix));
        buffer->modifyBuffer(MATRIX_SIZE * 2, MATRIX_SIZE, glm::value_ptr(t.model));
        buffer->endModification(context);
    }
};

class RMRSTransformer final : public IResourceSelectorTransformer
{
    DEFAULT_CONSTRUCT_PU(RMRSTransformer);
    DEFAULT_DESTRUCT(RMRSTransformer);
    DELETE_COPY(RMRSTransformer);
public:
    [[nodiscard]] ResIndex transform(const DynString& key) noexcept override;

    [[nodiscard]] static ResIndex transform(const RenderingMode& rm) noexcept;
};

static void setupGameFolders() noexcept;

DX10Application::DX10Application() noexcept
    : Application(32), _config { false, 800, 600 },
      _window(null), _logger(null), _r(0), _g(0), _b(0), _colorState(-1), _aa(1), _uni(null), _matrices(null),
      _camera(null), _camera3D(null)
{ }

DX10Application::~DX10Application() noexcept
{
    delete _uni;
    delete _matrices;
    delete _camera;
    delete _camera3D;
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

    RenderingMode::getGlobalMode().setDebugMode(true);
    // RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX10);
    RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX11);
    // RenderingMode::getGlobalMode().setMode(RenderingMode::OpenGL4_3);

    _window = new(std::nothrow) Window(_config.windowWidth, _config.windowHeight, "DX10 Test", this);
    _window->createWindow();
    _window->showWindow();

    _graphicsInterface = SystemInterface::get()->createGraphicsInterface(RenderingMode::getGlobalMode());

    DepthStencilArgs dsArgs(tau::rec);
    NullableRef<IDepthStencilState> dsState = _graphicsInterface->createDepthStencilState().buildTauRef(dsArgs, null);

    RasterizerArgs rsArgs(tau::rec);
    NullableRef<IRasterizerState> rsState = _graphicsInterface->createRasterizerState().buildTauRef(rsArgs, null);

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
    // Mouse::setVisible(false);

    if(RenderingMode::getGlobalMode().isOpenGL())
    {
        setupDebugCallback(this);
        filterDebugOutput(GLDebugSeverity::Notification, false);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        ctx().setFaceWinding(true);
    }

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

    const CPPRef<IFile> testShader = VFS::Instance().openFile("|res/shader/TestShader.tausi", FileProps::Read);
    ShaderBundleLexer lexer(testShader);
    ShaderBundleParser parser(lexer);

    auto ast = parser.parse();

    PrintSBVArgs pArgs{};
    pArgs.indentCount = 4;
    pArgs.indentChar = PrintSBVArgs::Spaces;
    pArgs.newLineChar = PrintSBVArgs::LF;
    pArgs.tessCtrlName = PrintSBVArgs::TessellationControl;
    pArgs.tessEvalName = PrintSBVArgs::TessellationEvaluation;
    pArgs.pixelName = PrintSBVArgs::Fragment;
    pArgs.braces = PrintSBVArgs::SameLine;
    PrintShaderBundleVisitor visitor(&pArgs);
    ast->visit(visitor);

    // TextureLoader::setMissingTexture(TextureLoader::generateMissingTexture(ctx()));
    
    ResourceSelectorLoader::setCacheDirectory("|res/cache");
    
    ShaderArgs shaderArgs;
    shaderArgs.vfsMount = "|res";
    shaderArgs.path = "/shader/";
    
    shaderArgs.fileName = "TestVertexShader";
    shaderArgs.stage = EShader::Stage::Vertex;
    auto vertexShader = ctx().createShader().buildCPPRef(shaderArgs, null);
    
    shaderArgs.fileName = "TestPixelShader";
    shaderArgs.stage = EShader::Stage::Pixel;
    auto pixelShader = ctx().createShader().buildCPPRef(shaderArgs, null);
    
    _shader = IShaderProgram::create(ctx());
    _shader->setVertexShader(ctx(), vertexShader);
    _shader->setPixelShader(ctx(), pixelShader);
    _shader->link(ctx());
    
    float positionsSquare[2 * 4] = {
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f
    };
    
    u32 indicesSquare[3 * 2] = {
        0, 1, 3,
        2, 3, 1
    };
    
    BufferArgs bufBuilder(1);
    bufBuilder.type = EBuffer::Type::ArrayBuffer;
    bufBuilder.usage = EBuffer::UsageType::StaticDraw;
    bufBuilder.elementCount = 4;
    bufBuilder.initialBuffer = positionsSquare;
    bufBuilder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
    const CPPRef<IBuffer> posBuffer = ctx().createBuffer().buildCPPRef(bufBuilder, null);
    
    IndexBufferArgs indiceArgs;
    indiceArgs.usage = EBuffer::UsageType::StaticDraw;
    indiceArgs.elementCount = 6;
    indiceArgs.initialBuffer = indicesSquare;
    const CPPRef<IIndexBuffer> indices = ctx().createIndexBuffer().buildCPPRef(indiceArgs, null);
    
    float colorsSquare[4 * 4] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 1.0f, 1.0f, 1.0f
    };
    
    bufBuilder.initialBuffer = colorsSquare;
    bufBuilder.descriptor.reset(1);
    bufBuilder.descriptor.addDescriptor(ShaderSemantic::Color, ShaderDataType::Vector4Float);
    const CPPRef<IBuffer> colorBuffer = ctx().createBuffer().buildCPPRef(bufBuilder, null);
    
    float texCoords[2 * 4] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };
    
    bufBuilder.initialBuffer = texCoords;
    bufBuilder.descriptor.reset(1);
    bufBuilder.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);
    const CPPRef<IBuffer> texBuffer = ctx().createBuffer().buildCPPRef(bufBuilder, null);
    
    VertexArrayArgs vaArgs(3);
    
    vaArgs.buffers[0] = posBuffer;
    vaArgs.buffers[1] = colorBuffer;
    vaArgs.buffers[2] = texBuffer;
    vaArgs.indexBuffer = indices;
    vaArgs.shader = vertexShader;
    vaArgs.drawCount = 6;
    vaArgs.drawType = DrawType::SeparatedTriangles;

    _va = ctx().createVertexArray().buildCPPRef(vaArgs, null);
    
    _uni = new UniformBlockS<Uniforms>(ctx().createUniformBuffer(), RGBAColor { 0, 0, 0, 0 });
    _matrices = new UniformBlockS<Matrices>(ctx().createUniformBuffer());
    
    _camera = new(::std::nothrow) Camera2DController(*_window, 100.0f, 100.0f, Keyboard::Key::W, Keyboard::Key::S, Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::E, Keyboard::Key::Q);
    _camera3D = new(::std::nothrow) FreeCamCamera3DController(*_window, 90.0f, 0.0001f, 1000.0f, 10.0f, 40.0f, 0.03f, false,
        Keyboard::Key::Shift, Keyboard::Key::W, Keyboard::Key::S,
        Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Space, Keyboard::Key::Ctrl,
        null);
    
    _matrices->data().model = glmExt::rotateDegrees(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f)), glm::vec3(0.0f, 0.0f, -1.0f)), 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    if(glmExt::useTranspose(RenderingMode::getGlobalMode().currentMode()))
    {
        _matrices->data().model = glmExt::transpose(_matrices->data().model);
    }
    
    // TextureSamplerArgs textureSamplerArgs;
    // textureSamplerArgs.magFilter() = ETexture::Filter::Nearest;
    // textureSamplerArgs.minFilter() = ETexture::Filter::Nearest;
    // textureSamplerArgs.mipFilter() = ETexture::Filter::Nearest;
    // textureSamplerArgs.wrapU = ETexture::WrapMode::Repeat;
    // textureSamplerArgs.wrapV = ETexture::WrapMode::Repeat;
    // textureSamplerArgs.wrapW = ETexture::WrapMode::Repeat;
    // textureSamplerArgs.depthCompareFunc = ETexture::DepthCompareFunc::Never;
    //
    // CPPRef<ISingleTextureUploaderBuilder> uploaderBuilder = ctx().createSingleTextureUploader();
    // uploaderBuilder->texture(CPPRef<ITexture>(TextureLoader::generateDebugTexture16(ctx(), 5)));
    // uploaderBuilder->textureSampler(ctx().createTextureSampler().buildCPPRef(textureSamplerArgs, null));
    // _texUploader = CPPRef<ISingleTextureUploader>(uploaderBuilder->build());

    TimingsWriter::end();
    TimingsWriter::begin("DX10Test::Runtime", "|TERes/perfRuntime.json");
    return true;
}

void DX10Application::finalize() noexcept
{
    TimingsWriter::end();
    if(RenderingMode::getGlobalMode().isOpenGL())
    {
        stopDebugOutput();
    }
}

void DX10Application::onException(ExceptionData& ex) noexcept
{
#if defined(_DEBUG)
    _logger->trace("Exception Thrown in {}@{} \"{}\"", ex.file, ex.line, ex.func);
#endif
    ExceptionDispatcher dispatcher(*ex.ex);
    dispatcher.dispatch<IncorrectContextException>(this, &DX10Application::onIncorrectContext);
}

void DX10Application::update(float fixedDelta) noexcept
{
    UNUSED(fixedDelta);
    ResourceLoader::update();

    switch(_colorState)
    {
        case 0:
            --_b;
            ++_r;
            if(_r == 200) { _colorState = 1; }
            break;
        case 1:
            --_r;
            ++_g;
            if(_g == 200) { _colorState = 2; }
            break;
        case 2:
            --_g;
            ++_b;
            if(_b == 200) { _colorState = 0; }
            break;
        default:
            _r = 55;
            _g = 55;
            _b = 200;
            _colorState = 0;
            break;
    }

    _uni->data().color.r += _aa;
    _uni->data().color.g += _aa;
    _uni->data().color.b += _aa;
    if(_uni->data().color.r == 0 || _uni->data().color.r == 255)
    { _aa *= -1; }

    _camera->update(fixedDelta);
    // {
    //     const u32 screenCenterW = _window->width() >> 1;
    //     const u32 screenCenterH = _window->height() >> 1;
    //     const Mouse::Pos pos = Mouse::mousePos(*_window);
    //     Mouse::mousePos(*_window, screenCenterW, screenCenterH);
    //     const i32 mouseDifX = static_cast<i32>(screenCenterW) - static_cast<i32>(pos.x);
    //     const i32 mouseDifY = static_cast<i32>(screenCenterH) - static_cast<i32>(pos.y);
    //     _camera3D->update(fixedDelta, mouseDifX, mouseDifY);
    // }
    if(glmExt::useTranspose(ctx().mode()))
    {
        _matrices->data().projection = _camera3D->camera().projectionMatrixTrans();
    }
    else
    {
        _matrices->data().projection = _camera3D->camera().projectionMatrix();
    }
}

void DX10Application::render(const DeltaTime& delta) noexcept
{
    UNUSED(delta);
    _camera3D->lerp(delta);
    if(glmExt::useTranspose(ctx().mode()))
    {
        _matrices->data().viewMatrix = _camera3D->camera().viewMatrixTrans();
    }
    else
    {
        _matrices->data().viewMatrix = _camera3D->camera().viewMatrix();
    }

    ctx().beginFrame();
      ctx().clearScreen(true, true, false, RGBAColor { _r, _g, _b, 255 });
      _shader->bind(ctx());
        _matrices->upload(ctx(), EShader::Stage::Vertex, 0);
        _uni->upload(ctx(), EShader::Stage::Pixel, 1);
        // TextureIndices ind;
        // _texUploader->upload(ctx(), ind, EShader::Stage::Pixel);
      
        _va->bind(ctx());
        _va->preDraw(ctx());
        _va->draw(ctx());
        _va->postDraw(ctx());
        _va->unbind(ctx());
      
        // TextureIndices ind0;
        // _texUploader->unbind(ctx(), ind0, EShader::Stage::Pixel);
        _uni->unbind(ctx(), EShader::Stage::Pixel, 1);
        _matrices->unbind(ctx(), EShader::Stage::Vertex, 0);
      _shader->unbind(ctx());
    ctx().endFrame();
    ctx().swapFrame();
}

void DX10Application::renderFPS(const u32 ups, const u32 fps) noexcept
{
    char buf[256];
    snprintf(buf, 256, "DX10 Test: UPS / FPS: %u / %u", ups, fps);
    _window->setTitle(buf);
}

void DX10Application::runMessageLoop() noexcept
{
#ifndef NUM_MESSAGES_TO_READ
  #define NUM_MESSAGES_TO_READ 128
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
        CPPRef<IFile> configFile = VFS::Instance().openFile(CONFIG_PATH, FileProps::Read);
        Config tmp = { false, 0, 0 };
        const i32 read = configFile->readType(&tmp);
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
    CPPRef<IFile> file = VFS::Instance().openFile(CONFIG_PATH, FileProps::WriteNew);
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

bool DX10Application::onCharPress(WindowAsciiKeyEvent& e) noexcept
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

bool DX10Application::onWindowResize(WindowResizeEvent& e) noexcept
{
    _renderingContext->updateViewport(0, 0, e.newWidth(), e.newHeight());
    _camera->camera().setProjection(*_window);
    _camera3D->camera().setProjection(*_window, 90, 0.0001f, 1000.0f);
    // Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
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
    VFS::Instance().mountDynamic("res", "D:/TauEngine/test/DX10Testing/resources", Win32FileLoader::Instance());
}

IResourceSelectorTransformer::ResIndex RMRSTransformer::transform(const DynString& key) noexcept
{
    STR_SWITCH(key, {
        STR_CASE("DirectX10", { return 0; })
        STR_CASE("Vulkan",    { return 1; })
        STR_CASE("OpenGL",    { return 2; })
    }, { })
    return -1;
}

IResourceSelectorTransformer::ResIndex RMRSTransformer::transform(const RenderingMode& rm) noexcept
{
    switch(rm.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
        case RenderingMode::Mode::DirectX10:
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return 0;
        case RenderingMode::Mode::Vulkan:
            return 1;
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return 2;
        default: return -1;
    }
}

static bool setupDebugCallback(DX10Application* app) noexcept
{
#ifndef TAU_PRODUCTION
    setupDefaultDebugMessageCallback(app->logger(), true);
    return true;
#else
    return false;
#endif
}
