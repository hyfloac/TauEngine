#include "dx/dx10/DX10ResourceTexture.hpp"

#ifdef _WIN32
void* DX10ResourceTexture1D::map(const EResource::MapType mapType, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
    const UINT subResource = D3D10CalcSubresource(static_cast<UINT>(mipLevel), static_cast<UINT>(arrayIndex), _args.mipLevels);

    void* mapping;

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        if(mapType == EResource::MapType::NoWrite || (mapWriteRange && mapWriteRange->isNone()))
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

        if(mapWriteRange && !mapWriteRange->isNone())
        {
            mapping = reinterpret_cast<u8*>(mapping) + mapWriteRange->begin;
        }
    }

    return mapping;
}

void DX10ResourceTexture1D::unmap(const uSys mipLevel, const uSys arrayIndex) noexcept
{
    const UINT subResource = D3D10CalcSubresource(static_cast<UINT>(mipLevel), static_cast<UINT>(arrayIndex), _args.mipLevels);

    _d3dTexture->Unmap(subResource);
}

void* DX10ResourceTexture2D::map(const EResource::MapType mapType, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange) noexcept
{
    const UINT subResource = D3D10CalcSubresource(static_cast<UINT>(mipLevel), static_cast<UINT>(arrayIndex), _args.mipLevels);

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

void DX10ResourceTexture2D::unmap(const uSys mipLevel, const uSys arrayIndex) noexcept
{
    const UINT subResource = D3D10CalcSubresource(static_cast<UINT>(mipLevel), static_cast<UINT>(arrayIndex), _args.mipLevels);

    _d3dTexture->Unmap(subResource);
}

void* DX10ResourceTexture3D::map(const EResource::MapType mapType, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange) noexcept
{
    const UINT subResource = D3D10CalcSubresource(static_cast<UINT>(mipLevel), static_cast<UINT>(arrayIndex), _args.mipLevels);

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

void DX10ResourceTexture3D::unmap(const uSys mipLevel, const uSys arrayIndex) noexcept
{
    const UINT subResource = D3D10CalcSubresource(static_cast<UINT>(mipLevel), static_cast<UINT>(arrayIndex), _args.mipLevels);

    _d3dTexture->Unmap(subResource);
}
#endif
