#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLTextureView.hpp"
#include "gl/GLTextureSampler.hpp"

GLDescriptorTable::~GLDescriptorTable() noexcept
{
    if(_allocator)
    {
        _allocator->deallocate(_placement);
    }
    else
    {
        delete[] _placement;
    }
}

GLDescriptorSamplerTable::~GLDescriptorSamplerTable() noexcept
{
    for(uSys i = 0; i < _count; ++i)
    {
        _samplers[i].~GLTextureSampler();
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
                placement = allocator->allocate(sizeof(GLResourceTexture*) * descriptors);
                break;
            case DescriptorType::UniformBufferView:
                placement = allocator->allocate(sizeof(GLuint) * descriptors);
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
                break;
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

GLDescriptorSamplerHeap::GLDescriptorSamplerHeap(const uSys maxTables) noexcept
    : _allocator(sizeof(GLDescriptorSamplerTable), maxTables)
{ }

DescriptorSamplerTable GLDescriptorSamplerHeap::allocateTable(const uSys descriptors, TauAllocator* const allocator) noexcept
{
    void* placement;

    if(allocator)
    {
        placement = allocator->allocate(sizeof(GLTextureSampler) * descriptors);
    }
    else
    {
        placement = new(::std::nothrow) u8*[sizeof(GLTextureSampler) * descriptors];
    }

    if(!placement)
    { return { null }; }

    GLDescriptorSamplerTable* const ret = _allocator.allocateT<GLDescriptorSamplerTable>(allocator, descriptors, reinterpret_cast<u8*>(placement));
    return { ret };
}

void GLDescriptorSamplerHeap::destroyTable(const DescriptorSamplerTable table) noexcept
{
    GLDescriptorSamplerTable* const glTable = reinterpret_cast<GLDescriptorSamplerTable*>(table.raw);
    _allocator.deallocateT<GLDescriptorSamplerTable>(glTable);
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

IDescriptorSamplerHeap* GLDescriptorHeapBuilder::build(const DescriptorSamplerHeapArgs& args, Error* error) const noexcept
{
    GLDescriptorSamplerHeap* const heap = new(::std::nothrow) GLDescriptorSamplerHeap(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

IDescriptorSamplerHeap* GLDescriptorHeapBuilder::build(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLDescriptorSamplerHeap* const heap = new(::std::nothrow) GLDescriptorSamplerHeap(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

CPPRef<IDescriptorSamplerHeap> GLDescriptorHeapBuilder::buildCPPRef(const DescriptorSamplerHeapArgs& args, Error* error) const noexcept
{
    const CPPRef<GLDescriptorSamplerHeap> heap(new(::std::nothrow) GLDescriptorSamplerHeap(args.maxTables));
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableRef<IDescriptorSamplerHeap> GLDescriptorHeapBuilder::buildTauRef(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    const NullableRef<GLDescriptorSamplerHeap> heap(allocator, args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

NullableStrongRef<IDescriptorSamplerHeap> GLDescriptorHeapBuilder::buildTauSRef(const DescriptorSamplerHeapArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    const NullableStrongRef<GLDescriptorSamplerHeap> heap(allocator, args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}
