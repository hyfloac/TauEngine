/**
 * @file
 */
#include "system/SystemInterface.hpp"

#if defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
#include <TauCOM.impl.hpp>
#include <EASTL/unordered_map.h>
#include "EASTLString.hpp"

namespace tau {
class LinuxSystemInterface : public ISystemInterface
{
    TAU_COM_IMPL_REF_COUNT();
    DEFAULT_CONSTRUCT_PU(LinuxSystemInterface);
    DEFAULT_DESTRUCT_VIO(LinuxSystemInterface);
    DELETE_CM(LinuxSystemInterface);
public:
    // IUnknown
    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override;

    // ISystemInterface
    com::EResultCode EnumerateGraphicsModes(
        i32* index,
        C8DynString* mode
    ) noexcept override;

    com::EResultCode EnumerateGraphicsAccelerators(
        i32* index,
        const C8DynString& mode,
        IGraphicsAccelerator** graphicsAccelerator
    ) noexcept override;

    com::EResultCode BuildComManagerForGraphicsAPI(
        com::IComManager1** newComManager,
        com::IComManager* comManager,
        const C8DynString& mode
    ) noexcept override;

    com::EResultCode RegisterGraphicsInterface(
        const C8DynString& mode,
        com::IComManager::ComFactoryFunc graphicsInterfaceFactory,
        EnumerateGA graphicsAcceleratorEnumerator
    ) noexcept override;

    com::EResultCode CreateAlert(const C8DynString& title, const C8DynString& message) noexcept override;
public:
    static com::EResultCode Factory(const com::UUID& iid, void** const pInterface, const com::BaseConstructionInfo* const pConstructionInfo) noexcept;
private:
    eastl::unordered_map<C8DynString, eastl::pair<com::IComManager::ComFactoryFunc, EnumerateGA>> m_GIFactories;

    decltype(m_GIFactories.begin()) m_GIIterCacheIter;
    decltype(m_GIFactories.end()) m_GIIterCacheEnd;
    i32 m_GIIterCacheIndex = -1;
};

com::EResultCode LinuxSystemInterface::QueryInterface(const com::UUID& iid, void** const pInterface) noexcept
{
    if(!pInterface)
    {
        return com::RC_NullParam;
    }

    if(iid == com::iid_of<com::IUnknown> || iid == com::iid_of<ISystemInterface>)
    {
        *pInterface = static_cast<ISystemInterface*>(this);
    }
    else
    {
        return com::RC_InterfaceNotFound;
    }

    AddReference();
    return com::RC_Success;
}

com::EResultCode LinuxSystemInterface::EnumerateGraphicsModes(
    i32* const index,
    C8DynString* const mode
) noexcept
{
    if(!index)
    {
        return com::RC_NullParam;
    }

    if(*index < 0)
    {
        *index = static_cast<i32>(m_GIFactories.size());
        return com::RC_Success;
    }

    if(!mode)
    {
        return com::RC_NullParam;
    }

    if(m_GIIterCacheIndex < 0)
    {
        m_GIIterCacheIter = m_GIFactories.begin();
        m_GIIterCacheEnd = m_GIFactories.end();
        m_GIIterCacheIndex = 0;
    }

    for(i32 i = m_GIIterCacheIndex; i < *index && m_GIIterCacheIter != m_GIIterCacheEnd; ++i)
    {
        ++m_GIIterCacheIter;
    }

    *mode = m_GIIterCacheIter->first;

    if(m_GIIterCacheIter == m_GIIterCacheEnd)
    {
        return com::RC_Success;
    }
    else
    {
        return com::RC_MoreItems;
    }
}

com::EResultCode LinuxSystemInterface::EnumerateGraphicsAccelerators(
    i32* const index,
    const C8DynString& mode,
    IGraphicsAccelerator** const graphicsAccelerator
) noexcept
{
    if(!index)
    {
        return com::RC_NullParam;
    }

    if(m_GIFactories.find(mode) == m_GIFactories.end())
    {
        return com::RC_InterfaceNotFound;
    }

    const EnumerateGA enumerator = m_GIFactories[mode].second;
    return enumerator(index, graphicsAccelerator);
}

com::EResultCode LinuxSystemInterface::BuildComManagerForGraphicsAPI(
    com::IComManager1** const newComManager,
    com::IComManager* const comManager,
    const C8DynString& mode
) noexcept
{
    using namespace com;

    if(!newComManager)
    {
        return RC_NullParam;
    }

    if(!comManager)
    {
        return RC_NullParam;
    }

    if(m_GIFactories.find(mode) == m_GIFactories.end())
    {
        return RC_InterfaceNotFound;
    }

    // Retrieve IComManager1 so that we can use Duplicate().
    // This always exists.
    ComRef<IComManager1> comManager1;
    EResultCode result = comManager->QueryInterface(comManager1.Load());
    if(IsFailure(result) || !comManager1)
    {
        return result;
    }

    if(!comManager1)
    {
        return RC_Fail;
    }

    // Duplicate the COM manager.
    ComRef<IComManager1> newComManager0;
    result = comManager1->Duplicate(newComManager0.Load());
    if(IsFailure(result))
    {
        return result;
    }

    if(!newComManager0)
    {
        return RC_Fail;
    }

    // Register the requested IGraphicsInterface factory.
    const auto factory = m_GIFactories[mode].first;

    result = newComManager0->RegisterIidFactory(iid_of<IGraphicsInterface>, factory);
    if(IsFailure(result))
    {
        return result;
    }

    // Add a ref because otherwise it will be deleted when ComRef goes out of scope.
    newComManager0.AddReference();
    *newComManager = newComManager0;

    return RC_Success;
}

com::EResultCode LinuxSystemInterface::RegisterGraphicsInterface(
    const C8DynString& mode,
    const com::IComManager::ComFactoryFunc graphicsInterfaceFactory,
        EnumerateGA graphicsAcceleratorEnumerator
) noexcept
{
    m_GIIterCacheIndex = -1;
    m_GIFactories[mode] = { graphicsInterfaceFactory, graphicsAcceleratorEnumerator };

    return com::RC_Success;
}

com::EResultCode LinuxSystemInterface::CreateAlert(const C8DynString& title, const C8DynString& message) noexcept
{
    return com::RC_Success;
}

com::EResultCode LinuxSystemInterface::Factory(
    const com::UUID& iid,
    void** const pInterface,
    const com::BaseConstructionInfo* const pConstructionInfo
) noexcept
{
    using namespace com;

    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid != iid_of<ISystemInterface>)
    {
        return RC_InterfaceNotFound;
    }

    *pInterface = BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<LinuxSystemInterface>();

    if(!*pInterface)
    {
        return RC_OutOfMemory;
    }

    return RC_Success;
}

namespace internal {

void RegisterSystemInterface(com::IComManager* comManager) noexcept
{
    comManager->RegisterIidFactory(com::iid_of<ISystemInterface>, LinuxSystemInterface::Factory);
}

}

}

#endif
