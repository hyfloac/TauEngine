#pragma once

#include "graphics/DepthStencilState.hpp"

#ifdef _WIN32

#include <d3d10.h>

class DX10RenderingContext;

class DX10DepthStencilState final : public IDepthStencilState
{
private:
    ID3D10DepthStencilState* _d3dDepthStencilState;
public:
    DX10DepthStencilState(const DepthStencilParams& params, ID3D10DepthStencilState* const d3dDepthStencilState) noexcept
        : IDepthStencilState(params), _d3dDepthStencilState(d3dDepthStencilState)
    { }

    ~DX10DepthStencilState() noexcept
    {
        _d3dDepthStencilState->Release();
        _d3dDepthStencilState = null;
    }

    [[nodiscard]] const ID3D10DepthStencilState* d3dDepthStencilState() const noexcept { return _d3dDepthStencilState; }
    [[nodiscard]] ID3D10DepthStencilState* d3dDepthStencilState() noexcept { return _d3dDepthStencilState; }

    void apply(DX10RenderingContext& ctx) const noexcept;
protected:
    DSS_IMPL(DX10DepthStencilState);
};

class DX10DepthStencilStateBuilder final : public IDepthStencilStateBuilder
{
    DEFAULT_DESTRUCT(DX10DepthStencilStateBuilder);
    DELETE_COPY(DX10DepthStencilStateBuilder);
public:
    static D3D10_DEPTH_WRITE_MASK dxDepthWriteMask(DepthStencilParams::DepthWriteMask depthWriteMask) noexcept;
    static D3D10_COMPARISON_FUNC dxComparisonFunc(DepthStencilParams::CompareFunc compareFunc) noexcept;
    static D3D10_STENCIL_OP dxStencilOp(DepthStencilParams::StencilOp stencilOp) noexcept;
public:
    struct DXDepthStencilParams final
    {
        D3D10_DEPTH_STENCIL_DESC desc;
        ID3D10DepthStencilState* state;
    };
private:
    DX10RenderingContext& ctx;
public:
    DX10DepthStencilStateBuilder(DX10RenderingContext& ctx) noexcept
        : ctx(ctx)
    { }

    [[nodiscard]] DX10DepthStencilState* build(const DepthStencilParams& args, Error* error) const noexcept override;
    [[nodiscard]] DX10DepthStencilState* build(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilParams& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> buildTauRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    bool processArgs(const DepthStencilParams& args, [[tau::out]] ID3D10DepthStencilState** d3dDepthStencilState, [[tau::out]] Error* error) const noexcept;
};
#endif
