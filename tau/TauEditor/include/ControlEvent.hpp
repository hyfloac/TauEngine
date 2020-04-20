#pragma once

#include <events/Event.hpp>

class ControlEvent final : public Event
{
    DEFAULT_DESTRUCT(ControlEvent);
    EVENT_IMPL(ControlEvent);
private:
    uSys _type;
    void* _arg;
public:
    ControlEvent(const uSys type, void* const arg = null) noexcept
        : _type(type)
        , _arg(arg)
    { }

    [[nodiscard]] uSys type() const noexcept { return _type; }
    [[nodiscard]] void* arg() const noexcept { return _arg; }
};

#define CE_SET_NEXT_FB 1
#define CE_ACTIVATE_SAT_FB 2
#define CE_SET_SATURATION 3
