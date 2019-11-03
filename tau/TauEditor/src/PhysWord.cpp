#include "PhysWord.hpp"

#include <cmath>
#include <Utils.hpp>
#include <FastRand.hpp>

template<typename _T>
_T sgn(_T val)
{
    return (_T(0) < val) - (val < _T(0));
}

PhysWord::PhysWord(const char* str, const Window& window) noexcept
    : _str(str), _length(0.0f),
      _textPos(0.0f), _textVel(0.0f), _textAcc(0.0f, 0.0f),
      _color(0.0f), _colorVel(0.0f)
{
    randomReset(window);
}

void PhysWord::render(const float delta, RenderingPipeline& rp, TextHandler& th, const glm::mat4& ortho) noexcept
{
    UNUSED(delta);
    if(_length == 0.0f)
    {
        _length = th.computeLength(_str, _textScale);
    }
    rp.pushRenderText(&th, _str, _textPos.x(), _textPos.y(), _textScale, static_cast<u8>(_color.x() * 255), static_cast<u8>(_color.y() * 255), static_cast<u8>(_color.z() * 255), ortho);
}

void PhysWord::update(const float fixedDelta, const Window& window) noexcept
{
    const float maxX = static_cast<float>(window.width() - _length);
    const float maxY = static_cast<float>(window.height() - 15 * _offsetFactor);

    [[maybe_unused]] Mouse::Pos pos = Mouse::mousePos();

    // const Vector2f mousePos(mouseX, static_cast<i32>(window.height()) - mouseY);
    // const float distance = sqrtf(mousePos.dot(_textPos));
    //
    // _textAcc = mousePos - _textPos;
    // _textAcc *= 0.0000005f * distance;

    phaseColor();
    constexpr float gravity = -0.01f;
    constexpr float energyFactor = 0.8f;

    const float fixedDeltaMS = fixedDelta / 1000.0f;
    constexpr float hEnergyFactor = 0.99995f;
    constexpr float hFrictionFactor = 0.8f;

    const float mass = _length / 32.0f;
    const float forceY = mass * gravity;

    _textAcc.y() = forceY / mass;

    // _textVel.y() += gravity * fixedDeltaMS;
    _textVel += _textAcc * fixedDeltaMS;

    if(abs(_textVel.y()) < 0.2f && _textPos.y() <= 5.0f)
    {
        _textVel.x() *= hFrictionFactor * fixedDeltaMS * 0.032f;
        if(abs(_textVel.x()) < 0.2f)
        {
            randomReset(window);
        }
    }
    else
    {
        _textVel.x() *= hEnergyFactor * fixedDeltaMS * 0.032f;
    }

    _textPos.add(_textVel);

    if(_textPos.x() <= 0.0f)
    {
        _textVel.x() = _textVel.x() * sgn(_textVel.x()) * 1.0f * energyFactor;
        _textPos.x() = 0.0f;
    }
    if(_textPos.x() >= maxX)
    {
        _textVel.x() = _textVel.x() * sgn(_textVel.x()) * -1.0f * energyFactor;
        _textPos.x() = maxX;
    }
    if(_textPos.y() <= 0.0f)
    {
        _textVel.y() = _textVel.y() * sgn(_textVel.y()) * 1.0f * energyFactor;
        _textPos.y() = 0.0f;
    }
    if(_textPos.y() >= maxY)
    {
        _textVel.y() = _textVel.y() * sgn(_textVel.y()) * -1.0f * energyFactor;
        _textPos.y() = maxY;
    }
}

void PhysWord::randomReset(const Window& window) noexcept
{
    _textPos.x() = fastRand() % (window.width() - static_cast<int>(_length));
    _textPos.y() = fastRand() % (window.height() - int(15 * _offsetFactor));
    _textVel.x() = (fastRand() % 100) / 10.0f - 5.0f;
    _textVel.y() = 0.0f;
    _textAcc.x() = 0.0f;
    _textAcc.y() = -0.01f;

    _color.x() = (fastRand() % 512) / 512.0f;
    _color.y() = (fastRand() % 512) / 512.0f;
    _color.z() = (fastRand() % 512) / 512.0f;

    _colorVel.x() = (fastRand() % 100) / 9000.0f;
    _colorVel.y() = (fastRand() % 100) / 9000.0f;
    _colorVel.z() = (fastRand() % 100) / 9000.0f;
}

void PhysWord::phaseColor() noexcept
{
    _color += _colorVel;

#define CLAMP(_COMP, _VAL) _color._COMP() = _VAL; _colorVel._COMP() *= -1.0f

    if(_color.x() > 1.0f) { CLAMP(x, 1.0f); }
    if(_color.y() > 1.0f) { CLAMP(y, 1.0f); }
    if(_color.z() > 1.0f) { CLAMP(z, 1.0f); }

    if(_color.x() < 0.0f) { CLAMP(x, 0.0f); }
    if(_color.y() < 0.0f) { CLAMP(y, 0.0f); }
    if(_color.z() < 0.0f) { CLAMP(z, 0.0f); }
}
