#pragma once

#include "texture/TextureSampler.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;

class TAU_DLL DX10TextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_DESTRUCT(DX10TextureSamplerBuilder);
    DEFAULT_CM_PU(DX10TextureSamplerBuilder);
public:
    [[nodiscard]] static D3D10_FILTER dxFilter(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept;
    [[nodiscard]] static D3D10_TEXTURE_ADDRESS_MODE  dxWrapMode(ETexture::WrapMode wrapMode) noexcept;
    [[nodiscard]] static D3D10_COMPARISON_FUNC dxDepthComparison(ETexture::CompareFunc depthCompare) noexcept;
private:
    DX10GraphicsInterface& _gi;
public:
    DX10TextureSamplerBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] TextureSampler build(const TextureSamplerArgs& args, DescriptorSamplerTable table, uSys tableIndex, Error* error = null) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureSamplerArgs& args, [[tau::out]] ID3D10SamplerState** dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
