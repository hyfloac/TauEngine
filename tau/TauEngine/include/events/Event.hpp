#pragma once

#pragma warning(push, 0)
#include <cstdlib>
#include <cstdio>
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

#if TAU_RTTI_DEBUG
    #define EVENT_INTERNAL_TYPE_DECL(TYPE) ("Event::" TAU_RTTI_STRING(TYPE), nullptr)
#else
    #define EVENT_INTERNAL_TYPE_DECL(TYPE)
#endif

#define EVENT_IMPL_BASE(TYPE) \
    DELETE_COPY(TYPE); \
    public: \
        [[nodiscard]] static Event::EventType GetStaticType() noexcept \
        { static Event::EventType type EVENT_INTERNAL_TYPE_DECL(TYPE); \
          return type; } \
        [[nodiscard]] virtual Event::EventType GetEventType() const noexcept override \
        { return TYPE::GetStaticType(); }

#if EVENT_GEN_NAMES
    #define EVENT_IMPL(TYPE) \
        EVENT_IMPL_BASE(TYPE); \
        [[nodiscard]] virtual const c8* GetName() const noexcept override \
        { return u8 ## #TYPE; }
    #define EVENT_GET_NAME(EVENT_PTR) (EVENT_PTR)->GetName()
#else
    #define EVENT_IMPL(TYPE) EVENT_IMPL_BASE(TYPE)
#define EVENT_GET_NAME(EVENT_PTR) u8""
#endif

#define EVENT_INTERCEPTABLE(STATE) \
    [[nodiscard]] virtual bool CanBeIntercepted() const noexcept override \
    { return STATE;  }

class TAU_DLL Event
{
    DEFAULT_DESTRUCT_VI(Event);
    DELETE_COPY(Event);
    DEFAULT_MOVE_PO(Event);
public:
    using EventType = RunTimeType<Event>;
protected:
    Event() noexcept
        : m_Intercepted(false)
    { }
public:
    [[nodiscard]] bool Intercepted() const noexcept { return m_Intercepted; }
    [[nodiscard]] virtual bool CanBeIntercepted() const noexcept { return false; }
    [[nodiscard]] bool Interceptable() const noexcept { return CanBeIntercepted(); }

    [[nodiscard]] virtual Event::EventType GetEventType() const noexcept = 0;

#if EVENT_GEN_NAMES
    [[nodiscard]] virtual const c8* GetName() const noexcept = 0;
    [[nodiscard]] virtual C8DynString ToString() const noexcept { return C8DynString(GetName()); }
#endif

    template<typename TEvent>
    [[nodiscard]] bool IsEventType() const noexcept
    { return TEvent::GetStaticType() == GetEventType(); }
private:
    bool m_Intercepted;
private:
    friend class EventDispatcher;
};

class ExampleEvent final : public Event
{
    DEFAULT_DESTRUCT(ExampleEvent);
public:
    ExampleEvent(const int x = 42) noexcept
        : m_X(x)
    { }

    [[nodiscard]] int X() const noexcept { return m_X; }

    EVENT_IMPL(ExampleEvent);

#if EVENT_GEN_NAMES
    [[nodiscard]] virtual C8DynString ToString() const noexcept override
    {
        c8 buf[12];
        ::std::snprintf(reinterpret_cast<char*>(buf), sizeof(buf), "%d", m_X);
        return C8DynString(GetName()).Concat(buf);
    }
#endif
private:
    int m_X;
};

class EventDispatcher final
{
    DEFAULT_DESTRUCT(EventDispatcher);
    DEFAULT_CM_PU(EventDispatcher);
public:
    EventDispatcher(Event& event) noexcept
        : m_Event(&event)
        , m_TypeCache(event.GetEventType())
    { }

    template<typename TEvent, typename TFunc>
    bool dispatch(const TFunc& func) noexcept
    {
        if(m_Event->Intercepted())
        {
            return false;
        }

        if(m_TypeCache == TEvent::getStaticType())
        {
            const bool intercepted = func(reinterpret_cast<TEvent&>(m_Event));
            if(m_Event->CanBeIntercepted())
            {
                m_Event->m_Intercepted = intercepted;
            }
            return true;
        }

        return false;
    }

    template<typename TEvent, typename TClass, typename TFunction>
    bool dispatch(TClass* instance, const TFunction& func) noexcept
    {
        if(m_Event->Intercepted())
        {
            return false;
        }

        if(m_TypeCache == TEvent::getStaticType())
        {
            const bool intercepted = (instance->*func)(reinterpret_cast<TEvent&>(m_Event));
            if(m_Event->CanBeIntercepted())
            {
                m_Event->m_Intercepted = intercepted;
            }
            return true;
        }

        return false;
    }
private:
    Event* m_Event;
    Event::EventType m_TypeCache;
};
