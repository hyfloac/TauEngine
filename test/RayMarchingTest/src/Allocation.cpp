#include "Allocation.hpp"
#include <d3dx12.h>

HRESULT Allocation::map(const D3D12_RANGE* readRange, void** const buffer) noexcept
{
    return _resource->Map(0, readRange, buffer);
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
