#include "gl/GLBuffer.hpp"

GLBuffer::GLBuffer(const Type type, const UsageType usage, const std::size_t descriptorCount, GLuint buffer) noexcept
    : IBuffer(type, usage, descriptorCount),
      _buffer(buffer),
      _glType(getGLType(type)), _glUsage(getGLUsageType(usage))
{ }

GLBuffer::~GLBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void GLBuffer::bind(IRenderingContext& context) noexcept
{
    glBindBuffer(this->_glType, this->_buffer);
}

void GLBuffer::unbind(IRenderingContext& context) noexcept
{
    glBindBuffer(this->_glType, 0);
}

void GLBuffer::fillBuffer(IRenderingContext& context, const std::ptrdiff_t size, const void* const data) noexcept
{
    glBufferData(this->_glType, size, data, this->_glUsage);
}

void GLBuffer::modifyBuffer(IRenderingContext& context, const intptr_t offset, const std::ptrdiff_t size, const void* const data) noexcept
{
    glBufferSubData(this->_glType, offset, size, data);
}

GLIndexBuffer::GLIndexBuffer(const IBuffer::UsageType usage, const GLuint buffer) noexcept
    : IIndexBuffer(usage),
      _buffer(buffer),  _glUsage(GLBuffer::getGLUsageType(usage))
{ }

GLIndexBuffer::~GLIndexBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void GLIndexBuffer::bind(IRenderingContext& context) noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_buffer);
}

void GLIndexBuffer::unbind(IRenderingContext& context) noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::fillBuffer(IRenderingContext& context, const std::ptrdiff_t size, const void* const data) noexcept
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, this->_glUsage);
}

void GLIndexBuffer::modifyBuffer(IRenderingContext& context, const intptr_t offset, const std::ptrdiff_t size, const void* const data) noexcept
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

GLuint GLBuffer::createBuffer() noexcept
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

GLenum GLBuffer::getGLType(const Type bt) noexcept
{
    switch(bt)
    {
        case Type::ArrayBuffer:             return GL_ARRAY_BUFFER;
        case Type::AtomicCounterBuffer:     return GL_ATOMIC_COUNTER_BUFFER;
        case Type::CopyReadBuffer:          return GL_COPY_READ_BUFFER;
        case Type::CopyWriteBuffer:         return GL_COPY_WRITE_BUFFER;
        case Type::DispatchIndirectBuffer:  return GL_DISPATCH_INDIRECT_BUFFER;
        case Type::DrawIndirectBuffer:      return GL_DRAW_INDIRECT_BUFFER;
        case Type::ElementArrayBuffer:      return GL_ELEMENT_ARRAY_BUFFER;
        case Type::PixelPackBuffer:         return GL_PIXEL_PACK_BUFFER;
        case Type::PixelUnpackBuffer:       return GL_PIXEL_UNPACK_BUFFER;
        case Type::QueryBuffer:             return GL_QUERY_BUFFER;
        case Type::ShaderStorageBuffer:     return GL_SHADER_STORAGE_BUFFER;
        case Type::TextureBuffer:           return GL_TEXTURE_BUFFER;
        case Type::TransformFeedbackBuffer: return GL_TRANSFORM_FEEDBACK_BUFFER;
        case Type::UniformBuffer:           return GL_UNIFORM_BUFFER;
        default:                            return static_cast<GLenum>(-1);
    }
}

IBuffer::Type GLBuffer::getType(const GLenum bt) noexcept
{
    switch(bt)
    {
        case GL_ARRAY_BUFFER:              return Type::ArrayBuffer;
        case GL_ATOMIC_COUNTER_BUFFER:     return Type::AtomicCounterBuffer;
        case GL_COPY_READ_BUFFER:          return Type::CopyReadBuffer;
        case GL_COPY_WRITE_BUFFER:         return Type::CopyWriteBuffer;
        case GL_DISPATCH_INDIRECT_BUFFER:  return Type::DispatchIndirectBuffer;
        case GL_DRAW_INDIRECT_BUFFER:      return Type::DrawIndirectBuffer;
        case GL_ELEMENT_ARRAY_BUFFER:      return Type::ElementArrayBuffer;
        case GL_PIXEL_PACK_BUFFER:         return Type::PixelPackBuffer;
        case GL_PIXEL_UNPACK_BUFFER:       return Type::PixelUnpackBuffer;
        case GL_QUERY_BUFFER:              return Type::QueryBuffer;
        case GL_SHADER_STORAGE_BUFFER:     return Type::ShaderStorageBuffer;
        case GL_TEXTURE_BUFFER:            return Type::TextureBuffer;
        case GL_TRANSFORM_FEEDBACK_BUFFER: return Type::TransformFeedbackBuffer;
        case GL_UNIFORM_BUFFER:            return Type::UniformBuffer;
        default:                           return static_cast<Type>(-1);
    }
}

GLenum GLBuffer::getGLUsageType(const UsageType usage) noexcept
{
    switch(usage)
    {
        case UsageType::StreamDraw:  return GL_STREAM_DRAW;
        case UsageType::StreamRead:  return GL_STREAM_READ;
        case UsageType::StreamCopy:  return GL_STREAM_COPY;
        case UsageType::StaticDraw:  return GL_STATIC_DRAW;
        case UsageType::StaticRead:  return GL_STATIC_READ;
        case UsageType::StaticCopy:  return GL_STATIC_COPY;
        case UsageType::DynamicDraw: return GL_DYNAMIC_DRAW;
        case UsageType::DynamicRead: return GL_DYNAMIC_READ;
        case UsageType::DynamicCopy: return GL_DYNAMIC_COPY;
        default:                     return static_cast<GLenum>(-1);
    }
}

IBuffer::UsageType GLBuffer::getUsageType(const GLenum usage) noexcept
{
    switch(usage)
    {
        case  GL_STREAM_DRAW: return UsageType::StreamDraw;
        case  GL_STREAM_READ: return UsageType::StreamRead;
        case  GL_STREAM_COPY: return UsageType::StreamCopy;
        case  GL_STATIC_DRAW: return UsageType::StaticDraw;
        case  GL_STATIC_READ: return UsageType::StaticRead;
        case  GL_STATIC_COPY: return UsageType::StaticCopy;
        case GL_DYNAMIC_DRAW: return UsageType::DynamicDraw;
        case GL_DYNAMIC_READ: return UsageType::DynamicRead;
        case GL_DYNAMIC_COPY: return UsageType::DynamicCopy;
        default:              return static_cast<UsageType>(-1);
    }
}
