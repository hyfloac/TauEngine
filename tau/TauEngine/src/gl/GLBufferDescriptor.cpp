#include "gl/GLBufferDescriptor.hpp"
#include "system/RenderingContext.hpp"

GLuint GLBufferDescriptor::generate() noexcept
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void GLBufferDescriptor::_bind(const GLuint vao) noexcept
{
    glBindVertexArray(vao);
}

void GLBufferDescriptor::destroy(const GLuint vao) noexcept
{
    glDeleteVertexArrays(1, &vao);
}

void GLBufferDescriptor::attribPointer(const GLuint index, const u32 size, const DataType type, const bool normalized, const i32 stride, const void* const pointer) noexcept
{
    glVertexAttribPointer(index, size, getGLType(type), normalized, stride, pointer);
}

void GLBufferDescriptor::addAttribute(const Ref<IBuffer> buffer, const u32 size, const DataType type, const bool normalized, const i32 stride, const void* const pointer) noexcept
{
    _attribs[_currAttrib++] = { buffer, size, type, normalized, stride, pointer };
}

void GLBufferDescriptor::bind(IRenderingContext& context) noexcept
{
    _bind(*context.getBufferDescriptorHandle<GLuint>(this));
}

void GLBufferDescriptor::unbind(IRenderingContext& context) noexcept
{
    glBindVertexArray(0);
}

void GLBufferDescriptor::enableAttributes(IRenderingContext& context) noexcept
{
    for(GLuint attrib = 0; attrib < _attribs.count(); ++attrib)
    {
        glEnableVertexAttribArray(attrib);
    }
}

void GLBufferDescriptor::disableAttributes(IRenderingContext& context) noexcept
{
    for(GLuint attrib = _attribs.count(); attrib > 0; --attrib)
    {
        glDisableVertexAttribArray(attrib - 1);
    }
}

GLenum GLBufferDescriptor::getGLType(const DataType type) noexcept
{
    switch(type)
    {
        case DataType::Byte:                return GL_BYTE;
        case DataType::UByte:               return GL_UNSIGNED_BYTE;
        case DataType::Short:               return GL_SHORT;
        case DataType::UShort:              return GL_UNSIGNED_SHORT;
        case DataType::Int:                 return GL_INT;
        case DataType::UInt:                return GL_UNSIGNED_INT;
        case DataType::HalfFloat:           return GL_HALF_FLOAT;
        case DataType::Float:               return GL_FLOAT;
        case DataType::Double:              return GL_DOUBLE;
        case DataType::Fixed:               return GL_FIXED;
        case DataType::Int2_10_10_10_Rev:   return GL_INT_2_10_10_10_REV;
        case DataType::UInt2_10_10_10_Rev:  return GL_UNSIGNED_INT_2_10_10_10_REV;
        case DataType::UInt10F_11F_11F_Rev: return GL_UNSIGNED_INT_10F_11F_11F_REV;
        default:                            return static_cast<GLenum>(-1);
    }
}

DataType GLBufferDescriptor::getType(const GLenum type) noexcept
{
    switch(type)
    {
        case GL_BYTE:                         return DataType::Byte;
        case GL_UNSIGNED_BYTE:                return DataType::UByte;
        case GL_SHORT:                        return DataType::Short;
        case GL_UNSIGNED_SHORT:               return DataType::UShort;
        case GL_INT:                          return DataType::Int;
        case GL_UNSIGNED_INT:                 return DataType::UInt;
        case GL_HALF_FLOAT:                   return DataType::HalfFloat;
        case GL_FLOAT:                        return DataType::Float;
        case GL_DOUBLE:                       return DataType::Double;
        case GL_FIXED:                        return DataType::Fixed;
        case GL_INT_2_10_10_10_REV:           return DataType::Int2_10_10_10_Rev;
        case GL_UNSIGNED_INT_2_10_10_10_REV:  return DataType::UInt2_10_10_10_Rev;
        case GL_UNSIGNED_INT_10F_11F_11F_REV: return DataType::UInt10F_11F_11F_Rev;
        default:                              return static_cast<DataType>(-1);
    }
}

