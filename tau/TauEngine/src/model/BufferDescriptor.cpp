#include "model/BufferDescriptor.hpp"
#include "Timings.hpp"

u32 ShaderDataType::size(Type type) noexcept
{
    switch(type)
    {
        case Unknown: return 0;
        case Bool: return 1;
        case Int: return 4;
        case UInt: return 4;
        case Float: return 4;
        case Double: return 8;
        case Vector2Bool: return 2 * 1;
        case Vector3Bool: return 3 * 1;
        case Vector4Bool: return 4 * 1;
        case Vector2Int: return 2 * 4;
        case Vector3Int: return 3 * 4;
        case Vector4Int: return 4 * 4;
        case Vector2UInt: return 2 * 4;
        case Vector3UInt: return 3 * 4;
        case Vector4UInt: return 4 * 4;
        case Vector2Float: return 2 * 4;
        case Vector3Float: return 3 * 4;
        case Vector4Float: return 4 * 4;
        case Vector2Double: return 2 * 8;
        case Vector3Double: return 3 * 8;
        case Vector4Double: return 4 * 8;
        case Matrix2x2Float: return 2 * 2 * 4;
        case Matrix2x3Float: return 2 * 3 * 4;
        case Matrix2x4Float: return 2 * 4 * 4;
        case Matrix3x2Float: return 3 * 2 * 4;
        case Matrix3x3Float: return 3 * 3 * 4;
        case Matrix3x4Float: return 3 * 4 * 4;
        case Matrix4x2Float: return 4 * 2 * 4;
        case Matrix4x3Float: return 4 * 3 * 4;
        case Matrix4x4Float: return 4 * 4 * 4;
        case Matrix2x2Double: return 2 * 2 * 8;
        case Matrix2x3Double: return 2 * 3 * 8;
        case Matrix2x4Double: return 2 * 4 * 8;
        case Matrix3x2Double: return 3 * 2 * 8;
        case Matrix3x3Double: return 3 * 3 * 8;
        case Matrix3x4Double: return 3 * 4 * 8;
        case Matrix4x2Double: return 4 * 2 * 8;
        case Matrix4x3Double: return 4 * 3 * 8;
        case Matrix4x4Double: return 4 * 4 * 8;
        default: return 0;
    }
}

u32 ShaderDataType::componentCount(Type type) noexcept
{
    switch(type)
    {
        case Unknown: return 0;
        case Bool: return 1;
        case Int: return 1;
        case UInt: return 1;
        case Float: return 1;
        case Double: return 1;
        case Vector2Bool: return 2;
        case Vector3Bool: return 3;
        case Vector4Bool: return 4;
        case Vector2Int: return 2;
        case Vector3Int: return 3;
        case Vector4Int: return 4;
        case Vector2UInt: return 2;
        case Vector3UInt: return 3;
        case Vector4UInt: return 4;
        case Vector2Float: return 2;
        case Vector3Float: return 3;
        case Vector4Float: return 4;
        case Vector2Double: return 2;
        case Vector3Double: return 3;
        case Vector4Double: return 4;
        case Matrix2x2Float: return 2 * 2;
        case Matrix2x3Float: return 2 * 3;
        case Matrix2x4Float: return 2 * 4;
        case Matrix3x2Float: return 3 * 2;
        case Matrix3x3Float: return 3 * 3;
        case Matrix3x4Float: return 3 * 4;
        case Matrix4x2Float: return 4 * 2;
        case Matrix4x3Float: return 4 * 3;
        case Matrix4x4Float: return 4 * 4;
        case Matrix2x2Double: return 2 * 2;
        case Matrix2x3Double: return 2 * 3;
        case Matrix2x4Double: return 2 * 4;
        case Matrix3x2Double: return 3 * 2;
        case Matrix3x3Double: return 3 * 3;
        case Matrix3x4Double: return 3 * 4;
        case Matrix4x2Double: return 4 * 2;
        case Matrix4x3Double: return 4 * 3;
        case Matrix4x4Double: return 4 * 4;
        default: return 0;
    }
}

ShaderDataType::Type ShaderDataType::underlyingType(Type type) noexcept
{
    switch(type)
    {
        case Matrix2x2Float:  return Type::Vector2Float;
        case Matrix2x3Float:  return Type::Vector3Float;
        case Matrix2x4Float:  return Type::Vector4Float;
        case Matrix3x2Float:  return Type::Vector2Float;
        case Matrix3x3Float:  return Type::Vector3Float;
        case Matrix3x4Float:  return Type::Vector4Float;
        case Matrix4x2Float:  return Type::Vector2Float;
        case Matrix4x3Float:  return Type::Vector3Float;
        case Matrix4x4Float:  return Type::Vector4Float;
        case Matrix2x2Double: return Type::Vector2Double;
        case Matrix2x3Double: return Type::Vector3Double;
        case Matrix2x4Double: return Type::Vector4Double;
        case Matrix3x2Double: return Type::Vector2Double;
        case Matrix3x3Double: return Type::Vector3Double;
        case Matrix3x4Double: return Type::Vector4Double;
        case Matrix4x2Double: return Type::Vector2Double;
        case Matrix4x3Double: return Type::Vector3Double;
        case Matrix4x4Double: return Type::Vector4Double;
        default: return type;
    }
}

ShaderDataType::Type ShaderDataType::underlyingTypeND(Type type) noexcept
{
    switch(type)
    {
        case Double:          return Type::Vector2UInt;
        case Vector2Double:   return Type::Vector4UInt;
        case Vector3Double:   return Type::Vector2UInt;
        case Vector4Double:   return Type::Vector4UInt;
        case Matrix2x2Float:  return Type::Vector2Float;
        case Matrix2x3Float:  return Type::Vector3Float;
        case Matrix2x4Float:  return Type::Vector4Float;
        case Matrix3x2Float:  return Type::Vector2Float;
        case Matrix3x3Float:  return Type::Vector3Float;
        case Matrix3x4Float:  return Type::Vector4Float;
        case Matrix4x2Float:  return Type::Vector2Float;
        case Matrix4x3Float:  return Type::Vector3Float;
        case Matrix4x4Float:  return Type::Vector4Float;
        case Matrix2x2Double: return Type::Vector4UInt;
        case Matrix2x3Double: return Type::Vector2UInt;
        case Matrix2x4Double: return Type::Vector4UInt;
        case Matrix3x2Double: return Type::Vector4UInt;
        case Matrix3x3Double: return Type::Vector2UInt;
        case Matrix3x4Double: return Type::Vector4UInt;
        case Matrix4x2Double: return Type::Vector4UInt;
        case Matrix4x3Double: return Type::Vector2UInt;
        case Matrix4x4Double: return Type::Vector4UInt;
        default: return type;
    }
}

void BufferDescriptor::addDescriptor(BufferElementDescriptor bed) noexcept
{
    bed._offsetCache = _stride;
    _elementDescriptors[_currentIndex++] = bed;

    _stride += bed._sizeCache;
}

void BufferDescriptor::addDescriptor(const ShaderDataType::Type type, const bool normalized) noexcept
{
    _elementDescriptors[_currentIndex++] = BufferElementDescriptor(type, _stride, normalized);

    const u32 size = ShaderDataType::size(type);

    _stride += size;
}
