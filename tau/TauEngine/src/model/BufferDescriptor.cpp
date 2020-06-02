#include "model/BufferDescriptor.hpp"
#include "Timings.hpp"

uSys ShaderDataType::size(const Type type) noexcept
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

uSys ShaderDataType::componentCount(const Type type) noexcept
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

ShaderDataType::Type ShaderDataType::underlyingType(const Type type) noexcept
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

ShaderDataType::Type ShaderDataType::underlyingTypeND(const Type type) noexcept
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

bool ShaderDataType::isSingle(const Type type) noexcept
{
    switch(type)
    {
        case Bool:
        case Int:  
        case UInt: 
        case Float:
        case Double:  return true;
        default: return false;
    }
}

bool ShaderDataType::isVector(const Type type) noexcept
{
    switch(type)
    {
        case Vector2Bool:
        case Vector3Bool:
        case Vector4Bool:
        case Vector2Int: 
        case Vector3Int: 
        case Vector4Int: 
        case Vector2UInt:
        case Vector3UInt:
        case Vector4UInt:
        case Vector2Float: 
        case Vector3Float: 
        case Vector4Float: 
        case Vector2Double:
        case Vector3Double:
        case Vector4Double: return true;
        default: return false;
    }
}

bool ShaderDataType::isMatrix(const Type type) noexcept
{
    switch(type)
    {
        case Matrix2x2Float:  
        case Matrix2x3Float:  
        case Matrix2x4Float:  
        case Matrix3x2Float:  
        case Matrix3x3Float:  
        case Matrix3x4Float:  
        case Matrix4x2Float:  
        case Matrix4x3Float:  
        case Matrix4x4Float:  
        case Matrix2x2Double: 
        case Matrix2x3Double: 
        case Matrix2x4Double: 
        case Matrix3x2Double: 
        case Matrix3x3Double: 
        case Matrix3x4Double: 
        case Matrix4x2Double: 
        case Matrix4x3Double: 
        case Matrix4x4Double: return true;
        default: return false;
    }
}

uSys ShaderDataType::columnCount(const Type type) noexcept
{
    switch(type)
    {
        case Matrix2x2Float:  
        case Matrix2x3Float:  
        case Matrix2x4Float: return 2;
        case Matrix3x2Float: 
        case Matrix3x3Float: 
        case Matrix3x4Float: return 3;
        case Matrix4x2Float: 
        case Matrix4x3Float: 
        case Matrix4x4Float: return 4;
        case Matrix2x2Double: 
        case Matrix2x3Double: 
        case Matrix2x4Double: return 2;
        case Matrix3x2Double: 
        case Matrix3x3Double: 
        case Matrix3x4Double: return 3;
        case Matrix4x2Double: 
        case Matrix4x3Double: 
        case Matrix4x4Double: return 4;
        default: return 1;
    }
}

uSys ShaderDataType::rowCount(const Type type) noexcept
{
    switch(type)
    {
        case Matrix2x2Float:
        case Matrix3x2Float:
        case Matrix4x2Float: return 2;
        case Matrix2x3Float:
        case Matrix3x3Float:
        case Matrix4x3Float: return 3;
        case Matrix2x4Float: 
        case Matrix3x4Float: 
        case Matrix4x4Float: return 4;
        case Matrix2x2Double:
        case Matrix3x2Double: 
        case Matrix4x2Double: return 2;
        case Matrix2x3Double:
        case Matrix3x3Double:
        case Matrix4x3Double: return 3;
        case Matrix2x4Double: 
        case Matrix3x4Double: 
        case Matrix4x4Double: return 4;
        default: return 1;
    }
}

ShaderDataType::Type ShaderSemantic::associatedType(const Semantic semantic) noexcept
{
    switch(semantic)
    {
        case BiNormal: return ShaderDataType::Vector4Float;
        case BlendIndices: return ShaderDataType::UInt;
        case BlendWeight: return ShaderDataType::Float;
        case Color: return ShaderDataType::Vector4Float;
        case Normal: return ShaderDataType::Vector4Float;
        case Position: return ShaderDataType::Vector4Float;
        case PositionTransformed: return ShaderDataType::Vector4Float;
        case PointSize: return ShaderDataType::Float;
        case Tangent: return ShaderDataType::Vector4Float;
        case TextureCoord: return ShaderDataType::Vector4Float;

        case Unknown:
        default: return ShaderDataType::Unknown;
    }
}

bool ShaderSemantic::hasIndices(const Semantic semantic) noexcept
{
    switch(semantic)
    {
        case Unknown:
        case BiNormal:
        case BlendIndices:
        case BlendWeight:
        case Color:
        case Normal:
        case Position:
        case Tangent:
        case TextureCoord: return true;

        case PositionTransformed:
        case PointSize:
        default: return false;
    }
}

void BufferDescriptorBuilder::addDescriptor(BufferElementDescriptor bed) noexcept
{
    bed._offsetCache = _stride;
    _elementDescriptors[_currentIndex++] = bed;

    _stride += bed._sizeCache;
}

void BufferDescriptorBuilder::addDescriptor(const ShaderSemantic::Semantic semantic, const ShaderDataType::Type type, const bool normalized) noexcept
{
    _elementDescriptors[_currentIndex++] = BufferElementDescriptor(semantic, type, _stride, normalized);

    const u32 size = ShaderDataType::size(type);

    _stride += size;
}

void BufferDescriptorBuilder::reset(const uSys descriptorCount, const bool instanced) noexcept
{
    _currentIndex = 0;
    _elementDescriptors = RefDynArray<BufferElementDescriptor>(descriptorCount);
    _stride = 0;
    _instanced = instanced;
}

BufferDescriptor BufferDescriptorBuilder::build() const noexcept
{
    return BufferDescriptor(_elementDescriptors, _stride, _instanced);
}
