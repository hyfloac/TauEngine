#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"
#include "system/RenderingContext.hpp"
#include "model/IBuffer.hpp"

enum class DrawType : u8
{
    SeparatedTriangles,
    ConnectedTriangles,
    PointConnectedTriangles
};

class TAU_DLL IVertexArray
{
    DEFAULT_DESTRUCT_VI(IVertexArray);
    DELETE_COPY(IVertexArray);
protected:
    uSys _currentIndex;
    uSys _attribCount;
    DynArray<Ref<IBuffer>> _buffers;
    Ref<IIndexBuffer> _indexBuffer;
    u32 _drawCount;
    DrawType _drawType;
protected:
    IVertexArray(uSys bufferCount, DrawType drawType) noexcept;
public:
    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void internalSetup(IRenderingContext& context) noexcept { }

    virtual void preDraw(IRenderingContext& context) noexcept = 0;
    virtual void postDraw(IRenderingContext& context) noexcept = 0;

    virtual void draw(IRenderingContext& context) noexcept = 0;

    virtual void drawIndexed(IRenderingContext& context) noexcept = 0;

    virtual void addVertexBuffer(IRenderingContext& context, const Ref<IBuffer>& vertexBuffer) noexcept;

    virtual void setIndexBuffer(IRenderingContext& context, const Ref<IIndexBuffer>& indexBuffer) noexcept;

    [[nodiscard]] const u32& drawCount() const noexcept { return _drawCount; }
    [[nodiscard]] u32& drawCount() noexcept { return _drawCount; }
    [[nodiscard]] DrawType drawType() const noexcept { return _drawType; }
    virtual void drawType(const DrawType drawType) noexcept { _drawType = drawType; }
};
