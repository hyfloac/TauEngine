#pragma once

#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <TextHandler.hpp>

class PhysWord final
{
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    const char* _str;
    float _length;

    static constexpr float _textScale = 0.5f;
    static constexpr float _offsetFactor = _textScale * textScaleConverter;
    Vector2f _textPos;
    Vector2f _textVel;
    Vector2f _textAcc;
    Vector3f _color;
    Vector3f _colorVel;
public:
    PhysWord(const char* str, const Window& window) noexcept;

    void render(const float delta, RenderingPipeline& rp, TextHandler& th, const glm::mat4& ortho) noexcept;

    void update(const float fixedDelta, const Window& window) noexcept;

    void randomReset(const Window& window) noexcept;

    void phaseColor() noexcept;
};
