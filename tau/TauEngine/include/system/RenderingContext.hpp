/**
 * @file 
 */
#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <RunTimeType.hpp>

#include "Color.hpp"
#include "DLL.hpp"
#include "events/Exception.hpp"
#include "RenderingMode.hpp"

#define RC_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                            RTT_IMPL(_TYPE, IRenderingContext)

#define RC_IMPL(_TYPE) RC_IMPL_BASE(_TYPE)

class Window;

class IFrameBufferBuilder;
class IDepthStencilState;
struct DepthStencilArgs;
class IRasterizerState;
struct RasterizerArgs;

class TAU_DLL TAU_NOVTABLE IRenderingContext
{
    DEFAULT_DESTRUCT_VI(IRenderingContext);
    DELETE_COPY(IRenderingContext);
protected:
    RenderingMode _mode;
protected:
    IRenderingContext(const RenderingMode& mode)
        : _mode(mode)
    { }
public:
    [[nodiscard]] const RenderingMode& mode() const noexcept { return _mode; }

    virtual void deactivateContext() noexcept = 0;
    virtual void activateContext() noexcept = 0;

    virtual void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0.0f, float maxZ = 1.0f) noexcept = 0;

    virtual void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept = 0;

    virtual void setVSync(bool vsync) noexcept = 0;

    /**
     * Sets the current Depth-Stencil State.
     *
     *   Returns the previous Depth-Stencil State. The client
     * is expected to reset this state once they've finished.
     *
     * @param[in] dsState
     *      The new Depth-Stencil State
     * @return
     *      The old Depth-Stencil State
     */
    virtual NullableRef<IDepthStencilState> setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept = 0;

    /**
     *   Sets the default Depth-Stencil State that is used
     * by {@link resetDepthStencilState() @endlink}.
     *
     * @param[in] dsState
     *      The new default Depth-Stencil State.
     */
    virtual void setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept = 0;

    /**
     *   Resets the Depth-Stencil State to the default
     * value set by {@link setDefaultDepthStencilState(const Ref<IDepthStencilState>&) @endlink}.
     */
    virtual void resetDepthStencilState() noexcept = 0;
    [[nodiscard]] virtual NullableRef<IDepthStencilState> getDefaultDepthStencilState() noexcept = 0;

    /**
     * Retrieves the default Depth-Stencil State parameters.
     *
     * This is useful for creating sub-states.
     *
     * @return
     *      The default Depth-Stencil State parameters
     */
    virtual const DepthStencilArgs& getDefaultDepthStencilArgs() noexcept = 0;

    virtual NullableRef<IRasterizerState> setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept = 0;
    virtual void setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept = 0;
    virtual void resetRasterizerState() noexcept = 0;
    virtual const RasterizerArgs& getDefaultRasterizerArgs() noexcept = 0;
    [[nodiscard]] virtual NullableRef<IRasterizerState> getDefaultRasterizerState() noexcept = 0;

    virtual void beginFrame() noexcept = 0;
    virtual void endFrame() noexcept = 0;

    virtual void swapFrame() noexcept = 0;

    [[nodiscard]] virtual CPPRef<IFrameBufferBuilder> createFrameBuffer() noexcept = 0;

    RTT_BASE_IMPL(IRenderingContext);
    RTT_BASE_CHECK(IRenderingContext);
    RTT_BASE_CAST(IRenderingContext);
};

struct RenderingContextArgs final
{
    Window* window;
    bool vsync;
};

class TAU_DLL NOVTABLE IRenderingContextBuilder
{
    DEFAULT_CONSTRUCT_PO(IRenderingContextBuilder);
    DEFAULT_DESTRUCT_VI(IRenderingContextBuilder);
    DELETE_COPY(IRenderingContextBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationError,
        IncorrectGraphicsAPI,
        UnsupportedAPIVersion,
        NullWindow
    };
public:
    [[nodiscard]] virtual IRenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual IRenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<IRenderingContext> buildCPPRef(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<IRenderingContext> buildTauRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IRenderingContext> buildTauSRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};

class IncorrectContextException final : public Exception
{
public:
    IncorrectContextException() noexcept = default;
    ~IncorrectContextException() noexcept override = default;

    EXCEPTION_IMPL(IncorrectContextException);
};
