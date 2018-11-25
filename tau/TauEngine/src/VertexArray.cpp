#include <model/VertexArray.hpp>

GLenum getGLType(DataType type) noexcept
{
    switch(type)
    {
        case Byte:                return GL_BYTE;
        case UByte:               return GL_UNSIGNED_BYTE;
        case Short:               return GL_SHORT;
        case UShort:              return GL_UNSIGNED_SHORT;
        case Int:                 return GL_INT;
        case UInt:                return GL_UNSIGNED_INT;
        case HalfFloat:           return GL_HALF_FLOAT;
        case Float:               return GL_FLOAT;
        case Double:              return GL_DOUBLE;
        case Fixed:               return GL_FIXED;
        case Int2_10_10_10_Rev:   return GL_INT_2_10_10_10_REV;
        case UInt2_10_10_10_Rev:  return GL_UNSIGNED_INT_2_10_10_10_REV;
        case UInt10F_11F_11F_Rev: return GL_UNSIGNED_INT_10F_11F_11F_REV;
        default:                  return -1;
    }
}

DataType getType(GLenum type) noexcept
{
    switch(type)
    {
    case GL_BYTE:                         return  Byte;
    case GL_UNSIGNED_BYTE:                return  UByte;
    case GL_SHORT:                        return  Short;
    case GL_UNSIGNED_SHORT:               return  UShort;
    case GL_INT:                          return  Int;
    case GL_UNSIGNED_INT:                 return  UInt;
    case GL_HALF_FLOAT:                   return  HalfFloat;
    case GL_FLOAT:                        return  Float;
    case GL_DOUBLE:                       return  Double;
    case GL_FIXED:                        return  Fixed;
    case GL_INT_2_10_10_10_REV:           return  Int2_10_10_10_Rev;
    case GL_UNSIGNED_INT_2_10_10_10_REV:  return  UInt2_10_10_10_Rev;
    case GL_UNSIGNED_INT_10F_11F_11F_REV: return  UInt10F_11F_11F_Rev;
    default:                              return static_cast<DataType>(-1);
    }
}

VertexArray::VertexArray() noexcept
    : _array()
{
    glGenVertexArrays(1, &_array);
}

VertexArray::~VertexArray() noexcept
{
    glDeleteVertexArrays(1, &_array);
}

void VertexArray::bind() const noexcept
{
    glBindVertexArray(this->_array);
}

void VertexArray::unbind() noexcept
{
    glBindVertexArray(0);
}

void VertexArray::setAttribute(GLuint index, GLuint size, DataType type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) noexcept
{
    glVertexAttribPointer(index, size, getGLType(type), normalized, stride, pointer);
}

void VertexArray::enableAttribute(GLuint index) noexcept
{
    glEnableVertexAttribArray(index);
}


