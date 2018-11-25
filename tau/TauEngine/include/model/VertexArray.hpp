#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <DLL.hpp>

enum DataType
{
    Byte = 0,
    UByte,
    Short,
    UShort,
    Int,
    UInt,
    HalfFloat,
    Float,
    Double,
    Fixed,
    Int2_10_10_10_Rev,
    UInt2_10_10_10_Rev,
    UInt10F_11F_11F_Rev
};

TAU_DLL GLenum getGLType(DataType type) noexcept;

TAU_DLL DataType getType(GLenum type) noexcept;

class TAU_DLL VertexArray
{
private:
    GLuint _array;
public:
    static void unbind() noexcept;

    static void setAttribute(GLuint index, GLuint size, DataType type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) noexcept;

    static void enableAttribute(GLuint index) noexcept;
public:
    VertexArray() noexcept;
    VertexArray(const VertexArray& copy) noexcept = delete;
    VertexArray(VertexArray&& move) noexcept = delete;

    ~VertexArray() noexcept;

    VertexArray& operator=(const VertexArray& copy) noexcept = delete;
    VertexArray& operator=(VertexArray&& move) noexcept = delete;

    inline GLuint array() const noexcept { return _array; }
    inline operator GLuint() const noexcept { return _array; }
    inline GLuint operator()() const noexcept { return _array; }

    void bind() const noexcept;
};
