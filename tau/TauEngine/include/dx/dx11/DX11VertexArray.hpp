#pragma once

#include "model/VertexArray.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "model/BufferDescriptor.hpp"

class DX11RenderingContext;
class DX11IndexBuffer;

struct DXVertexArrayArgs final
{
    DELETE_COPY(DXVertexArrayArgs);
public:
    NullableRef<DX11IndexBuffer> indexBuffer;
    ID3D11Buffer** iaBuffers;

    DXVertexArrayArgs() noexcept
        : indexBuffer(nullptr)
        , iaBuffers(nullptr)
    { }

    ~DXVertexArrayArgs() noexcept
    {
        delete[] iaBuffers;
    }
};

class TAU_DLL DX11VertexArray final : public IVertexArray
{
    DELETE_COPY(DX11VertexArray);
public:
    static D3D11_PRIMITIVE_TOPOLOGY getDXDrawType(DrawType drawType) noexcept;
private:
    uSys _iaBufferCount;
    ID3D11Buffer** _iaBuffers;
    NullableRef<DX11IndexBuffer> _indexBuffer;
    D3D11_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX11VertexArray(const VertexArrayArgs& args, const DXVertexArrayArgs& dxArgs) noexcept
        : IVertexArray(args.drawCount, args.buffers)
        , _iaBufferCount(args.buffers.count())
        , _iaBuffers(dxArgs.iaBuffers)
        , _indexBuffer(dxArgs.indexBuffer)
        , _drawTypeCache(getDXDrawType(args.drawType))
    { }

    ~DX11VertexArray() noexcept
    {
        delete[] _iaBuffers;
    }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class TAU_DLL DX11VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(DX11VertexArrayBuilder);
    DEFAULT_DESTRUCT(DX11VertexArrayBuilder);
    DELETE_COPY(DX11VertexArrayBuilder);
public:
    [[nodiscard]] DX11VertexArray* build(const VertexArrayArgs& args, Error* error) noexcept override;
    [[nodiscard]] DX11VertexArray* build(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
