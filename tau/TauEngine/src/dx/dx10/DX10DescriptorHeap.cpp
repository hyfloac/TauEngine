#include "dx/dx10/DX10DescriptorHeap.hpp"

#ifdef _WIN32
DX10TextureViewDescriptorHeap::DX10TextureViewDescriptorHeap(uSys maxDescriptors) noexcept
    : _placement(::std::malloc(sizeof(ID3D10ShaderResourceView*) * maxDescriptors + sizeof(uSys)))
{
    // Store array count
    *reinterpret_cast<uSys*>(_placement) = maxDescriptors;
    _placement = reinterpret_cast<u8*>(_placement) + sizeof(uSys);

    // Zero memory to help with ID3D10ShaderResourceView releasing
    ::std::memset(_placement, 0, sizeof(ID3D10ShaderResourceView*) * maxDescriptors);
}

DX10TextureViewDescriptorHeap::~DX10TextureViewDescriptorHeap() noexcept
{
    // Retrieve the array count
    const uSys descriptorCount = *(reinterpret_cast<uSys*>(_placement) - 1);

    for(uSys i = 0; i < descriptorCount; ++i)
    {
        if(_heap[i])
        {
            // Release any active ID3DShaderResourceView's
            _heap[i]->Release();
        }
    }

    ::std::free(_placement);
}

DX10UniformBufferViewDescriptorHeap::DX10UniformBufferViewDescriptorHeap(uSys maxDescriptors) noexcept
    : _placement(::std::malloc(sizeof(ID3D10Buffer*) * maxDescriptors))
{ }

DX10UniformBufferViewDescriptorHeap::~DX10UniformBufferViewDescriptorHeap() noexcept
{ ::std::free(_placement); }


IDescriptorHeap* DX10DescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    IDescriptorHeap* heap = null;

    switch(args.type)
    {
        case EGraphics::DescriptorType::TextureView:
            heap = new(::std::nothrow) DX10TextureViewDescriptorHeap(args.numDescriptors);
            break;
        case EGraphics::DescriptorType::UniformBufferView:
            heap = new(::std::nothrow) DX10UniformBufferViewDescriptorHeap(args.numDescriptors);
            break;
        default: break;
    }

    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

IDescriptorHeap* DX10DescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    IDescriptorHeap* heap = null;

    switch(args.type)
    {
        case EGraphics::DescriptorType::TextureView:
            heap = allocator.allocateT<DX10TextureViewDescriptorHeap>(args.numDescriptors);
            break;
        case EGraphics::DescriptorType::UniformBufferView:
            heap = allocator.allocateT<DX10UniformBufferViewDescriptorHeap>(args.numDescriptors);
            break;
        default: break;
    }

    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

CPPRef<IDescriptorHeap> DX10DescriptorHeapBuilder::buildCPPRef(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    CPPRef<IDescriptorHeap> heap = null;
    
    switch(args.type)
    {
        case EGraphics::DescriptorType::TextureView:
            heap = CPPRef<IDescriptorHeap>(new(::std::nothrow) DX10TextureViewDescriptorHeap(args.numDescriptors));
            break;
        case EGraphics::DescriptorType::UniformBufferView:
            heap = CPPRef<IDescriptorHeap>(new(::std::nothrow) DX10UniformBufferViewDescriptorHeap(args.numDescriptors));
            break;
        default: break;
    }

    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableRef<IDescriptorHeap> DX10DescriptorHeapBuilder::buildTauRef(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    NullableRef<IDescriptorHeap> heap = null;
    
    switch(args.type)
    {
        case EGraphics::DescriptorType::TextureView:
            heap = NullableRef<DX10TextureViewDescriptorHeap>(allocator, args.numDescriptors);
            break;
        case EGraphics::DescriptorType::UniformBufferView:
            heap = NullableRef<DX10UniformBufferViewDescriptorHeap>(allocator, args.numDescriptors);
            break;
        default: break;
    }

    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableStrongRef<IDescriptorHeap> DX10DescriptorHeapBuilder::buildTauSRef(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    NullableStrongRef<IDescriptorHeap> heap = null;
    
    switch(args.type)
    {
        case EGraphics::DescriptorType::TextureView:
            heap = NullableStrongRef<DX10TextureViewDescriptorHeap>(allocator, args.numDescriptors);
            break;
        case EGraphics::DescriptorType::UniformBufferView:
            heap = NullableStrongRef<DX10UniformBufferViewDescriptorHeap>(allocator, args.numDescriptors);
            break;
        default: break;
    }

    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}
#endif
