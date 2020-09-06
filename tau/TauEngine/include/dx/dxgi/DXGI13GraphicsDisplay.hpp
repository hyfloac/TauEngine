#pragma once

#include "system/GraphicsDisplay.hpp"

#ifdef _WIN32
#include <dxgi1_3.h>
#include <Safeties.hpp>

class TAU_DLL DXGI13GraphicsDisplay final : public IGraphicsDisplay
{
    GRAPHICS_DISPLAY_IMPL(DXGI13GraphicsDisplay);
public:
    static NullableRef<DXGI13GraphicsDisplay> build(IDXGIOutput3* dxgiOutput) noexcept;
private:
    IDXGIOutput3* _dxgiOutput;
    RefDynArray<GraphicsDisplayMode> _displayModes;
public:
    DXGI13GraphicsDisplay(IDXGIOutput3* const dxgiOutput, const RefDynArray<GraphicsDisplayMode>& displayModes) noexcept
        : _dxgiOutput(dxgiOutput)
        , _displayModes(displayModes)
    { _dxgiOutput->AddRef(); }

    ~DXGI13GraphicsDisplay() noexcept override
    {
        if(!_dxgiOutput)
        { _dxgiOutput->Release(); }
    }

    DXGI13GraphicsDisplay(const DXGI13GraphicsDisplay& copy) noexcept
        : IGraphicsDisplay(copy)
        , _dxgiOutput(copy._dxgiOutput)
        , _displayModes(copy._displayModes)
    { _dxgiOutput->AddRef(); }

    DXGI13GraphicsDisplay(DXGI13GraphicsDisplay&& move) noexcept
        : IGraphicsDisplay(::std::move(move))
        , _dxgiOutput(move._dxgiOutput)
        , _displayModes(::std::move(move._displayModes))
    { move._dxgiOutput = null; }

    DXGI13GraphicsDisplay& operator=(const DXGI13GraphicsDisplay& copy) noexcept
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

    DXGI13GraphicsDisplay& operator=(DXGI13GraphicsDisplay&& move) noexcept
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

    [[nodiscard]] IDXGIOutput3* dxgiOutput() const noexcept { return _dxgiOutput; }

    [[nodiscard]] const RefDynArray<GraphicsDisplayMode>& displayModes() const noexcept override { return _displayModes; }
};
#endif
