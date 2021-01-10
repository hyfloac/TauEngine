#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <RunTimeType.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"
#include "GraphicsEnums.hpp"

struct DescriptorLayoutEntry final
{
    DEFAULT_CONSTRUCT_PU(DescriptorLayoutEntry);
    DEFAULT_DESTRUCT(DescriptorLayoutEntry);
    DEFAULT_CM_PU(DescriptorLayoutEntry);
public:
    enum class Type
    {
        UniformBufferView = 1,
        UnorderedAccessView,
        TextureView,
        TextureSampler,
        Constant
    };
public:
    /**
     * The type of descriptors stored in the table.
     */
    Type type;
    /**
     * The beginning index for binding.
     */
    uSys begin;
    /**
     * The number of descriptors within the table to bind.
     */
    uSys count;
    /**
     * Which shader is allowed to access the table.
     */
    EGraphics::ShaderAccess shaderAccess;

    DescriptorLayoutEntry(const Type _type, const uSys _begin, const uSys _count, const EGraphics::ShaderAccess _shaderAccess) noexcept
        : type(_type)
        , begin(_begin)
        , count(_count)
        , shaderAccess(_shaderAccess)
    { }
};

#define DESCRIPTOR_LAYOUT_IMPL(_TYPE) RTT_IMPL(_TYPE, IDescriptorLayout)

/**
 * @brief An opaque type for describing the descriptor layout.
 *
 *   Descriptor layouts are used with resource binding for
 * shaders. Some API's (DirectX12) have this as a native
 * concept. Older API's use a different binding system and thus
 * emulate descriptor tables; these API's can use
 * @link SimpleDescriptorLayout @endlink for simplicity.
 */
class TAU_DLL TAU_NOVTABLE IDescriptorLayout
{
    DEFAULT_CONSTRUCT_PO(IDescriptorLayout);
    DEFAULT_DESTRUCT_VI(IDescriptorLayout);
    DEFAULT_CM_PO(IDescriptorLayout);
public:
    RTT_BASE_IMPL(IDescriptorLayout);
    RTT_BASE_CHECK(IDescriptorLayout);
    RTT_BASE_CAST(IDescriptorLayout);
};

/**
 *   @brief An implementation of IDescriptorLayout for legacy
 * API's.
 *
 *   This simply stores all of the entries in an array. While
 * some API's (DirectX12) have a native concept for this, some
 * older API's simply emulate this. For ease of use this simple
 * implementation is provided.
 */
class TAU_DLL SimpleDescriptorLayout final : public IDescriptorLayout
{
    DESCRIPTOR_LAYOUT_IMPL(SimpleDescriptorLayout);
    DEFAULT_DESTRUCT(SimpleDescriptorLayout);
    DEFAULT_CM_PU(SimpleDescriptorLayout);
private:
    RefDynArray<DescriptorLayoutEntry> _entries;
public:
    SimpleDescriptorLayout(const RefDynArray<DescriptorLayoutEntry>& entries) noexcept
        : _entries(entries)
    { }

    SimpleDescriptorLayout(RefDynArray<DescriptorLayoutEntry>&& entries) noexcept
        : _entries(::std::move(entries))
    { }

    [[nodiscard]] const RefDynArray<DescriptorLayoutEntry>& entries() const noexcept { return _entries; }
};

struct DescriptorLayoutArgs final
{
    uSys entryCount;
    DescriptorLayoutEntry* entries;
};

class TAU_DLL TAU_NOVTABLE IDescriptorLayoutBuilder
{
    DEFAULT_CONSTRUCT_PO(IDescriptorLayoutBuilder);
    DEFAULT_DESTRUCT_VI(IDescriptorLayoutBuilder);
    DEFAULT_CM_PO(IDescriptorLayoutBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    /**
     * Gets the size required for an allocation.
     *
     * This is useful for fixed block allocators.
     */
    [[nodiscard]] uSys allocSize() const noexcept
    { return _allocSize(); }

    [[nodiscard]] virtual NullableRef<IDescriptorLayout> build(const DescriptorLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
protected:
    [[nodiscard]] virtual uSys _allocSize() const noexcept = 0;
};
