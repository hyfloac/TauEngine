#include "dx/dx12/DX12ResourceBuffer.hpp"

#ifdef _WIN32
#include "dx/dx12/d3dx12.h"
#include "graphics/CommandList.hpp"
#include "dx/dx12/DX12GraphicsInterface.hpp"

void* DX12ResourceBuffer::map(const EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept
{
    if(_resourceUsage == EGraphics::ResourceHeapUsageType::Upload)
    {
        if(!mapReadRange || mapReadRange->Begin < mapReadRange->End)
        { return nullptr; }

        if(mapType == EResource::MapType::ReadOnly)
        { return nullptr; }

        void* mapping;

        const CD3DX12_RANGE range(0, 0);
        const HRESULT res = _d3dResource->Map(0, &range, &mapping);

        if(FAILED(res))
        { return nullptr; }

        return mapping;
    }
    else if(_resourceUsage == EGraphics::ResourceHeapUsageType::Read)
    {
        if(mapReadRange && mapReadRange->Begin >= mapReadRange->End)
        { return nullptr; }

        void* mapping;
        if(mapType == EResource::MapType::Discard)
        { return nullptr; }

        if(mapReadRange)
        {
            const CD3DX12_RANGE range(mapReadRange->Begin, mapReadRange->End);
            const HRESULT res = _d3dResource->Map(0, &range, &mapping);

            if(FAILED(res))
            { return nullptr; }
        }
        else
        {
            const HRESULT res = _d3dResource->Map(0, nullptr, &mapping);

            if(FAILED(res))
            { return nullptr; }
        }
        return mapping;
    }

    return nullptr;
}

void DX12ResourceBuffer::unmap(const ResourceMapRange* mapWriteRange) noexcept
{
    if(mapWriteRange)
    {
        CD3DX12_RANGE range(mapWriteRange->Begin, mapWriteRange->End);
        _d3dResource->Unmap(0, &range);
    }
    else
    {
        _d3dResource->Unmap(0, nullptr);
    }
}
#endif
