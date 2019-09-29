#include <model/VertexBuffer.hpp>

GLenum getGLBufferType(const BufferType bt) noexcept
{
    switch(bt)
    {
        case BufferType::ArrayBuffer:             return GL_ARRAY_BUFFER;
        case BufferType::AtomicCounterBuffer:     return GL_ATOMIC_COUNTER_BUFFER;
        case BufferType::CopyReadBuffer:          return GL_COPY_READ_BUFFER;
        case BufferType::CopyWriteBuffer:         return GL_COPY_WRITE_BUFFER;
        case BufferType::DispatchIndirectBuffer:  return GL_DISPATCH_INDIRECT_BUFFER;
        case BufferType::DrawIndirectBuffer:      return GL_DRAW_INDIRECT_BUFFER;
        case BufferType::ElementArrayBuffer:      return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::PixelPackBuffer:         return GL_PIXEL_PACK_BUFFER;
        case BufferType::PixelUnpackBuffer:       return GL_PIXEL_UNPACK_BUFFER;
        case BufferType::QueryBuffer:             return GL_QUERY_BUFFER;
        case BufferType::ShaderStorageBuffer:     return GL_SHADER_STORAGE_BUFFER;
        case BufferType::TextureBuffer:           return GL_TEXTURE_BUFFER;
        case BufferType::TransformFeedbackBuffer: return GL_TRANSFORM_FEEDBACK_BUFFER;
        case BufferType::UniformBuffer:           return GL_UNIFORM_BUFFER;
        default:                                  return static_cast<GLenum>(-1);
    }
}

BufferType getBufferType(const GLenum bt) noexcept
{
    switch(bt)
    {
        case GL_ARRAY_BUFFER:              return BufferType::ArrayBuffer;
        case GL_ATOMIC_COUNTER_BUFFER:     return BufferType::AtomicCounterBuffer;
        case GL_COPY_READ_BUFFER:          return BufferType::CopyReadBuffer;
        case GL_COPY_WRITE_BUFFER:         return BufferType::CopyWriteBuffer;
        case GL_DISPATCH_INDIRECT_BUFFER:  return BufferType::DispatchIndirectBuffer;
        case GL_DRAW_INDIRECT_BUFFER:      return BufferType::DrawIndirectBuffer;
        case GL_ELEMENT_ARRAY_BUFFER:      return BufferType::ElementArrayBuffer;
        case GL_PIXEL_PACK_BUFFER:         return BufferType::PixelPackBuffer;
        case GL_PIXEL_UNPACK_BUFFER:       return BufferType::PixelUnpackBuffer;
        case GL_QUERY_BUFFER:              return BufferType::QueryBuffer;
        case GL_SHADER_STORAGE_BUFFER:     return BufferType::ShaderStorageBuffer;
        case GL_TEXTURE_BUFFER:            return BufferType::TextureBuffer;
        case GL_TRANSFORM_FEEDBACK_BUFFER: return BufferType::TransformFeedbackBuffer;
        case GL_UNIFORM_BUFFER:            return BufferType::UniformBuffer;
        default:                           return static_cast<BufferType>(-1);
    }
}

GLenum getGLBufferUsageType(const BufferUsageType usage) noexcept
{
    switch(usage)
    {
        case BufferUsageType::StreamDraw:  return GL_STREAM_DRAW;
        case BufferUsageType::StreamRead:  return GL_STREAM_READ;
        case BufferUsageType::StreamCopy:  return GL_STREAM_COPY;
        case BufferUsageType::StaticDraw:  return GL_STATIC_DRAW;
        case BufferUsageType::StaticRead:  return GL_STATIC_READ;
        case BufferUsageType::StaticCopy:  return GL_STATIC_COPY;
        case BufferUsageType::DynamicDraw: return GL_DYNAMIC_DRAW;
        case BufferUsageType::DynamicRead: return GL_DYNAMIC_READ;
        case BufferUsageType::DynamicCopy: return GL_DYNAMIC_COPY;
        default:                           return static_cast<GLenum>(-1);
    }
}

BufferUsageType getBufferUsageType(const GLenum usage) noexcept
{
    switch(usage)
    {
        case  GL_STREAM_DRAW: return BufferUsageType::StreamDraw;
        case  GL_STREAM_READ: return BufferUsageType::StreamRead;
        case  GL_STREAM_COPY: return BufferUsageType::StreamCopy;
        case  GL_STATIC_DRAW: return BufferUsageType::StaticDraw;
        case  GL_STATIC_READ: return BufferUsageType::StaticRead;
        case  GL_STATIC_COPY: return BufferUsageType::StaticCopy;
        case GL_DYNAMIC_DRAW: return BufferUsageType::DynamicDraw;
        case GL_DYNAMIC_READ: return BufferUsageType::DynamicRead;
        case GL_DYNAMIC_COPY: return BufferUsageType::DynamicCopy;
        default:              return static_cast<BufferUsageType>(-1);
    }
}

VertexBuffer::VertexBuffer(const BufferType type, const BufferUsageType usage) noexcept
    : _buffer(), _glType(getGLBufferType(type)), _glUsage(getGLBufferUsageType(usage)), _count(0)
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

void VertexBuffer::fillBuffer(const GLsizei renderCount, const GLsizeiptr size, const GLvoid* data) noexcept
{
    this->_count = renderCount;
    bind();
    glBufferData(this->_glType, size, data, this->_glUsage);
}

void VertexBuffer::draw() const noexcept
{
    glDrawArrays(GL_TRIANGLES, 0, this->_count);
}

void VertexBuffer::drawIndexed() const noexcept
{
    glDrawElements(GL_TRIANGLES, this->_count, GL_UNSIGNED_INT, nullptr);
}

void VertexBuffer::draw(RenderingPipeline& rp) const noexcept
{
    rp.pushGLDrawArrays(GL_TRIANGLES, 0, this->_count);
}

void VertexBuffer::drawIndexed(RenderingPipeline& rp) const noexcept
{
    rp.pushGLDrawElements(GL_TRIANGLES, this->_count, GL_UNSIGNED_INT, nullptr);
}

VertexBufferShared::VertexBufferShared(const BufferType type, const BufferUsageType usage) noexcept
    : _buffer(), 
      _glType(getGLBufferType(type)), 
      _glUsage(getGLBufferUsageType(usage)), 
      _count(0),
      _refCount(new u32)
{
    *_refCount = 1;
    glGenBuffers(1, &_buffer);
}

VertexBufferShared::VertexBufferShared(const VertexBufferShared& copy) noexcept
    : _buffer(copy._buffer),
      _glType(copy._glType),
      _glUsage(copy._glUsage),
      _count(copy._count),
      _refCount(copy._refCount)
{
    ++(*_refCount);
}

VertexBufferShared::VertexBufferShared(VertexBufferShared&& move) noexcept
    : _buffer(move._buffer),
      _glType(move._glType),
      _glUsage(move._glUsage),
      _count(move._count),
      _refCount(move._refCount)
{
    ++(*_refCount);
}

VertexBufferShared::~VertexBufferShared() noexcept
{
    if(--(*_refCount) <= 0)
    {
        glDeleteBuffers(1, &_buffer);
        delete _refCount;
    }
}

VertexBufferShared& VertexBufferShared::operator=(const VertexBufferShared& copy) noexcept
{
    if(this != &copy)
    {
        _buffer = copy._buffer;
        _glType = copy._glType;
        _glUsage = copy._glUsage;
        _count = copy._count;
        _refCount = copy._refCount;
        ++(*_refCount);
    }

    return *this;
}

VertexBufferShared& VertexBufferShared::operator=(VertexBufferShared&& move) noexcept
{
    if(this != &move)
    {
        _buffer = move._buffer;
        _glType = move._glType;
        _glUsage = move._glUsage;
        _count = move._count;
        _refCount = move._refCount;
        ++(*_refCount);
    }

    return *this;
}

void VertexBufferShared::bind() const noexcept
{
    glBindBuffer(this->_glType, this->_buffer);
}

void VertexBufferShared::unbind() const noexcept
{
    glBindBuffer(this->_glType, 0);
}

void VertexBufferShared::fillBuffer(const GLsizei renderCount, const GLsizeiptr size, const GLvoid* data) noexcept
{
    this->_count = renderCount;
    bind();
    glBufferData(this->_glType, size, data, this->_glUsage);
}

void VertexBufferShared::draw() const noexcept
{
    glDrawArrays(GL_TRIANGLES, 0, this->_count);
}

void VertexBufferShared::drawIndexed() const noexcept
{
    glDrawElements(GL_TRIANGLES, this->_count, GL_UNSIGNED_INT, nullptr);
}

void VertexBufferShared::draw(RenderingPipeline& rp) const noexcept
{
    rp.pushGLDrawArrays(GL_TRIANGLES, 0, this->_count);
}

void VertexBufferShared::drawIndexed(RenderingPipeline& rp) const noexcept
{
    rp.pushGLDrawElements(GL_TRIANGLES, this->_count, GL_UNSIGNED_INT, nullptr);
}
