/**
 * @file
 */

#include "gl/GLDriver.hpp"

#include <graphics/driver/GraphicsDriver.hpp>

#include "Logging.hpp"

namespace tau::graphics::driver::gl {

GLDriver::GLDriver(
    const EngineHandle engine,
    const EngineCallbacks& engineCallbacks
) noexcept
    : m_Engine(engine)
    , m_EngineCallbacks(engineCallbacks)
{ }

com::EResultCode GLDriver::FillFuncTable(DriverTableType tableType, void* pTable, uSys tableSize) noexcept
{
    if(!pTable)
    {
        LOG_ERROR(u8"pTable was not set.");
        return com::RC_NullParam;
    }

    if(tableSize == 0)
    {
        LOG_ERROR(u8"tableSize was 0.");
        return com::RC_InvalidParam;
    }

    return com::RC_NotImplemented;
}

uSys GLDriver::CalcPrivateDeviceSize(const CreateDeviceArgs& calcPrivateDeviceSize) noexcept
{
    (void) calcPrivateDeviceSize;

    return sizeof(0);
}

com::EResultCode GLDriver::CreateDevice(CreateDeviceArgs& createDeviceArgs)
{
    return com::RC_NotImplemented;
}

void GLDriver::DestroyDevice(DeviceHandle device)
{

}

com::EResultCode GLFillFuncTable(
    DriverHandle driver,
    DriverTableType tableType,
    void* pTable,
    uSys tableSize
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"tableType: 0x{X}, pTable: 0x{XP0}, tableSize: 0x{X}",
        static_cast<u32>(tableType),
        pTable,
        tableSize
    );

    if(!driver.raw)
    {
        LOG_ERROR(u8"driver was not set.");
        return com::RC_NullParam;
    }

    if(driver.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Driver header did not match TaGL.");
        return com::RC_InvalidParam;
    }

    return driver.get<GLDriver>()->FillFuncTable(tableType, pTable, tableSize);
}

uSys GLCalcPrivateDeviceSize(
    DriverHandle driver,
    const CreateDeviceArgs& createDeviceArgs
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"EngineDevice: 0x{XP0}, Callbacks: 0x{XP0}, Flags: 0x{XP0}",
        createDeviceArgs.EngineDevice.raw,
        createDeviceArgs.Callbacks,
        createDeviceArgs.Flags
    );

    if(!driver.raw)
    {
        LOG_ERROR(u8"driver was not set.");
        return 0;
    }

    if(driver.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Driver header did not match TaGL.");
        return 0;
    }

    return driver.get<GLDriver>()->CalcPrivateDeviceSize(createDeviceArgs);
}

com::EResultCode GLCreateDevice(
    DriverHandle driver,
    CreateDeviceArgs& createDeviceArgs
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"EngineDevice: 0x{XP0}, Callbacks: 0x{XP0}, Flags: 0x{XP0}",
        createDeviceArgs.EngineDevice.raw,
        createDeviceArgs.Callbacks,
        createDeviceArgs.Flags
    );

    if(!driver.raw)
    {
        LOG_ERROR(u8"driver was not set.");
        return com::RC_NullParam;
    }

    if(driver.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Driver header did not match TaGL.");
        return com::RC_InvalidParam;
    }

    return driver.get<GLDriver>()->CreateDevice(createDeviceArgs);
}

void GLDestroyDevice(
    DriverHandle driver,
    const DeviceHandle device
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"device: 0x{XP0}",
        device.raw
    );

    if(!driver.raw)
    {
        LOG_ERROR(u8"driver was not set.");
        return;
    }

    if(driver.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Driver header did not match TaGL.");
        return;
    }

    driver.get<GLDriver>()->DestroyDevice(device);
}

void GLCloseDriver(DriverHandle driver)
{
    if(!driver.raw)
    {
        return;
    }

    if(driver.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Driver header did not match TaGL.");
        return;
    }

    delete driver.get<GLDriver>();
}

extern "C" tau::com::EResultCode OpenGraphicsDriver(tau::graphics::driver::OpenDriver& pOpenDriver)
{
    using namespace com;


    if(pOpenDriver.Interface < Interface1_0)
    {
        return RC_InvalidParam;
    }

    auto* driver = new(::std::nothrow) GLDriver(pOpenDriver.Engine, *pOpenDriver.Callbacks);

    pOpenDriver.Driver = driver;
    pOpenDriver.Funcs->FillFuncTable = GLFillFuncTable;
    pOpenDriver.Funcs->CalcPrivateDeviceSize = GLCalcPrivateDeviceSize;
    pOpenDriver.Funcs->CreateDevice = GLCreateDevice;
    pOpenDriver.Funcs->DestroyDevice = GLDestroyDevice;
    pOpenDriver.Funcs->CloseDriver = GLCloseDriver;

    return RC_Success;
}

}
