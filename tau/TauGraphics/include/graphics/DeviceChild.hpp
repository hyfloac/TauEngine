/**
* @file
 */
#pragma once

#include <Objects.hpp>
#include <TauCOM.hpp>
#include <com/TauObject.hpp>

namespace tau::graphics {

class IDeviceContextChild : public com::ITauObject
{
    DEFAULT_CONSTRUCT_PO(IDeviceContextChild);
    DEFAULT_DESTRUCT_VIO(IDeviceContextChild);
    DEFAULT_CM_PO(IDeviceContextChild);
public:
    virtual com::EResultCode GetDeviceContext(const com::UUID& uuid, void** pInterface) noexcept = 0;
};

}

TAU_DECL_UUID(::tau::graphics::IDeviceContextChild, 0xF7239FAAC7F74EFDull, 0xBB3A76F6A87AC7C1ull);
