#pragma once

#include "graphics/DepthStencilState.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL DX10DepthStencilState final : public IDepthStencilState
{
    DSS_IMPL(DX10DepthStencilState);
private:
    ID3D10DepthStencilState* _d3dDepthStencilState;
public:
    DX10DepthStencilState(const DepthStencilArgs& params, ID3D10DepthStencilState* const d3dDepthStencilState) noexcept
        : IDepthStencilState(params)
        , _d3dDepthStencilState(d3dDepthStencilState)
    { }

    ~DX10DepthStencilState() noexcept
    { RELEASE_DX(_d3dDepthStencilState); }

    DX10DepthStencilState(const DX10DepthStencilState& copy) noexcept
        : IDepthStencilState(copy)
        , _d3dDepthStencilState(copy._d3dDepthStencilState)
    { _d3dDepthStencilState->AddRef(); }

    DX10DepthStencilState(DX10DepthStencilState&& move) noexcept
        : IDepthStencilState(::std::move(move))
        , _d3dDepthStencilState(move._d3dDepthStencilState)
    { move._d3dDepthStencilState = null; }

    DX10DepthStencilState& operator=(const DX10DepthStencilState& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilState);

        IDepthStencilState::operator=(copy);

        _d3dDepthStencilState = copy._d3dDepthStencilState;
        _d3dDepthStencilState->AddRef();

        return *this;
    }

    DX10DepthStencilState& operator=(DX10DepthStencilState&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilState);

        IDepthStencilState::operator=(std::move(move));

        _d3dDepthStencilState = move._d3dDepthStencilState;
        move._d3dDepthStencilState = null;

        return *this;
    }
    
    [[nodiscard]] ID3D10DepthStencilState* d3dDepthStencilState() const noexcept { return _d3dDepthStencilState; }
    // [[nodiscard]] const ID3D10DepthStencilState* d3dDepthStencilState() const noexcept { return _d3dDepthStencilState; }

    void apply(DX10RenderingContext& ctx) const noexcept;
};

class TAU_DLL DX10DepthStencilStateBuilder final : public IDepthStencilStateBuilder
{
    DEFAULT_DESTRUCT(DX10DepthStencilStateBuilder);
    DELETE_COPY(DX10DepthStencilStateBuilder);
public:
    static D3D10_DEPTH_WRITE_MASK dxDepthWriteMask(DepthStencilArgs::DepthWriteMask depthWriteMask) noexcept;
    static D3D10_COMPARISON_FUNC dxComparisonFunc(DepthStencilArgs::CompareFunc compareFunc) noexcept;
    static D3D10_STENCIL_OP dxStencilOp(DepthStencilArgs::StencilOp stencilOp) noexcept;
public:
    struct DXDepthStencilArgs final
    {
        D3D10_DEPTH_STENCIL_DESC desc;
        ID3D10DepthStencilState* state;
    };
private:
    DX10GraphicsInterface& _gi;
public:
    DX10DepthStencilStateBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10DepthStencilState* build(const DepthStencilArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10DepthStencilState* build(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> buildTauRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    bool processArgs(const DepthStencilArgs& args, [[tau::out]] ID3D10DepthStencilState** d3dDepthStencilState, [[tau::out]] Error* error) const noexcept;
};
#endif
