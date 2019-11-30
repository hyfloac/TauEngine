#include "model/IVertexArray.hpp"

IVertexArray::IVertexArray(const std::size_t bufferCount, const DrawType drawType) noexcept
    : _currentIndex(0), _attribCount(0), _buffers(bufferCount),
      _indexBuffer(nullptr), _drawCount(0), _drawType(drawType)
{ }

void IVertexArray::addVertexBuffer(IRenderingContext& context, const Ref<IBuffer>& vertexBuffer) noexcept
{
    _buffers[_currentIndex++] = vertexBuffer;
}

void IVertexArray::setIndexBuffer(IRenderingContext& context, const Ref<IIndexBuffer>& indexBuffer) noexcept
{
    _indexBuffer = indexBuffer;
    bind(context);
    indexBuffer->bind(context);
}
