#pragma once

#include <Objects.hpp>
#include "DLL.hpp"
#include "Timings.hpp"


#ifndef PP_LAYER_GEN_NAMES
  #if defined(TAU_PRODUCTION)
    #define PP_LAYER_GEN_NAMES 0
  #else
    #define PP_LAYER_GEN_NAMES 1
  #endif
#endif

#define PP_LAYER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE)

#if PP_LAYER_GEN_NAMES
  #define PP_LAYER_IMPL(_TYPE) PP_LAYER_IMPL_BASE(_TYPE); \
                            public: \
                                [[nodiscard]] virtual const char* getName() const noexcept override \
                                { return #_TYPE; }
  #define PP_LAYER_IMPL_NAME(_TYPE, _NAME) PP_LAYER_IMPL_BASE(_TYPE); \
                            public: \
                                [[nodiscard]] virtual const char* getName() const noexcept override \
                                { return _NAME; }
  #define PP_LAYER_GET_NAME(_LAYER_PTR) (_LAYER_PTR)->getName()
#else
  #define PP_LAYER_IMPL(_TYPE) PP_LAYER_IMPL_BASE(_TYPE)
  #define PP_LAYER_IMPL_NAME(_TYPE, _NAME) PP_LAYER_IMPL_BASE(_TYPE)
  #define PP_LAYER_GET_NAME(_LAYER_PTR) ""
#endif

class Event;

class TAU_DLL TAU_NOVTABLE IPostProcessLayer
{
    DEFAULT_DESTRUCT_VI(IPostProcessLayer);
    DELETE_CM(IPostProcessLayer);
protected:
    bool _active;
protected:
    IPostProcessLayer(bool active = true) noexcept
        : _active(active)
    { }
public:
    [[nodiscard]] bool active() const noexcept { return _active; }
    virtual bool active(const bool ac) noexcept { return _active = ac; }

    virtual void onUpdate(float fixedDelta) noexcept { }
    virtual void onEvent(Event& e) noexcept { }

    virtual void onFrameBufferBind() noexcept { }
    virtual void onFrameBufferUnbind() noexcept { }

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

#if PP_LAYER_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif
};
