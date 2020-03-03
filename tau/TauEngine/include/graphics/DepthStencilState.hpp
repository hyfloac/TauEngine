#pragma once

#include "DLL.hpp"
#include "texture/TextureEnums.hpp"
#include <Objects.hpp>
#include <Safeties.hpp>
#include <ReferenceCountingPointer.hpp>
#include <RunTimeType.hpp>

class IRenderingContext;

struct DepthStencilParams final
{
    DEFAULT_CONSTRUCT_PU(DepthStencilParams);
    DEFAULT_DESTRUCT(DepthStencilParams);
    DEFAULT_COPY(DepthStencilParams);
public:
    enum class DepthWriteMask : u8
    {
        Zero = 1,
        All
    };

    using CompareFunc = ::ETexture::DepthCompareFunc;

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

    struct StencilOpParams final
    {
        DEFAULT_CONSTRUCT_PU(StencilOpParams);
        DEFAULT_DESTRUCT(StencilOpParams);
        DEFAULT_COPY(StencilOpParams);
    public:
        StencilOp failOp;
        StencilOp stencilPassDepthFailOp;
        StencilOp passOp;
        CompareFunc compareFunc;
    public:
        StencilOpParams(const StencilOp failOp, const StencilOp stencilPassDepthFailOp, const StencilOp passOp, const CompareFunc compareFunc) noexcept
            : failOp(failOp), stencilPassDepthFailOp(stencilPassDepthFailOp),
              passOp(passOp), compareFunc(compareFunc)
        { }
    };
public:
    bool enableDepthTest;
    bool enableStencilTest;

    DepthWriteMask depthWriteMask;
    CompareFunc depthCompareFunc;

    u8 stencilReadMask;
    u8 stencilWriteMask;

    StencilOpParams frontFace;
    StencilOpParams backFace;
public:
    DepthStencilParams(const bool enableDepthTest, const bool enableStencilTest, const DepthWriteMask depthWriteMask, const CompareFunc depthCompareFunc, const u8 stencilReadMask, const u8 stencilWriteMask, const StencilOpParams& frontFace, const StencilOpParams& backFace) noexcept
        : enableDepthTest(enableDepthTest),
          enableStencilTest(enableStencilTest),
          depthWriteMask(depthWriteMask),
          depthCompareFunc(depthCompareFunc),
          stencilReadMask(stencilReadMask),
          stencilWriteMask(stencilWriteMask),
          frontFace(frontFace),
          backFace(backFace)
    { }
};

#define DSS_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                             RTT_IMPL(_TYPE, IDepthStencilState)

#define DSS_IMPL(_TYPE) DSS_IMPL_BASE(_TYPE)

class TAU_DLL NOVTABLE IDepthStencilState
{
    DEFAULT_DESTRUCT_VI(IDepthStencilState);
    DELETE_COPY(IDepthStencilState);
protected:
    DepthStencilParams _params;
protected:
    IDepthStencilState(const DepthStencilParams& params) noexcept
        : _params(params)
    { }
public:
    [[nodiscard]] const DepthStencilParams& params() const noexcept { return _params; }

    RTT_BASE_IMPL(IDepthStencilState);
    RTT_BASE_CHECK(IDepthStencilState);
    RTT_BASE_CAST(IDepthStencilState);
};

class TAU_DLL NOVTABLE IDepthStencilStateBuilder
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
    [[nodiscard]] virtual IDepthStencilState* build(const DepthStencilParams& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IDepthStencilState* build(const DepthStencilParams& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilParams& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IDepthStencilState> buildTauRef(const DepthStencilParams& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilParams& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
