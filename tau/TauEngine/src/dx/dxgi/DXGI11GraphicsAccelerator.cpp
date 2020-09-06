#include "dx/dxgi/DXGI11GraphicsAccelerator.hpp"

#ifdef _WIN32
#include "dx/dxgi/DXGI11GraphicsDisplay.hpp"

DXGI11GraphicsAccelerator::GAList DXGI11GraphicsAccelerator::graphicsAccelerators() noexcept
{
    IDXGIFactory1* dxgiFactory;
    HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

    if(FAILED(res))
    { return GAList(); }

    IDXGIAdapter1* dxgiAdapter;
    UINT adapterCount;
    for(adapterCount = 0; dxgiFactory->EnumAdapters1(adapterCount, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++adapterCount)
    { dxgiAdapter->Release(); }

    GAList accelerators(adapterCount);

    for(UINT i = 0; i < adapterCount; ++i)
    {
        res = dxgiFactory->EnumAdapters1(i, &dxgiAdapter);
        if(FAILED(res))
        {
            dxgiFactory->Release();
            return GAList();
        }

        accelerators[i] = build(dxgiAdapter);
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();
    return accelerators;
}

NullableRef<DXGI11GraphicsAccelerator> DXGI11GraphicsAccelerator::build(IDXGIAdapter1* const adapter) noexcept
{
    DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
    const HRESULT res = adapter->GetDesc1(&dxgiAdapterDesc);

    if(FAILED(res))
    { return null; }

    const uSys videoCardMemory = dxgiAdapterDesc.DedicatedVideoMemory;
    const uSys systemVideoMemory = dxgiAdapterDesc.DedicatedSystemMemory;
    const uSys sharedVideoMemory = dxgiAdapterDesc.SharedSystemMemory;

    const WDynString description(dxgiAdapterDesc.Description);

    return NullableRef<DXGI11GraphicsAccelerator>(DefaultTauAllocator::Instance(), L"", description, videoCardMemory, systemVideoMemory, sharedVideoMemory, adapter);
}

IGraphicsAccelerator::GDList DXGI11GraphicsAccelerator::graphicsDisplays() noexcept
{
    IDXGIOutput* dxgiOutput;
    UINT numOutputs;
    for(numOutputs = 0; _dxgiAdapter->EnumOutputs(numOutputs, &dxgiOutput) != DXGI_ERROR_NOT_FOUND; ++numOutputs)
    { dxgiOutput->Release(); }

    RefDynArray<NullableRef<IGraphicsDisplay>> displays(numOutputs);

    for(UINT i = 0; i < numOutputs; ++i)
    {
        const HRESULT res = _dxgiAdapter->EnumOutputs(i, &dxgiOutput);
        if(FAILED(res))
        { return GDList(); }
        displays[i] = DXGI11GraphicsDisplay::build(dxgiOutput);
        dxgiOutput->Release();
    }

    return displays;
}
#endif
