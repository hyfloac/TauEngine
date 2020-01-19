#pragma once

#include "system/GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi.h>

class TAU_DLL DX10GraphicsAccelerator final : public IGraphicsAccelerator
{
    DELETE_COPY(DX10GraphicsAccelerator);
private:
    IDXGIAdapter* _dxgiAdapter;
public:
    DX10GraphicsAccelerator(DynString vendor, const DynString& deviceName,
                            u64 videoMemory, u64 systemMemory, u64 sharedMemory,
                            IDXGIAdapter* dxgiAdapter) noexcept
        : IGraphicsAccelerator(vendor, deviceName, 
                               videoMemory, systemMemory, sharedMemory),
          _dxgiAdapter(dxgiAdapter)
    {
        dxgiAdapter->AddRef();
    }

    ~DX10GraphicsAccelerator() noexcept
    {
        _dxgiAdapter->Release();
    }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return false; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] RefDynArray<Ref<IGraphicsDisplay>> graphicsDisplays() noexcept override;
};

class TAU_DLL DX10GraphicsAcceleratorBuilder final
{
    DEFAULT_CONSTRUCT_PU(DX10GraphicsAcceleratorBuilder);
    DEFAULT_DESTRUCT(DX10GraphicsAcceleratorBuilder);
    DEFAULT_COPY(DX10GraphicsAcceleratorBuilder);
private:
    IDXGIAdapter* _dxgiAdapter;
public:
    void setAdapter(IDXGIAdapter* dxgiAdapter) noexcept { _dxgiAdapter = dxgiAdapter; }

    [[nodiscard]] DX10GraphicsAccelerator* build() const noexcept;
};
#endif
