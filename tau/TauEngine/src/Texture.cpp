#include <texture/Texture.hpp>
#include <gl/GLTexture.hpp>
#include <dx/DXTexture.hpp>
#include <RenderingMode.hpp>
#include <Utils.hpp>

ITexture* ITexture::create(const RenderingMode& mode, const u32 width, const u32 height, const ETexture::Format format, const ETexture::Type textureType) noexcept
{
    switch(mode.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
#ifdef _WIN32
            return new(std::nothrow) D3D9Texture2D(width, height, format);
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
            return new(std::nothrow) GLTexture2D(width, height, format);
        default: return null;
    }
}

ITexture::ITexture(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
    : _width(width), _height(height), _dataFormat(dataFormat)
{ }
