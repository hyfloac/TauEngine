#pragma once

#include "graphics/RasterizerState.hpp"

#include <GL/glew.h>

class TAU_DLL GLRasterizerState final : public IRasterizerState
{
    DEFAULT_DESTRUCT(GLRasterizerState);
    RS_IMPL(GLRasterizerState);
private:
    GLenum _frontFace;
    GLenum _cullMode;
    GLenum _fillMode;
public:
    GLRasterizerState(const RasterizerArgs& params, const GLenum frontFace, const GLenum cullMode, const GLenum fillMode) noexcept
        : IRasterizerState(params), _frontFace(frontFace), _cullMode(cullMode), _fillMode(fillMode)
    { }

    void apply() const noexcept;
};

class TAU_DLL GLRasterizerStateBuilder final : public IRasterizerStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GLRasterizerStateBuilder);
    DEFAULT_DESTRUCT(GLRasterizerStateBuilder);
    DELETE_COPY(GLRasterizerStateBuilder);
public:
    static GLenum glCullMode(RasterizerArgs::CullMode cullMode) noexcept;
    static GLenum glFillMode(RasterizerArgs::FillMode fillMode) noexcept;
public:
    struct GLRasterizerArgs final
    {
        GLenum frontFace;
        GLenum cullMode;
        GLenum fillMode;
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
