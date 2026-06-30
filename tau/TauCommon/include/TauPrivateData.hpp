/**
 * @file
 */
#pragma once

#include <TauCOM.hpp>
#include <EASTL/unordered_map.h>
#include <String.hpp>
#include <allocator/TauAllocator.hpp>
#include <algorithm>
#include "com/TauObject.hpp"

namespace tau {

class PrivateData final
{
public:
    ~PrivateData() noexcept
    {
        for(const auto& iter : m_PrivateData)
        {
            DefaultTauAllocator::Instance().Deallocate(iter.second.second);
        }

        for(const auto& iter : m_PrivateInterfaces)
        {
            iter.second->ReleaseReference();
        }
    }

    com::EResultCode GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept
    {
        if(!pDataSize)
        {
            return com::RC_NullParam;
        }

        u32 dataSize = 0;
        const void* rawData = nullptr;

        if(m_PrivateData.find(uuid) != m_PrivateData.end())
        {
            const auto& data = m_PrivateData[uuid];

            dataSize = data.first;
            rawData = data.second;
            const u32 bufSize = *pDataSize;

            *pDataSize = data.first;
            if(pData)
            {
                (void) ::std::memcpy(pData, data.second, ::std::min(bufSize, data.first));
            }

            if(data.first != bufSize)
            {
                return com::RC_MoreItems;
            }

            return com::RC_Success;
        }
        else if(m_PrivateInterfaces.find(uuid) != m_PrivateInterfaces.end())
        {
            com::IUnknown* interface = m_PrivateInterfaces[uuid];

            dataSize = sizeof(interface);
            rawData = interface;

            if(interface && pData && *pDataSize >= dataSize)
            {
                interface->AddReference();
            }
        }

        if(rawData)
        {
            const u32 bufSize = *pDataSize;

            *pDataSize = dataSize;
            if(pData)
            {
                (void) ::std::memcpy(pData, rawData, ::std::min(bufSize, dataSize));
            }

            if(dataSize != bufSize)
            {
                return com::RC_MoreItems;
            }

            return com::RC_Success;
        }

        return com::RC_InterfaceNotFound;
    }

    void SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept
    {
        Remove(uuid);
        if(dataSize != 0 && pData)
        {
            auto* dataBlock = static_cast<u8*>(DefaultTauAllocator::Instance().Allocate(dataSize));
            (void) ::std::memcpy(dataBlock, pData, dataSize);
            m_PrivateData[uuid] = { dataSize, dataBlock };
        }
    }

    void SetPrivateDataInterface(const com::UUID& uuid, com::IUnknown* pInterface) noexcept
    {
        Remove(uuid);
        if(pInterface)
        {
            pInterface->AddReference();
            m_PrivateInterfaces[uuid] = pInterface;
        }

    }

    void SetName(const C8DynString& name) noexcept
    {
        SetPrivateData(com::UUID_TauObjectSetName, name.Length(), name.String());
    }

    C8DynString GetName() noexcept
    {
        if(m_PrivateData.find(com::UUID_TauObjectSetName) != m_PrivateData.end())
        {
            const auto& data = m_PrivateData[com::UUID_TauObjectSetName];

            return C8DynString(data.first, static_cast<const c8*>(data.second));
        }

        return C8DynString();
    }

    void Remove(const com::UUID& uuid) noexcept
    {
        if(m_PrivateData.find(uuid) != m_PrivateData.end())
        {
            DefaultTauAllocator::Instance().Deallocate(m_PrivateData[uuid].second);
            m_PrivateData.erase(uuid);
        }

        if(m_PrivateInterfaces.find(uuid) != m_PrivateInterfaces.end())
        {
            m_PrivateInterfaces[uuid]->ReleaseReference();
            m_PrivateData.erase(uuid);
        }
    }
private:
    eastl::unordered_map<com::UUID, eastl::pair<u32, void*>> m_PrivateData;
    eastl::unordered_map<com::UUID, com::IUnknown*> m_PrivateInterfaces;
};

}
