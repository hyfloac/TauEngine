/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <com/TauObject.hpp>
#include <EnumBitFields.hpp>
#include "driver/GraphicsDriver.hpp"

namespace tau::graphics {

enum class EGraphicsDriverFlags : u32
{
    None = 0,
    Persist = 1 << 0,
};

class IGraphicsManager : public com::ITauObject
{
    DEFAULT_CONSTRUCT_PO(IGraphicsManager);
    DEFAULT_DESTRUCT_VI(IGraphicsManager);
    DEFAULT_CM_PO(IGraphicsManager);
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_O(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
    public:
    };
public:
    virtual com::EResultCode RegisterDriver(driver::OpenGraphicsDriver_f* entrypoint, EGraphicsDriverFlags flags) noexcept = 0;

    virtual com::EResultCode RegisterDriver(const C8DynString& driverPath, EGraphicsDriverFlags flags) noexcept = 0;

    virtual com::EResultCode EnumerateDrivers(
        i32* index,
        C8DynString* driverName
    ) noexcept = 0;

    virtual com::EResultCode BuildComManagerForDriver(
        com::IComManager1** newComManager,
        com::IComManager* comManager,
        const C8DynString& driverName
    ) noexcept = 0;
};

}

TAU_DECL_UUID(::tau::graphics::IGraphicsManager, 0xE84B15D929D44FE4ull, 0x93F611780D8B39F2ull);

ENUM_FLAGS(::tau::graphics::EGraphicsDriverFlags);
