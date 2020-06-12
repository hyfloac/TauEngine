#include "dx/dx10/DX10ResourceTexture.hpp"

#ifdef _WIN32
void* DX10ResourceTexture1D::map(IRenderingContext&, const EResource::MapType mapType, const uSys subResource, const ResourceMapRange* const mapReadRange) noexcept
{
    void* mapping;

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::NoWrite)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ, 0, &mapping);
        }
        else if(mapType == EResource::MapType::Default)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ_WRITE, 0, &mapping);
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
            case EResource::MapType::NoWrite:
            default: return null;
        }
        _d3dTexture->Map(subResource, dxMapType, 0, &mapping);
    }

    return mapping;
}

void DX10ResourceTexture1D::unmap(IRenderingContext&, const uSys subResource) noexcept
{
    _d3dTexture->Unmap(subResource);
}

void* DX10ResourceTexture2D::map(IRenderingContext&, const EResource::MapType mapType, const uSys subResource, const ResourceMapRange* const mapReadRange) noexcept
{
    D3D10_MAPPED_TEXTURE2D mapping;

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::NoWrite)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ, 0, &mapping);
        }
        else if(mapType == EResource::MapType::Default)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ_WRITE, 0, &mapping);
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
            case EResource::MapType::NoWrite:
            default: return null;
        }
        _d3dTexture->Map(subResource, dxMapType, 0, &mapping);
    }

    return mapping.pData;
}

void DX10ResourceTexture2D::unmap(IRenderingContext&, const uSys subResource) noexcept
{
    _d3dTexture->Unmap(subResource);
}

void* DX10ResourceTexture3D::map(IRenderingContext&, const EResource::MapType mapType, const uSys subResource, const ResourceMapRange* const mapReadRange) noexcept
{
    D3D10_MAPPED_TEXTURE3D mapping;

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::NoWrite)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ, 0, &mapping);
        }
        else if(mapType == EResource::MapType::Default)
        {
            _d3dTexture->Map(subResource, D3D10_MAP_READ_WRITE, 0, &mapping);
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
            case EResource::MapType::NoWrite:
            default: return null;
        }
        _d3dTexture->Map(subResource, dxMapType, 0, &mapping);
    }

    return mapping.pData;
}

void DX10ResourceTexture3D::unmap(IRenderingContext&, const uSys subResource) noexcept
{
    _d3dTexture->Unmap(subResource);
}
#endif
