#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>

#ifndef UI_ELEMENT_GEN_NAMES
  #if defined(TAU_PRODUCTION)
    #define UI_ELEMENT_GEN_NAMES 0
  #else
    #define UI_ELEMENT_GEN_NAMES 1
  #endif
#endif

#define UI_ELEMENT_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE)

#if UI_ELEMENT_GEN_NAMES
  #define UI_ELEMENT_IMPL(_TYPE) UI_ELEMENT_IMPL_BASE(_TYPE); \
                                 public: \
                                     [[nodiscard]] virtual const char* getName() const noexcept override \
                                     { return #_TYPE; }
  #define UI_ELEMENT_GET_NAME(_EVENT_PTR) (_EVENT_PTR)->getName()
#else
  #define UI_ELEMENT_IMPL(_TYPE) UI_ELEMENT_IMPL_BASE(_TYPEs)
  #define UI_ELEMENT_GET_NAME(_EVENT_PTR) ""
#endif

class Event;

class UIElement
{
    DEFAULT_DESTRUCT_VI(UIElement);
    DELETE_COPY(UIElement);
protected:
    u32 _x;
    u32 _y;
    UIElement* _parent;
    bool _visible;
protected:
    UIElement(u32 x, u32 y, UIElement* parent = nullptr, bool visible = true) noexcept
        : _x(x), _y(y), _parent(parent), _visible(visible)
    { }
public:
    [[nodiscard]] u32 x() const noexcept { return _x; }
    [[nodiscard]] u32& x() noexcept { return _x; }
    [[nodiscard]] u32 y() const noexcept { return _y; }
    [[nodiscard]] u32& y() noexcept { return _y; }

    [[nodiscard]] const UIElement* parent() const noexcept { return _parent; }
    [[nodiscard]] UIElement*& parent() noexcept { return _parent; }

    [[nodiscard]] bool visible() const noexcept { return _visible; }
    virtual bool visible(bool vis) noexcept { return _visible = vis; }

    virtual void onUpdate(float fixedDelta) noexcept { }
    virtual void onRender(float delta) noexcept { }
    virtual void onEvent(Event& e) noexcept { }

#if UI_ELEMENT_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif
};
