/**
 * @file
 */
#pragma once

#include "graphics/DescriptorHeap.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "allocator/FixedBlockAllocator.hpp"

class TAU_DLL DX10DescriptorTable final
{
    DELETE_CM(DX10DescriptorTable);
private:
    TauAllocator* _allocator;
    DescriptorType _type;
    uSys _count;
    union
    {
        u8* _placement;
        ID3D10Buffer** _bufferViews;
        ID3D10ShaderResourceView** _srvViews;
    };
public:
    DX10DescriptorTable(TauAllocator* const allocator, const DescriptorType type, const uSys count, u8* const placement) noexcept
        : _allocator(allocator)
        , _type(type)
        , _count(count)
        , _placement(placement)
    { }

    ~DX10DescriptorTable() noexcept;

    [[nodiscard]] DescriptorType type() const noexcept { return _type; }
    [[nodiscard]] uSys count() const noexcept { return _count; }

    [[nodiscard]] u8* placement() noexcept { return _placement; }

    [[nodiscard]] ID3D10Buffer*       * bufferViews()       noexcept { return _bufferViews; }
    [[nodiscard]] ID3D10Buffer* const * bufferViews() const noexcept { return _bufferViews; }

    [[nodiscard]] ID3D10ShaderResourceView*       * srvViews()       noexcept { return _srvViews; }
    [[nodiscard]] ID3D10ShaderResourceView* const * srvViews() const noexcept { return _srvViews; }
};

class TAU_DLL DX10DescriptorSamplerTable final
{
    DELETE_CM(DX10DescriptorSamplerTable);
private:
    TauAllocator* _allocator;
    uSys _count;
    union
    {
        u8* _placement;
        ID3D10SamplerState** _samplers;
    };
public:
    DX10DescriptorSamplerTable(TauAllocator* const allocator, const DescriptorType type, const uSys count, u8* const placement) noexcept
        : _allocator(allocator)
        , _count(count)
        , _placement(placement)
    { }

    ~DX10DescriptorSamplerTable() noexcept;

    [[nodiscard]] uSys count() const noexcept { return _count; }

    [[nodiscard]] u8* placement() noexcept { return _placement; }

    [[nodiscard]] ID3D10SamplerState*      * samplers()       noexcept { return _samplers; }
    [[nodiscard]] ID3D10SamplerState* const* samplers() const noexcept { return _samplers; }
};

class TAU_DLL DX10DescriptorHeap final : public IDescriptorHeap
{
    DEFAULT_DESTRUCT(DX10DescriptorHeap);
    DELETE_CM(DX10DescriptorHeap);
    DESCRIPTOR_HEAP_IMPL(DX10DescriptorHeap);
private:
#if defined(TAU_PRODUCTION)
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
#else
    using FBAllocator = FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#endif
private:
    /**
     * Used to allocate the actual descriptor table objects.
     *
     *   This allocator operates on a fixed size block and is
     * capable of deallocating blocks. This makes it a very fast
     * allocator, ensuring minimal overhead.
     *
     *   Depending on whether or not we are in production mode this
     * allocator may track double deletions.
     */
    FBAllocator _allocator;
public:
    DX10DescriptorHeap(uSys maxTables) noexcept;

    [[nodiscard]] DescriptorTable allocateTable(uSys descriptors, DescriptorType type, TauAllocator* allocator) noexcept override;
    [[nodiscard]] void destroyTable(DescriptorTable table) noexcept override;
};

class TAU_DLL DX10DescriptorSamplerHeap final : public IDescriptorSamplerHeap
{
    DEFAULT_DESTRUCT(DX10DescriptorSamplerHeap);
    DELETE_CM(DX10DescriptorSamplerHeap);
    DESCRIPTOR_SAMPLER_HEAP_IMPL(DX10DescriptorSamplerHeap);
private:
#if defined(TAU_PRODUCTION)
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
#else
    using FBAllocator = FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#endif
private:
    /**
     * Used to allocate the actual descriptor table objects.
     *
     *   This allocator operates on a fixed size block and is
     * capable of deallocating blocks. This makes it a very fast
     * allocator, ensuring minimal overhead.
     *
     *   Depending on whether or not we are in production mode this
     * allocator may track double deletions.
     */
    FBAllocator _allocator;
public:
    DX10DescriptorSamplerHeap(uSys maxTables) noexcept;

    [[nodiscard]] DescriptorSamplerTable allocateTable(uSys descriptors, TauAllocator* allocator) noexcept override;
    [[nodiscard]] void destroyTable(DescriptorSamplerTable table) noexcept override;
};

class TAU_DLL DX10DescriptorHeapBuilder final : public IDescriptorHeapBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10DescriptorHeapBuilder);
    DEFAULT_DESTRUCT(DX10DescriptorHeapBuilder);
    DEFAULT_CM_PU(DX10DescriptorHeapBuilder);
public:
    [[nodiscard]] DX10DescriptorHeap* build(const DescriptorHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10DescriptorHeap* build(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDescriptorHeap> buildCPPRef(const DescriptorHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDescriptorHeap> buildTauRef(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDescriptorHeap> buildTauSRef(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10DescriptorSamplerHeap* build(const DescriptorSamplerHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10DescriptorSamplerHeap* build(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDescriptorSamplerHeap> buildCPPRef(const DescriptorSamplerHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDescriptorSamplerHeap> buildTauRef(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDescriptorSamplerHeap> buildTauSRef(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] uSys _allocSize(uSys type) const noexcept override
    {
        switch(type)
        {
            case 1: return sizeof(DX10DescriptorHeap);
            case 2: return NullableRef<DX10DescriptorHeap>::allocSize();
            case 3: return NullableStrongRef<DX10DescriptorHeap>::allocSize();
            case 4: return sizeof(DX10DescriptorSamplerHeap);
            case 5: return NullableRef<DX10DescriptorSamplerHeap>::allocSize();
            case 6: return NullableStrongRef<DX10DescriptorSamplerHeap>::allocSize();
            default: return 0;
        }
    }
};
#endif
