#pragma once

#include "model/VertexArray.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "model/BufferDescriptor.hpp"

class DX11RenderingContext;
class DX11InputLayout;

class TAU_DLL DX11VertexArray final : public IVertexArray
{
    DELETE_COPY(DX11VertexArray);
public:
    static DXGI_FORMAT getDXType(ShaderDataType::Type type) noexcept;

    /**
     *   DirectX doesn't natively allow you to pass doubles or
     * matrices to a shader. As such these types take up more
     * than one element in the input layout.
     */
    static uSys computeNumElements(ShaderDataType::Type type) noexcept;

    static D3D11_PRIMITIVE_TOPOLOGY getDXDrawType(DrawType drawType) noexcept;

    static const char* getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept;
private:
    ID3D11InputLayout* _inputLayout;
    uSys _iaBufferCount;
    ID3D11Buffer** _iaBuffers;
    UINT* _iaStrides;
    UINT* _iaOffsets;
    ID3D11Buffer* _dxIndexBuffer;
    D3D11_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX11VertexArray(u32 drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers, ID3D11InputLayout* inputLayout, uSys bufferCount, ID3D11Buffer** iaBuffers,
                    UINT* iaStrides, UINT* iaOffsets, ID3D11Buffer* indexBuffer, DrawType drawType) noexcept;

    ~DX11VertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void preDraw(IRenderingContext& context) noexcept override;
    void postDraw(IRenderingContext& context) noexcept override;
    void draw(IRenderingContext& context) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount) noexcept override;
};

class TAU_DLL DX11VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_DESTRUCT(DX11VertexArrayBuilder);
    DELETE_COPY(DX11VertexArrayBuilder);
private:
    DX11RenderingContext& _ctx;
    ID3D11Buffer* _indexBufferCache;
    ID3DBlob* _shaderBlobCache;
public:
    DX11VertexArrayBuilder(const uSys bufferCount, DX11RenderingContext& ctx) noexcept
        : IVertexArrayBuilder(bufferCount), _ctx(ctx), _indexBufferCache(null), _shaderBlobCache(null)
    { }

    void shader(const CPPRef<IShader>& shader) noexcept override;
    void setVertexBuffer(uSys index, const CPPRef<IBuffer>& vertexBuffer) noexcept override;
    void indexBuffer(const CPPRef<IIndexBuffer>& indexBuffer) noexcept override;

    [[nodiscard]] DX11VertexArray* build([[tau::out]] Error* error) noexcept override;
};
#endif
