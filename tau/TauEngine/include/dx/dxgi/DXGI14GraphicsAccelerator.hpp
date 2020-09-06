#pragma once

#include "system/GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi1_4.h>

class TAU_DLL DXGI14GraphicsAccelerator final : public IGraphicsAccelerator
{
    GRAPHICS_ACCELERATOR_IMPL(DXGI14GraphicsAccelerator);
public:
    using GAList = RefDynArray<NullableRef<IGraphicsAccelerator>>;

    [[nodiscard]] static GAList graphicsAccelerators(bool debug) noexcept;
    [[nodiscard]] static NullableRef<DXGI14GraphicsAccelerator> build(IDXGIAdapter3* adapter) noexcept;
private:
    IDXGIAdapter3* _dxgiAdapter;
public:
    DXGI14GraphicsAccelerator(const WDynString& vendor, const WDynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory, IDXGIAdapter3* const dxgiAdapter)
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory)
        , _dxgiAdapter(dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    ~DXGI14GraphicsAccelerator() noexcept override
    {
        if(_dxgiAdapter) 
        { _dxgiAdapter->Release(); }
    }

    DXGI14GraphicsAccelerator(const DXGI14GraphicsAccelerator& copy) noexcept
        : IGraphicsAccelerator(copy)
        , _dxgiAdapter(copy._dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    DXGI14GraphicsAccelerator(DXGI14GraphicsAccelerator&& move) noexcept
        : IGraphicsAccelerator(::std::move(move))
        , _dxgiAdapter(move._dxgiAdapter)
    { move._dxgiAdapter = null; }

    DXGI14GraphicsAccelerator& operator=(const DXGI14GraphicsAccelerator& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IGraphicsAccelerator::operator=(copy);
        
        _dxgiAdapter->Release();

        _dxgiAdapter = copy._dxgiAdapter;
        _dxgiAdapter->AddRef();

        return *this;
    }

    DXGI14GraphicsAccelerator& operator=(DXGI14GraphicsAccelerator&& move) noexcept
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
