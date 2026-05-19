#pragma once

#include <TauCOM.impl.hpp>
#include "graphics/ResourceHeap.hpp"

class DX10ResourceHeap final : public tau::IResourceHeap
{
    DEFAULT_CM_PU(DX10ResourceHeap);
    DEFAULT_DESTRUCT(DX10ResourceHeap);
    TAU_COM_IMPL_REF_COUNT();
public:
    DX10ResourceHeap(const tau::ResourceHeapArgs& heapArgs) noexcept
        : m_HeapArgs(heapArgs)
    { }

    // IUnknown
    tau::com::EResultCode QueryInterface(const tau::com::UUID& iid, void** const pInterface) noexcept override;

    // IResourceHeap
    [[nodiscard]] const tau::ResourceHeapArgs& HeapArgs() const noexcept override { return m_HeapArgs; }
public:
    static tau::com::EResultCode Factory(const tau::com::UUID& iid, void** const pInterface, const tau::com::BaseConstructionInfo* const pConstructionInfo) noexcept;
public:
    tau::ResourceHeapArgs m_HeapArgs;
};
