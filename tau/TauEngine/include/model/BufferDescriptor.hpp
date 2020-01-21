#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>
#include <DynArray.hpp>

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

    class Typed
    {
        DEFAULT_CONSTRUCT_PO(Typed);
        DEFAULT_DESTRUCT_VI(Typed);
        DELETE_COPY(Typed);
    public:
        [[nodiscard]] virtual ShaderDataType::Type dataType() noexcept = 0;
    };
public:
    static u32 size(Type type) noexcept;

    static u32 componentCount(Type type) noexcept;

    /**
     *   The underlying type of what needs to be passed to the
     * shader. This is primarily used for matrices, which need
     * to be interpreted as vectors by the CPU.
     */
    static Type underlyingType(Type type) noexcept;

    /**
     *   The underlying type when doubles aren't allowed to be
     * passed to the shader.
     */
    static Type underlyingTypeND(Type type) noexcept;
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
    friend class IInputLayout;
};

class TAU_DLL BufferDescriptor final
{
private:
    u32 _currentIndex;
    RefDynArray<BufferElementDescriptor> _elementDescriptors;
    u32 _stride;
public:
    BufferDescriptor(const std::size_t descriptorCount) noexcept
        : _currentIndex(0), _elementDescriptors(descriptorCount), _stride(0)
    { }

    [[nodiscard]] const RefDynArray<BufferElementDescriptor>& elements() const noexcept { return _elementDescriptors; }
    [[nodiscard]] u32 stride() const noexcept { return _stride; }

    void addDescriptor(BufferElementDescriptor bed) noexcept;
    void addDescriptor(ShaderDataType::Type type, bool normalized = false) noexcept;
};

