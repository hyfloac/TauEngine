#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d10.h>
#include <DirectXMath.h>
#pragma warning(pop)

#include "DLL.hpp"

class DX10ShaderProgram;

class TAU_DLL DX10RenderingContext final : public IRenderingContext
{
private:
    ID3D10Device* _d3d10Device;
    ID3D10RenderTargetView* _renderTargetView;
    ID3D10Texture2D* _depthStencilBuffer;
    ID3D10DepthStencilState* _depthStencilState;
    ID3D10DepthStencilView* _depthStencilView;
    ID3D10RasterizerState* _rasterizerState;
    IDXGISwapChain* _swapChain;

    bool _vsync;

    ID3D10Blob* _tmpShaderBlob;
    ID3D10InputLayout* _tmpInputLayout;
public:
    DX10RenderingContext(const RenderingMode& mode) noexcept;

    ~DX10RenderingContext() noexcept override;

    [[nodiscard]] const ID3D10Device* d3d10Device() const noexcept { return _d3d10Device; }
    [[nodiscard]] ID3D10Device* d3d10Device() noexcept { return _d3d10Device; }

    [[nodiscard]] const ID3D10Blob* tmpShaderBlob() const noexcept { return _tmpShaderBlob; }
    [[nodiscard]] ID3D10Blob*& tmpShaderBlob() noexcept { return _tmpShaderBlob; }

    [[nodiscard]] const ID3D10InputLayout* tmpInputLayout() const noexcept { return _tmpInputLayout; }
    [[nodiscard]] ID3D10InputLayout*& tmpInputLayout() noexcept { return _tmpInputLayout; }

    [[nodiscard]] bool createContext(Window& window) noexcept override;
    void createFromShared(void* param) noexcept override { }
    void deactivateContext() noexcept override { }
    void activateContext() noexcept override { }
    [[nodiscard]] Ref<IVertexArray> createVertexArray(std::size_t attribCount, DrawType drawType) noexcept override { return null; }
    [[nodiscard]] void* getVertexArrayHandle(IVertexArray* vertexArray) noexcept override { return null; }
    void destroyVA(IVertexArray* vertexArray) noexcept override { }
    void clearVAs() noexcept override { }
    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept override;
    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, UINT8 stencilValue) noexcept override;
    void setVSync(bool vsync) noexcept override;
    void setFaceWinding(bool clockwise) noexcept override;
    void enableDepthWriting(bool writing) noexcept override;
    void beginFrame() noexcept override;
    void endFrame() noexcept override;
    void swapFrame() noexcept override;

    [[nodiscard]] Ref<IBufferBuilder> createBuffer(std::size_t descriptorCount) noexcept override { return null; }
    [[nodiscard]] Ref<IIndexBufferBuilder> createIndexBuffer() noexcept override { return null; }
    [[nodiscard]] Ref<IFrameBufferBuilder> createFrameBuffer() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createTexture2D() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createNullTexture() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createTextureDepth() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureCubeBuilder> createTextureCube() noexcept override { return null; }
    [[nodiscard]] Ref<IShaderBuilder> createShader() noexcept override;
protected:
    bool createContextsShared(Window& window, IRenderingContext** sharers, std::size_t count) noexcept override { return true; }

    RC_IMPL(DX10RenderingContext);
};

#endif
