#include "TERenderer.hpp"
#include <system/Window.hpp>
#include <gl/GLUtils.hpp>
#include <PhysWordLayer.hpp>
#include "ConsoleLayer.hpp"

static void __cdecl setupParams(RenderingPipeline&, Window&, void*) noexcept;

static void finalizeLoad(void* file, void* finalizeParam) noexcept
{
    const intptr_t errorPtr = reinterpret_cast<intptr_t>(file);
    const FT_Error error = errorPtr;
    if(error)
    {
        
    }
    TextHandler* th = reinterpret_cast<TextHandler*>(finalizeParam);
    th->generateBitmapCharacters();
    th->finishLoad();
}

TERenderer::TERenderer(Window& window, ResourceLoader& rl, State& state, bool async) noexcept
    : _window(window), _rl(rl),
      _th(nullptr), _rp(nullptr), _state(state),
      _camera(window, 800.0f, 100.0f, Keyboard::Key::W, Keyboard::Key::S, Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Q, Keyboard::Key::E),
      _layerStack()
{
    _rp = new RenderingPipeline(window, setupParams, nullptr, async);
    _th = new TextHandler(*window.renderingContext(), "|TERes/TextVertexShader.glsl", "|TERes/TextFragmentShader.glsl");
    (void) _th->init();
    (void) _th->loadTTFFile("|TERes/MonoConsole.ttf", rl, finalizeLoad);

    _layerStack.pushLayer(new PhysWordLayer(150, "O", window, *_th, *_rp, _camera->compoundedMatrix(), state));
    _layerStack.pushOverlay(new ConsoleLayer(window, *_th, _camera->projectionMatrix(), *_rp, state, _camera, 0.6f));
}

void TERenderer::render(const float delta) noexcept
{
    _rp->pushLoadContext(*_window.renderingContext());
    _rp->pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onRender(delta);
    }
    for(auto* layer : _layerStack.layers())
    {
        layer->onRender(delta);
    }
    _rp->pushFinishRender();
}

void TERenderer::update(const float fixedDelta) noexcept
{
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

    window.renderingContext()->updateViewport(0, 0, window.width(), window.height());
}
