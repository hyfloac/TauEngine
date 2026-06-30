#pragma once

#include "GLBuffer.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "../graphics/GraphicsInterface.hpp"
#include "GLGraphicsCapabilities.hpp"
#include <TauCOM.impl.hpp>

class GLShaderBuilder;
class GLShaderProgramBuilder;
class GLResourceBuilder;
class GLInputLayoutBuilder;
class GLVertexArrayBuilder;
class GLBufInterface;
class GLDepthStencilStateBuilder;
class GLRasterizerStateBuilder;
class GLBlendingStateBuilder;
class GLSingleTextureUploaderBuilder;
class GLTextureSamplerBuilder;
class GLTextureUploaderBuilder;
class GLFrameBufferBuilder;
class GLRenderingContextBuilder;

namespace tau::gl {

class GLGraphicsInterface final : public graphics::IGraphicsInterface
{
    DELETE_CM(GLGraphicsInterface);
    TAU_COM_IMPL_REF_COUNT_CUSTOM();
// private:
//     ::std::atomic<::std::int32_t> m_AutoRefCount = 1;
// public:
//     ::std::int32_t AddReference() noexcept override final { return ++m_AutoRefCount; }
//     ::std::int32_t ReleaseReference() noexcept override final
//     {
//         const ::std::int32_t ret = m_AutoRefCount;
//         if((--m_AutoRefCount) == 0)
//         {
//             DestroySelf();
//         }
//         return ret;
//     };
public:
    enum class GLProfile
    {
        Core = 0,
        Compat,
        Neither
    };
public:
    GLGraphicsInterface(const C8DynString& mode, int majorVersion, int minorVersion, GLProfile compat, bool forwardCompatible);
    ~GLGraphicsInterface() noexcept override;

    [[nodiscard]] int majorVersion() const noexcept { return _majorVersion; }
    [[nodiscard]] int minorVersion() const noexcept { return _minorVersion; }
    [[nodiscard]] GLProfile compat() const noexcept { return _compat; }
    [[nodiscard]] bool forwardCompatible() const noexcept { return _forwardCompatible; }

    // IUnknown
    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override;

    // ITauObject
    com::EResultCode GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept override;
    com::EResultCode SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept override;
    com::EResultCode SetPrivateDataInterface(const com::UUID& uuid, IUnknown* pInterface) noexcept override;
    com::EResultCode SetName(const C8DynString& name) noexcept override;

    // IGraphicsInterface
    [[nodiscard]] const C8DynString& RenderingMode() const noexcept override;

    [[nodiscard]] GLGraphicsCapabilities& Capabilities() noexcept override { return _graphicsCapabilities; }

    com::EResultCode CreateCommandList(
        graphics::ICommandList** pCommandList,
        const graphics::ICommandList::ConstructionInfo* constructionInfo
    ) override;

    // [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    // [[nodiscard]] IShaderProgramBuilder& createShaderProgram() noexcept override;
    // [[nodiscard]] IResourceBuilder& createResource() noexcept override;
    // [[nodiscard]] IInputLayoutBuilder& createInputLayout() noexcept override;
    // [[nodiscard]] IVertexArrayBuilder& createVertexArray() noexcept override;
    // [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    // [[nodiscard]] IBlendingStateBuilder& createBlendingState() noexcept override;
    // [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    // [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    // // TODO: port to ICommandQueue upload path
    // // [[nodiscard]] ITextureUploaderBuilder& createTextureUploader() noexcept override;
    // [[nodiscard]] IFrameBufferBuilder& createFrameBuffer() noexcept override;
    // [[nodiscard]] IRenderingContextBuilder& createRenderingContext() noexcept override;
private:
    void DestroySelf() noexcept;
private:
    int _majorVersion;
    int _minorVersion;
    GLProfile _compat;
    bool _forwardCompatible;

    // ShaderInfoExtractorVisitor _shaderInfoExtractor;

    GLGraphicsCapabilities _graphicsCapabilities;

    //     GLShaderBuilder* _shaderBuilder;
    //     GLShaderProgramBuilder* _shaderProgramBuilder;
    //     GLResourceBuilder* _resourceBuilder;
    //     GLInputLayoutBuilder* _inputLayoutBuilder;
    //     GLVertexArrayBuilder* _vertexArrayBuilder;
    //     GLBufInterface* _bufInterface;
    //     GLDepthStencilStateBuilder* _depthStencilStateBuilder;
    //     GLRasterizerStateBuilder* _rasterizerStateBuilder;
    //     GLBlendingStateBuilder* _blendingStateBuilder;
    //     GLTextureSamplerBuilder* _textureSamplerBuilder;
    // #if 0 // TODO: port to ICommandQueue upload path
    //     GLTextureUploaderBuilder* _textureUploaderBuilder;
    // #endif
    //     GLFrameBufferBuilder* _frameBufferBuilder;
    //     GLRenderingContextBuilder* _renderingContextBuilder;
};

}

struct GLGraphicsInterfaceArgs final
{
    RenderingMode mode;
    int majorVersion;
    int minorVersion;
    tau::gl::GLGraphicsInterface::GLProfile compat;
    bool forwardCompatible;
};

class GLGraphicsInterfaceBuilder final
{
    DEFAULT_CONSTRUCT_PU(GLGraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT(GLGraphicsInterfaceBuilder);
    DEFAULT_CM(GLGraphicsInterfaceBuilder);
public:
    [[nodiscard]] static NullableRef<GLGraphicsInterface> build(const GLGraphicsInterfaceArgs& args, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept;
};
