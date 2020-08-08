#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>

class ColorCycler final
{
    DEFAULT_DESTRUCT(ColorCycler);
    DEFAULT_CM_PU(ColorCycler);
public:
    struct RotateArgs final
    {
        uSys index;
        iSys phaseShift;
    };

    static ColorCycler initRotator(u8 r, u8 g, u8 b, u8 phaseShift) noexcept;
private:
    u8 _r;
    u8 _g;
    u8 _b;

    union
    {
        RotateArgs _rotateArgs;
        uSys _compressed;
    };
public:
    ColorCycler() noexcept
        : _r(0)
        , _g(0)
        , _b(0)
        , _compressed(0)
    { }

    ColorCycler(const u8 r, const u8 g, const u8 b) noexcept
        : _r(r)
        , _g(g)
        , _b(b)
        , _compressed(0)
    { }

    [[nodiscard]] u8 r() const noexcept { return _r; }
    [[nodiscard]] u8 g() const noexcept { return _g; }
    [[nodiscard]] u8 b() const noexcept { return _b; }

    [[nodiscard]] u8& r() noexcept { return _r; }
    [[nodiscard]] u8& g() noexcept { return _g; }
    [[nodiscard]] u8& b() noexcept { return _b; }

    [[nodiscard]] float rf() const noexcept { return static_cast<float>(_r) / 255.0f; }
    [[nodiscard]] float gf() const noexcept { return static_cast<float>(_g) / 255.0f; }
    [[nodiscard]] float bf() const noexcept { return static_cast<float>(_b) / 255.0f; }

    void rf(const float r) noexcept { _r = static_cast<u8>(r * 255.0f); }
    void gf(const float g) noexcept { _g = static_cast<u8>(g * 255.0f); }
    void bf(const float b) noexcept { _b = static_cast<u8>(b * 255.0f); }

    void rotate() noexcept;
};
