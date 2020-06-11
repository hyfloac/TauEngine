#include "dx/dx10/DX10ResourceBuffer.hpp"

#ifdef _WIN32
void* DX10ResourceBuffer::map(IRenderingContext&, const EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept
{
    if(_currentMapping)
    { return _currentMapping; }

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::NoWrite)
        {
            _d3dBuffer->Map(D3D10_MAP_READ, 0, &_currentMapping);
        }
        else if(mapType == EResource::MapType::Default)
        {
            _d3dBuffer->Map(D3D10_MAP_READ_WRITE, 0, &_currentMapping);
        }
    }
    else
    {
        D3D10_MAP mapping;
        switch(mapType)
        {
            case EResource::MapType::Default:     mapping = D3D10_MAP_WRITE; break;
            case EResource::MapType::Discard:     mapping = D3D10_MAP_WRITE_DISCARD; break;
            case EResource::MapType::NoOverwrite: mapping = D3D10_MAP_WRITE_NO_OVERWRITE; break;
            case EResource::MapType::NoWrite:
            default: return null;
        }
        _d3dBuffer->Map(mapping, 0, &_currentMapping);
    }

    return _currentMapping;
}

void DX10ResourceBuffer::unmap(IRenderingContext&) noexcept
{
    if(_currentMapping)
    {
        _d3dBuffer->Unmap();
    }
}
#endif
