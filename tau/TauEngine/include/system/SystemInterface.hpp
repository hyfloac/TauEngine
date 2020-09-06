#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <RenderingMode.hpp>
#include "DLL.hpp"
#include "_SysContainer.hpp"

struct GraphicsInterfaceArgs;
class IGraphicsInterface;
class IGraphicsInterfaceBuilder;
class RenderingMode;
class IGraphicsAccelerator;

class TAU_DLL SystemInterface final
{
    DELETE_CONSTRUCT(SystemInterface);
    DELETE_DESTRUCT(SystemInterface);
    DELETE_CM(SystemInterface);
public:
    using GAList = RefDynArray<NullableRef<IGraphicsAccelerator>>;
private:
    static _SysContainer _sysContainer;

    static IGraphicsInterfaceBuilder* _giBuilders[RenderingMode::_MAX_VALUE];
public:
    [[nodiscard]] static const _SysContainer& sysContainer() noexcept;

    [[nodiscard]] static GAList graphicsAccelerators(const RenderingMode& renderingMode) noexcept;

    [[nodiscard]] static IGraphicsInterfaceBuilder* createGraphicsInterface(RenderingMode::Mode mode) noexcept;
    [[nodiscard]] static NullableRef<IGraphicsInterface> createGraphicsInterface(const GraphicsInterfaceArgs& args) noexcept;

    static void registerGraphicsInterface(RenderingMode::Mode mode, IGraphicsInterfaceBuilder* builder) noexcept;

    static void createAlert(const char* title, const char* message) noexcept;
};
