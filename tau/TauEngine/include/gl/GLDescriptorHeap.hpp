/**
 * @file
 */
#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "graphics/DescriptorHeap.hpp"
#include "allocator/FixedBlockAllocator.hpp"

class GLTextureView;
class GLTextureSampler;

class TAU_DLL GLTextureViewDescriptorHeap final : public IDescriptorHeap
{
    DELETE_CM(GLTextureViewDescriptorHeap);
    DESCRIPTOR_HEAP_IMPL(GLTextureViewDescriptorHeap);
private:
    union
    {
        void* _placement;
        GLTextureView* _heap;
    };
public:
    GLTextureViewDescriptorHeap(uSys maxDescriptors) noexcept;

    ~GLTextureViewDescriptorHeap();

    [[nodiscard]] EGraphics::DescriptorType type() const noexcept override { return EGraphics::DescriptorType::TextureView; }

    [[nodiscard]] CPUDescriptorHandle getBaseCPUHandle() const noexcept override { return CPUDescriptorHandle(static_cast<uSys>(reinterpret_cast<uPtr>(_heap))); }
    [[nodiscard]] GPUDescriptorHandle getBaseGPUHandle() const noexcept override { return GPUDescriptorHandle(static_cast<u64> (reinterpret_cast<uPtr>(_heap))); }

    [[nodiscard]] uSys getOffsetStride() const noexcept override;
};

class TAU_DLL GLUniformBufferViewDescriptorHeap final : public IDescriptorHeap
{
    DELETE_CM(GLUniformBufferViewDescriptorHeap);
    DESCRIPTOR_HEAP_IMPL(GLUniformBufferViewDescriptorHeap);
private:
    union
    {
        void* _placement;
        GLuint* _heap;
    };
public:
    GLUniformBufferViewDescriptorHeap(uSys maxDescriptors) noexcept;

    ~GLUniformBufferViewDescriptorHeap();

    [[nodiscard]] EGraphics::DescriptorType type() const noexcept override { return EGraphics::DescriptorType::UniformBufferView; }

    [[nodiscard]] CPUDescriptorHandle getBaseCPUHandle() const noexcept override { return CPUDescriptorHandle(static_cast<uSys>(reinterpret_cast<uPtr>(_heap))); }
    [[nodiscard]] GPUDescriptorHandle getBaseGPUHandle() const noexcept override { return GPUDescriptorHandle(static_cast<u64> (reinterpret_cast<uPtr>(_heap))); }

    [[nodiscard]] uSys getOffsetStride() const noexcept override { return sizeof(GLuint); }
};

class TAU_DLL GLDescriptorHeapBuilder final : public IDescriptorHeapBuilder
{
    DEFAULT_CONSTRUCT_PU(GLDescriptorHeapBuilder);
    DEFAULT_DESTRUCT(GLDescriptorHeapBuilder);
    DEFAULT_CM_PU(GLDescriptorHeapBuilder);
public:
    [[nodiscard]] IDescriptorHeap* build(const DescriptorHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] IDescriptorHeap* build(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDescriptorHeap> buildCPPRef(const DescriptorHeapArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDescriptorHeap> buildTauRef(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDescriptorHeap> buildTauSRef(const DescriptorHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

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
