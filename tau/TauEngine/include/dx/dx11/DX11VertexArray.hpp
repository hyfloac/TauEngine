#pragma once

#include "model/VertexArray.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "model/BufferDescriptor.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;
class DX11IndexBuffer;

struct DXVertexArrayArgs final
{
    DELETE_COPY(DXVertexArrayArgs);
public:
    CPPRef<DX11IndexBuffer> indexBuffer;
    ID3D11Buffer** iaBuffers;
    UINT* iaStrides;
    UINT* iaOffsets;
    ID3D11InputLayout* inputLayout;

    DXVertexArrayArgs() noexcept
        : indexBuffer(nullptr)
        , iaBuffers(nullptr)
        , iaStrides(nullptr)
        , iaOffsets(nullptr)
        , inputLayout(nullptr)
    { }

    ~DXVertexArrayArgs() noexcept
    {
        delete[] iaBuffers;
        delete[] iaStrides;
        delete[] iaOffsets;
        if(inputLayout)
        { inputLayout->Release(); }
    }
};

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
    CPPRef<DX11IndexBuffer> _indexBuffer;
    D3D11_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX11VertexArray(const VertexArrayArgs& args, const DXVertexArrayArgs& dxArgs) noexcept
        : IVertexArray(args.drawCount, args.buffers)
        , _inputLayout(dxArgs.inputLayout)
        , _iaBufferCount(args.buffers.count())
        , _iaBuffers(dxArgs.iaBuffers)
        , _iaStrides(dxArgs.iaStrides)
        , _iaOffsets(dxArgs.iaOffsets)
        , _indexBuffer(dxArgs.indexBuffer)
        , _drawTypeCache(getDXDrawType(args.drawType))
    { }

    ~DX11VertexArray() noexcept
    {
        delete[] _iaBuffers;
        delete[] _iaStrides;
        delete[] _iaOffsets;
        _inputLayout->Release();
    }

    void bind(IRenderingContext& context) noexcept override { }
    void unbind(IRenderingContext& context) noexcept override { }

    void preDraw(IRenderingContext& context) noexcept override;
    void postDraw(IRenderingContext& context) noexcept override { }

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class TAU_DLL DX11VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_DESTRUCT(DX11VertexArrayBuilder);
    DELETE_COPY(DX11VertexArrayBuilder);
public:
private:
    DX11GraphicsInterface& _gi;
public:
    DX11VertexArrayBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11VertexArray* build(const VertexArrayArgs& args, Error* error) noexcept override;
    [[nodiscard]] DX11VertexArray* build(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
