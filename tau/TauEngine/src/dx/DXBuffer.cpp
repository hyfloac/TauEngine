#include "dx/DXBuffer.hpp"
#include <Safeties.hpp>

DXBuffer::~DXBuffer() noexcept
{
    if(_buffer)
    {
        _buffer->Release();
    }
}

void DXBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    void* storeHandle;
    _buffer->Lock(0, 0, &storeHandle, 0);
    ::std::memcpy(storeHandle, data, _bufferSize);
    _buffer->Unlock();
}

void DXBuffer::modifyBuffer(IRenderingContext& context, const intptr_t offset, const std::ptrdiff_t size, const void* const data) noexcept
{
    void* storeHandle;
    _buffer->Lock(offset, size, &storeHandle, 0);
    ::std::memcpy(storeHandle, data, size);
    _buffer->Unlock();
}

DXBufferBuilder::DXBufferBuilder(const uSys descriptorCount, DX9RenderingContext& context) noexcept
    : IBufferBuilder(descriptorCount),
      _context(context)
{ }

IBuffer* DXBufferBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_type == static_cast<EBuffer::Type>(0), Error::TypeIsUnset);
    ERROR_CODE_COND_N(_usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_N(_type == EBuffer::Type::IndexBuffer, Error::BufferCannotBeIndexBuffer);
    ERROR_CODE_COND_N(_bufferSize == 0, Error::BufferSizeIsZero);

    IDirect3DVertexBuffer9* d3dBuffer;
    const HRESULT res = _context.d3d9Device()->CreateVertexBuffer(_bufferSize, 0, 0, D3DPOOL_MANAGED, &d3dBuffer, null);
    if(res != D3D_OK)
    {
        /* The call was incorrect, something must be wrong with the implementation of TauEngine */
        ERROR_CODE_COND_N(res == D3DERR_INVALIDCALL, Error::LibImplementationError);
        /* Ran out of video memory */
        ERROR_CODE_COND_N(res == D3DERR_OUTOFVIDEOMEMORY, Error::GPUMemoryAllocationFailure);
        /*
         * The driver was unable to allocate memory within the system.
         *
         *   This is unique from D3DERR_OUTOFVIDEOMEMORY in that
         * it specifically refers to memory in the main system and not
         * on the GPU.
         */
        ERROR_CODE_COND_N(res == E_OUTOFMEMORY, Error::DriverMemoryAllocationFailure);
        ERROR_CODE_N(Error::UnknownError);
    }

    DXBuffer* const buffer = new(std::nothrow) DXBuffer(_type, _usage, _bufferSize, _descriptor, d3dBuffer);

    if(!buffer)
    {
        d3dBuffer->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}
