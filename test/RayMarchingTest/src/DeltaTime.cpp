#include "DeltaTime.hpp"

DeltaTime::DeltaTime() noexcept
    : _us(0.0f), _ms(0.0f), _s(0.0f),
    _usSinceLastUpdate(0.0f), _msSinceLastUpdate(0.0f), _sSinceLastUpdate(0.0f),
    _usSinceLaunch(0.0f), _msSinceLaunch(0.0f), _sSinceLaunch(0.0f)
{ }

void DeltaTime::setDeltaMicro(float us) noexcept
{
    _us = us;
    _ms = _us / 1000.0f;
    _s = _us / 1000000.0f;
    _usSinceLastUpdate += us;
    _msSinceLastUpdate = _usSinceLastUpdate / 1000.0f;
    _sSinceLastUpdate = _usSinceLastUpdate / 1000000.0f;
    _usSinceLaunch += us;
    _msSinceLaunch = _usSinceLaunch / 1000.0f;
    _sSinceLaunch = _usSinceLaunch / 1000000.0f;
}

void DeltaTime::onUpdate() noexcept
{
    _usSinceLastUpdate = 0.0f;
    _msSinceLastUpdate = 0.0f;
    _sSinceLastUpdate = 0.0f;
}
