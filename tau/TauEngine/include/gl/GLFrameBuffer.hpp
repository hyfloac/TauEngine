#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "texture/FrameBuffer.hpp"

class TAU_DLL GLFrameBuffer final : public IFrameBuffer
{
    DELETE_COPY(GLFrameBuffer);
private:
    GLuint _buffer;
    GLenum _modeCache;
public:
    GLFrameBuffer(GLuint buffer, const RefDynArray<CPPRef<ITexture>>& colorAttachments, const CPPRef<ITextureDepthStencil>& depthStencilAttachment) noexcept
        : IFrameBuffer(colorAttachments, depthStencilAttachment), _buffer(buffer), _modeCache(0)
    { }

    ~GLFrameBuffer() noexcept override final;

    void bind(IRenderingContext& context, AccessMode mode = AccessMode::ReadWrite) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void clearFrameBuffer(IRenderingContext& context, bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept override;
};

class TAU_DLL GLFrameBufferBuilder final : public IFrameBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLFrameBufferBuilder);
    DEFAULT_DESTRUCT(GLFrameBufferBuilder);
    DELETE_COPY(GLFrameBufferBuilder);
public:
    struct GLFrameBufferArgs final
    {
        GLuint fbo;
    };
public:
    [[nodiscard]] GLFrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLFrameBuffer* build(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IFrameBuffer> buildCPPRef(const FrameBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IFrameBuffer> buildTauRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IFrameBuffer> buildTauSRef(const FrameBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const FrameBufferArgs& args, GLFrameBufferArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
