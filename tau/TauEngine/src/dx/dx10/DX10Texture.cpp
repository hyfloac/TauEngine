#include "dx/dx10/DX10Texture.hpp"
#include <d3d9.h>

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"

DX10Texture2D::~DX10Texture2D() noexcept
{
    _d3dTexture->Release();
    _d3dTextureView->Release();
    _d3dTexture = null;
    _d3dTextureView = null;
}

void DX10Texture2D::set(const u32 level, const void* const data) noexcept
{
    _ctx.d3d10Device()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(level, 0, _mipLevels), NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
}

void DX10Texture2D::bind(const u8 textureUnit, const EShader::Stage stage) noexcept
{
    switch(stage)
    {
        case EShader::Stage::Vertex:
            _ctx.d3d10Device()->VSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Geometry:
            _ctx.d3d10Device()->GSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Pixel:
            _ctx.d3d10Device()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        default: return;
    }
}

void DX10Texture2D::unbind(u8, EShader::Stage) noexcept
{ }

void DX10Texture2D::generateMipmaps() noexcept
{
    _ctx.d3d10Device()->GenerateMips(_d3dTextureView);
}

DX10TextureCube::~DX10TextureCube() noexcept
{
    _d3dTexture->Release();
    _d3dTextureView->Release();
    _d3dTexture = null;
    _d3dTextureView = null;
}

void DX10TextureCube::bind(const u8 textureUnit, const EShader::Stage stage) noexcept
{
    switch(stage)
    {
        case EShader::Stage::Vertex:
            _ctx.d3d10Device()->VSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Geometry:
            _ctx.d3d10Device()->GSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Pixel:
            _ctx.d3d10Device()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        default: return;
    }
    _ctx.d3d10Device()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
}

void DX10TextureCube::unbind(u8, EShader::Stage) noexcept
{ }

void DX10TextureCube::generateMipmaps() noexcept
{
    _ctx.d3d10Device()->GenerateMips(_d3dTextureView);
}

void DX10TextureCube::setCube(const u32 level, const ETexture::CubeSide side, const void* const data) noexcept
{
    _ctx.d3d10Device()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(level, dxCubeSide(side), _mipLevels), NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
}

DX10NullTexture* DX10NullTextureBuilder::build(const TextureArgs& args, Error* error) const noexcept
{
    DX10NullTexture* const texture = new(::std::nothrow) DX10NullTexture;
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, texture);
}

DX10NullTexture* DX10NullTextureBuilder::build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX10NullTexture* const texture = allocator.allocateT<DX10NullTexture>();
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture> DX10NullTextureBuilder::buildCPPRef(const TextureArgs& args, Error* error) const noexcept
{
    const CPPRef<DX10NullTexture> texture = CPPRef<DX10NullTexture>(new(::std::nothrow) DX10NullTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture> DX10NullTextureBuilder::buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    const NullableRef<DX10NullTexture> texture(allocator);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, RefCast<ITexture>(texture));
}

NullableStrongRef<ITexture> DX10NullTextureBuilder::buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    const NullableStrongRef<DX10NullTexture> texture(allocator);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, RefCast<ITexture>(texture));
}

DX10Texture2D* DX10Texture2DBuilder::build(const TextureArgs& args, Error* error) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2D* const texture = new(::std::nothrow) DX10Texture2D(args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Texture2D* DX10Texture2DBuilder::build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2D* const texture = allocator.allocateT<DX10Texture2D>(args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture> DX10Texture2DBuilder::buildCPPRef(const TextureArgs& args, Error* error) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture2D> texture = CPPRef<DX10Texture2D>(new(::std::nothrow) DX10Texture2D(args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels));

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture> DX10Texture2DBuilder::buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture2D> texture(allocator, args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<ITexture>(texture));
}

NullableStrongRef<ITexture> DX10Texture2DBuilder::buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture2D> texture(allocator, args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<ITexture>(texture));
}

bool DX10Texture2DBuilder::processArgs(const TextureArgs& args, DXTextureArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = dxTextureFormat(args.dataFormat);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

    D3D10_SUBRESOURCE_DATA initialDesc;
    initialDesc.pSysMem = args.initialBuffer;
    initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
    initialDesc.SysMemSlicePitch = 0;

    HRESULT res = _ctx.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer ? &initialDesc : NULL, &dxArgs->d3dTexture);

    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels = -1;

    res = _ctx.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &viewDesc, &dxArgs->d3dTextureView);

    if(FAILED(res))
    {
        dxArgs->d3dTexture->Release();
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DX10TextureCube* DX10TextureCubeBuilder::build(const TextureCubeArgs& args, Error* error) const noexcept
{
    DXTextureCubeArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCube* const texture = new(::std::nothrow) DX10TextureCube(args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

DX10TextureCube* DX10TextureCubeBuilder::build(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureCubeArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCube* const texture = allocator.allocateT<DX10TextureCube>(args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITextureCube> DX10TextureCubeBuilder::buildCPPRef(const TextureCubeArgs& args, Error* error) const noexcept
{
    DXTextureCubeArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10TextureCube> texture = CPPRef<DX10TextureCube>(new(::std::nothrow) DX10TextureCube(args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels));

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITextureCube> DX10TextureCubeBuilder::buildTauRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureCubeArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10TextureCube> texture(allocator, args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<ITextureCube>(texture));
}

NullableStrongRef<ITextureCube> DX10TextureCubeBuilder::buildTauSRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureCubeArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10TextureCube> texture(allocator, args.width, args.height, args.dataFormat, _ctx, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<ITextureCube>(texture));
}

bool DX10TextureCubeBuilder::processArgs(const TextureCubeArgs& args, DXTextureCubeArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.Format = DX10Texture2DBuilder::dxTextureFormat(args.dataFormat);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS | D3D10_RESOURCE_MISC_TEXTURECUBE;

    D3D10_SUBRESOURCE_DATA initialDesc[6];
    for(uSys i = 0; i < 6; ++i)
    {
        initialDesc[i].pSysMem = args.initialBuffer[DX10TextureCube::dxCubeSide(static_cast<ETexture::CubeSide>(i + static_cast<uSys>(ETexture::CubeSide::Front)))];
        initialDesc[i].SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
        initialDesc[i].SysMemSlicePitch = 0;
    }

    HRESULT res = _ctx.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer[0] ? initialDesc : NULL, &dxArgs->d3dTexture);

    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MostDetailedMip = 0;
    viewDesc.TextureCube.MipLevels = -1;

    res = _ctx.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &viewDesc, &dxArgs->d3dTextureView);

    if(FAILED(res))
    {
        dxArgs->d3dTexture->Release();
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DXGI_FORMAT DX10Texture2DBuilder::dxTextureFormat(const ETexture::Format format) noexcept
{
    switch(format)
    {
        case ETexture::Format::Red8UnsignedInt:                 return DXGI_FORMAT_R8_UNORM;
        case ETexture::Format::Red16UnsignedInt:                return DXGI_FORMAT_R16_UNORM;
        case ETexture::Format::Red32UnsignedInt:                return DXGI_FORMAT_R32_UINT;
        case ETexture::Format::RedGreen8UnsignedInt:            return DXGI_FORMAT_R8G8_UNORM;
        case ETexture::Format::RedGreen16UnsignedInt:           return DXGI_FORMAT_R16G16_UNORM;
        case ETexture::Format::RedGreen32UnsignedInt:           return DXGI_FORMAT_R32G32_UINT;
        // case ETexture::Format::RedGreenBlue8UnsignedInt:        return DXGI_FORMAT_R8G8B8_UINT;
        // case ETexture::Format::RedGreenBlue16UnsignedInt:       return DXGI_FORMAT_R16G16B16_UINT;
        case ETexture::Format::RedGreenBlue32UnsignedInt:       return DXGI_FORMAT_R32G32B32_UINT;
        case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:   return DXGI_FORMAT_R8G8B8A8_UNORM;
        case ETexture::Format::RedGreenBlueAlpha16UnsignedInt:  return DXGI_FORMAT_R16G16B16A16_UNORM;
        case ETexture::Format::RedGreenBlueAlpha32UnsignedInt:  return DXGI_FORMAT_R32G32B32A32_UINT;
        case ETexture::Format::Red16Float:                      return DXGI_FORMAT_R16_FLOAT;
        case ETexture::Format::Red32Float:                      return DXGI_FORMAT_R32_FLOAT;
        case ETexture::Format::RedGreen16Float:                 return DXGI_FORMAT_R16G16_FLOAT;
        case ETexture::Format::RedGreen32Float:                 return DXGI_FORMAT_R32G32_FLOAT;
        // case ETexture::Format::RedGreenBlue16Float:             return DXGI_FORMAT_R16G16B16_FLOAT;
        case ETexture::Format::RedGreenBlue32Float:             return DXGI_FORMAT_R32G32B32_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha16Float:        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha32Float:        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ETexture::Format::Depth16UnsignedInt:              return DXGI_FORMAT_D16_UNORM;
        // case ETexture::Format::Depth32UnsignedInt:              return DXGI_FORMAT_D32;
        case ETexture::Format::Depth32Float:                    return DXGI_FORMAT_D32_FLOAT;
        // case ETexture::Format::Stencil8UnsignedInt:             return DXGI_FORMAT_S8;
        // case ETexture::Format::Stencil16UnsignedInt:            return DXGI_FORMAT_;
        case ETexture::Format::Depth24Stencil8:                 return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case ETexture::Format::Depth32FloatStencil8UnsignedInt: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        default: return DXGI_FORMAT_UNKNOWN;
    }
}

UINT DX10TextureCube::dxCubeSide(const ETexture::CubeSide side) noexcept
{
    switch(side)
    {
        case ETexture::CubeSide::Front:  return 5;
        case ETexture::CubeSide::Back:   return 4;
        case ETexture::CubeSide::Left:   return 1;
        case ETexture::CubeSide::Right:  return 0;
        case ETexture::CubeSide::Top:    return 2;
        case ETexture::CubeSide::Bottom: return 3;
        default: return 0;
    }
}
#endif
