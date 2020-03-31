#include "dx/dx10/DX10TextureSampler.hpp"

#ifdef _WIN32
#include <cfloat>
#include "dx/dx10/DX10RenderingContext.hpp"

void DX10TextureSampler::bind(const UINT slot) const noexcept
{
    _ctx.d3dDevice()->PSSetSamplers(slot, 1, &_samplerState);
}

void DX10TextureSampler::unbind(const UINT slot) const noexcept
{
    _ctx.d3dDevice()->PSSetSamplers(slot, 0, null);
}

DX10TextureSampler* DX10TextureSamplerBuilder::build(const TextureSamplerArgs& args, Error* const error) const noexcept
{
    D3D10_SAMPLER_DESC d3dSamplerDesc;
    if(!processTextureSamplerArgs(args, &d3dSamplerDesc, error))
    { return null; }

    ID3D10SamplerState* d3dSampler;
    const HRESULT h = _ctx.d3dDevice()->CreateSamplerState(&d3dSamplerDesc, &d3dSampler);

    ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);

    DX10TextureSampler* const sampler = new(::std::nothrow) DX10TextureSampler(_ctx, d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

DX10TextureSampler* DX10TextureSamplerBuilder::build(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    D3D10_SAMPLER_DESC d3dSamplerDesc;
    if(!processTextureSamplerArgs(args, &d3dSamplerDesc, error))
    { return null; }

    ID3D10SamplerState* d3dSampler;
    const HRESULT h = _ctx.d3dDevice()->CreateSamplerState(&d3dSamplerDesc, &d3dSampler);

    ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);

    DX10TextureSampler* const sampler = allocator.allocateT<DX10TextureSampler>(_ctx, d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

CPPRef<ITextureSampler> DX10TextureSamplerBuilder::buildCPPRef(const TextureSamplerArgs& args, Error* const error) const noexcept
{
    D3D10_SAMPLER_DESC d3dSamplerDesc;
    if(!processTextureSamplerArgs(args, &d3dSamplerDesc, error))
    { return null; }

    ID3D10SamplerState* d3dSampler;
    const HRESULT h = _ctx.d3dDevice()->CreateSamplerState(&d3dSamplerDesc, &d3dSampler);

    ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);

    const CPPRef<DX10TextureSampler> sampler = CPPRef<DX10TextureSampler>(new(::std::nothrow) DX10TextureSampler(_ctx, d3dSampler));

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

NullableReferenceCountingPointer<ITextureSampler> DX10TextureSamplerBuilder::buildTauRef(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    D3D10_SAMPLER_DESC d3dSamplerDesc;
    if(!processTextureSamplerArgs(args, &d3dSamplerDesc, error))
    { return null; }

    ID3D10SamplerState* d3dSampler;
    const HRESULT h = _ctx.d3dDevice()->CreateSamplerState(&d3dSamplerDesc, &d3dSampler);

    ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);

    const NullableReferenceCountingPointer<DX10TextureSampler> sampler(allocator, _ctx, d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    NullableReferenceCountingPointer<ITextureSampler> iSampler = RCPCast<ITextureSampler>(sampler);

    ERROR_CODE_V(Error::NoError, iSampler);
}

NullableStrongReferenceCountingPointer<ITextureSampler> DX10TextureSamplerBuilder::buildTauSRef(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    D3D10_SAMPLER_DESC d3dSamplerDesc;
    if(!processTextureSamplerArgs(args, &d3dSamplerDesc, error))
    { return null; }

    ID3D10SamplerState* d3dSampler;
    const HRESULT h = _ctx.d3dDevice()->CreateSamplerState(&d3dSamplerDesc, &d3dSampler);

    ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);

    const NullableStrongReferenceCountingPointer<DX10TextureSampler> sampler(allocator, _ctx, d3dSampler);

    if(!sampler)
    {
        d3dSampler->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    NullableStrongReferenceCountingPointer<ITextureSampler> iSampler = RCPCast<ITextureSampler>(sampler);

    ERROR_CODE_V(Error::NoError, iSampler);
}

bool DX10TextureSamplerBuilder::processTextureSamplerArgs(const TextureSamplerArgs& args, D3D10_SAMPLER_DESC* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.magFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.minFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.mipFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.wrapU == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapV == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapW == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.depthCompareFunc == static_cast<ETexture::DepthCompareFunc>(0), Error::DepthComparisonIsUnset);

    dxArgs->Filter = dxFilter(args.magFilter(), args.minFilter(), args.mipFilter());

    dxArgs->MipLODBias = 0.0f;
    dxArgs->MaxAnisotropy = 16;

    dxArgs->ComparisonFunc = dxDepthComparison(args.depthCompareFunc);

    dxArgs->AddressU = dxWrapMode(args.wrapU);
    dxArgs->AddressV = dxWrapMode(args.wrapV);
    dxArgs->AddressW = dxWrapMode(args.wrapW);
    dxArgs->BorderColor[0] = static_cast<FLOAT>(args.borderColor.r) / 255.0f;
    dxArgs->BorderColor[1] = static_cast<FLOAT>(args.borderColor.g) / 255.0f;
    dxArgs->BorderColor[2] = static_cast<FLOAT>(args.borderColor.b) / 255.0f;
    dxArgs->BorderColor[3] = static_cast<FLOAT>(args.borderColor.a) / 255.0f;

    dxArgs->MinLOD = 0.0f;
    dxArgs->MaxLOD = FLT_MAX;

    return true;
}

D3D10_FILTER DX10TextureSamplerBuilder::dxFilter(const ETexture::Filter magnificationFilter, const ETexture::Filter minificationFilter, const ETexture::Filter mipmapMinificationFilter) noexcept
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

D3D10_COMPARISON_FUNC DX10TextureSamplerBuilder::dxDepthComparison(const ETexture::DepthCompareFunc depthCompare) noexcept
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
