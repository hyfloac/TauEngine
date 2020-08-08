#pragma once

#include "Allocation.hpp"
#include "Allocator.hpp"
#include "Window.hpp"
#include "ColorCycler.hpp"

class TriangleRenderer final
{
public:
    static constexpr UINT DynamicCycles = 3;
private:
    Allocation _positionsBuffer;
    Allocation _colorBuffer;
    Allocation _uploadBuffer;
    UINT _dynamicCycle;
    winrt::com_ptr<ID3D12GraphicsCommandList> _dynamicBundles[DynamicCycles];
public:
    TriangleRenderer() noexcept
        : _positionsBuffer(nullptr, 0, 0)
        , _colorBuffer(nullptr, 0, 0)
        , _uploadBuffer(nullptr, 0, 0)
        , _dynamicCycle(0)
    { }

    void initBuffers(BufferAllocator& bufferAllocator, BufferAllocator& uploadAllocator, const Window& window, ID3D12GraphicsCommandList* uploadCmdList) noexcept;
    void initCmdLists(ID3D12Device* device, ID3D12CommandAllocator* bundleAllocator, ID3D12PipelineState* baseState) noexcept;

    void updateSize(UINT width, UINT height, ID3D12GraphicsCommandList* uploadCmdList) noexcept;
    void updateColor(const ColorCycler& top, const ColorCycler& right, const ColorCycler& left, ID3D12GraphicsCommandList* uploadCmdList) noexcept;

    void render(ID3D12GraphicsCommandList* renderCmdList) noexcept;
};
