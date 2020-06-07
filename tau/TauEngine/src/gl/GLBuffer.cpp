#include "gl/GLBuffer.hpp"
#include "gl/gl4_5/GLBuffer4_5.hpp"
#include "TauEngine.hpp"
#include <Safeties.hpp>

GLVertexBuffer::~GLVertexBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void GLVertexBuffer::bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
}

void GLVertexBuffer::unbind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool GLVertexBuffer::beginModification(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
        ++_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount > 1)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
            return false;
        }
  #endif
#endif
    bind();
    return true;
}

void GLVertexBuffer::endModification(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
    if(_modificationLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyEnd);
    }
  #endif
#endif
    unbind();
}

void GLVertexBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    if(_modificationLockCount <= 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedWithoutBegin);
        return;
    }
  #endif
#endif

    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void GLVertexBuffer::fillBuffer(IRenderingContext&, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
    if(_modificationLockCount > 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::FilledWhileModifying);
        return;
    }
  #endif
#endif

    bind();
    glBufferData(GL_ARRAY_BUFFER, _bufferSize, data, _glUsage);
    unbind();
}

GLIndexBuffer::~GLIndexBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void GLIndexBuffer::bind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
}

void GLIndexBuffer::unbind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool GLIndexBuffer::beginModification(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
        ++_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount > 1)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
            return false;
        }
  #endif
#endif
    bind();
    return true;
}

void GLIndexBuffer::endModification(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
    if(_modificationLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyEnd);
    }
  #endif
#endif
    unbind();
}

void GLIndexBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    if(_modificationLockCount <= 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedWithoutBegin);
        return;
    }
  #endif
#endif

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void GLIndexBuffer::fillBuffer(IRenderingContext&, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
    if(_modificationLockCount > 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::FilledWhileModifying);
        return;
    }
  #endif
#endif

    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _bufferSize, data, _glUsage);
    unbind();
}

GLUniformBuffer::~GLUniformBuffer() noexcept
{
    glDeleteBuffers(1, &_buffer);
}

void GLUniformBuffer::bind() const noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, _buffer);
}

void GLUniformBuffer::unbind() const noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformBuffer::bind(const u32 index) const noexcept
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, _buffer);
}

void GLUniformBuffer::unbind(const u32 index) const noexcept
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
}

void GLUniformBuffer::bind(IRenderingContext&, EShader::Stage, const u32 index) const noexcept
{
#if TAU_BUFFER_SAFETY
    ++_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND
    if(_uniformBindLockCount > 1)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferBind);
    }
  #endif
#endif
    bind(index);
}

void GLUniformBuffer::unbind(IRenderingContext&, EShader::Stage, const u32 index) const noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferUnbind);
    }
  #endif
#endif
    unbind(index);
}

void GLUniformBuffer::fastUnbind() const noexcept
{
#if TAU_BUFFER_SAFETY
    --_uniformBindLockCount;
  #if TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND
    if(_uniformBindLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleUniformBufferUnbind);
    }
  #endif
#endif
}

bool GLUniformBuffer::beginModification(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
        ++_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
        if(_modificationLockCount > 1)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyBegin);
            return false;
        }
  #endif
#endif
    bind();
    return true;
}

void GLUniformBuffer::endModification(IRenderingContext&) noexcept
{
#if TAU_BUFFER_SAFETY
    --_modificationLockCount;
  #if TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
    if(_modificationLockCount < 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::DoubleModifyEnd);
    }
  #endif
#endif
    unbind();
}

void GLUniformBuffer::modifyBuffer(const uSys offset, const uSys size, const void* const data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    if(_modificationLockCount <= 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::ModifiedWithoutBegin);
        return;
    }
  #endif
#endif

    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void GLUniformBuffer::fillBuffer(IRenderingContext&, const void* data) noexcept
{
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
    if(_modificationLockCount > 0)
    {
        TAU_THROW(BufferSafetyException, BufferSafetyException::FilledWhileModifying);
        return;
    }
  #endif
#endif

    bind();
    glBufferData(GL_UNIFORM_BUFFER, _bufferSize, data, _glUsage);
    unbind();
}

GLuint GLBufInterface::createBuffer() const noexcept
{
    GLuint bufferHandle;
    glGenBuffers(1, &bufferHandle);
    return bufferHandle;
}

GLuint GLBuf4_5Interface::createBuffer() const noexcept
{
    GLuint bufferHandle;
    glCreateBuffers(1, &bufferHandle);
    return bufferHandle;
}

GLVertexBuffer* GLBufferBuilder::build(const VertexBufferArgs& args, Error* const error) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLVertexBuffer* const buffer = new(std::nothrow) GLVertexBuffer(args.usage, args.bufferSize(), args.descriptor.build(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

GLVertexBuffer* GLBufferBuilder::build(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLVertexBuffer* const buffer = allocator.allocateT<GLVertexBuffer>(args.usage, args.bufferSize(), args.descriptor.build(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IVertexBuffer> GLBufferBuilder::buildCPPRef(const VertexBufferArgs& args, Error* const error) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLVertexBuffer> buffer(new(std::nothrow) GLVertexBuffer(args.usage, args.bufferSize(), args.descriptor.build(), glArgs));

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IVertexBuffer> GLBufferBuilder::buildTauRef(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLVertexBuffer> buffer(allocator, args.usage, args.bufferSize(), args.descriptor.build(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, RCPCast<IVertexBuffer>(buffer));
}

NullableStrongRef<IVertexBuffer> GLBufferBuilder::buildTauSRef(const VertexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLVertexBuffer> buffer(allocator, args.usage, args.bufferSize(), args.descriptor.build(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, RCPCast<IVertexBuffer>(buffer));
}

GLIndexBuffer* GLBufferBuilder::build(const IndexBufferArgs& args, Error* const error) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLIndexBuffer* const buffer = new(std::nothrow) GLIndexBuffer(args.usage, args.indexSize, args.bufferSize(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

GLIndexBuffer* GLBufferBuilder::build(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLIndexBuffer* const buffer = allocator.allocateT<GLIndexBuffer>(args.usage, args.indexSize, args.bufferSize(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IIndexBuffer> GLBufferBuilder::buildCPPRef(const IndexBufferArgs& args, Error* const error) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLIndexBuffer> buffer(new(std::nothrow) GLIndexBuffer(args.usage, args.indexSize, args.bufferSize(), glArgs));

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IIndexBuffer> GLBufferBuilder::buildTauRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLIndexBuffer> buffer(allocator, args.usage, args.indexSize, args.bufferSize(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, RCPCast<IIndexBuffer>(buffer));
}

NullableStrongRef<IIndexBuffer> GLBufferBuilder::buildTauSRef(const IndexBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLIndexBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLIndexBuffer> buffer(allocator, args.usage, args.indexSize, args.bufferSize(), glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, RCPCast<IIndexBuffer>(buffer));
}

GLUniformBuffer* GLBufferBuilder::build(const UniformBufferArgs& args, Error* const error) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLUniformBuffer* const buffer = new(std::nothrow) GLUniformBuffer(args.usage, args.bufferSize, glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

GLUniformBuffer* GLBufferBuilder::build(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLUniformBuffer* const buffer = allocator.allocateT<GLUniformBuffer>(args.usage, args.bufferSize, glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

CPPRef<IUniformBuffer> GLBufferBuilder::buildCPPRef(const UniformBufferArgs& args, Error* const error) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLUniformBuffer> buffer(new(std::nothrow) GLUniformBuffer(args.usage, args.bufferSize, glArgs));

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, buffer);
}

NullableRef<IUniformBuffer> GLBufferBuilder::buildTauRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLUniformBuffer> buffer(allocator, args.usage, args.bufferSize, glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, RCPCast<IUniformBuffer>(buffer));
}

NullableStrongRef<IUniformBuffer> GLBufferBuilder::buildTauSRef(const UniformBufferArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLUniformBuffer> buffer(allocator, args.usage, args.bufferSize, glArgs);

    if(!buffer)
    {
        glDeleteBuffers(1, &glArgs.buffer);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    initBuffer(args, glArgs);

    ERROR_CODE_V(Error::NoError, RCPCast<IUniformBuffer>(buffer));
}

bool GLBufferBuilder::processArgs(const VertexBufferArgs& args, GLBufferArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    glArgs->glUsage = GLVertexBuffer::getGLUsageType(args.usage);
    glArgs->buffer = _glInterface.createBuffer();

    return true;
}

bool GLBufferBuilder::processArgs(const IndexBufferArgs& args, GLIndexBufferArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.elementCount == 0, Error::BufferSizeIsZero);

    glArgs->glUsage = GLVertexBuffer::getGLUsageType(args.usage);
    glArgs->glIndexSize = GLIndexBuffer::glIndexSize(args.indexSize);
    glArgs->buffer = _glInterface.createBuffer();

    return true;
}

bool GLBufferBuilder::processArgs(const UniformBufferArgs& args, GLBufferArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.usage == static_cast<EBuffer::UsageType>(0), Error::UsageIsUnset);
    ERROR_CODE_COND_F(args.bufferSize == 0, Error::BufferSizeIsZero);

    glArgs->glUsage = GLVertexBuffer::getGLUsageType(args.usage);
    glArgs->buffer = _glInterface.createBuffer();

    return true;
}

void GLBufferBuilder::initBuffer(const VertexBufferArgs& args, const GLBufferArgs& glArgs) noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, glArgs.buffer);
    glBufferData(GL_ARRAY_BUFFER, args.bufferSize(), args.initialBuffer, glArgs.glUsage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLBufferBuilder::initBuffer(const IndexBufferArgs& args, const GLIndexBufferArgs& glArgs) noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glArgs.buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, args.bufferSize(), args.initialBuffer, glArgs.glUsage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLBufferBuilder::initBuffer(const UniformBufferArgs& args, const GLBufferArgs& glArgs) noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, glArgs.buffer);
    glBufferData(GL_UNIFORM_BUFFER, args.bufferSize, args.initialBuffer, glArgs.glUsage);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLenum GLVertexBuffer::getGLUsageType(const EBuffer::UsageType usage) noexcept
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

EBuffer::UsageType GLVertexBuffer::getUsageType(const GLenum usage) noexcept
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

GLenum GLIndexBuffer::glIndexSize(const EBuffer::IndexSize indexSize) noexcept
{
    switch(indexSize)
    {
        case EBuffer::IndexSize::Uint32: return GL_UNSIGNED_INT;
        case EBuffer::IndexSize::Uint16: return GL_UNSIGNED_SHORT;
        default: return 0;
    }
}
