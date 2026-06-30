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

private:
    GLDriverHeader m_Magic;
    EngineDeviceHandle m_Engine;
    DeviceCallbacks m_Callbacks;
};

}
