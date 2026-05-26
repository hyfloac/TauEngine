#include "debug/IDebugInterface.hpp"
#include <TauCOM.impl.hpp>

#ifdef SendMessage
#undef SendMessage
#endif

namespace tau {

using namespace com;

class DebugInterface final : public IDebugInterface
{
    DELETE_CM(DebugInterface);
    DEFAULT_DESTRUCT(DebugInterface);
    TAU_COM_IMPL_REF_COUNT();
public:
    DebugInterface(const ConstructionInfo& info) noexcept;

    // IUnknown
    EResultCode QueryInterface(const UUID& iid, void** const pInterface) noexcept override;

    // IDebugInterface
    void SendMessage(const DebugMessageSeverity severity, const DebugMessageType types, const BaseDebugMessage* pMessage) const noexcept override;

    [[nodiscard]] DebugMessageSeverity GetSeverityFilter() const noexcept override { return m_SeverityFilter; }
    void SetSeverityFilter(const DebugMessageSeverity filter) noexcept override { m_SeverityFilter = filter; }

    [[nodiscard]] DebugMessageType GetTypesFilter() const noexcept override { return m_TypesFilter; }
    void SetTypesFilter(const DebugMessageType filter) noexcept override { m_TypesFilter = filter; }

    [[nodiscard]] DebugCallback_f GetCallback() const noexcept override { return m_Callback; }
    void SetCallback(const DebugCallback_f callback) noexcept override { m_Callback = callback; }

    [[nodiscard]] void* GetUserData() const noexcept override { return m_UserData; }
    void SetUserData(void* pUserData) noexcept override { m_UserData = pUserData; }
public:
    static EResultCode Factory(const UUID& iid, void** const pInterface, const BaseConstructionInfo* const pConstructionInfo) noexcept;
private:
    DebugMessageSeverity m_SeverityFilter;
    DebugMessageType m_TypesFilter;
    DebugCallback_f m_Callback;
    void* m_UserData;
};

DebugInterface::DebugInterface(const ConstructionInfo& info) noexcept
    : m_SeverityFilter(info.SeverityFilter)
    , m_TypesFilter(info.TypesFilter)
    , m_Callback(info.Callback)
    , m_UserData(info.pUserData)
{ }

EResultCode DebugInterface::QueryInterface(const UUID& iid, void** const pInterface) noexcept
{
    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid == iid_of<IUnknown>)
    {
        *pInterface = static_cast<IUnknown*>(this);
        AddReference();
    }
    else if(iid == iid_of<IDebugInterface>)
    {
        *pInterface = this;
        AddReference();
    }
    else
    {
        return RC_InterfaceNotFound;
    }

    return RC_Success;
}

void DebugInterface::SendMessage(const DebugMessageSeverity severity, const DebugMessageType types, const BaseDebugMessage* pMessage) const noexcept
{
    if(!pMessage)
    {
        return;
    }

    if(severity < m_SeverityFilter)
    {
        return;
    }

    if((m_TypesFilter & types) != types)
    {
        return;
    }

    if(!m_Callback)
    {
        return;
    }

    m_Callback(severity, types, pMessage, m_UserData);
}

EResultCode DebugInterface::Factory(const UUID& iid, void** const pInterface, const BaseConstructionInfo* const pConstructionInfo) noexcept
{
    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(!pConstructionInfo)
    {
        return RC_NullParam;
    }

    if(iid != iid_of<IDebugInterface>)
    {
        return RC_InterfaceNotFound;
    }

    if(pConstructionInfo->Iid != iid_of<IDebugInterface>)
    {
        return RC_InterfaceNotFound;
    }

    const ConstructionInfo* constructionInfo = reinterpret_cast<const ConstructionInfo*>(pConstructionInfo);

    *pInterface = BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<DebugInterface>(*constructionInfo);

    return RC_Success;
}

namespace internal {

void RegisterDebugInterface(IComManager* const comManager) noexcept
{
    (void) comManager->RegisterIidFactory(iid_of<IDebugInterface>, DebugInterface::Factory);
}

}

}
