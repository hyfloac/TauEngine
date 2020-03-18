#pragma once

#include "graphics/RasterizerState.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL DX10RasterizerState final : public IRasterizerState
{
private:
    ID3D10RasterizerState * _d3dRasterizerState;
public:
    DX10RasterizerState(const RasterizerArgs& params, ID3D10RasterizerState* const d3dRasterizerState) noexcept
        : IRasterizerState(params), _d3dRasterizerState(d3dRasterizerState)
    { }

    ~DX10RasterizerState() noexcept
    {
        _d3dRasterizerState->Release();
        _d3dRasterizerState = null;
    }

    [[nodiscard]] const ID3D10RasterizerState* d3dRasterizerState() const noexcept { return _d3dRasterizerState; }
    [[nodiscard]] ID3D10RasterizerState* d3dRasterizerState() noexcept { return _d3dRasterizerState; }

    void apply(DX10RenderingContext& ctx) const noexcept;
};

class TAU_DLL DX10RasterizerStateBuilder final : public IRasterizerStateBuilder
{
    DEFAULT_DESTRUCT(DX10RasterizerStateBuilder);
    DELETE_COPY(DX10RasterizerStateBuilder);
public:
    static D3D10_CULL_MODE dxCullMode(RasterizerArgs::CullMode cullMode) noexcept;
    static D3D10_FILL_MODE dxFillMode(RasterizerArgs::FillMode fillMode) noexcept;
public:
    struct DXRasterizerArgs final
    {
        D3D10_RASTERIZER_DESC desc;
        ID3D10RasterizerState* state;
    };
private:
    DX10RenderingContext& ctx;
public:
    DX10RasterizerStateBuilder(DX10RenderingContext& ctx) noexcept
        : ctx(ctx)
    { }
};
#endif
