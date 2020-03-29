#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>
#include <Safeties.hpp>
#include "DLL.hpp"

enum class DrawType : u8
{
    SeparatedTriangles = 1,
    ConnectedTriangles,
    PointConnectedTriangles
};

class IBuffer;
class IIndexBuffer;
class IShader;
class IRenderingContext;

class TAU_DLL NOVTABLE IVertexArray
{
    DEFAULT_DESTRUCT_VI(IVertexArray);
    DELETE_COPY(IVertexArray);
protected:
    uSys _drawCount;
    /**
     *   It is necessary to hold on to a pointer to the
     * buffers to ensure they don't get destroyed.
     */
    RefDynArray<CPPRef<IBuffer>> _buffers;
protected:
    inline IVertexArray(const uSys drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers) noexcept
        : _drawCount(drawCount), _buffers(buffers)
    { }
public:
    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void preDraw(IRenderingContext& context) noexcept = 0;
    virtual void postDraw(IRenderingContext& context) noexcept = 0;

    virtual void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept = 0;
    virtual void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept = 0;

    [[nodiscard]] uSys drawCount() const noexcept { return _drawCount; }
};

class IShaderProgram;

struct VertexArrayArgs final
{
    DEFAULT_DESTRUCT(VertexArrayArgs);
    DEFAULT_COPY(VertexArrayArgs);
public:
    CPPRef<IShader> shader;
    RefDynArray<CPPRef<IBuffer>> buffers;
    CPPRef<IIndexBuffer> indexBuffer;
    u32 drawCount;
    DrawType drawType;
public:
    inline VertexArrayArgs(const uSys bufferCount) noexcept
        : shader(null), buffers(bufferCount),
          indexBuffer(null),
          drawCount(0), drawType(static_cast<DrawType>(0))
    { }
};

class TAU_DLL NOVTABLE IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PO(IVertexArrayBuilder);
    DEFAULT_DESTRUCT_VI(IVertexArrayBuilder);
    DELETE_COPY(IVertexArrayBuilder);
public:
    enum class Error
    {
        NoError = 0,
        DrawCountNotSet,
        DrawTypeNotSet,
        BuffersNotSet,
        ShaderNotSet,
        ShaderMustBeVertexShader,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        MultipleSemanticOfInvalidType,
        InternalError
    };
public:
    [[nodiscard]] virtual IVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual IVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
