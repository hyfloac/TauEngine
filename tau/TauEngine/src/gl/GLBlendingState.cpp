#include "gl/GLBlendingState.hpp"
#include "gl/GLStateHelper.hpp"

void GL2_0BlendingState::apply(GLStateHelper& glStateHelper) const noexcept
{
    glStateHelper.setBlending(_glArgs.enableBlending);
    glStateHelper.blendFunc(_glArgs.colorSrcFactor, _glArgs.colorDstFactor, _glArgs.alphaSrcFactor, _glArgs.alphaDstFactor);
    glStateHelper.blendEquation(_glArgs.colorBlendOp, _glArgs.alphaBlendOp);
}

GLBlendingState* GLBlendingStateBuilder::build(const BlendingArgs& args, Error* const error) const noexcept
{
    GL2_0BlendingArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GL2_0BlendingState* const blendingState = new(::std::nothrow) GL2_0BlendingState(args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

GLBlendingState* GLBlendingStateBuilder::build(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GL2_0BlendingArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GL2_0BlendingState* const blendingState = allocator.allocateT<GL2_0BlendingState>(args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

CPPRef<IBlendingState> GLBlendingStateBuilder::buildCPPRef(const BlendingArgs& args, Error* const error) const noexcept
{
    GL2_0BlendingArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GL2_0BlendingState> blendingState = CPPRef<GL2_0BlendingState>(new(::std::nothrow) GL2_0BlendingState(args, glArgs));
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableRef<IBlendingState> GLBlendingStateBuilder::buildTauRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GL2_0BlendingArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GL2_0BlendingState> blendingState(allocator, args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableStrongRef<IBlendingState> GLBlendingStateBuilder::buildTauSRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GL2_0BlendingArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GL2_0BlendingState> blendingState(allocator, args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

bool GLBlendingStateBuilder::processArgs(const BlendingArgs& args, GL2_0BlendingArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.independentBlending, Error::DriverDoesNotSupportIndependentBlending);

    glArgs->enableBlending = args.frameBuffers[0].enableBlending;
    glArgs->colorSrcFactor = glBlendFactor(args.frameBuffers[0].colorSrcFactor);
    glArgs->colorDstFactor = glBlendFactor(args.frameBuffers[0].colorDstFactor);
    glArgs->alphaSrcFactor = glBlendFactor(args.frameBuffers[0].alphaSrcFactor);
    glArgs->alphaDstFactor = glBlendFactor(args.frameBuffers[0].alphaDstFactor);
    glArgs->colorBlendOp   = glBlendOp(args.frameBuffers[0].colorBlendOp);
    glArgs->alphaBlendOp   = glBlendOp(args.frameBuffers[0].alphaBlendOp);

    ERROR_CODE_COND_F(glArgs->colorSrcFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->colorDstFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->alphaSrcFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->alphaDstFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->colorBlendOp == 0, Error::InvalidBlendOp);
    ERROR_CODE_COND_F(glArgs->alphaBlendOp == 0, Error::InvalidBlendOp);

    return true;
}

GLenum GLBlendingStateBuilder::glBlendFactor(const BlendingArgs::BlendFactor blendFactor) noexcept
{
    switch(blendFactor)
    {
        case BlendingArgs::BlendFactor::Zero:             return GL_ZERO;
        case BlendingArgs::BlendFactor::One:              return GL_ONE;
        case BlendingArgs::BlendFactor::SrcColor:         return GL_SRC_COLOR;
        case BlendingArgs::BlendFactor::InvSrcColor:      return GL_ONE_MINUS_SRC_COLOR;
        case BlendingArgs::BlendFactor::SrcAlpha:         return GL_SRC_ALPHA;
        case BlendingArgs::BlendFactor::InvSrcAlpha:      return GL_ONE_MINUS_SRC_ALPHA;
        case BlendingArgs::BlendFactor::DestAlpha:        return GL_DST_ALPHA;
        case BlendingArgs::BlendFactor::InvDestAlpha:     return GL_ONE_MINUS_DST_ALPHA;
        case BlendingArgs::BlendFactor::DestColor:        return GL_DST_COLOR;
        case BlendingArgs::BlendFactor::InvDestColor:     return GL_ONE_MINUS_DST_COLOR;
        case BlendingArgs::BlendFactor::SrcAlphaSat:      return GL_SRC_ALPHA_SATURATE;
        case BlendingArgs::BlendFactor::BlendConstant:    return GL_CONSTANT_COLOR;
        case BlendingArgs::BlendFactor::InvBlendConstant: return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendingArgs::BlendFactor::Src1Color:        return GL_SRC1_COLOR;
        case BlendingArgs::BlendFactor::InvSrc1Color:     return GL_ONE_MINUS_SRC1_COLOR;
        case BlendingArgs::BlendFactor::Src1Alpha:        return GL_SRC1_ALPHA;
        case BlendingArgs::BlendFactor::InvSrc1Alpha:     return GL_ONE_MINUS_SRC1_ALPHA;
        default: return 0;
    }
}

GLenum GLBlendingStateBuilder::glBlendOp(const BlendingArgs::BlendOp blendOp) noexcept
{
    switch(blendOp)
    {
        case BlendingArgs::BlendOp::Add:         return GL_FUNC_ADD;
        case BlendingArgs::BlendOp::Subtract:    return GL_FUNC_SUBTRACT;
        case BlendingArgs::BlendOp::RevSubtract: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendingArgs::BlendOp::Min:         return GL_MIN;
        case BlendingArgs::BlendOp::Max:         return GL_MAX;
        default: return 0;
    }
}
