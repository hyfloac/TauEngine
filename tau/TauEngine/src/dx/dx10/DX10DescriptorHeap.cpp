#include "dx/dx10/DX10DescriptorHeap.hpp"

#ifdef _WIN32
DX10DescriptorTable::~DX10DescriptorTable() noexcept
{
    switch(_type)
    {
        case DescriptorType::TextureView:
            for(uSys i = 0; i < _count; ++i)
            {
                _srvViews[i]->Release();
            }
            break;
        case DescriptorType::UniformBufferView:
            for(uSys i = 0; i < _count; ++i)
            {
                _bufferViews[i]->Release();
            }
            break;
        default: break;
    }

    if(_allocator)
    {
        _allocator->deallocate(_placement);
    }
    else
    {
        delete[] _placement;
    }
}

DX10DescriptorHeap::DX10DescriptorHeap(const uSys maxTables) noexcept
    : _allocator(sizeof(DX10DescriptorTable), maxTables)
{ }

DescriptorTable DX10DescriptorHeap::allocateTable(const uSys descriptors, const DescriptorType type, TauAllocator* const allocator) noexcept
{
    void* placement;

    if(allocator)
    {
        switch(type)
        {
            case DescriptorType::TextureView:
                placement = allocator->allocateT<ID3D10ShaderResourceView*>(descriptors);
                break;
            case DescriptorType::UniformBufferView:
                placement = allocator->allocateT<ID3D10Buffer*>(descriptors);
                break;
            default: return { null };
        }
    }
    else
    {
        switch(type)
        {
            case DescriptorType::TextureView:
                placement = new(::std::nothrow) ID3D10ShaderResourceView* [descriptors];
                break;
            case DescriptorType::UniformBufferView:
                placement = new(::std::nothrow) ID3D10Buffer* [descriptors];
                break;
            default: return { null };
        }
    }

    if(!placement)
    { return { null }; }

    DX10DescriptorTable* const ret = _allocator.allocateT<DX10DescriptorTable>(allocator, type, descriptors, reinterpret_cast<u8*>(placement));
    return { ret };
}

void DX10DescriptorHeap::destroyTable(const DescriptorTable table) noexcept
{
    DX10DescriptorTable* const dxTable = reinterpret_cast<DX10DescriptorTable*>(table.raw);
    _allocator.deallocateT<DX10DescriptorTable>(dxTable);
}

DX10DescriptorHeap* DX10DescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    DX10DescriptorHeap* const heap = new(::std::nothrow) DX10DescriptorHeap(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

DX10DescriptorHeap* DX10DescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DX10DescriptorHeap* const heap = allocator.allocateT<DX10DescriptorHeap>(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

CPPRef<IDescriptorHeap> DX10DescriptorHeapBuilder::buildCPPRef(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    const CPPRef<DX10DescriptorHeap> heap(new(::std::nothrow) DX10DescriptorHeap(args.maxTables));
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableRef<IDescriptorHeap> DX10DescriptorHeapBuilder::buildTauRef(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    const NullableRef<DX10DescriptorHeap> heap(allocator, args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableStrongRef<IDescriptorHeap> DX10DescriptorHeapBuilder::buildTauSRef(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    const NullableStrongRef<DX10DescriptorHeap> heap(allocator, args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}
#endif
