#pragma once

#include "system/GraphicsDisplay.hpp"

#ifdef _WIN32
#include <dxgi1_6.h>
#include <Safeties.hpp>

class TAU_DLL DXGI16GraphicsDisplay final : public IGraphicsDisplay
{
    GRAPHICS_DISPLAY_IMPL(DXGI16GraphicsDisplay);
public:
    static NullableRef<DXGI16GraphicsDisplay> build(IDXGIOutput6* dxgiOutput) noexcept;
private:
    IDXGIOutput6* _dxgiOutput;
    RefDynArray<GraphicsDisplayMode> _displayModes;
public:
    DXGI16GraphicsDisplay(IDXGIOutput6* const dxgiOutput, const RefDynArray<GraphicsDisplayMode>& displayModes) noexcept
        : _dxgiOutput(dxgiOutput)
        , _displayModes(displayModes)
    { _dxgiOutput->AddRef(); }

    ~DXGI16GraphicsDisplay() noexcept override
    {
        if(!_dxgiOutput)
        { _dxgiOutput->Release(); }
    }

    DXGI16GraphicsDisplay(const DXGI16GraphicsDisplay& copy) noexcept
        : IGraphicsDisplay(copy)
        , _dxgiOutput(copy._dxgiOutput)
        , _displayModes(copy._displayModes)
    { _dxgiOutput->AddRef(); }

    DXGI16GraphicsDisplay(DXGI16GraphicsDisplay&& move) noexcept
        : IGraphicsDisplay(::std::move(move))
        , _dxgiOutput(move._dxgiOutput)
        , _displayModes(::std::move(move._displayModes))
    { move._dxgiOutput = null; }

    DXGI16GraphicsDisplay& operator=(const DXGI16GraphicsDisplay& copy) noexcept
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

    DXGI16GraphicsDisplay& operator=(DXGI16GraphicsDisplay&& move) noexcept
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

    [[nodiscard]] IDXGIOutput6* dxgiOutput() const noexcept { return _dxgiOutput; }

    [[nodiscard]] const RefDynArray<GraphicsDisplayMode>& displayModes() const noexcept override { return _displayModes; }
};
#endif
