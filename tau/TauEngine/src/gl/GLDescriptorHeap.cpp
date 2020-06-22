#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLTextureView.hpp"

GLDescriptorTable::~GLDescriptorTable() noexcept
{
    switch(_type)
    {
        case DescriptorType::TextureView:
        case DescriptorType::UniformBufferView:
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

GLDescriptorHeap::GLDescriptorHeap(const uSys maxTables) noexcept
    : _allocator(sizeof(GLDescriptorTable), maxTables)
{ }

DescriptorTable GLDescriptorHeap::allocateTable(const uSys descriptors, const DescriptorType type, TauAllocator* allocator) noexcept
{
    void* placement;

    if(allocator)
    {
        switch(type)
        {
            case DescriptorType::TextureView:
                placement = allocator->allocateT<GLResourceTexture*>(descriptors);
                break;
            case DescriptorType::UniformBufferView:
                placement = allocator->allocateT<GLuint>(descriptors);
                break;
            default: return { null };
        }
    }
    else
    {
        switch(type)
        {
            case DescriptorType::TextureView:
                placement = new(::std::nothrow) GLResourceTexture* [descriptors];
            case DescriptorType::UniformBufferView:
                placement = new(::std::nothrow) GLuint[descriptors];
                break;
            default: return { null };
        }
    }

    if(!placement)
    { return { null }; }

    GLDescriptorTable* const ret = _allocator.allocateT<GLDescriptorTable>(allocator, type, descriptors, reinterpret_cast<u8*>(placement));
    return { ret };
}

void GLDescriptorHeap::destroyTable(const DescriptorTable table) noexcept
{
    GLDescriptorTable* const glTable = reinterpret_cast<GLDescriptorTable*>(table.raw);
    _allocator.deallocateT<GLDescriptorTable>(glTable);
}

GLDescriptorHeap* GLDescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    GLDescriptorHeap* const heap = new(::std::nothrow) GLDescriptorHeap(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

GLDescriptorHeap* GLDescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLDescriptorHeap* const heap = allocator.allocateT<GLDescriptorHeap>(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

CPPRef<IDescriptorHeap> GLDescriptorHeapBuilder::buildCPPRef(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    const CPPRef<GLDescriptorHeap> heap(new(::std::nothrow) GLDescriptorHeap(args.maxTables));
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableRef<IDescriptorHeap> GLDescriptorHeapBuilder::buildTauRef(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    const NullableRef<GLDescriptorHeap> heap(allocator, args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableStrongRef<IDescriptorHeap> GLDescriptorHeapBuilder::buildTauSRef(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    const NullableStrongRef<GLDescriptorHeap> heap(allocator, args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}
