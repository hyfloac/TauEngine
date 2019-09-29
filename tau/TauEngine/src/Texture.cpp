#include <texture/Texture.hpp>
#include <texture/GLTexture.hpp>
#include <texture/DXTexture.hpp>
#include <RenderingMode.hpp>
#include <Utils.hpp>

ITexture* ITexture::create(const RenderingMode& mode, const TextureType textureType) noexcept
{
    switch(mode.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
#ifdef _WIN32
            return new D3D9Texture(textureType);
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
            return new GLTexture(textureType);
        default: return null;
    }
}
