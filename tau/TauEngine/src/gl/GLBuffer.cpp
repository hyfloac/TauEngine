#include "gl/GLBuffer.hpp"
#include "gl/gl4_5/GLBuffer4_5.hpp"
#include <Safeties.hpp>

GLBuffer::GLBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const bool instanced, const BufferDescriptor& descriptor, const GLuint buffer, const GLenum glType, const GLenum glUsage) noexcept
    : IBuffer(type, usage, bufferSize, instanced, descriptor),
      _buffer(buffer), _glType(glType), _glUsage(glUsage)
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

void GLBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    bind(context);
    glBufferData(this->_glType, _bufferSize, data, this->_glUsage);
    unbind(context);
}

void GLBuffer::beginModification(IRenderingContext& context) noexcept
{
    bind(context);
}

void GLBuffer::endModification(IRenderingContext& context) noexcept
{
    unbind(context);
}

void GLBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
{
    glBufferSubData(this->_glType, offset, size, data);
}

GLIndexBuffer::GLIndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const GLuint buffer, const GLenum glUsage) noexcept
    : IIndexBuffer(usage, bufferSize),
      _buffer(buffer),  _glUsage(glUsage)
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

void GLIndexBuffer::fillBuffer(IRenderingContext& context, const void* const data) noexcept
{
    bind(context);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _bufferSize, data, this->_glUsage);
    unbind(context);
}

void GLIndexBuffer::beginModification(IRenderingContext& context) noexcept
{
    bind(context);
}

void GLIndexBuffer::endModification(IRenderingContext& context) noexcept
{
    unbind(context);
}

void GLIndexBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

GLUniformBuffer::GLUniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const GLuint buffer, const GLenum glUsage) noexcept
    : IUniformBuffer(usage, bufferSize),
      _buffer(buffer), _glUsage(glUsage)
{ }

GLUniformBuffer::~GLUniformBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void GLUniformBuffer::bind(IRenderingContext& context) noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, _buffer);
}

void GLUniformBuffer::unbind(IRenderingContext& context) noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformBuffer::bind(IRenderingContext& context, EShader::Stage, const u32 index) noexcept
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, _buffer);
}

void GLUniformBuffer::unbind(IRenderingContext& context, EShader::Stage, const u32 index) noexcept
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
}

void GLUniformBuffer::fillBuffer(IRenderingContext& context, const void* data) noexcept
{
    bind(context);
    glBufferData(GL_UNIFORM_BUFFER, _bufferSize, data, this->_glUsage);
    unbind(context);
}

void GLUniformBuffer::beginModification(IRenderingContext& context) noexcept
{
    bind(context);
}

void GLUniformBuffer::endModification(IRenderingContext& context) noexcept
{
    unbind(context);
}

void GLUniformBuffer::modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

GLuint GLBuffer::createBuffer() noexcept
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

GLBuffer* GLBufferBuilder::build(const BufferArgs& args, Error* error) const noexcept
{
    GLBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    GLBuffer* const buffer = new(std::nothrow) GLBuffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), glArgs.bufferHandle, glArgs.glType, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

GLBuffer* GLBufferBuilder::build(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    GLBuffer* const buffer = allocator.allocateT<GLBuffer>(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), glArgs.bufferHandle, glArgs.glType, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

Ref<IBuffer> GLBufferBuilder::buildCPPRef(const BufferArgs& args, Error* error) const noexcept
{
    GLBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const Ref<GLBuffer> buffer = Ref<GLBuffer>(new(std::nothrow) GLBuffer(args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), glArgs.bufferHandle, glArgs.glType, glArgs.glUsage));

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableReferenceCountingPointer<IBuffer> GLBufferBuilder::buildTauRef(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const NullableReferenceCountingPointer<GLBuffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), glArgs.bufferHandle, glArgs.glType, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    NullableReferenceCountingPointer<IBuffer> iBuffer = RCPCast<IBuffer>(buffer);

    ERROR_CODE_V(Error::NoError, iBuffer);
}

NullableStrongReferenceCountingPointer<IBuffer> GLBufferBuilder::buildTauSRef(const BufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<GLBuffer> buffer(allocator, args.type, args.usage, args.bufferSize(), args.instanced, args.descriptor.build(), glArgs.bufferHandle, glArgs.glType, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    NullableStrongReferenceCountingPointer<IBuffer> iBuffer = RCPCast<IBuffer>(buffer);

    ERROR_CODE_V(Error::NoError, iBuffer);
}

bool GLBufferBuilder::processBufferArgs(const BufferArgs& args, GLBufferArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.type == static_cast<EBuffer::Type>(0), Error::TypeIsUnset);
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.type == EBuffer::Type::IndexBuffer, Error::BufferCannotBeIndexBuffer);
    ERROR_CODE_COND_F(args.type == EBuffer::Type::UniformBuffer, Error::BufferCannotBeUniformBuffer);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    glArgs->glType = GLBuffer::getGLType(args.type);
    glArgs->glUsage = GLBuffer::getGLUsageType(args.usage);
    glArgs->bufferHandle = createBuffer();

    return true;
}

void GLBufferBuilder::initBuffer(const BufferArgs& args, const GLBufferArgs& glArgs) noexcept
{
    glBindBuffer(glArgs.glType, glArgs.bufferHandle);
    glBufferData(glArgs.glType, args.bufferSize(), args.initialBuffer, glArgs.glUsage);
    glBindBuffer(glArgs.glType, 0);
}

GLIndexBuffer* GLIndexBufferBuilder::build(const IndexBufferArgs& args, Error* error) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    GLIndexBuffer* const buffer = new(std::nothrow) GLIndexBuffer(args.usage, args.bufferSize(), glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

GLIndexBuffer* GLIndexBufferBuilder::build(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    GLIndexBuffer* const buffer = allocator.allocateT<GLIndexBuffer>(args.usage, args.bufferSize(), glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

Ref<IIndexBuffer> GLIndexBufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* error) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const Ref<GLIndexBuffer> buffer = Ref<GLIndexBuffer>(new(std::nothrow) GLIndexBuffer(args.usage, args.bufferSize(), glArgs.bufferHandle, glArgs.glUsage));

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableReferenceCountingPointer<IIndexBuffer> GLIndexBufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const NullableReferenceCountingPointer<GLIndexBuffer> buffer(allocator, args.usage, args.bufferSize(), glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    NullableReferenceCountingPointer<IIndexBuffer> iBuffer = RCPCast<IIndexBuffer>(buffer);

    ERROR_CODE_V(Error::NoError, iBuffer);
}

NullableStrongReferenceCountingPointer<IIndexBuffer> GLIndexBufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<GLIndexBuffer> buffer(allocator, args.usage, args.bufferSize(), glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    NullableStrongReferenceCountingPointer<IIndexBuffer> iBuffer = RCPCast<IIndexBuffer>(buffer);

    ERROR_CODE_V(Error::NoError, iBuffer);
}

bool GLIndexBufferBuilder::processBufferArgs(const IndexBufferArgs& args, GLIndexBufferArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    glArgs->glUsage = GLBuffer::getGLUsageType(args.usage);
    glArgs->bufferHandle = createBuffer();

    return true;
}

void GLIndexBufferBuilder::initBuffer(const IndexBufferArgs& args, const GLIndexBufferArgs& glArgs) noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glArgs.bufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, args.bufferSize(), args.initialBuffer, glArgs.glUsage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLUniformBuffer* GLUniformBufferBuilder::build(const UniformBufferArgs& args, Error* error) const noexcept
{
    GLUniformBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    GLUniformBuffer* const buffer = new(std::nothrow) GLUniformBuffer(args.usage, args.bufferSize, glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

GLUniformBuffer* GLUniformBufferBuilder::build(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLUniformBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    GLUniformBuffer* const buffer = allocator.allocateT<GLUniformBuffer>(args.usage, args.bufferSize, glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

Ref<IUniformBuffer> GLUniformBufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* error) const noexcept
{
    GLUniformBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const Ref<GLUniformBuffer> buffer = Ref<GLUniformBuffer>(new(std::nothrow) GLUniformBuffer(args.usage, args.bufferSize, glArgs.bufferHandle, glArgs.glUsage));

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableReferenceCountingPointer<IUniformBuffer> GLUniformBufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLUniformBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const NullableReferenceCountingPointer<GLUniformBuffer> buffer(allocator, args.usage, args.bufferSize, glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    NullableReferenceCountingPointer<IUniformBuffer> iBuffer = RCPCast<IUniformBuffer>(buffer);

    ERROR_CODE_V(Error::NoError, iBuffer);
}

NullableStrongReferenceCountingPointer<IUniformBuffer> GLUniformBufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLUniformBufferArgs glArgs;
    if(!processBufferArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<GLUniformBuffer> buffer(allocator, args.usage, args.bufferSize, glArgs.bufferHandle, glArgs.glUsage);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.bufferHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    NullableStrongReferenceCountingPointer<IUniformBuffer> iBuffer = RCPCast<IUniformBuffer>(buffer);

    ERROR_CODE_V(Error::NoError, iBuffer);
}

bool GLUniformBufferBuilder::processBufferArgs(const UniformBufferArgs& args, GLUniformBufferArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.bufferSize == 0, Error::BufferSizeIsZero);

    glArgs->glUsage = GLBuffer::getGLUsageType(args.usage);
    glArgs->bufferHandle = createBuffer();

    return true;
}

void GLUniformBufferBuilder::initBuffer(const UniformBufferArgs& args, const GLUniformBufferArgs& glArgs) noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, glArgs.bufferHandle);
    glBufferData(GL_UNIFORM_BUFFER, args.bufferSize, args.initialBuffer, glArgs.glUsage);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint GLBuffer4_5Builder::createBuffer() const noexcept
{
    GLuint bufferHandle;
    glCreateBuffers(1, &bufferHandle);
    return bufferHandle;
}

GLuint GLIndexBuffer4_5Builder::createBuffer() const noexcept
{
    GLuint bufferHandle;
    glCreateBuffers(1, &bufferHandle);
    return bufferHandle;
}

GLuint GLUniformBuffer4_5Builder::createBuffer() const noexcept
{
    GLuint bufferHandle;
    glCreateBuffers(1, &bufferHandle);
    return bufferHandle;
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
