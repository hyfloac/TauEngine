#pragma once

#include <Objects.hpp>

#include "DLL.hpp"
#include "system/GraphicsAccelerator.hpp"
#include "RenderingMode.hpp"
#include "com/TauObject.hpp"
#include "CommandList.hpp"

class IGraphicsCapabilities;

class IShaderBuilder;
class IShaderProgramBuilder;
class IResourceBuilder;
class IInputLayoutBuilder;
class IVertexArrayBuilder;
class IBufferBuilder;
class IDepthStencilStateBuilder;
class IRasterizerStateBuilder;
class IBlendingStateBuilder;
class ITextureSamplerBuilder;
class IFrameBufferBuilder;
class IDescriptorHeapBuilder;
class IDescriptorLayoutBuilder;
class ITextureViewBuilder;
class IRenderingContextBuilder;

namespace tau::graphics {

class IGraphicsInterface : public com::ITauObject
{
    DEFAULT_CONSTRUCT_PO(IGraphicsInterface);
    DEFAULT_DESTRUCT_VI(IGraphicsInterface);
    DEFAULT_CM_PO(IGraphicsInterface);
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_O(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
    public:
        C8DynString RenderingMode;
        bool DebugMode;
        com::ComRef<IGraphicsAccelerator> GraphicsAccelerator;
    };
public:
    [[nodiscard]] virtual const C8DynString& RenderingMode() const noexcept = 0;

    [[nodiscard]] virtual IGraphicsCapabilities& Capabilities() noexcept = 0;

    virtual com::EResultCode CreateCommandList(
        ICommandList** pCommandList,
        const ICommandList::ConstructionInfo* constructionInfo
    ) = 0;
};

}

TAU_DECL_UUID(::tau::graphics::IGraphicsInterface, 0xC6BF4BAEE08D4C7Dull, 0xA5B09288EFAC6E6Cull);

struct GraphicsInterfaceArgs final
{
    RenderingMode renderingMode;
    NullableRef<tau::IGraphicsAccelerator> graphicsAccelerator;
};

class TAU_DLL TAU_NOVTABLE IGraphicsInterfaceBuilder
{
    DEFAULT_CONSTRUCT_PO(IGraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT_VI(IGraphicsInterfaceBuilder);
    DEFAULT_CM_PO(IGraphicsInterfaceBuilder);
public:
    enum Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        /**
         * Indicates that a chosen rendering API is not supported.
         *
         *   This can be caused by using an unimplemented API, using an
         * API that does not support the current system, or by using an
         * API with a graphics accelerator that does not carry support
         * for that API.
         */
        UnsupportedAPI
    };
public:
    [[nodiscard]] virtual tau::IGraphicsInterface* build(const GraphicsInterfaceArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual tau::IGraphicsInterface* build(const GraphicsInterfaceArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<tau::IGraphicsInterface> buildTauRef(const GraphicsInterfaceArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
