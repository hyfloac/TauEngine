#include "Renderer.hpp"
#include "Window.hpp"
#include "d3dx12.h"
#include <d3dcompiler.h>

static void chooseGPU(IDXGIFactory4* factory, IDXGIAdapter1** adapter) noexcept;

Renderer::~Renderer() noexcept
{
    if(_fenceEvent)
    {
        if(_commandQueue && _swapChain && _fence)
        { waitForLastFrame(); }
        CloseHandle(_fenceEvent);
    }

    if(_uploadFenceEvent)
    {
        if(_uploadQueue && _uploadFence)
        { waitForUpload(); }
        CloseHandle(_uploadFenceEvent);
    }

    delete _bufferAllocator;
    delete _uploadBufferAllocator;
}

RetCode Renderer::loadPipeline(const Window& window) noexcept
{
    _viewport.Width = static_cast<FLOAT>(window.cWidth());
    _viewport.Height = static_cast<FLOAT>(window.cHeight());
    _scissorRect.right = window.cWidth();
    _scissorRect.bottom = window.cHeight();

#ifdef _DEBUG
    winrt::com_ptr<ID3D12Debug> debugController;
    if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    { debugController->EnableDebugLayer(); }
#endif

    winrt::com_ptr<IDXGIFactory4> dxgiFactory;
    HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if(FAILED(res))
    { return dxgiFactorySetupError(res); }

    chooseGPU(dxgiFactory.get(), _gpu.put());
    if(!_gpu)
    { return RC_Error; }

    res = D3D12CreateDevice(_gpu.get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&_device));
    if(FAILED(res))
    { return deviceSetupError(res); }

    RetCode ret = setupCommandQueue();
    if(ret != RC_Success)
    { return ret; }

    ret = setupSwapChain(dxgiFactory, window);
    if(ret != RC_Success)
    { return ret; }

    ret = setupRTVDescriptorHeap();
    if(ret != RC_Success)
    { return ret; }

    ret = setupFrames();
    if(ret != RC_Success)
    { return ret; }

    ret = setupAllocators();
    if(ret != RC_Success)
    { return ret; }

    return RC_Success;
}

RetCode Renderer::loadAssets(const Window& window) noexcept
{
    RetCode ret = setupRootSignature();
    if(ret != RC_Success)
    { return ret; }

    ret = setupPipelineState();
    if(ret != RC_Success)
    { return ret; }

    ret = setupCommandList();
    if(ret != RC_Success)
    { return ret; }

    ret = setupFence();
    if(ret != RC_Success)
    { return ret; }

    if(FAILED(_uploadAllocator->Reset()))
    { return RC_Error; }

    ret = setupTriangle(window);
    if(ret != RC_Success)
    { return ret; }

    ret = populateCommandList(0);
    if(ret != RC_Success)
    { return ret; }

    ret = populateCommandList(1);
    if(ret != RC_Success)
    { return ret; }

    ret = waitForUpload();
    if(ret != RC_Success)
    { return ret; }

    return RC_Success;
}

RetCode Renderer::update() noexcept
{
    _topColor.rotate();
    _leftColor.rotate();
    _rightColor.rotate();

    if(FAILED(_uploadAllocator->Reset()))
    { return RC_Error; }

    if(FAILED(_uploadCmdList->Reset(_uploadAllocator.get(), NULL)))
    { return RC_Error; }

    _triangleRenderer.updateColor(_topColor, _leftColor, _rightColor, _uploadCmdList.get());

    if(FAILED(_uploadCmdList->Close()))
    { return RC_Error; }

    ID3D12CommandList* commandLists[1] = { _uploadCmdList.get() };
    _uploadQueue->ExecuteCommandLists(1, commandLists);

    const RetCode ret = waitForUpload();
    if(ret != RC_Success)
    { return ret; }

    return RC_Success;
}

RetCode Renderer::render() noexcept
{
    RetCode ret = populateCommandList(_frameIndex);
    if(ret != RC_Success)
    { return ret; }

    ID3D12CommandList* commandLists[1] = { _cmdLists[_frameIndex].get() };
    _commandQueue->ExecuteCommandLists(1, commandLists);

    _swapChain->Present(0, 0);
    ret = waitForLastFrame();
    if(ret != RC_Success)
    { return ret; }

    return RC_Success;
}

RetCode Renderer::onWindowSizeChange(const UINT width, const UINT height) noexcept
{
    RetCode ret = waitForLastFrame();
    if(ret != RC_Success)
    { return ret; }

    for(UINT i = 0; i < FrameCount; ++i)
    {
        _renderTargets[i] = NULL;
    }

    _swapChain->ResizeBuffers(FrameCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    _viewport.Width = static_cast<FLOAT>(width);
    _viewport.Height = static_cast<FLOAT>(height);
    _scissorRect.right = width;
    _scissorRect.bottom = height;

    ret = setupFrames();
    if(ret != RC_Success)
    { return ret; }

    if(FAILED(_uploadAllocator->Reset()))
    { return RC_Error; }

    ret = updateTriangleSize(width, height);
    if(ret != RC_Success)
    { return ret; }

    ret = populateCommandList(0);
    if(ret != RC_Success)
    { return ret; }

    ret = populateCommandList(1);
    if(ret != RC_Success)
    { return ret; }

    ret = waitForUpload();
    if(ret != RC_Success)
    { return ret; }

    return RC_Success;
}

RetCode Renderer::setupCommandQueue() noexcept
{
    {
        D3D12_COMMAND_QUEUE_DESC commandQueueArgs;
        commandQueueArgs.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        commandQueueArgs.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        commandQueueArgs.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        commandQueueArgs.NodeMask = 0;

        const HRESULT res = _device->CreateCommandQueue(&commandQueueArgs, IID_PPV_ARGS(&_commandQueue));
        if(FAILED(res))
        { return commandQueueSetupError(res); }
    }

    {
        D3D12_COMMAND_QUEUE_DESC uploadQueueArgs;
        uploadQueueArgs.Type = D3D12_COMMAND_LIST_TYPE_COPY;
        uploadQueueArgs.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        uploadQueueArgs.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        uploadQueueArgs.NodeMask = 0;

        const HRESULT res = _device->CreateCommandQueue(&uploadQueueArgs, IID_PPV_ARGS(&_uploadQueue));
        if(FAILED(res))
        { return uploadQueueSetupError(res); }
    }

    return RC_Success;
}

RetCode Renderer::setupSwapChain(const winrt::com_ptr<IDXGIFactory4>& dxgiFactory, const Window& window) noexcept
{
    DXGI_SWAP_CHAIN_DESC1 swapChainArgs;
    swapChainArgs.Width = 0;
    swapChainArgs.Height = 0;
    swapChainArgs.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainArgs.Stereo = FALSE;
    swapChainArgs.SampleDesc.Count = 1;
    swapChainArgs.SampleDesc.Quality = 0;
    swapChainArgs.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainArgs.BufferCount = FrameCount;
    swapChainArgs.Scaling = DXGI_SCALING_NONE;
    swapChainArgs.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainArgs.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainArgs.Flags = 0;

    winrt::com_ptr<IDXGISwapChain1> tmpSwapChain;
    const HRESULT swapChainResult = dxgiFactory->CreateSwapChainForHwnd(_commandQueue.get(), window.hWnd(), &swapChainArgs, NULL, NULL, tmpSwapChain.put());
    if(FAILED(swapChainResult))
    { return swapChainSetupError(swapChainResult); }

    if(!tmpSwapChain.try_as(_swapChain))
    {
        MessageBoxW(NULL, L"Unable to retrieve IDXGISwapChain3 from IDXGISwapChain1.", NULL, MB_OK | MB_ICONERROR);
        return RC_ErrorReported;
    }

    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    return RC_Success;
}

RetCode Renderer::setupRTVDescriptorHeap() noexcept
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapArgs;
    rtvHeapArgs.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapArgs.NumDescriptors = FrameCount;
    rtvHeapArgs.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapArgs.NodeMask = 0;

    if(FAILED(_device->CreateDescriptorHeap(&rtvHeapArgs, IID_PPV_ARGS(&_rtvHeap))))
    { return RC_Error; }

    _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    return RC_Success;
}

RetCode Renderer::setupFrames() noexcept
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for(UINT n = 0; n < FrameCount; ++n)
    {
        if(FAILED(_swapChain->GetBuffer(n, IID_PPV_ARGS(&_renderTargets[n]))))
        { return RC_Error; }
        _device->CreateRenderTargetView(_renderTargets[n].get(), NULL, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);
    }

    return RC_Success;
}

RetCode Renderer::setupAllocators() noexcept
{
    for(UINT i = 0; i < FrameCount; ++i)
    {
        if(FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocators[i]))))
        { return RC_Error; }
    }

    if(FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&_uploadAllocator))))
    { return RC_Error; }

    if(FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&_bundleAllocator))))
    { return RC_Error; }

    return RC_Success;
}

RetCode Renderer::setupRootSignature() noexcept
{
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureArgs;
    rootSignatureArgs.Init(0, NULL, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    winrt::com_ptr<ID3DBlob> signature;
    winrt::com_ptr<ID3DBlob> error;

    if(FAILED(D3D12SerializeRootSignature(&rootSignatureArgs, D3D_ROOT_SIGNATURE_VERSION_1, signature.put(), error.put())))
    { return RC_Error; }

    if(FAILED(_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature))))
    { return RC_Error; }

    return RC_Success;
}

RetCode Renderer::setupPipelineState() noexcept
{
#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
    const UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    const UINT compileFlags = 0;
#endif

    winrt::com_ptr<ID3DBlob> vsShader;
    winrt::com_ptr<ID3DBlob> psShader;

    if(FAILED(D3DCompileFromFile(L"D:\\TauEngine\\test\\RayMarchingTest\\resources\\BasicVS.hlsl", NULL, NULL, "vsMain", "vs_5_0", compileFlags, 0, vsShader.put(), NULL)))
    { return RC_Error; }

    if(FAILED(D3DCompileFromFile(L"D:\\TauEngine\\test\\RayMarchingTest\\resources\\BasicPS.hlsl", NULL, NULL, "psMain", "ps_5_0", compileFlags, 0, psShader.put(), NULL)))
    { return RC_Error; }

    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineArgs;
    pipelineArgs.pRootSignature = _rootSignature.get();
    pipelineArgs.VS = { vsShader->GetBufferPointer(), vsShader->GetBufferSize() };
    pipelineArgs.PS = { psShader->GetBufferPointer(), psShader->GetBufferSize() };
    pipelineArgs.DS = { NULL, 0 };
    pipelineArgs.HS = { NULL, 0 };
    pipelineArgs.GS = { NULL, 0 };
    pipelineArgs.StreamOutput.pSODeclaration = NULL;
    pipelineArgs.StreamOutput.NumEntries = 0;
    pipelineArgs.StreamOutput.pBufferStrides = NULL;
    pipelineArgs.StreamOutput.NumStrides = 0;
    pipelineArgs.StreamOutput.RasterizedStream = 0;
    pipelineArgs.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pipelineArgs.SampleMask = UINT_MAX;
    pipelineArgs.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pipelineArgs.DepthStencilState.DepthEnable = TRUE;
    pipelineArgs.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    pipelineArgs.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    pipelineArgs.DepthStencilState.StencilEnable = FALSE;
    pipelineArgs.DepthStencilState.StencilReadMask = 0xFF;
    pipelineArgs.DepthStencilState.StencilWriteMask = 0xFF;
    pipelineArgs.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    pipelineArgs.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    pipelineArgs.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    pipelineArgs.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    pipelineArgs.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    pipelineArgs.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    pipelineArgs.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    pipelineArgs.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    pipelineArgs.InputLayout = { inputLayout, 2 };
    pipelineArgs.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    pipelineArgs.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineArgs.NumRenderTargets = 1;
    pipelineArgs.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pipelineArgs.RTVFormats[1] = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.RTVFormats[2] = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.RTVFormats[3] = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.RTVFormats[4] = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.RTVFormats[5] = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.RTVFormats[6] = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.RTVFormats[7] = DXGI_FORMAT_UNKNOWN;
    // pipelineArgs.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    pipelineArgs.DSVFormat = DXGI_FORMAT_UNKNOWN;
    pipelineArgs.SampleDesc.Count = 1;
    pipelineArgs.SampleDesc.Quality = 0;
    pipelineArgs.NodeMask = 0;
    pipelineArgs.CachedPSO = { NULL, 0 };
    pipelineArgs.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

    if(FAILED(_device->CreateGraphicsPipelineState(&pipelineArgs, IID_PPV_ARGS(&_pipelineState))))
    { return RC_Error; }

    return RC_Success;
}

RetCode Renderer::setupCommandList() noexcept
{
    {
        if(FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocators[0].get(), _pipelineState.get(), IID_PPV_ARGS(_cmdLists[0].put()))))
        { return RC_Error; }

        if(FAILED(_cmdLists[0]->Close()))
        { return RC_Error; }

        if(FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocators[1].get(), _pipelineState.get(), IID_PPV_ARGS(_cmdLists[1].put()))))
        { return RC_Error; }

        if(FAILED(_cmdLists[1]->Close()))
        { return RC_Error; }

        if(FAILED(_commandAllocators[0]->Reset()))
        { return RC_Error; }

        if(FAILED(_commandAllocators[1]->Reset()))
        { return RC_Error; }
    }

    if(FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COPY, _uploadAllocator.get(), NULL, IID_PPV_ARGS(_uploadCmdList.put()))))
    { return RC_Error; }

    if(FAILED(_uploadCmdList->Close()))
    { return RC_Error; }

    return RC_Success;
}

RetCode Renderer::setupFence() noexcept
{
    if(FAILED(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence))))
    { return RC_Error; }

    if(FAILED(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_uploadFence))))
    { return RC_Error; }

    _fenceEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
    if(!_fenceEvent && FAILED(HRESULT_FROM_WIN32(GetLastError())))
    { return RC_Error; }

    _uploadFenceEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
    if(!_uploadFenceEvent && FAILED(HRESULT_FROM_WIN32(GetLastError())))
    { return RC_Error; }

    waitForLastFrame();
    waitForUpload();

    return RC_Success;
}

RetCode Renderer::setupTriangle(const Window& window) noexcept
{
    if(FAILED(_uploadAllocator->Reset()))
    { return RC_Error; }

    if(FAILED(_uploadCmdList->Reset(_uploadAllocator.get(), NULL)))
    { return RC_Error; }

    _bufferAllocator = new BufferAllocator(_device, BufferAllocator::Type::Default);
    _uploadBufferAllocator = new BufferAllocator(_device, BufferAllocator::Type::Upload);
    _triangleRenderer.initBuffers(*_bufferAllocator, *_uploadBufferAllocator, window, _uploadCmdList.get());

    if(FAILED(_uploadCmdList->Close()))
    { return RC_Error; }

    ID3D12CommandList* commandLists[1] = { _uploadCmdList.get() };
    _uploadQueue->ExecuteCommandLists(1, commandLists);

    _triangleRenderer.initCmdLists(_device.get(), _bundleAllocator.get(), _pipelineState.get());

    return RC_Success;
}

RetCode Renderer::updateTriangleSize(const UINT width, const UINT height)
{
    if(FAILED(_uploadAllocator->Reset()))
    { return RC_Error; }

    if(FAILED(_uploadCmdList->Reset(_uploadAllocator.get(), NULL)))
    { return RC_Error; }

    _triangleRenderer.updateSize(width, height, _uploadCmdList.get());

    if(FAILED(_uploadCmdList->Close()))
    { return RC_Error; }

    ID3D12CommandList* commandLists[1] = { _uploadCmdList.get() };
    _uploadQueue->ExecuteCommandLists(1, commandLists);

    return RC_Success;
}

RetCode Renderer::populateCommandList(const UINT frameIndex) noexcept
{
    if(FAILED(_commandAllocators[frameIndex]->Reset()))
    { return RC_Error; }

    if(FAILED(_cmdLists[frameIndex]->Reset(_commandAllocators[frameIndex].get(), _pipelineState.get())))
    { return RC_Error; }

    _cmdLists[frameIndex]->SetGraphicsRootSignature(_rootSignature.get());
    _cmdLists[frameIndex]->RSSetViewports(1, &_viewport);
    _cmdLists[frameIndex]->RSSetScissorRects(1, &_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    const CD3DX12_RESOURCE_BARRIER resourceBarrierBegin = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[frameIndex].get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _cmdLists[frameIndex]->ResourceBarrier(1, &resourceBarrierBegin);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, _rtvDescriptorSize);
    _cmdLists[frameIndex]->OMSetRenderTargets(1, &rtvHandle, FALSE, NULL);

    const float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    _cmdLists[frameIndex]->ClearRenderTargetView(rtvHandle, clearColor, 0, NULL);

    _triangleRenderer.render(_cmdLists[frameIndex].get());

    // _cmdLists[frameIndex]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //
    // const D3D12_VERTEX_BUFFER_VIEW buffers[2] = { _vertices, _colors };
    //
    // _cmdLists[frameIndex]->IASetVertexBuffers(0, 2, buffers);
    // _cmdLists[frameIndex]->DrawInstanced(3, 1, 0, 0);

    // Indicate that the back buffer will now be used to present.
    const CD3DX12_RESOURCE_BARRIER resourceBarrierEnd = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[frameIndex].get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    _cmdLists[frameIndex]->ResourceBarrier(1, &resourceBarrierEnd);

    if(FAILED(_cmdLists[frameIndex]->Close()))
    { return RC_Error; }

    return RC_Success;
}

RetCode Renderer::waitForLastFrame() noexcept
{
    // Signal and increment fence value
    const UINT64 fenceValue = _fenceValue;
    if(FAILED(_commandQueue->Signal(_fence.get(), fenceValue)))
    { return RC_Error; }
    ++_fenceValue;

    // Wait until the last frame is finished
    if(_fence->GetCompletedValue() < fenceValue)
    {
        if(FAILED(_fence->SetEventOnCompletion(fenceValue, _fenceEvent)))
        { return RC_Error; }
        WaitForSingleObject(_fenceEvent, INFINITE);
    }

    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    return RC_Success;
}

RetCode Renderer::waitForUpload() noexcept
{
    // Signal and increment fence value
    const UINT64 fenceValue = _uploadFenceValue;
    if(FAILED(_uploadQueue->Signal(_uploadFence.get(), fenceValue)))
    { return RC_Error; }
    ++_uploadFenceValue;

    // Wait until the last frame is finished
    if(_uploadFence->GetCompletedValue() < fenceValue)
    {
        if(FAILED(_uploadFence->SetEventOnCompletion(fenceValue, _uploadFenceEvent)))
        { return RC_Error; }
        WaitForSingleObject(_uploadFenceEvent, INFINITE);
    }

    return RC_Success;
}

static void chooseGPU(IDXGIFactory4* const factory, IDXGIAdapter1** const adapter) noexcept
{
    for(UINT i = 0; true; ++i)
    {
        IDXGIAdapter1* currAdapter;
        if(factory->EnumAdapters1(i, &currAdapter) == DXGI_ERROR_NOT_FOUND)
        { break; }

        // Check to see if GPU supports DirectX 12.1
        if(SUCCEEDED(D3D12CreateDevice(currAdapter, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device5), NULL)))
        {
            *adapter = currAdapter;
            return;
        }
        currAdapter->Release();
    }
}
