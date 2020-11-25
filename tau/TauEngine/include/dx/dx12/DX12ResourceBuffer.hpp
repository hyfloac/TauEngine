#pragma once

#include "DX12Resource.hpp"
#include <system/Win32Event.hpp>

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

    [[nodiscard]] void* map(ICommandList&, const EResource::MapType mapType, uSys, uSys, const ResourceMapRange* const mapReadRange, const ResourceMapRange*) noexcept override
    { return map(mapType, mapReadRange); }

    void unmap(ICommandList&, uSys, uSys, const ResourceMapRange* const mapWriteRange) noexcept override
    { unmap(mapWriteRange); }

    [[nodiscard]] void* map(EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept;
    void unmap(const ResourceMapRange* mapWriteRange) noexcept;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

class TAU_DLL DX12ResourceBufferNoMapping final : public DX12ResourceBuffer
{
    DEFAULT_DESTRUCT(DX12ResourceBufferNoMapping);
    DELETE_CM(DX12ResourceBufferNoMapping);
public:
    DX12ResourceBufferNoMapping(const uSys size, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceBufferArgs& args) noexcept
        : DX12ResourceBuffer(size, d3dResource, allocation, resourceUsage, args)
    { }

    [[nodiscard]] void* map(ICommandList& cmdList, EResource::MapType mapType, uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override
    { return nullptr; }

    void unmap(ICommandList& cmdList, uSys, uSys, const ResourceMapRange* mapWriteRange) noexcept override
    { }
};
#endif
