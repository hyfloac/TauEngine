#pragma once

#include "graphics/VertexArray.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12VertexArray final : public IVertexArray
{
    DEFAULT_DESTRUCT(DX12VertexArray);
    DEFAULT_CM_PU(DX12VertexArray);
    VERTEX_ARRAY_IMPL(DX12VertexArray);
private:
    DynArray<D3D12_VERTEX_BUFFER_VIEW> _d3dBuffers;
public:
    DX12VertexArray(const DynArray<NullableRef<IResource>>& buffers, const DynArray<D3D12_VERTEX_BUFFER_VIEW>& d3dBuffers) noexcept
        : IVertexArray(buffers)
        , _d3dBuffers(d3dBuffers)
    { }

    DX12VertexArray(const DynArray<NullableRef<IResource>>& buffers, DynArray<D3D12_VERTEX_BUFFER_VIEW>&& d3dBuffers) noexcept
        : IVertexArray(buffers)
        , _d3dBuffers(::std::move(d3dBuffers))
    { }

    DX12VertexArray(DynArray<NullableRef<IResource>>&& buffers, const DynArray<D3D12_VERTEX_BUFFER_VIEW>& d3dBuffers) noexcept
        : IVertexArray(::std::move(buffers))
        , _d3dBuffers(d3dBuffers)
    { }

    DX12VertexArray(DynArray<NullableRef<IResource>>&& buffers, DynArray<D3D12_VERTEX_BUFFER_VIEW>&& d3dBuffers) noexcept
        : IVertexArray(::std::move(buffers))
        , _d3dBuffers(::std::move(d3dBuffers))
    { }

    [[nodiscard]] const DynArray<D3D12_VERTEX_BUFFER_VIEW>& d3dBuffers() const noexcept { return _d3dBuffers; }
};

class TAU_DLL DX12VertexArrayBuilder final : public IVertexArrayBuilder
{
private:
    struct DXVertexArrayArgs final
    {
        DynArray<NullableRef<IResource>> buffers;
        DynArray<D3D12_VERTEX_BUFFER_VIEW> d3dBuffers;
    };
public:
    [[nodiscard]] DX12VertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] DX12VertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, [[tau::out]] DXVertexArrayArgs* dxArgs, [[tau::out]] Error* error) noexcept;
};
#endif
