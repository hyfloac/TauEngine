#pragma once

#include <Objects.hpp>
#include <String.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "system/GraphicsDisplay.hpp"

#define GRAPHICS_ACCELERATOR_IMPL(_TYPE) RTT_IMPL(_TYPE, IGraphicsAccelerator)

class TAU_DLL TAU_NOVTABLE IGraphicsAccelerator
{
    DEFAULT_DESTRUCT_VI(IGraphicsAccelerator);
    DEFAULT_CM_PO(IGraphicsAccelerator);
public:
    using GDList = RefDynArray<NullableRef<IGraphicsDisplay>>;
protected:
    WDynString _vendor;
    WDynString _deviceName;
    uSys _videoMemory;
    uSys _systemMemory;
    uSys _sharedMemory;
protected:
    IGraphicsAccelerator(const WDynString& vendor, const WDynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory) noexcept
        : _vendor(vendor)
        , _deviceName(deviceName)
        , _videoMemory(videoMemory)
        , _systemMemory(systemMemory)
        , _sharedMemory(sharedMemory)
    { }
public:
    [[nodiscard]] const WDynString& vendor() const noexcept { return _vendor; }
    [[nodiscard]] const WDynString& deviceName() const noexcept { return _vendor; }

    /**
     * The amount of video memory on the GPU.
     */
    [[nodiscard]] uSys videoMemory() const noexcept { return _videoMemory; }
    /**
     * The amount of system memory allocated to the GPU.
     */
    [[nodiscard]] uSys systemMemory() const noexcept { return _systemMemory; }
    /**
     * The amount of system memory allocated to the GPU that the CPU can access.
     */
    [[nodiscard]] uSys sharedMemory() const noexcept { return _sharedMemory; }

    [[nodiscard]] virtual bool hasTessellationShaders() noexcept = 0;
    [[nodiscard]] virtual bool hasGeometryShaders() noexcept = 0;
    [[nodiscard]] virtual bool has64BitFloat() noexcept = 0;
    [[nodiscard]] virtual bool has64BitInt() noexcept = 0;
    [[nodiscard]] virtual bool has16BitInt() noexcept = 0;

    [[nodiscard]] virtual GDList graphicsDisplays() noexcept = 0;

    RTT_BASE_CAST(IGraphicsAccelerator);
    RTT_BASE_CHECK(IGraphicsAccelerator);
    RTT_BASE_IMPL(IGraphicsAccelerator);
};
