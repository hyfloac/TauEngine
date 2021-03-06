#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"

class IRenderingContext;
class IVertexBuffer;

class ShaderDataType final
{
    DELETE_CONSTRUCT(ShaderDataType);
    DELETE_DESTRUCT(ShaderDataType);
    DELETE_CM(ShaderDataType);
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
        DEFAULT_CM_PO(Typed);
    public:
        [[nodiscard]] virtual ShaderDataType::Type dataType() noexcept = 0;
    };
public:
    static uSys size(Type type) noexcept;

    static uSys componentCount(Type type) noexcept;

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

    /**
     * Is this a single element. i.e. not a vector or matrix.
     */
    static bool isSingle(Type type) noexcept;

    /**
     * Is this a vector. i.e. not a single or a matrix.
     */
    static bool isVector(Type type) noexcept;

    /**
     * Is this a matrix. i.e. not a single or a vector.
     */
    static bool isMatrix(Type type) noexcept;

    /**
     * How many columns does the matrix have.
     *
     * This uses a column major format.
     *
     * If type is not a matrix returns 1.
     */
    static uSys columnCount(Type type) noexcept;

    /**
     * How many rows does the matrix have.
     *
     * This uses a column major format.
     *
     * If type is not a matrix returns 1.
     */
    static uSys rowCount(Type type) noexcept;

    static bool isBool(Type type) noexcept;
    static bool isInt(Type type) noexcept;
    static bool isUInt(Type type) noexcept;
    static bool isFloat(Type type) noexcept;
    static bool isDouble(Type type) noexcept;
    static bool isValidInput(Type type) noexcept;
};

class ShaderSemantic final
{
    DELETE_CONSTRUCT(ShaderSemantic);
    DELETE_DESTRUCT(ShaderSemantic);
    DELETE_CM(ShaderSemantic);
public:
    enum Semantic
    {
        Unknown = 0,
        BiNormal,
        BlendIndices,
        BlendWeight,
        Color,
        Colour = Color,
        Normal,
        Position,
        PositionTransformed,
        PointSize,
        Tangent,
        TextureCoord,
        MAX_VALUE = TextureCoord
    };
public:
    static ShaderDataType::Type associatedType(Semantic semantic) noexcept;

    static bool hasIndices(Semantic semantic) noexcept;
};

class TAU_DLL BufferElementDescriptor final
{
    DEFAULT_CONSTRUCT_PU(BufferElementDescriptor);
    DEFAULT_DESTRUCT(BufferElementDescriptor);
    DEFAULT_CM_PU(BufferElementDescriptor);
private:
    ShaderSemantic::Semantic _semantic;
    ShaderDataType::Type _type;
    u32 _sizeCache;
    u32 _offsetCache;
    bool _normalized;
private:
    BufferElementDescriptor(const ShaderSemantic::Semantic semantic, const ShaderDataType::Type type, const u32 offsetCache, const bool normalized = false) noexcept
        : _semantic(semantic)
        , _type(type)
        , _sizeCache(static_cast<u32>(ShaderDataType::size(type)))
        , _offsetCache(offsetCache)
        , _normalized(normalized)
    { }
public:
    BufferElementDescriptor(const ShaderSemantic::Semantic semantic, const ShaderDataType::Type type, const bool normalized = false) noexcept
        : _semantic(semantic)
        , _type(type)
        , _sizeCache(static_cast<u32>(ShaderDataType::size(type)))
        , _offsetCache(0)
        , _normalized(normalized)
    { }

    [[nodiscard]] ShaderSemantic::Semantic semantic() const noexcept { return _semantic; }
    [[nodiscard]] ShaderDataType::Type type() const noexcept { return _type; }
    [[nodiscard]] u32 size() const noexcept { return _sizeCache; }
    [[nodiscard]] u32 offset() const noexcept { return _offsetCache; }
    [[nodiscard]] bool normalized() const noexcept { return _normalized; }
private:
    friend class BufferDescriptorBuilder;
};

class TAU_DLL BufferDescriptor final
{
    DEFAULT_CONSTRUCT_PU(BufferDescriptor);
    DEFAULT_DESTRUCT(BufferDescriptor);
    DEFAULT_CM_PU(BufferDescriptor);
private:
    RefDynArray<BufferElementDescriptor> _elementDescriptors;
    u32 _stride;
    bool _instanced;
private:
    BufferDescriptor(const RefDynArray<BufferElementDescriptor>& elementDescriptors, const u32 stride, const bool instanced) noexcept
        : _elementDescriptors(elementDescriptors)
        , _stride(stride)
        , _instanced(instanced)
    { }
public:
    [[nodiscard]] const RefDynArray<BufferElementDescriptor>& elements() const noexcept { return _elementDescriptors; }
    [[nodiscard]] u32 stride() const noexcept { return _stride; }
    [[nodiscard]] bool instanced() const noexcept { return _instanced; }
private:
    friend class BufferDescriptorBuilder;
};

class TAU_DLL BufferDescriptorBuilder final
{
    DEFAULT_DESTRUCT(BufferDescriptorBuilder);
    DEFAULT_CM_PU(BufferDescriptorBuilder);
private:
    u32 _currentIndex;
    RefDynArray<BufferElementDescriptor> _elementDescriptors;
    u32 _stride;
    bool _instanced;
public:
    BufferDescriptorBuilder(const uSys descriptorCount, const bool instanced) noexcept
        : _currentIndex(0)
        , _elementDescriptors(descriptorCount)
        , _stride(0)
        , _instanced(instanced)
    { }

    [[nodiscard]] const RefDynArray<BufferElementDescriptor>& elements() const noexcept { return _elementDescriptors; }
    [[nodiscard]] u32 stride() const noexcept { return _stride; }
    [[nodiscard]] bool instanced() const noexcept { return _instanced; }

    void addDescriptor(BufferElementDescriptor bed) noexcept;
    void addDescriptor(ShaderSemantic::Semantic semantic, ShaderDataType::Type type, bool normalized = false) noexcept;

    void reset(uSys descriptorCount, bool instanced) noexcept;

    [[nodiscard]] BufferDescriptor build() const noexcept;
};

