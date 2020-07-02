/**
 * @file
 */
#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "graphics/DescriptorHeap.hpp"
#include "allocator/FixedBlockAllocator.hpp"

class GLResourceTexture;
class GLUniformBufferView;
class GLTextureSampler;

class TAU_DLL GLDescriptorTable final
{
    DELETE_CM(GLDescriptorTable);
private:
    TauAllocator* _allocator;
    DescriptorType _type;
    uSys _count;
    union
    {
        u8* _placement;
        GLResourceTexture** _texViews;
        GLuint* _uniViews;
    };
public:
    GLDescriptorTable(TauAllocator* const allocator, const DescriptorType type, const uSys count, u8* const placement) noexcept
        : _allocator(allocator)
        , _type(type)
        , _count(count)
        , _placement(placement)
    { }

    ~GLDescriptorTable() noexcept;

    [[nodiscard]] DescriptorType type() const noexcept { return _type; }
    [[nodiscard]] uSys count() const noexcept { return _count; }

    [[nodiscard]] u8* placement() noexcept { return _placement; }

    [[nodiscard]] GLResourceTexture*       * texViews()       noexcept { return _texViews; }
    [[nodiscard]] GLResourceTexture* const * texViews() const noexcept { return _texViews; }

    [[nodiscard]]       GLuint* uniViews()       noexcept { return _uniViews; }
    [[nodiscard]] const GLuint* uniViews() const noexcept { return _uniViews; }
};

class TAU_DLL GLDescriptorSamplerTable final
{
    DELETE_CM(GLDescriptorSamplerTable);
private:
    TauAllocator* _allocator;
    uSys _count;
    union
    {
        u8* _placement;
        GLuint* _samplers;
    };
public:
    GLDescriptorSamplerTable(TauAllocator* const allocator, const uSys count, u8* const placement) noexcept
        : _allocator(allocator)
        , _count(count)
        , _placement(placement)
    { }

    ~GLDescriptorSamplerTable() noexcept;

    [[nodiscard]] uSys count() const noexcept { return _count; }

    [[nodiscard]] u8* placement() noexcept { return _placement; }

    [[nodiscard]]       GLuint* samplers()       noexcept { return _samplers; }
    [[nodiscard]] const GLuint* samplers() const noexcept { return _samplers; }
};

class TAU_DLL GLDescriptorHeap final : public IDescriptorHeap
{
    DEFAULT_DESTRUCT(GLDescriptorHeap);
    DELETE_CM(GLDescriptorHeap);
    DESCRIPTOR_HEAP_IMPL(GLDescriptorHeap);
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
    GLDescriptorHeap(uSys maxTables) noexcept;

    [[nodiscard]] DescriptorTable allocateTable(uSys descriptors, DescriptorType type, TauAllocator* allocator) noexcept override;
    [[nodiscard]] void destroyTable(DescriptorTable table) noexcept override;
};

class TAU_DLL GLDescriptorSamplerHeap final : public IDescriptorSamplerHeap
{
    DEFAULT_DESTRUCT(GLDescriptorSamplerHeap);
    DELETE_CM(GLDescriptorSamplerHeap);
    DESCRIPTOR_SAMPLER_HEAP_IMPL(GLDescriptorSamplerHeap);
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
    GLDescriptorSamplerHeap(uSys maxTables) noexcept;

    [[nodiscard]] DescriptorSamplerTable allocateTable(uSys descriptors, TauAllocator* allocator) noexcept override;
    [[nodiscard]] void destroyTable(DescriptorSamplerTable table) noexcept override;
};

class TAU_DLL GLDescriptorHeapBuilder final : public IDescriptorHeapBuilder
{
    DEFAULT_CONSTRUCT_PU(GLDescriptorHeapBuilder);
    DEFAULT_DESTRUCT(GLDescriptorHeapBuilder);
    DEFAULT_CM_PU(GLDescriptorHeapBuilder);
public:
    [[nodiscard]] GLDescriptorHeap* build(const DescriptorHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLDescriptorHeap* build(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDescriptorHeap> buildCPPRef(const DescriptorHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDescriptorHeap> buildTauRef(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDescriptorHeap> buildTauSRef(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] IDescriptorSamplerHeap* build(const DescriptorSamplerHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] IDescriptorSamplerHeap* build(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDescriptorSamplerHeap> buildCPPRef(const DescriptorSamplerHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDescriptorSamplerHeap> buildTauRef(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDescriptorSamplerHeap> buildTauSRef(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] uSys _allocSize(const uSys type) const noexcept override
    {
        switch(type)
        {
            case 1: return sizeof(GLDescriptorHeap);
            case 2: return NullableRef<GLDescriptorHeap>::allocSize();
            case 3: return NullableStrongRef<GLDescriptorHeap>::allocSize();
            case 4: return sizeof(GLDescriptorSamplerHeap);
            case 5: return NullableRef<GLDescriptorSamplerHeap>::allocSize();
            case 6: return NullableStrongRef<GLDescriptorSamplerHeap>::allocSize();
            default: return 0;
        }
    }
};
