#pragma once

#pragma warning(push, 0)
#include <cstdlib>
#pragma warning(pop)

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <String.hpp>

#ifndef EVENT_GEN_NAMES
  #define EVENT_GEN_NAMES !defined(TAU_PRODUCTION)
#endif

#define EVENT_IMPL_BASE(_TYPE) private: \
                                   _TYPE(const _TYPE& copy) = delete;                     \
                                   _TYPE(_TYPE&& move) noexcept = delete;                 \
                                   _TYPE& operator=(const _TYPE& copy) = delete;          \
                                   _TYPE& operator=(_TYPE&& move) noexcept = delete;      \
                               public: \
                                   [[nodiscard]] static Event::EventType getStaticType() noexcept \
                                   { static Event::EventType type = Event::EventType::define(); \
                                     return type; } \
                                   [[nodiscard]] virtual Event::EventType getEventType() const noexcept override \
                                   { return _TYPE::getStaticType(); }

#if EVENT_GEN_NAMES
#define EVENT_IMPL(_TYPE) EVENT_IMPL_BASE(_TYPE); \
                          [[nodiscard]] virtual const char* getName() const noexcept override \
                          { return #_TYPE; }
#else
#define EVENT_IMPL(_TYPE) EVENT_IMPL_BASE(_TYPE)
#endif
                               
#if EVENT_GEN_NAMES
#define EVENT_IMPL_INTERCEPTABLE(_TYPE) EVENT_IMPL(_TYPE); \
                                        [[nodiscard]] virtual bool canBeIntercepted() const noexcept override \
                                        { return true;  }
#else
#define EVENT_IMPL_INTERCEPTABLE(_TYPE) EVENT_IMPL(_TYPE) \
                                        [[nodiscard]] virtual bool canBeIntercepted() const noexcept override \
                                        { return true;  }
#endif

class TAU_DLL Event
{
public:
    friend class EventDispatcher;

    class EventType final
    {
    public:
        static EventType define() noexcept
        {
            static u64 currentUID = 0;
            return EventType(++currentUID);
        }
    private:
        u64 _uid;

        inline EventType(u64 uid) noexcept
            : _uid(uid)
        { }
    public:
        inline ~EventType() noexcept = default;

        EventType(const EventType& copy) = default;
        EventType(EventType&& move) noexcept = default;

        EventType& operator=(const EventType& copy) = default;
        EventType& operator=(EventType&& move) noexcept = default;

        [[nodiscard]] inline u64 uid() const noexcept { return _uid; }
        inline operator u64() const noexcept { return _uid; }
        inline u64 operator ()() const noexcept { return _uid; }

        inline bool operator ==(const EventType& other) const noexcept { return _uid == other._uid; }
        inline bool operator !=(const EventType& other) const noexcept { return _uid != other._uid; }

        inline bool operator ==(const u64 uid) const noexcept { return _uid == uid; }
        inline bool operator !=(const u64 uid) const noexcept { return _uid != uid; }
    };
private:
    bool _intercepted;
private:
    Event(const Event& copy) = delete;
    Event(Event&& move) noexcept = delete;

    Event& operator=(const Event& copy) = delete;
    Event& operator=(Event&& move) noexcept = delete;
protected:
    inline Event() noexcept
        : _intercepted(false)
    { }
public:
    inline virtual ~Event() noexcept = default;

    [[nodiscard]] inline bool intercepted() const noexcept { return _intercepted; }
    [[nodiscard]] virtual bool canBeIntercepted() const noexcept { return false; }

    [[nodiscard]] virtual EventType getEventType() const noexcept = 0;

#if defined(TAU_PRODUCTION) || 1
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
    [[nodiscard]] virtual DynString toString() const noexcept { return DynString(getName()); }
#endif

    template<typename _T>
    [[nodiscard]] bool isEventType() const noexcept
    { return _T::getStaticType() == getEventType(); }
};

class ExampleEvent final : public Event
{
private:
    int _x;
public:
    inline ExampleEvent(int x = 42) noexcept
        : _x(x)
    { }

    inline virtual ~ExampleEvent() noexcept = default;

    [[nodiscard]] int x() const noexcept { return _x; }

    EVENT_IMPL(ExampleEvent);

#if EVENT_GEN_NAMES
    [[nodiscard]] virtual DynString toString() const noexcept override
    {
        char buf[12];
        _itoa_s(_x, buf, 10);
        DynString ret(getName());
        ret.append(buf);
        return ret;
    }
#endif
};

class EventDispatcher final
{
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
};
