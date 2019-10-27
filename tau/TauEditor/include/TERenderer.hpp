#pragma once

#include <TextHandler.hpp>
#include <layer/LayerStack.hpp>
#include <ResourceLoader.hpp>

class Window;
class Vector2f;

class TERenderer final
{
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    Window& _window;
    ResourceLoader& _rl;
    TextHandler* _th;
    RenderingPipeline* _rp;
    glm::mat4 _ortho;

    LayerStack _layerStack;
public:
    TERenderer(Window& window, ResourceLoader& _rl, bool async) noexcept;
    ~TERenderer() noexcept
    {
        delete _rp;
        delete _th;
    }

    [[nodiscard]] TextHandler& textHandler() noexcept { return *_th; }

    [[nodiscard]] RenderingPipeline& renderingPipeline() noexcept { return *_rp; }

    [[nodiscard]] const glm::mat4& ortho() const noexcept { return _ortho; }
    [[nodiscard]] glm::mat4& ortho() noexcept { return _ortho; }

    void render(const float delta) noexcept;
    void update(const float fixedDelta) noexcept;
    void onEvent(Event& e) noexcept;
};
