#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "GraphicsEnums.hpp"

class IVertexBuffer;
class IIndexBuffer;
class IRenderingContext;

#define VERTEX_ARRAY_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IVertexArray)

#define VERTEX_ARRAY_IMPL(_TYPE) VERTEX_ARRAY_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IVertexArray
{
    DEFAULT_DESTRUCT_VI(IVertexArray);
    DEFAULT_CM_PO(IVertexArray);
protected:
    uSys _drawCount;
    /**
     *   It is necessary to hold on to a pointer to the
     * buffers to ensure they don't get destroyed.
     */
    DynArray<NullableRef<IVertexBuffer>> _buffers;
protected:
    inline IVertexArray(const uSys drawCount, const DynArray<NullableRef<IVertexBuffer>>& buffers) noexcept
        : _drawCount(drawCount)
        , _buffers(buffers)
    { }
public:
    [[nodiscard]] uSys drawCount() const noexcept { return _drawCount; }

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept = 0;
    virtual void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept = 0;

    RTT_BASE_IMPL(IVertexArray);
    RTT_BASE_CHECK(IVertexArray);
    RTT_BASE_CAST(IVertexArray);
};

class IShaderProgram;

struct VertexArrayArgs final
{
    DEFAULT_DESTRUCT(VertexArrayArgs);
    DEFAULT_CM_PU(VertexArrayArgs);
public:
    DynArray<NullableRef<IVertexBuffer>> buffers;
    NullableRef<IIndexBuffer> indexBuffer;
    u32 drawCount;
    EGraphics::DrawType drawType;
public:
    inline VertexArrayArgs(const uSys bufferCount) noexcept
        : buffers(bufferCount)
        , indexBuffer(null)
        , drawCount(0)
        , drawType(static_cast<EGraphics::DrawType>(0))
    { }
};

class TAU_DLL NOVTABLE IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PO(IVertexArrayBuilder);
    DEFAULT_DESTRUCT_VI(IVertexArrayBuilder);
    DEFAULT_CM_PO(IVertexArrayBuilder);
public:
    enum class Error
    {
        NoError = 0,
        DrawCountNotSet,
        DrawTypeNotSet,
        BuffersNotSet,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual IVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual IVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
