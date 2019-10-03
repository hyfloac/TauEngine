#pragma once

#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <Utils.hpp>
#include <TextHandler.hpp>
#include <cmath>

template<typename _T>
int sgn(_T val)
{
    return (_T(0) < val) - (val < _T(0));
}

static unsigned int g_seed;

// Used to seed the generator.           
static inline void fast_srand(int seed)
{
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
static inline int fast_rand()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

class PhysWord final
{
private:
    static constexpr float textScaleConverter = 2.8571428571428571428571428571429f;

    const char* _str;
    float _length;
    Window& _window;
    TextHandler& _th;
    RenderingPipeline& _rp;

    static constexpr float _textScale = 0.5f;
    static constexpr float _offsetFactor = _textScale * textScaleConverter;
    Vector2f _textPos;
    Vector2f _textVel;
    Vector3f _color;
    Vector3f _colorVel;
public:
    PhysWord(const char* str, Window& window, TextHandler& th, RenderingPipeline& rp) noexcept
        : _str(str), _length(th.computeLength(str, _textScale)), _window(window), _th(th), _rp(rp),
          _textPos(0.0f), _textVel(0.0f), _color(0.0f), _colorVel(0.0f)
    {
        randomReset();
    }

    void render(const float delta, Matrix4x4f& ortho) noexcept
    {
        UNUSED(delta);
        _rp.pushRenderText(&_th, _str, _textPos.x(), _textPos.y(), _textScale, _color, ortho);
    }

    void update(const float fixedDelta) noexcept
    {
        const float maxX = static_cast<float>(_window.width() - _length);
        const float maxY = static_cast<float>(_window.height() - 15 * _offsetFactor);

        phaseColor();
        constexpr float gravity = -0.01f;
        constexpr float energyFactor = 0.8f;

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

        const float fixedDeltaMS = fixedDelta / 1000.0f;
        constexpr float hEnergyFactor = 0.99995f;
        constexpr float hFrictionFactor = 0.8f;

        _textVel.y() += gravity * fixedDeltaMS;

        if(abs(_textVel.y()) < 0.2f && _textPos.y() <= 5.0f)
        {
            _textVel.x() *= hFrictionFactor * fixedDeltaMS * 0.032f;
            if(abs(_textVel.x()) < 0.2f)
            {
                randomReset();
            }
        }
        else
        {
            _textVel.x() *= hEnergyFactor * fixedDeltaMS * 0.032f;
        }

        _textPos.add(_textVel);
    }

    void randomReset() noexcept
    {
        _textPos.x() = fast_rand() % (_window.width() - (int) _length);
        _textPos.y() = fast_rand() % (_window.height() - int(15 * _offsetFactor));
        _textVel.x() = (fast_rand() % 100) / 10.0f - 5.0f;
        _textVel.y() = 0.0f;

        _color.x() = (fast_rand() % 512) / 512.0f;
        _color.y() = (fast_rand() % 512) / 512.0f;
        _color.z() = (fast_rand() % 512) / 512.0f;

        _colorVel.x() = (fast_rand() % 100) / 9000.0f;
        _colorVel.y() = (fast_rand() % 100) / 9000.0f;
        _colorVel.z() = (fast_rand() % 100) / 9000.0f;

        _colorVel.z() = pow(_colorVel.z(), (fast_rand() % 100) / 100.0f);
    }

    void phaseColor() noexcept
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
};
