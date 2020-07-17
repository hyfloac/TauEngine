#include "gl/gl3_0/GL3_0BlendingState.hpp"
#include "gl/GLStateManager.hpp"

void GL3_0BlendingState::apply(GLStateManager& glStateManager) const noexcept
{
    for(uSys i = 0; i < 8; ++i)
    {
        glStateManager.setBlending(i, _glArgs.enableBlending[i]);
    }

    glStateManager.blendFunc(_glArgs.colorSrcFactor, _glArgs.colorDstFactor, _glArgs.alphaSrcFactor, _glArgs.alphaDstFactor);
    glStateManager.blendEquation(_glArgs.colorBlendOp, _glArgs.alphaBlendOp);
}

GLBlendingState* GL3_0BlendingStateBuilder::build(const BlendingArgs& args, Error* const error) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::build(args, error); }

    GL3_0BlendingArgs glArgs;
    if(!processArgs3_0(args, &glArgs, error))
    { return null; }

    GL3_0BlendingState* const blendingState = new(::std::nothrow) GL3_0BlendingState(args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

GLBlendingState* GL3_0BlendingStateBuilder::build(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::build(args, error, allocator); }

    GL3_0BlendingArgs glArgs;
    if(!processArgs3_0(args, &glArgs, error))
    { return null; }

    GL3_0BlendingState* const blendingState = allocator.allocateT<GL3_0BlendingState>(args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

CPPRef<IBlendingState> GL3_0BlendingStateBuilder::buildCPPRef(const BlendingArgs& args, Error* const error) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::buildCPPRef(args, error); }

    GL3_0BlendingArgs glArgs;
    if(!processArgs3_0(args, &glArgs, error))
    { return null; }

    const CPPRef<GL3_0BlendingState> blendingState = CPPRef<GL3_0BlendingState>(new(::std::nothrow) GL3_0BlendingState(args, glArgs));
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableRef<IBlendingState> GL3_0BlendingStateBuilder::buildTauRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::buildTauRef(args, error, allocator); }

    GL3_0BlendingArgs glArgs;
    if(!processArgs3_0(args, &glArgs, error))
    { return null; }

    const NullableRef<GL3_0BlendingState> blendingState(allocator, args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableStrongRef<IBlendingState> GL3_0BlendingStateBuilder::buildTauSRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::buildTauSRef(args, error, allocator); }

    GL3_0BlendingArgs glArgs;
    if(!processArgs3_0(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GL3_0BlendingState> blendingState(allocator, args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

bool GL3_0BlendingStateBuilder::processArgs3_0(const BlendingArgs& args, GL3_0BlendingArgs* const glArgs, Error* const error) const noexcept
{
    for(uSys i = 0; i < 8; ++i)
    {
        glArgs->enableBlending[i] = args.frameBuffers[i].enableBlending;
    }

    glArgs->colorSrcFactor = glBlendFactor(args.frameBuffers[0].colorSrcFactor);
    glArgs->colorDstFactor = glBlendFactor(args.frameBuffers[0].colorDstFactor);
    glArgs->alphaSrcFactor = glBlendFactor(args.frameBuffers[0].alphaSrcFactor);
    glArgs->alphaDstFactor = glBlendFactor(args.frameBuffers[0].alphaDstFactor);
    glArgs->colorBlendOp = glBlendOp(args.frameBuffers[0].colorBlendOp);
    glArgs->alphaBlendOp = glBlendOp(args.frameBuffers[0].alphaBlendOp);

    ERROR_CODE_COND_F(glArgs->colorSrcFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->colorDstFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->alphaSrcFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->alphaDstFactor == 0, Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(glArgs->colorBlendOp == 0, Error::InvalidBlendOp);
    ERROR_CODE_COND_F(glArgs->alphaBlendOp == 0, Error::InvalidBlendOp);

    return true;
}
