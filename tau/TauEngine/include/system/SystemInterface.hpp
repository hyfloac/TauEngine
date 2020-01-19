#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>
#include "DLL.hpp"
#include "_SysContainer.hpp"
#include "system/GraphicsAccelerator.hpp"
#include "system/GraphicsInterface.hpp"
#include "RenderingMode.hpp"

class TAU_DLL SystemInterface final
{
    DELETE_COPY(SystemInterface);
public:
    static SystemInterface* create() noexcept;

    static SystemInterface* get() noexcept;
private:
    _SysContainer _sysContainer;
private:
    SystemInterface() noexcept;
public:
    [[nodiscard]] const _SysContainer& sysContainer() const noexcept { return _sysContainer; }

    [[nodiscard]] Ref<IGraphicsInterface> createGraphicsInterface(const RenderingMode& renderingMode) noexcept;
};
