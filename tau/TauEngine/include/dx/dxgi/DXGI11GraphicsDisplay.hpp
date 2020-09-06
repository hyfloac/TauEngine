#pragma once

#include "system/GraphicsDisplay.hpp"

#ifdef _WIN32
#include <dxgi.h>
#include <Safeties.hpp>

class TAU_DLL DXGI11GraphicsDisplay final : public IGraphicsDisplay
{
    GRAPHICS_DISPLAY_IMPL(DXGI11GraphicsDisplay);
public:
    static NullableRef<DXGI11GraphicsDisplay> build(IDXGIOutput* dxgiOutput) noexcept;
private:
    IDXGIOutput* _dxgiOutput;
    RefDynArray<GraphicsDisplayMode> _displayModes;
public:
    DXGI11GraphicsDisplay(IDXGIOutput* const dxgiOutput, const RefDynArray<GraphicsDisplayMode>& displayModes) noexcept
        : _dxgiOutput(dxgiOutput)
        , _displayModes(displayModes)
    { _dxgiOutput->AddRef(); }

    ~DXGI11GraphicsDisplay() noexcept override
    {
        if(!_dxgiOutput)
        { _dxgiOutput->Release(); }
    }

    DXGI11GraphicsDisplay(const DXGI11GraphicsDisplay& copy) noexcept
        : IGraphicsDisplay(copy)
        , _dxgiOutput(copy._dxgiOutput)
        , _displayModes(copy._displayModes)
    { _dxgiOutput->AddRef(); }

    DXGI11GraphicsDisplay(DXGI11GraphicsDisplay&& move) noexcept
        : IGraphicsDisplay(::std::move(move))
        , _dxgiOutput(move._dxgiOutput)
        , _displayModes(::std::move(move._displayModes))
    { move._dxgiOutput = null; }

    DXGI11GraphicsDisplay& operator=(const DXGI11GraphicsDisplay& copy) noexcept
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

    DXGI11GraphicsDisplay& operator=(DXGI11GraphicsDisplay&& move) noexcept
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

    [[nodiscard]] IDXGIOutput* dxgiOutput() const noexcept { return _dxgiOutput; }

    [[nodiscard]] const RefDynArray<GraphicsDisplayMode>& displayModes() const noexcept override { return _displayModes; }
};
#endif
