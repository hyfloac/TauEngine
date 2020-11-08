#include "dx/dx12/DX12GraphicsInterface.hpp"

#ifdef _WIN32
#include "dx/dxgi/DXGI16GraphicsAccelerator.hpp"
#include "dx/dx12/D3D12MemAlloc.h"

DX12GraphicsInterface::~DX12GraphicsInterface() noexcept
{
    _gpuAllocator->Release();
    _d3d12Device->Release();
}

IGraphicsInterface* DX12GraphicsInterfaceBuilder::build(const GraphicsInterfaceArgs& args, Error* error) const noexcept
{
    DXGraphicsInterfaceArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX12GraphicsInterface* const gi = new(::std::nothrow) DX12GraphicsInterface(args.renderingMode, dxArgs.d3dDevice, dxArgs.gpuAllocator);
    ERROR_CODE_COND_N(!gi, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, gi);
}

IGraphicsInterface* DX12GraphicsInterfaceBuilder::build(const GraphicsInterfaceArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXGraphicsInterfaceArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX12GraphicsInterface* const gi = allocator.allocateT<DX12GraphicsInterface>(args.renderingMode, dxArgs.d3dDevice, dxArgs.gpuAllocator);
    ERROR_CODE_COND_N(!gi, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, gi);
}

NullableRef<IGraphicsInterface> DX12GraphicsInterfaceBuilder::buildTauRef(const GraphicsInterfaceArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXGraphicsInterfaceArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX12GraphicsInterface> gi(allocator, args.renderingMode, dxArgs.d3dDevice, dxArgs.gpuAllocator);
    ERROR_CODE_COND_N(!gi, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, gi);
}

bool DX12GraphicsInterfaceBuilder::processArgs(const GraphicsInterfaceArgs& args, DXGraphicsInterfaceArgs* const dxArgs, Error* error) const noexcept
{
    if(!RTT_CHECK(args.graphicsAccelerator.get(), DXGI16GraphicsAccelerator))
    { return null; }

    const NullableRef<DXGI16GraphicsAccelerator> gpu = RefCast<DXGI16GraphicsAccelerator>(args.graphicsAccelerator);

    constexpr D3D_FEATURE_LEVEL TargetLevel = D3D_FEATURE_LEVEL_12_0;

    HRESULT res = D3D12CreateDevice(gpu->dxgiAdapter(), TargetLevel, __uuidof(*dxArgs->d3dDevice), null);
    ERROR_CODE_COND_F(FAILED(res), Error::UnsupportedAPI);

    if(args.renderingMode.debugMode())
    {
        ID3D12Debug* debugController;
        if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            debugController->Release();
        }
    }

    res = D3D12CreateDevice(gpu->dxgiAdapter(), TargetLevel, IID_PPV_ARGS(&dxArgs->d3dDevice));
    ERROR_CODE_COND_F(FAILED(res) || !dxArgs->d3dDevice, Error::DriverMemoryAllocationFailure);

    D3D12MA::ALLOCATOR_DESC allocDesc;
    allocDesc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
    allocDesc.pDevice = dxArgs->d3dDevice;
    allocDesc.PreferredBlockSize = 0;
    allocDesc.pAllocationCallbacks = nullptr;

    res = D3D12MA::CreateAllocator(&allocDesc, &dxArgs->gpuAllocator);
    ERROR_CODE_COND_F(FAILED(res) || !dxArgs->gpuAllocator, Error::SystemMemoryAllocationFailure);

    return true;
}
#endif
