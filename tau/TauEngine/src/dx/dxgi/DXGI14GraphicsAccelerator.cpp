#include "dx/dxgi/DXGI14GraphicsAccelerator.hpp"

#ifdef _WIN32
#include "dx/dxgi/DXGI14GraphicsDisplay.hpp"

DXGI14GraphicsAccelerator::GAList DXGI14GraphicsAccelerator::graphicsAccelerators(const bool debug) noexcept
{
    const UINT flags = debug ? DXGI_CREATE_FACTORY_DEBUG : 0;

    IDXGIFactory4* dxgiFactory;
    HRESULT res = CreateDXGIFactory2(flags, IID_PPV_ARGS(&dxgiFactory));

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

        IDXGIAdapter3* dxgiAdapter3;
        res = dxgiAdapter->QueryInterface(&dxgiAdapter3);

        if(FAILED(res))
        {
            dxgiAdapter->Release();
            dxgiFactory->Release();
            return GAList();
        }

        accelerators[i] = build(dxgiAdapter3);
        dxgiAdapter3->Release();
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();
    return accelerators;
}

NullableRef<DXGI14GraphicsAccelerator> DXGI14GraphicsAccelerator::build(IDXGIAdapter3* const adapter) noexcept
{
    DXGI_ADAPTER_DESC2 dxgiAdapterDesc;
    const HRESULT res = adapter->GetDesc2(&dxgiAdapterDesc);

    if(FAILED(res))
    { return null; }

    const uSys videoCardMemory = dxgiAdapterDesc.DedicatedVideoMemory;
    const uSys systemVideoMemory = dxgiAdapterDesc.DedicatedSystemMemory;
    const uSys sharedVideoMemory = dxgiAdapterDesc.SharedSystemMemory;

    const WDynString description(dxgiAdapterDesc.Description);

    return NullableRef<DXGI14GraphicsAccelerator>(DefaultTauAllocator::Instance(), L"", description, videoCardMemory, systemVideoMemory, sharedVideoMemory, adapter);
}

IGraphicsAccelerator::GDList DXGI14GraphicsAccelerator::graphicsDisplays() noexcept
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

        IDXGIOutput4* dxgiOutput4;
        res = dxgiOutput->QueryInterface(&dxgiOutput4);
        if(FAILED(res))
        { return GDList(); }

        displays[i] = DXGI14GraphicsDisplay::build(dxgiOutput4);
        dxgiOutput4->Release();
        dxgiOutput->Release();
    }

    return displays;
}
#endif
