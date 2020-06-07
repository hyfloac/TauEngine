#pragma once

#include "DLL.hpp"
#include <Objects.hpp>
#include <NumTypes.hpp>
#include <RunTimeType.hpp>
#include <Safeties.hpp>

struct RasterizerArgs final
{
    DEFAULT_CONSTRUCT_PU(RasterizerArgs);
    DEFAULT_DESTRUCT(RasterizerArgs);
    DEFAULT_CM_PU(RasterizerArgs);
public:
    enum class CullMode : u8
    {
        None = 1,
        Front,
        Back
    };

    enum class FillMode : u8
    {
        Vertices = 1,
        Wireframe,
        Filled
    };
public:
    bool enableScissorTest;
    bool frontFaceCounterClockwise;
    CullMode cullMode;
    FillMode fillMode;
public:
    RasterizerArgs(const bool _enableScissorTest, const bool _frontFaceCounterClockwise, const CullMode _cullMode, const FillMode _fillMode) noexcept
        : enableScissorTest(_enableScissorTest),
          frontFaceCounterClockwise(_frontFaceCounterClockwise),
          cullMode(_cullMode), fillMode(_fillMode)
    { }

    explicit RasterizerArgs(tau::TIPRecommended) noexcept
        : enableScissorTest(false),
          frontFaceCounterClockwise(true),
          cullMode(CullMode::Back), fillMode(FillMode::Filled)
    { }

    // ReSharper disable once CppPossiblyUninitializedMember
    explicit RasterizerArgs(tau::TIPDefault) noexcept { }
};

#define RS_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IRasterizerState)

#define RS_IMPL(_TYPE) RS_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IRasterizerState
{
    DEFAULT_DESTRUCT_VI(IRasterizerState);
    DEFAULT_CM_PO(IRasterizerState);
protected:
    RasterizerArgs _args;
protected:
    IRasterizerState(const RasterizerArgs& args) noexcept
        : _args(args)
    { }
public:
    [[nodiscard]] const RasterizerArgs& args() const noexcept { return _args; }

    RTT_BASE_IMPL(IRasterizerState);
    RTT_BASE_CHECK(IRasterizerState);
    RTT_BASE_CAST(IRasterizerState);
};

class TAU_DLL TAU_NOVTABLE IRasterizerStateBuilder
{
    DEFAULT_CONSTRUCT_PO(IRasterizerStateBuilder);
    DEFAULT_DESTRUCT_VI(IRasterizerStateBuilder);
    DEFAULT_CM_PO(IRasterizerStateBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InvalidCullMode,
        InvalidFillMode
    };
public:
    [[nodiscard]] virtual IRasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IRasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IRasterizerState> buildCPPRef(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IRasterizerState> buildTauRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IRasterizerState> buildTauSRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
