#pragma once

#include "graphics/RasterizerState.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL DX10RasterizerState final : public IRasterizerState
{
    RS_IMPL(DX10RasterizerState);
private:
    ID3D10RasterizerState* _d3dRasterizerState;
public:
    DX10RasterizerState(const RasterizerArgs& params, ID3D10RasterizerState* const d3dRasterizerState) noexcept
        : IRasterizerState(params)
        , _d3dRasterizerState(d3dRasterizerState)
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
    DX10GraphicsInterface& _gi;
public:
    DX10RasterizerStateBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10RasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10RasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IRasterizerState> buildCPPRef(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> buildTauRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IRasterizerState> buildTauSRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const RasterizerArgs& args, [[tau::out]] DXRasterizerArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
