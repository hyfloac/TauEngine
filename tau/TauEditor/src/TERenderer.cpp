#include "TERenderer.hpp"
#include <system/Window.hpp>
#include <gl/GLUtils.hpp>
#include <PhysWordLayer.hpp>
#include "ConsoleLayer.hpp"
#include "Layer3D.hpp"
#include "Timings.hpp"

static void __cdecl setupParams(RenderingPipeline&, Window&, void*) noexcept;

void __cdecl TERenderer::finalizeLoadConsolas(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolas = th.generateBitmapCharacters(*renderer->_window.renderingContext(), "Consolas", 32, 126, false, file->face);
    delete file;
}

void TERenderer::finalizeLoadConsolasBold(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolasBold = th.generateBitmapCharacters(*renderer->_window.renderingContext(), "ConsolasBold", 32, 126, false, file->face);
    delete file;
}

void TERenderer::finalizeLoadConsolasItalic(TextHandler::FileData* file,TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolasItalic = th.generateBitmapCharacters(*renderer->_window.renderingContext(), "ConsolasItalic", 32, 126, false, file->face);
    delete file;
}

void TERenderer::finalizeLoadConsolasBoldItalic(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolasBoldItalic = th.generateBitmapCharacters(*renderer->_window.renderingContext(), "ConsolasBoldItalic", 32, 126, false, file->face);
    delete file;
}

TERenderer::TERenderer(Window& window, State& state, bool async) noexcept
    : _window(window),
      _consolas(-1), _consolasBold(-1), _consolasItalic(-1), _consolasBoldItalic(-1),
      _th(nullptr), _rp(nullptr), _state(state),
      _camera(window, 800.0f, 100.0f, Keyboard::Key::W, Keyboard::Key::S, Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Q, Keyboard::Key::E),
      _recorder(), _layerStack()
{
    PERF();
    _rp = new RenderingPipeline(window, setupParams, nullptr, async);
    _th = new TextHandler(*window.renderingContext(), "|TERes/shader/Text/TextVertexShader.glsl", "|TERes/shader/Text/TextPixelShader.glsl");
    (void) _th->init();
    // (void) _th->loadTTFFile("|TERes/Sansation_Regular.ttf", 0, 48, finalizeLoadSansation, this);
    // (void) _th->loadTTFFile("|TERes/MonoConsole.ttf",  0, 48, finalizeLoadMono, this);
    DynString path = findSystemFont("Consolas (TrueType)");
    (void) _th->loadTTFFile(path.c_str(),  0, 48, finalizeLoadConsolas, this);
    path = findSystemFont("Consolas Bold (TrueType)");
    (void) _th->loadTTFFile(path.c_str(),  0, 48, finalizeLoadConsolasBold, this);
    path = findSystemFont("Consolas Italic (TrueType)");
    (void) _th->loadTTFFile(path.c_str(),  0, 48, finalizeLoadConsolasItalic, this);
    path = findSystemFont("Consolas Bold Italic (TrueType)");
    (void) _th->loadTTFFile(path.c_str(),  0, 48, finalizeLoadConsolasBoldItalic, this);

    Layer3D* layer3D = new Layer3D(window, *_rp, &_recorder, state);
    _camera3D = &layer3D->camera().camera();

    // _layerStack.pushLayer(new PhysWordLayer(150, "O", window, *_th, *_rp, _camera->compoundedMatrix(), state));
    _layerStack.pushLayer(layer3D);
    _layerStack.pushOverlay(new ConsoleLayer(window, _recorder, *_th, _consolas, _consolasBold, _consolasItalic, _consolasBoldItalic, _camera->projectionMatrix(), *_rp, state, layer3D->camera().camera(), 0.5f));
    
}

TERenderer::~TERenderer() noexcept
{
    delete _rp;
    delete _th;
}

void TERenderer::render(const DeltaTime& delta) noexcept
{
    PERF();
    _rp->pushLoadContext(*_window.renderingContext());
    // _rp->pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    TAU_RENDER(*_rp, {
        context.clearScreen(true, true, true, { 127, 127, 255, 255 });
        context.beginFrame();
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    });
    for(auto* layer : _layerStack.layers())
    {
        layer->onRender(delta);
    }
    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onRender(delta);
    }
    TAU_RENDER(*_rp, {
        context.endFrame();
    });
    _rp->pushFinishRender();
}

void TERenderer::update(const float fixedDelta) noexcept
{
    PERF();
    if(_state == State::Game)
    {
        _camera.update(fixedDelta);
    }

    for(auto* layer : _layerStack.layers())
    {
        layer->onUpdate(fixedDelta);
    }
    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onUpdate(fixedDelta);
    }
}

void TERenderer::onEvent(Event& e) noexcept
{
    PERF();
    for(auto* layer : _layerStack.layers())
    {
        layer->onEvent(e);
    }
    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onEvent(e);
    }
}

static void __cdecl setupParams(RenderingPipeline&, Window& window, void*) noexcept
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

    window.renderingContext()->updateViewport(0, 0, window.width(), window.height());
}
