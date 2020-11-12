#pragma once

#include "DX12Resource.hpp"

#ifdef _WIN32

class TAU_DLL DX12ResourceTexture1D final : public DX12Resource
{
    DEFAULT_DESTRUCT(DX12ResourceTexture1D);
    DELETE_CM(DX12ResourceTexture1D);
private:
    ResourceTexture1DArgs _args;
public:
    DX12ResourceTexture1D(const uSys size, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceTexture1DArgs& args) noexcept
        : DX12Resource(size, EResource::Type::Texture1D, d3dResource, allocation, resourceUsage)
        , _args(args)
    { }

    [[nodiscard]] void* map(ICommandList& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& context, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapWriteRange) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

class TAU_DLL DX12ResourceTexture2D final : public DX12Resource
{
    DEFAULT_DESTRUCT(DX12ResourceTexture2D);
    DELETE_CM(DX12ResourceTexture2D);
private:
    ResourceTexture2DArgs _args;
public:
    DX12ResourceTexture2D(const uSys size, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceTexture2DArgs& args) noexcept
        : DX12Resource(size, EResource::Type::Texture1D, d3dResource, allocation, resourceUsage)
        , _args(args)
    { }

    [[nodiscard]] void* map(ICommandList& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& context, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapWriteRange) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

class TAU_DLL DX12ResourceTexture3D final : public DX12Resource
{
    DEFAULT_DESTRUCT(DX12ResourceTexture3D);
    DELETE_CM(DX12ResourceTexture3D);
private:
    ResourceTexture3DArgs _args;
public:
    DX12ResourceTexture3D(const uSys size, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage, const ResourceTexture3DArgs& args) noexcept
        : DX12Resource(size, EResource::Type::Texture1D, d3dResource, allocation, resourceUsage)
        , _args(args)
    { }

    [[nodiscard]] void* map(ICommandList& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(ICommandList& context, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapWriteRange) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
#endif
