#pragma once

#include "Texture.hpp"

class TAU_DLL NullTexture final : public ITexture
{
    DEFAULT_CONSTRUCT_PU(NullTexture);
    DEFAULT_DESTRUCT(NullTexture);
    TEXTURE_IMPL(NullTexture);
public:
    [[nodiscard]] ETexture::Format dataFormat() const noexcept override { return static_cast<ETexture::Format>(0); }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override { return static_cast<ETexture::Type>(0); }
};

class TAU_DLL NullTexture2D final : public ITexture2D
{
    DEFAULT_DESTRUCT(NullTexture2D);
    TEXTURE_IMPL(NullTexture2D);
public:
    inline NullTexture2D() noexcept
        : ITexture2D(0, 0, static_cast<ETexture::Format>(0))
    { }

    void set(IRenderingContext& context, u32 mipLevel, const void* data) noexcept override { }
};

class TAU_DLL NullTexture3D final : public ITexture3D
{
    DEFAULT_DESTRUCT(NullTexture3D);
    TEXTURE_IMPL(NullTexture3D);
public:
    inline NullTexture3D() noexcept
        : ITexture3D(0, 0, 0, static_cast<ETexture::Format>(0))
    { }

    void set(IRenderingContext& context, u32 depthLevel, u32 mipLevel, const void* data) noexcept override { }
};

class TAU_DLL NullTextureCube final : public ITextureCube
{
    DEFAULT_DESTRUCT(NullTextureCube);
    TEXTURE_IMPL(NullTextureCube);
public:
    inline NullTextureCube() noexcept
        : ITextureCube(0, 0, static_cast<ETexture::Format>(0))
    { }

    void set(IRenderingContext& context, u32 mipLevel, ETexture::CubeSide side, const void* data) noexcept override { }
};

class TAU_DLL NullTextureDepthStencil final : public ITextureDepthStencil
{
    DEFAULT_DESTRUCT(NullTextureDepthStencil);
    TEXTURE_IMPL(NullTextureDepthStencil);
public:
    inline NullTextureDepthStencil() noexcept
        : ITextureDepthStencil(0, 0)
    { }

    void set(IRenderingContext& context, const void* data) noexcept override { }
};
