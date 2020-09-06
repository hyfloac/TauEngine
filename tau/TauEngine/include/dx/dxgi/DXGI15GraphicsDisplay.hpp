#pragma once

#include "system/GraphicsDisplay.hpp"

#ifdef _WIN32
#include <dxgi1_5.h>
#include <Safeties.hpp>

class TAU_DLL DXGI15GraphicsDisplay final : public IGraphicsDisplay
{
    GRAPHICS_DISPLAY_IMPL(DXGI15GraphicsDisplay);
public:
    static NullableRef<DXGI15GraphicsDisplay> build(IDXGIOutput5* dxgiOutput) noexcept;
private:
    IDXGIOutput5* _dxgiOutput;
    RefDynArray<GraphicsDisplayMode> _displayModes;
public:
    DXGI15GraphicsDisplay(IDXGIOutput5* const dxgiOutput, const RefDynArray<GraphicsDisplayMode>& displayModes) noexcept
        : _dxgiOutput(dxgiOutput)
        , _displayModes(displayModes)
    { _dxgiOutput->AddRef(); }

    ~DXGI15GraphicsDisplay() noexcept override
    {
        if(!_dxgiOutput)
        { _dxgiOutput->Release(); }
    }

    DXGI15GraphicsDisplay(const DXGI15GraphicsDisplay& copy) noexcept
        : IGraphicsDisplay(copy)
        , _dxgiOutput(copy._dxgiOutput)
        , _displayModes(copy._displayModes)
    { _dxgiOutput->AddRef(); }

    DXGI15GraphicsDisplay(DXGI15GraphicsDisplay&& move) noexcept
        : IGraphicsDisplay(::std::move(move))
        , _dxgiOutput(move._dxgiOutput)
        , _displayModes(::std::move(move._displayModes))
    { move._dxgiOutput = null; }

    DXGI15GraphicsDisplay& operator=(const DXGI15GraphicsDisplay& copy) noexcept
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

    DXGI15GraphicsDisplay& operator=(DXGI15GraphicsDisplay&& move) noexcept
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

    [[nodiscard]] IDXGIOutput5* dxgiOutput() const noexcept { return _dxgiOutput; }

    [[nodiscard]] const RefDynArray<GraphicsDisplayMode>& displayModes() const noexcept override { return _displayModes; }
};
#endif
