#pragma once

#include <NumTypes.hpp>

class IRenderingContext
{
public:
    virtual ~IRenderingContext() = default;

    virtual void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) = 0;

    virtual void createContext(void* param) = 0;
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

IRenderingContext* createGLContext(GLContextSettings settings) noexcept;

IRenderingContext* createDXContext() noexcept;
