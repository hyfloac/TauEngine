#include "dx/dxgi/DXGI12GraphicsAccelerator.hpp"

#ifdef _WIN32
#include "dx/dxgi/DXGI12GraphicsDisplay.hpp"

DXGI12GraphicsAccelerator::GAList DXGI12GraphicsAccelerator::graphicsAccelerators() noexcept
{
    IDXGIFactory2* dxgiFactory;
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

        IDXGIAdapter2* dxgiAdapter2;
        res = dxgiAdapter->QueryInterface(&dxgiAdapter2);

        if(FAILED(res))
        {
            dxgiAdapter->Release();
            dxgiFactory->Release();
            return GAList();
        }

        accelerators[i] = build(dxgiAdapter2);
        dxgiAdapter2->Release();
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();
    return accelerators;
}

NullableRef<DXGI12GraphicsAccelerator> DXGI12GraphicsAccelerator::build(IDXGIAdapter2* const adapter) noexcept
{
    DXGI_ADAPTER_DESC2 dxgiAdapterDesc;
    const HRESULT res = adapter->GetDesc2(&dxgiAdapterDesc);

    if(FAILED(res))
    { return null; }

    const uSys videoCardMemory = dxgiAdapterDesc.DedicatedVideoMemory;
    const uSys systemVideoMemory = dxgiAdapterDesc.DedicatedSystemMemory;
    const uSys sharedVideoMemory = dxgiAdapterDesc.SharedSystemMemory;

    const WDynString description(dxgiAdapterDesc.Description);

    return NullableRef<DXGI12GraphicsAccelerator>(DefaultTauAllocator::Instance(), L"", description, videoCardMemory, systemVideoMemory, sharedVideoMemory, adapter);
}

IGraphicsAccelerator::GDList DXGI12GraphicsAccelerator::graphicsDisplays() noexcept
{
    IDXGIOutput* dxgiOutput;
    UINT numOutputs;
    for(numOutputs = 0; _dxgiAdapter->EnumOutputs(numOutputs, &dxgiOutput) != DXGI_ERROR_NOT_FOUND; ++numOutputs)
    { dxgiOutput->Release(); }

    RefDynArray<NullableRef<IGraphicsDisplay>> displays(numOutputs);

    for(UINT i = 0; i < numOutputs; ++i)
    {
        HRESULT res = _dxgiAdapter->EnumOutputs(i, &dxgiOutput);
        if(FAILED(res))
        { return GDList(); }

        IDXGIOutput1* dxgiOutput1;
        res = dxgiOutput->QueryInterface(&dxgiOutput1);
        if(FAILED(res))
        { return GDList(); }

        displays[i] = DXGI12GraphicsDisplay::build(dxgiOutput1);
        dxgiOutput1->Release();
        dxgiOutput->Release();
    }

    return displays;
}
#endif
