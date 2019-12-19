#include "PhysWordLayer.hpp"

PhysWordLayer::PhysWordLayer(size_t wordCount, const char* word, Window& window, TextHandler& th, const GlyphSetHandle& glyphSetHandle, RenderingPipeline& rp, const glm::mat4& ortho, State& state) noexcept
    : ILayer(true),
      _window(window), _th(th), _glyphSetHandle(glyphSetHandle), _rp(rp), _ortho(ortho), _state(state),
      _physWords()
{
    for(size_t i = 0; i < wordCount; ++i)
    {
        _physWords.emplace_back(word, _window);
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

void PhysWordLayer::onRender(const DeltaTime& delta) noexcept
{
    if(_state != State::ConsoleExclusive)
    {
        for(auto& physWord : _physWords)
        {
            if(_glyphSetHandle != -1)
            {
                physWord.render(delta.microseconds(), _rp, _th, _glyphSetHandle, _ortho);
            }
        }
    }
}

void PhysWordLayer::onEvent(Event& e) noexcept
{
    UNUSED(e);
    // EventDispatcher dispatcher(e);
}
