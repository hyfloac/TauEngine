#pragma once

#include <layer/ILayer.hpp>
#include <PhysWord.hpp>

class PhysWordLayer final : public ILayer
{
private:
    Window& _window;
    TextHandler& _th;
    RenderingPipeline& _rp;
    glm::mat4& _ortho;

    std::vector<PhysWord> _physWords;
public:
    PhysWordLayer(size_t wordCount, const char* word, Window& window, TextHandler& th, RenderingPipeline& rp, glm::mat4& ortho) noexcept;

    ~PhysWordLayer() noexcept override = default;

    void onUpdate(float fixedDelta) noexcept override;

    void onRender(float delta) noexcept override;

    void onEvent(Event& e) noexcept override;

    LAYER_IMPL(PhysWordLayer);
};
