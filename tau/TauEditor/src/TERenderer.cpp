#include "TERenderer.hpp"
#include <system/Window.hpp>
#include <gl/GLUtils.hpp>
#include <FastRand.hpp>
#include <PhysWordLayer.hpp>

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

TERenderer::TERenderer(Window& window, ResourceLoader& rl, bool async) noexcept
    : _window(window), _rl(rl),
      _th(nullptr), _rp(nullptr),
      _ortho(glm::ortho(0.0f, static_cast<float>(window.width()), 0.0f, static_cast<float>(window.height()))),
      _layerStack()
{
    _rp = new RenderingPipeline(window, setupParams, nullptr, async);
    _th = new TextHandler(*window.renderingContext(), "|TERes/TextVertexShader.glsl", "|TERes/TextFragmentShader.glsl");
    const FT_Error thError = _th->init();
    if(thError)
    {
        // clientLogger->error("FreeType Initialization Error: {0}", thError);
    }
    const int fileError = _th->loadTTFFile("|TERes/MonoConsole.ttf", rl, finalizeLoad);
    if(fileError)
    {
        
    }


    // if(thError)
    // {
    //     // clientLogger->error("FreeType TTF Loading Error: {0}", thError);
    // }
    // _th->generateBitmapCharacters();
    // _th->finishLoad();
    // seedFastRand(time(NULL));

    // window.renderingContext()->activateContext();
    // setupParams(*_rp, window, null);

    _layerStack.pushLayer(new PhysWordLayer(150, "O", window, *_th, *_rp, _ortho));
}

void TERenderer::render(const float delta) noexcept
{
    _rp->pushLoadContext(*_window.renderingContext());
    _rp->pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(auto* layer : _layerStack)
    {
        layer->onRender(delta);
    }
    _rp->pushFinishRender();
}

void TERenderer::update(const float fixedDelta) noexcept
{
    for(auto* layer : _layerStack)
    {
        layer->onUpdate(fixedDelta);
    }
}

void TERenderer::onEvent(Event& e) noexcept
{
    for(auto* layer : _layerStack)
    {
        layer->onEvent(e);
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
