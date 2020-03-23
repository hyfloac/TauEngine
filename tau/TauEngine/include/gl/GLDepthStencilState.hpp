#pragma once

#include "graphics/DepthStencilState.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

struct GLDepthStencilArgs final
{
    DEFAULT_CONSTRUCT_PU(GLDepthStencilArgs);
    DEFAULT_DESTRUCT(GLDepthStencilArgs);
    DEFAULT_COPY(GLDepthStencilArgs);
public:
    struct GLStencilOpArgs final
    {
        DEFAULT_CONSTRUCT_PU(GLStencilOpArgs);
        DEFAULT_DESTRUCT(GLStencilOpArgs);
        DEFAULT_COPY(GLStencilOpArgs);
    public:
        GLenum failOp;
        GLenum stencilPassDepthFailOp;
        GLenum passOp;
        GLenum compareFunc;
    public:
        GLStencilOpArgs(const GLenum failOp, const GLenum stencilPassDepthFailOp, const GLenum passOp, const GLenum compareFunc) noexcept
            : failOp(failOp), stencilPassDepthFailOp(stencilPassDepthFailOp),
              passOp(passOp), compareFunc(compareFunc)
        { }
    };
public:
    void (* GLAPIENTRY depthTestControl)(GLenum);
    void (* GLAPIENTRY stencilTestControl)(GLenum);

    GLboolean depthWriteMask;
    GLenum depthCompareFunc;

    GLuint stencilReadMask;
    GLuint stencilWriteMask;

    GLStencilOpArgs frontFace;
    GLStencilOpArgs backFace;
public:
    GLDepthStencilArgs(void(* const GLAPIENTRY depthTestControl)(GLenum), void(* const GLAPIENTRY stencilTestControl)(GLenum), const GLboolean depthWriteMask, const GLenum depthCompareFunc, const GLuint stencilReadMask, const GLuint stencilWriteMask, const GLStencilOpArgs& frontFace, const GLStencilOpArgs& backFace) noexcept
        : depthTestControl{ depthTestControl },
          stencilTestControl{ stencilTestControl },
          depthWriteMask(depthWriteMask),
          depthCompareFunc(depthCompareFunc),
          stencilReadMask(stencilReadMask),
          stencilWriteMask(stencilWriteMask),
          frontFace(frontFace),
          backFace(backFace)
    { }
};

class GLDepthStencilState final : public IDepthStencilState
{
    DEFAULT_DESTRUCT(GLDepthStencilState);
private:
    GLDepthStencilArgs _glParams;
public:
    GLDepthStencilState(const DepthStencilArgs& params, const GLDepthStencilArgs& glParams) noexcept
        : IDepthStencilState(params), _glParams(glParams)
    { }

    void apply() const noexcept;
protected:
    DSS_IMPL(GLDepthStencilState);
};

class GLDepthStencilStateBuilder final : public IDepthStencilStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GLDepthStencilStateBuilder);
    DEFAULT_DESTRUCT(GLDepthStencilStateBuilder);
    DELETE_COPY(GLDepthStencilStateBuilder);
public:
    [[nodiscard]] static GLenum glStencilOperation(DepthStencilArgs::StencilOp stencilOp) noexcept;
public:
    [[nodiscard]] GLDepthStencilState* build(const DepthStencilArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLDepthStencilState* build(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> buildTauRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    static bool processArgs(const DepthStencilArgs& args, [[tau::out]] GLDepthStencilArgs* glArgs, [[tau::out]] Error* error) noexcept;
};