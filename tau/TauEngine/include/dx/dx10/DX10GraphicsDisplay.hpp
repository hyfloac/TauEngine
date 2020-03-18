#pragma once

#include "Safeties.hpp"
#include "system/GraphicsDisplay.hpp"

#ifdef _WIN32
#include <dxgi.h>

class TAU_DLL DX10GraphicsDisplay final : public IGraphicsDisplay
{
    DELETE_COPY(DX10GraphicsDisplay);
private:
    IDXGIOutput* _dxgiAdapterOutput;
    RefDynArray<GraphicsDisplayMode> _displayModes;
public:
    DX10GraphicsDisplay(IDXGIOutput* const dxgiAdapterOutput, const RefDynArray<GraphicsDisplayMode>& displayModes) noexcept
        : IGraphicsDisplay(), _dxgiAdapterOutput(dxgiAdapterOutput), _displayModes(displayModes)
    { dxgiAdapterOutput->AddRef(); }

    ~DX10GraphicsDisplay() noexcept
    { _dxgiAdapterOutput->Release(); }

    [[nodiscard]] RefDynArray<GraphicsDisplayMode> displayModes() noexcept override { return _displayModes; }
};

class TAU_DLL DX10GraphicsDisplayBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10GraphicsDisplayBuilder);
    DEFAULT_DESTRUCT(DX10GraphicsDisplayBuilder);
    DEFAULT_COPY(DX10GraphicsDisplayBuilder);
private:
    IDXGIOutput* _dxgiAdapterOutput;
public:
    void setAdapterOutput(IDXGIOutput* const dxgiAdapterOutput) noexcept { _dxgiAdapterOutput = dxgiAdapterOutput; }

    [[nodiscard]] NullableRef<DX10GraphicsDisplay> build() const noexcept;
};

#endif
