#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <RenderingMode.hpp>
#include "DLL.hpp"
#include "_SysContainer.hpp"
#include <String.hpp>
#include <unordered_map>
#include <MapIterator.hpp>

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

    using GIMap = ::std::unordered_map<DynString, IGraphicsInterfaceBuilder*>;
    using GIMapIterator = IteratorBundle<GIMap::const_iterator>;
    using GIMapKeyIterator = IteratorBundle<HashMapConstKeyIterator<DynString, IGraphicsInterfaceBuilder*>>;
private:
    static _SysContainer _sysContainer;

    static IGraphicsInterfaceBuilder* _giBuilders0[RenderingMode::MAX_VALUE];

    static GIMap _giBuilders;
public:
    [[nodiscard]] static const _SysContainer& sysContainer() noexcept;

    [[nodiscard]] static GAList graphicsAccelerators(const RenderingMode& renderingMode) noexcept;

    [[nodiscard]] static IGraphicsInterfaceBuilder* createGraphicsInterface(RenderingMode::Mode mode) noexcept;
    [[nodiscard]] static IGraphicsInterfaceBuilder* createGraphicsInterface(const DynString& modeName) noexcept;
    [[nodiscard]] static NullableRef<IGraphicsInterface> createGraphicsInterface(const GraphicsInterfaceArgs& args) noexcept;

    [[nodiscard]] static GIMapKeyIterator iterateGraphicsInterfaceTypes() noexcept;
    [[nodiscard]] static GIMapIterator iterateGraphicsInterfaces() noexcept;

    static void registerGraphicsInterface(RenderingMode::Mode mode, IGraphicsInterfaceBuilder* builder) noexcept;
    static void registerGraphicsInterface(const DynString& mode, IGraphicsInterfaceBuilder* builder) noexcept;

    static void createAlert(const char* title, const char* message) noexcept;
private:
    friend class GraphicsAPILoader;
};
