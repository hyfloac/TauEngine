#pragma once

#include "DLL.hpp"
#include "texture/TextureEnums.hpp"
#include <Objects.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

struct DepthStencilArgs final
{
    DEFAULT_CONSTRUCT_PU(DepthStencilArgs);
    DEFAULT_DESTRUCT(DepthStencilArgs);
    DEFAULT_COPY(DepthStencilArgs);
public:
    enum class DepthWriteMask : u8
    {
        Zero = 1,
        All
    };

    using CompareFunc = ::ETexture::CompareFunc;

    enum class StencilOp : u8
    {
        Keep = 1,
        Zero,
        Replace,
        Invert,
        IncrementClamp,
        DecrementClamp,
        IncrementWrap,
        DecrementWrap
    };

    struct StencilOpArgs final
    {
        DEFAULT_CONSTRUCT_PU(StencilOpArgs);
        DEFAULT_DESTRUCT(StencilOpArgs);
        DEFAULT_COPY(StencilOpArgs);
    public:
        StencilOp failOp;
        StencilOp stencilPassDepthFailOp;
        StencilOp passOp;
        CompareFunc compareFunc;
    public:
        StencilOpArgs(const StencilOp failOp, const StencilOp stencilPassDepthFailOp, const StencilOp passOp, const CompareFunc compareFunc) noexcept
            : failOp(failOp)
            , stencilPassDepthFailOp(stencilPassDepthFailOp)
            , passOp(passOp)
            , compareFunc(compareFunc)
        { }
    };
public:
    bool enableDepthTest;
    bool enableStencilTest;

    DepthWriteMask depthWriteMask;
    CompareFunc depthCompareFunc;

    u8 stencilReadMask;
    u8 stencilWriteMask;

    StencilOpArgs frontFace;
    StencilOpArgs backFace;
public:
    DepthStencilArgs(const bool enableDepthTest, const bool enableStencilTest, const DepthWriteMask depthWriteMask, const CompareFunc depthCompareFunc, const u8 stencilReadMask, const u8 stencilWriteMask, const StencilOpArgs& frontFace, const StencilOpArgs& backFace) noexcept
        : enableDepthTest(enableDepthTest)
        , enableStencilTest(enableStencilTest)
        , depthWriteMask(depthWriteMask)
        , depthCompareFunc(depthCompareFunc)
        , stencilReadMask(stencilReadMask)
        , stencilWriteMask(stencilWriteMask)
        , frontFace(frontFace)
        , backFace(backFace)
    { }

    explicit DepthStencilArgs(tau::TIPRecommended) noexcept
        : enableDepthTest(true)
        , enableStencilTest(true)
        , depthWriteMask(DepthWriteMask::All)
        , depthCompareFunc(CompareFunc::LessThan)
        , stencilReadMask(0xFF)
        , stencilWriteMask(0xFF)
        , frontFace(StencilOp::Keep, StencilOp::IncrementClamp, StencilOp::Keep, CompareFunc::Always)
        , backFace(StencilOp::Keep, StencilOp::DecrementClamp, StencilOp::Keep, CompareFunc::Always)
    { }

    // ReSharper disable once CppPossiblyUninitializedMember
    explicit DepthStencilArgs(tau::TIPDefault) noexcept { }
};

#define DSS_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                             RTT_IMPL(_TYPE, IDepthStencilState)

#define DSS_IMPL(_TYPE) DSS_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IDepthStencilState
{
    DEFAULT_DESTRUCT_VI(IDepthStencilState);
    DELETE_COPY(IDepthStencilState);
protected:
    DepthStencilArgs _args;
protected:
    IDepthStencilState(const DepthStencilArgs& args) noexcept
        : _args(args)
    { }
public:
    [[nodiscard]] const DepthStencilArgs& args() const noexcept { return _args; }

    RTT_BASE_IMPL(IDepthStencilState);
    RTT_BASE_CHECK(IDepthStencilState);
    RTT_BASE_CAST(IDepthStencilState);
};

class TAU_DLL TAU_NOVTABLE IDepthStencilStateBuilder
{
    DEFAULT_CONSTRUCT_PO(IDepthStencilStateBuilder);
    DEFAULT_DESTRUCT_VI(IDepthStencilStateBuilder);
    DELETE_COPY(IDepthStencilStateBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InvalidDepthWriteMask,
        InvalidDepthCompareFunc,
        InvalidFrontFaceStencilFailOp,
        InvalidFrontFaceStencilPassDepthFailOp,
        InvalidFrontFaceStencilPassOp,
        InvalidFrontFaceStencilCompareFunc,
        InvalidBackFaceStencilFailOp,
        InvalidBackFaceStencilPassDepthFailOp,
        InvalidBackFaceStencilPassOp,
        InvalidBackFaceStencilCompareFunc,
    };
public:
    [[nodiscard]] virtual IDepthStencilState* build(const DepthStencilArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IDepthStencilState* build(const DepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IDepthStencilState> buildTauRef(const DepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
