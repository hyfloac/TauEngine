#pragma once

#include <Objects.hpp>
#include "DLL.hpp"
#include "_SysContainer.hpp"
#include "system/GraphicsAccelerator.hpp"
#include "system/GraphicsInterface.hpp"
#include "RenderingMode.hpp"

class TAU_DLL SystemInterface final
{
    DEFAULT_DESTRUCT(SystemInterface);
    DELETE_COPY(SystemInterface);
private:
    static SystemInterface* _instance;
private:
    static SystemInterface* create() noexcept;
    static void finalize() noexcept;
public:
    static SystemInterface* get() noexcept;
private:
    _SysContainer _sysContainer;
private:
    SystemInterface() noexcept;
public:
    [[nodiscard]] const _SysContainer& sysContainer() const noexcept { return _sysContainer; }

    [[nodiscard]] NullableRef<IGraphicsInterface> createGraphicsInterface(const RenderingMode& renderingMode) const noexcept;
private:
    friend bool tauInit(void) noexcept;
    friend void tauFinalize(void) noexcept;
};
