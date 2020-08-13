#include "DeltaTime.hpp"

DeltaTime::DeltaTime() noexcept
    : _us(0.0)
    , _ms(0.0)
    , _s(0.0)
    , _usSinceLastUpdate(0.0)
    , _msSinceLastUpdate(0.0)
    , _sSinceLastUpdate(0.0)
    , _usSinceLaunch(0.0)
    , _msSinceLaunch(0.0)
    , _sSinceLaunch(0.0)
{ }

void DeltaTime::setDeltaMicro(const double us) noexcept
{
    _us = us;
    _ms = _us / 1000.0;
    _s = _us / 1000000.0;
    _usSinceLastUpdate += us;
    _msSinceLastUpdate = _usSinceLastUpdate / 1000.0;
    _sSinceLastUpdate = _usSinceLastUpdate / 1000000.0;
    _usSinceLaunch += us;
    _msSinceLaunch = _usSinceLaunch / 1000.0;
    _sSinceLaunch = _usSinceLaunch / 1000000.0;
}

void DeltaTime::onUpdate() noexcept
{
    _usSinceLastUpdate = 0.0;
    _msSinceLastUpdate = 0.0;
    _sSinceLastUpdate = 0.0;
}
