#pragma once

#include "system/GraphicsInterface.hpp"

class GLShaderBuilder;
class GLVertexArrayBuilder;
class GLBufferBuilder;
class GLIndexBufferBuilder;
class GLUniformBufferBuilder;
class GLDepthStencilStateBuilder;
class GLRasterizerStateBuilder;
class GLBlendingStateBuilder;
class GLTextureBuilder;
class GLSingleTextureUploaderBuilder;
class GLTextureSamplerBuilder;
class GLTextureUploaderBuilder;
class GLFrameBufferBuilder;
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
    GLVertexArrayBuilder* _vertexArrayBuilder;
    GLBufferBuilder* _bufferBuilder;
    GLIndexBufferBuilder* _indexBufferBuilder;
    GLUniformBufferBuilder* _uniformBufferBuilder;
    GLDepthStencilStateBuilder* _depthStencilStateBuilder;
    GLRasterizerStateBuilder* _rasterizerStateBuilder;
    GLBlendingStateBuilder* _blendingStateBuilder;
    GLTextureBuilder* _textureBuilder;
    GLTextureSamplerBuilder* _textureSamplerBuilder;
    GLSingleTextureUploaderBuilder* _singleTextureUploaderBuilder;
    GLTextureUploaderBuilder* _textureUploaderBuilder;
    GLFrameBufferBuilder* _frameBufferBuilder;
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
    [[nodiscard]] IVertexArrayBuilder& createVertexArray() noexcept override;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override;
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override;
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IBlendingStateBuilder& createBlendingState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] ITextureBuilder& createTexture() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] ISingleTextureUploaderBuilder& createSingleTextureUploader() noexcept override;
    [[nodiscard]] ITextureUploaderBuilder& createTextureUploader() noexcept override;
    [[nodiscard]] IFrameBufferBuilder& createFrameBuffer() noexcept override;
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
