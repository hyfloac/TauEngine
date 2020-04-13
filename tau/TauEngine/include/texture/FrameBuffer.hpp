#pragma once

#include "DLL.hpp"
#include "texture/Texture.hpp"
#include "Color.hpp"

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <DynArray.hpp>
#include <TUMaths.hpp>

#ifndef TAU_MAX_FRAME_BUFFER_ATTACHMENTS
  #define TAU_MAX_FRAME_BUFFER_ATTACHMENTS (static_cast<uSys>(8))
#endif

class IRenderingContext;

class TAU_NOVTABLE IFrameBuffer
{
    DEFAULT_DESTRUCT_VI(IFrameBuffer);
    DELETE_COPY(IFrameBuffer);
public:
    enum AccessMode
    {
        Read,
        Write,
        ReadWrite
    };
protected:
    RefDynArray<CPPRef<ITexture>> _colorAttachments;
    CPPRef<ITextureDepthStencil> _depthStencilAttachment;
protected:
    inline IFrameBuffer(const RefDynArray<CPPRef<ITexture>>& colorAttachments, const CPPRef<ITextureDepthStencil>& depthStencilAttachment) noexcept
        : _colorAttachments(colorAttachments), _depthStencilAttachment(depthStencilAttachment)
    { }
public:
    [[nodiscard]] inline const RefDynArray<CPPRef<ITexture>>& colorAttachments() const noexcept { return _colorAttachments; }
    [[nodiscard]] inline const CPPRef<ITextureDepthStencil> depthStencilAttachment() const noexcept { return _depthStencilAttachment; }

    virtual void bind(IRenderingContext& context, AccessMode mode = AccessMode::ReadWrite) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void clearFrameBuffer(IRenderingContext& context, bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept = 0;
};

struct FrameBufferArgs final
{
    DEFAULT_COPY(FrameBufferArgs);
    DEFAULT_DESTRUCT(FrameBufferArgs);
public:
    RefDynArray<CPPRef<ITexture>> colorAttachments;
    CPPRef<ITextureDepthStencil> depthStencilAttachment;
public:
    FrameBufferArgs(const uSys colorAttachmentCount = TAU_MAX_FRAME_BUFFER_ATTACHMENTS) noexcept
        : colorAttachments(minT(colorAttachmentCount, TAU_MAX_FRAME_BUFFER_ATTACHMENTS)),
          depthStencilAttachment(null)
    { }
};

class TAU_NOVTABLE IFrameBufferBuilder
{
    DEFAULT_CONSTRUCT_PO(IFrameBufferBuilder);
    DEFAULT_DESTRUCT_VI(IFrameBufferBuilder);
    DELETE_COPY(IFrameBufferBuilder);
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
    [[nodiscard]] virtual IFrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IFrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IFrameBuffer> buildCPPRef(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IFrameBuffer> buildTauRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IFrameBuffer> buildTauSRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
