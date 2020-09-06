#pragma once

#include "system/GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi.h>

class TAU_DLL DXGI11GraphicsAccelerator final : public IGraphicsAccelerator
{
    GRAPHICS_ACCELERATOR_IMPL(DXGI11GraphicsAccelerator);
public:
    using GAList = RefDynArray<NullableRef<IGraphicsAccelerator>>;

    [[nodiscard]] static GAList graphicsAccelerators() noexcept;
    [[nodiscard]] static NullableRef<DXGI11GraphicsAccelerator> build(IDXGIAdapter1* adapter) noexcept;
private:
    IDXGIAdapter1* _dxgiAdapter;
public:
    DXGI11GraphicsAccelerator(const WDynString& vendor, const WDynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory, IDXGIAdapter1* const dxgiAdapter)
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory)
        , _dxgiAdapter(dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    ~DXGI11GraphicsAccelerator() noexcept override
    {
        if(_dxgiAdapter) 
        { _dxgiAdapter->Release(); }
    }

    DXGI11GraphicsAccelerator(const DXGI11GraphicsAccelerator& copy) noexcept
        : IGraphicsAccelerator(copy)
        , _dxgiAdapter(copy._dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    DXGI11GraphicsAccelerator(DXGI11GraphicsAccelerator&& move) noexcept
        : IGraphicsAccelerator(::std::move(move))
        , _dxgiAdapter(move._dxgiAdapter)
    { move._dxgiAdapter = null; }

    DXGI11GraphicsAccelerator& operator=(const DXGI11GraphicsAccelerator& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IGraphicsAccelerator::operator=(copy);
        
        _dxgiAdapter->Release();

        _dxgiAdapter = copy._dxgiAdapter;
        _dxgiAdapter->AddRef();

        return *this;
    }

    DXGI11GraphicsAccelerator& operator=(DXGI11GraphicsAccelerator&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IGraphicsAccelerator::operator=(::std::move(move));

        _dxgiAdapter->Release();

        _dxgiAdapter = move._dxgiAdapter;
        move._dxgiAdapter = null;

        return *this;
    }

    [[nodiscard]] IDXGIAdapter1* dxgiAdapter() const noexcept { return _dxgiAdapter; }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return false; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] GDList graphicsDisplays() noexcept override;
};
#endif
