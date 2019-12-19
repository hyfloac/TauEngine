#include "gl/GLBuffer.hpp"
#include <Safeties.hpp>

GLBuffer::GLBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, const GLuint buffer, const GLenum glType, const GLenum glUsage) noexcept
    : IBuffer(type, usage, bufferSize, descriptor),
      _buffer(buffer), _glType(glType), _glUsage(glUsage)
{ }

GLBuffer::~GLBuffer() noexcept
{ GLBuffer::destroyBuffer(_buffer); }

void GLBuffer::bind(IRenderingContext& context) noexcept
{
    glBindBuffer(this->_glType, this->_buffer);
}

void GLBuffer::unbind(IRenderingContext& context) noexcept
{
    glBindBuffer(this->_glType, 0);
}

void GLBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    glBufferData(this->_glType, _bufferSize, data, this->_glUsage);
}

void GLBuffer::modifyBuffer(IRenderingContext& context, const intptr_t offset, const std::ptrdiff_t size, const void* const data) noexcept
{
    glBufferSubData(this->_glType, offset, size, data);
}

GLIndexBuffer::GLIndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const GLuint buffer, const GLenum glUsage) noexcept
    : IIndexBuffer(usage, bufferSize),
      _buffer(buffer),  _glUsage(glUsage)
{ }

GLIndexBuffer::~GLIndexBuffer() noexcept
{ GLBuffer::destroyBuffer(_buffer); }

void GLIndexBuffer::bind(IRenderingContext& context) noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_buffer);
}

void GLIndexBuffer::unbind(IRenderingContext& context) noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _bufferSize, data, this->_glUsage);
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

void GLBuffer::destroyBuffer(GLuint bufferHandle) noexcept
{
    glDeleteBuffers(1, &bufferHandle);
}

GLBufferBuilder::GLBufferBuilder(uSys descriptorCount) noexcept
    : IBufferBuilder(descriptorCount),
      _glType(0), _glUsage(0)
{ }

void GLBufferBuilder::type(const EBuffer::Type type) noexcept
{
    IBufferBuilder::type(type);
    _glType = GLBuffer::getGLType(type);
}

void GLBufferBuilder::usage(const EBuffer::UsageType usage) noexcept
{
    IBufferBuilder::usage(usage);
    _glUsage = GLBuffer::getGLUsageType(usage);
}

IBuffer* GLBufferBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_type == static_cast<EBuffer::Type>(0), Error::TypeIsUnset);
    ERROR_CODE_COND_N(_usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_N(_type == EBuffer::Type::IndexBuffer, Error::BufferCannotBeIndexBuffer);
    ERROR_CODE_COND_N(_bufferSize == 0, Error::BufferSizeIsZero);

    const GLuint bufferHandle = GLBuffer::createBuffer();

    GLBuffer* const buffer = new(std::nothrow) GLBuffer(_type, _usage, _bufferSize, _descriptor, bufferHandle, _glType, _glUsage);

    if(!buffer)
    {
        GLBuffer::destroyBuffer(bufferHandle);
        ERROR_CODE_N(Error::MemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

GLIndexBufferBuilder::GLIndexBufferBuilder() noexcept
    : IIndexBufferBuilder(), _glUsage(0)
{ }

void GLIndexBufferBuilder::usage(const EBuffer::UsageType usage) noexcept
{
    IIndexBufferBuilder::usage(usage);
    _glUsage = GLBuffer::getGLUsageType(usage);
}

IIndexBuffer* GLIndexBufferBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_N(_bufferSize == 0, Error::BufferSizeIsZero);

    const GLuint bufferHandle = GLBuffer::createBuffer();

    GLIndexBuffer* buffer =  new(std::nothrow) GLIndexBuffer(_usage, _bufferSize, bufferHandle, _glUsage);

    if(!buffer)
    {
        GLBuffer::destroyBuffer(bufferHandle);
        ERROR_CODE_N(Error::MemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, buffer);
}

GLenum GLBuffer::getGLType(const EBuffer::Type bt) noexcept
{
    switch(bt)
    {
        case EBuffer::Type::ArrayBuffer:             return GL_ARRAY_BUFFER;
        case EBuffer::Type::AtomicCounterBuffer:     return GL_ATOMIC_COUNTER_BUFFER;
        case EBuffer::Type::CopyReadBuffer:          return GL_COPY_READ_BUFFER;
        case EBuffer::Type::CopyWriteBuffer:         return GL_COPY_WRITE_BUFFER;
        case EBuffer::Type::DispatchIndirectBuffer:  return GL_DISPATCH_INDIRECT_BUFFER;
        case EBuffer::Type::DrawIndirectBuffer:      return GL_DRAW_INDIRECT_BUFFER;
        case EBuffer::Type::ElementArrayBuffer:      return GL_ELEMENT_ARRAY_BUFFER;
        case EBuffer::Type::PixelPackBuffer:         return GL_PIXEL_PACK_BUFFER;
        case EBuffer::Type::PixelUnpackBuffer:       return GL_PIXEL_UNPACK_BUFFER;
        case EBuffer::Type::QueryBuffer:             return GL_QUERY_BUFFER;
        case EBuffer::Type::ShaderStorageBuffer:     return GL_SHADER_STORAGE_BUFFER;
        case EBuffer::Type::TextureBuffer:           return GL_TEXTURE_BUFFER;
        case EBuffer::Type::TransformFeedbackBuffer: return GL_TRANSFORM_FEEDBACK_BUFFER;
        case EBuffer::Type::UniformBuffer:           return GL_UNIFORM_BUFFER;
        default:                            return static_cast<GLenum>(-1);
    }
}

EBuffer::Type GLBuffer::getType(const GLenum bt) noexcept
{
    switch(bt)
    {
        case GL_ARRAY_BUFFER:              return EBuffer::Type::ArrayBuffer;
        case GL_ATOMIC_COUNTER_BUFFER:     return EBuffer::Type::AtomicCounterBuffer;
        case GL_COPY_READ_BUFFER:          return EBuffer::Type::CopyReadBuffer;
        case GL_COPY_WRITE_BUFFER:         return EBuffer::Type::CopyWriteBuffer;
        case GL_DISPATCH_INDIRECT_BUFFER:  return EBuffer::Type::DispatchIndirectBuffer;
        case GL_DRAW_INDIRECT_BUFFER:      return EBuffer::Type::DrawIndirectBuffer;
        case GL_ELEMENT_ARRAY_BUFFER:      return EBuffer::Type::ElementArrayBuffer;
        case GL_PIXEL_PACK_BUFFER:         return EBuffer::Type::PixelPackBuffer;
        case GL_PIXEL_UNPACK_BUFFER:       return EBuffer::Type::PixelUnpackBuffer;
        case GL_QUERY_BUFFER:              return EBuffer::Type::QueryBuffer;
        case GL_SHADER_STORAGE_BUFFER:     return EBuffer::Type::ShaderStorageBuffer;
        case GL_TEXTURE_BUFFER:            return EBuffer::Type::TextureBuffer;
        case GL_TRANSFORM_FEEDBACK_BUFFER: return EBuffer::Type::TransformFeedbackBuffer;
        case GL_UNIFORM_BUFFER:            return EBuffer::Type::UniformBuffer;
        default:                           return static_cast<EBuffer::Type>(-1);
    }
}

GLenum GLBuffer::getGLUsageType(const EBuffer::UsageType usage) noexcept
{
    switch(usage)
    {
        case EBuffer::UsageType::StreamDraw:  return GL_STREAM_DRAW;
        case EBuffer::UsageType::StreamRead:  return GL_STREAM_READ;
        case EBuffer::UsageType::StreamCopy:  return GL_STREAM_COPY;
        case EBuffer::UsageType::StaticDraw:  return GL_STATIC_DRAW;
        case EBuffer::UsageType::StaticRead:  return GL_STATIC_READ;
        case EBuffer::UsageType::StaticCopy:  return GL_STATIC_COPY;
        case EBuffer::UsageType::DynamicDraw: return GL_DYNAMIC_DRAW;
        case EBuffer::UsageType::DynamicRead: return GL_DYNAMIC_READ;
        case EBuffer::UsageType::DynamicCopy: return GL_DYNAMIC_COPY;
        default:                     return static_cast<GLenum>(-1);
    }
}

EBuffer::UsageType GLBuffer::getUsageType(const GLenum usage) noexcept
{
    switch(usage)
    {
        case  GL_STREAM_DRAW: return EBuffer::UsageType::StreamDraw;
        case  GL_STREAM_READ: return EBuffer::UsageType::StreamRead;
        case  GL_STREAM_COPY: return EBuffer::UsageType::StreamCopy;
        case  GL_STATIC_DRAW: return EBuffer::UsageType::StaticDraw;
        case  GL_STATIC_READ: return EBuffer::UsageType::StaticRead;
        case  GL_STATIC_COPY: return EBuffer::UsageType::StaticCopy;
        case GL_DYNAMIC_DRAW: return EBuffer::UsageType::DynamicDraw;
        case GL_DYNAMIC_READ: return EBuffer::UsageType::DynamicRead;
        case GL_DYNAMIC_COPY: return EBuffer::UsageType::DynamicCopy;
        default:              return static_cast<EBuffer::UsageType>(-1);
    }
}
