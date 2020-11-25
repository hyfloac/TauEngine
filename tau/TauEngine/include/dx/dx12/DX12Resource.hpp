#pragma once

#include "graphics/Resource.hpp"

#ifdef _WIN32
#include <d3d12.h>
#include <atomic>
#include "D3D12MemAlloc.h"
#include "graphics/ResourceRawInterface.hpp"
#include "graphics/GraphicsEnums.hpp"

class DX12GraphicsInterface;

class TAU_DLL DX12ResourceRawInterface final : public IResourceRawInterface
{
    DEFAULT_DESTRUCT(DX12ResourceRawInterface);
    DEFAULT_CM_PU(DX12ResourceRawInterface);
private:
    ID3D12Resource* _d3dResource;
public:
    DX12ResourceRawInterface(ID3D12Resource* const d3dResource) noexcept
        : _d3dResource(d3dResource)
    { }

    [[nodiscard]] void* rawHandle() const noexcept override { return _d3dResource; }

    [[nodiscard]] ID3D12Resource* dx12Resource() const noexcept override { return _d3dResource; }
private:
    friend class DX12Resource;
};

class TAU_DLL TAU_NOVTABLE DX12Resource : public IResource
{
    DELETE_CM(DX12Resource);
    RESOURCE_IMPL(DX12Resource);
protected:
    DX12ResourceRawInterface _rawInterface;
    __declspec(property(get = d3dResource, put = setD3DResource)) ID3D12Resource* _d3dResource;
    D3D12MA::Allocation* _allocation;
    EGraphics::ResourceHeapUsageType _resourceUsage;
public:
    DX12Resource(const uSys size, const EResource::Type resourceType, ID3D12Resource* const d3dResource, D3D12MA::Allocation* const allocation, const EGraphics::ResourceHeapUsageType resourceUsage) noexcept
        : IResource(size, resourceType)
        , _rawInterface(d3dResource)
        , _allocation(allocation)
        , _resourceUsage(resourceUsage)
    { }

    ~DX12Resource() noexcept override
    {
        if(_rawInterface._d3dResource)
        { _rawInterface._d3dResource->Release(); }

        if(_allocation)
        { _allocation->Release(); }
    }

    [[nodiscard]] ID3D12Resource* d3dResource() const noexcept { return _rawInterface._d3dResource; }
    [[nodiscard]] D3D12MA::Allocation* allocation() const noexcept { return _allocation; }
    [[nodiscard]] EGraphics::ResourceHeapUsageType resourceUsage() const noexcept { return _resourceUsage; }

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }
protected:
#if defined(_MSVC_LANG) || 1
    void setD3DResource(ID3D12Resource* resource) noexcept { _rawInterface._d3dResource = resource; }
#endif
};

class TAU_DLL DX12ResourceBuilder final : public IResourceBuilder
{
    DEFAULT_DESTRUCT(DX12ResourceBuilder);
    DELETE_CM(DX12ResourceBuilder);
private:
    DX12GraphicsInterface& _gi;
public:
    DX12ResourceBuilder(DX12GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] uSys _allocSize(uSys type) const noexcept override;
};
#endif
