#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/IBuffer.hpp"

class TAU_DLL GLBuffer final : public IBuffer
{
    DELETE_COPY(GLBuffer);
public:
    static GLenum getGLType(EBuffer::Type bt) noexcept;

    static EBuffer::Type getType(GLenum bt) noexcept;

    static GLenum getGLUsageType(EBuffer::UsageType usage) noexcept;

    static EBuffer::UsageType getUsageType(GLenum usage) noexcept;

    static GLuint createBuffer() noexcept;

    static void destroyBuffer(GLuint bufferHandle) noexcept;
private:
    GLuint _buffer;
    GLenum _glType;
    GLenum _glUsage;
private:
    GLBuffer(EBuffer::Type type, EBuffer::UsageType usage, uSys bufferSize, const BufferDescriptor& descriptor, GLuint buffer, GLenum glType, GLenum glUsage) noexcept;
public:
    ~GLBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;
private:
    friend class GLBufferBuilder;
};

class TAU_DLL GLIndexBuffer final : public IIndexBuffer
{
    DELETE_COPY(GLIndexBuffer);
private:
    GLuint _buffer;
    GLenum _glUsage;
private:
    GLIndexBuffer(EBuffer::UsageType usage, uSys bufferSize, GLuint buffer, GLenum glUsage) noexcept;
public:
    ~GLIndexBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;
private:
    friend class GLIndexBufferBuilder;
};

class TAU_DLL GLBufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(GLBufferBuilder);
    DELETE_COPY(GLBufferBuilder);
private:
    GLenum _glType;
    GLenum _glUsage;
public:
    GLBufferBuilder(uSys descriptorCount) noexcept;

    void type(const EBuffer::Type type) noexcept override;
    void usage(const EBuffer::UsageType usage) noexcept override;

    [[nodiscard]] IBuffer* build([[tau::out]] Error* error) const noexcept override;
};

class TAU_DLL GLIndexBufferBuilder final : public IIndexBufferBuilder
{
    DEFAULT_DESTRUCT(GLIndexBufferBuilder);
    DELETE_COPY(GLIndexBufferBuilder);
private:
    GLenum _glUsage;
public:
    GLIndexBufferBuilder() noexcept;

    void usage(const EBuffer::UsageType usage) noexcept override;

    [[nodiscard]] IIndexBuffer* build([[tau::out]] Error* error) const noexcept override;
};
