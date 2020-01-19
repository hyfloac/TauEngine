#include "dx/dx10/DX10GraphicsAccelerator.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsDisplay.hpp"

RefDynArray<Ref<IGraphicsDisplay>> DX10GraphicsAccelerator::graphicsDisplays() noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return RefDynArray<Ref<IGraphicsDisplay>>(0); } } while(0)

    IDXGIOutput* dxgiAdapterOutput;
    UINT i;
    for(i = 0; _dxgiAdapter->EnumOutputs(i, &dxgiAdapterOutput) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        dxgiAdapterOutput->Release();
    }

    RefDynArray<Ref<IGraphicsDisplay>> displays(i);

    DX10GraphicsDisplayBuilder* builder = new(std::nothrow) DX10GraphicsDisplayBuilder;

    const UINT maxAdapterOutput = i;
    for(i = 0; i < maxAdapterOutput; ++i)
    {
        CHECK(_dxgiAdapter->EnumOutputs(i, &dxgiAdapterOutput));
        builder->setAdapterOutput(dxgiAdapterOutput);
        displays[i] = Ref<IGraphicsDisplay>(builder->build());
        dxgiAdapterOutput->Release();
    }

    return displays;
#undef CHECK
}

DX10GraphicsAccelerator* DX10GraphicsAcceleratorBuilder::build() const noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return null; } } while(0)

    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    CHECK(_dxgiAdapter->GetDesc(&dxgiAdapterDesc));

    const uSys videoCardMemory = dxgiAdapterDesc.DedicatedVideoMemory;
    const uSys systemVideoMemory = dxgiAdapterDesc.DedicatedSystemMemory;
    const uSys sharedVideoMemory = dxgiAdapterDesc.SharedSystemMemory;

    uSys videoCardDescLen;
    errno_t error = wcstombs_s(&videoCardDescLen, null, 0, dxgiAdapterDesc.Description, 128);
    if(error || !videoCardDescLen)
    {
        return null;
    }

    char* videoCardDesc = new(std::nothrow) char[videoCardDescLen + 1];
    error = wcstombs_s(&videoCardDescLen, videoCardDesc, videoCardDescLen, dxgiAdapterDesc.Description, 128);
    if(error)
    {
        return null;
    }

    const DynString deviceName = DynString::passControl(videoCardDesc);

    DX10GraphicsAccelerator* gpu = new(std::nothrow) DX10GraphicsAccelerator("", deviceName, videoCardMemory, systemVideoMemory, sharedVideoMemory, _dxgiAdapter);

    return gpu;
#undef CHECK
}
#endif
