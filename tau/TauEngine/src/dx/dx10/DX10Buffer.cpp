#include "dx/dx10/DX10Buffer.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauEngine.hpp"

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX10RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#endif

bool DX10VertexBuffer::beginModification(IRenderingContext&) noexcept
{
    if(canReWrite(_usage))
    {
#if TAU_BUFFER_SAFETY
        ++_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount > 1)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
            return false;
        }
  #endif
#endif

        void* bufferAccess;
        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;
            return true;
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
    return false;
}

void DX10VertexBuffer::endModification(IRenderingContext&) noexcept
{
    if(_currentMapping)
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;

#if TAU_BUFFER_SAFETY
        --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount < 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyEnd);
        }
  #endif
#endif
    }
}

void DX10VertexBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    if(_modificationLockCount <= 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedWithoutBegin);
        return;
    }
  #endif
#endif

	if(_currentMapping)
	{
        ::std::memcpy(reinterpret_cast<u8*>(_currentMapping) + offset, data, size);
	}
}

void DX10VertexBuffer::fillBuffer(IRenderingContext&, const void* const data) noexcept
{
    if(DX10VertexBuffer::canReWrite(_usage))
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
        if(_modificationLockCount > 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::FilledWhileModifying);
            return;
        }
  #endif
#endif

        void* bufferAccess;
        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

bool DX10IndexBuffer::beginModification(IRenderingContext&) noexcept
{
    if(DX10VertexBuffer::canReWrite(_usage))
    {
#if TAU_BUFFER_SAFETY
        ++_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount > 1)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
            return false;
        }
  #endif
#endif

        void* bufferAccess;
        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;
            return true;
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
    return false;
}

void DX10IndexBuffer::endModification(IRenderingContext&) noexcept
{
    if(_currentMapping)
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;

#if TAU_BUFFER_SAFETY
        --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount < 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyEnd);
        }
  #endif
#endif
    }
}

void DX10IndexBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    if(_modificationLockCount <= 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedWithoutBegin);
        return;
    }
  #endif
#endif
    if(_currentMapping)
    {
        ::std::memcpy(reinterpret_cast<u8*>(_currentMapping) + offset, data, size);
    }
}

void DX10IndexBuffer::fillBuffer(IRenderingContext&, const void* const data) noexcept
{
    if(DX10VertexBuffer::canReWrite(_usage))
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
        if(_modificationLockCount > 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::FilledWhileModifying);
            return;
        }
  #endif
#endif

        void* bufferAccess;
        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

void DX10UniformBuffer::bind(IRenderingContext& context, const EShader::Stage stage, const u32 index) noexcept
{
#if TAU_BUFFER_SAFETY
    ++_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND
    if(_uniformBindLockCount > 1)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferBind);
    }
  #endif
#endif

    CTX();
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

void DX10UniformBuffer::unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferUnbind);
    }
  #endif
#endif

    CTX();
    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetConstantBuffers(index, 0, null);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetConstantBuffers(index, 0, null);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetConstantBuffers(index, 0, null);
            break;
        default: break;
    }
}

void DX10UniformBuffer::fastUnbind() noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferUnbind);
    }
  #endif
#endif
}

bool DX10UniformBuffer::beginModification(IRenderingContext&) noexcept
{
    if(DX10VertexBuffer::canReWrite(_usage))
    {
#if TAU_BUFFER_SAFETY
        ++_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount > 1)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
            return false;
        }
  #endif
#endif

        void* bufferAccess;
        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;
            return true;
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
    return false;
}

void DX10UniformBuffer::endModification(IRenderingContext&) noexcept
{
    if(_currentMapping)
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;

#if TAU_BUFFER_SAFETY
        --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount < 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyEnd);
        }
  #endif
#endif
    }
}

void DX10UniformBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    if(_modificationLockCount <= 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedWithoutBegin);
        return;
    }
  #endif
#endif
    if(_currentMapping)
    {
        ::std::memcpy(reinterpret_cast<u8*>(_currentMapping) + offset, data, size);
    }
}

void DX10UniformBuffer::fillBuffer(IRenderingContext&, const void* const data) noexcept
{
    if(DX10VertexBuffer::canReWrite(_usage))
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
        if(_modificationLockCount > 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::FilledWhileModifying);
            return;
        }
  #endif
#endif

        void* bufferAccess;
        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess, data, _bufferSize);
            _d3dBuffer->Unmap();
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

DX10VertexBuffer* DX10BufferBuilder::build(const VertexBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10VertexBuffer* const buffer = new(::std::nothrow) DX10VertexBuffer(args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10VertexBuffer* DX10BufferBuilder::build(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10VertexBuffer* const buffer = allocator.allocateT<DX10VertexBuffer>(args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IVertexBuffer> DX10BufferBuilder::buildCPPRef(const VertexBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX10VertexBuffer> buffer = CPPRef<DX10VertexBuffer>(new(::std::nothrow) DX10VertexBuffer(args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IVertexBuffer> DX10BufferBuilder::buildTauRef(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX10VertexBuffer> buffer(allocator, args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableStrongRef<IVertexBuffer> DX10BufferBuilder::buildTauSRef(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX10VertexBuffer> buffer(allocator, args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

DX10IndexBuffer* DX10BufferBuilder::build(const IndexBufferArgs& args, Error* const error) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10IndexBuffer* const buffer = new(::std::nothrow) DX10IndexBuffer(args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10IndexBuffer* DX10BufferBuilder::build(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10IndexBuffer* const buffer = allocator.allocateT<DX10IndexBuffer>(args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IIndexBuffer> DX10BufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* const error) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10IndexBuffer> buffer = CPPRef<DX10IndexBuffer>(new(::std::nothrow) DX10IndexBuffer(args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer));
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IIndexBuffer> DX10BufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10IndexBuffer> buffer(allocator, args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableStrongRef<IIndexBuffer> DX10BufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10IndexBuffer> buffer(allocator, args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

DX10UniformBuffer* DX10BufferBuilder::build(const UniformBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10UniformBuffer* const buffer = new(::std::nothrow) DX10UniformBuffer(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10UniformBuffer* DX10BufferBuilder::build(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10UniformBuffer* const buffer = allocator.allocateT<DX10UniformBuffer>(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IUniformBuffer> DX10BufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX10UniformBuffer> buffer = CPPRef<DX10UniformBuffer>(new(::std::nothrow) DX10UniformBuffer(args.usage, args.bufferSize, d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IUniformBuffer> DX10BufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX10UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableStrongRef<IUniformBuffer> DX10BufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX10UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

bool DX10BufferBuilder::processArgs(const VertexBufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize();
    bufferDesc.Usage = DX10VertexBuffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = DX10VertexBuffer::getDXAccess(args.usage);
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX10BufferBuilder::processArgs(const IndexBufferArgs& args, DXIndexBufferArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    dxArgs->indexSize = DX10IndexBuffer::dxIndexSize(args.indexSize);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize();
    bufferDesc.Usage = DX10VertexBuffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = DX10VertexBuffer::getDXAccess(args.usage);
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

bool DX10BufferBuilder::processArgs(const UniformBufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.bufferSize == 0, Error::BufferSizeIsZero);

    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize;
    bufferDesc.Usage = DX10VertexBuffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D10_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _gi.d3d10Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

D3D10_USAGE DX10VertexBuffer::getDXUsage(const EBuffer::UsageType usage) noexcept
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

D3D10_CPU_ACCESS_FLAG DX10VertexBuffer::getDXAccess(const EBuffer::UsageType usage) noexcept
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

bool DX10VertexBuffer::canReWrite(const EBuffer::UsageType usage) noexcept
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

DXGI_FORMAT DX10IndexBuffer::dxIndexSize(const EBuffer::IndexSize indexSize) noexcept
{
    switch(indexSize)
    {
        case EBuffer::IndexSize::Uint32: return DXGI_FORMAT_R32_UINT;
        case EBuffer::IndexSize::Uint16: return DXGI_FORMAT_R16_UINT;
        default: return static_cast<DXGI_FORMAT>(0);
    }
}
#endif
