#include "dx/dx10/DX10Resource.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"

bool DX10ResourceBuilder::processArgs(const ResourceBufferArgs& args, DXResourceBufferArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.size == 0, Error::InvalidSize);
    ERROR_CODE_COND_F(args.bufferType < EBuffer::Type::MIN || args.bufferType > EBuffer::Type::MAX, Error::InvalidBufferType);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.size;
    bufferDesc.Usage = getDXUsage(args.usageType);
    bufferDesc.BindFlags = getDXBind(args.bufferType);
    bufferDesc.CPUAccessFlags = getDXAccess(args.usageType);
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

D3D10_USAGE DX10ResourceBuilder::getDXUsage(const EResource::UsageType usage) noexcept
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

D3D10_CPU_ACCESS_FLAG DX10ResourceBuilder::getDXAccess(const EResource::UsageType usage) noexcept
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

D3D10_BIND_FLAG DX10ResourceBuilder::getDXBind(const EBuffer::Type type) noexcept
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
