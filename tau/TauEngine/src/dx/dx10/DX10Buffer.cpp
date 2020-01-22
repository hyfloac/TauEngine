#include "dx/dx10/DX10Buffer.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx10/DX10RenderingContext.hpp"

DX10Buffer::DX10Buffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize,
                       const BufferDescriptor& descriptor, ID3D10Buffer* const d3dBuffer) noexcept
    : IBuffer(type, usage, bufferSize, descriptor), _d3dBuffer(d3dBuffer)
{ }

DX10Buffer::~DX10Buffer() noexcept
{
    _d3dBuffer->Release();
    _d3dBuffer = null;
}

void DX10Buffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
}

void DX10Buffer::modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(reinterpret_cast<void*>(reinterpret_cast<u8*>(bufferAccess) + offset), data, size);
            _d3dBuffer->Unmap();
        }
    }
}

DX10IndexBuffer::DX10IndexBuffer(EBuffer::UsageType usage, uSys bufferSize, ID3D10Buffer* d3dBuffer) noexcept
    : IIndexBuffer(usage, bufferSize), _d3dBuffer(d3dBuffer)
{ }

DX10IndexBuffer::~DX10IndexBuffer() noexcept
{
    _d3dBuffer->Release();
    _d3dBuffer = null;
}

void DX10IndexBuffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
}

void DX10IndexBuffer::modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
}

DX10BufferBuilder::DX10BufferBuilder(uSys descriptorCount, DX10RenderingContext& context) noexcept
    : IBufferBuilder(descriptorCount), _context(context)
{ }

DX10Buffer* DX10BufferBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_type == static_cast<EBuffer::Type>(0), Error::TypeIsUnset);
    ERROR_CODE_COND_N(_usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_N(_type == EBuffer::Type::IndexBuffer, Error::BufferCannotBeIndexBuffer);
    ERROR_CODE_COND_N(_bufferSize == 0, Error::BufferSizeIsZero);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = _descriptor.stride() * _bufferSize;
    bufferDesc.Usage = DX10Buffer::getDXUsage(_usage);
    bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    ID3D10Buffer* d3dBuffer;

    if(_initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = _initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _context.d3d10Device()->CreateBuffer(&bufferDesc, &initialBuffer, &d3dBuffer);
        ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _context.d3d10Device()->CreateBuffer(&bufferDesc, NULL, &d3dBuffer);
        ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    DX10Buffer* buffer = new(::std::nothrow) DX10Buffer(_type, _usage, _bufferSize, _descriptor, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

DX10IndexBufferBuilder::DX10IndexBufferBuilder(DX10RenderingContext& context) noexcept
    : IIndexBufferBuilder(), _context(context)
{ }

DX10IndexBuffer* DX10IndexBufferBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_N(_bufferSize == 0, Error::BufferSizeIsZero);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(u32) * _bufferSize;
    bufferDesc.Usage = DX10Buffer::getDXUsage(_usage);
    bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    ID3D10Buffer* d3dBuffer;
    if(_initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = _initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _context.d3d10Device()->CreateBuffer(&bufferDesc, &initialBuffer, &d3dBuffer);
        ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _context.d3d10Device()->CreateBuffer(&bufferDesc, NULL, &d3dBuffer);
        ERROR_CODE_COND_N(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    DX10IndexBuffer* buffer = new(::std::nothrow) DX10IndexBuffer(_usage, _bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

D3D10_USAGE DX10Buffer::getDXUsage(EBuffer::UsageType usage) noexcept
{
    switch(usage)
    {
        case EBuffer::UsageType::StreamDraw: 
        case EBuffer::UsageType::StreamCopy: 
        case EBuffer::UsageType::DynamicDraw: 
        case EBuffer::UsageType::DynamicCopy: return D3D10_USAGE_DYNAMIC;

        case EBuffer::UsageType::StaticRead: 
        case EBuffer::UsageType::StreamRead: 
        case EBuffer::UsageType::DynamicRead: return D3D10_USAGE_STAGING;

        case EBuffer::UsageType::StaticCopy:
        case EBuffer::UsageType::StaticDraw:
        default: return D3D10_USAGE_DEFAULT;
    }
}

bool DX10Buffer::canReWrite(EBuffer::UsageType usage) noexcept
{
    switch(usage)
    {
        case EBuffer::UsageType::StreamDraw: 
        case EBuffer::UsageType::StreamCopy: 
        case EBuffer::UsageType::DynamicDraw: 
        case EBuffer::UsageType::DynamicCopy: return true;

        case EBuffer::UsageType::StaticDraw:
        case EBuffer::UsageType::StaticCopy:
        case EBuffer::UsageType::StaticRead:
        case EBuffer::UsageType::DynamicRead:
        case EBuffer::UsageType::StreamRead:
        default: return false;
    }
}
#endif
