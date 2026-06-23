#pragma once

#include <Objects.hpp>
#include <String.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>
#include <RunTimeType.hpp>
#include <TauCOM.hpp>

#include "DLL.hpp"
#include "system/GraphicsDisplay.hpp"

#define GRAPHICS_ACCELERATOR_IMPL(_TYPE) RTT_IMPL(_TYPE, IGraphicsAccelerator)

namespace tau {

class IGraphicsAccelerator : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IGraphicsAccelerator);
    DEFAULT_DESTRUCT_VIO(IGraphicsAccelerator);
    DEFAULT_CM_PO(IGraphicsAccelerator);
public:
    using GDList = RefDynArray<com::ComRef<IGraphicsDisplay>>;
public:
    [[nodiscard]] virtual const C8DynString& Vendor() const noexcept = 0;
    [[nodiscard]] virtual const C8DynString& DeviceName() const noexcept = 0;

    /**
     * The amount of video memory on the GPU.
     */
    [[nodiscard]] virtual uSys videoMemory() const noexcept = 0;
    /**
     * The amount of system memory allocated to the GPU.
     */
    [[nodiscard]] virtual uSys systemMemory() const noexcept = 0;
    /**
     * The amount of system memory allocated to the GPU that the CPU can access.
     */
    [[nodiscard]] virtual uSys sharedMemory() const noexcept = 0;

    [[nodiscard]] virtual bool hasTessellationShaders() noexcept = 0;
    [[nodiscard]] virtual bool hasGeometryShaders() noexcept = 0;
    [[nodiscard]] virtual bool has64BitFloat() noexcept = 0;
    [[nodiscard]] virtual bool has64BitInt() noexcept = 0;
    [[nodiscard]] virtual bool has16BitInt() noexcept = 0;

    [[nodiscard]] virtual GDList graphicsDisplays() noexcept = 0;
};

}
