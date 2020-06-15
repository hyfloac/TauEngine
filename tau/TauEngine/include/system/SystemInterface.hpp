#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include "DLL.hpp"
#include "_SysContainer.hpp"

class IGraphicsInterface;
class RenderingMode;

class TAU_DLL SystemInterface final
{
    DELETE_CONSTRUCT(SystemInterface);
    DELETE_DESTRUCT(SystemInterface);
    DELETE_CM(SystemInterface);
private:
    static _SysContainer _sysContainer;
public:
    [[nodiscard]] static const _SysContainer& sysContainer() noexcept;

    [[nodiscard]] static NullableRef<IGraphicsInterface> createGraphicsInterface(const RenderingMode& renderingMode) noexcept;

    static void createAlert(const char* title, const char* message) noexcept;
};
