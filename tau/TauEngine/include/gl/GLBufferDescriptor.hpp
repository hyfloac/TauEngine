#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <model/BufferDescriptor.hpp>

class TAU_DLL GLBufferDescriptor final : public IBufferDescriptor
{
private:
    GLuint _array;
    GLuint _maxVertexAttrib;
public:
    GLBufferDescriptor() noexcept
        : _array(), _maxVertexAttrib(0)
    {
        glGenVertexArrays(1, &_array);
    }

    ~GLBufferDescriptor() noexcept
    {
        glDeleteVertexArrays(1, &_array);
    }

    void addAttribute(u32 size, DataType type, bool normalized, i32 stride, const void* pointer) noexcept override
    {
        glVertexAttribPointer(_maxVertexAttrib++, size, getGLType(type), normalized, stride, pointer);
    }

    void initialize() noexcept override { /* NO-OP */ }

    void bind() const noexcept override final
    {
        glBindVertexArray(_array);
    }

    void unbind() const noexcept override final
    {
        glBindVertexArray(0);
    }

    void enableAttributes() const noexcept override final
    {
        for(GLuint attrib = 0; attrib < _maxVertexAttrib; ++attrib)
        {
            glEnableVertexAttribArray(attrib);
        }
    }

    void disableAttributes() const noexcept override final
    {
        for(GLuint attrib = _maxVertexAttrib; attrib > 0; --attrib)
        {
            glDisableVertexAttribArray(attrib - 1);
        }
    }
private:
    GLBufferDescriptor(const GLBufferDescriptor& copy) noexcept = delete;
    GLBufferDescriptor(GLBufferDescriptor&& move) noexcept = delete;

    GLBufferDescriptor& operator=(const GLBufferDescriptor& copy) noexcept = delete;
    GLBufferDescriptor& operator=(GLBufferDescriptor&& move) noexcept = delete;
};
