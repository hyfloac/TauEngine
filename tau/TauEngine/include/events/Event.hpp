#pragma once

#pragma warning(push, 0)
#include <cstdlib>
#pragma warning(pop)

#include <DLL.hpp>
#include <String.hpp>
#include <RunTimeType.hpp>
#include <Objects.hpp>

#ifndef EVENT_GEN_NAMES
  #ifndef TAU_PRODUCTION
    #define EVENT_GEN_NAMES 1
  #else
    #define EVENT_GEN_NAMES 0
  #endif
#endif

#if _DEBUG
#define EVENT_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                               public: \
                                   [[nodiscard]] static Event::EventType getStaticType() noexcept \
                                   { static Event::EventType type(TAU_RTTI_STRING(_TYPE), nullptr); \
                                     return type; } \
                                   [[nodiscard]] virtual Event::EventType getEventType() const noexcept override \
                                   { return _TYPE::getStaticType(); }
#else
#define EVENT_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                               public: \
                                   [[nodiscard]] static Event::EventType getStaticType() noexcept \
                                   { static Event::EventType type = Event::EventType::define(); \
                                     return type; } \
                                   [[nodiscard]] virtual Event::EventType getEventType() const noexcept override \
                                   { return _TYPE::getStaticType(); }
#endif

#if EVENT_GEN_NAMES
  #define EVENT_IMPL(_TYPE) EVENT_IMPL_BASE(_TYPE); \
                            [[nodiscard]] virtual const char* getName() const noexcept override \
                            { return #_TYPE; }
  #define EVENT_GET_NAME(_EVENT_PTR) (_EVENT_PTR)->getName()
#else
  #define EVENT_IMPL(_TYPE) EVENT_IMPL_BASE(_TYPE)
  #define EVENT_GET_NAME(_EVENT_PTR) ""
#endif

#define EVENT_INTERCEPTABLE(_STATE) [[nodiscard]] virtual bool canBeIntercepted() const noexcept override \
                                    { return _STATE;  }

class TAU_DLL Event
{
    DEFAULT_DESTRUCT_VI(Event);
    DELETE_COPY(Event);
public:
    using EventType = RunTimeType<Event>;
private:
    bool _intercepted;
protected:
    inline Event() noexcept
        : _intercepted(false)
    { }
public:
    [[nodiscard]] inline bool intercepted() const noexcept { return _intercepted; }
    [[nodiscard]] virtual bool canBeIntercepted() const noexcept { return false; }
    [[nodiscard]] bool interceptable() const noexcept { return canBeIntercepted(); }

    [[nodiscard]] virtual Event::EventType getEventType() const noexcept = 0;

#if EVENT_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
    [[nodiscard]] virtual DynString toString() const noexcept { return DynString(getName()); }
#endif

    template<typename _T>
    [[nodiscard]] bool isEventType() const noexcept
    { return _T::getStaticType() == getEventType(); }
private:
    friend class EventDispatcher;
};

class ExampleEvent final : public Event
{
    DEFAULT_DESTRUCT(ExampleEvent);
private:
    int _x;
public:
    inline ExampleEvent(int x = 42) noexcept
        : _x(x)
    { }

    [[nodiscard]] int x() const noexcept { return _x; }

    EVENT_IMPL(ExampleEvent);

#if EVENT_GEN_NAMES
    [[nodiscard]] virtual DynString toString() const noexcept override
    {
        char buf[12];
        _itoa_s(_x, buf, 10);
        return DynString(getName()).concat(buf);
    }
#endif
};

class EventDispatcher final
{
    DEFAULT_DESTRUCT(EventDispatcher);
    DEFAULT_CM_PU(EventDispatcher);
private:
    Event& _event;
    Event::EventType _cache;
public:
    inline EventDispatcher(Event& event) noexcept
        : _event(event), _cache(event.getEventType())
    { }

    template<typename _T, typename _F>
    inline bool dispatch(const _F& func) noexcept
    {
        if(_event.intercepted()) { return false; }
        if(_cache == _T::getStaticType())
        {
            const bool intercepted = func(reinterpret_cast<_T&>(_event));
            if(_event.canBeIntercepted())
            {
                _event._intercepted = intercepted;
            }
            return true;
        }
        return false;
    }

    template<typename _T, typename _C, typename _F>
    inline bool dispatch(_C* instance, const _F& func) noexcept
    {
        if(_event.intercepted()) { return false; }
        if(_cache == _T::getStaticType())
        {
            const bool intercepted = (instance->*func)(reinterpret_cast<_T&>(_event));
            if(_event.canBeIntercepted())
            {
                _event._intercepted = intercepted;
            }
            return true;
        }
        return false;
    }
};
