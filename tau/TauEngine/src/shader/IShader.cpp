#include "shader/IShader.hpp"
#include "RenderingMode.hpp"
#include "gl/GLShader.hpp"
#include "system/RenderingContext.hpp"
#include "Timings.hpp"

Ref<IShader> IShader::create(IRenderingContext& context, Type shaderType, const NotNull<const char>& shaderPath) noexcept
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
            return Ref<IShader>(GLShader::create(shaderType, shaderPath));
        default: return null;
    }
}
