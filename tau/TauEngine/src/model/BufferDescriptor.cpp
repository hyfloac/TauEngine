#include "model/BufferDescriptor.hpp"

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

void BufferDescriptor::addDescriptor(BufferElementDescriptor bed) noexcept
{
    bed._offsetCache = _offsetCache;
    _elementDescriptors[_currentIndex++] = bed;

    _offsetCache += bed._sizeCache;
    _stride += bed._sizeCache;
}

void BufferDescriptor::addDescriptor(const ShaderDataType::Type type, const bool normalized) noexcept
{
    _elementDescriptors[_currentIndex++] = BufferElementDescriptor(type, _offsetCache, normalized);

    const u32 size = ShaderDataType::size(type);

    _offsetCache += size;
    _stride += size;
}
