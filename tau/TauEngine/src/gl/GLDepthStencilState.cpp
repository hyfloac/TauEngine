#include "gl/GLDepthStencilState.hpp"
#include "gl/GLTexture.hpp"

void GLDepthStencilState::apply() const noexcept
{
    _glParams.depthTestControl(GL_DEPTH_TEST);
    _glParams.stencilTestControl(GL_STENCIL_TEST);

    glDepthMask(_glParams.depthWriteMask);
    glDepthFunc(_glParams.depthCompareFunc);

    glStencilMask(_glParams.stencilWriteMask);

    glStencilOpSeparate(GL_FRONT, _glParams.frontFace.failOp, _glParams.frontFace.stencilPassDepthFailOp, _glParams.frontFace.passOp);
    glStencilFuncSeparate(GL_FRONT, _glParams.frontFace.compareFunc, 1, _glParams.stencilReadMask);

    glStencilOpSeparate(GL_BACK, _glParams.backFace.failOp, _glParams.backFace.stencilPassDepthFailOp, _glParams.backFace.passOp);
    glStencilFuncSeparate(GL_BACK, _glParams.backFace.compareFunc, 1, _glParams.stencilReadMask);
}

GLDepthStencilState* GLDepthStencilStateBuilder::build(const DepthStencilParams& args, Error* error) const noexcept
{
    GLDepthStencilParams glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLDepthStencilState* const ret = new(::std::nothrow) GLDepthStencilState(args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ret);
}


GLDepthStencilState* GLDepthStencilStateBuilder::build(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDepthStencilParams glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLDepthStencilState* const ret = allocator.allocateT<GLDepthStencilState>(args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ret);
}


CPPRef<IDepthStencilState> GLDepthStencilStateBuilder::buildCPPRef(const DepthStencilParams& args, Error* error) const noexcept
{
    GLDepthStencilParams glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLDepthStencilState> ret = CPPRef<GLDepthStencilState>(new(::std::nothrow) GLDepthStencilState(args, glArgs));

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ret);
}

NullableRef<IDepthStencilState> GLDepthStencilStateBuilder::buildTauRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDepthStencilParams glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLDepthStencilState> ret(allocator, args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

NullableStrongRef<IDepthStencilState> GLDepthStencilStateBuilder::buildTauSRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDepthStencilParams glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLDepthStencilState> ret(allocator, args, glArgs);

    ERROR_CODE_COND_N(!ret, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

bool GLDepthStencilStateBuilder::processArgs(const DepthStencilParams& args, GLDepthStencilParams* glArgs, Error* error) noexcept
{
    glArgs->depthTestControl = args.enableDepthTest ? glEnable : glDisable;
    glArgs->stencilTestControl = args.enableStencilTest ? glEnable : glDisable;

    glArgs->depthWriteMask = args.depthWriteMask == DepthStencilParams::DepthWriteMask::Zero ? GL_FALSE : GL_TRUE;
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

    ERROR_CODE_COND_F(args.depthWriteMask != DepthStencilParams::DepthWriteMask::Zero && args.depthWriteMask != DepthStencilParams::DepthWriteMask::All, Error::InvalidDepthWriteMask);
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

GLenum GLDepthStencilStateBuilder::glStencilOperation(DepthStencilParams::StencilOp stencilOp) noexcept
{
    switch(stencilOp)
    {
        case DepthStencilParams::StencilOp::Keep:           return GL_KEEP;
        case DepthStencilParams::StencilOp::Zero:           return GL_ZERO;
        case DepthStencilParams::StencilOp::Replace:        return GL_REPLACE;
        case DepthStencilParams::StencilOp::Invert:         return GL_INVERT;
        case DepthStencilParams::StencilOp::IncrementClamp: return GL_INCR;
        case DepthStencilParams::StencilOp::DecrementClamp: return GL_DECR;
        case DepthStencilParams::StencilOp::IncrementWrap:  return GL_INCR_WRAP;
        case DepthStencilParams::StencilOp::DecrementWrap:  return GL_DECR_WRAP;
        default:                                            return static_cast<GLenum>(IntMaxMin<u32>::Max());
    }
}
