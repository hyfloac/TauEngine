#pragma once

#include "texture/TextureSampler.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11RenderingContext;
class DX11GraphicsInterface;

class TAU_DLL DX11TextureSampler final : public ITextureSampler
{
    DEFAULT_DESTRUCT(DX11TextureSampler);
    TEXTURE_SAMPLER_IMPL(DX11TextureSampler);
private:
    ID3D11SamplerState* _samplerState;
public:
    inline DX11TextureSampler(ID3D11SamplerState* const samplerState) noexcept
        : _samplerState(samplerState)
    { }

    [[nodiscard]] const ID3D11SamplerState* d3dSampler() const noexcept { return _samplerState; }
    [[nodiscard]] ID3D11SamplerState* d3dSampler() noexcept { return _samplerState; }

    void bind(DX11RenderingContext& context, UINT slot) const noexcept;
    void unbind(DX11RenderingContext& context, UINT slot) const noexcept;
};

class TAU_DLL DX11TextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_DESTRUCT(DX11TextureSamplerBuilder);
    DELETE_COPY(DX11TextureSamplerBuilder);
public:
    [[nodiscard]] static D3D11_FILTER dxFilter(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept;
    [[nodiscard]] static D3D11_TEXTURE_ADDRESS_MODE  dxWrapMode(ETexture::WrapMode wrapMode) noexcept;
    [[nodiscard]] static D3D11_COMPARISON_FUNC dxDepthComparison(ETexture::CompareFunc depthCompare) noexcept;
private:
    DX11GraphicsInterface& _gi;
public:
    DX11TextureSamplerBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11TextureSampler* build(const TextureSamplerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11TextureSampler* build(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureSampler> buildCPPRef(const TextureSamplerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureSampler> buildTauRef(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureSampler> buildTauSRef(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureSamplerArgs& args, [[tau::out]] ID3D11SamplerState** dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
