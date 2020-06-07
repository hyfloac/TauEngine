#pragma once

#include "gl/GLBlendingState.hpp"

struct GL3_0BlendingArgs final
{
    DEFAULT_CONSTRUCT_PU(GL3_0BlendingArgs);
    DEFAULT_DESTRUCT(GL3_0BlendingArgs);
    DEFAULT_CM_PU(GL3_0BlendingArgs);
public:
    void (*GLAPIENTRY blendingControl[8])(GLenum, GLuint);
    GLenum colorSrcFactor;
    GLenum colorDstFactor;
    GLenum alphaSrcFactor;
    GLenum alphaDstFactor;
    GLenum colorBlendOp;
    GLenum alphaBlendOp;
public:
    GL3_0BlendingArgs(void(* const blendingControl[8])(GLenum, GLuint), const GLenum colorSrcFactor, const GLenum colorDstFactor, const GLenum alphaSrcFactor, const GLenum alphaDstFactor, const GLenum colorBlendOp, const GLenum alphaBlendOp) noexcept
        : blendingControl{ blendingControl[0], blendingControl[1], blendingControl[3], blendingControl[3], blendingControl[4], blendingControl[5], blendingControl[6], blendingControl[7] }
        , colorSrcFactor(colorSrcFactor)
        , colorDstFactor(colorDstFactor)
        , alphaSrcFactor(alphaSrcFactor)
        , alphaDstFactor(alphaDstFactor)
        , colorBlendOp(colorBlendOp)
        , alphaBlendOp(alphaBlendOp)
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

    void apply() const noexcept override;
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
