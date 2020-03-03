#pragma once

#include "model/VertexArray.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "model/BufferDescriptor.hpp"

class DX10RenderingContext;
class DX10InputLayout;

class TAU_DLL DX10VertexArray final : public IVertexArray
{
    DELETE_COPY(DX10VertexArray);
public:
    static DXGI_FORMAT getDXType(ShaderDataType::Type type) noexcept;

    /**
     *   DirectX doesn't natively allow you to pass doubles or
     * matrices to a shader. As such these types take up more
     * than one element in the input layout.
     */
    static uSys computeNumElements(ShaderDataType::Type type) noexcept;

    static D3D10_PRIMITIVE_TOPOLOGY getDXDrawType(DrawType drawType) noexcept;

    static const char* getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept;
private:
    ID3D10InputLayout* _inputLayout;
    uSys _iaBufferCount;
    ID3D10Buffer** _iaBuffers;
    UINT* _iaStrides;
    UINT* _iaOffsets;
    ID3D10Buffer* _dxIndexBuffer;
    D3D10_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX10VertexArray(u32 drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers, ID3D10InputLayout* inputLayout, uSys bufferCount, ID3D10Buffer** iaBuffers,
                    UINT* iaStrides, UINT* iaOffsets, ID3D10Buffer* indexBuffer, DrawType drawType) noexcept;

    ~DX10VertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    // void internalSetup(IRenderingContext& context) noexcept override;
    void preDraw(IRenderingContext& context) noexcept override;
    void postDraw(IRenderingContext& context) noexcept override;
    void draw(IRenderingContext& context) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount) noexcept override;
private:
    // void handleInsertion(uSys& insertIndex, ID3D10Buffer* buffer, const BufferElementDescriptor& bed) const noexcept;
};

class TAU_DLL DX10VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_DESTRUCT(DX10VertexArrayBuilder);
    DELETE_COPY(DX10VertexArrayBuilder);
private:
    DX10RenderingContext& _ctx;
    ID3D10Buffer* _indexBufferCache;
    ID3D10Blob* _shaderBlobCache;
public:
    DX10VertexArrayBuilder(const uSys bufferCount, DX10RenderingContext& ctx) noexcept
        : IVertexArrayBuilder(bufferCount), _ctx(ctx), _indexBufferCache(null), _shaderBlobCache(null)
    { }

    void shader(const CPPRef<IShader>& shader) noexcept override;
    void setVertexBuffer(uSys index, const CPPRef<IBuffer>& vertexBuffer) noexcept override;
    void indexBuffer(const CPPRef<IIndexBuffer>& indexBuffer) noexcept override;
    // void inputLayout(const CPPRef<IInputLayout>& inputLayout) noexcept override;

    [[nodiscard]] DX10VertexArray* build([[tau::out]] Error* error) noexcept override;
};
#endif
