#include "Allocation.hpp"
#include <d3dx12.h>

HRESULT Allocation::map(const D3D12_RANGE* readRange, void** const buffer) noexcept
{
    HRESULT ret;
    if(readRange)
    {
        CD3DX12_RANGE realRange(readRange->Begin + _offset, readRange->End + _offset);
        ret = _resource->Map(0, &realRange, buffer);
    }
    else
    {
        ret = _resource->Map(0, NULL, buffer);
    }

    if(SUCCEEDED(ret))
    {
        unsigned char* offBuf = reinterpret_cast<unsigned char*>(*buffer);
        offBuf += _offset;
        *buffer = offBuf;
    }
    return ret;
}

void Allocation::unmap(const D3D12_RANGE* writtenRange) noexcept
{
    if(writtenRange)
    {
        CD3DX12_RANGE realRange(writtenRange->Begin + _offset, writtenRange->End + _offset);
        _resource->Unmap(0, &realRange);
    }
    else
    {
        _resource->Unmap(0, NULL);
    }
}

D3D12_GPU_VIRTUAL_ADDRESS Allocation::getGPUVirtualAddress() const noexcept
{
    return _resource->GetGPUVirtualAddress() + _offset;
}
