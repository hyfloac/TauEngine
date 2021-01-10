#include "dx/dx12/DX12ResourceBuffer.hpp"

#ifdef _WIN32
#include "dx/dx12/d3dx12.h"
#include "graphics/CommandList.hpp"
#include "dx/dx12/DX12GraphicsInterface.hpp"

void* DX12ResourceBuffer::map(const EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept
{
    if(_resourceUsage == EGraphics::ResourceHeapUsageType::Upload)
    {
        if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
        { return null; }

        if(mapType == EResource::MapType::ReadOnly)
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
#endif
