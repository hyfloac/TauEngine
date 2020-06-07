#pragma once

#include "system/GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi.h>

class TAU_DLL DX10GraphicsAccelerator final : public IGraphicsAccelerator
{
    DELETE_CM(DX10GraphicsAccelerator);
private:
    IDXGIAdapter* _dxgiAdapter;
public:
    DX10GraphicsAccelerator(const DynString vendor, const DynString& deviceName,
                            const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory,
                            IDXGIAdapter* const dxgiAdapter) noexcept
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory),
          _dxgiAdapter(dxgiAdapter)
    { dxgiAdapter->AddRef(); }

    ~DX10GraphicsAccelerator() noexcept
    { _dxgiAdapter->Release(); }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return true; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsDisplay>> graphicsDisplays() noexcept override;
};

class TAU_DLL DX10GraphicsAcceleratorBuilder final
{
    DEFAULT_CONSTRUCT_PU(DX10GraphicsAcceleratorBuilder);
    DEFAULT_DESTRUCT(DX10GraphicsAcceleratorBuilder);
    DEFAULT_CM_PU(DX10GraphicsAcceleratorBuilder);
private:
    IDXGIAdapter* _dxgiAdapter;
public:
    void setAdapter(IDXGIAdapter* dxgiAdapter) noexcept { _dxgiAdapter = dxgiAdapter; }

    [[nodiscard]] NullableRef<DX10GraphicsAccelerator> build() const noexcept;
};
#endif
