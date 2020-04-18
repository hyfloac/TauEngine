#pragma once

#include "DLL.hpp"

#include <Objects.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

struct BlendingArgs final
{
    DEFAULT_CONSTRUCT_PU(BlendingArgs);
    DEFAULT_DESTRUCT(BlendingArgs);
    DEFAULT_COPY(BlendingArgs);
public:
    /**
     * Specifies what factor to use for blending.
     *
     * The factor is multiplied by the component it is applied to.
     */
    enum class BlendFactor
    {
        /**
         * (0.0, 0.0, 0.0, 0.0).
         */
        Zero = 1,
        /**
         * (1.0, 1.0, 1.0, 1.0).
         */
        One,
        /**
         * (R_src, G_src, B_src, A_src).
         */
        SrcColor,
        /**
         * (1 - R_src, 1 - G_src, 1 - B_src, 1 - A_src).
         */
        InvSrcColor,
        /**
         * (A_src, A_src, A_src, A_src)
         */
        SrcAlpha,
        /**
         * (1 - A_src, 1 - A_src, 1 - A_src, 1 - A_src)
         */
        InvSrcAlpha,
        /**
         * (A_dst, A_dst, A_dst, A_dst)
         */
        DestAlpha,
        /**
         * (1 - A_dst, 1 - A_dst, 1 - A_dst, 1 - A_dst)
         */
        InvDestAlpha,
        /**
         * (R_dst, G_dst, B_dst, A_dst).
         */
        DestColor,
        /**
         * (1 - R_dst, 1 - G_dst, 1 - B_dst, 1 - A_dst).
         */
        InvDestColor,
        /**
         * (f, f, f, 1.0)
         * f = min(A_src, 1 - A_dest)
         */
        SrcAlphaSat,
        /**
         * Blends based on the control constant.
         */
        BlendConstant,
        /**
         * Blends based on the inverse the control constant.
         */
        InvBlendConstant,
        Src1Color,
        InvSrc1Color,
        Src1Alpha,
        InvSrc1Alpha
    };

    enum class BlendOp
    {
        /**
         * A + B
         */
        Add = 1,
        /**
         * A - B
         */
        Subtract,
        /**
         * B - A
         */
        RevSubtract,
        /**
         * Min(A, B)
         */
        Min,
        /**
         * Max(A, B)
         */
        Max
    };

    struct FrameBufferBlendingArgs final
    {
        DEFAULT_CONSTRUCT_PU(FrameBufferBlendingArgs);
        DEFAULT_DESTRUCT(FrameBufferBlendingArgs);
        DEFAULT_COPY(FrameBufferBlendingArgs);
    public:
        bool enableBlending;
        BlendFactor colorSrcFactor;
        BlendFactor colorDstFactor;
        BlendFactor alphaSrcFactor;
        BlendFactor alphaDstFactor;
        BlendOp colorBlendOp;
        BlendOp alphaBlendOp;
    public:
        FrameBufferBlendingArgs(const bool enableBlending, const BlendFactor colorSrcFactor, const BlendFactor colorDstFactor, const BlendFactor alphaSrcFactor, const BlendFactor alphaDstFactor, const BlendOp colorBlendOp, const BlendOp alphaBlendOp) noexcept
            : enableBlending(enableBlending)
            , colorSrcFactor(colorSrcFactor)
            , colorDstFactor(colorDstFactor)
            , alphaSrcFactor(alphaSrcFactor)
            , alphaDstFactor(alphaDstFactor)
            , colorBlendOp(colorBlendOp)
            , alphaBlendOp(alphaBlendOp)
        { }

        FrameBufferBlendingArgs(tau::TIPRecommended) noexcept
            : enableBlending(false)
            , colorSrcFactor(BlendFactor::One)
            , colorDstFactor(BlendFactor::Zero)
            , alphaSrcFactor(BlendFactor::One)
            , alphaDstFactor(BlendFactor::Zero)
            , colorBlendOp(BlendOp::Add)
            , alphaBlendOp(BlendOp::Add)
        { }

        // ReSharper disable once CppPossiblyUninitializedMember
        explicit FrameBufferBlendingArgs(tau::TIPDefault) noexcept { }
    };
public:
    /**
     * Sets different blending options for each framebuffer.
     *
     * Not supported for all API's.
     * If false, only the first FrameBufferBlendArgs is used.
     *
     *   This does not apply to enableBlending argument which is
     * usable by all current backend drivers.
     */
    bool independentBlending;
    /**
     * The individual blending settings for each framebuffer.
     */
    FrameBufferBlendingArgs frameBuffers[8];
public:
    explicit BlendingArgs(const bool independentBlending) noexcept
        : independentBlending(independentBlending)
        , frameBuffers{}
    { }

    explicit BlendingArgs(tau::TIPRecommended) noexcept
        : independentBlending(false)
        , frameBuffers{ tau::rec, tau::rec, tau::rec, tau::rec, tau::rec, tau::rec, tau::rec, tau::rec }
    { }

    // ReSharper disable once CppPossiblyUninitializedMember
    explicit BlendingArgs(tau::TIPDefault) noexcept { }
};

#define BS_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                            RTT_IMPL(_TYPE, IBlendingState)

#define BS_IMPL(_TYPE) BS_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IBlendingState
{
    DEFAULT_DESTRUCT_VI(IBlendingState);
    DELETE_COPY(IBlendingState);
protected:
    BlendingArgs _args;
protected:
    IBlendingState(const BlendingArgs& args) noexcept
        : _args(args)
    { }
public:
    [[nodiscard]] const BlendingArgs& args() const noexcept { return _args; }

    RTT_BASE_IMPL(IBlendingState);
    RTT_BASE_CHECK(IBlendingState);
    RTT_BASE_CAST(IBlendingState);
};

class TAU_DLL TAU_NOVTABLE IBlendingStateBuilder
{
    DEFAULT_CONSTRUCT_PO(IBlendingStateBuilder);
    DEFAULT_DESTRUCT_VI(IBlendingStateBuilder);
    DELETE_COPY(IBlendingStateBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        DriverDoesNotSupportIndependentBlending,
        InvalidBlendFactor,
        InvalidBlendOp
    };
public:
    [[nodiscard]] virtual IBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IBlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
