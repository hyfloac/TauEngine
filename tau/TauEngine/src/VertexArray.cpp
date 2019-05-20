#include <model/VertexArray.hpp>

GLenum getGLType(const DataType type) noexcept
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

DataType getType(const GLenum type) noexcept
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

VertexArrayShared::VertexArrayShared() noexcept
    : _array(), _refCount(new u32)
{
    *_refCount = 1;
    glGenVertexArrays(1, &_array);
}

VertexArrayShared::VertexArrayShared(const VertexArrayShared& copy) noexcept
    : _array(copy._array), _refCount(copy._refCount)
{
    ++(*_refCount);
}

VertexArrayShared::VertexArrayShared(VertexArrayShared&& move) noexcept
    : _array(move._array), _refCount(move._refCount)
{
    ++(*_refCount);
}

VertexArrayShared::~VertexArrayShared() noexcept
{
    if(--(*_refCount) <= 0)
    {
        glDeleteVertexArrays(1, &_array);
        delete _refCount;
    }
}

VertexArrayShared& VertexArrayShared::operator =(const VertexArrayShared& copy) noexcept
{
    if(this != &copy)
    {
        _array = copy._array;
        _refCount = copy._refCount;
        ++(*_refCount);
    }

    return *this;
}

VertexArrayShared& VertexArrayShared::operator =(VertexArrayShared&& move) noexcept
{
    if(this != &move)
    {
        _array = move._array;
        _refCount = move._refCount;
        ++(*_refCount);
    }

    return *this;
}
