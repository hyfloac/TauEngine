/**
* @file
 */
#pragma once

#include <Objects.hpp>
#include <TauCOM.hpp>
#include <NumTypes.hpp>
#include <String.hpp>

namespace tau::com {

class ITauObject : public IUnknown
{
    DEFAULT_CONSTRUCT_PO(ITauObject);
    DEFAULT_DESTRUCT_VIO(ITauObject);
    DEFAULT_CM_PO(ITauObject);
public:
    virtual EResultCode GetPrivateData(const UUID& uuid, u32* pDataSize, void* pData) noexcept = 0;
    virtual EResultCode SetPrivateData(const UUID& uuid, u32 dataSize, const void* pData) noexcept = 0;
    virtual EResultCode SetPrivateDataInterface(const UUID& uuid, IUnknown* pInterface) noexcept = 0;
    virtual EResultCode SetName(const C8DynString& name) noexcept = 0;
    virtual C8DynString GetName() noexcept = 0;
};

static inline constexpr UUID UUID_TauObjectSetName(0xD7088DC280D848DBull, 0xA4F5036E08E23871ull);

}

TAU_DECL_UUID(::tau::com::ITauObject, 0x8D900004FFDC48A5ull, 0xAB67F4D4445ED956ull);
