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
    DX10RenderingContext& _ctx;
    ID3D10SamplerState* _samplerState;
public:
    inline DX10TextureSampler(DX10RenderingContext& context, ID3D10SamplerState* const samplerState) noexcept
        : ITextureSampler(), _ctx(context), _samplerState(samplerState)
    { }

    [[nodiscard]] const ID3D10SamplerState* d3dSampler() const noexcept { return _samplerState; }
    [[nodiscard]] ID3D10SamplerState* d3dSampler() noexcept { return _samplerState; }

    void bind(UINT slot) const noexcept;
    void unbind(UINT slot) const noexcept;
};

class TAU_DLL DX10TextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_DESTRUCT(DX10TextureSamplerBuilder);
    DELETE_COPY(DX10TextureSamplerBuilder);
public:
    [[nodiscard]] static D3D10_FILTER dxFilter(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept;
    [[nodiscard]] static D3D10_TEXTURE_ADDRESS_MODE  dxWrapMode(ETexture::WrapMode wrapMode) noexcept;
    [[nodiscard]] static D3D10_COMPARISON_FUNC dxDepthComparison(ETexture::DepthCompareFunc depthCompare) noexcept;
private:
    DX10RenderingContext& _ctx;
public:
    DX10TextureSamplerBuilder(DX10RenderingContext& context) noexcept
        : ITextureSamplerBuilder(), _ctx(context)
    { }

    [[nodiscard]] DX10TextureSampler* build(const TextureSamplerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10TextureSampler* build(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureSampler> buildCPPRef(const TextureSamplerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableReferenceCountingPointer<ITextureSampler> buildTauRef(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongReferenceCountingPointer<ITextureSampler> buildTauSRef(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    bool processTextureSamplerArgs(const TextureSamplerArgs& args, [[tau::out]] D3D10_SAMPLER_DESC* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
