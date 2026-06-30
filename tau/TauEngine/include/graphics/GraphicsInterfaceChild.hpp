/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <TauCOM.hpp>
#include "com/TauObject.hpp"

namespace tau::graphics {

class IGraphicsInterfaceChild : public com::ITauObject
{
    DEFAULT_CONSTRUCT_PO(IGraphicsInterfaceChild);
    DEFAULT_DESTRUCT_VIO(IGraphicsInterfaceChild);
    DEFAULT_CM_PO(IGraphicsInterfaceChild);
public:
    virtual com::EResultCode GetGraphicsInterface(const com::UUID& uuid, void* pInterface) noexcept = 0;
};

}

TAU_DECL_UUID(::tau::graphics::IGraphicsInterfaceChild, 0xF7239FAAC7F74EFDull, 0xBB3A76F6A87AC7C1ull);
