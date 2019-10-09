#include "PhysWordLayer.hpp"

PhysWordLayer::PhysWordLayer(size_t wordCount, const char* word, Window& window, TextHandler& th, RenderingPipeline& rp, Matrix4x4f& ortho) noexcept
    : _window(window), _th(th), _rp(rp), _ortho(ortho),
      _physWords()
{
    for(size_t i = 0; i < wordCount; ++i)
    {
        _physWords.emplace_back(word, _window, _th);
    }
}

void PhysWordLayer::onUpdate(float fixedDelta) noexcept
{
    for(auto& physWord : _physWords)
    {
        physWord.update(fixedDelta, _window);
    }
}

void PhysWordLayer::onRender(float delta) noexcept
{
    for(auto& physWord : _physWords)
    {
        physWord.render(delta, _rp, _th, _ortho);
    }
}

void PhysWordLayer::onEvent(Event& e) noexcept
{
    UNUSED(e);
    // EventDispatcher dispatcher(e);
}
