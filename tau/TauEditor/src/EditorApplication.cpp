#define APP_MAIN
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
#include <graphics/RasterizerState.hpp>
#include <graphics/BlendingState.hpp>

#include <vr/VRUtils.hpp>

static void setupGameFolders(bool dbgVFSFolder) noexcept;
static bool setupDebugCallback(TauEditorApplication* tea) noexcept;

TauEditorApplication::TauEditorApplication() noexcept
    : Application(32), _config { false, true, 800, 600 },
      _window(null), _logger(null), _renderer(null), _vr(null), _gameState(State::Game), _globals(nullptr),
      _vrProjLeft(1.0f), _vrProjRight(1.0f), _vrLeftFB(null), _vrRightFB(null)
{ }

TauEditorApplication::~TauEditorApplication() noexcept
{
    delete _renderer;
    delete _window;
    delete _globals;
    delete _vrLeftFB;
    delete _vrRightFB;
}

bool TauEditorApplication::init(int argCount, char* args[]) noexcept
{
    UNUSED(argCount);
    UNUSED(args);

    _logger = spdlog::stdout_color_mt("TauEditor");
    _logger->set_level(spdlog::level::trace);
    _logger->set_pattern("%^[%H:%M:%S:%e] [%n] [%l]%$: %v");

    bool dbgVFSFolder = false;
    bool useVR = false;


#ifdef TAU_PRODUCTION
    RenderingMode::getGlobalMode().setDebugMode(false);
#else
    RenderingMode::getGlobalMode().setDebugMode(true);
#endif
    RenderingMode::getGlobalMode().setMode(RenderingMode::OpenGL4_3);
    // RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX10);
    // RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX11);

    for(int i = 1; i < argCount; ++i)
    {
        if(strcmp(args[i], "-gl") == 0)
        {
            RenderingMode::getGlobalMode().setMode(RenderingMode::OpenGL4_3);
        }
        else if(strcmp(args[i], "-dx10") == 0)
        {
            RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX10);
        }
        else if(strcmp(args[i], "-dx11") == 0)
        {
            RenderingMode::getGlobalMode().setMode(RenderingMode::DirectX11);
        }
        else if(strcmp(args[i], "-dbgVFS") == 0)
        {
            dbgVFSFolder = true;
        }
        else if(strcmp(args[i], "-vr") == 0)
        {
            useVR = true;
        }
        else if(strcmp(args[i], "-gDebug") == 0)
        {
            RenderingMode::getGlobalMode().setDebugMode(true);
        }
        else if(strcmp(args[i], "--gDebug") == 0)
        {
            RenderingMode::getGlobalMode().setDebugMode(false);
        }
    }

    setupGameFolders(dbgVFSFolder);
    setupConfig();

    _config.useVR = useVR;

    ResourceSelectorLoader::setCacheDirectory("|TERes/cache");

    TimingsWriter::begin("TauEditor::Initialization", "|TERes/perfInit.json");
    PERF();

    _window = new Window(_config.windowWidth, _config.windowHeight, "Tau Editor", this);
    _window->createWindow();
    _window->showWindow();

    _graphicsInterface = SystemInterface::get()->createGraphicsInterface(RenderingMode::getGlobalMode());
    if(!_graphicsInterface)
    {
        SystemInterface::get()->createAlert("Critical Error", "Failed to create graphics interface.");
        return false;
    }

    {
        RenderingContextArgs rcArgs;
        rcArgs.window = _window;
        rcArgs.vsync = _config.vsync || true;

        IRenderingContextBuilder::Error error;
        _renderingContext = _graphicsInterface->createRenderingContext().buildTauRef(rcArgs, &error);

        if(error != IRenderingContextBuilder::Error::NoError)
        {
            switch(error)
            {
                case IRenderingContextBuilder::Error::SystemMemoryAllocationError:
                    SystemInterface::get()->createAlert("Critical Error", "System failed to allocate default rendering context.");
                    break;
                case IRenderingContextBuilder::Error::UnsupportedAPIVersion:
                    SystemInterface::get()->createAlert("Critical Error", "Requested graphics API version is not supported by the system.");
                    break;
                case IRenderingContextBuilder::Error::NullWindow:
                    SystemInterface::get()->createAlert("Critical Error", "Failed to create a window.");
                    break;
                default: break;
            }
            return false;
        }
    }

    _renderingContext->activateContext();

    {
        const DepthStencilArgs dsArgs(tau::rec);

        IDepthStencilStateBuilder::Error error;
        const NullableRef<IDepthStencilState> dsState = _graphicsInterface->createDepthStencilState().buildTauRef(dsArgs, &error);

        if(error != IDepthStencilStateBuilder::Error::NoError)
        {
            if(error == IDepthStencilStateBuilder::Error::SystemMemoryAllocationFailure)
            { SystemInterface::get()->createAlert("Critical Error", "System failed to allocate default depth stencil state."); }
            else if(error == IDepthStencilStateBuilder::Error::DriverMemoryAllocationFailure)
            { SystemInterface::get()->createAlert("Critical Error", "Graphics driver failed to allocate default depth stencil state."); }
            else
            { SystemInterface::get()->createAlert("Critical Error", "Failed to create default depth stencil state."); }
            return false;
        }

        _renderingContext->setDefaultDepthStencilState(dsState);
    }

    {
        const RasterizerArgs rsArgs(tau::rec);

        IRasterizerStateBuilder::Error error;
        const NullableRef<IRasterizerState> rsState = _graphicsInterface->createRasterizerState().buildTauRef(rsArgs, &error);

        if(error != IRasterizerStateBuilder::Error::NoError)
        {
            if(error == IRasterizerStateBuilder::Error::SystemMemoryAllocationFailure)
            { SystemInterface::get()->createAlert("Critical Error", "System failed to allocate default rasterizer state."); }
            else if(error == IRasterizerStateBuilder::Error::DriverMemoryAllocationFailure)
            { SystemInterface::get()->createAlert("Critical Error", "Graphics driver failed to allocate default rasterizer state."); }
            else
            { SystemInterface::get()->createAlert("Critical Error", "Failed to create default rasterizer state."); }
            return false;
        }

        _renderingContext->setDefaultRasterizerState(rsState);
    }

    {
        BlendingArgs bsArgs(tau::rec);
        bsArgs.independentBlending = false;
        bsArgs.frameBuffers[0].enableBlending = true;
        bsArgs.frameBuffers[0].colorSrcFactor = BlendingArgs::BlendFactor::SrcAlpha;
        bsArgs.frameBuffers[0].colorDstFactor = BlendingArgs::BlendFactor::InvSrcAlpha;
        bsArgs.frameBuffers[0].alphaSrcFactor = BlendingArgs::BlendFactor::SrcAlpha;
        bsArgs.frameBuffers[0].alphaDstFactor = BlendingArgs::BlendFactor::InvSrcAlpha;

        IBlendingStateBuilder::Error error;
        const NullableRef<IBlendingState> bsState = _graphicsInterface->createBlendingState().buildTauRef(bsArgs, &error);

        if(error != IBlendingStateBuilder::Error::NoError)
        {
            if(error == IBlendingStateBuilder::Error::SystemMemoryAllocationFailure)
            { SystemInterface::get()->createAlert("Critical Error", "System failed to allocate default blending state."); }
            else if(error == IBlendingStateBuilder::Error::DriverMemoryAllocationFailure)
            { SystemInterface::get()->createAlert("Critical Error", "Graphics driver failed to allocate default blending state."); }
            else
            { SystemInterface::get()->createAlert("Critical Error", "Failed to create default blending state."); }
            return false;
        }

        _renderingContext->setDefaultBlendingState(bsState);
    }

    _renderingContext->resetDepthStencilState();
    _renderingContext->resetRasterizerState();
    {
        const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        _renderingContext->resetBlendingState(color);
    }

    _window->setEventHandler(::onWindowEvent);
    Mouse::mousePos(_window->width() >> 1, _window->height() >> 1);
    Mouse::setVisible(false);

    if(RenderingMode::getGlobalMode().isOpenGL())
    {
        setupDebugCallback(this);
        filterDebugOutput(GLDebugSeverity::Notification, false);
    }

    _renderingContext->setVSync(_config.vsync);

    TextureLoader::setMissingTexture(TextureLoader::generateMissingTexture(*_graphicsInterface, *_renderingContext));

    _globals = new Globals(*_window, *_graphicsInterface, *_renderingContext, _gr, _gameState);

    _renderingContext->updateViewport(0, 0, _window->width(), _window->height());

    _renderer = new TERenderer(*_globals);
    _globals->renderer = _renderer;

    if(_config.useVR)
    {
        if(vr::VR_IsRuntimeInstalled() && vr::VR_IsHmdPresent())
        {
            vr::EVRInitError error;
            _vr = vr::VR_Init(&error, vr::VRApplication_Scene);

            if(error != vr::VRInitError_None)
            {
                _vr = null;
                _logger->error("Unable to init VR runtime: {}", vr::VR_GetVRInitErrorAsEnglishDescription(error));
            }
            else if(!vr::VRCompositor())
            {
                _vr = null;
            }
            else
            {
                initVRControls();

                vr::HmdMatrix44_t tmpLeft = _vr->GetProjectionMatrix(vr::Eye_Left, 0.0001f, 1000.0f);
                vr::HmdMatrix44_t tmpRight = _vr->GetProjectionMatrix(vr::Eye_Right, 0.0001f, 1000.0f);
                _vrProjLeft = reinterpret_cast<glm::mat4&>(tmpLeft);
                _vrProjRight = reinterpret_cast<glm::mat4&>(tmpRight);

                initVRFrameBuffers();

                _globals->vr = _vr;

                _globals->vrCamera = new VRFreeCamCamera3DController(*_window, 90.0f, 0.0001f, 1000.0f, 10.0f, 40.0f, _globals->vrHandles.moveUp,
                    _globals->vrHandles.moveDown, _globals->vrHandles.movePlane, _globals->vrHandles.actionSet, _vr);

                _vr->GetRecommendedRenderTargetSize(&_width, &_height);
                RECT rect{ 0, 0, (LONG) _width, (LONG) _height };
                AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
                SetWindowPos(_window->sysWindowContainer().windowHandle, null, 0, 0, static_cast<int>(rect.right), static_cast<int>(rect.bottom), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSENDCHANGING);

                Mouse::setVisible(true);
            }
        }
    }

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
    if(_vr)
    {
        vr::VR_Shutdown();
        _vr = null;
    }
}

void TauEditorApplication::onException(ExceptionData& ex) noexcept
{
#if defined(_DEBUG)
    _logger->trace("Exception Thrown in {}@{} \"{}\"", ex.file, ex.line, ex.func);
#endif
    ExceptionDispatcher dispatcher(*ex.ex);
    dispatcher.dispatch<IncorrectContextException>(this, &TauEditorApplication::onIncorrectContext);
    dispatcher.dispatch<BufferSafetyException>(this, &TauEditorApplication::onBufferSafetyException);
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

    if(_vr)
    {
        _renderer->preRender(delta);

        {
            _globals->currentLeftEye = true;
            _vrLeftFB->bind(*_renderingContext);
            _renderer->render();
            _vrLeftFB->unbind(*_renderingContext);

            // _vrLeftFB->colorAttachments()[0]->bind(*_renderingContext, 0, EShader::Stage::Pixel);

            const vr::Texture_t leftTexture = {
                tauGetVRTextureHandle(_vrLeftFB->colorAttachments()[0].get()),
                tauGetTextureType(RenderingMode::getGlobalMode()),
                vr::ColorSpace_Gamma
            };
            vr::VRCompositor()->Submit(vr::Eye_Left, &leftTexture);

            // _vrLeftFB->colorAttachments()[0]->bind(*_renderingContext, 0, EShader::Stage::Pixel);
        }

        {
            _globals->currentLeftEye = false;
            _vrRightFB->bind(*_renderingContext);
            _renderer->render();

            const vr::Texture_t rightTexture = {
                tauGetVRTextureHandle(_vrLeftFB->colorAttachments()[0].get()),
                tauGetTextureType(RenderingMode::getGlobalMode()),
                vr::ColorSpace_Gamma
            };
            vr::VRCompositor()->Submit(vr::Eye_Right, &rightTexture);
            _vrRightFB->unbind(*_renderingContext);

            _vrRightFB->bind(*_renderingContext, IFrameBuffer::AccessMode::Read);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
            _vrRightFB->unbind(*_renderingContext);
        }

        if(RenderingMode::getGlobalMode().isOpenGL())
        {
            if constexpr(true)
            {
                glFinish();
            }
        }

        _renderingContext->swapFrame();

        _renderer->postRender();
    }
    else
    {
        _renderer->preRender(delta);

        _renderer->render();
        _renderingContext->swapFrame();

        _renderer->postRender();
    }
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
        Config tmp = { false, false, 0, 0 };
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

    if(_renderer)
    {
        _renderer->onEvent(e);
    }
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
    _globals->rc.resizeSwapChain(e.newWidth(), e.newHeight());
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

void TauEditorApplication::onBufferSafetyException(BufferSafetyException& ex) const noexcept
{
    const char* exMsg;
    switch(ex.type())
    {
        case BufferSafetyException::DoubleBufferBind: exMsg = "Buffer double bound."; break;
        case BufferSafetyException::DoubleBufferUnbind: exMsg = "Buffer double unbound."; break;
        case BufferSafetyException::DoubleUniformBufferBind: exMsg = "Uniform Buffer double bound to shader pipeline."; break;
        case BufferSafetyException::DoubleUniformBufferUnbind: exMsg = "Uniform Buffer double unbound from shader pipeline."; break;
        case BufferSafetyException::ModifiedWithoutBegin: exMsg = "Modified buffer without call to beginModification."; break;
        case BufferSafetyException::DoubleModifyBegin: exMsg = "Attempted to begin buffer modification twice."; break;
        case BufferSafetyException::DoubleModifyEnd: exMsg = "Attempted to end buffer modification twice."; break;
        case BufferSafetyException::ModifiedStaticBuffer: exMsg = "Attempted to modify a static buffer."; break;
        case BufferSafetyException::Unknown:
        default: exMsg = "Unknown Error"; break;
    }

    _logger->trace("Buffer safety error occured: {}", exMsg);
}

void TauEditorApplication::initVRControls() const noexcept
{
    const auto path = VFS::Instance().resolvePath("|TERes/vr/vr_actions.json");
    vr::VRInput()->SetActionManifestPath(path.path.c_str());

    vr::VRInput()->GetActionHandle("/actions/TauEditor/in/Up", &_globals->vrHandles.moveUp);
    vr::VRInput()->GetActionHandle("/actions/TauEditor/in/Down", &_globals->vrHandles.moveDown);
    vr::VRInput()->GetActionHandle("/actions/TauEditor/in/MovePlane", &_globals->vrHandles.movePlane);

    vr::VRInput()->GetActionHandle("/actions/TauEditor/in/TriggerLeft", &_globals->vrHandles.triggerLeft);
    vr::VRInput()->GetActionHandle("/actions/TauEditor/in/TriggerRight", &_globals->vrHandles.triggerRight);

    vr::VRInput()->GetActionHandle("/actions/TauEditor/out/haptic_left", &_globals->vrHandles.hapticLeft);
    vr::VRInput()->GetActionHandle("/actions/TauEditor/out/haptic_right", &_globals->vrHandles.hapticRight);

    vr::VRInput()->GetActionSetHandle("/actions/TauEditor", &_globals->vrHandles.actionSet);

    vr::VRInput()->GetInputSourceHandle("/user/hand/left", &_globals->vrHandles.sourceLeft);
    vr::VRInput()->GetInputSourceHandle("/user/hand/right", &_globals->vrHandles.sourceRight);
}

void TauEditorApplication::initVRFrameBuffers() noexcept
{
    u32 width;
    u32 height;
    _vr->GetRecommendedRenderTargetSize(&width, &height);

    // CPPRef<IFrameBufferBuilder> fbBuilder = _renderingContext->createFrameBuffer();
    // IFrameBufferAttachment* color = IFrameBufferAttachment::create(*_renderingContext, IFrameBufferAttachment::Type::Color, width, height);
    // IFrameBufferAttachment* depth = IFrameBufferAttachment::create(*_renderingContext, IFrameBufferAttachment::Type::DepthStencil, width, height);
    // fbBuilder->attach(color);
    // fbBuilder->attach(depth);
    // _vrLeftFB = fbBuilder->build();
    //
    // fbBuilder = _renderingContext->createFrameBuffer();
    // color = IFrameBufferAttachment::create(*_renderingContext, IFrameBufferAttachment::Type::Color, width, height);
    // depth = IFrameBufferAttachment::create(*_renderingContext, IFrameBufferAttachment::Type::DepthStencil, width, height);
    // fbBuilder->attach(color);
    // fbBuilder->attach(depth);
    // _vrRightFB = fbBuilder->build();
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

static void setupGameFolders(const bool dbgVFSFolder) noexcept
{
    if(dbgVFSFolder)
    {
        VFS::Instance().mountDynamic("TERes", "D:/TauEngine/tau/TauEditor/resources", Win32FileLoader::Instance());
    }
    else
    {
        VFS::Instance().mountDynamic("TERes", "resources", Win32FileLoader::Instance());
    }
}

static bool setupDebugCallback(TauEditorApplication* tea) noexcept
{
#ifndef TAU_PRODUCTION
    setupDefaultDebugMessageCallback(tea->logger(), true);
    return true;
#else
    UNUSED(tea);
    return false;
#endif
}
