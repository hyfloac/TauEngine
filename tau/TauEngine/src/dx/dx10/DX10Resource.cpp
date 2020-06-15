#include "dx/dx10/DX10Resource.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10ResourceTexture.hpp"
#include "dx/dx10/DX10TextureView.hpp"

DX10Resource* DX10ResourceBuilder::build(const ResourceBufferArgs& args, Error* error) const noexcept
{
    DXResourceBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10ResourceBuffer* const buffer = new(::std::nothrow) DX10ResourceBuffer(args, dxArgs.d3dBuffer);
    ERROR_CODE_COND_N(!buffer, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, buffer);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10ResourceBuffer* const buffer = allocator.allocateT<DX10ResourceBuffer>(args, dxArgs.d3dBuffer);
    ERROR_CODE_COND_N(!buffer, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IResource> DX10ResourceBuilder::buildCPPRef(const ResourceBufferArgs& args, Error* error) const noexcept
{
    DXResourceBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10ResourceBuffer> buffer(new(::std::nothrow) DX10ResourceBuffer(args, dxArgs.d3dBuffer));
    ERROR_CODE_COND_N(!buffer, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10ResourceBuffer> buffer(allocator, args, dxArgs.d3dBuffer);
    ERROR_CODE_COND_N(!buffer, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableStrongRef<IResource> DX10ResourceBuilder::buildTauSRef(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10ResourceBuffer> buffer(allocator, args, dxArgs.d3dBuffer);
    ERROR_CODE_COND_N(!buffer, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, buffer);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceTexture1DArgs& args, Error* error) const noexcept
{
    DXResourceTexture1DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Resource* const texture = new(::std::nothrow) DX10ResourceTexture1D(args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceTexture1DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture1DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Resource* const texture = allocator.allocateT<DX10ResourceTexture1D>(args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<IResource> DX10ResourceBuilder::buildCPPRef(const ResourceTexture1DArgs& args, Error* error) const noexcept
{
    DXResourceTexture1DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Resource> texture(new(::std::nothrow) DX10ResourceTexture1D(args, dxArgs.d3dTexture));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceTexture1DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture1DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<IResource> DX10ResourceBuilder::buildTauSRef(const ResourceTexture1DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture1DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceTexture2DArgs& args, Error* error) const noexcept
{
    DXResourceTexture2DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Resource* const texture = new(::std::nothrow) DX10ResourceTexture2D(args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceTexture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture2DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Resource* const texture = allocator.allocateT<DX10ResourceTexture2D>(args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<IResource> DX10ResourceBuilder::buildCPPRef(const ResourceTexture2DArgs& args, Error* error) const noexcept
{
    DXResourceTexture2DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Resource> texture(new(::std::nothrow) DX10ResourceTexture2D(args, dxArgs.d3dTexture));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceTexture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture2DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<IResource> DX10ResourceBuilder::buildTauSRef(const ResourceTexture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture2DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceTexture3DArgs& args, Error* error) const noexcept
{
    DXResourceTexture3DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Resource* const texture = new(::std::nothrow) DX10ResourceTexture3D(args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

DX10Resource* DX10ResourceBuilder::build(const ResourceTexture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture3DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10Resource* const texture = allocator.allocateT<DX10ResourceTexture3D>(args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<IResource> DX10ResourceBuilder::buildCPPRef(const ResourceTexture3DArgs& args, Error* error) const noexcept
{
    DXResourceTexture3DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10Resource> texture(new(::std::nothrow) DX10ResourceTexture3D(args, dxArgs.d3dTexture));
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceTexture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture3DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<IResource> DX10ResourceBuilder::buildTauSRef(const ResourceTexture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture3DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

bool DX10ResourceBuilder::processArgs(const ResourceBufferArgs& args, DXResourceBufferArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.size == 0, Error::InvalidSize);
    ERROR_CODE_COND_F(args.bufferType < EBuffer::Type::MIN || args.bufferType > EBuffer::Type::MAX, Error::InvalidBufferType);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.size;
    bufferDesc.Usage = dxUsage(args.usageType);
    bufferDesc.BindFlags = dxBind(args.bufferType);
    bufferDesc.CPUAccessFlags = dxAccess(args.usageType);
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, &initialBuffer, &dxArgs->d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, NULL, &dxArgs->d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10ResourceBuilder::processArgs(const ResourceTexture1DArgs& args, DXResourceTexture1DArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::InvalidWidth);
    ERROR_CODE_COND_F(args.arrayCount == 0, Error::InvalidArrayCount);

    D3D10_TEXTURE1D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.MipLevels = args.mipLevels;
    textureDesc.ArraySize = args.arrayCount;
    textureDesc.Format = DX10TextureViewBuilder::dxTextureFormat(args.dataFormat);
    textureDesc.Usage = dxUsage(args.usageType);
    textureDesc.BindFlags = DX10TextureViewBuilder::dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = DX10TextureViewBuilder::dxMiscFlags(args.flags);

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialDesc;
        initialDesc.pSysMem = args.initialBuffer;
        initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
        initialDesc.SysMemSlicePitch = 0;

        const HRESULT res = _gi.d3d10Device()->CreateTexture1D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT res = _gi.d3d10Device()->CreateTexture1D(&textureDesc, NULL, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10ResourceBuilder::processArgs(const ResourceTexture2DArgs& args, DXResourceTexture2DArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::InvalidWidth);
    ERROR_CODE_COND_F(args.height == 0, Error::InvalidHeight);
    ERROR_CODE_COND_F(args.arrayCount == 0, Error::InvalidArrayCount);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.MipLevels = args.mipLevels;
    textureDesc.ArraySize = args.arrayCount;
    textureDesc.Format = DX10TextureViewBuilder::dxTextureFormat(args.dataFormat);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = dxUsage(args.usageType);
    textureDesc.BindFlags = DX10TextureViewBuilder::dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = DX10TextureViewBuilder::dxMiscFlags(args.flags);

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialDesc;
        initialDesc.pSysMem = args.initialBuffer;
        initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
        initialDesc.SysMemSlicePitch = 0;

        const HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, NULL, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10ResourceBuilder::processArgs(const ResourceTexture3DArgs& args, DXResourceTexture3DArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::InvalidWidth);
    ERROR_CODE_COND_F(args.height == 0, Error::InvalidHeight);
    ERROR_CODE_COND_F(args.depth == 0, Error::InvalidDepth);

    D3D10_TEXTURE3D_DESC textureDesc;
    textureDesc.Width = args.width;
    textureDesc.Height = args.height;
    textureDesc.Depth = args.depth;
    textureDesc.MipLevels = args.mipLevels;
    textureDesc.Format = DX10TextureViewBuilder::dxTextureFormat(args.dataFormat);
    textureDesc.Usage = dxUsage(args.usageType);
    textureDesc.BindFlags = DX10TextureViewBuilder::dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = DX10TextureViewBuilder::dxMiscFlags(args.flags);

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialDesc;
        initialDesc.pSysMem = args.initialBuffer;
        initialDesc.SysMemPitch = args.width * ETexture::bytesPerPixel(args.dataFormat);
        initialDesc.SysMemSlicePitch = 0;

        const HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, NULL, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

D3D10_USAGE DX10ResourceBuilder::dxUsage(const EResource::UsageType usage) noexcept
{
    switch(usage)
    {
        case EResource::UsageType::Default:   return D3D10_USAGE_DEFAULT;
        case EResource::UsageType::Immutable: return D3D10_USAGE_IMMUTABLE;
        case EResource::UsageType::Dynamic:   return D3D10_USAGE_DYNAMIC;
        case EResource::UsageType::Readable:  return D3D10_USAGE_STAGING;
        default:                              return D3D10_USAGE_DEFAULT;
    }
}

D3D10_CPU_ACCESS_FLAG DX10ResourceBuilder::dxAccess(const EResource::UsageType usage) noexcept
{
    switch(usage)
    {
        case EResource::UsageType::Default:   
        case EResource::UsageType::Immutable: 
        case EResource::UsageType::Dynamic:   return D3D10_CPU_ACCESS_WRITE;
        case EResource::UsageType::Readable:  return D3D10_CPU_ACCESS_READ;
        default:                              return static_cast<D3D10_CPU_ACCESS_FLAG>(0);
    }
}

D3D10_BIND_FLAG DX10ResourceBuilder::dxBind(const EBuffer::Type type) noexcept
{
    switch(type)
    {
        case EBuffer::Type::Vertex:  return D3D10_BIND_VERTEX_BUFFER;
        case EBuffer::Type::Index:   return D3D10_BIND_INDEX_BUFFER;
        case EBuffer::Type::Uniform: return D3D10_BIND_CONSTANT_BUFFER;
        default: return static_cast<D3D10_BIND_FLAG>(0);
    }
}
#endif
