#include "dx/dx12/DX12ResourceBuffer.hpp"

#ifdef _WIN32
#include "dx/dx12/d3dx12.h"
#include "graphics/CommandList.hpp"

void* DX12ResourceBuffer::map(ICommandList& cmdList, const EResource::MapType mapType, uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
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
    else if(_resourceUsage == EGraphics::ResourceHeapUsageType::Default)
    {
        if(mapWriteRange && mapWriteRange->begin >= mapWriteRange->end)
        { return null; }

        if(!mapWriteRange)
        {
            _uploadBuffer = getResourceUploader()->allocAuto(_args.size);
        }
        else
        {
            _uploadBuffer = getResourceUploader()->allocAuto(mapWriteRange->length());
        }

        if(!_uploadBuffer)
        { return null; }

        void* mapping;
        const CD3DX12_RANGE range(0, 0);
        const HRESULT res = _uploadBuffer->allocation().resource->Map(0, &range, &mapping);
        if(FAILED(res))
        {
            _uploadBuffer = null;
            return null;
        }

        return mapping;
    }

    return null;
}

void DX12ResourceBuffer::unmap(ICommandList& cmdList, uSys, uSys, const ResourceMapRange* mapWriteRange) noexcept
{
    if(_uploadBuffer)
    {
        ID3D12GraphicsCommandList* d3dCmdList = null;

        if(mapWriteRange)
        {
            CD3DX12_RANGE range(mapWriteRange->begin, mapWriteRange->end);
            _uploadBuffer->allocation().resource->Unmap(0, &range);
            d3dCmdList->CopyBufferRegion(_d3dResource, mapWriteRange->begin, _uploadBuffer->allocation().resource, _uploadBuffer->allocation().offset, _uploadBuffer->allocation().size);
        }
        else
        {
            _uploadBuffer->allocation().resource->Unmap(0, null);
            d3dCmdList->CopyBufferRegion(_d3dResource, 0, _uploadBuffer->allocation().resource, _uploadBuffer->allocation().offset, _uploadBuffer->allocation().size);
        }
    }
    else
    {
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
}
#endif
