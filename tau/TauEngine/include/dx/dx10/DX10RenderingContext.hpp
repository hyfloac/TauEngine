#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d10.h>
#pragma warning(pop)

#include "DLL.hpp"

class DX10VertexArrayBuilder;
class DX10BufferBuilder;
class DX10IndexBufferBuilder;
class DX10UniformBufferBuilder;
class DX10TextureSamplerBuilder;
class DX10ShaderBuilder;
class DX10Texture2DBuilder;
class DX10NullTextureBuilder;
class DX10TextureCubeBuilder;
class DX10DepthStencilState;
class DX10RasterizerState;
class DX10DepthStencilStateBuilder;
class DX10GraphicsInterface;

struct DX10RenderingContextArgs final
{
    ID3D10RenderTargetView* renderTargetView;
    ID3D10Texture2D* depthStencilBuffer;
    ID3D10DepthStencilView* depthStencilView;
    IDXGISwapChain* swapChain;
};

class TAU_DLL DX10RenderingContext final : public IRenderingContext
{
private:
    DX10GraphicsInterface& _gi;

    ID3D10RenderTargetView* _renderTargetView;
    ID3D10Texture2D* _depthStencilBuffer;
    ID3D10DepthStencilView* _depthStencilView;
    IDXGISwapChain* _swapChain;

    bool _vsync;

    NullableRef<DX10DepthStencilState> _defaultDepthStencilState;
    NullableRef<DX10DepthStencilState> _currentDepthStencilState;
    NullableRef<DX10RasterizerState> _defaultRasterizerState;
    NullableRef<DX10RasterizerState> _currentRasterizerState;

    DX10VertexArrayBuilder* _vertexArrayBuilder;
    DX10BufferBuilder* _bufferBuilder;
    DX10IndexBufferBuilder* _indexBufferBuilder;
    DX10UniformBufferBuilder* _uniformBufferBuilder;
    DX10TextureSamplerBuilder* _textureSamplerBuilder;
    DX10Texture2DBuilder* _texture2DBuilder;
    DX10NullTextureBuilder* _textureNullBuilder;
    DX10TextureCubeBuilder* _textureCubeBuilder;
public:
    DX10RenderingContext(DX10GraphicsInterface& gi, const DX10RenderingContextArgs& args) noexcept;

    ~DX10RenderingContext() noexcept override;

    [[nodiscard]] const ID3D10Device* d3dDevice() const noexcept;
    [[nodiscard]] ID3D10Device* d3dDevice() noexcept;

    [[nodiscard]] bool createContext(Window& window) noexcept override;
    void deactivateContext() noexcept override { }
    void activateContext() noexcept override { }
    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept override;
    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept override;
    void setVSync(bool vsync) noexcept override;

    NullableRef<IDepthStencilState> setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void resetDepthStencilState() noexcept override;
    const DepthStencilArgs& getDefaultDepthStencilArgs() noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> getDefaultDepthStencilState() noexcept override;

    NullableRef<IRasterizerState> setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void resetRasterizerState() noexcept override;
    const RasterizerArgs& getDefaultRasterizerArgs() noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> getDefaultRasterizerState() noexcept override;

    void beginFrame() noexcept override;
    void endFrame() noexcept override;
    void swapFrame() noexcept override;

    [[nodiscard]] IVertexArrayBuilder& createVertexArray() noexcept override;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override;
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override;
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override;
    [[nodiscard]] CPPRef<IFrameBufferBuilder> createFrameBuffer() noexcept override { return null; }
    [[nodiscard]] ITextureBuilder& createTexture2D() noexcept override;
    [[nodiscard]] ITextureBuilder& createNullTexture() noexcept override;
    [[nodiscard]] ITextureBuilder& createTextureDepth() noexcept override;
    [[nodiscard]] ITextureCubeBuilder& createTextureCube() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] CPPRef<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept override;
    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
protected:
    RC_IMPL(DX10RenderingContext);
};

class TAU_DLL DX10RenderingContextBuilder final : public IRenderingContextBuilder
{
    DEFAULT_DESTRUCT(DX10RenderingContextBuilder);
    DELETE_COPY(DX10RenderingContextBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10RenderingContextBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10RenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] DX10RenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IRenderingContext> buildCPPRef(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IRenderingContext> buildTauRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
    [[nodiscard]] NullableStrongRef<IRenderingContext> buildTauSRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
private:
    [[nodiscard]] bool processArgs(const RenderingContextArgs& args, [[tau::out]] DX10RenderingContextArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
