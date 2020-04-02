#pragma once

#include <Objects.hpp>
#include "Timings.hpp"

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
protected:
    bool _visible;
protected:
    ILayer(bool visible = true) noexcept
        : _visible(visible)
    { }
public:
    [[nodiscard]] bool visible() const noexcept { return _visible; }
    virtual bool visible(bool vis) noexcept { return _visible = vis; }

    virtual void onAttach() noexcept { }
    virtual void onDetach() noexcept { }

    virtual void onUpdate(float fixedDelta) noexcept { }
    virtual void onEvent(Event& e) noexcept { }

    /**
     *   This should be used to update the state of any world
     * objects including lerping and camera rotation. This is
     * called once per render cycle.
     */
    virtual void onPreRender(const DeltaTime& delta) noexcept { }
    /**
     *   This should only be used to render the scene. Any updating
     * of the world objects should be done in
     * {@link onPreRender(const DeltaTime&) noexcept @endlink}.
     * This may be called multiple times in a render cycle. This
     * is done for the use of multiple render outputs like in VR.
     */
    virtual void onRender() noexcept { }
    /**
     *   This should be used to update the state of any objects
     * you want after {@link onRender() noexcept @endlink}.
     * This is called once per render cycle.
     */
    virtual void onPostRender() noexcept { }

#if LAYER_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif
};
