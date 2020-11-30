#pragma once

#include "graphics/VertexArray.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "graphics/BufferDescriptor.hpp"

namespace EGraphics {
    enum class DrawType;
}

class DX10RenderingContext;
class DX10IndexBuffer;

struct DXVertexArrayArgs final
{
    DELETE_CM(DXVertexArrayArgs);
public:
    DynArray<NullableRef<IResource>> buffers;
    ID3D10Buffer** iaBuffers;

    DXVertexArrayArgs(const uSys bufferCount) noexcept
        : buffers(bufferCount)
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
    VERTEX_ARRAY_IMPL(DX10VertexArray);
private:
    uSys _iaBufferCount;
    ID3D10Buffer** _iaBuffers;
public:
    DX10VertexArray(const VertexArrayArgs& args, const DXVertexArrayArgs& dxArgs) noexcept
        : IVertexArray(dxArgs.buffers)
        , _iaBufferCount(args.bufferCount)
        , _iaBuffers(dxArgs.iaBuffers)
    { }

    ~DX10VertexArray() noexcept override
    {
        delete[] _iaBuffers;
    }

    [[nodiscard]] uSys iaBufferCount() const noexcept { return _iaBufferCount; }

    [[nodiscard]] ID3D10Buffer** iaBuffers() const noexcept { return _iaBuffers; }
};

class TAU_DLL DX10VertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10VertexArrayBuilder);
    DEFAULT_DESTRUCT(DX10VertexArrayBuilder);
    DEFAULT_CM_PU(DX10VertexArrayBuilder);
public:
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
