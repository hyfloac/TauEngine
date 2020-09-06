#include "dx/dx11/DX11Buffer.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX11RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#endif

bool DX11VertexBuffer::beginModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11VertexBuffer::canReWrite(_usage))
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

        D3D11_MAPPED_SUBRESOURCE bufferAccess;
        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess.pData;
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

void DX11VertexBuffer::endModification(IRenderingContext& context) noexcept
{
    CTX();

    if(_currentMapping)
    {
        ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
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

void DX11VertexBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
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

void DX11VertexBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    CTX();

    if(canReWrite(_usage))
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

        D3D11_MAPPED_SUBRESOURCE bufferAccess;
        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess.pData, data, _bufferSize);
            ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

bool DX11IndexBuffer::beginModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11VertexBuffer::canReWrite(_usage))
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

        D3D11_MAPPED_SUBRESOURCE bufferAccess;
        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess.pData;
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

void DX11IndexBuffer::endModification(IRenderingContext& context) noexcept
{
    CTX();

    if(_currentMapping)
    {
        ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
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

void DX11IndexBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
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

void DX11IndexBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    CTX();

    if(DX11VertexBuffer::canReWrite(_usage))
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

        D3D11_MAPPED_SUBRESOURCE bufferAccess;
        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess.pData, data, _bufferSize);
            ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

void DX11UniformBuffer::bind(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
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
            ctx.d3d11DeviceContext()->VSSetConstantBuffers(index, 1, &_d3dBuffer);
            break;
        case EShader::Stage::Hull:
            ctx.d3d11DeviceContext()->HSSetConstantBuffers(index, 1, &_d3dBuffer);
            break;
        case EShader::Stage::Domain:
            ctx.d3d11DeviceContext()->DSSetConstantBuffers(index, 1, &_d3dBuffer);
            break;
        case EShader::Stage::Geometry:
            ctx.d3d11DeviceContext()->GSSetConstantBuffers(index, 1, &_d3dBuffer);
            break;
        case EShader::Stage::Pixel:
            ctx.d3d11DeviceContext()->PSSetConstantBuffers(index, 1, &_d3dBuffer);
            break;
        default: break;
    }
}

void DX11UniformBuffer::unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferBind);
    }
  #endif
#endif

    CTX();

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3d11DeviceContext()->VSSetConstantBuffers(index, 0, null);
            break;
        case EShader::Stage::Hull:
            ctx.d3d11DeviceContext()->HSSetConstantBuffers(index, 0, null);
            break;
        case EShader::Stage::Domain:
            ctx.d3d11DeviceContext()->DSSetConstantBuffers(index, 0, null);
            break;
        case EShader::Stage::Geometry:
            ctx.d3d11DeviceContext()->GSSetConstantBuffers(index, 0, null);
            break;
        case EShader::Stage::Pixel:
            ctx.d3d11DeviceContext()->PSSetConstantBuffers(index, 0, null);
            break;
        default: break;
    }
}

void DX11UniformBuffer::fastUnbind() const noexcept
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

bool DX11UniformBuffer::beginModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11VertexBuffer::canReWrite(_usage))
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

        D3D11_MAPPED_SUBRESOURCE bufferAccess;
        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess.pData;
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

void DX11UniformBuffer::endModification(IRenderingContext& context) noexcept
{
    CTX();

    if(_currentMapping)
    {
        ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
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

void DX11UniformBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
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

void DX11UniformBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    CTX();

    if(DX11VertexBuffer::canReWrite(_usage))
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

        D3D11_MAPPED_SUBRESOURCE bufferAccess;
        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            ::std::memcpy(bufferAccess.pData, data, _bufferSize);
            ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
        }
    }
    else
    {
#if TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedStaticBuffer);
#endif
    }
}

DX11VertexBuffer* DX11BufferBuilder::build(const VertexBufferArgs& args, Error* const error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX11VertexBuffer* const buffer = new(::std::nothrow) DX11VertexBuffer(args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX11VertexBuffer* DX11BufferBuilder::build(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX11VertexBuffer* const buffer = allocator.allocateT<DX11VertexBuffer>(args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IVertexBuffer> DX11BufferBuilder::buildCPPRef(const VertexBufferArgs& args, Error* const error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX11VertexBuffer> buffer(new(::std::nothrow) DX11VertexBuffer(args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IVertexBuffer> DX11BufferBuilder::buildTauRef(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX11VertexBuffer> buffer(allocator, args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPReinterpretCast<IVertexBuffer>(buffer));
}

NullableStrongRef<IVertexBuffer> DX11BufferBuilder::buildTauSRef(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX11VertexBuffer> buffer(allocator, args.usage, args.bufferSize(), args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPReinterpretCast<IVertexBuffer>(buffer));
}

DX11IndexBuffer* DX11BufferBuilder::build(const IndexBufferArgs& args, Error* const error) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11IndexBuffer* const buffer = new(::std::nothrow) DX11IndexBuffer(args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX11IndexBuffer* DX11BufferBuilder::build(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11IndexBuffer* const buffer = allocator.allocateT<DX11IndexBuffer>(args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IIndexBuffer> DX11BufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* const error) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX11IndexBuffer> buffer(new(::std::nothrow) DX11IndexBuffer(args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer));
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IIndexBuffer> DX11BufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX11IndexBuffer> buffer(allocator, args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPReinterpretCast<IIndexBuffer>(buffer));
}

NullableStrongRef<IIndexBuffer> DX11BufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXIndexBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX11IndexBuffer> buffer(allocator, args.usage, args.indexSize, args.bufferSize(), dxArgs.indexSize, dxArgs.d3dBuffer);
    if(!buffer)
    {
        dxArgs.d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPReinterpretCast<IIndexBuffer>(buffer));
}

DX11UniformBuffer* DX11BufferBuilder::build(const UniformBufferArgs& args, Error* const error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX11UniformBuffer* const buffer = new(::std::nothrow) DX11UniformBuffer(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX11UniformBuffer* DX11BufferBuilder::build(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    DX11UniformBuffer* const buffer = allocator.allocateT<DX11UniformBuffer>(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IUniformBuffer> DX11BufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* const error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX11UniformBuffer> buffer(new(::std::nothrow) DX11UniformBuffer(args.usage, args.bufferSize, d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IUniformBuffer> DX11BufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX11UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPReinterpretCast<IUniformBuffer>(buffer));
}

NullableStrongRef<IUniformBuffer> DX11BufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX11UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPReinterpretCast<IUniformBuffer>(buffer));
}

bool DX11BufferBuilder::processArgs(const VertexBufferArgs& args, ID3D11Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize();
    bufferDesc.Usage = DX11VertexBuffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = DX11VertexBuffer::getDXAccess(args.usage);
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D11_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _gi.d3d11Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _gi.d3d11Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX11BufferBuilder::processArgs(const IndexBufferArgs& args, DXIndexBufferArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    dxArgs->indexSize = DX11IndexBuffer::dxIndexSize(args.indexSize);

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize();
    bufferDesc.Usage = DX11VertexBuffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = DX11VertexBuffer::getDXAccess(args.usage);
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D11_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _gi.d3d11Device()->CreateBuffer(&bufferDesc, &initialBuffer, &dxArgs->d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _gi.d3d11Device()->CreateBuffer(&bufferDesc, NULL, &dxArgs->d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

bool DX11BufferBuilder::processArgs(const UniformBufferArgs& args, ID3D11Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.bufferSize == 0, Error::BufferSizeIsZero);

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize;
    bufferDesc.Usage = DX11VertexBuffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D11_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _gi.d3d11Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _gi.d3d11Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

D3D11_USAGE DX11VertexBuffer::getDXUsage(const EBuffer::UsageType usage) noexcept
{
    switch(usage)
    {
        case EBuffer::UsageType::StreamDraw: 
        case EBuffer::UsageType::StreamCopy: 
        case EBuffer::UsageType::DynamicDraw: 
        case EBuffer::UsageType::DynamicCopy: return D3D11_USAGE_DYNAMIC;

        case EBuffer::UsageType::StaticRead: 
        case EBuffer::UsageType::StreamRead: 
        case EBuffer::UsageType::DynamicRead: return D3D11_USAGE_STAGING;

        case EBuffer::UsageType::StaticCopy:
        case EBuffer::UsageType::StaticDraw:
        default: return D3D11_USAGE_DEFAULT;
    }
}

D3D11_CPU_ACCESS_FLAG DX11VertexBuffer::getDXAccess(const EBuffer::UsageType usage) noexcept
{
    switch (usage)
    {
        case EBuffer::UsageType::StreamDraw:
        case EBuffer::UsageType::StreamCopy:
        case EBuffer::UsageType::DynamicDraw:
        case EBuffer::UsageType::DynamicCopy: return D3D11_CPU_ACCESS_WRITE;

        case EBuffer::UsageType::StaticRead:
        case EBuffer::UsageType::StreamRead:
        case EBuffer::UsageType::DynamicRead: return D3D11_CPU_ACCESS_READ;

        case EBuffer::UsageType::StaticCopy:
        case EBuffer::UsageType::StaticDraw:
        default: return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
    }
}

bool DX11VertexBuffer::canReWrite(const EBuffer::UsageType usage) noexcept
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

DXGI_FORMAT DX11IndexBuffer::dxIndexSize(const EBuffer::IndexSize indexSize) noexcept
{
    switch(indexSize)
    {
        case EBuffer::IndexSize::Uint32: return DXGI_FORMAT_R32_UINT;
        case EBuffer::IndexSize::Uint16: return DXGI_FORMAT_R16_UINT;
        default: return static_cast<DXGI_FORMAT>(0);
    }
}
#endif
