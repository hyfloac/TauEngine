#pragma once

#include <Objects.hpp>

#ifndef LAYER_GEN_NAMES
  #if defined(TAU_PRODUCTION)
    #define LAYER_GEN_NAMES 0
  #else
    #define LAYER_GEN_NAMES 1
  #endif
#endif

#define LAYER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE)

#if LAYER_GEN_NAMES
  #define LAYER_IMPL(_TYPE) LAYER_IMPL_BASE(_TYPE); \
                            public: \
                                [[nodiscard]] virtual const char* getName() const noexcept override \
                                { return #_TYPE; }
  #define LAYER_GET_NAME(_EVENT_PTR) (_EVENT_PTR)->getName()
#else
  #define LAYER_IMPL(_TYPE) LAYER_IMPL_BASE(_TYPE)
  #define LAYER_GET_NAME(_EVENT_PTR) ""
#endif

class Event;

class ILayer
{
    DEFAULT_CONSTRUCT_PO(ILayer);
    DEFAULT_DESTRUCT_VI(ILayer);
    DELETE_COPY(ILayer);
public:
    virtual void onAttach() noexcept { }
    virtual void onDetach() noexcept { }
    virtual void onUpdate(float fixedDelta) noexcept { }
    virtual void onRender(float delta) noexcept { }
    virtual void onEvent(Event& e) noexcept { }

#if LAYER_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif
};
