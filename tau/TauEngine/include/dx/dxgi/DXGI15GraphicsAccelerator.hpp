#pragma once

#include "system/GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi1_5.h>

class TAU_DLL DXGI15GraphicsAccelerator final : public IGraphicsAccelerator
{
    GRAPHICS_ACCELERATOR_IMPL(DXGI15GraphicsAccelerator);
public:
    using GAList = RefDynArray<NullableRef<IGraphicsAccelerator>>;

    [[nodiscard]] static GAList graphicsAccelerators(bool debug) noexcept;
    [[nodiscard]] static NullableRef<DXGI15GraphicsAccelerator> build(IDXGIAdapter3* adapter) noexcept;
private:
    IDXGIAdapter3* _dxgiAdapter;
public:
    DXGI15GraphicsAccelerator(const WDynString& vendor, const WDynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory, IDXGIAdapter3* const dxgiAdapter)
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory)
        , _dxgiAdapter(dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    ~DXGI15GraphicsAccelerator() noexcept override
    {
        if(_dxgiAdapter) 
        { _dxgiAdapter->Release(); }
    }

    DXGI15GraphicsAccelerator(const DXGI15GraphicsAccelerator& copy) noexcept
        : IGraphicsAccelerator(copy)
        , _dxgiAdapter(copy._dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    DXGI15GraphicsAccelerator(DXGI15GraphicsAccelerator&& move) noexcept
        : IGraphicsAccelerator(::std::move(move))
        , _dxgiAdapter(move._dxgiAdapter)
    { move._dxgiAdapter = null; }

    DXGI15GraphicsAccelerator& operator=(const DXGI15GraphicsAccelerator& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IGraphicsAccelerator::operator=(copy);
        
        _dxgiAdapter->Release();

        _dxgiAdapter = copy._dxgiAdapter;
        _dxgiAdapter->AddRef();

        return *this;
    }

    DXGI15GraphicsAccelerator& operator=(DXGI15GraphicsAccelerator&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IGraphicsAccelerator::operator=(::std::move(move));

        _dxgiAdapter->Release();

        _dxgiAdapter = move._dxgiAdapter;
        move._dxgiAdapter = null;

        return *this;
    }

    [[nodiscard]] IDXGIAdapter3* dxgiAdapter() const noexcept { return _dxgiAdapter; }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return false; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] GDList graphicsDisplays() noexcept override;
};
#endif
