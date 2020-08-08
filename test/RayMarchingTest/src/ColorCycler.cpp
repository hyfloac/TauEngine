#include "ColorCycler.hpp"

ColorCycler ColorCycler::initRotator(const u8 r, const u8 g, const u8 b, const u8 phaseShift) noexcept
{
    ColorCycler cycler(r, g, b);
    cycler._rotateArgs.index = 0;
    cycler._rotateArgs.phaseShift = phaseShift;
    return cycler;
}

void ColorCycler::rotate() noexcept
{
    switch(_rotateArgs.index)
    {
        case 0:
        {
            iSys r = _r;
            iSys g = _g;
            r -= _rotateArgs.phaseShift;
            g += _rotateArgs.phaseShift;
            if(r <= 0 || g >= 255)
            {
                _r = 0;
                _g = 255;
                _rotateArgs.index = 1;
            }
            else
            {
                _r = r;
                _g = g;
            }
            break;
        }
        case 1:
        {
            iSys g = _g;
            iSys b = _b;
            g -= _rotateArgs.phaseShift;
            b += _rotateArgs.phaseShift;
            if(g <= 0 || b >= 255)
            {
                _g = 0;
                _b = 255;
                _rotateArgs.index = 2;
            }
            else
            {
                _g = g;
                _b = b;
            }
            break;
        }
        case 2:
        {
            iSys b = _b;
            iSys r = _r;
            b -= _rotateArgs.phaseShift;
            r += _rotateArgs.phaseShift;
            if(b <= 0 || r >= 255)
            {
                _b = 0;
                _r = 255;
                _rotateArgs.index = 0;
            }
            else
            {
                _b = b;
                _r = r;
            }
            break;
        }
        default:
            _rotateArgs.index = 0;
            break;
    }
}
