#include "dx/dx12/DX12ResourceBuffer.hpp"

#ifdef _WIN32
#include "dx/dx12/d3dx12.h"
#include "graphics/CommandList.hpp"
#include "dx/dx12/DX12GraphicsInterface.hpp"
#include "dx/dx12/D3D12MemAlloc.h"

void* DX12ResourceBuffer::map(const EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept
{
    if(_args.usageType == EResource::UsageType::Immutable)
    { return null; }

    if(_resourceUsage == EGraphics::ResourceHeapUsageType::Upload)
    {
        if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
        { return null; }

        if(mapType == EResource::MapType::NoWrite)
        { return null; }

        void* mapping;

        const CD3DX12_RANGE range(0, 0);
        const HRESULT res = _d3dResource->Map(0, &range, &mapping);

        if(FAILED(res))
        { return null; }

        return mapping;
    }
    else if(_resourceUsage == EGraphics::ResourceHeapUsageType::Read)
    {
        if(mapReadRange && mapReadRange->begin >= mapReadRange->end)
        { return null; }

        void* mapping;
        if(mapType == EResource::MapType::Discard)
        { return null; }

        if(mapReadRange)
        {
            const CD3DX12_RANGE range(mapReadRange->begin, mapReadRange->end);
            const HRESULT res = _d3dResource->Map(0, &range, &mapping);

            if(FAILED(res))
            { return null; }
        }
        else
        {
            const HRESULT res = _d3dResource->Map(0, null, &mapping);

            if(FAILED(res))
            { return null; }
        }
        return mapping;
    }

    return null;
}

void DX12ResourceBuffer::unmap(const ResourceMapRange* mapWriteRange) noexcept
{
    if(_args.usageType == EResource::UsageType::Immutable)
    { return; }

    if(mapWriteRange)
    {
        CD3DX12_RANGE range(mapWriteRange->begin, mapWriteRange->end);
        _d3dResource->Unmap(0, &range);
    }
    else
    {
        _d3dResource->Unmap(0, null);
    }
}

void* DX12ResourceBufferIndirectMapping::map() noexcept
{
    if(!_uploadBuffer)
    {
        D3D12MA::ALLOCATION_DESC allocDesc;
        allocDesc.Flags = D3D12MA::ALLOCATION_FLAG_NONE;
        allocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

        const CD3DX12_RESOURCE_DESC uploadDesc = CD3DX12_RESOURCE_DESC::Buffer(_args.size);

        const HRESULT res = _gi.gpuAllocator()->CreateResource(&allocDesc, &uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, &_uploadAllocation, IID_PPV_ARGS(&_uploadBuffer));

        if(FAILED(res))
        { return null; }
    }

    if(!_uploadBuffer)
    { return null; }

    void* mapping;
    const CD3DX12_RANGE range(0, 0);
    const HRESULT res = _uploadBuffer->Map(0, &range, &mapping);
    if(FAILED(res))
    {
        _uploadBuffer->Release();
        _uploadBuffer = null;

        _uploadAllocation->Release();
        _uploadAllocation = null;
        return null;
    }

    return mapping;
}

void DX12ResourceBufferIndirectMapping::unmap(ICommandList& cmdList, const ResourceMapRange* mapWriteRange) noexcept
{
    if(!_uploadBuffer)
    { return; }

    ID3D12GraphicsCommandList* d3dCmdList = null;

    if(mapWriteRange)
    {
        CD3DX12_RANGE range(mapWriteRange->begin, mapWriteRange->end);
        _uploadBuffer->Unmap(0, &range);
        d3dCmdList->CopyBufferRegion(_d3dResource, mapWriteRange->begin, _uploadBuffer, mapWriteRange->begin, mapWriteRange->length());
    }
    else
    {
        _uploadBuffer->Unmap(0, null);
        d3dCmdList->CopyBufferRegion(_d3dResource, 0, _uploadBuffer, 0, _args.size);
    }

    if(!_keepUploadAllocation)
    {
        _uploadBuffer->Release();
        _uploadBuffer = null;

        _uploadAllocation->Release();
        _uploadAllocation = null;
    }
}
#endif
