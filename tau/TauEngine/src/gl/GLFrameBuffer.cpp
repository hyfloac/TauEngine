#include "gl/GLFrameBuffer.hpp"
#include "system/RenderingContext.hpp"
#include "gl/GLRenderingContext.hpp"
#include "gl/GLTexture.hpp"

static GLenum getFrameBufferMode(IFrameBuffer::AccessMode mode) noexcept;

GLFrameBuffer::GLFrameBuffer(GLuint buffer, IFrameBufferAttachment* attachment) noexcept
    : IFrameBuffer(attachment), _buffer(buffer), _modeCache(0)
{ }

GLFrameBuffer::~GLFrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &_buffer);
}

void GLFrameBuffer::bind(IRenderingContext& context, AccessMode mode) noexcept
{
    const GLenum glMode = getFrameBufferMode(mode);
    _modeCache = glMode;
    glBindFramebuffer(glMode, _buffer);
}

void GLFrameBuffer::unbind(IRenderingContext& context) noexcept
{
    if(_modeCache)
    {
        glBindFramebuffer(_modeCache, 0);
        _modeCache = 0;
    }
}

static GLenum getFrameBufferMode(IFrameBuffer::AccessMode mode) noexcept
{
    switch(mode)
    {
        case IFrameBuffer::Read:      return GL_READ_FRAMEBUFFER;
        case IFrameBuffer::Write:     return GL_DRAW_FRAMEBUFFER;
        case IFrameBuffer::ReadWrite: return GL_FRAMEBUFFER;
        default: return 0;
    }
}

GLRenderTexture::GLRenderTexture(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
    : IRenderTexture(width, height, dataFormat)
{
    glGenRenderbuffers(1, &_rbo);
    _bind(_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GLTexture2D::glInternalFormat(dataFormat), width, height);
    _unbind();
}

void GLRenderTexture::_bind(GLuint rbo) noexcept
{
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void GLRenderTexture::_unbind() noexcept
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GLRenderTexture::bind(u8, EShader::Stage) noexcept
{
    _bind(_rbo);
}

void GLRenderTexture::unbind(u8, EShader::Stage) noexcept
{
    _unbind();
}

GLFrameBufferColorAttachment::GLFrameBufferColorAttachment(const u32 width, const u32 height, IRenderingContext& context) noexcept
    : IFrameBufferAttachment(width, height, CPPRef<ITexture>(null))
{
    TextureArgs args;
    args.width = width;
    args.height = height;
    args.dataFormat = ETexture::Format::RedGreenBlue8UnsignedInt;
    args.mipmapLevels = 0;

    _texture = context.createTexture2D().buildCPPRef(args, null);
}

GLFrameBufferDepthStencilAttachment::GLFrameBufferDepthStencilAttachment(const u32 width, const u32 height) noexcept
    : IFrameBufferAttachment(width, height, CPPRef<ITexture>(new GLRenderTexture(width, height, ETexture::Format::Depth24Stencil8)))
{ }

void GLFrameBufferColorAttachment::attach() noexcept
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RefCast<GLTexture2D>(_texture)->texture(), 0);
}

void GLFrameBufferDepthStencilAttachment::attach() noexcept
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RefCast<GLRenderTexture>(_texture)->rbo());
}

IFrameBuffer* GLFrameBufferBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(!_attachment, NoAttachment);
    ERROR_CODE_COND_N(_attachment->type() != IFrameBufferAttachment::Color, NoColorAttachment);

    GLuint bufferHandle;
    glGenFramebuffers(1, &bufferHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, bufferHandle);

    for(IFrameBufferAttachment* curr = _attachment; curr; curr = curr->next())
    {
        curr->attach();
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLFrameBuffer* frameBuffer = new(std::nothrow) GLFrameBuffer(bufferHandle, _attachment);

    if(!frameBuffer)
    {
        glDeleteFramebuffers(1, &bufferHandle);
        ERROR_CODE_N(MemoryAllocationFailure);
    }

    ERROR_CODE_V(NoError, frameBuffer);
}
