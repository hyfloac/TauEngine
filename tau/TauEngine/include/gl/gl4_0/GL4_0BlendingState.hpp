#pragma once

#include "gl/GLBlendingState.hpp"

struct GL4_0FrameBufferBlendingArgs final
{
    DEFAULT_CONSTRUCT_PU(GL4_0FrameBufferBlendingArgs);
    DEFAULT_DESTRUCT(GL4_0FrameBufferBlendingArgs);
    DEFAULT_COPY(GL4_0FrameBufferBlendingArgs);
public:
    void (*GLAPIENTRY blendingControl)(GLenum, GLuint);
    GLenum colorSrcFactor;
    GLenum colorDstFactor;
    GLenum alphaSrcFactor;
    GLenum alphaDstFactor;
    GLenum colorBlendOp;
    GLenum alphaBlendOp;
public:
    GL4_0FrameBufferBlendingArgs(void(* const blendingControl)(GLenum, GLuint), const GLenum colorSrcFactor, const GLenum colorDstFactor, const GLenum alphaSrcFactor, const GLenum alphaDstFactor, const GLenum colorBlendOp, const GLenum alphaBlendOp) noexcept
        : blendingControl{ blendingControl }
        , colorSrcFactor(colorSrcFactor)
        , colorDstFactor(colorDstFactor)
        , alphaSrcFactor(alphaSrcFactor)
        , alphaDstFactor(alphaDstFactor)
        , colorBlendOp(colorBlendOp)
        , alphaBlendOp(alphaBlendOp)
    { }
};

struct GL4_0BlendingArgs final
{
    GL4_0FrameBufferBlendingArgs frameBuffers[8];
};

class TAU_DLL GL4_0BlendingState final : public GLBlendingState
{
    DEFAULT_DESTRUCT(GL4_0BlendingState);
    BS_IMPL(GL4_0BlendingState);
private:
    GL4_0FrameBufferBlendingArgs _glArgs[8];
public:
    GL4_0BlendingState(const BlendingArgs& args, GL4_0FrameBufferBlendingArgs glArgs[8]) noexcept
        : GLBlendingState(args)
        , _glArgs{ glArgs[0], glArgs[1], glArgs[2], glArgs[3], glArgs[4], glArgs[5], glArgs[6], glArgs[7] }
    { }

    void apply() const noexcept override;
};

class TAU_DLL GL4_0BlendingStateBuilder final : public GLBlendingStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GL4_0BlendingStateBuilder);
    DEFAULT_DESTRUCT(GL4_0BlendingStateBuilder);
    DELETE_COPY(GL4_0BlendingStateBuilder);
public:
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs4_0(const BlendingArgs& args, [[tau::out]] GL4_0BlendingArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
