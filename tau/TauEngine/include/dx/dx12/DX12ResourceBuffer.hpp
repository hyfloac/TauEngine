#pragma once

#include "DX12Resource.hpp"

#ifdef _WIN32
#include "DX12ResourceUploadHandler.hpp"

class TAU_DLL DX12ResourceBuffer final : public DX12Resource
{
private:
    static DX12BufferResourceUploadHandler2* getResourceUploader() noexcept { return null; }
private:
    ResourceBufferArgs _args;
    NullableRef<DX12AutoAllocation2> _uploadBuffer;
public:
    DX12ResourceBuffer(const uSys size, ID3D12Resource* const d3dResource, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceBufferArgs& args) noexcept
        : DX12Resource(size, d3dResource, resourceUsage)
        , _args(args)
        , _uploadBuffer(null)
    { }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Buffer; }

    [[nodiscard]] void* map(ICommandList& cmdList, EResource::MapType mapType, uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& cmdList, uSys, uSys, const ResourceMapRange* mapWriteRange) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
#endif
