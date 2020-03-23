#include "dx/dx10/DX10Buffer.hpp"
#include "TauEngine.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx10/DX10RenderingContext.hpp"

DX10Buffer::DX10Buffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize,
                       const bool instanced, const BufferDescriptor& descriptor, ID3D10Buffer* const d3dBuffer) noexcept
    : IBuffer(type, usage, bufferSize, instanced, descriptor), _d3dBuffer(d3dBuffer), _currentMapping(null)
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

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
}

void DX10Buffer::beginModification(IRenderingContext& context) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;
        }
    }
}

void DX10Buffer::endModification(IRenderingContext& context) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;
    }
}

void DX10Buffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
{
	if(_currentMapping)
	{
        ::std::memcpy(reinterpret_cast<u8*>(_currentMapping) + offset, data, size);
	}
}

DX10IndexBuffer::DX10IndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, ID3D10Buffer* const d3dBuffer) noexcept
    : IIndexBuffer(usage, bufferSize), _d3dBuffer(d3dBuffer), _currentMapping(null)
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

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
}

void DX10IndexBuffer::beginModification(IRenderingContext& context) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;
        }
    }
}

void DX10IndexBuffer::endModification(IRenderingContext& context) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;
    }
}

void DX10IndexBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
{
    if(_currentMapping)
    {
        ::std::memcpy(reinterpret_cast<u8*>(_currentMapping) + offset, data, size);
    }
}

DX10UniformBuffer::DX10UniformBuffer(EBuffer::UsageType usage, uSys bufferSize, ID3D10Buffer* d3dBuffer) noexcept
    : IUniformBuffer(usage, bufferSize), _d3dBuffer(d3dBuffer), _currentMapping(null)
{ }

DX10UniformBuffer::~DX10UniformBuffer() noexcept
{
    _d3dBuffer->Release();
    _d3dBuffer = null;
}

void DX10UniformBuffer::bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept
{
    if(RTT_CHECK(context, DX10RenderingContext))
    {
        auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);
        switch(stage)
        {
            case EShader::Stage::Vertex:
                ctx.d3dDevice()->VSSetConstantBuffers(index, 1, &_d3dBuffer);
                break;
            case EShader::Stage::Geometry:
                ctx.d3dDevice()->GSSetConstantBuffers(index, 1, &_d3dBuffer);
                break;
            case EShader::Stage::Pixel:
                ctx.d3dDevice()->PSSetConstantBuffers(index, 1, &_d3dBuffer);
                break;
            default: break;
        }
    }
    else
    {
        TAU_THROW(IncorrectContextException);
    }
}

void DX10UniformBuffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
}

void DX10UniformBuffer::beginModification(IRenderingContext& context) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;
        }
    }
}

void DX10UniformBuffer::endModification(IRenderingContext& context) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;
    }
}

void DX10UniformBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
{
    if(_currentMapping)
    {
        ::std::memcpy(reinterpret_cast<u8*>(_currentMapping) + offset, data, size);
    }
}

DX10BufferBuilder::DX10BufferBuilder(DX10RenderingContext& context) noexcept
    : IBufferBuilder(), _context(context)
{ }

DX10Buffer* DX10BufferBuilder::build(const BufferArgs& args, Error* error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX10Buffer* buffer = new(::std::nothrow) DX10Buffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10Buffer* DX10BufferBuilder::build(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX10Buffer* buffer = allocator.allocateT<DX10Buffer>(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IBuffer> DX10BufferBuilder::buildCPPRef(const BufferArgs& args, Error* error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    CPPRef<DX10Buffer> buffer = CPPRef<DX10Buffer>(new(::std::nothrow) DX10Buffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableReferenceCountingPointer<IBuffer> DX10BufferBuilder::buildTauRef(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    NullableReferenceCountingPointer<DX10Buffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IBuffer>(buffer));
}

NullableStrongReferenceCountingPointer<IBuffer> DX10BufferBuilder::buildTauSRef(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    NullableStrongReferenceCountingPointer<DX10Buffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IBuffer>(buffer));
}

bool DX10BufferBuilder::processBufferArgs(const BufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
{
     ERROR_CODE_COND_F(args.type == static_cast<EBuffer::Type>(0), Error::TypeIsUnset);
     ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
     ERROR_CODE_COND_F(args.type == EBuffer::Type::IndexBuffer, Error::BufferCannotBeIndexBuffer);
     ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

     D3D10_BUFFER_DESC bufferDesc;
     bufferDesc.ByteWidth = args.bufferSize();
     bufferDesc.Usage = DX10Buffer::getDXUsage(args.usage);
     bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
     bufferDesc.CPUAccessFlags = DX10Buffer::getDXAccess(args.usage);
     bufferDesc.MiscFlags = 0;

     if(args.initialBuffer)
     {
         D3D10_SUBRESOURCE_DATA initialBuffer;
         initialBuffer.pSysMem = args.initialBuffer;
         initialBuffer.SysMemPitch = 0;
         initialBuffer.SysMemSlicePitch = 0;

         const HRESULT h = _context.d3dDevice()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
         ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
     }
     else
     {
         const HRESULT h = _context.d3dDevice()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
         ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
     }

     return true;
}

DX10IndexBufferBuilder::DX10IndexBufferBuilder(DX10RenderingContext& context) noexcept
    : IIndexBufferBuilder(), _context(context)
{ }

DX10IndexBuffer* DX10IndexBufferBuilder::build(const IndexBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX10IndexBuffer* buffer = new(::std::nothrow) DX10IndexBuffer(args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10IndexBuffer* DX10IndexBufferBuilder::build(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX10IndexBuffer* buffer = allocator.allocateT<DX10IndexBuffer>(args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IIndexBuffer> DX10IndexBufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    CPPRef<DX10IndexBuffer> buffer = CPPRef<DX10IndexBuffer>(new(::std::nothrow) DX10IndexBuffer(args.usage, args.bufferSize(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableReferenceCountingPointer<IIndexBuffer> DX10IndexBufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableReferenceCountingPointer<DX10IndexBuffer> buffer(allocator, args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IIndexBuffer>(buffer));
}

NullableStrongReferenceCountingPointer<IIndexBuffer> DX10IndexBufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<DX10IndexBuffer> buffer(allocator, args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IIndexBuffer>(buffer));
}

bool DX10IndexBufferBuilder::processBufferArgs(const IndexBufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize();
    bufferDesc.Usage = DX10Buffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = DX10Buffer::getDXAccess(args.usage);
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _context.d3dDevice()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _context.d3dDevice()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DX10UniformBufferBuilder::DX10UniformBufferBuilder(DX10RenderingContext& context) noexcept
    : IUniformBufferBuilder(), _context(context)
{ }

DX10UniformBuffer* DX10UniformBufferBuilder::build(const UniformBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX10UniformBuffer* buffer = new(::std::nothrow) DX10UniformBuffer(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10UniformBuffer* DX10UniformBufferBuilder::build(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX10UniformBuffer* buffer = allocator.allocateT<DX10UniformBuffer>(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IUniformBuffer> DX10UniformBufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    CPPRef<DX10UniformBuffer> buffer = CPPRef<DX10UniformBuffer>(new(::std::nothrow) DX10UniformBuffer(args.usage, args.bufferSize, d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableReferenceCountingPointer<IUniformBuffer> DX10UniformBufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableReferenceCountingPointer<DX10UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IUniformBuffer>(buffer));
}

NullableStrongReferenceCountingPointer<IUniformBuffer> DX10UniformBufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<DX10UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IUniformBuffer>(buffer));
}

bool DX10UniformBufferBuilder::processBufferArgs(const UniformBufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.bufferSize == 0, Error::BufferSizeIsZero);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize;
    bufferDesc.Usage = DX10Buffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _context.d3dDevice()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _context.d3dDevice()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
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

D3D10_CPU_ACCESS_FLAG DX10Buffer::getDXAccess(EBuffer::UsageType usage) noexcept
{
    switch (usage)
    {
        case EBuffer::UsageType::StreamDraw:
        case EBuffer::UsageType::StreamCopy:
        case EBuffer::UsageType::DynamicDraw:
        case EBuffer::UsageType::DynamicCopy: return D3D10_CPU_ACCESS_WRITE;

        case EBuffer::UsageType::StaticRead:
        case EBuffer::UsageType::StreamRead:
        case EBuffer::UsageType::DynamicRead: return D3D10_CPU_ACCESS_READ;

        case EBuffer::UsageType::StaticCopy:
        case EBuffer::UsageType::StaticDraw:
        default: return static_cast<D3D10_CPU_ACCESS_FLAG>(0);
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