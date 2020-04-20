#include "TERenderer.hpp"
#include <system/Window.hpp>
#include "ConsoleLayer.hpp"
#include "Layer3D.hpp"
#include "Timings.hpp"
#include "Globals.hpp"

void __cdecl TERenderer::finalizeLoadConsolas(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolas = th.generateBitmapCharacters(renderer->_globals.gi, renderer->_globals.rc, "Consolas", 32, 126, false, file->face);
    delete file;
}

void TERenderer::finalizeLoadConsolasBold(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolasBold = th.generateBitmapCharacters(renderer->_globals.gi, renderer->_globals.rc, "ConsolasBold", 32, 126, false, file->face);
    delete file;
}

void TERenderer::finalizeLoadConsolasItalic(TextHandler::FileData* file,TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolasItalic = th.generateBitmapCharacters(renderer->_globals.gi, renderer->_globals.rc, "ConsolasItalic", 32, 126, false, file->face);
    delete file;
}

void TERenderer::finalizeLoadConsolasBoldItalic(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept
{
    TextHandler& th = finalizeParam->th;
    TERenderer* renderer = reinterpret_cast<TERenderer*>(finalizeParam->param);
    renderer->_consolasBoldItalic = th.generateBitmapCharacters(renderer->_globals.gi, renderer->_globals.rc, "ConsolasBoldItalic", 32, 126, false, file->face);
    delete file;
}

TERenderer::TERenderer(Globals& globals) noexcept
    : _globals(globals),
      _consolas(null), _consolasBold(null), _consolasItalic(null), _consolasBoldItalic(null),
      _th(nullptr), 
      _camera(globals.window, 800.0f, 100.0f, Keyboard::Key::W, Keyboard::Key::S, Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::Q, Keyboard::Key::E),
      _layerStack()
{
    PERF();
    _th = new TextHandler(globals.gi, globals.rc, "|TERes", "/shader/Text/", "TextVertexShader", "TextPixelShader");
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

    Layer3D* layer3D = new Layer3D(globals);
    _camera3D = &layer3D->camera().camera();

    _layerStack.pushLayer(layer3D);
    _layerStack.pushOverlay(new ConsoleLayer(globals, *_th, _consolas, _consolasBold, _consolasItalic, _consolasBoldItalic, _camera->projectionMatrix(), layer3D->camera().camera(), 0.5f));

    _saturationLayer = new SaturationPPLayer(globals, 1.25f);
    _saturationLayer->init();
}

TERenderer::~TERenderer() noexcept
{
    delete _th;
}

void TERenderer::preRender(const DeltaTime& delta) noexcept
{
    PERF();

    for(auto* layer : _layerStack.layers())
    {
        layer->onPreRender(delta);
    }
    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onPreRender(delta);
    }
}

void TERenderer::render() noexcept
{
    PERF();

    _globals.rc.clearScreen(true, true, false, { 127, 127, 255, 255 });
    _globals.rc.beginFrame();
    for(auto* layer : _layerStack.layers())
    {
        layer->onRender();
    }

    if(_saturationLayer && _saturationLayer->active())
    {
        _saturationLayer->onFrameBufferUnbind();
        _saturationLayer->onRender();
    }

    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onRender();
    }
    _globals.rc.endFrame();
}

void TERenderer::postRender() noexcept
{
    PERF();

    for(auto* layer : _layerStack.layers())
    {
        layer->onPostRender();
    }

    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onPostRender();
    }
}

void TERenderer::update(const float fixedDelta) noexcept
{
    PERF();
    if(_globals.gameState == State::Game)
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

    EventDispatcher dispatcher(e);
    dispatcher.dispatch<ControlEvent>(this, &TERenderer::onControlEvent);

    for(auto* layer : _layerStack.layers())
    {
        layer->onEvent(e);
    }

    if(_saturationLayer)
    {
        _saturationLayer->onEvent(e);
    }

    for(auto* overlay : _layerStack.overlays())
    {
        overlay->onEvent(e);
    }
}

bool TERenderer::onControlEvent(ControlEvent& e) noexcept
{
    if(e.type() == CE_SET_NEXT_FB)
    {
        if(_saturationLayer && _saturationLayer->active())
        {
            _saturationLayer->onFrameBufferBind();
        }
    }
    else if(e.type() == CE_ACTIVATE_SAT_FB)
    {
        if(_saturationLayer)
        {
            _saturationLayer->active(*reinterpret_cast<bool*>(e.arg()));
        }
    }

    return false;
}
