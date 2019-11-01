#pragma once

#include <TextHandler.hpp>
#include <layer/LayerStack.hpp>
#include <ResourceLoader.hpp>
#include <camera/Camera2D.hpp>
#include "State.hpp"

class Window;
class Vector2f;

class TERenderer final
{
    DELETE_COPY(TERenderer);
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    Window& _window;
    ResourceLoader& _rl;
    TextHandler* _th;
    RenderingPipeline* _rp;
    State& _state;
    Camera2DController _camera;

    LayerStack _layerStack;
public:
    TERenderer(Window& window, ResourceLoader& _rl, State& state, bool async) noexcept;
    ~TERenderer() noexcept
    {
        delete _rp;
        delete _th;
    }

    [[nodiscard]] TextHandler& textHandler() noexcept { return *_th; }

    [[nodiscard]] RenderingPipeline& renderingPipeline() noexcept { return *_rp; }

    [[nodiscard]] const Camera2DController& camera() const noexcept { return _camera; }
    [[nodiscard]] Camera2DController& camera() noexcept { return _camera; }

    void render(const float delta) noexcept;
    void update(const float fixedDelta) noexcept;
    void onEvent(Event& e) noexcept;
};
