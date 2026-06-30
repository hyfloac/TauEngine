/**
 * @file
 */
#include "graphics/DeviceContext.hpp"
#include <TauCOM.impl.hpp>
#include <TauPrivateData.hpp>

namespace tau::graphics {

class DeviceContext final : public IDeviceContext
{
    DEFAULT_DESTRUCT_VIO(DeviceContext);
    DELETE_CM(DeviceContext);
    TAU_COM_IMPL_REF_COUNT();
public:
    // IUnknown
    com::EResultCode QueryInterface(const com::UUID& iid, void** pInterface) noexcept override;

    // ITauObject
    com::EResultCode GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept override;
    com::EResultCode SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept override;
    com::EResultCode SetPrivateDataInterface(const com::UUID& uuid, IUnknown* pInterface) noexcept override;
    com::EResultCode SetName(const C8DynString& name) noexcept override;
    C8DynString GetName() noexcept override;

    // IDeviceContext
    [[nodiscard]] const C8DynString& RenderingMode() const noexcept override;

    com::EResultCode CheckCapability(
        const com::UUID& uuid,
        void* pCapability,
        u32 size
    ) noexcept override;

    com::EResultCode CreateCommandList(
        ICommandList** pCommandList,
        const ICommandList::ConstructionInfo* constructionInfo
    ) override;
private:
    PrivateData m_PrivateData;
    C8DynString m_RenderingMode;
};

com::EResultCode DeviceContext::QueryInterface(const com::UUID& iid, void** const pInterface) noexcept
{
    using namespace com;

    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid == iid_of<IUnknown> || iid == iid_of<ITauObject> || iid == iid_of<IDeviceContext>)
    {
        *pInterface = static_cast<IDeviceContext*>(this);
    }
    else
    {
        return RC_InterfaceNotFound;
    }

    AddReference();
    return RC_Success;
}

com::EResultCode DeviceContext::GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept
{
    return m_PrivateData.GetPrivateData(uuid, pDataSize, pData);
}

com::EResultCode DeviceContext::SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept
{
    m_PrivateData.SetPrivateData(uuid, dataSize, pData);
    return com::RC_Success;
}

com::EResultCode DeviceContext::SetPrivateDataInterface(const com::UUID& uuid, IUnknown* pInterface) noexcept
{
    m_PrivateData.SetPrivateDataInterface(uuid, pInterface);
    return com::RC_Success;
}

com::EResultCode DeviceContext::SetName(const C8DynString& name) noexcept
{
    m_PrivateData.SetName(name);
    return com::RC_Success;
}

C8DynString DeviceContext::GetName() noexcept
{
    return m_PrivateData.GetName();
}

const C8DynString& DeviceContext::RenderingMode() const noexcept
{
    return m_RenderingMode;
}

com::EResultCode DeviceContext::CheckCapability(const com::UUID& uuid, void* pCapability, u32 size) noexcept
{
    return com::RC_NotReady;
}

com::EResultCode DeviceContext::CreateCommandList(
    ICommandList** pCommandList,
    const ICommandList::ConstructionInfo* constructionInfo
)
{
    return com::RC_NotReady;
}

}
