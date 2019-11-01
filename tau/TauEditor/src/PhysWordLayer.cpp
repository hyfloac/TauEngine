#include "PhysWordLayer.hpp"

PhysWordLayer::PhysWordLayer(size_t wordCount, const char* word, Window& window, TextHandler& th, RenderingPipeline& rp, const glm::mat4& ortho, State& state) noexcept
    : ILayer(true),
      _window(window), _th(th), _rp(rp), _ortho(ortho), _state(state),
      _physWords()
{
    for(size_t i = 0; i < wordCount; ++i)
    {
        _physWords.emplace_back(word, _window, _th);
    }
}

void PhysWordLayer::onUpdate(float fixedDelta) noexcept
{
    if(_state == State::Game)
    {
        for(auto& physWord : _physWords)
        {
            physWord.update(fixedDelta, _window);
        }
    }
}

void PhysWordLayer::onRender(float delta) noexcept
{
    if(_state != State::ConsoleExclusive)
    {
        for(auto& physWord : _physWords)
        {
            physWord.render(delta, _rp, _th, _ortho);
        }
    }
}

void PhysWordLayer::onEvent(Event& e) noexcept
{
    UNUSED(e);
    // EventDispatcher dispatcher(e);
}
