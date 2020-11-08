#pragma once

#include "DX12Resource.hpp"

#ifdef _WIN32
#include "DX12ResourceUploadHandler.hpp"

class TAU_DLL DX12ResourceBuffer : public DX12Resource
{
    DEFAULT_DESTRUCT_VI(DX12ResourceBuffer);
    DELETE_CM(DX12ResourceBuffer);
private:
    static DX12BufferResourceUploadHandler2* getResourceUploader() noexcept { return null; }
protected:
    ResourceBufferArgs _args;
public:
    DX12ResourceBuffer(const uSys size, ID3D12Resource* const d3dResource, const EGraphics::ResourceHeapUsageType resourceUsage, D3D12MA::Allocation* const allocation, DX12GraphicsInterface& gi, const ResourceBufferArgs& args) noexcept
        : DX12Resource(size, d3dResource, resourceUsage, allocation, gi)
        , _args(args)
    { }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Buffer; }

    [[nodiscard]] void* map(ICommandList& cmdList, EResource::MapType mapType, uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& cmdList, uSys, uSys, const ResourceMapRange* mapWriteRange) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

class TAU_DLL DX12ResourceBufferIndirectMapping final : public DX12ResourceBuffer
{
    DELETE_CM(DX12ResourceBufferIndirectMapping);
private:
    ID3D12Resource* _uploadBuffer;
    D3D12MA::Allocation* _uploadAllocation;
    bool _keepUploadAllocation;
public:
    DX12ResourceBufferIndirectMapping(const uSys size, ID3D12Resource* const d3dResource, const EGraphics::ResourceHeapUsageType resourceUsage, D3D12MA::Allocation* const allocation, DX12GraphicsInterface& gi, const ResourceBufferArgs& args, const bool keepUploadAllocation) noexcept
        : DX12ResourceBuffer(size, d3dResource, resourceUsage, allocation, gi, args)
        , _uploadBuffer(nullptr)
        , _uploadAllocation(nullptr)
        , _keepUploadAllocation(keepUploadAllocation)
    { }

    ~DX12ResourceBufferIndirectMapping() noexcept override
    {
        if(_uploadBuffer)
        { _uploadBuffer->Release(); }

        if(_uploadAllocation)
        { _uploadAllocation->Release(); }
    }

    [[nodiscard]] void* map(ICommandList& cmdList, EResource::MapType mapType, uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& cmdList, uSys, uSys, const ResourceMapRange* mapWriteRange) noexcept override;
};

#endif
