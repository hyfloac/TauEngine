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

    ~GLTextureViewDescriptorHeap() noexcept override;

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

    ~GLUniformBufferViewDescriptorHeap() noexcept override;

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
            case _DHB_AS_RAW_TV:  return sizeof(GLTextureViewDescriptorHeap);
            case _DHB_AS_NR_TV:   return NullableRef<GLTextureViewDescriptorHeap>::allocSize();
            case _DHB_AS_NSR_TV:  return NullableStrongRef<GLTextureViewDescriptorHeap>::allocSize();
            case _DHB_AS_RAW_RTV:
            case _DHB_AS_NR_RTV:
            case _DHB_AS_NSR_RTV:
            case _DHB_AS_RAW_DSV:
            case _DHB_AS_NR_DSV:
            case _DHB_AS_NSR_DSV: return 0;
            case _DHB_AS_RAW_UBV: return sizeof(GLUniformBufferViewDescriptorHeap);
            case _DHB_AS_NR_UBV:  return NullableRef<GLUniformBufferViewDescriptorHeap>::allocSize();
            case _DHB_AS_NSR_UBV: return NullableStrongRef<GLUniformBufferViewDescriptorHeap>::allocSize();
            case _DHB_AS_RAW_UAV: 
            case _DHB_AS_NR_UAV:  
            case _DHB_AS_NSR_UAV:
            case _DHB_AS_RAW_S: 
            case _DHB_AS_NR_S:  
            case _DHB_AS_NSR_S:   return 0;
            default:              return 0;
        }
    }
};
