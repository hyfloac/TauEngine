#pragma once

#include <TextHandler.hpp>
#include <layer/LayerStack.hpp>
#include <ResourceLoader.hpp>
#include <camera/Camera2D.hpp>
#include "State.hpp"
#include <GameRecorder.hpp>

class Window;
class Vector2f;
class RenderingPipeline;

class TERenderer final
{
    DELETE_COPY(TERenderer);
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    Window& _window;
    ResourceLoader& _rl;
    GlyphSetHandle _consolas;
    GlyphSetHandle _consolasBold;
    GlyphSetHandle _consolasItalic;
    GlyphSetHandle _consolasBoldItalic;
    TextHandler* _th;
    RenderingPipeline* _rp;
    State& _state;
    Camera2DController _camera;
    GameRecorder _recorder;

    LayerStack _layerStack;
public:
    TERenderer(Window& window, ResourceLoader& _rl, State& state, bool async) noexcept;

    ~TERenderer() noexcept;

    [[nodiscard]] TextHandler& textHandler() noexcept { return *_th; }

    [[nodiscard]] RenderingPipeline& renderingPipeline() noexcept { return *_rp; }

    [[nodiscard]] const Camera2DController& camera() const noexcept { return _camera; }
    [[nodiscard]] Camera2DController& camera() noexcept { return _camera; }

    void render(const float delta) noexcept;
    void update(const float fixedDelta) noexcept;
    void onEvent(Event& e) noexcept;
private:
    static void __cdecl finalizeLoadConsolas(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
    static void __cdecl finalizeLoadConsolasBold(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
    static void __cdecl finalizeLoadConsolasItalic(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
    static void __cdecl finalizeLoadConsolasBoldItalic(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
};
