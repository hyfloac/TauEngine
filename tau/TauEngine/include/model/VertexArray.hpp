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
    u32 _drawCount;
    /**
     *   It is necessary to hold on to a pointer to the
     * buffers to ensure they don't get destroyed.
     */
    RefDynArray<CPPRef<IBuffer>> _buffers;
protected:
    inline IVertexArray(uSys drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers) noexcept
        : _drawCount(drawCount), _buffers(buffers)
    { }
public:
    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    // virtual void internalSetup(IRenderingContext& context) noexcept { }

    virtual void preDraw(IRenderingContext& context) noexcept = 0;
    virtual void postDraw(IRenderingContext& context) noexcept = 0;

    virtual void draw(IRenderingContext& context) noexcept = 0;
    virtual void drawInstanced(IRenderingContext& context, uSys instanceCount) noexcept = 0;

    [[nodiscard]] u32 drawCount() const noexcept { return _drawCount; }
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
    DEFAULT_DESTRUCT_VI(IVertexArrayBuilder);
    DELETE_COPY(IVertexArrayBuilder);
public:
    enum class Error
    {
        NoError = 0,
        DrawCountNotSet,
        DrawTypeNotSet,
        BuffersNotSet,
        MemoryAllocationFailure,
        MultipleSemanticOfInvalidType,
        InternalError,
    };
protected:
    CPPRef<IShader> _shader;
    RefDynArray<CPPRef<IBuffer>> _buffers;
    CPPRef<IIndexBuffer> _indexBuffer;
    u32 _drawCount;
    DrawType _drawType;
protected:
    IVertexArrayBuilder(const uSys bufferCount) noexcept
        : _shader(null), _buffers(bufferCount),
          _indexBuffer(null), 
          _drawCount(0), _drawType(static_cast<DrawType>(0))
    { }
public:
    virtual void shader(const CPPRef<IShader>& shader) noexcept { _shader = shader; }

    virtual void setVertexBuffer(uSys index, const CPPRef<IBuffer>& vertexBuffer) noexcept { _buffers[index] = vertexBuffer; }

    virtual void indexBuffer(const CPPRef<IIndexBuffer>& indexBuffer) noexcept { _indexBuffer = indexBuffer; }

    // virtual void inputLayout(const CPPRef<IInputLayout>& inputLayout) noexcept { _inputLayout = inputLayout; }

    virtual void drawCount(u32 drawCount) noexcept { _drawCount = drawCount; }
    virtual void drawType(DrawType drawType) noexcept { _drawType = drawType; }

    [[nodiscard]] const CPPRef<IBuffer>& getVertexBuffer(uSys index) const noexcept { return _buffers[index]; }
    [[nodiscard]] const CPPRef<IIndexBuffer>& indexBuffer() const noexcept { return _indexBuffer; }
    // [[nodiscard]] const CPPRef<IInputLayout>& inputLayout() const noexcept { return _inputLayout; }
    [[nodiscard]] u32 drawCount() const noexcept { return _drawCount; }
    [[nodiscard]] DrawType drawType() const noexcept { return _drawType; }

    [[nodiscard]] virtual IVertexArray* build([[tau::out]] Error* error = null) noexcept = 0;
};