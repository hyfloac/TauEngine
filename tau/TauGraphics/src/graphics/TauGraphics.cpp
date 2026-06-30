/**
 * @file
 */
#include "graphics/TauGraphics.hpp"
#include "TauPrivateData.hpp"
#include <TauCOM.impl.hpp>
#include <EASTL/unordered_map.h>
#include <EASTLString.hpp>

namespace tau::graphics {

struct InternalDriver final
{
    driver::OpenGraphicsDriver_f* Entrypoint;
    EGraphicsDriverFlags Flags;
    driver::DriverHandle Driver;
    driver::DriverFuncs Funcs;
    b8 Open : 1;
};

struct ExternalDriver final
{
    C8DynString DriverPath;
    driver::OpenGraphicsDriver_f* Entrypoint;
    EGraphicsDriverFlags Flags;
    driver::DriverHandle Driver;
    driver::DriverFuncs Funcs;
    b8 Loaded : 1;
    b8 Open : 1;
};

class GraphicsManager final : public IGraphicsManager
{
    DEFAULT_DESTRUCT_VIO(GraphicsManager);
    DELETE_CM(GraphicsManager);
    TAU_COM_IMPL_REF_COUNT();
public:
    // IUnknown
    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override;

    // ITauObject
    com::EResultCode GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept override;
    com::EResultCode SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept override;
    com::EResultCode SetPrivateDataInterface(const com::UUID& uuid, IUnknown* pInterface) noexcept override;
    com::EResultCode SetName(const C8DynString& name) noexcept override;
    C8DynString GetName() noexcept override;

    // IGraphicsManager
    com::EResultCode RegisterDriver(driver::OpenGraphicsDriver_f* entrypoint, EGraphicsDriverFlags flags) noexcept override;
    com::EResultCode RegisterDriver(const C8DynString& driverPath, EGraphicsDriverFlags flags) noexcept override;
    com::EResultCode EnumerateDrivers(i32* index, C8DynString* driverName) noexcept override;
    com::EResultCode BuildComManagerForDriver(
        com::IComManager1** newComManager,
        com::IComManager* comManager,
        const C8DynString& driverName
    ) noexcept override;
private:
    PrivateData m_PrivateData;
    eastl::unordered_map<C8DynString, InternalDriver> m_InternalDrivers;
    eastl::unordered_map<C8DynString, ExternalDriver> m_ExternalDrivers;
};

com::EResultCode GraphicsManager::QueryInterface(const com::UUID& iid, void** const pInterface) noexcept
{
    using namespace com;

    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid == iid_of<IUnknown> || iid == iid_of<ITauObject> || iid == iid_of<IGraphicsManager>)
    {
        *pInterface = static_cast<IGraphicsManager*>(this);
    }
    else
    {
        return RC_InterfaceNotFound;
    }

    AddReference();
    return RC_Success;
}

com::EResultCode GraphicsManager::GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept
{
    return m_PrivateData.GetPrivateData(uuid, pDataSize, pData);
}

com::EResultCode GraphicsManager::SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept
{
    m_PrivateData.SetPrivateData(uuid, dataSize, pData);
    return com::RC_Success;
}

com::EResultCode GraphicsManager::SetPrivateDataInterface(const com::UUID& uuid, IUnknown* pInterface) noexcept
{
    m_PrivateData.SetPrivateDataInterface(uuid, pInterface);
    return com::RC_Success;
}

com::EResultCode GraphicsManager::SetName(const C8DynString& name) noexcept
{
    m_PrivateData.SetName(name);
    return com::RC_Success;
}

C8DynString GraphicsManager::GetName() noexcept
{
    return m_PrivateData.GetName();
}

com::EResultCode GraphicsManager::RegisterDriver(
    driver::OpenGraphicsDriver_f* const entrypoint,
    const EGraphicsDriverFlags flags
) noexcept
{
    using namespace com;

    if(!entrypoint)
    {
        return RC_NullParam;
    }

    // This flag technically is harmless since we can't unload the driver, but we'll be pedantic about it.
    if(HasFlag(flags, EGraphicsDriverFlags::Persist))
    {
        return RC_InvalidParam;
    }

    driver::EngineCallbacks engineCallbacks { };

    driver::DriverFuncs driverFuncs { };

    driver::OpenDriver openDriver { };
    openDriver.Interface = TAU_GRAPHICS_INTERFACE_VERSION_CURRENT;
    openDriver.Engine.raw = nullptr;
    openDriver.Driver.raw = nullptr;
    openDriver.Callbacks = &engineCallbacks;
    openDriver.Funcs = &driverFuncs;
    openDriver.DriverName = u8"";

    const EResultCode status = entrypoint(openDriver);

    if(IsFailure(status))
    {
        return RC_Fail;
    }

    driverFuncs.CloseDriver(openDriver.Driver);

    if(!openDriver.DriverName)
    {
        return RC_Fail;
    }

    m_InternalDrivers[openDriver.DriverName] = {
        .Entrypoint = entrypoint,
        .Flags = flags,
        .Driver = nullptr,
        .Funcs = { },
        .Open = false
    };

    return RC_Success;
}

com::EResultCode GraphicsManager::RegisterDriver(const C8DynString& driverPath, EGraphicsDriverFlags flags) noexcept
{
    using namespace com;

    if(!driverPath)
    {
        return RC_NullParam;
    }

    driver::OpenGraphicsDriver_f* entrypoint = nullptr;

    driver::EngineCallbacks engineCallbacks { };

    driver::DriverFuncs driverFuncs { };

    driver::OpenDriver openDriver { };
    openDriver.Interface = TAU_GRAPHICS_INTERFACE_VERSION_CURRENT;
    openDriver.Engine.raw = nullptr;
    openDriver.Driver.raw = nullptr;
    openDriver.Callbacks = &engineCallbacks;
    openDriver.Funcs = &driverFuncs;
    openDriver.DriverName = u8"";

    const EResultCode status = entrypoint(openDriver);

    if(IsFailure(status))
    {
        return RC_Fail;
    }

    if(!HasFlag(flags, EGraphicsDriverFlags::Persist))
    {
        driverFuncs.CloseDriver(openDriver.Driver);
    }

    if(!openDriver.DriverName)
    {
        return RC_Fail;
    }

    m_ExternalDrivers[openDriver.DriverName] = {
        .DriverPath = driverPath,
        .Entrypoint = entrypoint,
        .Flags = flags,
        .Driver = nullptr,
        .Funcs = { },
        .Open = false
    };

    return RC_Success;
}

com::EResultCode GraphicsManager::EnumerateDrivers(i32* index, C8DynString* driverName) noexcept
{
    using namespace com;

    return RC_NotImplemented;
}

com::EResultCode GraphicsManager::BuildComManagerForDriver(
    com::IComManager1** newComManager,
    com::IComManager* comManager,
    const C8DynString& driverName
) noexcept
{
    using namespace com;

    return RC_NotImplemented;
}

}
