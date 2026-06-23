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
#include <TauCOM.hpp>

struct GraphicsInterfaceArgs;
class IGraphicsInterface;
class IGraphicsInterfaceBuilder;
class RenderingMode;
class IGraphicsAccelerator;

namespace tau {

class ISystemInterface : public tau::com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(ISystemInterface);
    DEFAULT_DESTRUCT_VIO(ISystemInterface);
    DEFAULT_CM_PO(ISystemInterface);
public:
    using EnumerateGA = com::EResultCode(*)(i32* index, IGraphicsAccelerator** graphicsAccelerator);
public:
    virtual com::EResultCode EnumerateGraphicsModes(
        i32* index,
        C8DynString* mode
    ) noexcept = 0;

    virtual com::EResultCode EnumerateGraphicsAccelerators(
        i32* index,
        const C8DynString& mode,
        IGraphicsAccelerator** graphicsAccelerator
    ) noexcept = 0;

    virtual com::EResultCode BuildComManagerForGraphicsAPI(
        com::IComManager1** newComManager,
        com::IComManager* comManager,
        const C8DynString& mode
    ) noexcept = 0;

    virtual com::EResultCode RegisterGraphicsInterface(
        const C8DynString& mode,
        com::IComManager::ComFactoryFunc graphicsInterfaceFactory,
        EnumerateGA graphicsAcceleratorEnumerator
    ) noexcept = 0;

    virtual com::EResultCode CreateAlert(const C8DynString& title, const C8DynString& message) noexcept = 0;
};

namespace internal {

void RegisterSystemInterface(com::IComManager* comManager) noexcept;

}

}

TAU_DECL_UUID(::tau::ISystemInterface, 0x6D63E045FE9A4775ull, 0xAD56BBE228E794ECull);

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
