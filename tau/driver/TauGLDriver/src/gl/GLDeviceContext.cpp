/**
 * @file
 */
#include "gl/GLDeviceContext.hpp"

#include <graphics/driver/GraphicsDriver.hpp>

#include "gl/GLCommandList.hpp"
#include "Logging.hpp"
#include <new>

namespace tau::graphics::driver::gl {

GLDeviceContext::GLDeviceContext(
    EngineDeviceHandle engine,
    const DeviceCallbacks& callbacks
) noexcept
    : m_Engine(engine)
    , m_Callbacks(callbacks)
{ }

com::EResultCode GLDeviceContext::CheckCapability(const CheckCapabilityArgs& checkCapabilityArgs) noexcept
{
    return com::RC_NotImplemented;
}

uSys GLDeviceContext::CalcPrivateCommandListSize(const CreateCommandListArgs& createCommandListArgs) noexcept
{
    (void) createCommandListArgs;

    return sizeof(GLCommandList);
}

com::EResultCode GLDeviceContext::CreateCommandList(CreateCommandListArgs& createCommandListArgs) noexcept
{
    auto* commandList = new(::std::nothrow) GLCommandList(
        createCommandListArgs.EngineCommandList,
        createCommandListArgs.Type
    );

    if(!commandList)
    {
        return com::RC_OutOfMemory;
    }

    createCommandListArgs.CommandList = commandList;

    return com::RC_Success;
}

void GLDeviceContext::DestroyCommandList(CommandListHandle commandList) noexcept
{
    if(!commandList.raw)
    {
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        return;
    }

    delete commandList.get<GLCommandList>();
}

com::EResultCode GLCheckCapability(DeviceHandle device, const CheckCapabilityArgs& checkCapabilityArgs)
{
    TRACE_ENTRYPOINT_ARG(
        u8"UUID: {XP0}{XP0}, pCapability: 0x{XP0}, Size: 0x{X}",
        checkCapabilityArgs.UUID.High, checkCapabilityArgs.UUID.Low,
        checkCapabilityArgs.pCapability,
        checkCapabilityArgs.Size
    );

    if(!device.raw)
    {
        LOG_ERROR(u8"device was not set.");
        return com::RC_NullParam;
    }

    if(device.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Device header did not match TaGL.");
        return com::RC_InvalidParam;
    }

    return device.get<GLDeviceContext>()->CheckCapability(checkCapabilityArgs);
}

uSys GLCalcPrivateCommandListSize(DeviceHandle device, const CreateCommandListArgs& createCommandListArgs)
{
    TRACE_ENTRYPOINT_ARG(
        u8"EngineCommandList: 0x{XP0}, Type: 0x{X}",
        createCommandListArgs.EngineCommandList.raw,
        static_cast<u32>(createCommandListArgs.Type)
    );

    if(!device.raw)
    {
        LOG_ERROR(u8"device was not set.");
        return com::RC_NullParam;
    }

    if(device.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Device header did not match TaGL.");
        return com::RC_InvalidParam;
    }

    return device.get<GLDeviceContext>()->CalcPrivateCommandListSize(createCommandListArgs);
}

com::EResultCode GLCreateCommandList(DeviceHandle device, CreateCommandListArgs& createCommandListArgs)
{
    TRACE_ENTRYPOINT_ARG(
        u8"EngineCommandList: 0x{XP0}, Type: 0x{X}",
        createCommandListArgs.EngineCommandList.raw,
        static_cast<u32>(createCommandListArgs.Type)
    );

    if(!device.raw)
    {
        LOG_ERROR(u8"device was not set.");
        return com::RC_NullParam;
    }

    if(device.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Device header did not match TaGL.");
        return com::RC_InvalidParam;
    }

    return device.get<GLDeviceContext>()->CreateCommandList(createCommandListArgs);
}

void GLDestroyCommandList(DeviceHandle device, CommandListHandle commandList)
{
    TRACE_ENTRYPOINT_ARG(
        u8"commandList: 0x{XP0}",
        commandList.raw
    );

    if(!device.raw)
    {
        LOG_ERROR(u8"device was not set.");
        return;
    }

    if(device.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Device header did not match TaGL.");
        return;
    }

    return device.get<GLDeviceContext>()->DestroyCommandList(commandList);
}
}
