#pragma once

#include "model/VertexArray.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "model/BufferDescriptor.hpp"

class DX10RenderingContext;
class DX10IndexBuffer;
class DX10GraphicsInterface;

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
    CPPRef<DX10IndexBuffer> _indexBuffer;
    D3D10_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX10VertexArray(uSys drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers, ID3D10InputLayout* inputLayout, uSys bufferCount, ID3D10Buffer** iaBuffers,
                    UINT* iaStrides, UINT* iaOffsets, const CPPRef<DX10IndexBuffer>& indexBuffer, DrawType drawType) noexcept;

    ~DX10VertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override { }
    void unbind(IRenderingContext& context) noexcept override { }

    void preDraw(IRenderingContext& context) noexcept override;
    void postDraw(IRenderingContext& context) noexcept override { }

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class TAU_DLL DX10VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_DESTRUCT(DX10VertexArrayBuilder);
    DELETE_COPY(DX10VertexArrayBuilder);
public:
    struct DXVertexArrayArgs final
    {
        DELETE_COPY(DXVertexArrayArgs);
    public:

        CPPRef<DX10IndexBuffer> indexBuffer;
        ID3D10Buffer** iaBuffers;
        UINT* iaStrides;
        UINT* iaOffsets;
        ID3D10InputLayout* inputLayout;

        DXVertexArrayArgs() noexcept
            : indexBuffer(nullptr), iaBuffers(nullptr), iaStrides(nullptr), iaOffsets(nullptr),
              inputLayout(nullptr)
        { }

        ~DXVertexArrayArgs() noexcept
        {
            delete[] iaBuffers;
            delete[] iaStrides;
            delete[] iaOffsets;
            if(inputLayout)
            {
                inputLayout->Release();
            }
        }
    };
private:
    DX10GraphicsInterface& _gi;
public:
    DX10VertexArrayBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10VertexArray* build([[tau::out]] Error* error) noexcept;

    [[nodiscard]] DX10VertexArray* build(const VertexArrayArgs& args, Error* error) noexcept override;
    [[nodiscard]] DX10VertexArray* build(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
