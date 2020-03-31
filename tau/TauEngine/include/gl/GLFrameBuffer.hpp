#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "texture/FrameBuffer.hpp"

class TAU_DLL GLRenderTexture final : public IRenderTexture
{
    DEFAULT_DESTRUCT(GLRenderTexture);
    DELETE_COPY(GLRenderTexture);
private:
    GLuint _rbo;
public:
    static void _bind(GLuint rbo) noexcept;
    static void _unbind() noexcept;
public:
    GLRenderTexture(u32 width, u32 height, ETexture::Format dataFormat) noexcept;

    [[nodiscard]] inline GLuint rbo() const noexcept { return _rbo; }

    void bind(IRenderingContext&, u8 textureUnit, EShader::Stage stage) noexcept override;

    void unbind(IRenderingContext&, u8 textureUnit, EShader::Stage stage) noexcept override;
};

class TAU_DLL GLFrameBufferColorAttachment final : public IFrameBufferAttachment
{
    DEFAULT_DESTRUCT(GLFrameBufferColorAttachment);
    DELETE_COPY(GLFrameBufferColorAttachment);
public:
    GLFrameBufferColorAttachment(u32 width, u32 height, IRenderingContext& context) noexcept;

    [[nodiscard]] Type type() const noexcept override final { return Type::Color; }

    void attach() noexcept override;
};

class TAU_DLL GLFrameBufferDepthStencilAttachment final : public IFrameBufferAttachment
{
    DEFAULT_DESTRUCT(GLFrameBufferDepthStencilAttachment);
    DELETE_COPY(GLFrameBufferDepthStencilAttachment);
public:
    GLFrameBufferDepthStencilAttachment(u32 width, u32 height) noexcept;
    
    [[nodiscard]] Type type() const noexcept override final { return Type::DepthStencil; }

    void attach() noexcept override;
};

class TAU_DLL GLFrameBuffer final : public IFrameBuffer
{
    DELETE_COPY(GLFrameBuffer);
private:
    GLuint _buffer;
    GLenum _modeCache;
private:
    GLFrameBuffer(GLuint buffer, IFrameBufferAttachment* attachment) noexcept;
public:
    ~GLFrameBuffer() noexcept override final;

    void bind(IRenderingContext& context, AccessMode mode = AccessMode::ReadWrite) noexcept override final;

    void unbind(IRenderingContext& context) noexcept override final;
private:
    friend class GLFrameBufferBuilder;
};

class TAU_DLL GLFrameBufferBuilder final : public IFrameBufferBuilder
{
    DELETE_COPY(GLFrameBufferBuilder);
public:
    GLFrameBufferBuilder() noexcept = default;

    inline ~GLFrameBufferBuilder() noexcept override final = default;

    [[nodiscard]] IFrameBuffer* build([[tau::out]] Error* error) const noexcept override final;
};
