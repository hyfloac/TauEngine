#include "dx/dx10/DX10Texture.hpp"
#include <d3d9.h>

#include "TauEngine.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"

void DX10NullTexture::bind(IRenderingContext& context, const u8 textureUnit, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ID3D10ShaderResourceView* nullSRV = null;

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        default: return;
    }
}

void DX10NullTexture::unbind(IRenderingContext& context, const u8 textureUnit, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ID3D10ShaderResourceView* nullSRV = null;

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        default: return;
    }
}

DX10Texture2D::~DX10Texture2D() noexcept
{
    _d3dTexture->Release();
    _d3dTextureView->Release();
    _d3dTexture = null;
    _d3dTextureView = null;
}

void DX10Texture2D::set(IRenderingContext& context, const u32 level, const void* const data) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(level, 0, _mipLevels), NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
}

void DX10Texture2D::bind(IRenderingContext& context, const u8 textureUnit, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        default: return;
    }
}

void DX10Texture2D::unbind(IRenderingContext& context, const u8 textureUnit, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ID3D10ShaderResourceView* nullSRV = null;

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        default: return;
    }
}

void DX10Texture2D::generateMipmaps(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->GenerateMips(_d3dTextureView);
}

DX10DepthTexture::~DX10DepthTexture() noexcept
{
    _d3dDepthStencilView->Release();
    _d3dDepthStencilView = null;
}

DX10TextureCube::~DX10TextureCube() noexcept
{
    _d3dTexture->Release();
    _d3dTextureView->Release();
    _d3dTexture = null;
    _d3dTextureView = null;
}

void DX10TextureCube::bind(IRenderingContext& context, const u8 textureUnit, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
            break;
        default: return;
    }
    ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
}

void DX10TextureCube::unbind(IRenderingContext& context, const u8 textureUnit, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ID3D10ShaderResourceView* nullSRV = null;

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &nullSRV);
            break;
        default: return;
    }
    ctx.d3dDevice()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
}

void DX10TextureCube::generateMipmaps(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->GenerateMips(_d3dTextureView);
}

void DX10TextureCube::setCube(IRenderingContext& context, const u32 level, const ETexture::CubeSide side, const void* const data) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(level, dxCubeSide(side), _mipLevels), NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
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

    DX10Texture2D* const texture = new(::std::nothrow) DX10Texture2D(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    DX10Texture2D* const texture = allocator.allocateT<DX10Texture2D>(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    const CPPRef<DX10Texture2D> texture = CPPRef<DX10Texture2D>(new(::std::nothrow) DX10Texture2D(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels));

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

    const NullableRef<DX10Texture2D> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    const NullableStrongRef<DX10Texture2D> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer ? &initialDesc : NULL, &dxArgs->d3dTexture);

    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels = -1;

    res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &viewDesc, &dxArgs->d3dTextureView);

    if(FAILED(res))
    {
        dxArgs->d3dTexture->Release();
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DX10DepthTexture* DX10DepthTextureBuilder::build(const TextureArgs& args, Error* error) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10DepthTexture* const texture = new(::std::nothrow) DX10DepthTexture(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, dxArgs.d3dDepthStencilView);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        dxArgs.d3dDepthStencilView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

DX10DepthTexture* DX10DepthTextureBuilder::build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10DepthTexture* const texture = allocator.allocateT<DX10DepthTexture>(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, dxArgs.d3dDepthStencilView);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        dxArgs.d3dDepthStencilView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture> DX10DepthTextureBuilder::buildCPPRef(const TextureArgs& args, Error* error) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10DepthTexture> texture = CPPRef<DX10DepthTexture>(new(::std::nothrow) DX10DepthTexture(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, dxArgs.d3dDepthStencilView));

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        dxArgs.d3dDepthStencilView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture> DX10DepthTextureBuilder::buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10DepthTexture> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, dxArgs.d3dDepthStencilView);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        dxArgs.d3dDepthStencilView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<ITexture>(texture));
}

NullableStrongRef<ITexture> DX10DepthTextureBuilder::buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10DepthTexture> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, dxArgs.d3dDepthStencilView);

    if(!texture)
    {
        dxArgs.d3dTexture->Release();
        dxArgs.d3dTextureView->Release();
        dxArgs.d3dDepthStencilView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<ITexture>(texture));
}

bool DX10DepthTextureBuilder::processArgs(const TextureArgs& args, DXTextureArgs* dxArgs, Error* error) const noexcept
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
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D10_SUBRESOURCE_DATA initialDesc;
    initialDesc.pSysMem = args.initialBuffer;
    initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
    initialDesc.SysMemSlicePitch = 0;

    HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer ? &initialDesc : NULL, &dxArgs->d3dTexture);

    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels = -1;

    res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &viewDesc, &dxArgs->d3dTextureView);
    if(FAILED(res))
    {
        dxArgs->d3dTexture->Release();
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    D3D10_DEPTH_STENCIL_VIEW_DESC dsDesc;
    dsDesc.Format = textureDesc.Format;
    dsDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
    dsDesc.Texture2D.MipSlice = 0;

    res = _gi.d3d10Device()->CreateDepthStencilView(dxArgs->d3dTexture, &dsDesc, &dxArgs->d3dDepthStencilView);
    if(FAILED(res))
    {
        dxArgs->d3dTexture->Release();
        dxArgs->d3dTextureView->Release();
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DX10TextureCube* DX10TextureCubeBuilder::build(const TextureCubeArgs& args, Error* error) const noexcept
{
    DXTextureCubeArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCube* const texture = new(::std::nothrow) DX10TextureCube(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    DX10TextureCube* const texture = allocator.allocateT<DX10TextureCube>(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    const CPPRef<DX10TextureCube> texture = CPPRef<DX10TextureCube>(new(::std::nothrow) DX10TextureCube(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels));

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

    const NullableRef<DX10TextureCube> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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

    const NullableStrongRef<DX10TextureCube> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.d3dTextureView, args.mipmapLevels);

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
        const uSys index = DX10TextureCube::dxCubeSide(static_cast<ETexture::CubeSide>(i + static_cast<uSys>(ETexture::CubeSide::Front)));
        initialDesc[index].pSysMem = args.initialBuffer[i];
        initialDesc[index].SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
        initialDesc[index].SysMemSlicePitch = 0;
    }

    HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer[0] ? initialDesc : NULL, &dxArgs->d3dTexture);

    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MostDetailedMip = 0;
    viewDesc.TextureCube.MipLevels = -1;

    res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &viewDesc, &dxArgs->d3dTextureView);

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
