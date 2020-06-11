#include "dx/dx10/DX10BufferView.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10ResourceBuffer.hpp"

DX10VertexBufferView* DX10BufferViewBuilder::build(const VertexBufferViewArgs& args, Error* error) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10VertexBufferView* const bufferView = new(::std::nothrow) DX10VertexBufferView(args.descriptor, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

DX10VertexBufferView* DX10BufferViewBuilder::build(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10VertexBufferView* const bufferView = allocator.allocateT<DX10VertexBufferView>(args.descriptor, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

CPPRef<IVertexBufferView> DX10BufferViewBuilder::buildCPPRef(const VertexBufferViewArgs& args, Error* error) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10VertexBufferView> bufferView(new(::std::nothrow) DX10VertexBufferView(args.descriptor, *dxArgs.buffer));
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableRef<IVertexBufferView> DX10BufferViewBuilder::buildTauRef(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10VertexBufferView> bufferView(allocator, args.descriptor, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableStrongRef<IVertexBufferView> DX10BufferViewBuilder::buildTauSRef(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10VertexBufferView> bufferView(allocator, args.descriptor, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

DX10IndexBufferView* DX10BufferViewBuilder::build(const IndexBufferViewArgs& args, Error* error) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10IndexBufferView* const bufferView = new(::std::nothrow) DX10IndexBufferView(args.indexSize, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

DX10IndexBufferView* DX10BufferViewBuilder::build(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10IndexBufferView* const bufferView = allocator.allocateT<DX10IndexBufferView>(args.indexSize, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

CPPRef<IIndexBufferView> DX10BufferViewBuilder::buildCPPRef(const IndexBufferViewArgs& args, Error* error) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10IndexBufferView> bufferView(new(::std::nothrow) DX10IndexBufferView(args.indexSize, *dxArgs.buffer));
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableRef<IIndexBufferView> DX10BufferViewBuilder::buildTauRef(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10IndexBufferView> bufferView(allocator, args.indexSize, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableStrongRef<IIndexBufferView> DX10BufferViewBuilder::buildTauSRef(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10IndexBufferView> bufferView(allocator, args.indexSize, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

DX10UniformBufferView* DX10BufferViewBuilder::build(const UniformBufferViewArgs& args, Error* error) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10UniformBufferView* const bufferView = new(::std::nothrow) DX10UniformBufferView(*dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

DX10UniformBufferView* DX10BufferViewBuilder::build(const UniformBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10UniformBufferView* const bufferView = allocator.allocateT<DX10UniformBufferView>(*dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

CPPRef<IUniformBufferView> DX10BufferViewBuilder::buildCPPRef(const UniformBufferViewArgs& args, Error* error) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10UniformBufferView> bufferView(new(::std::nothrow) DX10UniformBufferView(*dxArgs.buffer));
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableRef<IUniformBufferView> DX10BufferViewBuilder::buildTauRef(const UniformBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10UniformBufferView> bufferView(allocator, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

NullableStrongRef<IUniformBufferView> DX10BufferViewBuilder::buildTauSRef(const UniformBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXBufferViewArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10UniformBufferView> bufferView(allocator, *dxArgs.buffer);
    ERROR_CODE_COND_F(!bufferView, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, bufferView);
}

bool DX10BufferViewBuilder::processArgs(const VertexBufferViewArgs& args, DXBufferViewArgs* dxArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_F(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    DX10Resource* dxResource = RTTD_CAST(args.buffer, DX10Resource, IResource);
    ERROR_CODE_COND_F(!dxArgs->buffer, Error::InternalError);

    dxArgs->buffer = static_cast<DX10ResourceBuffer*>(dxResource);

    return true;
}

bool DX10BufferViewBuilder::processArgs(const IndexBufferViewArgs& args, DXBufferViewArgs* dxArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_F(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    DX10Resource* dxResource = RTTD_CAST(args.buffer, DX10Resource, IResource);
    ERROR_CODE_COND_F(!dxArgs->buffer, Error::InternalError);

    dxArgs->buffer = static_cast<DX10ResourceBuffer*>(dxResource);

    return true;
}

bool DX10BufferViewBuilder::processArgs(const UniformBufferViewArgs& args, DXBufferViewArgs* dxArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_F(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    DX10Resource* dxResource = RTTD_CAST(args.buffer, DX10Resource, IResource);
    ERROR_CODE_COND_F(!dxArgs->buffer, Error::InternalError);

    dxArgs->buffer = static_cast<DX10ResourceBuffer*>(dxResource);

    return true;
}
#endif
