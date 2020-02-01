#pragma once

#include "texture/TextureSampler.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL DX10TextureSampler final : public ITextureSampler
{
    DEFAULT_DESTRUCT(DX10TextureSampler);
    TEXTURE_SAMPLER_IMPL(DX10TextureSampler);
private:
    ID3D10SamplerState* _samplerState;
private:
    inline DX10TextureSampler(ID3D10SamplerState* const _samplerState) noexcept
        : ITextureSampler(), _samplerState(_samplerState)
    { }
private:
    friend class DX10TextureSamplerBuilder;
};

class TAU_DLL DX10TextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_DESTRUCT(DX10TextureSamplerBuilder);
    DELETE_COPY(DX10TextureSamplerBuilder);
public:
    [[nodiscard]] static D3D10_FILTER getDXFilter(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept;
    [[nodiscard]] static D3D10_TEXTURE_ADDRESS_MODE  getDXWrapMode(ETexture::WrapMode wrapMode) noexcept;
    [[nodiscard]] static D3D10_COMPARISON_FUNC getDXDepthComparison(ETexture::DepthCompareFunc depthCompare) noexcept;
private:
    DX10RenderingContext& _ctx;
    D3D10_SAMPLER_DESC _samplerDescriptor;
public:
    DX10TextureSamplerBuilder(DX10RenderingContext& context) noexcept;

    void setFilterMode(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept override;
    void setWrapMode(ETexture::WrapMode u, ETexture::WrapMode v, ETexture::WrapMode w) noexcept override;
    void setDepthComparison(ETexture::DepthCompareFunc compareFunc) noexcept override;
    void setBorderColor(RGBAColor color) noexcept override;

    [[nodiscard]] DX10TextureSampler* build(Error* error) noexcept override;
};
#endif
