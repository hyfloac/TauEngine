#include "dx/dx10/DX10Buffer.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauEngine.hpp"

void DX10Buffer::bind(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    ++_bindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_BIND
    if(_bindLockCount > 1)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleBufferBind);
    }
  #endif
#endif
}

void DX10Buffer::unbind(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    --_bindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_UNBIND
    if(_bindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleBufferUnbind);
    }
  #endif
#endif
}

bool DX10Buffer::beginModification(IRenderingContext&) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;

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

void DX10Buffer::endModification(IRenderingContext&) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;

#if TAU_BUFFER_SAFETY
        --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount < 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
        }
  #endif
#endif
    }
}

void DX10Buffer::modifyBuffer(const ::std::intptr_t offset, const ::std::ptrdiff_t size, const void* const data) noexcept
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

void DX10Buffer::fillBuffer(IRenderingContext&, const void* const data) noexcept
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
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

void DX10IndexBuffer::bind(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    ++_bindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_BIND
    if(_bindLockCount > 1)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleBufferBind);
    }
  #endif
#endif
}

void DX10IndexBuffer::unbind(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    --_bindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_UNBIND
    if(_bindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleBufferUnbind);
    }
  #endif
#endif
}

bool DX10IndexBuffer::beginModification(IRenderingContext&) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;

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
    if(DX10Buffer::canReWrite(_usage))
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;

#if TAU_BUFFER_SAFETY
        --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount < 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
        }
  #endif
#endif
    }
}

void DX10IndexBuffer::modifyBuffer(const ::std::intptr_t offset, const ::std::ptrdiff_t size, const void* const data) noexcept
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
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

void DX10UniformBuffer::bind(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    ++_bindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_BIND
    if(_bindLockCount > 1)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleBufferBind);
    }
  #endif
#endif
}

void DX10UniformBuffer::unbind(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    --_bindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_UNBIND
    if(_bindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleBufferUnbind);
    }
  #endif
#endif
}

void DX10UniformBuffer::bind(IRenderingContext& context, const EShader::Stage stage, const u32 index) noexcept
{
#if TAU_BUFFER_SAFETY
    ++_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_BIND
    if(_uniformBindLockCount > 1)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferBind);
    }
  #endif
#endif
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

void DX10UniformBuffer::unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_UNBIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferUnbind);
    }
  #endif
#endif
    if(RTT_CHECK(context, DX10RenderingContext))
    {
        auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);
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
    else
    {
        TAU_THROW(IncorrectContextException);
    }
}

bool DX10UniformBuffer::beginModification(IRenderingContext&) noexcept
{
    if(DX10Buffer::canReWrite(_usage))
    {
        void* bufferAccess;

        const HRESULT h = _d3dBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess;

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
    if(DX10Buffer::canReWrite(_usage))
    {
        _d3dBuffer->Unmap();
        _currentMapping = null;

#if TAU_BUFFER_SAFETY
        --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount < 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
        }
  #endif
#endif
    }
}

void DX10UniformBuffer::modifyBuffer(const ::std::intptr_t offset, const ::std::ptrdiff_t size, const void* const data) noexcept
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
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

DX10Buffer* DX10BufferBuilder::build(const BufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10Buffer* const buffer = new(::std::nothrow) DX10Buffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX10Buffer* DX10BufferBuilder::build(const BufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10Buffer* const buffer = allocator.allocateT<DX10Buffer>(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IBuffer> DX10BufferBuilder::buildCPPRef(const BufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX10Buffer> buffer = CPPRef<DX10Buffer>(new(::std::nothrow) DX10Buffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IBuffer> DX10BufferBuilder::buildTauRef(const BufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX10Buffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableStrongRef<IBuffer> DX10BufferBuilder::buildTauSRef(const BufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX10Buffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

bool DX10BufferBuilder::processArgs(const BufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
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

DX10IndexBuffer* DX10IndexBufferBuilder::build(const IndexBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10IndexBuffer* const buffer = new(::std::nothrow) DX10IndexBuffer(args.usage, args.bufferSize(), d3dBuffer);
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
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX10IndexBuffer* const buffer = allocator.allocateT<DX10IndexBuffer>(args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IIndexBuffer> DX10IndexBufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* const error) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX10IndexBuffer> buffer = CPPRef<DX10IndexBuffer>(new(::std::nothrow) DX10IndexBuffer(args.usage, args.bufferSize(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IIndexBuffer> DX10IndexBufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX10IndexBuffer> buffer(allocator, args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableStrongRef<IIndexBuffer> DX10IndexBufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX10IndexBuffer> buffer(allocator, args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

bool DX10IndexBufferBuilder::processArgs(const IndexBufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
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

DX10UniformBuffer* DX10UniformBufferBuilder::build(const UniformBufferArgs& args, Error* const error) const noexcept
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

DX10UniformBuffer* DX10UniformBufferBuilder::build(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
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

CPPRef<IUniformBuffer> DX10UniformBufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* const error) const noexcept
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

NullableRef<IUniformBuffer> DX10UniformBufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
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

NullableStrongRef<IUniformBuffer> DX10UniformBufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
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

bool DX10UniformBufferBuilder::processArgs(const UniformBufferArgs& args, ID3D10Buffer** const d3dBuffer, Error* const error) const noexcept
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

D3D10_USAGE DX10Buffer::getDXUsage(const EBuffer::UsageType usage) noexcept
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

D3D10_CPU_ACCESS_FLAG DX10Buffer::getDXAccess(const EBuffer::UsageType usage) noexcept
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

bool DX10Buffer::canReWrite(const EBuffer::UsageType usage) noexcept
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
