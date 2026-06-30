/**
 * @file
 */
#pragma once

#include <graphics/driver/GraphicsDriver.hpp>

#include "GLHeader.hpp"

namespace tau::graphics::driver::gl {

class GLDeviceContext final
{
public:
    GLDeviceContext(
        EngineDeviceHandle engine,
        const DeviceCallbacks& callbacks
    ) noexcept;

    com::EResultCode CheckCapability(const CheckCapabilityArgs& checkCapabilityArgs) noexcept;

    uSys CalcPrivateCommandListSize(const CreateCommandListArgs& createCommandListArgs) noexcept;
    com::EResultCode CreateCommandList(CreateCommandListArgs& createCommandListArgs) noexcept;
    void DestroyCommandList(CommandListHandle commandList) noexcept;
private:
    GLDriverHeader m_Magic;
    EngineDeviceHandle m_Engine;
    DeviceCallbacks m_Callbacks;
};

com::EResultCode GLCheckCapability(DeviceHandle driver, const CheckCapabilityArgs& checkCapabilityArgs);

uSys GLCalcPrivateCommandListSize(DeviceHandle driver, const CreateCommandListArgs& createCommandListArgs);

com::EResultCode GLCreateCommandList(DeviceHandle driver, CreateCommandListArgs& createCommandListArgs);

void GLDestroyCommandList(DeviceHandle driver, CommandListHandle commandList);

}
