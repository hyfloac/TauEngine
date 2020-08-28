/**
 * @file
 */
#pragma once

#include "graphics/DescriptorHeap.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "allocator/FixedBlockAllocator.hpp"

class TAU_DLL DX10TextureViewDescriptorHeap final : public IDescriptorHeap
{
    DELETE_CM(DX10TextureViewDescriptorHeap);
    DESCRIPTOR_HEAP_IMPL(DX10TextureViewDescriptorHeap);
private:
    union
    {
        void* _placement;
        ID3D10ShaderResourceView** _heap;
    };
public:
    DX10TextureViewDescriptorHeap(uSys maxDescriptors) noexcept;

    ~DX10TextureViewDescriptorHeap() noexcept override;

    [[nodiscard]] EGraphics::DescriptorType type() const noexcept override { return EGraphics::DescriptorType::TextureView; }

    [[nodiscard]] CPUDescriptorHandle getBaseCPUHandle() const noexcept override { return CPUDescriptorHandle(static_cast<uSys>(reinterpret_cast<uPtr>(_heap))); }
    [[nodiscard]] GPUDescriptorHandle getBaseGPUHandle() const noexcept override { return GPUDescriptorHandle(static_cast<u64> (reinterpret_cast<uPtr>(_heap))); }

    [[nodiscard]] uSys getOffsetStride() const noexcept override { return sizeof(ID3D10ShaderResourceView*); }
};

class TAU_DLL DX10UniformBufferViewDescriptorHeap final : public IDescriptorHeap
{
    DELETE_CM(DX10UniformBufferViewDescriptorHeap);
    DESCRIPTOR_HEAP_IMPL(DX10UniformBufferViewDescriptorHeap);
private:
    union
    {
        void* _placement;
        ID3D10Buffer** _heap;
    };
public:
    DX10UniformBufferViewDescriptorHeap(uSys maxDescriptors) noexcept;

    ~DX10UniformBufferViewDescriptorHeap() noexcept override;

    [[nodiscard]] EGraphics::DescriptorType type() const noexcept override { return EGraphics::DescriptorType::UniformBufferView; }

    [[nodiscard]] CPUDescriptorHandle getBaseCPUHandle() const noexcept override { return CPUDescriptorHandle(static_cast<uSys>(reinterpret_cast<uPtr>(_heap))); }
    [[nodiscard]] GPUDescriptorHandle getBaseGPUHandle() const noexcept override { return GPUDescriptorHandle(static_cast<u64> (reinterpret_cast<uPtr>(_heap))); }

    [[nodiscard]] uSys getOffsetStride() const noexcept override { return sizeof(ID3D10Buffer*); }
};

class TAU_DLL DX10DescriptorHeapBuilder final : public IDescriptorHeapBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10DescriptorHeapBuilder);
    DEFAULT_DESTRUCT(DX10DescriptorHeapBuilder);
    DEFAULT_CM_PU(DX10DescriptorHeapBuilder);
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
            case _DHB_AS_RAW_TV:  return sizeof(DX10TextureViewDescriptorHeap);
            case _DHB_AS_NR_TV:   return NullableRef<DX10TextureViewDescriptorHeap>::allocSize();
            case _DHB_AS_NSR_TV:  return NullableStrongRef<DX10TextureViewDescriptorHeap>::allocSize();
            case _DHB_AS_RAW_RTV:
            case _DHB_AS_NR_RTV:
            case _DHB_AS_NSR_RTV:
            case _DHB_AS_RAW_DSV:
            case _DHB_AS_NR_DSV:
            case _DHB_AS_NSR_DSV: return 0;
            case _DHB_AS_RAW_UBV: return sizeof(DX10UniformBufferViewDescriptorHeap);
            case _DHB_AS_NR_UBV:  return NullableRef<DX10UniformBufferViewDescriptorHeap>::allocSize();
            case _DHB_AS_NSR_UBV: return NullableStrongRef<DX10UniformBufferViewDescriptorHeap>::allocSize();
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
#endif
