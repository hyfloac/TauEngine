#pragma once

#include "gl/GLBlendingState.hpp"

struct GL3_0BlendingArgs final
{
    DEFAULT_CONSTRUCT_PU(GL3_0BlendingArgs);
    DEFAULT_DESTRUCT(GL3_0BlendingArgs);
    DEFAULT_CM_PU(GL3_0BlendingArgs);
public:
    bool enableBlending[8];
    GLenum colorSrcFactor;
    GLenum colorDstFactor;
    GLenum alphaSrcFactor;
    GLenum alphaDstFactor;
    GLenum colorBlendOp;
    GLenum alphaBlendOp;
public:
    GL3_0BlendingArgs(bool _enableBlending[8], const GLenum _colorSrcFactor, const GLenum _colorDstFactor, const GLenum _alphaSrcFactor, const GLenum _alphaDstFactor, const GLenum _colorBlendOp, const GLenum _alphaBlendOp) noexcept
        : enableBlending { _enableBlending[0], _enableBlending[1], _enableBlending[2], _enableBlending[3], _enableBlending[4], _enableBlending[5], _enableBlending[6], _enableBlending[7] }
        , colorSrcFactor(_colorSrcFactor)
        , colorDstFactor(_colorDstFactor)
        , alphaSrcFactor(_alphaSrcFactor)
        , alphaDstFactor(_alphaDstFactor)
        , colorBlendOp(_colorBlendOp)
        , alphaBlendOp(_alphaBlendOp)
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

    void apply(GLStateManager& glStateManager) const noexcept override;
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
