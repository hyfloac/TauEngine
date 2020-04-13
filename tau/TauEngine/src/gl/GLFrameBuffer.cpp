#include "gl/GLFrameBuffer.hpp"
#include "system/RenderingContext.hpp"
#include "gl/GLRenderingContext.hpp"
#include "gl/GLTexture.hpp"

static GLenum getFrameBufferMode(IFrameBuffer::AccessMode mode) noexcept;

GLFrameBuffer::~GLFrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &_buffer);
}

void GLFrameBuffer::bind(IRenderingContext&, const AccessMode mode) noexcept
{
    const GLenum glMode = getFrameBufferMode(mode);
    _modeCache = glMode;
    glBindFramebuffer(glMode, _buffer);
}

void GLFrameBuffer::unbind(IRenderingContext&) noexcept
{
    if(_modeCache)
    {
        glBindFramebuffer(_modeCache, 0);
        _modeCache = 0;
    }
}

void GLFrameBuffer::clearFrameBuffer(IRenderingContext& context, bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
{
    GLbitfield flags = 0;
    if(clearColorBuffer) { flags = GL_COLOR_BUFFER_BIT; }
    if(clearDepthBuffer) { flags |= GL_DEPTH_BUFFER_BIT; }
    if(clearStencilBuffer) { flags |= GL_STENCIL_BUFFER_BIT; }

    glClearDepth(depthValue);
    glClearStencil(stencilValue);
    glClearColor(static_cast<float>(color.r) / 255.0f,
                 static_cast<float>(color.g) / 255.0f,
                 static_cast<float>(color.b) / 255.0f,
                 static_cast<float>(color.a) / 255.0f);
    glClear(flags);
}

static GLenum getFrameBufferMode(const IFrameBuffer::AccessMode mode) noexcept
{
    switch(mode)
    {
        case IFrameBuffer::Read:      return GL_READ_FRAMEBUFFER;
        case IFrameBuffer::Write:     return GL_DRAW_FRAMEBUFFER;
        case IFrameBuffer::ReadWrite: return GL_FRAMEBUFFER;
        default: return 0;
    }
}

GLFrameBuffer* GLFrameBufferBuilder::build(const FrameBufferArgs& args, Error* error) const noexcept
{
    GLFrameBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLFrameBuffer* const frameBuffer = new(::std::nothrow) GLFrameBuffer(glArgs.fbo, args.colorAttachments, args.depthStencilAttachment);

    if(!frameBuffer)
    {
        glDeleteFramebuffers(1, &glArgs.fbo);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, frameBuffer);
}

GLFrameBuffer* GLFrameBufferBuilder::build(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLFrameBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLFrameBuffer* const frameBuffer = allocator.allocateT<GLFrameBuffer>(glArgs.fbo, args.colorAttachments, args.depthStencilAttachment);

    if(!frameBuffer)
    {
        glDeleteFramebuffers(1, &glArgs.fbo);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, frameBuffer);
}

CPPRef<IFrameBuffer> GLFrameBufferBuilder::buildCPPRef(const FrameBufferArgs& args, Error* error) const noexcept
{
    GLFrameBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLFrameBuffer> frameBuffer = CPPRef<GLFrameBuffer>(new(::std::nothrow) GLFrameBuffer(glArgs.fbo, args.colorAttachments, args.depthStencilAttachment));

    if(!frameBuffer)
    {
        glDeleteFramebuffers(1, &glArgs.fbo);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, frameBuffer);
}

NullableRef<IFrameBuffer> GLFrameBufferBuilder::buildTauRef(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLFrameBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLFrameBuffer> frameBuffer(allocator, glArgs.fbo, args.colorAttachments, args.depthStencilAttachment);

    if(!frameBuffer)
    {
        glDeleteFramebuffers(1, &glArgs.fbo);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, frameBuffer);
}

NullableStrongRef<IFrameBuffer> GLFrameBufferBuilder::buildTauSRef(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLFrameBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLFrameBuffer> frameBuffer(allocator, glArgs.fbo, args.colorAttachments, args.depthStencilAttachment);

    if(!frameBuffer)
    {
        glDeleteFramebuffers(1, &glArgs.fbo);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, frameBuffer);
}

static void attachCubeMap(const GLenum colorSlot, const CPPRef<GLTextureCube>& texture) noexcept;

bool GLFrameBufferBuilder::processArgs(const FrameBufferArgs& args, GLFrameBufferArgs* glArgs, Error* error) const noexcept
{
    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.colorAttachments[i], Error::NullAttachment);
        ERROR_CODE_COND_F(args.colorAttachments[i]->textureType() == ETexture::Type::DepthStencil, Error::ColorCannotBeDepthStencil);
        ERROR_CODE_COND_F(args.colorAttachments[i]->dataFormat() == ETexture::Format::Depth24Stencil8, Error::ColorCannotBeDepthStencil);
        ERROR_CODE_COND_F(!RTT_CHECK(args.colorAttachments[i].get(), GLTexture2D) && !RTT_CHECK(args.colorAttachments[i].get(), GLTextureCube), Error::CrossAPIFailure);
    }

    if(args.depthStencilAttachment)
    {
        ERROR_CODE_COND_F(args.depthStencilAttachment->textureType() != ETexture::Type::DepthStencil, Error::DepthStencilNotDepthStencil);
        ERROR_CODE_COND_F(args.depthStencilAttachment->dataFormat() != ETexture::Format::Depth24Stencil8, Error::DepthStencilNotDepthStencil);
        ERROR_CODE_COND_F(!RTT_CHECK(args.depthStencilAttachment.get(), GLTextureDepthStencil), Error::CrossAPIFailure);
    }

    glGenFramebuffers(1, &glArgs->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, glArgs->fbo);

    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        if(args.colorAttachments[i]->textureType() == ETexture::Type::Cube)
        {
            attachCubeMap(GL_COLOR_ATTACHMENT0 + i, RefCast<GLTextureCube>(args.colorAttachments[i]));
        }
        else if(args.colorAttachments[i]->textureType() == ETexture::Type::T2D)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, RefCast<GLTexture2D>(args.colorAttachments[i])->texture(), 0);
        }
    }

    if(args.depthStencilAttachment)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, RefCast<GLTextureDepthStencil>(args.depthStencilAttachment)->texture(), 0);
    }

    const GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(fbStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ERROR_CODE_F(Error::InternalError);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

static void attachCubeMap(const GLenum colorSlot, const CPPRef<GLTextureCube>& texture) noexcept
{
    if(GLEW_ARB_direct_state_access)
    {
        glNamedFramebufferTextureLayer(GL_FRAMEBUFFER, colorSlot, texture->texture(), 0, 0);
    }
    else
    {
        for(uSys i = 0; i < 6; ++i)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, colorSlot, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->texture(), 0);
        }
    }
}
