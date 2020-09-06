#include "dx/dxgi/DXGI13GraphicsAccelerator.hpp"

#ifdef _WIN32
#include "dx/dxgi/DXGI13GraphicsDisplay.hpp"

DXGI13GraphicsAccelerator::GAList DXGI13GraphicsAccelerator::graphicsAccelerators(const bool debug) noexcept
{
    const UINT flags = debug ? DXGI_CREATE_FACTORY_DEBUG : 0;

    IDXGIFactory3* dxgiFactory;
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

NullableRef<DXGI13GraphicsAccelerator> DXGI13GraphicsAccelerator::build(IDXGIAdapter2* const adapter) noexcept
{
    DXGI_ADAPTER_DESC2 dxgiAdapterDesc;
    const HRESULT res = adapter->GetDesc2(&dxgiAdapterDesc);

    if(FAILED(res))
    { return null; }

    const uSys videoCardMemory = dxgiAdapterDesc.DedicatedVideoMemory;
    const uSys systemVideoMemory = dxgiAdapterDesc.DedicatedSystemMemory;
    const uSys sharedVideoMemory = dxgiAdapterDesc.SharedSystemMemory;

    const WDynString description(dxgiAdapterDesc.Description);

    return NullableRef<DXGI13GraphicsAccelerator>(DefaultTauAllocator::Instance(), L"", description, videoCardMemory, systemVideoMemory, sharedVideoMemory, adapter);
}

IGraphicsAccelerator::GDList DXGI13GraphicsAccelerator::graphicsDisplays() noexcept
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

        IDXGIOutput3* dxgiOutput3;
        res = dxgiOutput->QueryInterface(&dxgiOutput3);
        if(FAILED(res))
        { return GDList(); }

        displays[i] = DXGI13GraphicsDisplay::build(dxgiOutput3);
        dxgiOutput3->Release();
        dxgiOutput->Release();
    }

    return displays;
}
#endif
