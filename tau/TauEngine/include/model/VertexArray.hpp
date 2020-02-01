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
    RefDynArray<Ref<IBuffer>> _buffers;
protected:
    inline IVertexArray(uSys drawCount, const RefDynArray<Ref<IBuffer>>& buffers) noexcept
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
    Ref<IShader> _shader;
    RefDynArray<Ref<IBuffer>> _buffers;
    Ref<IIndexBuffer> _indexBuffer;
    u32 _drawCount;
    DrawType _drawType;
protected:
    IVertexArrayBuilder(const uSys bufferCount) noexcept
        : _buffers(bufferCount),
          _indexBuffer(null), //_inputLayout(null),
          _drawCount(0), _drawType(static_cast<DrawType>(0))
    { }
public:
    virtual void shader(const Ref<IShader>& shader) noexcept { _shader = shader; }

    virtual void setVertexBuffer(uSys index, const Ref<IBuffer>& vertexBuffer) noexcept { _buffers[index] = vertexBuffer; }

    virtual void indexBuffer(const Ref<IIndexBuffer>& indexBuffer) noexcept { _indexBuffer = indexBuffer; }

    // virtual void inputLayout(const Ref<IInputLayout>& inputLayout) noexcept { _inputLayout = inputLayout; }

    virtual void drawCount(u32 drawCount) noexcept { _drawCount = drawCount; }
    virtual void drawType(DrawType drawType) noexcept { _drawType = drawType; }

    [[nodiscard]] const Ref<IBuffer>& getVertexBuffer(uSys index) const noexcept { return _buffers[index]; }
    [[nodiscard]] const Ref<IIndexBuffer>& indexBuffer() const noexcept { return _indexBuffer; }
    // [[nodiscard]] const Ref<IInputLayout>& inputLayout() const noexcept { return _inputLayout; }
    [[nodiscard]] u32 drawCount() const noexcept { return _drawCount; }
    [[nodiscard]] DrawType drawType() const noexcept { return _drawType; }

    [[nodiscard]] virtual IVertexArray* build([[tau::out]] Error* error = null) noexcept = 0;
};
