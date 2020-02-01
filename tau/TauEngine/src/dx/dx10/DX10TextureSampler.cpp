#include "dx/dx10/DX10TextureSampler.hpp"

#ifdef _WIN32
#include <cfloat>
#include "dx/dx10/DX10RenderingContext.hpp"

DX10TextureSamplerBuilder::DX10TextureSamplerBuilder(DX10RenderingContext& context) noexcept
    : ITextureSamplerBuilder(), _ctx(context), _samplerDescriptor()
{
    ZeroMemory(&_samplerDescriptor, sizeof(_samplerDescriptor));
    _samplerDescriptor.MipLODBias = 0.0f;
    _samplerDescriptor.MaxAnisotropy = 16;
    _samplerDescriptor.MinLOD = 0.0f;
    _samplerDescriptor.MaxLOD = FLT_MAX;
}

void DX10TextureSamplerBuilder::setFilterMode(const ETexture::Filter magnificationFilter, const ETexture::Filter minificationFilter, const ETexture::Filter mipmapMinificationFilter) noexcept
{
    _samplerDescriptor.Filter = getDXFilter(magnificationFilter, minificationFilter, mipmapMinificationFilter);
}

void DX10TextureSamplerBuilder::setWrapMode(const ETexture::WrapMode u, const ETexture::WrapMode v, const ETexture::WrapMode w) noexcept
{
    _samplerDescriptor.AddressU = getDXWrapMode(u);
    _samplerDescriptor.AddressV = getDXWrapMode(v);
    _samplerDescriptor.AddressW = getDXWrapMode(w);
}

void DX10TextureSamplerBuilder::setDepthComparison(const ETexture::DepthCompareFunc compareFunc) noexcept
{
    _samplerDescriptor.ComparisonFunc = getDXDepthComparison(compareFunc);
}

void DX10TextureSamplerBuilder::setBorderColor(const RGBAColor color) noexcept
{
    _samplerDescriptor.BorderColor[0] = static_cast<FLOAT>(color.r) / 255.0f;
    _samplerDescriptor.BorderColor[1] = static_cast<FLOAT>(color.g) / 255.0f;
    _samplerDescriptor.BorderColor[2] = static_cast<FLOAT>(color.b) / 255.0f;
    _samplerDescriptor.BorderColor[3] = static_cast<FLOAT>(color.a) / 255.0f;
}

DX10TextureSampler* DX10TextureSamplerBuilder::build(Error* const error) noexcept
{
    ID3D10SamplerState* samplerState;
    const HRESULT h = _ctx.d3d10Device()->CreateSamplerState(&_samplerDescriptor, &samplerState);

    ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);

    DX10TextureSampler* const sampler = new(::std::nothrow) DX10TextureSampler(samplerState);

    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, sampler);
}

D3D10_FILTER DX10TextureSamplerBuilder::getDXFilter(const ETexture::Filter magnificationFilter, const ETexture::Filter minificationFilter, const ETexture::Filter mipmapMinificationFilter) noexcept
{
    uSys mask = 0;
    if(magnificationFilter      == ETexture::Filter::Point)
    { mask |= (1 << 0); }
    if(minificationFilter       == ETexture::Filter::Point)
    { mask |= (1 << 1); }
    if(mipmapMinificationFilter == ETexture::Filter::Point)
    { mask |= (1 << 2); }

    switch(mask)
    {
        case 0b000: return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
        case 0b001: return D3D10_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        case 0b010: return D3D10_FILTER_MIN_POINT_MAG_MIP_LINEAR;
        case 0b011: return D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        case 0b100: return D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        case 0b101: return D3D10_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        case 0b110: return D3D10_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        case 0b111: return D3D10_FILTER_MIN_MAG_MIP_POINT;
        default: return static_cast<D3D10_FILTER>(-1);
    }
}


D3D10_TEXTURE_ADDRESS_MODE DX10TextureSamplerBuilder::getDXWrapMode(const ETexture::WrapMode wrapMode) noexcept
{
    switch(wrapMode)
    {
        case ETexture::WrapMode::ClampToEdge:       return D3D10_TEXTURE_ADDRESS_CLAMP;
        case ETexture::WrapMode::ClampToBorder:     return D3D10_TEXTURE_ADDRESS_BORDER;
        case ETexture::WrapMode::MirroredRepeat:    return D3D10_TEXTURE_ADDRESS_MIRROR;
        case ETexture::WrapMode::Repeat:            return D3D10_TEXTURE_ADDRESS_WRAP;
        case ETexture::WrapMode::MirrorClampToEdge: return D3D10_TEXTURE_ADDRESS_MIRROR_ONCE;
        default: return static_cast<D3D10_TEXTURE_ADDRESS_MODE>(-1);
    }
}


D3D10_COMPARISON_FUNC DX10TextureSamplerBuilder::getDXDepthComparison(const ETexture::DepthCompareFunc depthCompare) noexcept
{
    switch(depthCompare)
    {
        case ETexture::DepthCompareFunc::LessThanOrEqual:    return D3D10_COMPARISON_LESS_EQUAL;
        case ETexture::DepthCompareFunc::GreaterThanOrEqual: return D3D10_COMPARISON_GREATER_EQUAL;
        case ETexture::DepthCompareFunc::LessThan:           return D3D10_COMPARISON_LESS;
        case ETexture::DepthCompareFunc::GreaterThan:        return D3D10_COMPARISON_GREATER;
        case ETexture::DepthCompareFunc::Equal:              return D3D10_COMPARISON_EQUAL;
        case ETexture::DepthCompareFunc::NotEqual:           return D3D10_COMPARISON_NOT_EQUAL;
        case ETexture::DepthCompareFunc::Always:             return D3D10_COMPARISON_ALWAYS;
        case ETexture::DepthCompareFunc::Never:              return D3D10_COMPARISON_NEVER;
        default: return static_cast<D3D10_COMPARISON_FUNC>(-1);
    }
}
#endif
