#include "dx/dx10/DX10ResourceTexture.hpp"

#ifdef _WIN32

void* DX10ResourceTexture1D::map(IRenderingContext&, EResource::MapType mapType, uSys subResource, const ResourceMapRange* mapReadRange) noexcept
{
    if(_currentMapping)
    { return _currentMapping; }

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::NoWrite)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ, 0, &_currentMapping);
        }
        else if(mapType == EResource::MapType::Default)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ_WRITE, 0, &_currentMapping);
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
        _d3dTexture->Map(subResource, mapping, 0, &_currentMapping);
    }

    return _currentMapping;
}

void DX10ResourceTexture1D::unmap(IRenderingContext&, uSys subResource) noexcept
{
    if(_currentMapping)
    {
        _d3dTexture->Unmap(subResource);
    }
}
#endif
