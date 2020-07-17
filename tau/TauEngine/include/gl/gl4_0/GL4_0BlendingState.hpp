#pragma once

#include "gl/GLBlendingState.hpp"

struct GL4_0BlendingArgs final
{
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
        GLFrameBufferBlendingArgs(bool _enableBlending, const GLenum _colorSrcFactor, const GLenum _colorDstFactor, const GLenum _alphaSrcFactor, const GLenum _alphaDstFactor, const GLenum _colorBlendOp, const GLenum _alphaBlendOp) noexcept
            : enableBlending(_enableBlending)
            , colorSrcFactor(_colorSrcFactor)
            , colorDstFactor(_colorDstFactor)
            , alphaSrcFactor(_alphaSrcFactor)
            , alphaDstFactor(_alphaDstFactor)
            , colorBlendOp(_colorBlendOp)
            , alphaBlendOp(_alphaBlendOp)
        { }
    };
public:
    GLFrameBufferBlendingArgs frameBuffers[8];
};

class TAU_DLL GL4_0BlendingState final : public GLBlendingState
{
    DEFAULT_DESTRUCT(GL4_0BlendingState);
    DEFAULT_CM_PU(GL4_0BlendingState);
    BS_IMPL(GL4_0BlendingState);
private:
    GL4_0BlendingArgs _glArgs;
public:
    GL4_0BlendingState(const BlendingArgs& args, const GL4_0BlendingArgs& glArgs) noexcept
        : GLBlendingState(args)
        , _glArgs(glArgs)
    { }

    void apply(GLStateManager& glStateManager) const noexcept override;
};

class TAU_DLL GL4_0BlendingStateBuilder final : public GLBlendingStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GL4_0BlendingStateBuilder);
    DEFAULT_DESTRUCT(GL4_0BlendingStateBuilder);
    DEFAULT_CM_PU(GL4_0BlendingStateBuilder);
public:
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs4_0(const BlendingArgs& args, [[tau::out]] GL4_0BlendingArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
