#include "TERenderer.hpp"
#include <system/Window.hpp>
#include <gl/GLUtils.hpp>
#include <FastRand.hpp>
#include <PhysWordLayer.hpp>

static void __cdecl setupParams(void) noexcept;

TERenderer::TERenderer(Window& window) noexcept
    : _th("|TERes/TextVertexShader.glsl", "|TERes/TextFragmentShader.glsl"),
      _rp(window, setupParams, false),
      _ortho(Matrix4f::orthographic(0.0f, static_cast<float>(window.width()), 0.0f, static_cast<float>(window.height()))),
      _window(window), _layerStack()
{
    FT_Error thError = _th.init();
    if(thError)
    {
        // clientLogger->error("FreeType Initialization Error: {0}", thError);
    }
    thError = _th.loadTTFFile("|TERes/MonoConsole.ttf");
    if(thError)
    {
        // clientLogger->error("FreeType TTF Loading Error: {0}", thError);
    }
    _th.generateBitmapCharacters();
    _th.finishLoad();
    seedFastRand(time(NULL));

    _layerStack.pushLayer(new PhysWordLayer(150, "O", window, _th, _rp, _ortho));
}

void TERenderer::render(const float delta) noexcept
{
    _rp.pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(auto* layer : _layerStack)
    {
        layer->onRender(delta);
    }
    _rp.pushFinishRender();
}

void TERenderer::update(const float fixedDelta) noexcept
{
    for(auto* layer : _layerStack)
    {
        layer->onUpdate(fixedDelta);
    }
}

static void __cdecl setupParams(void) noexcept
{
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    enableGLDepthTest();

    enableGLCullFace();
    glCullFace(GL_BACK);

    glFrontFace(GL_CW);

    setGLBlend(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
