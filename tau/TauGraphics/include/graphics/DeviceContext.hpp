/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <com/TauObject.hpp>
#include "CommandList.hpp"

namespace tau::graphics {

class IDeviceContext : public com::ITauObject
{
    DEFAULT_CONSTRUCT_PO(IDeviceContext);
    DEFAULT_DESTRUCT_VI(IDeviceContext);
    DEFAULT_CM_PO(IDeviceContext);
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_O(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
    public:
        C8DynString RenderingMode;
        bool DebugMode;
        // com::ComRef<IGraphicsAccelerator> GraphicsAccelerator;
    };
public:
    [[nodiscard]] virtual const C8DynString& RenderingMode() const noexcept = 0;

    virtual com::EResultCode CheckCapability(
        const com::UUID& uuid,
        void* pCapability,
        u32 size
    ) noexcept = 0;

    virtual com::EResultCode CreateCommandList(
        ICommandList** pCommandList,
        const ICommandList::ConstructionInfo* constructionInfo
    ) = 0;
};

}

TAU_DECL_UUID(::tau::graphics::IDeviceContext, 0xC6BF4BAEE08D4C7Dull, 0xA5B09288EFAC6E6Cull);
