#pragma once

#include <d3d12.h>
#include <winrt/base.h>
#include <Objects.hpp>

class Allocation
{
    DEFAULT_DESTRUCT_VI(Allocation);
    DEFAULT_CM_PU(Allocation);
private:
    winrt::com_ptr<ID3D12Resource> _resource;
    UINT _offset;
    UINT _size;
public:
    Allocation(const winrt::com_ptr<ID3D12Resource>& resource, const UINT offset, const UINT size) noexcept
        : _resource(resource)
        , _offset(offset)
        , _size(size)
    { }

    [[nodiscard]] const winrt::com_ptr<ID3D12Resource>& resource() const noexcept { return _resource; }
    [[nodiscard]] UINT offset() const noexcept { return _offset; }
    [[nodiscard]] UINT size() const noexcept { return _size; }

    HRESULT map(const D3D12_RANGE* readRange, void** buffer) noexcept;
    void unmap(const D3D12_RANGE* writtenRange) noexcept;

    [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS getGPUVirtualAddress() const noexcept;
};
