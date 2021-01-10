#include "dx/dx10/DX10Resource.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10ResourceTexture.hpp"
#include "dx/dx10/DX10TextureView.hpp"
#include "TauConfig.hpp"

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceBufferArgs& args, ResourceHeap, Error* const error, TauAllocator& allocator) const noexcept
{
    DXResourceBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10ResourceBuffer> buffer(allocator, args, dxArgs.d3dBuffer);
    ERROR_CODE_COND_N(!buffer, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap, Error* const error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture1DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap, Error* const error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture2DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<IResource> DX10ResourceBuilder::buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap, Error* const error, TauAllocator& allocator) const noexcept
{
    DXResourceTexture3DArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10Resource> texture(allocator, args, dxArgs.d3dTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

bool DX10ResourceBuilder::processArgs(const ResourceBufferArgs& args, DXResourceBufferArgs* const dxArgs, Error* const error) const noexcept
{
#if TAU_GENERAL_SAFETY_CHECK
    ERROR_CODE_COND_F(args.size == 0, Error::InvalidSize);
    ERROR_CODE_COND_F(args.bufferType < EBuffer::Type::MIN || args.bufferType > EBuffer::Type::MAX, Error::InvalidBufferType);
#endif

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = static_cast<UINT>(args.size);
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
        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, nullptr, &dxArgs->d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10ResourceBuilder::processArgs(const ResourceTexture1DArgs& args, DXResourceTexture1DArgs* const dxArgs, Error* const error) const noexcept
{
#if TAU_GENERAL_SAFETY_CHECK
    ERROR_CODE_COND_F(args.width == 0, Error::InvalidWidth);
    ERROR_CODE_COND_F(args.arrayCount == 0, Error::InvalidArrayCount);
#endif

    D3D10_TEXTURE1D_DESC textureDesc;
    textureDesc.Width = static_cast<UINT>(args.width);
    textureDesc.MipLevels = args.mipLevels;
    textureDesc.ArraySize = args.arrayCount;
    textureDesc.Format = DX10TextureViewBuilder::dxTextureFormat(args.dataFormat);
    textureDesc.Usage = dxUsage(args.usageType);
    textureDesc.BindFlags = DX10TextureViewBuilder::dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = DX10TextureViewBuilder::dxMiscFlags(args.flags);

    if(args.initialBuffers)
    {
        if((args.mipLevels == 0 || args.mipLevels == 1) && args.arrayCount == 1)
        {
            D3D10_SUBRESOURCE_DATA initialDesc;
            initialDesc.pSysMem = args.initialBuffers[0];
            initialDesc.SysMemPitch = static_cast<UINT>(args.width * ETexture::bytesPerPixel(args.dataFormat));
            initialDesc.SysMemSlicePitch = 0;

            const HRESULT res = _gi.d3d10Device()->CreateTexture1D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
            ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
        }
        else
        {
            const uSys mipLevels = args.mipLevels == 0 ? 1 : args.mipLevels;

            const uSys subResourceCount = mipLevels * args.arrayCount;
            D3D10_SUBRESOURCE_DATA* initialDescriptors = new(::std::nothrow) D3D10_SUBRESOURCE_DATA[subResourceCount];

            const uSys bpp = ETexture::bytesPerPixel(args.dataFormat);
            uSys width = args.width;
            for(uSys i = 0; i < subResourceCount; ++i)
            {
                initialDescriptors[i].pSysMem = args.initialBuffers[i];
                initialDescriptors[i].SysMemPitch = static_cast<UINT>(width * bpp);
                initialDescriptors[i].SysMemSlicePitch = 0;

                width = ETexture::computeMipSide(width);
            }

            const HRESULT res = _gi.d3d10Device()->CreateTexture1D(&textureDesc, initialDescriptors, &dxArgs->d3dTexture);
            ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
        }
    }
    else
    {
        const HRESULT res = _gi.d3d10Device()->CreateTexture1D(&textureDesc, nullptr, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10ResourceBuilder::processArgs(const ResourceTexture2DArgs& args, DXResourceTexture2DArgs* const dxArgs, Error* const error) const noexcept
{
#if TAU_GENERAL_SAFETY_CHECK
    ERROR_CODE_COND_F(args.width == 0, Error::InvalidWidth);
    ERROR_CODE_COND_F(args.height == 0, Error::InvalidHeight);
    ERROR_CODE_COND_F(args.arrayCount == 0, Error::InvalidArrayCount);
#endif

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = static_cast<UINT>(args.width);
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

    if(args.initialBuffers)
    {
        if((args.mipLevels == 0 || args.mipLevels == 1) && args.arrayCount == 1)
        {
            D3D10_SUBRESOURCE_DATA initialDesc;
            initialDesc.pSysMem = args.initialBuffers[0];
            initialDesc.SysMemPitch = 0;
            initialDesc.SysMemSlicePitch = 0;

            const HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
            ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
        }
        else
        {
            const uSys mipLevels = args.mipLevels == 0 ? 1 : args.mipLevels;

            const uSys subResourceCount = mipLevels * args.arrayCount;
            D3D10_SUBRESOURCE_DATA* initialDescriptors = new(::std::nothrow) D3D10_SUBRESOURCE_DATA[subResourceCount];

            for(uSys i = 0; i < subResourceCount; ++i)
            {
                initialDescriptors[i].pSysMem = args.initialBuffers[i];
                initialDescriptors[i].SysMemPitch = 0;
                initialDescriptors[i].SysMemSlicePitch = 0;
            }

            const HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, initialDescriptors, &dxArgs->d3dTexture);
            ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
        }
    }
    else
    {
        const HRESULT res = _gi.d3d10Device()->CreateTexture2D(&textureDesc, nullptr, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10ResourceBuilder::processArgs(const ResourceTexture3DArgs& args, DXResourceTexture3DArgs* const dxArgs, Error* const error) const noexcept
{
#if TAU_GENERAL_SAFETY_CHECK
    ERROR_CODE_COND_F(args.width == 0, Error::InvalidWidth);
    ERROR_CODE_COND_F(args.height == 0, Error::InvalidHeight);
    ERROR_CODE_COND_F(args.depth == 0, Error::InvalidDepth);
#endif

    D3D10_TEXTURE3D_DESC textureDesc;
    textureDesc.Width = static_cast<UINT>(args.width);
    textureDesc.Height = args.height;
    textureDesc.Depth = args.depth;
    textureDesc.MipLevels = args.mipLevels;
    textureDesc.Format = DX10TextureViewBuilder::dxTextureFormat(args.dataFormat);
    textureDesc.Usage = dxUsage(args.usageType);
    textureDesc.BindFlags = DX10TextureViewBuilder::dxBindFlags(args.flags);
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = DX10TextureViewBuilder::dxMiscFlags(args.flags);

    if(args.initialBuffers)
    {
        if((args.mipLevels == 0 || args.mipLevels == 1) && args.depth == 1)
        {
            D3D10_SUBRESOURCE_DATA initialDesc;
            initialDesc.pSysMem = args.initialBuffers[0];
            initialDesc.SysMemPitch = static_cast<UINT>(args.width * ETexture::bytesPerPixel(args.dataFormat));
            initialDesc.SysMemSlicePitch = 0;

            const HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
            ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
        }
        else
        {
            const uSys mipLevels = args.mipLevels == 0 ? 1 : args.mipLevels;

            const uSys subResourceCount = mipLevels * args.depth;
            D3D10_SUBRESOURCE_DATA* initialDescriptors = new(::std::nothrow) D3D10_SUBRESOURCE_DATA[subResourceCount];

            const uSys bpp = ETexture::bytesPerPixel(args.dataFormat);
            uSys width = args.width;
            uSys height = args.height;
            for(uSys i = 0; i < subResourceCount; ++i)
            {
                initialDescriptors[i].pSysMem = args.initialBuffers[i];
                initialDescriptors[i].SysMemPitch = static_cast<UINT>(width * bpp);
                initialDescriptors[i].SysMemSlicePitch = static_cast<UINT>(width * height * bpp);

                width = ETexture::computeMipSide(width);
                height = ETexture::computeMipSide(height);
            }

            const HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, initialDescriptors, &dxArgs->d3dTexture);
            ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
        }
        D3D10_SUBRESOURCE_DATA initialDesc;
        initialDesc.pSysMem = args.initialBuffers;
        initialDesc.SysMemPitch = static_cast<UINT>(args.width * ETexture::bytesPerPixel(args.dataFormat));
        initialDesc.SysMemSlicePitch = 0;

        const HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, &initialDesc, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT res = _gi.d3d10Device()->CreateTexture3D(&textureDesc, nullptr, &dxArgs->d3dTexture);
        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

uSys DX10ResourceBuilder::_allocSize(const uSys type) const noexcept
{
    switch(type)
    {
        case RB_AS_BUFFER:     return NullableRef<DX10ResourceBuffer>::allocSize();
        case RB_AS_TEXTURE_1D: return NullableRef<DX10ResourceTexture1D>::allocSize();
        case RB_AS_TEXTURE_2D: return NullableRef<DX10ResourceTexture2D>::allocSize();
        case RB_AS_TEXTURE_3D: return NullableRef<DX10ResourceTexture3D>::allocSize();
        default: return 0;
    }
}

D3D10_USAGE DX10ResourceBuilder::dxUsage(const EResource::UsageType usage) noexcept
{
    switch(usage)
    {
        case EResource::UsageType::Default:   return D3D10_USAGE_DEFAULT;
        case EResource::UsageType::Upload:    return D3D10_USAGE_DYNAMIC;
        case EResource::UsageType::ReadBack:  return D3D10_USAGE_STAGING;
        default:                              return D3D10_USAGE_DEFAULT;
    }
}

D3D10_CPU_ACCESS_FLAG DX10ResourceBuilder::dxAccess(const EResource::UsageType usage) noexcept
{
    switch(usage)
    {
        case EResource::UsageType::Default:   return static_cast<D3D10_CPU_ACCESS_FLAG>(0);
        case EResource::UsageType::Upload:    return D3D10_CPU_ACCESS_WRITE;
        case EResource::UsageType::ReadBack:  return D3D10_CPU_ACCESS_READ;
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
