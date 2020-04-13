#include "dx/dx10/DX10Texture.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauEngine.hpp"
#include <EnumBitFields.hpp>

DX10Texture2D::~DX10Texture2D() noexcept
{
    _d3dTexture->Release();
    _d3dTexture = null;
}

DX10Texture3D::~DX10Texture3D() noexcept
{
    _d3dTexture->Release();
    _d3dTexture = null;
}

DX10TextureCube::~DX10TextureCube() noexcept
{
    _d3dTexture->Release();
    _d3dTexture = null;
}

DX10TextureDepthStencil::~DX10TextureDepthStencil() noexcept
{
    _d3dTexture->Release();
    _d3dTexture = null;
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

void DX10Texture3D::set(IRenderingContext& context, const u32 depthLevel, const u32 level, const void* const data) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(level, depthLevel, _mipLevels), NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
}

void DX10TextureCube::set(IRenderingContext& context, const u32 level, const ETexture::CubeSide side, const void* const data) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(level, dxCubeSide(side), _mipLevels), NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
}

void DX10TextureDepthStencil::set(IRenderingContext& context, const void* const data) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->UpdateSubresource(_d3dTexture, D3D10CalcSubresource(0, 0, 1), NULL, data, _width * ETexture::bytesPerPixel(ETexture::Format::Depth24Stencil8), 0);
}

DX10Texture2D* DX10TextureBuilder::build(const Texture2DArgs& args, Error* error) const noexcept
{
    DXTexture2DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2D* const texture = new(::std::nothrow) DX10Texture2D(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Texture2D* DX10TextureBuilder::build(const Texture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTexture2DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Texture2D* const texture = allocator.allocateT<DX10Texture2D>(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture2D> DX10TextureBuilder::buildCPPRef(const Texture2DArgs& args, Error* error) const noexcept
{
    DXTexture2DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Texture2D> texture = CPPRef<DX10Texture2D>(new(::std::nothrow) DX10Texture2D(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture2D> DX10TextureBuilder::buildTauRef(const Texture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTexture2DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Texture2D> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITexture2D> DX10TextureBuilder::buildTauSRef(const Texture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTexture2DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Texture2D> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Texture3D* DX10TextureBuilder::build(const Texture3DArgs& args, Error* error) const noexcept
{
    DXTexture3DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    {
        return null;
    }

    DX10Texture3D* const texture = new(::std::nothrow) DX10Texture3D(args.width, args.height, args.depth, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Texture3D* DX10TextureBuilder::build(const Texture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTexture3DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    {
        return null;
    }

    DX10Texture3D* const texture = allocator.allocateT<DX10Texture3D>(args.width, args.height, args.depth, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture3D> DX10TextureBuilder::buildCPPRef(const Texture3DArgs& args, Error* error) const noexcept
{
    DXTexture3DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    {
        return null;
    }

    const CPPRef<DX10Texture3D> texture = CPPRef<DX10Texture3D>(new(::std::nothrow) DX10Texture3D(args.width, args.height, args.depth, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture3D> DX10TextureBuilder::buildTauRef(const Texture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTexture3DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    {
        return null;
    }

    const NullableRef<DX10Texture3D> texture(allocator, args.width, args.height, args.depth, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITexture3D> DX10TextureBuilder::buildTauSRef(const Texture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTexture3DArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    {
        return null;
    }

    const NullableStrongRef<DX10Texture3D> texture(allocator,  args.width, args.height, args.depth, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10TextureCube* DX10TextureBuilder::build(const TextureCubeArgs& args, Error* error) const noexcept
{
    DXTextureCubeArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCube* const texture = new(::std::nothrow) DX10TextureCube(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10TextureCube* DX10TextureBuilder::build(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureCubeArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureCube* const texture = allocator.allocateT<DX10TextureCube>(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITextureCube> DX10TextureBuilder::buildCPPRef(const TextureCubeArgs& args, Error* error) const noexcept
{
    DXTextureCubeArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10TextureCube> texture = CPPRef<DX10TextureCube>(new(::std::nothrow) DX10TextureCube(args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITextureCube> DX10TextureBuilder::buildTauRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureCubeArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10TextureCube> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITextureCube> DX10TextureBuilder::buildTauSRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureCubeArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10TextureCube> texture(allocator, args.width, args.height, args.dataFormat, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.textureView, args.mipmapLevels);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.textureView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10TextureDepthStencil* DX10TextureBuilder::build(const TextureDepthStencilArgs& args, Error* error) const noexcept
{
    DXTextureDepthStencilArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureDepthStencil* const texture = new(::std::nothrow) DX10TextureDepthStencil(args.width, args.height, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.depthView, dxArgs.stencilView);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.depthView = null;
    dxArgs.stencilView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

DX10TextureDepthStencil* DX10TextureBuilder::build(const TextureDepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureDepthStencilArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureDepthStencil* const texture = allocator.allocateT<DX10TextureDepthStencil>(args.width, args.height, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.depthView, dxArgs.stencilView);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.depthView = null;
    dxArgs.stencilView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITextureDepthStencil> DX10TextureBuilder::buildCPPRef(const TextureDepthStencilArgs& args, Error* error) const noexcept
{
    DXTextureDepthStencilArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10TextureDepthStencil> texture = CPPRef<DX10TextureDepthStencil>(new(::std::nothrow) DX10TextureDepthStencil(args.width, args.height, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.depthView, dxArgs.stencilView));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.depthView = null;
    dxArgs.stencilView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITextureDepthStencil> DX10TextureBuilder::buildTauRef(const TextureDepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureDepthStencilArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10TextureDepthStencil> texture(allocator, args.width, args.height, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.depthView, dxArgs.stencilView);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.depthView = null;
    dxArgs.stencilView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITextureDepthStencil> DX10TextureBuilder::buildTauSRef(const TextureDepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureDepthStencilArgs dxArgs { };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10TextureDepthStencil> texture(allocator, args.width, args.height, dxArgs.d3dTexture, dxArgs.renderTarget, dxArgs.depthView, dxArgs.stencilView);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dTexture = null;
    dxArgs.renderTarget = null;
    dxArgs.depthView = null;
    dxArgs.stencilView = null;

    ERROR_CODE_V(Error::NoError, texture);
}

bool DX10TextureBuilder::processArgs(const Texture2DArgs& args, DXTexture2DArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == ETexture::Format::Depth24Stencil8, Error::DataFormatIsInvalid);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.MipLevels = args.mipmapLevels;
    textureDesc.ArraySize = 1;
    textureDesc.Format = dxTextureFormat(args.dataFormat);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = dxMiscFlags(args.flags);

    D3D10_SUBRESOURCE_DATA initialDesc;
    initialDesc.pSysMem = args.initialBuffer;
    initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
    initialDesc.SysMemSlicePitch = 0;

    HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer ? &initialDesc : NULL, &dxArgs->d3dTexture);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    if(hasFlag(args.flags, ETexture::BindFlags::ShaderAccess))
    {
        D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = args.mipmapLevels;

        ID3D10ShaderResourceView* srv;
        res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &srvDesc, &srv);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

        if(hasFlag(args.flags, ETexture::BindFlags::GenerateMipmaps))
        {
            dxArgs->textureView = new(::std::nothrow) DX10TextureView(srv);
        }
        else
        {
            dxArgs->textureView = new(::std::nothrow) DX10NoMipmapTextureView(srv);
        }

        if(!dxArgs->textureView)
        {
            srv->Release();
            ERROR_CODE_F(Error::SystemMemoryAllocationFailure);
        }
    }

    if(hasFlag(args.flags, ETexture::BindFlags::RenderTarget))
    {
        D3D10_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = textureDesc.Format;
        rtvDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        res = _gi.d3d10Device()->CreateRenderTargetView(dxArgs->d3dTexture, &rtvDesc, &dxArgs->renderTarget);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10TextureBuilder::processArgs(const Texture3DArgs& args, DXTexture3DArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.depth == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == ETexture::Format::Depth24Stencil8, Error::DataFormatIsInvalid);

    D3D10_TEXTURE3D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.Depth = args.depth;
    textureDesc.MipLevels = args.mipmapLevels;
    textureDesc.Format = dxTextureFormat(args.dataFormat);
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = dxMiscFlags(args.flags);

    HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, NULL, &dxArgs->d3dTexture);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    if(hasFlag(args.flags, ETexture::BindFlags::ShaderAccess))
    {
        D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE3D;
        srvDesc.Texture3D.MostDetailedMip = 0;
        srvDesc.Texture3D.MipLevels = args.mipmapLevels;

        ID3D10ShaderResourceView* srv;
        res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &srvDesc, &srv);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

        if(hasFlag(args.flags, ETexture::BindFlags::GenerateMipmaps))
        {
            dxArgs->textureView = new(::std::nothrow) DX10TextureView(srv);
        }
        else
        {
            dxArgs->textureView = new(::std::nothrow) DX10NoMipmapTextureView(srv);
        }

        if(!dxArgs->textureView)
        {
            srv->Release();
            ERROR_CODE_F(Error::SystemMemoryAllocationFailure);
        }
    }

    if(hasFlag(args.flags, ETexture::BindFlags::RenderTarget))
    {
        D3D10_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = textureDesc.Format;
        rtvDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE3D;
        rtvDesc.Texture3D.MipSlice = 0;
        rtvDesc.Texture3D.FirstWSlice = 0;
        rtvDesc.Texture3D.FirstWSlice = -1;

        res = _gi.d3d10Device()->CreateRenderTargetView(dxArgs->d3dTexture, &rtvDesc, &dxArgs->renderTarget);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10TextureBuilder::processArgs(const TextureCubeArgs& args, DXTextureCubeArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == ETexture::Format::Depth24Stencil8, Error::DataFormatIsInvalid);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.MipLevels = args.mipmapLevels;
    textureDesc.ArraySize = 6;
    textureDesc.Format = dxTextureFormat(args.dataFormat);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = dxMiscFlags(args.flags) | D3D10_RESOURCE_MISC_TEXTURECUBE;

    D3D10_SUBRESOURCE_DATA initialDesc[6];
    for(uSys i = 0; i < 6; ++i)
    {
        const uSys index = DX10TextureCube::dxCubeSide(static_cast<ETexture::CubeSide>(i + static_cast<uSys>(ETexture::CubeSide::Front)));
        initialDesc[index].pSysMem = args.initialBuffers[i];
        initialDesc[index].SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
        initialDesc[index].SysMemSlicePitch = 0;
    }

    HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffers[0] ? initialDesc : NULL, &dxArgs->d3dTexture);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    if(hasFlag(args.flags, ETexture::BindFlags::ShaderAccess))
    {
        D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
        srvDesc.TextureCube.MostDetailedMip = 0;
        srvDesc.TextureCube.MipLevels = args.mipmapLevels;

        ID3D10ShaderResourceView* srv;
        res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &srvDesc, &srv);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

        if(hasFlag(args.flags, ETexture::BindFlags::GenerateMipmaps))
        {
            dxArgs->textureView = new(::std::nothrow) DX10TextureView(srv);
        }
        else
        {
            dxArgs->textureView = new(::std::nothrow) DX10NoMipmapTextureView(srv);
        }

        if(!dxArgs->textureView)
        {
            srv->Release();
            ERROR_CODE_F(Error::SystemMemoryAllocationFailure);
        }
    }

    if(hasFlag(args.flags, ETexture::BindFlags::RenderTarget))
    {
        D3D10_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = textureDesc.Format;
        rtvDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DARRAY;
        rtvDesc.Texture2DArray.MipSlice = 0;
        rtvDesc.Texture2DArray.FirstArraySlice = 0;
        rtvDesc.Texture2DArray.ArraySize = 6;

        res = _gi.d3d10Device()->CreateRenderTargetView(dxArgs->d3dTexture, &rtvDesc, &dxArgs->renderTarget);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10TextureBuilder::processArgs(const TextureDepthStencilArgs& args, DXTextureDepthStencilArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = dxMiscFlags(args.flags);

    D3D10_SUBRESOURCE_DATA initialDesc;
    initialDesc.pSysMem = args.initialBuffer;
    initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(ETexture::Format::Depth24Stencil8);
    initialDesc.SysMemSlicePitch = 0;

    HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, args.initialBuffer ? &initialDesc : NULL, &dxArgs->d3dTexture);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    if(hasFlag(args.flags, ETexture::DepthStencilBindFlags::DepthShaderAccess))
    {
        D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &srvDesc, &dxArgs->depthView);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    if(hasFlag(args.flags, ETexture::DepthStencilBindFlags::StencilShaderAccess))
    {
        D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        res = _gi.d3d10Device()->CreateShaderResourceView(dxArgs->d3dTexture, &srvDesc, &dxArgs->stencilView);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    if(hasFlag(args.flags, ETexture::DepthStencilBindFlags::RenderTarget))
    {
        D3D10_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;

        res = _gi.d3d10Device()->CreateDepthStencilView(dxArgs->d3dTexture, &dsvDesc, &dxArgs->renderTarget);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DXGI_FORMAT DX10TextureBuilder::dxTextureFormat(const ETexture::Format format) noexcept
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
        default: return DXGI_FORMAT_UNKNOWN;
    }
}

D3D10_BIND_FLAG DX10TextureBuilder::dxBindFlags(const ETexture::BindFlags flags) noexcept
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

D3D10_BIND_FLAG DX10TextureBuilder::dxBindFlags(const ETexture::DepthStencilBindFlags flags) noexcept
{
    D3D10_BIND_FLAG ret = static_cast<D3D10_BIND_FLAG>(0);

    if(hasFlag(flags, ETexture::DepthStencilBindFlags::RenderTarget))
    {
        ret |= D3D10_BIND_DEPTH_STENCIL;
    }
    if(hasFlag(flags, ETexture::DepthStencilBindFlags::DepthShaderAccess) ||
       hasFlag(flags, ETexture::DepthStencilBindFlags::StencilShaderAccess))
    {
        ret |= D3D10_BIND_SHADER_RESOURCE;
    }

    return ret;
}

D3D10_RESOURCE_MISC_FLAG DX10TextureBuilder::dxMiscFlags(const ETexture::BindFlags flags) noexcept
{
    if(hasFlag(flags, ETexture::BindFlags::GenerateMipmaps) &&
       hasFlag(flags, ETexture::BindFlags::ShaderAccess))
    {
        return D3D10_RESOURCE_MISC_GENERATE_MIPS;
    }

    return static_cast<D3D10_RESOURCE_MISC_FLAG>(0);
}

D3D10_RESOURCE_MISC_FLAG DX10TextureBuilder::dxMiscFlags(const ETexture::DepthStencilBindFlags) noexcept
{
    return static_cast<D3D10_RESOURCE_MISC_FLAG>(0);
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
