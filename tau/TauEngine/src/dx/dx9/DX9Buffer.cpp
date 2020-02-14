#include "dx/dx9/DX9Buffer.hpp"
#include <Safeties.hpp>

DX9Buffer::~DX9Buffer() noexcept
{
    if(_buffer)
    {
        _buffer->Release();
    }
}

void DX9Buffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    void* storeHandle;
    _buffer->Lock(0, 0, &storeHandle, 0);
    ::std::memcpy(storeHandle, data, _bufferSize);
    _buffer->Unlock();
}
