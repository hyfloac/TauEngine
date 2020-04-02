#include "dx/dx11/DX11Buffer.hpp"
#include "TauEngine.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx11/DX11RenderingContext.hpp"

DX11Buffer::DX11Buffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize,
                       const bool instanced, const BufferDescriptor& descriptor, ID3D11Buffer* const d3dBuffer) noexcept
    : IBuffer(type, usage, bufferSize, instanced, descriptor), _d3dBuffer(d3dBuffer), _currentMapping(null)
{ }

DX11Buffer::~DX11Buffer() noexcept
{
    _d3dBuffer->Release();
    _d3dBuffer = null;
}

#define CTX() \
    if(!RTT_CHECK(context, DX11RenderingContext)) \
    { TAU_THROW(IncorrectContextException); } \
    auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)


void DX11Buffer::bind(IRenderingContext&) noexcept
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

void DX11Buffer::unbind(IRenderingContext&) noexcept
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

bool DX11Buffer::beginModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
        D3D11_MAPPED_SUBRESOURCE bufferAccess;

        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess.pData;

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

void DX11Buffer::endModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
        ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
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

void DX11Buffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
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

void DX11Buffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
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

DX11IndexBuffer::DX11IndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, ID3D11Buffer* const d3dBuffer) noexcept
    : IIndexBuffer(usage, bufferSize), _d3dBuffer(d3dBuffer), _currentMapping(null)
{ }

DX11IndexBuffer::~DX11IndexBuffer() noexcept
{
    _d3dBuffer->Release();
    _d3dBuffer = null;
}

void DX11IndexBuffer::bind(IRenderingContext&) noexcept
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

void DX11IndexBuffer::unbind(IRenderingContext&) noexcept
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

bool DX11IndexBuffer::beginModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
        D3D11_MAPPED_SUBRESOURCE bufferAccess;

        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess.pData;

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

void DX11IndexBuffer::endModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
        ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
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

void DX11IndexBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
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

void DX11IndexBuffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
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

DX11UniformBuffer::DX11UniformBuffer(EBuffer::UsageType usage, uSys bufferSize, ID3D11Buffer* d3dBuffer) noexcept
    : IUniformBuffer(usage, bufferSize), _d3dBuffer(d3dBuffer), _currentMapping(null)
{ }

DX11UniformBuffer::~DX11UniformBuffer() noexcept
{
    _d3dBuffer->Release();
    _d3dBuffer = null;
}

void DX11UniformBuffer::bind(IRenderingContext&) noexcept
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

void DX11UniformBuffer::unbind(IRenderingContext&) noexcept
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

void DX11UniformBuffer::bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept
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
    if(RTT_CHECK(context, DX11RenderingContext))
    {
        auto& ctx = reinterpret_cast<DX11RenderingContext&>(context);
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
    else
    {
        TAU_THROW(IncorrectContextException);
    }
}

void DX11UniformBuffer::unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_BIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferBind);
    }
  #endif
#endif
    if(RTT_CHECK(context, DX11RenderingContext))
    {
        auto& ctx = reinterpret_cast<DX11RenderingContext&>(context);
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
    else
    {
        TAU_THROW(IncorrectContextException);
    }
}

bool DX11UniformBuffer::beginModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
        D3D11_MAPPED_SUBRESOURCE bufferAccess;

        const HRESULT h = ctx.d3d11DeviceContext()->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferAccess);
        if(!FAILED(h))
        {
            _currentMapping = bufferAccess.pData;

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

void DX11UniformBuffer::endModification(IRenderingContext& context) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
        ctx.d3d11DeviceContext()->Unmap(_d3dBuffer, 0);
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

void DX11UniformBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
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

void DX11UniformBuffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    CTX();

    if(DX11Buffer::canReWrite(_usage))
    {
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

DX11BufferBuilder::DX11BufferBuilder(DX11RenderingContext& context) noexcept
    : IBufferBuilder(), _context(context)
{ }

DX11Buffer* DX11BufferBuilder::build(const BufferArgs& args, Error* error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX11Buffer* const buffer = new(::std::nothrow) DX11Buffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX11Buffer* DX11BufferBuilder::build(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX11Buffer* const buffer = allocator.allocateT<DX11Buffer>(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IBuffer> DX11BufferBuilder::buildCPPRef(const BufferArgs& args, Error* error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX11Buffer> buffer = CPPRef<DX11Buffer>(new(::std::nothrow) DX11Buffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IBuffer> DX11BufferBuilder::buildTauRef(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX11Buffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IBuffer>(buffer));
}

NullableStrongRef<IBuffer> DX11BufferBuilder::buildTauSRef(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX11Buffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IBuffer>(buffer));
}

bool DX11BufferBuilder::processBufferArgs(const BufferArgs& args, ID3D11Buffer** const d3dBuffer, Error* const error) const noexcept
{
     ERROR_CODE_COND_F(args.type == static_cast<EBuffer::Type>(0), Error::TypeIsUnset);
     ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
     ERROR_CODE_COND_F(args.type == EBuffer::Type::IndexBuffer, Error::BufferCannotBeIndexBuffer);
     ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

     D3D11_BUFFER_DESC bufferDesc;
     bufferDesc.ByteWidth = args.bufferSize();
     bufferDesc.Usage = DX11Buffer::getDXUsage(args.usage);
     bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
     bufferDesc.CPUAccessFlags = DX11Buffer::getDXAccess(args.usage);
     bufferDesc.MiscFlags = 0;

     if(args.initialBuffer)
     {
         D3D11_SUBRESOURCE_DATA initialBuffer;
         initialBuffer.pSysMem = args.initialBuffer;
         initialBuffer.SysMemPitch = 0;
         initialBuffer.SysMemSlicePitch = 0;

         const HRESULT h = _context.d3d11Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
         ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
     }
     else
     {
         const HRESULT h = _context.d3d11Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
         ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
     }

     return true;
}

DX11IndexBufferBuilder::DX11IndexBufferBuilder(DX11RenderingContext& context) noexcept
    : IIndexBufferBuilder(), _context(context)
{ }

DX11IndexBuffer* DX11IndexBufferBuilder::build(const IndexBufferArgs& args, Error* const error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX11IndexBuffer* const buffer = new(::std::nothrow) DX11IndexBuffer(args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX11IndexBuffer* DX11IndexBufferBuilder::build(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX11IndexBuffer* const buffer = allocator.allocateT<DX11IndexBuffer>(args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IIndexBuffer> DX11IndexBufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX11IndexBuffer> buffer = CPPRef<DX11IndexBuffer>(new(::std::nothrow) DX11IndexBuffer(args.usage, args.bufferSize(), d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IIndexBuffer> DX11IndexBufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX11IndexBuffer> buffer(allocator, args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IIndexBuffer>(buffer));
}

NullableStrongRef<IIndexBuffer> DX11IndexBufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX11IndexBuffer> buffer(allocator, args.usage, args.bufferSize(), d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IIndexBuffer>(buffer));
}

bool DX11IndexBufferBuilder::processBufferArgs(const IndexBufferArgs& args, ID3D11Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize();
    bufferDesc.Usage = DX11Buffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = DX11Buffer::getDXAccess(args.usage);
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D11_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _context.d3d11Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _context.d3d11Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

DX11UniformBufferBuilder::DX11UniformBufferBuilder(DX11RenderingContext& context) noexcept
    : IUniformBufferBuilder(), _context(context)
{ }

DX11UniformBuffer* DX11UniformBufferBuilder::build(const UniformBufferArgs& args, Error* const error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX11UniformBuffer* const buffer = new(::std::nothrow) DX11UniformBuffer(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

DX11UniformBuffer* DX11UniformBufferBuilder::build(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    DX11UniformBuffer* const buffer = allocator.allocateT<DX11UniformBuffer>(args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IUniformBuffer> DX11UniformBufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* error) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const CPPRef<DX11UniformBuffer> buffer = CPPRef<DX11UniformBuffer>(new(::std::nothrow) DX11UniformBuffer(args.usage, args.bufferSize, d3dBuffer));
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }
    
    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IUniformBuffer> DX11UniformBufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableRef<DX11UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IUniformBuffer>(buffer));
}

NullableStrongRef<IUniformBuffer> DX11UniformBufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D11Buffer* d3dBuffer;
    if(!processBufferArgs(args, &d3dBuffer, error))
    { return null; }

    const NullableStrongRef<DX11UniformBuffer> buffer(allocator, args.usage, args.bufferSize, d3dBuffer);
    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RCPCast<IUniformBuffer>(buffer));
}

bool DX11UniformBufferBuilder::processBufferArgs(const UniformBufferArgs& args, ID3D11Buffer** const d3dBuffer, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.bufferSize == 0, Error::BufferSizeIsZero);

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = args.bufferSize;
    bufferDesc.Usage = DX11Buffer::getDXUsage(args.usage);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;

    if(args.initialBuffer)
    {
        D3D11_SUBRESOURCE_DATA initialBuffer;
        initialBuffer.pSysMem = args.initialBuffer;
        initialBuffer.SysMemPitch = 0;
        initialBuffer.SysMemSlicePitch = 0;

        const HRESULT h = _context.d3d11Device()->CreateBuffer(&bufferDesc, &initialBuffer, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }
    else
    {
        const HRESULT h = _context.d3d11Device()->CreateBuffer(&bufferDesc, NULL, d3dBuffer);
        ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);
    }

    return true;
}

D3D11_USAGE DX11Buffer::getDXUsage(EBuffer::UsageType usage) noexcept
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

D3D11_CPU_ACCESS_FLAG DX11Buffer::getDXAccess(EBuffer::UsageType usage) noexcept
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

bool DX11Buffer::canReWrite(EBuffer::UsageType usage) noexcept
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
