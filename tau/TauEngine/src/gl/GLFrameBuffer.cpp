#include "gl/GLFrameBuffer.hpp"
#include "system/RenderingContext.hpp"
#include "gl/GLRenderingContext.hpp"
#include "../../../TauEditor/include/Layer3D.hpp"
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

void GLRenderTexture::bind(u8) noexcept
{
    _bind(_rbo);
}

void GLRenderTexture::unbind(u8) noexcept
{
    _unbind();
}

GLFrameBufferColorAttachment::GLFrameBufferColorAttachment(u32 width, u32 height, IRenderingContext& context) noexcept
    : IFrameBufferAttachment(width, height, Ref<ITexture>(ITexture::create(context, width, height, ETexture::Format::RedGreenBlue8UnsignedInt)))
{
    _texture->setFilterMode(ETexture::Filter::Nearest, ETexture::Filter::Nearest);
    _texture->set(nullptr);
}

GLFrameBufferDepthAttachment::GLFrameBufferDepthAttachment(u32 width, u32 height, IRenderingContext& context) noexcept
    : IFrameBufferAttachment(width, height, Ref<ITexture>(ITexture::create(context, width, height, ETexture::Format::Depth32Float, ETexture::Type::Depth)))
{
    _texture->setFilterMode(ETexture::Filter::Nearest, ETexture::Filter::Nearest);
    _texture->setDepthComparison(true, ETexture::DepthCompareFunc::LessThanOrEqual);
    _texture->set(nullptr);
}

GLFrameBufferStencilAttachment::GLFrameBufferStencilAttachment(u32 width, u32 height, IRenderingContext& context) noexcept
    : IFrameBufferAttachment(width, height, Ref<ITexture>(ITexture::create(context, width, height, ETexture::Format::Stencil8UnsignedInt)))
{
    _texture->setFilterMode(ETexture::Filter::Nearest, ETexture::Filter::Nearest);
    _texture->set(nullptr);
}

GLFrameBufferDepthStencilAttachment::GLFrameBufferDepthStencilAttachment(u32 width, u32 height) noexcept
    : IFrameBufferAttachment(width, height, Ref<ITexture>(new GLRenderTexture(width, height, ETexture::Format::Depth24Stencil8)))
{ }


void GLFrameBufferColorAttachment::attach() noexcept
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RefCast<GLTexture2D>(_texture)->texture(), 0);
}

void GLFrameBufferDepthAttachment::attach() noexcept
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, RefCast<GLTexture2D>(_texture)->texture(), 0);
}

void GLFrameBufferStencilAttachment::attach() noexcept
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, RefCast<GLTexture2D>(_texture)->texture(), 0);
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
