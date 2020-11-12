#pragma once

#include "DX12Resource.hpp"

#ifdef _WIN32
class TAU_DLL DX12ResourceBuffer : public DX12Resource
{
    DEFAULT_DESTRUCT_VI(DX12ResourceBuffer);
    DELETE_CM(DX12ResourceBuffer);
protected:
    ResourceBufferArgs _args;
public:
    DX12ResourceBuffer(const uSys size, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceBufferArgs& args) noexcept
        : DX12Resource(size, EResource::Type::Buffer, d3dResource, allocation, resourceUsage)
        , _args(args)
    { }

    [[nodiscard]] void* map(ICommandList&, EResource::MapType mapType, uSys, uSys, const ResourceMapRange* const mapReadRange, const ResourceMapRange*) noexcept override
    { return map(mapType, mapReadRange); }

    void unmap(ICommandList&, uSys, uSys, const ResourceMapRange* const mapWriteRange) noexcept override
    { unmap(mapWriteRange); }

    [[nodiscard]] void* map(EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept;
    void unmap(const ResourceMapRange* mapWriteRange) noexcept;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

/**
 * A form of buffer that cannot be mapped by itself.
 *
 *   In this case an upload buffer must be allocated to be used
 * to store the data. After unmapping a copy instruction is
 * automatically added to the command list.
 *
 *   If this buffer has a usage type of Default the upload
 * buffer is released after unmapping. If this buffer has a
 * usage type of Dynamic then it will keep the buffer for
 * better performance. If you want to create a buffer
 * specifically for uploading then use the usage type
 * Streaming.
 */
class TAU_DLL DX12ResourceBufferIndirectMapping final : public DX12ResourceBuffer
{
    DELETE_CM(DX12ResourceBufferIndirectMapping);
private:
    ID3D12Resource* _uploadBuffer;
    D3D12MA::Allocation* _uploadAllocation;
    DX12GraphicsInterface& _gi;
    bool _keepUploadAllocation;
public:
    DX12ResourceBufferIndirectMapping(const uSys size, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceBufferArgs& args, DX12GraphicsInterface& gi, const bool keepUploadAllocation) noexcept
        : DX12ResourceBuffer(size, d3dResource, allocation, resourceUsage, args)
        , _uploadBuffer(nullptr)
        , _uploadAllocation(nullptr)
        , _gi(gi)
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
    { return map(); }

    void unmap(ICommandList& cmdList, uSys, uSys, const ResourceMapRange* mapWriteRange) noexcept override
    { unmap(cmdList, mapWriteRange); }

    [[nodiscard]] void* map() noexcept;
    void unmap(ICommandList& cmdList, const ResourceMapRange* mapWriteRange) noexcept;
};

#endif
