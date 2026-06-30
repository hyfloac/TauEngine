/**
 * @file
 */
#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <String.hpp>
#include <TauCOM.hpp>
#include "DeviceContext.hpp"

namespace tau::graphics::driver {

DECL_OPAQUE_TYPE(EngineHandle);
DECL_OPAQUE_TYPE(DriverHandle);

struct EngineCallbacks final
{

};

enum class DriverTableType : u32
{
    DriverCore = 0,
    CommandQueue3D,
    CommandList3D
};

typedef com::EResultCode FillFuncTable_f(
    DriverHandle driver,
    DriverTableType tableType,
    void* pTable,
    uSys tableSize
);


typedef uSys CalcPrivateDeviceSize_f(DriverHandle driver, const CreateDeviceArgs& createDeviceArgs);

typedef com::EResultCode CreateDevice_f(DriverHandle driver, CreateDeviceArgs& createDeviceArgs);

typedef void DestroyDevice_f(DriverHandle driver, DeviceHandle device);

typedef void CloseDriver_f(DriverHandle driver);

struct DriverFuncs final
{
    FillFuncTable_f* FillFuncTable;
    CalcPrivateDeviceSize_f* CalcPrivateDeviceSize;
    CreateDevice_f* CreateDevice;
    DestroyDevice_f* DestroyDevice;
    CloseDriver_f* CloseDriver;
};

#define TAU_GRAPHICS_INTERFACE_VERSION_1_0 0x00010000
#ifndef TAU_GRAPHICS_INTERFACE_VERSION_CURRENT
    #define TAU_GRAPHICS_INTERFACE_VERSION_CURRENT TAU_GRAPHICS_INTERFACE_VERSION_1_0
#endif

struct OpenDriver final
{
    u32 Interface;
    EngineHandle Engine;
    DriverHandle Driver;
    const EngineCallbacks* Callbacks;
    DriverFuncs* Funcs;
    C8DynString DriverName;
};

typedef com::EResultCode OpenGraphicsDriver_f(OpenDriver& pOpenDriver);

}
