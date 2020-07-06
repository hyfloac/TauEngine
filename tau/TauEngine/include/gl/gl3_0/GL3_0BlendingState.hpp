#pragma once

#include "gl/GLBlendingState.hpp"

struct GL3_0BlendingArgs final
{
    DEFAULT_DESTRUCT(GL3_0BlendingArgs);
    DEFAULT_CM_PU(GL3_0BlendingArgs);
public:
    struct GLFrameBufferBlendingArgs final
    {
        DEFAULT_CONSTRUCT_PU(GLFrameBufferBlendingArgs);
        DEFAULT_DESTRUCT(GLFrameBufferBlendingArgs);
        DEFAULT_CM_PU(GLFrameBufferBlendingArgs);
    public:
        bool enableBlending;
        GLenum colorSrcFactor;
        GLenum colorDstFactor;
        GLenum alphaSrcFactor;
        GLenum alphaDstFactor;
        GLenum colorBlendOp;
        GLenum alphaBlendOp;
    public:
        GLFrameBufferBlendingArgs(bool _enableBlending, const GLenum colorSrcFactor, const GLenum colorDstFactor, const GLenum alphaSrcFactor, const GLenum alphaDstFactor, const GLenum colorBlendOp, const GLenum alphaBlendOp) noexcept
            : enableBlending(_enableBlending)
            , colorSrcFactor(colorSrcFactor)
            , colorDstFactor(colorDstFactor)
            , alphaSrcFactor(alphaSrcFactor)
            , alphaDstFactor(alphaDstFactor)
            , colorBlendOp(colorBlendOp)
            , alphaBlendOp(alphaBlendOp)
        { }
    };
public:
    GLFrameBufferBlendingArgs frameBuffers[8];
public:
    GL3_0BlendingArgs() noexcept
        : frameBuffers { }
    { }
};

class TAU_DLL GL3_0BlendingState final : public GLBlendingState
{
    DEFAULT_DESTRUCT(GL3_0BlendingState);
    DEFAULT_CM_PU(GL3_0BlendingState);
private:
    GL3_0BlendingArgs _glArgs;
public:
    GL3_0BlendingState(const BlendingArgs& args, const GL3_0BlendingArgs& glArgs) noexcept
        : GLBlendingState(args)
        , _glArgs(glArgs)
    { }

    void apply(GLStateHelper& glStateHelper) const noexcept override;
};

class TAU_DLL GL3_0BlendingStateBuilder final : public GLBlendingStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GL3_0BlendingStateBuilder);
    DEFAULT_DESTRUCT(GL3_0BlendingStateBuilder);
    DEFAULT_CM_PU(GL3_0BlendingStateBuilder);
public:
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs3_0(const BlendingArgs& args, [[tau::out]] GL3_0BlendingArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
