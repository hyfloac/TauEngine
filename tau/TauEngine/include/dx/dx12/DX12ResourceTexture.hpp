#pragma once

#include "DX12Resource.hpp"

#ifdef _WIN32
#include "DX12ResourceUploadHandler.hpp"

class TAU_DLL DX12ResourceTexture1D final : public DX12Resource
{
private:
    ResourceTexture1DArgs _args;
public:
    DX12ResourceTexture1D(const uSys size, ID3D12Resource* const d3dResource, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceTexture1DArgs& args) noexcept
        : DX12Resource(size, d3dResource, resourceUsage)
        , _args(args)
    { }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture1D; }

    [[nodiscard]] void* map(ICommandList& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& context, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapWriteRange) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
#endif
