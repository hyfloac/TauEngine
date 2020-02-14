#pragma once

#include "texture/Texture.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL DX10Texture2D final : public ITexture
{
    TEXTURE_IMPL(DX10Texture2D);
private:
    DX10RenderingContext& _ctx;
    ID3D10Texture2D* _d3dTexture;
    ID3D10ShaderResourceView* _d3dTextureView;
public:
    DX10Texture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, DX10RenderingContext& ctx, ID3D10Texture2D* const d3dTexture, ID3D10ShaderResourceView* const d3dTextureView) noexcept
        : ITexture(width, height, dataFormat), _ctx(ctx), _d3dTexture(d3dTexture), _d3dTextureView(d3dTextureView)
    { }

    ~DX10Texture2D() noexcept;

    [[nodiscard]] ID3D10Texture2D* d3dTexture() noexcept { return _d3dTexture; }
    [[nodiscard]] const ID3D10Texture2D* d3dTexture() const noexcept { return _d3dTexture; }

    [[nodiscard]] ID3D10ShaderResourceView* d3dTextureView() noexcept { return _d3dTextureView; }
    [[nodiscard]] const ID3D10ShaderResourceView* d3dTextureView() const noexcept { return _d3dTextureView; }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }

    void set(u32 level, const void* data) noexcept override;
    void bind(u8 textureUnit) noexcept override;
    void unbind(u8 textureUnit) noexcept override;
    void generateMipmaps() noexcept override;
};

class TAU_DLL DX10Texture2DBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10Texture2DBuilder);
    DEFAULT_DESTRUCT(DX10Texture2DBuilder);
    DELETE_COPY(DX10Texture2DBuilder);
public:
    static DXGI_FORMAT dxTextureFormat(ETexture::Format format) noexcept;
private:
    DX10RenderingContext& _ctx;
public:
    DX10Texture2DBuilder(DX10RenderingContext& ctx) noexcept
        : _ctx(ctx)
    { }

    [[nodiscard]] ITexture* build([[tau::out]] Error* error) const noexcept override;
};
#endif
