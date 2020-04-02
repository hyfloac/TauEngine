#include "vr/VRUtils.hpp"
#include "texture/Texture.hpp"

vr::ETextureType tauGetTextureType(const RenderingMode::Mode mode) noexcept
{
    switch(mode)
    {
        case RenderingMode::DirectX9:
        case RenderingMode::DirectX10:
        case RenderingMode::DirectX11:
            return vr::TextureType_DirectX;
        case RenderingMode::DirectX12:
        case RenderingMode::DirectX12_1:
            return vr::TextureType_DirectX12;
        case RenderingMode::Vulkan:
            return vr::TextureType_Vulkan;
        case RenderingMode::OpenGL3:
        case RenderingMode::OpenGL3_1:
        case RenderingMode::OpenGL3_2:
        case RenderingMode::OpenGL3_3:
        case RenderingMode::OpenGL4:
        case RenderingMode::OpenGL4_2:
        case RenderingMode::OpenGL4_3:
        case RenderingMode::OpenGL4_4:
        case RenderingMode::OpenGL4_5:
        case RenderingMode::OpenGL4_6:
            return vr::TextureType_OpenGL;
        default: return vr::TextureType_Invalid;
    }
}

VRTextureHandle_t tauGetVRTextureHandle(ITexture* texture) noexcept
{
    return reinterpret_cast<VRTextureHandle_t>(static_cast<uintptr_t>(texture->_getHandle()));
}
