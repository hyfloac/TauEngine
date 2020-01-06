#pragma once

#include "model/IBuffer.hpp"
#include "dx/DX9RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

class TAU_DLL DXBuffer final : public IBuffer
{
    DELETE_COPY(DXBuffer);
private:
    IDirect3DVertexBuffer9* _buffer;
public:
    DXBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, DX9RenderingContext& context)
        : IBuffer(type, usage, bufferSize, descriptor)
    {
        context.d3d9Device()->CreateVertexBuffer(bufferSize, 0, 0, D3DPOOL_MANAGED, &_buffer, null);
    }

    ~DXBuffer() noexcept override final
    {
        if(_buffer)
        {
            _buffer->Release();
        }
    }


    void bind(IRenderingContext& context) noexcept override
    {    
    }

    void unbind(IRenderingContext& context) noexcept override
    {
        
    }

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override
    {
        void* storeHandle;
        _buffer->Lock(0, 0, &storeHandle, 0);
        ::std::memcpy(storeHandle, data, _bufferSize);
        _buffer->Unlock();
    }

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override
    {
        void* storeHandle;
        _buffer->Lock(offset, size, &storeHandle, 0);
        ::std::memcpy(storeHandle, data, size);
        _buffer->Unlock();
    }
};
#endif
