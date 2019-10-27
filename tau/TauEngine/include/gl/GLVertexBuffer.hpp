#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/VertexBuffer.hpp"

class TAU_DLL GLVertexBuffer final : public IVertexBuffer
{
    DELETE_COPY(GLVertexBuffer);
public:
    static GLenum getGLType(Type bt) noexcept;

    static Type getType(GLenum bt) noexcept;

    static GLenum getGLUsageType(UsageType usage) noexcept;

    static UsageType getUsageType(GLenum usage) noexcept;
private:
    GLuint _buffer;
    GLenum _glType;
    GLenum _glUsage;
    GLsizei _count;
public:
    GLVertexBuffer(Type type, UsageType usage) noexcept;

    ~GLVertexBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void fillBuffer(IRenderingContext& context, std::size_t renderCount, std::ptrdiff_t size, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, std::size_t renderCount, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;

    void draw(IRenderingContext& context) noexcept override;

    void drawIndexed(IRenderingContext& context) noexcept override;
};
