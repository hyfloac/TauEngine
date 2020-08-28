#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLTextureView.hpp"
#include "gl/GLTextureSampler.hpp"

GLTextureViewDescriptorHeap::GLTextureViewDescriptorHeap(uSys maxDescriptors) noexcept
    : _placement(::std::malloc(sizeof(GLTextureView) * maxDescriptors))
{ }

GLTextureViewDescriptorHeap::~GLTextureViewDescriptorHeap() noexcept
{ ::std::free(_placement); }

uSys GLTextureViewDescriptorHeap::getOffsetStride() const noexcept
{ return sizeof(GLTextureView); }

GLUniformBufferViewDescriptorHeap::GLUniformBufferViewDescriptorHeap(uSys maxDescriptors) noexcept
    : _placement(::std::malloc(sizeof(GLuint) * maxDescriptors))
{ }

GLUniformBufferViewDescriptorHeap::~GLUniformBufferViewDescriptorHeap()
{ ::std::free(_placement); }

IDescriptorHeap* GLDescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error) const noexcept
{
    GLDescriptorHeap* const heap = new(::std::nothrow) GLDescriptorHeap(args.maxTables);
    ERROR_CODE_COND_N(!heap, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, heap);
}

IDescriptorHeap* GLDescriptorHeapBuilder::build(const DescriptorHeapArgs& args, Error* const error, TauAllocator& allocator) const noexcept
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
