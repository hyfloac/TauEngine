#pragma once

#include "DLL.hpp"
#include <Objects.hpp>
#include <DynArray.hpp>

#include "system/RenderingContext.hpp"
#include "model/Buffer.hpp"

enum class DrawType : u8
{
    SeparatedTriangles = 1,
    ConnectedTriangles,
    PointConnectedTriangles
};

class TAU_DLL NOVTABLE IVertexArray
{
    DEFAULT_DESTRUCT_VI(IVertexArray);
    DELETE_COPY(IVertexArray);
protected:
    u32 _drawCount;
protected:
    inline IVertexArray(uSys drawCount) noexcept
        : _drawCount(drawCount)
    { }
public:
    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void internalSetup(IRenderingContext& context) noexcept { }

    virtual void preDraw(IRenderingContext& context) noexcept = 0;
    virtual void postDraw(IRenderingContext& context) noexcept = 0;

    virtual void draw(IRenderingContext& context) noexcept = 0;

    [[nodiscard]] u32 drawCount() const noexcept { return _drawCount; }
};

class IInputLayout;

class TAU_DLL NOVTABLE IVertexArrayBuilder
{
    DEFAULT_DESTRUCT_VI(IVertexArrayBuilder);
    DELETE_COPY(IVertexArrayBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InputLayoutNotSet,
        BufferCountDoesntMatchDescriptors,
        DrawCountNotSet,
        DrawTypeNotSet,
        BuffersNotSet,
        MemoryAllocationFailure
    };
protected:
    RefDynArray<Ref<IBuffer>> _buffers;
    Ref<IIndexBuffer> _indexBuffer;
    Ref<IInputLayout> _inputLayout;
    u32 _drawCount;
    DrawType _drawType;
protected:
    IVertexArrayBuilder(const uSys bufferCount) noexcept
        : _buffers(bufferCount),
          _indexBuffer(null), _inputLayout(null),
          _drawCount(0), _drawType(static_cast<DrawType>(0))
    { }
public:
    virtual void setVertexBuffer(uSys index, const Ref<IBuffer>& vertexBuffer) noexcept { _buffers[index] = vertexBuffer; }

    virtual void indexBuffer(const Ref<IIndexBuffer>& indexBuffer) noexcept { _indexBuffer = indexBuffer; }

    virtual void inputLayout(const Ref<IInputLayout>& inputLayout) noexcept { _inputLayout = inputLayout; }

    virtual void drawCount(u32 drawCount) noexcept { _drawCount = drawCount; }
    virtual void drawType(DrawType drawType) noexcept { _drawType = drawType; }

    [[nodiscard]] const Ref<IBuffer>& getVertexBuffer(uSys index) const noexcept { return _buffers[index]; }
    [[nodiscard]] const Ref<IIndexBuffer>& indexBuffer() const noexcept { return _indexBuffer; }
    [[nodiscard]] const Ref<IInputLayout>& inputLayout() const noexcept { return _inputLayout; }
    [[nodiscard]] u32 drawCount() const noexcept { return _drawCount; }
    [[nodiscard]] DrawType drawType() const noexcept { return _drawType; }

    [[nodiscard]] virtual IVertexArray* build([[tau::out]] Error* error = null) noexcept = 0;
};
