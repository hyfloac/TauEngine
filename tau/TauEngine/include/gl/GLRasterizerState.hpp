#pragma once

#include "graphics/RasterizerState.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class GLStateManager;

class TAU_DLL GLRasterizerState final : public IRasterizerState
{
    DEFAULT_DESTRUCT(GLRasterizerState);
    DEFAULT_CM_PU(GLRasterizerState);
    RS_IMPL(GLRasterizerState);
private:
    GLenum _frontFace;
    GLenum _cullMode;
    GLenum _fillMode;
    GLfloat _polygonOffsetFactor;
    GLfloat _polygonOffsetUnits;
    GLfloat _polygonOffsetClamp;
public:
    GLRasterizerState(const RasterizerArgs& params, const GLenum frontFace, const GLenum cullMode, const GLenum fillMode, const GLfloat polygonOffsetFactor, const GLfloat polygonOffsetUnits, const GLfloat polygonOffsetClamp) noexcept
        : IRasterizerState(params)
        , _frontFace(frontFace)
        , _cullMode(cullMode)
        , _fillMode(fillMode)
        , _polygonOffsetFactor(polygonOffsetFactor)
        , _polygonOffsetUnits(polygonOffsetUnits)
        , _polygonOffsetClamp(polygonOffsetClamp)
    { }

    void apply(GLStateManager& glStateManager) const noexcept;
};

class TAU_DLL GLRasterizerStateBuilder final : public IRasterizerStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GLRasterizerStateBuilder);
    DEFAULT_DESTRUCT(GLRasterizerStateBuilder);
    DEFAULT_CM_PO(GLRasterizerStateBuilder);
public:
    static GLenum glCullMode(RasterizerArgs::CullMode cullMode) noexcept;
    static GLenum glFillMode(RasterizerArgs::FillMode fillMode) noexcept;
public:
    struct GLRasterizerArgs final
    {
        GLenum frontFace;
        GLenum cullMode;
        GLenum fillMode;
        GLfloat polygonOffsetFactor;
        GLfloat polygonOffsetUnits;
        GLfloat polygonOffsetClamp;
    };
public:
    [[nodiscard]] GLRasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLRasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IRasterizerState> buildCPPRef(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> buildTauRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IRasterizerState> buildTauSRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const RasterizerArgs& args, [[tau::out]] GLRasterizerArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
