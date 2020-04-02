#pragma once

#include <TextHandler.hpp>
#include <layer/LayerStack.hpp>
#include <camera/Camera2D.hpp>
#include <camera/Camera3D.hpp>
#include <GameRecorder.hpp>
#include <Timings.hpp>

struct Globals;
class Window;
class Vector2f;
class RenderingPipeline;

class TERenderer final
{
    DELETE_COPY(TERenderer);
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    Globals& _globals;

    GlyphSetHandle _consolas;
    GlyphSetHandle _consolasBold;
    GlyphSetHandle _consolasItalic;
    GlyphSetHandle _consolasBoldItalic;
    TextHandler* _th;
    Camera2DController _camera;
    Camera3D* _camera3D;

    LayerStack _layerStack;
public:
    TERenderer(Globals& globals) noexcept;

    ~TERenderer() noexcept;

    [[nodiscard]] TextHandler& textHandler() noexcept { return *_th; }

    [[nodiscard]] const Camera2DController& camera() const noexcept { return _camera; }
    [[nodiscard]] Camera2DController& camera() noexcept { return _camera; }

    [[nodiscard]] const Camera3D& camera3D() const noexcept { return *_camera3D; }
    [[nodiscard]] Camera3D& camera3D() noexcept { return *_camera3D; }

    void preRender(const DeltaTime& delta) noexcept;
    void render() noexcept;
    void postRender() noexcept;

    void update(const float fixedDelta) noexcept;
    void onEvent(Event& e) noexcept;
private:
    static void __cdecl finalizeLoadConsolas(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
    static void __cdecl finalizeLoadConsolasBold(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
    static void __cdecl finalizeLoadConsolasItalic(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
    static void __cdecl finalizeLoadConsolasBoldItalic(TextHandler::FileData* file, TextHandler::FinalizeData* finalizeParam) noexcept;
};
