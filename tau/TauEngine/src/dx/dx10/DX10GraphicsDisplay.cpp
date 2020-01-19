#include "dx/dx10/DX10GraphicsDisplay.hpp"

#ifdef _WIN32
DX10GraphicsDisplay* DX10GraphicsDisplayBuilder::build() const noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return null; } } while(0)

    // Get the number of modes that match the specified display mode for the adapter output
    u32 numDisplayModes;
    CHECK(_dxgiAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL));

    // A list to hold all possible display modes for the monitor/video card combination
    DynArray<DXGI_MODE_DESC> dxgiDisplayModeList(numDisplayModes);
    RefDynArray<IGraphicsDisplay::GraphicsDisplayMode> displayModeList(numDisplayModes);

    CHECK(_dxgiAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, dxgiDisplayModeList));

    for(uSys i = 0; i < numDisplayModes; ++i)
    {
        auto& mode = dxgiDisplayModeList[i];
        displayModeList[i] = { mode.Width, mode.Height, mode.RefreshRate.Numerator, mode.RefreshRate.Denominator };
    }

    DX10GraphicsDisplay* ret = new(::std::nothrow) DX10GraphicsDisplay(_dxgiAdapterOutput, displayModeList);

    return ret;
}
#endif
