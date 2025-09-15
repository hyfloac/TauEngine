#include "dx/dx10/DX10ResourceBuffer.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10CommandList.hpp"
#include "TauConfig.hpp"

void* DX10ResourceDefaultBuffer::map(uSys, uSys, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
    void* mapping;

    if(!mapReadRange || mapReadRange->Begin < mapReadRange->End)
    {
        if(_usageType == EResource::UsageType::ReadBack || (mapWriteRange && mapWriteRange->IsNone()))
        {
            _d3dBuffer->Map(D3D10_MAP_READ, 0, &mapping);
        }
        else if(_usageType == EResource::UsageType::Default)
        {
            _d3dBuffer->Map(D3D10_MAP_READ_WRITE, 0, &mapping);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        D3D10_MAP dxMapType;
        switch(_usageType)
        {
            case EResource::UsageType::Default:     dxMapType = D3D10_MAP_WRITE; break;
            case EResource::UsageType::Upload:      dxMapType = D3D10_MAP_WRITE_NO_OVERWRITE; break;
            case EResource::UsageType::ReadBack:    dxMapType = D3D10_MAP_READ; break;
            default: return nullptr;
        }
        _d3dBuffer->Map(dxMapType, 0, &mapping);

        if(mapWriteRange && !mapWriteRange->IsNone())
        {
            mapping = static_cast<u8*>(mapping) + mapWriteRange->Begin;
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

    DX10TransferResource::deallocate(_mapping);
    _mapping = ptr;
}

void* DX10ResourceTransferBuffer::transferMapping() noexcept
{
    void* const ret = _mapping;
    _mapping = DX10TransferResource::allocate(_args.size);
    return ret;
}

void* DX10ResourceTransferBuffer::map() noexcept
{
    return _mapping;
}

void DX10ResourceTransferBuffer::unmap() noexcept
{
    // _d3dBuffer->Unmap();
}
#endif
