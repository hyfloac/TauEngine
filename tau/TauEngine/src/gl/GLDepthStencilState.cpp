#include "gl/GLDepthStencilState.hpp"

void GLDepthStencilState::apply() const noexcept
{
    _glArgs.depthTestControl(GL_DEPTH_TEST);
    _glArgs.stencilTestControl(GL_STENCIL_TEST);

    glDepthMask(_glArgs.depthWriteMask);
    glDepthFunc(_glArgs.depthCompareFunc);

    glStencilMask(_glArgs.stencilWriteMask);

    glStencilOpSeparate(GL_FRONT, _glArgs.frontFace.failOp, _glArgs.frontFace.stencilPassDepthFailOp, _glArgs.frontFace.passOp);
    glStencilFuncSeparate(GL_FRONT, _glArgs.frontFace.compareFunc, 1, _glArgs.stencilReadMask);

    glStencilOpSeparate(GL_BACK, _glArgs.backFace.failOp, _glArgs.backFace.stencilPassDepthFailOp, _glArgs.backFace.passOp);
    glStencilFuncSeparate(GL_BACK, _glArgs.backFace.compareFunc, 1, _glArgs.stencilReadMask);
}

GLDepthStencilState* GLDepthStencilStateBuilder::build(const DepthStencilArgs& args, Error* error) const noexcept
{
    GLDepthStencilArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLDepthStencilState* const ret = new(::std::nothrow) GLDepthStencilState(args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ret);
}

GLDepthStencilState* GLDepthStencilStateBuilder::build(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDepthStencilArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLDepthStencilState* const ret = allocator.allocateT<GLDepthStencilState>(args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ret);
}

CPPRef<IDepthStencilState> GLDepthStencilStateBuilder::buildCPPRef(const DepthStencilArgs& args, Error* error) const noexcept
{
    GLDepthStencilArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLDepthStencilState> ret = CPPRef<GLDepthStencilState>(new(::std::nothrow) GLDepthStencilState(args, glArgs));

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ret);
}

NullableRef<IDepthStencilState> GLDepthStencilStateBuilder::buildTauRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDepthStencilArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLDepthStencilState> ret(allocator, args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

NullableStrongRef<IDepthStencilState> GLDepthStencilStateBuilder::buildTauSRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDepthStencilArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLDepthStencilState> ret(allocator, args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

bool GLDepthStencilStateBuilder::processArgs(const DepthStencilArgs& args, GLDepthStencilArgs* glArgs, Error* error) noexcept
{
    glArgs->depthTestControl = args.enableDepthTest ? glEnable : glDisable;
    glArgs->stencilTestControl = args.enableStencilTest ? glEnable : glDisable;

    glArgs->depthWriteMask = args.depthWriteMask == DepthStencilArgs::DepthWriteMask::Zero ? GL_FALSE : GL_TRUE;
    glArgs->depthCompareFunc = GLTexture2D::glDepthCompareFunc(args.depthCompareFunc);

    glArgs->stencilReadMask = args.stencilReadMask;
    glArgs->stencilWriteMask = args.stencilWriteMask;
    
    glArgs->frontFace.failOp = glStencilOperation(args.frontFace.failOp);
    glArgs->frontFace.stencilPassDepthFailOp = glStencilOperation(args.frontFace.stencilPassDepthFailOp);
    glArgs->frontFace.passOp = glStencilOperation(args.frontFace.passOp);
    glArgs->frontFace.compareFunc = GLTexture2D::glDepthCompareFunc(args.frontFace.compareFunc);

    glArgs->backFace.failOp = glStencilOperation(args.backFace.failOp);
    glArgs->backFace.stencilPassDepthFailOp = glStencilOperation(args.backFace.stencilPassDepthFailOp);
    glArgs->backFace.passOp = glStencilOperation(args.backFace.passOp);
    glArgs->backFace.compareFunc = GLTexture2D::glDepthCompareFunc(args.backFace.compareFunc);

    ERROR_CODE_COND_F(args.depthWriteMask != DepthStencilArgs::DepthWriteMask::Zero && args.depthWriteMask != DepthStencilArgs::DepthWriteMask::All, Error::InvalidDepthWriteMask);
    ERROR_CODE_COND_F(glArgs->depthCompareFunc == 0, Error::InvalidDepthCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(glArgs->frontFace.failOp) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(glArgs->frontFace.stencilPassDepthFailOp) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(glArgs->frontFace.passOp) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilPassOp);
    ERROR_CODE_COND_F(glArgs->frontFace.compareFunc == 0, Error::InvalidFrontFaceStencilCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(glArgs->backFace.failOp) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(glArgs->backFace.stencilPassDepthFailOp) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(glArgs->backFace.passOp) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilPassOp);
    ERROR_CODE_COND_F(glArgs->backFace.compareFunc == 0, Error::InvalidBackFaceStencilCompareFunc);

    return true;
}

GLenum GLDepthStencilStateBuilder::glStencilOperation(DepthStencilArgs::StencilOp stencilOp) noexcept
{
    switch(stencilOp)
    {
        case DepthStencilArgs::StencilOp::Keep:           return GL_KEEP;
        case DepthStencilArgs::StencilOp::Zero:           return GL_ZERO;
        case DepthStencilArgs::StencilOp::Replace:        return GL_REPLACE;
        case DepthStencilArgs::StencilOp::Invert:         return GL_INVERT;
        case DepthStencilArgs::StencilOp::IncrementClamp: return GL_INCR;
        case DepthStencilArgs::StencilOp::DecrementClamp: return GL_DECR;
        case DepthStencilArgs::StencilOp::IncrementWrap:  return GL_INCR_WRAP;
        case DepthStencilArgs::StencilOp::DecrementWrap:  return GL_DECR_WRAP;
        default:                                            return static_cast<GLenum>(IntMaxMin<u32>::Max());
    }
}
