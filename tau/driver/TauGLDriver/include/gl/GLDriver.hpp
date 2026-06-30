/**
 * @file
 */
#pragma once

#include <graphics/driver/GraphicsDriver.hpp>

#include "GLHeader.hpp"

namespace tau::graphics::driver::gl {

class GLDriver final
{
public:
    GLDriver(
        EngineHandle engine,
        const EngineCallbacks& engineCallbacks
    ) noexcept;

    com::EResultCode FillFuncTable(DriverTableType tableType, void* pTable, uSys tableSize) noexcept;
    uSys CalcPrivateDeviceSize(const CreateDeviceArgs& createDeviceArgs) noexcept;
    com::EResultCode CreateDevice(CreateDeviceArgs& createDeviceArgs);
    void DestroyDevice(DeviceHandle device);
private:
    GLDriverHeader m_Magic;
    EngineHandle m_Engine;
    EngineCallbacks m_EngineCallbacks;
};

}
