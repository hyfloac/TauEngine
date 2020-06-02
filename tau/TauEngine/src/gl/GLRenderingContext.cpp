#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <Utils.hpp>

#include "gl/GLRenderingContext.hpp"

#include "gl/GLShader.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "gl/GLBufferDescriptor.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLRasterizerState.hpp"
#include "gl/GLBlendingState.hpp"
#include "gl/GLGraphicsInterface.hpp"

#include "gl/GLVertexBuffer.hpp"

#include "Timings.hpp"

GLRenderingContext::GLRenderingContext(const RenderingMode& mode, const GLRenderingContextArgs& glArgs, const GLSystemRenderingContextArgs& glSysArgs) noexcept
    : IRenderingContext(mode)
    , _gi(glArgs.gi)
    , _device(glSysArgs.device)
    , _context(glSysArgs.context)
    , _iaOffsets(null)
    , _iaStrides(null)
    , _defaultDepthStencilState(null)
    , _currentDepthStencilState(null)
    , _defaultRasterizerState(null)
    , _currentRasterizerState(null)
    , _defaultBlendingState(null)
    , _currentBlendingState(null)
{ }

GLRenderingContext::~GLRenderingContext() noexcept
{ systemDestruct(); }

void GLRenderingContext::setBuffers(uSys count, const GLuint* buffers) const noexcept
{
    glBindVertexBuffers(0, count, buffers, _iaOffsets, _iaStrides);
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

NullableRef<IDepthStencilState> GLRenderingContext::getDefaultDepthStencilState() noexcept
{ return _defaultDepthStencilState; }

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

NullableRef<IRasterizerState> GLRenderingContext::getDefaultRasterizerState() noexcept
{ return _defaultRasterizerState; }

NullableRef<IBlendingState> GLRenderingContext::setBlendingState(const NullableRef<IBlendingState>& bsState, const float color[4]) noexcept
{
    NullableRef<IBlendingState> ret = RefCast<IBlendingState>(_currentBlendingState);

    if(!bsState || !RTT_CHECK(bsState.get(), GLBlendingState))
    { return ret; }

    _currentBlendingState = RefCast<GLBlendingState>(bsState);
    _currentBlendingState->apply();
    glBlendColor(color[0], color[1], color[2], color[3]);

    return ret;
}

void GLRenderingContext::setDefaultBlendingState(const NullableRef<IBlendingState>& bsState) noexcept
{
    if(!bsState || !RTT_CHECK(bsState.get(), GLBlendingState))
    { return; }

    _defaultBlendingState = RefCast<GLBlendingState>(bsState);
}

void GLRenderingContext::resetBlendingState(const float color[4]) noexcept
{
    _currentBlendingState = _defaultBlendingState;
    _currentBlendingState->apply();
    glBlendColor(color[0], color[1], color[2], color[3]);
}

const BlendingArgs& GLRenderingContext::getDefaultBlendingArgs() noexcept
{ return _defaultBlendingState->args(); }

NullableRef<IBlendingState> GLRenderingContext::getDefaultBlendingState() noexcept
{ return _defaultBlendingState; }

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
