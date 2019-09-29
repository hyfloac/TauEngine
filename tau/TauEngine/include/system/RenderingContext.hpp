#pragma once

#include <NumTypes.hpp>
#include <Color.hpp>
#include <DLL.hpp>

class TAU_DLL IRenderingContext
{
public:
    IRenderingContext() = default;

    virtual ~IRenderingContext() = default;

    IRenderingContext(const IRenderingContext& copy) = delete;
    IRenderingContext(IRenderingContext&& move) = delete;

    IRenderingContext& operator =(const IRenderingContext& copy) = delete;
    IRenderingContext& operator =(IRenderingContext&& move) = delete;

    virtual void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) = 0;

    virtual void createContext(void* param) = 0;

    virtual void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) = 0;
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

TAU_DLL IRenderingContext* createGLContext(GLContextSettings settings) noexcept;

TAU_DLL IRenderingContext* createDXContext() noexcept;
