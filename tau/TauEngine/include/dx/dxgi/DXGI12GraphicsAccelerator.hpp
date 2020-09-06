#pragma once

#include "system/GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi1_2.h>

class TAU_DLL DXGI12GraphicsAccelerator final : public IGraphicsAccelerator
{
    GRAPHICS_ACCELERATOR_IMPL(DXGI12GraphicsAccelerator);
public:
    using GAList = RefDynArray<NullableRef<IGraphicsAccelerator>>;

    [[nodiscard]] static GAList graphicsAccelerators() noexcept;
    [[nodiscard]] static NullableRef<DXGI12GraphicsAccelerator> build(IDXGIAdapter2* adapter) noexcept;
private:
    IDXGIAdapter2* _dxgiAdapter;
public:
    DXGI12GraphicsAccelerator(const WDynString& vendor, const WDynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory, IDXGIAdapter2* const dxgiAdapter)
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory)
        , _dxgiAdapter(dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    ~DXGI12GraphicsAccelerator() noexcept override
    {
        if(_dxgiAdapter) 
        { _dxgiAdapter->Release(); }
    }

    DXGI12GraphicsAccelerator(const DXGI12GraphicsAccelerator& copy) noexcept
        : IGraphicsAccelerator(copy)
        , _dxgiAdapter(copy._dxgiAdapter)
    { _dxgiAdapter->AddRef(); }

    DXGI12GraphicsAccelerator(DXGI12GraphicsAccelerator&& move) noexcept
        : IGraphicsAccelerator(::std::move(move))
        , _dxgiAdapter(move._dxgiAdapter)
    { move._dxgiAdapter = null; }

    DXGI12GraphicsAccelerator& operator=(const DXGI12GraphicsAccelerator& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IGraphicsAccelerator::operator=(copy);
        
        _dxgiAdapter->Release();

        _dxgiAdapter = copy._dxgiAdapter;
        _dxgiAdapter->AddRef();

        return *this;
    }

    DXGI12GraphicsAccelerator& operator=(DXGI12GraphicsAccelerator&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IGraphicsAccelerator::operator=(::std::move(move));

        _dxgiAdapter->Release();

        _dxgiAdapter = move._dxgiAdapter;
        move._dxgiAdapter = null;

        return *this;
    }

    [[nodiscard]] IDXGIAdapter2* dxgiAdapter() const noexcept { return _dxgiAdapter; }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return false; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] GDList graphicsDisplays() noexcept override;
};
#endif
