#pragma once

#include "system/GraphicsInterface.hpp"

class GLShaderBuilder;
class GLDepthStencilStateBuilder;
class GLRasterizerStateBuilder;
class GLRenderingContextBuilder;

class TAU_DLL GLGraphicsInterface final : public IGraphicsInterface
{
    DELETE_COPY(GLGraphicsInterface);
public:
    enum class GLProfile
    {
        Core = 0,
        Compat,
        Neither
    };
private:
    int _majorVersion;
    int _minorVersion;
    GLProfile _compat;
    bool _forwardCompatible;

    GLShaderBuilder* _shaderBuilder;
    GLDepthStencilStateBuilder* _depthStencilStateBuilder;
    GLRasterizerStateBuilder* _rasterizerStateBuilder;
    GLRenderingContextBuilder* _renderingContextBuilder;
public:
    GLGraphicsInterface(const RenderingMode& mode, int majorVersion, int minorVersion, GLProfile compat, bool forwardCompatible);
    ~GLGraphicsInterface() noexcept;

    [[nodiscard]] inline int majorVersion() const noexcept { return _majorVersion; }
    [[nodiscard]] inline int minorVersion() const noexcept { return _minorVersion; }
    [[nodiscard]] inline GLProfile compat() const noexcept { return _compat; }
    [[nodiscard]] inline bool forwardCompatible() const noexcept { return _forwardCompatible; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] IRenderingContextBuilder& createRenderingContext() noexcept override;
};

struct GLGraphicsInterfaceArgs final
{
    RenderingMode mode;
    int majorVersion;
    int minorVersion;
    GLGraphicsInterface::GLProfile compat;
    bool forwardCompatible;
};

class GLGraphicsInterfaceBuilder final
{
    DEFAULT_CONSTRUCT_PU(GLGraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT(GLGraphicsInterfaceBuilder);
    DEFAULT_COPY(GLGraphicsInterfaceBuilder);
public:
    [[nodiscard]] static NullableRef<GLGraphicsInterface> build(const GLGraphicsInterfaceArgs& args, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept;
};
