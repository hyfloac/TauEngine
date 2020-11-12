#include "dx/dx12/DX12ResourceTexture.hpp"

#ifdef _WIN32
#include "dx/dx12/d3dx12.h"

void* DX12ResourceTexture1D::map(ICommandList& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept
{
    if(_args.usageType == EResource::UsageType::Immutable)
    { return null; }
}

void DX12ResourceTexture1D::unmap(ICommandList& context, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapWriteRange) noexcept
{
}
#endif
