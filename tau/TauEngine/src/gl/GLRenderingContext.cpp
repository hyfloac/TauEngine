#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <Utils.hpp>

#include "gl/GLRenderingContext.hpp"

#include "gl/GLShader.hpp"
#include "gl/GLTexture.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "gl/GLTextureSampler.hpp"
#include "gl/GLTextureUploader.hpp"
#include "gl/GLBufferDescriptor.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLRasterizerState.hpp"
#include "gl/GLGraphicsInterface.hpp"

#include "gl/GLBuffer.hpp"
#include "gl/gl4_5/GLBuffer4_5.hpp"

#include "Timings.hpp"

GLRenderingContext::GLRenderingContext(const RenderingMode& mode, const GLRenderingContextArgs& glArgs, const GLSystemRenderingContextArgs& glSysArgs) noexcept
    : IRenderingContext(mode), _gi(glArgs.gi),
      _device(glSysArgs.device), _context(glSysArgs.context),
      _defaultDepthStencilState(null), _currentDepthStencilState(null),
      _defaultRasterizerState(null), _currentRasterizerState(null)
{
    switch(_mode.currentMode())
    {
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
            _bufferBuilder = new(::std::nothrow) GLBufferBuilder;
            _indexBufferBuilder = new(::std::nothrow) GLIndexBufferBuilder;
            _uniformBufferBuilder = new(::std::nothrow) GLUniformBufferBuilder;
            break;
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            _bufferBuilder = new(::std::nothrow) GLBuffer4_5Builder;
            _indexBufferBuilder = new(::std::nothrow) GLIndexBuffer4_5Builder;
            _uniformBufferBuilder = new(::std::nothrow) GLUniformBuffer4_5Builder;
            break;
        default: break;
    }

    _textureSamplerBuilder = new(::std::nothrow) GLTextureSamplerBuilder;
    _texture2DBuilder = new(::std::nothrow) GLTexture2DBuilder;
    _textureNullBuilder = new(::std::nothrow) GLTextureNullBuilder;
    _textureDepthBuilder = new(::std::nothrow) GLTextureDepthBuilder;
    _textureCubeBuilder = new(::std::nothrow) GLTextureCubeBuilder;
}

GLRenderingContext::~GLRenderingContext() noexcept
{
    delete _bufferBuilder;
    delete _indexBufferBuilder;
    delete _uniformBufferBuilder;
    delete _textureSamplerBuilder;
    delete _texture2DBuilder;
    delete _textureNullBuilder;
    delete _textureDepthBuilder;
    delete _textureCubeBuilder;

    systemDestruct();
}

void GLRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    glViewport(x, y, width, height);
}

void GLRenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
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

void GLRenderingContext::setFaceWinding(const bool clockwise) noexcept
{
    glFrontFace(clockwise ? GL_CW : GL_CCW);
}

void GLRenderingContext::enableDepthWriting(bool writing) noexcept
{
    glDepthMask(writing ? GL_TRUE : GL_FALSE);
}

NullableRef<IDepthStencilState> GLRenderingContext::setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept
{
    NullableRef<IDepthStencilState> ret = RefCast<IDepthStencilState>(_currentDepthStencilState);

    if(!dsState || !RTT_CHECK(dsState.get(), GLDepthStencilState))
    { return ret; }

    _currentDepthStencilState = RefCast<GLDepthStencilState>(dsState);
    _currentDepthStencilState->apply();

    return ret;
}

void GLRenderingContext::setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept
{
    if(!dsState || !RTT_CHECK(dsState.get(), GLDepthStencilState))
    { return; }

    _defaultDepthStencilState = RefCast<GLDepthStencilState>(dsState);
}

void GLRenderingContext::resetDepthStencilState() noexcept
{
    _currentDepthStencilState = _defaultDepthStencilState;
    _currentDepthStencilState->apply();
}

const DepthStencilArgs& GLRenderingContext::getDefaultDepthStencilArgs() noexcept
{ return _defaultDepthStencilState->args(); }

NullableRef<IRasterizerState> GLRenderingContext::setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept
{
    NullableRef<IRasterizerState> ret = RefCast<IRasterizerState>(_currentRasterizerState);

    if(!rsState || !RTT_CHECK(rsState.get(), GLRasterizerState))
    { return ret; }

    _currentRasterizerState = RefCast<GLRasterizerState>(rsState);
    _currentRasterizerState->apply();

    return ret;
}

void GLRenderingContext::setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept
{
    if(!rsState || !RTT_CHECK(rsState.get(), GLRasterizerState))
    { return; }

    _defaultRasterizerState = RefCast<GLRasterizerState>(rsState);
}

void GLRenderingContext::resetRasterizerState() noexcept
{
    _currentRasterizerState = _defaultRasterizerState;
    _currentRasterizerState->apply();
}

const RasterizerArgs& GLRenderingContext::getDefaultRasterizerArgs() noexcept
{ return _defaultRasterizerState->args(); }

CPPRef<IVertexArrayBuilder> GLRenderingContext::createVertexArray(const uSys bufferCount) noexcept
{
    return CPPRef<IVertexArrayBuilder>(new(::std::nothrow) GLVertexArrayBuilder(bufferCount, *this));
}

IBufferBuilder& GLRenderingContext::createBuffer() noexcept
{ return *_bufferBuilder; }

IIndexBufferBuilder& GLRenderingContext::createIndexBuffer() noexcept
{ return *_indexBufferBuilder; }

IUniformBufferBuilder& GLRenderingContext::createUniformBuffer() noexcept
{ return *_uniformBufferBuilder; }

CPPRef<IFrameBufferBuilder> GLRenderingContext::createFrameBuffer() noexcept
{
    return CPPRef<GLFrameBufferBuilder>(new(::std::nothrow) GLFrameBufferBuilder);
}

ITextureBuilder& GLRenderingContext::createTexture2D() noexcept
{ return *_texture2DBuilder; }

ITextureBuilder& GLRenderingContext::createNullTexture() noexcept
{ return *_textureNullBuilder; }

ITextureBuilder& GLRenderingContext::createTextureDepth() noexcept
{ return *_textureDepthBuilder; }

ITextureCubeBuilder& GLRenderingContext::createTextureCube() noexcept
{ return *_textureCubeBuilder; }

ITextureSamplerBuilder& GLRenderingContext::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

CPPRef<ITextureUploaderBuilder> GLRenderingContext::createTextureUploader(const uSys textureCount) noexcept
{
    return CPPRef<GLTextureUploaderBuilder>(new(::std::nothrow) GLTextureUploaderBuilder(textureCount, *this));
}

CPPRef<ISingleTextureUploaderBuilder> GLRenderingContext::createSingleTextureUploader() noexcept
{
    return CPPRef<GLSingleTextureUploaderBuilder>(new(::std::nothrow) GLSingleTextureUploaderBuilder(*this));
}

IShaderBuilder& GLRenderingContext::createShader() noexcept
{ return _gi.createShader(); }

GLRenderingContext* GLRenderingContextBuilder::build(const RenderingContextArgs& args, Error* const error) noexcept
{
    GLRenderingContextArgs glArgs { _gi };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLSystemRenderingContextArgs glSysArgs { };
    if(!processSysArgs(args, glArgs, &glSysArgs, error))
    { return null; }

    GLRenderingContext* const context = new(::std::nothrow) GLRenderingContext(_gi.renderingMode(), glArgs, glSysArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

GLRenderingContext* GLRenderingContextBuilder::build(const RenderingContextArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GLRenderingContextArgs glArgs { _gi };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLSystemRenderingContextArgs glSysArgs { };
    if(!processSysArgs(args, glArgs, &glSysArgs, error))
    { return null; }

    GLRenderingContext* const context = allocator.allocateT<GLRenderingContext>(_gi.renderingMode(), glArgs, glSysArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

CPPRef<IRenderingContext> GLRenderingContextBuilder::buildCPPRef(const RenderingContextArgs& args, Error* const error) noexcept
{
    GLRenderingContextArgs glArgs { _gi };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLSystemRenderingContextArgs glSysArgs { };
    if(!processSysArgs(args, glArgs, &glSysArgs, error))
    { return null; }

    const CPPRef<GLRenderingContext> context = CPPRef<GLRenderingContext>(new(::std::nothrow) GLRenderingContext(_gi.renderingMode(), glArgs, glSysArgs));

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

NullableRef<IRenderingContext> GLRenderingContextBuilder::buildTauRef(const RenderingContextArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GLRenderingContextArgs glArgs { _gi };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLSystemRenderingContextArgs glSysArgs { };
    if(!processSysArgs(args, glArgs, &glSysArgs, error))
    { return null; }

    const NullableRef<GLRenderingContext> context(allocator, _gi.renderingMode(), glArgs, glSysArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, RefCast<IRenderingContext>(context));
}

NullableStrongRef<IRenderingContext> GLRenderingContextBuilder::buildTauSRef(const RenderingContextArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GLRenderingContextArgs glArgs { _gi };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLSystemRenderingContextArgs glSysArgs { };
    if(!processSysArgs(args, glArgs, &glSysArgs, error))
    { return null; }

    const NullableStrongRef<GLRenderingContext> context(allocator, _gi.renderingMode(), glArgs, glSysArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, RefCast<IRenderingContext>(context));
}

bool GLRenderingContextBuilder::processArgs(const RenderingContextArgs& args, GLRenderingContextArgs* const glArgs, Error* const error) noexcept
{
    return true;
}
