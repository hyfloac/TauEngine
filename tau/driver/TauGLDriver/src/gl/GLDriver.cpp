/**
 * @file
 */
#include "gl/GLDriver.hpp"
#include "gl/GLDeviceContext.hpp"
#include "Logging.hpp"
#include "gl/GLCommandList.hpp"

namespace tau::graphics::driver::gl {

GLDriver::GLDriver(
    const EngineHandle engine,
    const EngineCallbacks& engineCallbacks
) noexcept
    : m_Engine(engine)
    , m_EngineCallbacks(engineCallbacks)
{ }

com::EResultCode GLDriver::FillFuncTable(const DriverTableType tableType, void* const pTable, const uSys tableSize) noexcept
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

    switch(tableType)
    {
        case DriverTableType::DriverCore:
        {
            if(tableSize != sizeof(DeviceFuncs))
            {
                LOG_ERROR(u8"tableSize was not equal to sizeof(DeviceFuncs) (0x{X} != 0x{X}).", tableSize, sizeof(DeviceFuncs));
                return com::RC_InvalidParam;
            }

            auto* const funcs = static_cast<DeviceFuncs*>(pTable);

            funcs->CheckCapability = GLCheckCapability;
            funcs->CalcPrivateCommandListSize = GLCalcPrivateCommandListSize;
            funcs->CreateCommandList = GLCreateCommandList;
            funcs->DestroyCommandList = GLDestroyCommandList;

            break;
        }
        case DriverTableType::CommandQueue3D:return com::RC_NotImplemented;
        case DriverTableType::CommandList3D:
        {
            if(tableSize != sizeof(CommandListFuncs))
            {
                LOG_ERROR(u8"tableSize was not equal to sizeof(CommandListFuncs) (0x{X} != 0x{X}).", tableSize, sizeof(CommandListFuncs));
                return com::RC_InvalidParam;
            }

            auto* const funcs = static_cast<CommandListFuncs*>(pTable);

            funcs->Begin = GLBeginCommandList;
            funcs->End = GLEndCommandList;
            funcs->Draw = GLDraw;
            funcs->DrawIndexed = GLDrawIndexed;
            funcs->DrawInstanced = GLDrawInstanced;
            funcs->DrawIndexedInstanced = GLDrawIndexedInstanced;
            funcs->Dispatch = GLDispatch;
            funcs->SetDrawType = GLSetDrawType;
            funcs->SetBlendFactor = GLSetBlendFactor;
            funcs->SetStencilRef = GLSetStencilRef;
            funcs->ExecuteCommandList = GLExecuteCommandList;

            break;
        }
    }

    return com::RC_Success;
}

uSys GLDriver::CalcPrivateDeviceSize(const CreateDeviceArgs& calcPrivateDeviceSize) noexcept
{
    (void) calcPrivateDeviceSize;

    return sizeof(GLDeviceContext);
}

com::EResultCode GLDriver::CreateDevice(CreateDeviceArgs& createDeviceArgs)
{
    using namespace com;

    if(!createDeviceArgs.Callbacks)
    {
        return RC_NullParam;
    }

    auto* deviceContext = new(::std::nothrow) GLDeviceContext(
        createDeviceArgs.EngineDevice,
        *createDeviceArgs.Callbacks
    );

    if(!deviceContext)
    {
        return RC_OutOfMemory;
    }

    createDeviceArgs.DriverDevice = deviceContext;

    return RC_Success;
}

void GLDriver::DestroyDevice(DeviceHandle device)
{
    if(!device.raw)
    {
        return;
    }

    if(device.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Device header did not match TaGL.");
        return;
    }

    delete device.get<GLDeviceContext>();
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

extern "C" tau::com::EResultCode OpenGraphicsDriver(OpenDriver& openDriver)
{
    using namespace com;

    if(openDriver.Interface < TAU_GRAPHICS_INTERFACE_VERSION_1_0)
    {
        return RC_InvalidParam;
    }

    if(!openDriver.Callbacks)
    {
        return RC_NullParam;
    }

    auto* driver = new(::std::nothrow) GLDriver(openDriver.Engine, *openDriver.Callbacks);

    if(!driver)
    {
        return RC_OutOfMemory;
    }

    openDriver.Interface = TAU_GRAPHICS_INTERFACE_VERSION_CURRENT;
    openDriver.Driver = driver;
    openDriver.Funcs->FillFuncTable = GLFillFuncTable;
    openDriver.Funcs->CalcPrivateDeviceSize = GLCalcPrivateDeviceSize;
    openDriver.Funcs->CreateDevice = GLCreateDevice;
    openDriver.Funcs->DestroyDevice = GLDestroyDevice;
    openDriver.Funcs->CloseDriver = GLCloseDriver;
    openDriver.DriverName = u8"OpenGL";

    return RC_Success;
}

}
