/**
 * @file
 */
#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <TauCOM.hpp>
#include "CommandList.hpp"

namespace tau::graphics::driver {

DECL_OPAQUE_TYPE(EngineDeviceHandle);
DECL_OPAQUE_TYPE(DeviceHandle);

struct DeviceCallbacks final
{

};

struct CheckCapabilityArgs final
{
    const com::UUID& UUID;
    void* pCapability;
    u32 Size;
};

typedef com::EResultCode CheckCapability_f(DeviceHandle driver, CheckCapabilityArgs& checkCapabilityArgs);

typedef uSys CalcPrivateCommandListSize_f(DeviceHandle driver, const CreateCommandListArgs& createCommandListArgs);

typedef com::EResultCode CreateCommandList_f(DeviceHandle driver, CreateCommandListArgs& createCommandListArgs);

typedef void DestroyCommandList_f(DeviceHandle driver, CommandListHandle commandList);

struct DeviceFuncs final
{
    CheckCapability_f* CheckCapability;

    CalcPrivateCommandListSize_f* CalcPrivateCommandListSize;
    CreateCommandList_f* CreateCommandList;
    DestroyCommandList_f* DestroyCommandList;
};

struct CreateDeviceArgs final
{
    EngineDeviceHandle EngineDevice;
    DeviceHandle DriverDevice;
    const DeviceCallbacks* Callbacks;
    u32 Flags;
};

}
