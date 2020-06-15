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

DX10Texture1DView* DX10TextureViewBuilder::build(const Texture1DViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture1DView* const textureView = new(::std::nothrow) DX10Texture1DView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture1DView* DX10TextureViewBuilder::build(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture1DView* const textureView = allocator.allocateT<DX10Texture1DView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const Texture1DViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture1DView> textureView(new(::std::nothrow) DX10Texture1DView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture1DView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture1DView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture1DArrayView* DX10TextureViewBuilder::build(const Texture1DArrayViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture1DArrayView* const textureView = new(::std::nothrow) DX10Texture1DArrayView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture1DArrayView* DX10TextureViewBuilder::build(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture1DArrayView* const textureView = allocator.allocateT<DX10Texture1DArrayView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const Texture1DArrayViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture1DArrayView> textureView(new(::std::nothrow) DX10Texture1DArrayView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture1DArrayView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture1DArrayView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture2DView* DX10TextureViewBuilder::build(const Texture2DViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2DView* const textureView = new(::std::nothrow) DX10Texture2DView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture2DView* DX10TextureViewBuilder::build(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2DView* const textureView = allocator.allocateT<DX10Texture2DView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const Texture2DViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture2DView> textureView(new(::std::nothrow) DX10Texture2DView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture2DView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture2DView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture2DArrayView* DX10TextureViewBuilder::build(const Texture2DArrayViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2DArrayView* const textureView = new(::std::nothrow) DX10Texture2DArrayView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture2DArrayView* DX10TextureViewBuilder::build(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2DArrayView* const textureView = allocator.allocateT<DX10Texture2DArrayView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const Texture2DArrayViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture2DArrayView> textureView(new(::std::nothrow) DX10Texture2DArrayView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture2DArrayView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture2DArrayView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture3DView* DX10TextureViewBuilder::build(const Texture3DViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture3DView* const textureView = new(::std::nothrow) DX10Texture3DView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10Texture3DView* DX10TextureViewBuilder::build(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture3DView* const textureView = allocator.allocateT<DX10Texture3DView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const Texture3DViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture3DView> textureView(new(::std::nothrow) DX10Texture3DView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture3DView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture3DView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10TextureCubeView* DX10TextureViewBuilder::build(const TextureCubeViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCubeView* const textureView = new(::std::nothrow) DX10TextureCubeView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10TextureCubeView* DX10TextureViewBuilder::build(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCubeView* const textureView = allocator.allocateT<DX10TextureCubeView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const TextureCubeViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10TextureCubeView> textureView(new(::std::nothrow) DX10TextureCubeView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10TextureCubeView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10TextureCubeView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10TextureCubeArrayView* DX10TextureViewBuilder::build(const TextureCubeArrayViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCubeArrayView* const textureView = new(::std::nothrow) DX10TextureCubeArrayView(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

DX10TextureCubeArrayView* DX10TextureViewBuilder::build(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCubeArrayView* const textureView = allocator.allocateT<DX10TextureCubeArrayView>(dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

CPPRef<ITextureView> DX10TextureViewBuilder::buildCPPRef(const TextureCubeArrayViewArgs& args, Error* error) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10TextureCubeArrayView> textureView(new(::std::nothrow) DX10TextureCubeArrayView(dxArgs));
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableRef<ITextureView> DX10TextureViewBuilder::buildTauRef(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10TextureCubeArrayView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

NullableStrongRef<ITextureView> DX10TextureViewBuilder::buildTauSRef(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10TextureCubeArrayView> textureView(allocator, dxArgs);
    ERROR_CODE_COND_N(!textureView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, textureView);
}

bool DX10TextureViewBuilder::processArgs(const Texture1DViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture1DArgs* texArgs = args.texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture1D* const dxTexture = static_cast<DX10ResourceTexture1D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    srvDesc.Texture1D.MostDetailedMip = 0;
    srvDesc.Texture1D.MipLevels = texArgs->mipLevels;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = texArgs->width;
    dxArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture1DArrayViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture1DArgs* texArgs = args.texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount == 1, Error::TextureIsNotArray);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture1D* const dxTexture = static_cast<DX10ResourceTexture1D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1DARRAY;
    srvDesc.Texture1DArray.MostDetailedMip = 0;
    srvDesc.Texture1DArray.MipLevels = texArgs->mipLevels;
    srvDesc.Texture1DArray.FirstArraySlice = 0;
    srvDesc.Texture1DArray.ArraySize = texArgs->arrayCount;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = texArgs->width;
    dxArgs->mipLevels = texArgs->mipLevels;
    dxArgs->arrayCount = texArgs->arrayCount;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture2DViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture2D* const dxTexture = static_cast<DX10ResourceTexture2D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = texArgs->mipLevels;
    
    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = texArgs->width;
    dxArgs->height = texArgs->height;
    dxArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture2DArrayViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount == 1, Error::TextureIsNotArray);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture2D* const dxTexture = static_cast<DX10ResourceTexture2D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MostDetailedMip = 0;
    srvDesc.Texture2DArray.MipLevels = texArgs->mipLevels;
    srvDesc.Texture1DArray.FirstArraySlice = 0;
    srvDesc.Texture1DArray.ArraySize = texArgs->arrayCount;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = texArgs->width;
    dxArgs->height = texArgs->height;
    dxArgs->mipLevels = texArgs->mipLevels;
    dxArgs->arrayCount = texArgs->arrayCount;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const Texture3DViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture3D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture3DArgs* texArgs = args.texture->getArgs<ResourceTexture3DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture3D* const dxTexture = static_cast<DX10ResourceTexture3D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE3D;
    srvDesc.Texture3D.MostDetailedMip = 0;
    srvDesc.Texture3D.MipLevels = texArgs->mipLevels;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = texArgs->width;
    dxArgs->height = texArgs->height;
    dxArgs->depth = texArgs->depth;
    dxArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const TextureCubeViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount != 6, Error::TextureIsNotArray);

    DX10Resource* const dxResource = RTTD_CAST(args.texture, DX10Resource, IResource);
    DX10ResourceTexture2D* const dxTexture = static_cast<DX10ResourceTexture2D* const>(dxResource);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = dxTextureFormat(args.dataFormat);
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = texArgs->mipLevels;

    const HRESULT res = _gi.d3d10Device()->CreateShaderResourceView(dxTexture->d3dTexture(), &srvDesc, &dxArgs->d3dSRV);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    dxArgs->dataFormat = args.dataFormat;
    dxArgs->width = texArgs->width;
    dxArgs->height = texArgs->height;
    dxArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool DX10TextureViewBuilder::processArgs(const TextureCubeArrayViewArgs& args, DXTextureViewArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_F(Error::UnsupportedType);
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
