/**
 * @file
 */
#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <TauCOM.hpp>
#include "DeviceContext.hpp"

namespace tau::graphics::driver {

constexpr u32 MakeVersion(const u32 major, const u32 minor) noexcept
{
    return (major << 16) | minor;
}

DECL_OPAQUE_TYPE(EngineHandle);
DECL_OPAQUE_TYPE(DriverHandle);

struct EngineCallbacks final
{

};

enum class DriverTableType : u32
{
    DriverCore = 0,
    CommandList3D,
    CommandQueue3D
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

constexpr u32 Interface1_0 = MakeVersion(1, 0);
constexpr u32 InterfaceCurrent = Interface1_0;

struct OpenDriver final
{
    u32 Interface;
    EngineHandle Engine;
    DriverHandle Driver;
    const EngineCallbacks* Callbacks;
    DriverFuncs* Funcs;
};

extern "C" com::EResultCode OpenGraphicsDriver_f(OpenDriver& pOpenDriver);

}
