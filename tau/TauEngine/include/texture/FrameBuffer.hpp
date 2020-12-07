#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "graphics/RenderTarget.hpp"

#ifndef TAU_MAX_FRAME_BUFFER_ATTACHMENTS
  #define TAU_MAX_FRAME_BUFFER_ATTACHMENTS (static_cast<uSys>(8))
#endif

#define FRAME_BUFFER_IMPL(_TYPE) RTT_IMPL(_TYPE, IFrameBuffer)

class IRenderingContext;

class TAU_DLL TAU_NOVTABLE IFrameBuffer
{
    DEFAULT_DESTRUCT_VI(IFrameBuffer);
    DEFAULT_CM_PO(IFrameBuffer);
public:
    enum AccessMode
    {
        Read,
        Write,
        ReadWrite
    };
protected:
    RefDynArray<NullableRef<IRenderTargetView>> _colorAttachments;
    NullableRef<IDepthStencilView> _depthStencilAttachment;
protected:
    IFrameBuffer(const RefDynArray<NullableRef<IRenderTargetView>>& colorAttachments, const NullableRef<IDepthStencilView>& depthStencilAttachment) noexcept
        : _colorAttachments(colorAttachments)
        , _depthStencilAttachment(depthStencilAttachment)
    { }
public:
    [[nodiscard]] const RefDynArray<NullableRef<IRenderTargetView>>& colorAttachments() const noexcept { return _colorAttachments; }
    [[nodiscard]] const NullableRef<IDepthStencilView>& depthStencilAttachment() const noexcept { return _depthStencilAttachment; }
    
    RTT_BASE_IMPL(IFrameBuffer);
    RTT_BASE_CHECK(IFrameBuffer);
    RTT_BASE_CAST(IFrameBuffer);
};

class TAU_DLL BasicFrameBuffer final : public IFrameBuffer
{
    DEFAULT_DESTRUCT_VI(BasicFrameBuffer);
    DEFAULT_CM_PU(BasicFrameBuffer);
    FRAME_BUFFER_IMPL(BasicFrameBuffer);
public:
    BasicFrameBuffer(const RefDynArray<NullableRef<IRenderTargetView>>& colorAttachments, const NullableRef<IDepthStencilView>& depthStencilAttachment) noexcept
        : IFrameBuffer(colorAttachments, depthStencilAttachment)
    { }
};

struct FrameBufferArgs final
{
    DEFAULT_DESTRUCT(FrameBufferArgs);
    DEFAULT_CM_PU(FrameBufferArgs);
public:
    RefDynArray<NullableRef<IRenderTargetView>> colorAttachments;
    NullableRef<IDepthStencilView> depthStencilAttachment;
public:
    FrameBufferArgs(const uSys colorAttachmentCount = TAU_MAX_FRAME_BUFFER_ATTACHMENTS) noexcept
        : colorAttachments(minT(colorAttachmentCount, TAU_MAX_FRAME_BUFFER_ATTACHMENTS))
        , depthStencilAttachment(null)
    { }
};

class TAU_NOVTABLE IFrameBufferBuilder
{
    DEFAULT_CONSTRUCT_PO(IFrameBufferBuilder);
    DEFAULT_DESTRUCT_VI(IFrameBufferBuilder);
    DEFAULT_CM_PO(IFrameBufferBuilder);
public:
    enum Error
    {
        NoError = 0,
        NullAttachment,
        BufferSizeDoesNotMatch,
        /**
         *   When the depthStencilAttachment does not have the
         * Depth24Stencil8 format or the texture does not have
         * the type of DepthStencil.
         */
        DepthStencilNotDepthStencil,
        ColorCannotBeDepthStencil,
        CrossAPIFailure,
        InternalError,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure
    };
public:
    [[nodiscard]] virtual NullableRef<IFrameBuffer> buildTauRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
