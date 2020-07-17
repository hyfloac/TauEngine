#include "gl/gl4_0/GL4_0BlendingState.hpp"
#include "gl/GLStateManager.hpp"

void GL4_0BlendingState::apply(GLStateManager& glStateManager) const noexcept
{
    for(uSys i = 0; i < 8; ++i)
    {
        glStateManager.setBlending(i, _glArgs.frameBuffers[i].enableBlending);
        glStateManager.blendFunc(i, _glArgs.frameBuffers[i].colorSrcFactor, _glArgs.frameBuffers[i].colorDstFactor, _glArgs.frameBuffers[i].alphaSrcFactor, _glArgs.frameBuffers[i].alphaDstFactor);
        glStateManager.blendEquation(i, _glArgs.frameBuffers[i].colorBlendOp, _glArgs.frameBuffers[i].alphaBlendOp);
    }
}

GLBlendingState* GL4_0BlendingStateBuilder::build(const BlendingArgs& args, Error* const error) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::build(args, error); }

    GL4_0BlendingArgs glArgs;
    if(!processArgs4_0(args, &glArgs, error))
    { return null; }

    GL4_0BlendingState* const blendingState = new(::std::nothrow) GL4_0BlendingState(args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

GLBlendingState* GL4_0BlendingStateBuilder::build(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::build(args, error, allocator); }

    GL4_0BlendingArgs glArgs;
    if(!processArgs4_0(args, &glArgs, error))
    { return null; }

    GL4_0BlendingState* const blendingState = allocator.allocateT<GL4_0BlendingState>(args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

CPPRef<IBlendingState> GL4_0BlendingStateBuilder::buildCPPRef(const BlendingArgs& args, Error* const error) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::buildCPPRef(args, error); }

    GL4_0BlendingArgs glArgs;
    if(!processArgs4_0(args, &glArgs, error))
    { return null; }

    const CPPRef<GL4_0BlendingState> blendingState = CPPRef<GL4_0BlendingState>(new(::std::nothrow) GL4_0BlendingState(args, glArgs));
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableRef<IBlendingState> GL4_0BlendingStateBuilder::buildTauRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::buildTauRef(args, error, allocator); }

    GL4_0BlendingArgs glArgs;
    if(!processArgs4_0(args, &glArgs, error))
    { return null; }

    const NullableRef<GL4_0BlendingState> blendingState(allocator, args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableStrongRef<IBlendingState> GL4_0BlendingStateBuilder::buildTauSRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!args.independentBlending)
    { return GLBlendingStateBuilder::buildTauSRef(args, error, allocator); }

    GL4_0BlendingArgs glArgs;
    if(!processArgs4_0(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GL4_0BlendingState> blendingState(allocator, args, glArgs);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

bool GL4_0BlendingStateBuilder::processArgs4_0(const BlendingArgs& args, GL4_0BlendingArgs* const glArgs, Error* const error) const noexcept
{
    for(uSys i = 0; i < 8; ++i)
    {
        glArgs->frameBuffers[i].enableBlending = args.frameBuffers[i].enableBlending;
        glArgs->frameBuffers[i].colorSrcFactor = glBlendFactor(args.frameBuffers[i].colorSrcFactor);
        glArgs->frameBuffers[i].colorDstFactor = glBlendFactor(args.frameBuffers[i].colorDstFactor);
        glArgs->frameBuffers[i].alphaSrcFactor = glBlendFactor(args.frameBuffers[i].alphaSrcFactor);
        glArgs->frameBuffers[i].alphaDstFactor = glBlendFactor(args.frameBuffers[i].alphaDstFactor);
        glArgs->frameBuffers[i].colorBlendOp   = glBlendOp(args.frameBuffers[i].colorBlendOp);
        glArgs->frameBuffers[i].alphaBlendOp   = glBlendOp(args.frameBuffers[i].alphaBlendOp);

        ERROR_CODE_COND_F(glArgs->frameBuffers[i].colorSrcFactor == 0, Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(glArgs->frameBuffers[i].colorDstFactor == 0, Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(glArgs->frameBuffers[i].alphaSrcFactor == 0, Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(glArgs->frameBuffers[i].alphaDstFactor == 0, Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(glArgs->frameBuffers[i].colorBlendOp == 0, Error::InvalidBlendOp);
        ERROR_CODE_COND_F(glArgs->frameBuffers[i].alphaBlendOp == 0, Error::InvalidBlendOp);
    }

    return true;
}
