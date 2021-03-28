#include "dx/DXUtils.hpp"

#ifdef _WIN32

#include <dxgi.h>

// As a default we'll assume 128 byte alignment.
// The D3D documentation recommends this for UMA adapters, so, might as well.
static constexpr u64 DefaultAlignment = 128;

static constexpr UINT NVidiaVendorId = 0x10DE;

// My GPU
static constexpr UINT NVidiaRtx2070SuperDeviceId = 0x1E84;

static constexpr inline u64 getNVidiaRowPitchAlignment(const UINT deviceId) noexcept
{
    switch(deviceId)
    {
        case NVidiaRtx2070SuperDeviceId: return 128;
        default: return DefaultAlignment;
    }
}

static constexpr inline u64 getNVidiaDepthPitchAlignment(const UINT deviceId) noexcept
{
    switch(deviceId)
    {
        case NVidiaRtx2070SuperDeviceId: return 128;
        default: return DefaultAlignment;
    }
}

u64 DX10Utils::getRowPitchAlignment(IDXGIAdapter* adapter) noexcept
{
    DXGI_ADAPTER_DESC desc;
    adapter->GetDesc(&desc);

    switch(desc.VendorId)
    {
        case NVidiaVendorId: return getNVidiaRowPitchAlignment(desc.DeviceId);
        default: return DefaultAlignment;
    }
}

u64 DX10Utils::getDepthPitchAlignment(IDXGIAdapter* adapter) noexcept
{
    DXGI_ADAPTER_DESC desc;
    adapter->GetDesc(&desc);

    switch(desc.VendorId)
    {
        case NVidiaVendorId: return getNVidiaDepthPitchAlignment(desc.DeviceId);
        default: return DefaultAlignment;
    }
}
#endif
