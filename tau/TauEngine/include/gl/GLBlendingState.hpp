#pragma once

#include "graphics/BlendingState.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class TAU_DLL GLBlendingState : public IBlendingState
{
    DEFAULT_DESTRUCT(GLBlendingState);
    DEFAULT_COPY_PO(GLBlendingState);
    BS_IMPL(GLBlendingState);
protected:
    GLBlendingState(const BlendingArgs& args) noexcept
        : IBlendingState(args)
    { }
public:
    virtual void apply() const noexcept = 0;
};

struct GL2_0BlendingArgs final
{
    DEFAULT_CONSTRUCT_PU(GL2_0BlendingArgs);
    DEFAULT_DESTRUCT(GL2_0BlendingArgs);
    DEFAULT_CM_PU(GL2_0BlendingArgs);
public:
    void (*GLAPIENTRY blendingControl)(GLenum);
    GLenum colorSrcFactor;
    GLenum colorDstFactor;
    GLenum alphaSrcFactor;
    GLenum alphaDstFactor;
    GLenum colorBlendOp;
    GLenum alphaBlendOp;
public:
    GL2_0BlendingArgs(void(* const blendingControl)(GLenum), const GLenum colorSrcFactor, const GLenum colorDstFactor, const GLenum alphaSrcFactor, const GLenum alphaDstFactor, const GLenum colorBlendOp, const GLenum alphaBlendOp) noexcept
        : blendingControl{ blendingControl }
        , colorSrcFactor(colorSrcFactor)
        , colorDstFactor(colorDstFactor)
        , alphaSrcFactor(alphaSrcFactor)
        , alphaDstFactor(alphaDstFactor)
        , colorBlendOp(colorBlendOp)
        , alphaBlendOp(alphaBlendOp)
    { }
};

class TAU_DLL GL2_0BlendingState final : public GLBlendingState
{
    DEFAULT_DESTRUCT(GL2_0BlendingState);
    DEFAULT_CM_PU(GL2_0BlendingState);
private:
    GL2_0BlendingArgs _glArgs;
public:
    GL2_0BlendingState(const BlendingArgs& args, const GL2_0BlendingArgs& glArgs) noexcept
        : GLBlendingState(args)
        , _glArgs(glArgs)
    { }

    void apply() const noexcept override;
};

class TAU_DLL GLBlendingStateBuilder : public IBlendingStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GLBlendingStateBuilder);
    DEFAULT_DESTRUCT(GLBlendingStateBuilder);
    DEFAULT_CM_PU(GLBlendingStateBuilder);
public:
    [[nodiscard]] static GLenum glBlendFactor(BlendingArgs::BlendFactor blendFactor) noexcept;
    [[nodiscard]] static GLenum glBlendOp(BlendingArgs::BlendOp blendOp) noexcept;
public:
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const BlendingArgs& args, [[tau::out]] GL2_0BlendingArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
