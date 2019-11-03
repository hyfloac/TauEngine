#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/IBuffer.hpp"

class TAU_DLL GLBuffer final : public IBuffer
{
    DELETE_COPY(GLBuffer);
public:
    static GLenum getGLType(Type bt) noexcept;

    static Type getType(GLenum bt) noexcept;

    static GLenum getGLUsageType(UsageType usage) noexcept;

    static UsageType getUsageType(GLenum usage) noexcept;
private:
    GLuint _buffer;
    GLenum _glType;
    GLenum _glUsage;
public:
    GLBuffer(Type type, UsageType usage, std::size_t descriptorCount) noexcept;

    ~GLBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void fillBuffer(IRenderingContext& context, std::ptrdiff_t size, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;
};

class TAU_DLL GLIndexBuffer final : public IIndexBuffer
{
    DELETE_COPY(GLIndexBuffer);
private:
    GLuint _buffer;
    GLenum _glUsage;
public:
    GLIndexBuffer(IBuffer::UsageType usage) noexcept;

    ~GLIndexBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void fillBuffer(IRenderingContext& context, std::ptrdiff_t size, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;
};
