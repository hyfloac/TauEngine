#pragma once

#include "system/GraphicsDisplay.hpp"

#ifdef _WIN32
#include <dxgi1_2.h>
#include <Safeties.hpp>

class TAU_DLL DXGI12GraphicsDisplay final : public IGraphicsDisplay
{
    GRAPHICS_DISPLAY_IMPL(DXGI12GraphicsDisplay);
public:
    static NullableRef<DXGI12GraphicsDisplay> build(IDXGIOutput1* dxgiOutput) noexcept;
private:
    IDXGIOutput1* _dxgiOutput;
    RefDynArray<GraphicsDisplayMode> _displayModes;
public:
    DXGI12GraphicsDisplay(IDXGIOutput1* const dxgiOutput, const RefDynArray<GraphicsDisplayMode>& displayModes) noexcept
        : _dxgiOutput(dxgiOutput)
        , _displayModes(displayModes)
    { _dxgiOutput->AddRef(); }

    ~DXGI12GraphicsDisplay() noexcept override
    {
        if(!_dxgiOutput)
        { _dxgiOutput->Release(); }
    }

    DXGI12GraphicsDisplay(const DXGI12GraphicsDisplay& copy) noexcept
        : IGraphicsDisplay(copy)
        , _dxgiOutput(copy._dxgiOutput)
        , _displayModes(copy._displayModes)
    { _dxgiOutput->AddRef(); }

    DXGI12GraphicsDisplay(DXGI12GraphicsDisplay&& move) noexcept
        : IGraphicsDisplay(::std::move(move))
        , _dxgiOutput(move._dxgiOutput)
        , _displayModes(::std::move(move._displayModes))
    { move._dxgiOutput = null; }

    DXGI12GraphicsDisplay& operator=(const DXGI12GraphicsDisplay& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IGraphicsDisplay::operator=(copy);

        _dxgiOutput->Release();

        _dxgiOutput = copy._dxgiOutput;
        _displayModes = copy._displayModes;

        _dxgiOutput->AddRef();

        return *this;
    }

    DXGI12GraphicsDisplay& operator=(DXGI12GraphicsDisplay&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IGraphicsDisplay::operator=(::std::move(move));

        _dxgiOutput->Release();

        _dxgiOutput = move._dxgiOutput;
        _displayModes = ::std::move(move._displayModes);

        move._dxgiOutput = null;

        return *this;
    }

    [[nodiscard]] IDXGIOutput1* dxgiOutput() const noexcept { return _dxgiOutput; }

    [[nodiscard]] const RefDynArray<GraphicsDisplayMode>& displayModes() const noexcept override { return _displayModes; }
};
#endif
