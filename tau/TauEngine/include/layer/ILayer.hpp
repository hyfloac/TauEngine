#pragma once

#ifndef LAYER_GEN_NAMES
  #define LAYER_GEN_NAMES !defined(TAU_PRODUCTION)
#endif

#define LAYER_IMPL_BASE(_TYPE) private: \
                                   _TYPE(const _TYPE& copy) = delete;                     \
                                   _TYPE(_TYPE&& move) noexcept = delete;                 \
                                   _TYPE& operator=(const _TYPE& copy) = delete;          \
                                   _TYPE& operator=(_TYPE&& move) noexcept = delete;

#if EVENT_GEN_NAMES
#define LAYER_IMPL(_TYPE) LAYER_IMPL_BASE(_TYPE); \
                          public: \
                              [[nodiscard]] virtual const char* getName() const noexcept override \
                              { return #_TYPE; }
#else
#define LAYER_IMPL(_TYPE) LAYER_IMPL_BASE(_TYPE)
#endif

class Event;

class ILayer
{
protected:
    inline ILayer() noexcept = default;
private:
    ILayer(const ILayer& copy) noexcept = delete;
    ILayer(ILayer&& move) noexcept = delete;

    ILayer& operator=(const ILayer& copy) noexcept = delete;
    ILayer& operator=(ILayer&& move) noexcept = delete;
public:
    virtual ~ILayer() noexcept = default;

    virtual void onAttach() noexcept { }
    virtual void onDetach() noexcept { }
    virtual void onUpdate(float delta) noexcept { }
    virtual void onEvent(Event& e) noexcept { }

#if LAYER_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif
};
