#include <model/VertexBuffer.hpp>
#include <Utils.hpp>

GLenum getGLBufferType(BufferType bt) noexcept
{
    switch(bt)
    {
        case ArrayBuffer:             return GL_ARRAY_BUFFER;
        case AtomicCounterBuffer:     return GL_ATOMIC_COUNTER_BUFFER;
        case CopyReadBuffer:          return GL_COPY_READ_BUFFER;
        case CopyWriteBuffer:         return GL_COPY_WRITE_BUFFER;
        case DispatchIndirectBuffer:  return GL_DISPATCH_INDIRECT_BUFFER;
        case DrawIndirectBuffer:      return GL_DRAW_INDIRECT_BUFFER;
        case ElementArrayBuffer:      return GL_ELEMENT_ARRAY_BUFFER;
        case PixelPackBuffer:         return GL_PIXEL_PACK_BUFFER;
        case PixelUnpackBuffer:       return GL_PIXEL_UNPACK_BUFFER;
        case QueryBuffer:             return GL_QUERY_BUFFER;
        case ShaderStorageBuffer:     return GL_SHADER_STORAGE_BUFFER;
        case TextureBuffer:           return GL_TEXTURE_BUFFER;
        case TransformFeedbackBuffer: return GL_TRANSFORM_FEEDBACK_BUFFER;
        case UniformBuffer:           return GL_UNIFORM_BUFFER;
        default:                      return -1;
    }
}

BufferType getBufferType(GLenum bt) noexcept
{
    switch(bt)
    {
        case GL_ARRAY_BUFFER:              return ArrayBuffer;
        case GL_ATOMIC_COUNTER_BUFFER:     return AtomicCounterBuffer;
        case GL_COPY_READ_BUFFER:          return CopyReadBuffer;
        case GL_COPY_WRITE_BUFFER:         return CopyWriteBuffer;
        case GL_DISPATCH_INDIRECT_BUFFER:  return DispatchIndirectBuffer;
        case GL_DRAW_INDIRECT_BUFFER:      return DrawIndirectBuffer;
        case GL_ELEMENT_ARRAY_BUFFER:      return ElementArrayBuffer;
        case GL_PIXEL_PACK_BUFFER:         return PixelPackBuffer;
        case GL_PIXEL_UNPACK_BUFFER:       return PixelUnpackBuffer;
        case GL_QUERY_BUFFER:              return QueryBuffer;
        case GL_SHADER_STORAGE_BUFFER:     return ShaderStorageBuffer;
        case GL_TEXTURE_BUFFER:            return TextureBuffer;
        case GL_TRANSFORM_FEEDBACK_BUFFER: return TransformFeedbackBuffer;
        case GL_UNIFORM_BUFFER:            return UniformBuffer;
        default:                           return static_cast<BufferType>(-1);
    }
}

GLenum getGLBufferUsageType(BufferUsageType usage) noexcept
{
    switch(usage)
    {
        case StreamDraw:  return GL_STREAM_DRAW;
        case StreamRead:  return GL_STREAM_READ;
        case StreamCopy:  return GL_STREAM_COPY;
        case StaticDraw:  return GL_STATIC_DRAW;
        case StaticRead:  return GL_STATIC_READ;
        case StaticCopy:  return GL_STATIC_COPY;
        case DynamicDraw: return GL_DYNAMIC_DRAW;
        case DynamicRead: return GL_DYNAMIC_READ;
        case DynamicCopy: return GL_DYNAMIC_COPY;
        default: return -1;
    }
}

BufferUsageType getBufferUsageType(GLenum usage) noexcept
{
    switch(usage)
    {
        case  GL_STREAM_DRAW: return StreamDraw;
        case  GL_STREAM_READ: return StreamRead;
        case  GL_STREAM_COPY: return StreamCopy;
        case  GL_STATIC_DRAW: return StaticDraw;
        case  GL_STATIC_READ: return StaticRead;
        case  GL_STATIC_COPY: return StaticCopy;
        case GL_DYNAMIC_DRAW: return DynamicDraw;
        case GL_DYNAMIC_READ: return DynamicRead;
        case GL_DYNAMIC_COPY: return DynamicCopy;
        default:              return static_cast<BufferUsageType>(-1);
    }
}

VertexBuffer::VertexBuffer(BufferType type, BufferUsageType usage) noexcept
    : _buffer(), _glType(getGLBufferType(type)), _glUsage(getGLBufferUsageType(usage))
{
    glGenBuffers(1, &_buffer);
}

VertexBuffer::~VertexBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void VertexBuffer::bind() const noexcept
{
    glBindBuffer(this->_glType, this->_buffer);
}

void VertexBuffer::unbind() const noexcept
{
    glBindBuffer(this->_glType, 0);
}

void VertexBuffer::fillBuffer(GLsizei renderCount, GLsizeiptr size, const GLvoid* data) noexcept
{
    this->_count = renderCount;
    glBufferData(this->_glType, size, data, this->_glUsage);
}

void VertexBuffer::draw() const noexcept
{
    glDrawArrays(GL_TRIANGLES, 0, this->_count);
}

void VertexBuffer::drawIndexed() const noexcept
{
    glDrawElements(GL_TRIANGLES, this->_count, GL_UNSIGNED_INT, 0);
}
