#include "gl/GLBufferView.hpp"

#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLResourceBuffer.hpp"

GLVertexBufferView* GLBufferViewBuilder::build(const VertexBufferViewArgs& args, Error* const error) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLVertexBufferView* const bufferView = new(::std::nothrow) GLVertexBufferView(args.descriptor, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

GLVertexBufferView* GLBufferViewBuilder::build(const VertexBufferViewArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLVertexBufferView* const bufferView = allocator.allocateT<GLVertexBufferView>(args.descriptor, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

CPPRef<IVertexBufferView> GLBufferViewBuilder::buildCPPRef(const VertexBufferViewArgs& args, Error* const error) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLVertexBufferView> bufferView(new(::std::nothrow) GLVertexBufferView(args.descriptor, *glArgs.buffer));
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableRef<IVertexBufferView> GLBufferViewBuilder::buildTauRef(const VertexBufferViewArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLVertexBufferView> bufferView(allocator, args.descriptor, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableStrongRef<IVertexBufferView> GLBufferViewBuilder::buildTauSRef(const VertexBufferViewArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLVertexBufferView> bufferView(allocator, args.descriptor, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

GLIndexBufferView* GLBufferViewBuilder::build(const IndexBufferViewArgs& args, Error* const error) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLIndexBufferView* const bufferView = new(::std::nothrow) GLIndexBufferView(args.indexSize, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

GLIndexBufferView* GLBufferViewBuilder::build(const IndexBufferViewArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLIndexBufferView* const bufferView = allocator.allocateT<GLIndexBufferView>(args.indexSize, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

CPPRef<IIndexBufferView> GLBufferViewBuilder::buildCPPRef(const IndexBufferViewArgs& args, Error* const error) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLIndexBufferView> bufferView(new(::std::nothrow) GLIndexBufferView(args.indexSize, *glArgs.buffer));
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableRef<IIndexBufferView> GLBufferViewBuilder::buildTauRef(const IndexBufferViewArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLIndexBufferView> bufferView(allocator, args.indexSize, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableStrongRef<IIndexBufferView> GLBufferViewBuilder::buildTauSRef(const IndexBufferViewArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLIndexBufferView> bufferView(allocator, args.indexSize, *glArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

GLUniformBufferView* GLBufferViewBuilder::build(const UniformBufferViewArgs& args, Error* const error, const DescriptorTable table, const uSys tableIndex) const noexcept
{
    ERROR_CODE_COND_N(!table.raw, Error::DescriptorTableIsNull);
    GLDescriptorTable* const glTable = reinterpret_cast<GLDescriptorTable*>(table.raw);
    ERROR_CODE_COND_N(glTable->type() != DescriptorType::UniformBufferView, Error::InternalError);

    GLBufferViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLUniformBufferView* const bufferView = new(glTable->placement() + (tableIndex * sizeof(GLUniformBufferView))) GLUniformBufferView(*glArgs.buffer);

    ERROR_CODE_V(Error::NoError, bufferView);
}

bool GLBufferViewBuilder::processArgs(const VertexBufferViewArgs& args, GLBufferViewArgs* const glArgs, Error* const error) noexcept
{
    ERROR_CODE_COND_F(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_F(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    GLResource* dxResource = RTTD_CAST(args.buffer, GLResource, IResource);
    ERROR_CODE_COND_F(!glArgs->buffer, Error::InternalError);

    glArgs->buffer = static_cast<GLResourceBuffer*>(dxResource);

    return true;
}

bool GLBufferViewBuilder::processArgs(const IndexBufferViewArgs& args, GLBufferViewArgs* const glArgs, Error* const error) noexcept
{
    ERROR_CODE_COND_F(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_F(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    GLResource* dxResource = RTTD_CAST(args.buffer, GLResource, IResource);
    ERROR_CODE_COND_F(!glArgs->buffer, Error::InternalError);

    glArgs->buffer = static_cast<GLResourceBuffer*>(dxResource);

    return true;
}

bool GLBufferViewBuilder::processArgs(const UniformBufferViewArgs& args, GLBufferViewArgs* const glArgs, Error* const error) noexcept
{
    ERROR_CODE_COND_F(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_F(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    GLResource* dxResource = RTTD_CAST(args.buffer, GLResource, IResource);
    ERROR_CODE_COND_F(!glArgs->buffer, Error::InternalError);

    glArgs->buffer = static_cast<GLResourceBuffer*>(dxResource);

    return true;
}
