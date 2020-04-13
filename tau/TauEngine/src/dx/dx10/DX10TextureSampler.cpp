#include "dx/dx10/DX10TextureSampler.hpp"

#ifdef _WIN32
#include <cfloat>
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"

void DX10TextureSampler::bind(DX10RenderingContext& context, const UINT slot) const noexcept
{
    context.d3dDevice()->PSSetSamplers(slot, 1, &_samplerState);
}

void DX10TextureSampler::unbind(DX10RenderingContext& context, const UINT slot) const noexcept
{
    ID3D10SamplerState* nullSS = null;
    context.d3dDevice()->PSSetSamplers(slot, 1, &nullSS);
}

DX10TextureSampler* DX10TextureSamplerBuilder::build(const TextureSamplerArgs& args, Error* const error) const noexcept
{
    ID3D10SamplerState* d3dSampler;
    if(!processArgs(args, &d3dSampler, error))
    { return null; }

    DX10TextureSampler* const sampler = new(::std::nothrow) DX10TextureSampler(d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

DX10TextureSampler* DX10TextureSamplerBuilder::build(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10SamplerState* d3dSampler;
    if(!processArgs(args, &d3dSampler, error))
    { return null; }

    DX10TextureSampler* const sampler = allocator.allocateT<DX10TextureSampler>(d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

CPPRef<ITextureSampler> DX10TextureSamplerBuilder::buildCPPRef(const TextureSamplerArgs& args, Error* const error) const noexcept
{
    ID3D10SamplerState* d3dSampler;
    if(!processArgs(args, &d3dSampler, error))
    { return null; }

    const CPPRef<DX10TextureSampler> sampler = CPPRef<DX10TextureSampler>(new(::std::nothrow) DX10TextureSampler(d3dSampler));

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

NullableRef<ITextureSampler> DX10TextureSamplerBuilder::buildTauRef(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10SamplerState* d3dSampler;
    if(!processArgs(args, &d3dSampler, error))
    { return null; }

    const NullableRef<DX10TextureSampler> sampler(allocator, d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

NullableStrongRef<ITextureSampler> DX10TextureSamplerBuilder::buildTauSRef(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10SamplerState* d3dSampler;
    if(!processArgs(args, &d3dSampler, error))
    { return null; }

    const NullableStrongRef<DX10TextureSampler> sampler(allocator, d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

bool DX10TextureSamplerBuilder::processArgs(const TextureSamplerArgs& args, ID3D10SamplerState** dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.magFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.minFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.mipFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.wrapU == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapV == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapW == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.depthCompareFunc == static_cast<ETexture::CompareFunc>(0), Error::DepthComparisonIsUnset);

    D3D10_SAMPLER_DESC d3dSamplerDesc;

    d3dSamplerDesc.Filter = dxFilter(args.magFilter(), args.minFilter(), args.mipFilter());

    d3dSamplerDesc.MipLODBias = 0.0f;
    d3dSamplerDesc.MaxAnisotropy = 16;

    d3dSamplerDesc.ComparisonFunc = dxDepthComparison(args.depthCompareFunc);

    d3dSamplerDesc.AddressU = dxWrapMode(args.wrapU);
    d3dSamplerDesc.AddressV = dxWrapMode(args.wrapV);
    d3dSamplerDesc.AddressW = dxWrapMode(args.wrapW);
    d3dSamplerDesc.BorderColor[0] = static_cast<FLOAT>(args.borderColor.r) / 255.0f;
    d3dSamplerDesc.BorderColor[1] = static_cast<FLOAT>(args.borderColor.g) / 255.0f;
    d3dSamplerDesc.BorderColor[2] = static_cast<FLOAT>(args.borderColor.b) / 255.0f;
    d3dSamplerDesc.BorderColor[3] = static_cast<FLOAT>(args.borderColor.a) / 255.0f;
    
    d3dSamplerDesc.MinLOD = 0.0f;
    d3dSamplerDesc.MaxLOD = FLT_MAX;

    const HRESULT h = _gi.d3d10Device()->CreateSamplerState(&d3dSamplerDesc, dxArgs);
    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    return true;
}

D3D10_FILTER DX10TextureSamplerBuilder::dxFilter(const ETexture::Filter magnificationFilter, const ETexture::Filter minificationFilter, const ETexture::Filter mipmapMinificationFilter) noexcept
{
    uSys mask = 0;
    if(magnificationFilter == ETexture::Filter::Point)
    { mask |= (1 << 0); }  
    if(minificationFilter == ETexture::Filter::Point)
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

D3D10_TEXTURE_ADDRESS_MODE DX10TextureSamplerBuilder::dxWrapMode(const ETexture::WrapMode wrapMode) noexcept
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

D3D10_COMPARISON_FUNC DX10TextureSamplerBuilder::dxDepthComparison(const ETexture::CompareFunc depthCompare) noexcept
{
    switch(depthCompare)
    {
        case ETexture::CompareFunc::LessThanOrEqual:    return D3D10_COMPARISON_LESS_EQUAL;
        case ETexture::CompareFunc::GreaterThanOrEqual: return D3D10_COMPARISON_GREATER_EQUAL;
        case ETexture::CompareFunc::LessThan:           return D3D10_COMPARISON_LESS;
        case ETexture::CompareFunc::GreaterThan:        return D3D10_COMPARISON_GREATER;
        case ETexture::CompareFunc::Equal:              return D3D10_COMPARISON_EQUAL;
        case ETexture::CompareFunc::NotEqual:           return D3D10_COMPARISON_NOT_EQUAL;
        case ETexture::CompareFunc::Always:             return D3D10_COMPARISON_ALWAYS;
        case ETexture::CompareFunc::Never:              return D3D10_COMPARISON_NEVER;
        default: return static_cast<D3D10_COMPARISON_FUNC>(-1);
    }
}
#endif
