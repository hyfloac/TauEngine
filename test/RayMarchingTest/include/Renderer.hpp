#pragma once

#include <Objects.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <Windows.h>
#include <winrt/base.h>

#include "ColorCycler.hpp"
#include "TriangleRenderer.hpp"

enum RetCode
{
    RC_Success = 0,
    RC_Error,
    RC_ErrorReported
};

class Window;

class Renderer final
{
    DELETE_CM(Renderer);
private:
    static constexpr UINT FrameCount = 2;

    winrt::com_ptr<IDXGISwapChain3> _swapChain;
    winrt::com_ptr<IDXGIAdapter1> _gpu;
    winrt::com_ptr<ID3D12Device5> _device;
    winrt::com_ptr<ID3D12Resource> _renderTargets[FrameCount];
    winrt::com_ptr<ID3D12CommandAllocator> _commandAllocators[2];
    winrt::com_ptr<ID3D12CommandAllocator> _uploadAllocator;
    winrt::com_ptr<ID3D12CommandAllocator> _bundleAllocator;
    winrt::com_ptr<ID3D12CommandQueue> _commandQueue;
    winrt::com_ptr<ID3D12CommandQueue> _uploadQueue;
    winrt::com_ptr<ID3D12GraphicsCommandList> _cmdLists[2];
    winrt::com_ptr<ID3D12GraphicsCommandList> _uploadCmdList;
    winrt::com_ptr<ID3D12DescriptorHeap> _rtvHeap;
    winrt::com_ptr<ID3D12PipelineState> _pipelineState;
    winrt::com_ptr<ID3D12RootSignature> _rootSignature;

    D3D12_VIEWPORT _viewport;
    D3D12_RECT _scissorRect;

    UINT _frameIndex;
    UINT _rtvDescriptorSize;

    UINT64 _fenceValue;
    HANDLE _fenceEvent;
    winrt::com_ptr<ID3D12Fence> _fence;

    UINT64 _uploadFenceValue;
    HANDLE _uploadFenceEvent;
    winrt::com_ptr<ID3D12Fence> _uploadFence;

    ColorCycler _topColor;
    ColorCycler _leftColor;
    ColorCycler _rightColor;

    BufferAllocator* _bufferAllocator;
    BufferAllocator* _uploadBufferAllocator;
    TriangleRenderer _triangleRenderer;
public:
    Renderer() noexcept
        : _viewport { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
        , _scissorRect{ 0, 0, 0, 0 }
        , _frameIndex(0)
        , _rtvDescriptorSize(0)
        , _fenceValue(1)
        , _fenceEvent(NULL)
        , _uploadFenceValue(1)
        , _uploadFenceEvent(NULL)
        , _topColor(ColorCycler::initRotator(255, 0, 0, 1))
        , _leftColor(ColorCycler::initRotator(0, 255, 0, 1))
        , _rightColor(ColorCycler::initRotator(0, 0, 255, 1))
        , _bufferAllocator(nullptr)
        , _uploadBufferAllocator(nullptr)
    { }

    ~Renderer() noexcept;

    RetCode loadPipeline(const Window& window) noexcept;
    RetCode loadAssets(const Window& window) noexcept;

    RetCode update() noexcept;
    RetCode render() noexcept;

    RetCode onWindowSizeChange(UINT width, UINT height) noexcept;
private:
    RetCode setupCommandQueue() noexcept;
    RetCode setupSwapChain(const winrt::com_ptr<IDXGIFactory4>& dxgiFactory, const Window& window) noexcept;
    RetCode setupRTVDescriptorHeap() noexcept;
    RetCode setupFrames() noexcept;
    RetCode setupAllocators() noexcept;

    RetCode setupRootSignature() noexcept;
    RetCode setupPipelineState() noexcept;
    RetCode setupCommandList() noexcept;
    RetCode setupFence() noexcept;
    RetCode setupTriangle(const Window& window) noexcept;
    RetCode updateTriangleSize(UINT width, UINT height);

    RetCode populateCommandList(UINT frameIndex) noexcept;

    RetCode waitForLastFrame() noexcept;
    RetCode waitForUpload() noexcept;
private:
    [[nodiscard]] RetCode dxgiFactorySetupError(HRESULT res) const noexcept;
    [[nodiscard]] RetCode deviceSetupError(HRESULT res) const noexcept;

    [[nodiscard]] RetCode commandQueueSetupError(HRESULT res) const noexcept;
    [[nodiscard]] RetCode uploadQueueSetupError(HRESULT res) const noexcept;

    [[nodiscard]] RetCode swapChainSetupError(HRESULT res) const noexcept;
};
