#include "dx/dx10/DX10TextureView.hpp"


#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10ResourceTexture.hpp"
#include "graphics/Resource.hpp"
#include "TauConfig.hpp"
#include <EnumBitFields.hpp>

#if TAU_RTTI_CHECK
  #include "TauEngine.hpp"
  #define CTX() \
      if(!RTT_CHECK(context, DX10RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#endif

void DX10TextureView::generateMipmaps(IRenderingContext& context) noexcept
{
    CTX();
    ctx.d3dDevice()->GenerateMips(_d3dSRV);
}


bool DX10TextureViewBuilder::processArgs(const Texture1DViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTextureArgs* texArgs = args.texture->getArgs<ResourceTextureArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture1D* const dxTexture = static_cast<DX10ResourceTexture1D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    srvDesc.Texture1D.MostDetailedMip = 0;
    srvDesc.Texture1D.MipLevels = args.mipCount;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = args.width;
    dxArgs->mipCount = args.mipCount;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture1DArrayViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.arrayCount == 0, Error::ArrayCountIsZero);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTextureArgs* texArgs = args.texture->getArgs<ResourceTextureArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture1D* const dxTexture = static_cast<DX10ResourceTexture1D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1DARRAY;
    srvDesc.Texture1DArray.MostDetailedMip = 0;
    srvDesc.Texture1DArray.MipLevels = args.mipCount;
    srvDesc.Texture1DArray.FirstArraySlice = 0;
    srvDesc.Texture1DArray.ArraySize = args.arrayCount;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = args.width;
    dxArgs->mipCount = args.mipCount;
    dxArgs->arrayCount = args.arrayCount;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture2DViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTextureArgs* texArgs = args.texture->getArgs<ResourceTextureArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture2D* const dxTexture = static_cast<DX10ResourceTexture2D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = args.mipCount;
    
    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = args.width;
    dxArgs->height = args.height;
    dxArgs->mipCount = args.mipCount;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture2DArrayViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
}

bool DX10TextureViewBuilder::processArgs(const Texture3DViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
}

bool DX10TextureViewBuilder::processArgs(const TextureCubeViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
}

bool DX10TextureViewBuilder::processArgs(const TextureCubeArrayViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
}

DXGI_FORMAT DX10TextureViewBuilder::dxTextureFormat(const ETexture::Format format) noexcept
{
    switch(format)
    {
        case ETexture::Format::Red8UnsignedInt:                 return DXGI_FORMAT_R8_UNORM;
        case ETexture::Format::Red16UnsignedInt:                return DXGI_FORMAT_R16_UNORM;
        case ETexture::Format::Red32UnsignedInt:                return DXGI_FORMAT_R32_UINT;
        case ETexture::Format::RedGreen8UnsignedInt:            return DXGI_FORMAT_R8G8_UNORM;
        case ETexture::Format::RedGreen16UnsignedInt:           return DXGI_FORMAT_R16G16_UNORM;
        case ETexture::Format::RedGreen32UnsignedInt:           return DXGI_FORMAT_R32G32_UINT;
        case ETexture::Format::RedGreenBlue32UnsignedInt:       return DXGI_FORMAT_R32G32B32_UINT;
        case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:   return DXGI_FORMAT_R8G8B8A8_UNORM;
        case ETexture::Format::RedGreenBlueAlpha16UnsignedInt:  return DXGI_FORMAT_R16G16B16A16_UNORM;
        case ETexture::Format::RedGreenBlueAlpha32UnsignedInt:  return DXGI_FORMAT_R32G32B32A32_UINT;
        case ETexture::Format::Red16Float:                      return DXGI_FORMAT_R16_FLOAT;
        case ETexture::Format::Red32Float:                      return DXGI_FORMAT_R32_FLOAT;
        case ETexture::Format::RedGreen16Float:                 return DXGI_FORMAT_R16G16_FLOAT;
        case ETexture::Format::RedGreen32Float:                 return DXGI_FORMAT_R32G32_FLOAT;
        case ETexture::Format::RedGreenBlue32Float:             return DXGI_FORMAT_R32G32B32_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha16Float:        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha32Float:        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ETexture::Format::Depth24Stencil8:                 return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case ETexture::Format::Depth24Typeless8:                return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case ETexture::Format::Typeless24Stencil8:              return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        case ETexture::Format::Red8Typeless:                    return DXGI_FORMAT_R8_TYPELESS;
        case ETexture::Format::Red16Typeless:                   return DXGI_FORMAT_R16_TYPELESS;
        case ETexture::Format::Red32Typeless:                   return DXGI_FORMAT_R32_TYPELESS;
        case ETexture::Format::RedGreen8Typeless:               return DXGI_FORMAT_R8G8_TYPELESS;
        case ETexture::Format::RedGreen16Typeless:              return DXGI_FORMAT_R16G16_TYPELESS;
        case ETexture::Format::RedGreen32Typeless:              return DXGI_FORMAT_R32G32_TYPELESS;
        case ETexture::Format::RedGreenBlue32Typeless:          return DXGI_FORMAT_R32G32B32_TYPELESS;
        case ETexture::Format::RedGreenBlueAlpha8Typeless:      return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case ETexture::Format::RedGreenBlueAlpha16Typeless:     return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        case ETexture::Format::RedGreenBlueAlpha32Typeless:     return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        case ETexture::Format::Typeless24Typeless8:             return DXGI_FORMAT_R24G8_TYPELESS;
        default:                                                return DXGI_FORMAT_UNKNOWN;
    }
}

D3D10_BIND_FLAG DX10TextureViewBuilder::dxBindFlags(const ETexture::BindFlags flags) noexcept
{
    D3D10_BIND_FLAG ret = static_cast<D3D10_BIND_FLAG>(0);

    if(hasFlag(flags, ETexture::BindFlags::RenderTarget) ||
        hasFlag(flags, ETexture::BindFlags::GenerateMipmaps))
    {
        ret |= D3D10_BIND_RENDER_TARGET;
    }
    if(hasFlag(flags, ETexture::BindFlags::ShaderAccess))
    {
        ret |= D3D10_BIND_SHADER_RESOURCE;
    }

    return ret;
}

D3D10_RESOURCE_MISC_FLAG DX10TextureViewBuilder::dxMiscFlags(const ETexture::BindFlags flags) noexcept
{
    if(hasFlag(flags, ETexture::BindFlags::GenerateMipmaps) &&
        hasFlag(flags, ETexture::BindFlags::ShaderAccess))
    {
        return D3D10_RESOURCE_MISC_GENERATE_MIPS;
    }

    return static_cast<D3D10_RESOURCE_MISC_FLAG>(0);
}
#endif
