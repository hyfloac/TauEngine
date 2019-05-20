#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <RenderingPipeline.hpp>
#include <NumTypes.hpp>
#include <DLL.hpp>

enum class BufferType : u8
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

enum class BufferUsageType : u8
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

TAU_DLL GLenum getGLBufferType(const BufferType bt) noexcept;

TAU_DLL BufferType getBufferType(const GLenum bt) noexcept;

TAU_DLL GLenum getGLBufferUsageType(const BufferUsageType usage) noexcept;

TAU_DLL BufferUsageType getBufferUsageType(const GLenum usage) noexcept;

class TAU_DLL VertexBuffer final
{
private:
    GLuint  _buffer;
    GLenum  _glType;
    GLenum  _glUsage;
    GLsizei _count;
public:
    VertexBuffer(const BufferType type, const BufferUsageType usage = BufferUsageType::StaticDraw) noexcept;
    VertexBuffer(const VertexBuffer& copy) noexcept = delete;
    VertexBuffer(VertexBuffer&& move) noexcept = delete;

    ~VertexBuffer() noexcept;

    VertexBuffer& operator=(const VertexBuffer& copy) noexcept = delete;
    VertexBuffer& operator=(VertexBuffer&& move) noexcept = delete;

    inline GLuint buffer()     const noexcept { return _buffer; }
    inline operator GLuint()   const noexcept { return _buffer; }
    inline GLuint operator()() const noexcept { return _buffer; }

    inline GLsizei count() const noexcept { return _count; }

    void bind() const noexcept;

    void unbind() const noexcept;

    void fillBuffer(const GLsizei renderCount, const GLsizeiptr size, const GLvoid* data) noexcept;

    void draw() const noexcept;

    void drawIndexed() const noexcept;

    void draw(RenderingPipeline& rp) const noexcept;

    void drawIndexed(RenderingPipeline& rp) const noexcept;
};

class TAU_DLL VertexBufferShared final
{
private:
    GLuint  _buffer;
    GLenum  _glType;
    GLenum  _glUsage;
    GLsizei _count;
    u32*    _refCount;
public:
    VertexBufferShared(const BufferType type, const BufferUsageType usage = BufferUsageType::StaticDraw) noexcept;
    VertexBufferShared(const VertexBufferShared& copy) noexcept;
    VertexBufferShared(VertexBufferShared&& move) noexcept;

    ~VertexBufferShared() noexcept;

    VertexBufferShared& operator=(const VertexBufferShared& copy) noexcept;
    VertexBufferShared& operator=(VertexBufferShared&& move) noexcept;

    inline GLuint buffer()     const noexcept { return _buffer; }
    inline operator GLuint()   const noexcept { return _buffer; }
    inline GLuint operator()() const noexcept { return _buffer; }

    inline GLsizei count() const noexcept { return _count; }

    void bind() const noexcept;

    void unbind() const noexcept;

    void fillBuffer(const GLsizei renderCount, const GLsizeiptr size, const GLvoid* data) noexcept;

    void draw() const noexcept;

    void drawIndexed() const noexcept;

    void draw(RenderingPipeline& rp) const noexcept;

    void drawIndexed(RenderingPipeline& rp) const noexcept;
};
