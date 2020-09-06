#pragma once

#include "graphics/Resource.hpp"

#ifdef _WIN32
#include <d3d12.h>
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
    RESOURCE_IMPL(DX12Resource);
protected:
    DX12ResourceRawInterface _rawInterface;
    __declspec(property(get = d3dResource, put = setD3DResource)) ID3D12Resource* _d3dResource;
    EGraphics::ResourceHeapUsageType _resourceUsage;
public:
    DX12Resource(const uSys size, ID3D12Resource* const d3dResource, const EGraphics::ResourceHeapUsageType resourceUsage) noexcept
        : IResource(size)
        , _rawInterface(d3dResource)
        , _resourceUsage(resourceUsage)
    { }

    ~DX12Resource() noexcept override
    {
        if(_rawInterface._d3dResource)
        { _rawInterface._d3dResource->Release(); }
    }

    DX12Resource(const DX12Resource& copy)
        : IResource(copy)
        , _rawInterface(copy._rawInterface)
        , _resourceUsage(copy._resourceUsage)
    { _rawInterface._d3dResource->AddRef(); }

    DX12Resource(DX12Resource&& move) noexcept
        : IResource(std::move(move))
        , _rawInterface(std::move(move._rawInterface))
        , _resourceUsage(move._resourceUsage)
    { }

    DX12Resource& operator=(const DX12Resource& copy)
    {
        if(this == &copy)
        { return *this; }

        _rawInterface._d3dResource->Release();

        IResource::operator=(copy);
        _rawInterface = copy._rawInterface;
        _resourceUsage = copy._resourceUsage;

        _rawInterface._d3dResource->AddRef();

        return *this;
    }

    DX12Resource& operator=(DX12Resource&& move) noexcept
    {
        if(this == &move)
        { return *this; }
        
        _rawInterface._d3dResource->Release();

        IResource::operator=(std::move(move));
        _rawInterface = std::move(move._rawInterface);
        _resourceUsage = move._resourceUsage;

        move._rawInterface._d3dResource = null;

        return *this;
    }

    [[nodiscard]] ID3D12Resource* d3dResource() const noexcept { return _rawInterface._d3dResource; }
    [[nodiscard]] EGraphics::ResourceHeapUsageType resourceUsage() const noexcept { return _resourceUsage; }

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }
protected:
#if defined(_MSVC_LANG) || 1
    void setD3DResource(ID3D12Resource* resource) noexcept { _rawInterface._d3dResource = resource; }
#endif
};
#endif
