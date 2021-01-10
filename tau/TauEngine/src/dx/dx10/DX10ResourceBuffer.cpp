#include "dx/dx10/DX10ResourceBuffer.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10CommandList.hpp"
#include "TauConfig.hpp"

void* DX10ResourceBuffer::map(const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
    void* mapping;

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::ReadOnly || (mapWriteRange && mapWriteRange->isNone()))
        {
            _d3dBuffer->Map(D3D10_MAP_READ, 0, &mapping);
        }
        else if(mapType == EResource::MapType::Default)
        {
            _d3dBuffer->Map(D3D10_MAP_READ_WRITE, 0, &mapping);
        }
        else
        {
            return null;
        }
    }
    else
    {
        D3D10_MAP dxMapType;
        switch(mapType)
        {
            case EResource::MapType::Default:     dxMapType = D3D10_MAP_WRITE; break;
            case EResource::MapType::Discard:     dxMapType = D3D10_MAP_WRITE_DISCARD; break;
            case EResource::MapType::NoOverwrite: dxMapType = D3D10_MAP_WRITE_NO_OVERWRITE; break;
            case EResource::MapType::ReadOnly:
            default: return null;
        }
        _d3dBuffer->Map(dxMapType, 0, &mapping);

        if(mapWriteRange && !mapWriteRange->isNone())
        {
            mapping = reinterpret_cast<u8*>(mapping) + mapWriteRange->begin;
        }
    }

    return mapping;
}

void DX10ResourceTransferBuffer::transferMapping(void* const ptr) noexcept
{
#if TAU_NULL_CHECK
    if(!ptr)
    { return; }
#endif

    deallocate(_mapping);
    _mapping = ptr;
}

void* DX10ResourceTransferBuffer::transferMapping() noexcept
{
    void* const ret = _mapping;
    _mapping = allocate(_args.size);
    return ret;
}

void* DX10ResourceTransferBuffer::map() noexcept
{
    return _mapping;
}

void DX10ResourceTransferBuffer::unmap() noexcept
{
    _d3dBuffer->Unmap();
}
#endif
