#include <TERenderer.hpp>
#include <system/Window.hpp>

TERenderer::TERenderer(Window& window) noexcept
    : _th("|TERes/TextVertexShader.glsl", "|TERes/TextFragmentShader.glsl"),
      _rp(&window),
      _ortho(Matrix4f::orthographic(0.0f, static_cast<float>(window.width()), 0.0f, static_cast<float>(window.height())))
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
}
