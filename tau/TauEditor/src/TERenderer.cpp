#include "TERenderer.hpp"
#include <system/Window.hpp>
#include <gl/GLUtils.hpp>
#include <maths/Vector2f.hpp>

static void __cdecl setupParams(void) noexcept;

TERenderer::TERenderer(Window& window) noexcept
    : _th("|TERes/TextVertexShader.glsl", "|TERes/TextFragmentShader.glsl"),
      _rp(window, setupParams, false),
      _ortho(Matrix4f::orthographic(0.0f, static_cast<float>(window.width()), 0.0f, static_cast<float>(window.height()))),
      _window(window)
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
    fast_srand(time(NULL));

    for(int i = 0; i < 150; ++i)
    {
        _physWords.emplace_back("test", _window, _th, _rp);
    }
}

void TERenderer::render(const float delta) noexcept
{
    _rp.pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(PhysWord& pw : _physWords)
    {
        pw.render(delta, _ortho);
    }
    _rp.pushFinishRender();
}

void TERenderer::update(const float fixedDelta) noexcept
{
    for(PhysWord& pw : _physWords)
    {
        pw.update(fixedDelta);
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
