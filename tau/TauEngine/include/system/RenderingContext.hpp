/**
 * @file 
 */
#pragma once

#include <NumTypes.hpp>
#include <RunTimeType.hpp>
#include "Color.hpp"
#include "DLL.hpp"
#include "events/Exception.hpp"
#include "RenderingMode.hpp"

#define RC_IMPL_BASE(_TYPE) private: \
                                _TYPE(const _TYPE& copy) noexcept = delete;            \
                                _TYPE(_TYPE&& move) noexcept = delete;                 \
                                _TYPE& operator=(const _TYPE& copy) noexcept = delete; \
                                _TYPE& operator=(_TYPE&& move) noexcept = delete;      \
                            public: \
                                [[nodiscard]] static _TYPE::ContextType getStaticType() noexcept \
                                { static IRenderingContext::ContextType type = IRenderingContext::ContextType::define(); \
                                  return type; } \
                                [[nodiscard]] virtual IRenderingContext::ContextType getContextType() const noexcept override \
                                { return _TYPE::getStaticType(); }

#define RC_IMPL(_TYPE) RC_IMPL_BASE(_TYPE)

struct GLContextSettings;

class TAU_DLL IRenderingContext
{
public:
    static IRenderingContext* create(const RenderingMode& mode, GLContextSettings) noexcept;

    using ContextType = RunTimeType<IRenderingContext>;
private:
    IRenderingContext(const IRenderingContext& copy) = delete;
    IRenderingContext(IRenderingContext&& move) = delete;

    IRenderingContext& operator =(const IRenderingContext& copy) = delete;
    IRenderingContext& operator =(IRenderingContext&& move) = delete;
protected:
    IRenderingContext() = default;
public:
    virtual ~IRenderingContext() = default;

    [[nodiscard]] virtual IRenderingContext::ContextType getContextType() const noexcept = 0;

    virtual void createContext(void* param) = 0;

    virtual void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) = 0;

    virtual void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) = 0;
    
    template<typename _T>
    [[nodiscard]] bool isEventType() const noexcept
    { return _T::getStaticType() == getContextType(); }
};

class IncorrectContextException final : public Exception
{
public:
    IncorrectContextException() noexcept = default;
    ~IncorrectContextException() noexcept = default;

    EXCEPTION_IMPL(IncorrectContextException);
};

struct GLContextSettings final
{
    u8 majorVersion;
    u8 minorVersion;
    union
    {
        struct
        {
            bool debug : 1;
            bool forwardCompatible : 1;
            bool coreProfile : 1;
            bool compatProfile : 1;
            u8 u0 : 1, u1 : 1, u2 : 1, u3 : 1;
        };
        u8 _compressed;
    };
};

// TAU_DLL IRenderingContext* createGLContext(GLContextSettings settings) noexcept;

// TAU_DLL IRenderingContext* createDXContext() noexcept;
