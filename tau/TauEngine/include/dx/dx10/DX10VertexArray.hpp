#pragma once

#include "graphics/VertexArray.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "graphics/BufferDescriptor.hpp"

class DX10RenderingContext;
class DX10IndexBuffer;

struct DXVertexArrayArgs final
{
    DELETE_CM(DXVertexArrayArgs);
public:
    NullableRef<DX10IndexBuffer> indexBuffer;
    ID3D10Buffer** iaBuffers;

    DXVertexArrayArgs() noexcept
        : indexBuffer(nullptr)
        , iaBuffers(nullptr)
    { }

    ~DXVertexArrayArgs() noexcept
    {
        delete[] iaBuffers;
    }
};

class TAU_DLL DX10VertexArray final : public IVertexArray
{
    DELETE_CM(DX10VertexArray);
public:
    static D3D10_PRIMITIVE_TOPOLOGY getDXDrawType(DrawType drawType) noexcept;
private:
    uSys _iaBufferCount;
    ID3D10Buffer** _iaBuffers;
    NullableRef<DX10IndexBuffer> _indexBuffer;
    D3D10_PRIMITIVE_TOPOLOGY _drawTypeCache;
public:
    DX10VertexArray(const VertexArrayArgs& args, const DXVertexArrayArgs& dxArgs) noexcept
        : IVertexArray(args.drawCount, args.buffers)
        , _iaBufferCount(args.buffers.count())
        , _iaBuffers(dxArgs.iaBuffers)
        , _indexBuffer(dxArgs.indexBuffer)
        , _drawTypeCache(getDXDrawType(args.drawType))
    { }

    ~DX10VertexArray() noexcept
    {
        delete[] _iaBuffers;
    }

    [[nodiscard]] uSys iaBufferCount() const noexcept { return _iaBufferCount; }

    [[nodiscard]] ID3D10Buffer** iaBuffers() const noexcept { return _iaBuffers; }

    [[nodiscard]] const NullableRef<DX10IndexBuffer>& indexBuffer() const noexcept { return _indexBuffer; }

    [[nodiscard]] D3D10_PRIMITIVE_TOPOLOGY drawTypeCache() const noexcept { return _drawTypeCache; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class TAU_DLL DX10VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10VertexArrayBuilder);
    DEFAULT_DESTRUCT(DX10VertexArrayBuilder);
    DEFAULT_CM_PU(DX10VertexArrayBuilder);
public:
    [[nodiscard]] DX10VertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] DX10VertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
