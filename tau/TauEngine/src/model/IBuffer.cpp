#include "model/IBuffer.hpp"
#include "gl/GLBuffer.hpp"
#include "Timings.hpp"

Ref<IBuffer> IBuffer::create(IRenderingContext& context, const std::size_t descriptorCount, const Type type, const UsageType usage) noexcept
{
    switch(context.mode().currentMode())
    {
        case RenderingMode::Mode::DirectX9:
#ifdef _WIN32
            return null;
#else
            return null;
#endif
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
#ifdef _WIN32
            return null;
#else
            return null;
#endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL2:
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return Ref<IBuffer>(new(std::nothrow) GLBuffer(type, usage, descriptorCount));
        default: return null;
    }
}

Ref<IIndexBuffer> IIndexBuffer::create(IRenderingContext& context, const IBuffer::UsageType usage) noexcept
{
    switch(context.mode().currentMode())
    {
        case RenderingMode::Mode::DirectX9:
        #ifdef _WIN32
            return null;
        #else
            return null;
        #endif
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
        #ifdef _WIN32
            return null;
        #else
            return null;
        #endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL2:
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return Ref<IIndexBuffer>(new(std::nothrow) GLIndexBuffer(usage));
        default: return null;
    }
}