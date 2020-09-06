#include "dx/dxgi/DXGI15GraphicsAccelerator.hpp"

#ifdef _WIN32
#include "dx/dxgi/DXGI15GraphicsDisplay.hpp"

DXGI15GraphicsAccelerator::GAList DXGI15GraphicsAccelerator::graphicsAccelerators(const bool debug) noexcept
{
    const UINT flags = debug ? DXGI_CREATE_FACTORY_DEBUG : 0;

    IDXGIFactory5* dxgiFactory;
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

NullableRef<DXGI15GraphicsAccelerator> DXGI15GraphicsAccelerator::build(IDXGIAdapter3* const adapter) noexcept
{
    DXGI_ADAPTER_DESC2 dxgiAdapterDesc;
    const HRESULT res = adapter->GetDesc2(&dxgiAdapterDesc);

    if(FAILED(res))
    { return null; }

    const uSys videoCardMemory = dxgiAdapterDesc.DedicatedVideoMemory;
    const uSys systemVideoMemory = dxgiAdapterDesc.DedicatedSystemMemory;
    const uSys sharedVideoMemory = dxgiAdapterDesc.SharedSystemMemory;

    const WDynString description(dxgiAdapterDesc.Description);

    return NullableRef<DXGI15GraphicsAccelerator>(DefaultTauAllocator::Instance(), L"", description, videoCardMemory, systemVideoMemory, sharedVideoMemory, adapter);
}

IGraphicsAccelerator::GDList DXGI15GraphicsAccelerator::graphicsDisplays() noexcept
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

        IDXGIOutput5* dxgiOutput5;
        res = dxgiOutput->QueryInterface(&dxgiOutput5);
        if(FAILED(res))
        { return GDList(); }

        displays[i] = DXGI15GraphicsDisplay::build(dxgiOutput5);
        dxgiOutput5->Release();
        dxgiOutput->Release();
    }

    return displays;
}
#endif
