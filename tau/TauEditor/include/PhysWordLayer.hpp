#pragma once

#include <layer/ILayer.hpp>
#include <PhysWord.hpp>
#include "State.hpp"

class PhysWordLayer final : public ILayer
{
    DEFAULT_DESTRUCT(PhysWordLayer);
    LAYER_IMPL(PhysWordLayer);
private:
    Window& _window;
    TextHandler& _th;
    RenderingPipeline& _rp;
    const glm::mat4& _ortho;
    State& _state;

    std::vector<PhysWord> _physWords;
public:
    PhysWordLayer(size_t wordCount, const char* word, Window& window, TextHandler& th, RenderingPipeline& rp, const glm::mat4& ortho, State& state) noexcept;

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(float delta) noexcept override;

    void onEvent(Event& e) noexcept override;
};
