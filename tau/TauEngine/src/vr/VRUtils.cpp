#include "vr/VRUtils.hpp"
#include "graphics/Resource.hpp"
#include "graphics/ResourceRawInterface.hpp"

vr::ETextureType tauGetTextureType(const RenderingMode::Mode mode) noexcept
{
    switch(mode)
    {
        case RenderingMode::DirectX10:
        case RenderingMode::DirectX11:
            return vr::TextureType_DirectX;
        case RenderingMode::DirectX12:
        case RenderingMode::DirectX12_1:
            return vr::TextureType_DirectX12;
        case RenderingMode::Vulkan:
            return vr::TextureType_Vulkan;
        case RenderingMode::OpenGL4_1:
        case RenderingMode::OpenGL4_2:
        case RenderingMode::OpenGL4_3:
        case RenderingMode::OpenGL4_4:
        case RenderingMode::OpenGL4_5:
        case RenderingMode::OpenGL4_6:
            return vr::TextureType_OpenGL;
        default: return vr::TextureType_Invalid;
    }
}

VRTextureHandle_t tauGetVRTextureHandle(const IResource* const texture) noexcept
{
    return texture->_getRawHandle().rawHandle();
}
