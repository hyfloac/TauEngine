#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/Buffer.hpp"

class TAU_DLL GLBuffer final : public IBuffer
{
    BUFFER_IMPL(GLBuffer);
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
    GLBuffer(EBuffer::Type type, EBuffer::UsageType usage, uSys bufferSize, bool instanced, const BufferDescriptor& descriptor, GLuint buffer, GLenum glType, GLenum glUsage) noexcept;
public:
    ~GLBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;

    [[nodiscard]] void* mapBuffer(IRenderingContext& context) noexcept override;
    void unmapBuffer(IRenderingContext& context) noexcept override;
private:
    friend class GLBufferBuilder;
};

class TAU_DLL GLIndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(GLIndexBuffer);
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

    [[nodiscard]] void* mapBuffer(IRenderingContext& context) noexcept override;
    void unmapBuffer(IRenderingContext& context) noexcept override;
private:
    friend class GLIndexBufferBuilder;
};

class TAU_DLL GLBufferBuilder : public IBufferBuilder
{
    DEFAULT_DESTRUCT(GLBufferBuilder);
    DELETE_COPY(GLBufferBuilder);
private:
    GLenum _glType;
    GLenum _glUsage;
public:
    GLBufferBuilder(uSys descriptorCount) noexcept;

    void type(const EBuffer::Type type) noexcept override final;
    void usage(const EBuffer::UsageType usage) noexcept override final;

    [[nodiscard]] IBuffer* build([[tau::out]] Error* error) const noexcept override final;
protected:
    virtual GLuint createBuffer() const noexcept
    { return GLBuffer::createBuffer(); }
};

class TAU_DLL GLIndexBufferBuilder : public IIndexBufferBuilder
{
    DEFAULT_DESTRUCT(GLIndexBufferBuilder);
    DELETE_COPY(GLIndexBufferBuilder);
private:
    GLenum _glUsage;
public:
    GLIndexBufferBuilder() noexcept;

    void usage(const EBuffer::UsageType usage) noexcept override final;

    [[nodiscard]] IIndexBuffer* build([[tau::out]] Error* error) const noexcept override final;
protected:
    virtual GLuint createBuffer() const noexcept
    { return GLBuffer::createBuffer(); }
};
