#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>
#include <DynArray.hpp>
#include <Safeties.hpp>

class IRenderingContext;
class IBuffer;

// enum class DataType : u8
// {
//     Byte = 1,
//     UByte,
//     Short,
//     UShort,
//     Int,
//     UInt,
//     HalfFloat,
//     Float,
//     Double,
//     Fixed,
//     Int2_10_10_10_Rev,
//     UInt2_10_10_10_Rev,
//     UInt10F_11F_11F_Rev
// };

// class TAU_DLL IBufferDescriptor
// {
//     DEFAULT_DESTRUCT_VI(IBufferDescriptor);
//     DELETE_COPY(IBufferDescriptor);
// public:
//     enum class DataType : u8
//     {
//         Unknown = 0,
//         Bool,
//         Int,
//         UInt,
//         Float,
//         Double,
//         Vector2Bool,
//         Vector3Bool,
//         Vector4Bool,
//         Vector2Int,
//         Vector3Int,
//         Vector4Int,
//         Vector2UInt,
//         Vector3UInt,
//         Vector4UInt,
//         Vector2Float,
//         Vector3Float,
//         Vector4Float,
//         Vector2Double,
//         Vector3Double,
//         Vector4Double,
//         Matrix2x2Float,
//         Matrix2x3Float,
//         Matrix2x4Float,
//         Matrix3x2Float,
//         Matrix3x3Float,
//         Matrix3x4Float,
//         Matrix4x2Float,
//         Matrix4x3Float,
//         Matrix4x4Float,
//         Matrix2x2Double,
//         Matrix2x3Double,
//         Matrix2x4Double,
//         Matrix3x2Double,
//         Matrix3x3Double,
//         Matrix3x4Double,
//         Matrix4x2Double,
//         Matrix4x3Double,
//         Matrix4x4Double
//     };
//
//     struct AttributeDescriptor final
//     {
//         Ref<IBuffer> buffer;
//         u32 size;
//         IBufferDescriptor::DataType type;
//         bool normalized;
//         i32 stride;
//         const void* pointer;
//     };
// protected:
//     u64 _uid;
//     DynArray<AttributeDescriptor> _attribs;
// protected:
//     IBufferDescriptor(const u64 uid, const std::size_t attribCount) noexcept
//         : _uid(uid), _attribs(attribCount)
//     { }
// public:
//     virtual void addAttribute(Ref<IBuffer> buffer, u32 size, DataType type, bool normalized, i32 stride, const void* pointer) noexcept = 0;
//
//     virtual void bind(IRenderingContext& context) noexcept = 0;
//
//     virtual void unbind(IRenderingContext& context) noexcept = 0;
//
//     virtual void enableAttributes(IRenderingContext& context) noexcept = 0;
//
//     virtual void disableAttributes(IRenderingContext& context) noexcept = 0;
//
//     [[nodiscard]] u64 uid() const noexcept { return _uid; }
//     [[nodiscard]] DynArray<AttributeDescriptor>& attribs() noexcept { return _attribs; }
// };

class ShaderDataType final
{
    DEFAULT_CONSTRUCT_PI(ShaderDataType);
    DEFAULT_DESTRUCT(ShaderDataType);
    DELETE_COPY(ShaderDataType);
public:
    enum Type : u8
    {
        Unknown = 0,
        Bool,
        Int,
        UInt,
        Float,
        Double,
        Vector2Bool,
        Vector3Bool,
        Vector4Bool,
        Vector2Int,
        Vector3Int,
        Vector4Int,
        Vector2UInt,
        Vector3UInt,
        Vector4UInt,
        Vector2Float,
        Vector3Float,
        Vector4Float,
        Vector2Double,
        Vector3Double,
        Vector4Double,
        Matrix2x2Float,
        Matrix2x3Float,
        Matrix2x4Float,
        Matrix3x2Float,
        Matrix3x3Float,
        Matrix3x4Float,
        Matrix4x2Float,
        Matrix4x3Float,
        Matrix4x4Float,
        Matrix2x2Double,
        Matrix2x3Double,
        Matrix2x4Double,
        Matrix3x2Double,
        Matrix3x3Double,
        Matrix3x4Double,
        Matrix4x2Double,
        Matrix4x3Double,
        Matrix4x4Double
    };
public:
    static u32 size(Type type) noexcept;

    static u32 componentCount(Type type) noexcept;
};

class TAU_DLL BufferElementDescriptor final
{
    DEFAULT_CONSTRUCT_PU(BufferElementDescriptor);
    DEFAULT_COPY(BufferElementDescriptor);
    DEFAULT_DESTRUCT(BufferElementDescriptor);
private:
    ShaderDataType::Type _type;
    u32 _sizeCache;
    u32 _offsetCache;
    bool _normalized;
private:
    BufferElementDescriptor(const ShaderDataType::Type type, const u32 offsetCache, const bool normalized = false)
        : _type(type), _sizeCache(ShaderDataType::size(type)),
        _offsetCache(offsetCache), _normalized(normalized)
    { }
public:
    BufferElementDescriptor(const ShaderDataType::Type type, const bool normalized = false)
        : _type(type), _sizeCache(ShaderDataType::size(type)),
          _offsetCache(0), _normalized(normalized)
    { }

    [[nodiscard]] ShaderDataType::Type type() const noexcept { return _type; }
    [[nodiscard]] bool normalized() const noexcept { return _normalized; }
    [[nodiscard]] u32 size() const noexcept { return _sizeCache; }
    [[nodiscard]] u32 offset() const noexcept { return _offsetCache; }
private:
    friend class BufferDescriptor;
};

class TAU_DLL BufferDescriptor final
{
private:
    u32 _currentIndex;
    DynArray<BufferElementDescriptor> _elementDescriptors;
    u32 _stride;
    u32 _offsetCache;
public:
    BufferDescriptor(const std::size_t descriptorCount) noexcept
        : _currentIndex(0), _elementDescriptors(descriptorCount), _stride(0), _offsetCache(0)
    { }

    [[nodiscard]] const DynArray<BufferElementDescriptor>& elements() const noexcept { return _elementDescriptors; }
    [[nodiscard]] u32 stride() const noexcept { return _stride; }

    void addDescriptor(BufferElementDescriptor bed) noexcept;
    void addDescriptor(ShaderDataType::Type type, bool normalized = false) noexcept;
};

