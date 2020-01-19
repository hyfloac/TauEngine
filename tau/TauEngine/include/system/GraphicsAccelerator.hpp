#pragma once

#include "DLL.hpp"
#include "system/GraphicsDisplay.hpp"
#include <Objects.hpp>
#include <String.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>

class TAU_DLL NOVTABLE IGraphicsAccelerator
{
    DEFAULT_DESTRUCT_VI(IGraphicsAccelerator);
    DELETE_COPY(IGraphicsAccelerator);
protected:
    DynString _vendor;
    DynString _deviceName;
    uSys _videoMemory;
    uSys _systemMemory;
    uSys _sharedMemory;
protected:
    IGraphicsAccelerator(const DynString& vendor, const DynString& deviceName, 
                         u64 videoMemory, u64 systemMemory, u64 sharedMemory)
        : _vendor(vendor), _deviceName(deviceName),
          _videoMemory(videoMemory), _systemMemory(systemMemory), _sharedMemory(sharedMemory)
    { }
public:
    [[nodiscard]] const DynString& vendor() const noexcept { return _vendor; }
    [[nodiscard]] const DynString& deviceName() const noexcept { return _vendor; }

    /**
     * The amount of video memory on the GPU.
     */
    [[nodiscard]] uSys  videoMemory() const noexcept { return _videoMemory;  }
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

    [[nodiscard]] virtual RefDynArray<Ref<IGraphicsDisplay>> graphicsDisplays() noexcept = 0;
};
