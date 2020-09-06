#include "dx/dxgi/DXGI11GraphicsDisplay.hpp"

#ifdef _WIN32
NullableRef<DXGI11GraphicsDisplay> DXGI11GraphicsDisplay::build(IDXGIOutput* const dxgiOutput) noexcept
{
    UINT numDisplayModes;
    HRESULT res = dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);

    if(FAILED(res))
    { return null; }

    DynArray<DXGI_MODE_DESC> dxgiDisplayModes(numDisplayModes);

    res = dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, dxgiDisplayModes);

    RefDynArray<IGraphicsDisplay::GraphicsDisplayMode> displayModes(numDisplayModes);

    if(FAILED(res))
    { return null; }

    for(uSys i = 0; i < numDisplayModes; ++i)
    {
        const auto& mode = dxgiDisplayModes[i];
        displayModes[i] = { mode.Width, mode.Height, mode.RefreshRate.Numerator, mode.RefreshRate.Denominator };
    }

    return NullableRef<DXGI11GraphicsDisplay>(DefaultTauAllocator::Instance(), dxgiOutput, displayModes);
}
#endif
