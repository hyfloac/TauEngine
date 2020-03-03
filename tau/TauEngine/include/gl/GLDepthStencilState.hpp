#pragma once

#include "graphics/DepthStencilState.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

struct GLDepthStencilParams final
{
    DEFAULT_CONSTRUCT_PU(GLDepthStencilParams);
    DEFAULT_DESTRUCT(GLDepthStencilParams);
    DEFAULT_COPY(GLDepthStencilParams);
public:
    struct GLStencilOpParams final
    {
        DEFAULT_CONSTRUCT_PU(GLStencilOpParams);
        DEFAULT_DESTRUCT(GLStencilOpParams);
        DEFAULT_COPY(GLStencilOpParams);
    public:
        GLenum failOp;
        GLenum stencilPassDepthFailOp;
        GLenum passOp;
        GLenum compareFunc;
    public:
        GLStencilOpParams(const GLenum failOp, const GLenum stencilPassDepthFailOp, const GLenum passOp, const GLenum compareFunc) noexcept
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

    GLStencilOpParams frontFace;
    GLStencilOpParams backFace;
public:
    GLDepthStencilParams(void(* const GLAPIENTRY depthTestControl)(GLenum), void(* const GLAPIENTRY stencilTestControl)(GLenum), const GLboolean depthWriteMask, const GLenum depthCompareFunc, const GLuint stencilReadMask, const GLuint stencilWriteMask, const GLStencilOpParams& frontFace, const GLStencilOpParams& backFace) noexcept
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
    GLDepthStencilParams _glParams;
public:
    GLDepthStencilState(const DepthStencilParams& params, const GLDepthStencilParams& glParams) noexcept
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
    [[nodiscard]] static GLenum glStencilOperation(DepthStencilParams::StencilOp stencilOp) noexcept;
public:
    [[nodiscard]] GLDepthStencilState* build(const DepthStencilParams& args, Error* error) const noexcept override;
    [[nodiscard]] GLDepthStencilState* build(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilParams& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> buildTauRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    static bool processArgs(const DepthStencilParams& args, [[tau::out]] GLDepthStencilParams* glArgs, [[tau::out]] Error* error) noexcept;
};
