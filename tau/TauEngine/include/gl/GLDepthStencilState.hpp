#pragma once

#include "graphics/DepthStencilState.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class GLStateManager;

struct GLDepthStencilArgs final
{
    DEFAULT_CONSTRUCT_PU(GLDepthStencilArgs);
    DEFAULT_DESTRUCT(GLDepthStencilArgs);
    DEFAULT_CM_PU(GLDepthStencilArgs);
public:
    struct GLStencilOpArgs final
    {
        DEFAULT_CONSTRUCT_PU(GLStencilOpArgs);
        DEFAULT_DESTRUCT(GLStencilOpArgs);
        DEFAULT_CM_PU(GLStencilOpArgs);
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
    bool enableDepthTest;
    bool enableStencilTest;

    GLboolean depthWriteMask;
    GLenum depthCompareFunc;

    GLuint stencilReadMask;
    GLuint stencilWriteMask;

    GLStencilOpArgs frontFace;
    GLStencilOpArgs backFace;
public:
    GLDepthStencilArgs(const bool _enableDepthTest, const bool _enableStencilTest, const GLboolean _depthWriteMask, const GLenum _depthCompareFunc, const GLuint _stencilReadMask, const GLuint _stencilWriteMask, const GLStencilOpArgs& _frontFace, const GLStencilOpArgs& _backFace) noexcept
        : enableDepthTest(_enableDepthTest)
        , enableStencilTest(_enableStencilTest)
        , depthWriteMask(_depthWriteMask)
        , depthCompareFunc(_depthCompareFunc)
        , stencilReadMask(_stencilReadMask)
        , stencilWriteMask(_stencilWriteMask)
        , frontFace(_frontFace)
        , backFace(_backFace)
    { }
};

class GLDepthStencilState final : public IDepthStencilState
{
    DEFAULT_DESTRUCT(GLDepthStencilState);
    DEFAULT_CM_PU(GLDepthStencilState);
    DSS_IMPL(GLDepthStencilState);
private:
    GLDepthStencilArgs _glArgs;
public:
    GLDepthStencilState(const DepthStencilArgs& args, const GLDepthStencilArgs& glArgs) noexcept
        : IDepthStencilState(args)
        , _glArgs(glArgs)
    { }

    void apply(GLStateManager& glStateHelper) const noexcept;
};

class GLDepthStencilStateBuilder final : public IDepthStencilStateBuilder
{
    DEFAULT_CONSTRUCT_PU(GLDepthStencilStateBuilder);
    DEFAULT_DESTRUCT(GLDepthStencilStateBuilder);
    DEFAULT_CM_PU(GLDepthStencilStateBuilder);
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
