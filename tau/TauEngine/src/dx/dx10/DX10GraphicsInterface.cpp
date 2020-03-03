#include "dx/dx10/DX10GraphicsInterface.hpp"

#ifdef _WIN32
#include "dxgi.h"
#include "dx/dx10/DX10GraphicsAccelerator.hpp"

RefDynArray<CPPRef<IGraphicsAccelerator>> DX10GraphicsInterface::graphicsAccelerators() noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return RefDynArray<CPPRef<IGraphicsAccelerator>>(0); } } while(0)

    IDXGIFactory* dxgiFactory;
    CHECK(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)));

    IDXGIAdapter* dxgiAdapter;
    UINT i;
    for(i = 0; dxgiFactory->EnumAdapters(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        dxgiAdapter->Release();
    }

    RefDynArray<CPPRef<IGraphicsAccelerator>> accelerators(i);

    DX10GraphicsAcceleratorBuilder* builder = new(std::nothrow) DX10GraphicsAcceleratorBuilder;

    const UINT maxAdapter = i;
    for(i = 0; i < maxAdapter; ++i)
    {
        CHECK(dxgiFactory->EnumAdapters(i, &dxgiAdapter));
        builder->setAdapter(dxgiAdapter);
        accelerators[i] = CPPRef<IGraphicsAccelerator>(builder->build());
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();

    return accelerators;
}
#endif
