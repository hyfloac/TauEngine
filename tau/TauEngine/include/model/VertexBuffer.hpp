#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <DLL.hpp>

enum BufferType : u8
{
    ArrayBuffer = 0,
    AtomicCounterBuffer,
    CopyReadBuffer,
    CopyWriteBuffer,
    DispatchIndirectBuffer,
    DrawIndirectBuffer,
    ElementArrayBuffer,
    PixelPackBuffer,
    PixelUnpackBuffer,
    QueryBuffer,
    ShaderStorageBuffer,
    TextureBuffer,
    TransformFeedbackBuffer,
    UniformBuffer
};

enum BufferUsageType : u8
{
    StreamDraw = 0,
    StreamRead,
    StreamCopy,
    StaticDraw,
    StaticRead,
    StaticCopy,
    DynamicDraw,
    DynamicRead,
    DynamicCopy
};

TAU_DLL GLenum getGLBufferType(BufferType bt) noexcept;

TAU_DLL BufferType getBufferType(GLenum bt) noexcept;

TAU_DLL GLenum getGLBufferUsageType(BufferUsageType usage) noexcept;

TAU_DLL BufferUsageType getBufferUsageType(GLenum usage) noexcept;

class TAU_DLL VertexBuffer
{
private:
    GLuint  _buffer;
    GLenum  _glType;
    GLenum  _glUsage;
    GLsizei _count;
public:
    VertexBuffer(BufferType type, BufferUsageType usage = StaticDraw) noexcept;
    VertexBuffer(const VertexBuffer& copy) noexcept = delete;
    VertexBuffer(VertexBuffer&& move) noexcept = delete;

    ~VertexBuffer() noexcept;

    VertexBuffer& operator=(const VertexBuffer& copy) noexcept = delete;
    VertexBuffer& operator=(VertexBuffer&& move) noexcept = delete;

    inline GLuint buffer() const noexcept { return _buffer; }
    inline operator GLuint() const noexcept { return _buffer; }
    inline GLuint operator()() const noexcept { return _buffer; }

    void bind() const noexcept;

    void unbind() const noexcept;

    void fillBuffer(GLsizei renderCount, GLsizeiptr size, const GLvoid* data) noexcept;

    void draw() const noexcept;

    void drawIndexed() const noexcept;
};
