#include "dx/dx12/DX12VertexArray.hpp"

#ifdef _WIN32
#include "dx/dx12/DX12ResourceBuffer.hpp"
#include "TauConfig.hpp"

DX12VertexArray* DX12VertexArrayBuilder::build(const VertexArrayArgs& args, Error* error) noexcept
{
    DXVertexArrayArgs dxArgs { DynArray<NullableRef<IResource>>(args.bufferCount), DynArray<D3D12_VERTEX_BUFFER_VIEW>(args.bufferCount) };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX12VertexArray* const va = new(::std::nothrow) DX12VertexArray(::std::move(dxArgs.buffers), ::std::move(dxArgs.d3dBuffers));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

DX12VertexArray* DX12VertexArrayBuilder::build(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs { DynArray<NullableRef<IResource>>(args.bufferCount), DynArray<D3D12_VERTEX_BUFFER_VIEW>(args.bufferCount) };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX12VertexArray* const va = allocator.allocateT<DX12VertexArray>(::std::move(dxArgs.buffers), ::std::move(dxArgs.d3dBuffers));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

CPPRef<IVertexArray> DX12VertexArrayBuilder::buildCPPRef(const VertexArrayArgs& args, Error* error) noexcept
{
    DXVertexArrayArgs dxArgs { DynArray<NullableRef<IResource>>(args.bufferCount), DynArray<D3D12_VERTEX_BUFFER_VIEW>(args.bufferCount) };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX12VertexArray> va(new(::std::nothrow) DX12VertexArray(::std::move(dxArgs.buffers), ::std::move(dxArgs.d3dBuffers)));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

NullableRef<IVertexArray> DX12VertexArrayBuilder::buildTauRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs { DynArray<NullableRef<IResource>>(args.bufferCount), DynArray<D3D12_VERTEX_BUFFER_VIEW>(args.bufferCount) };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX12VertexArray> va(allocator, ::std::move(dxArgs.buffers), ::std::move(dxArgs.d3dBuffers));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

NullableStrongRef<IVertexArray> DX12VertexArrayBuilder::buildTauSRef(const VertexArrayArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs { DynArray<NullableRef<IResource>>(args.bufferCount), DynArray<D3D12_VERTEX_BUFFER_VIEW>(args.bufferCount) };
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX12VertexArray> va(allocator, ::std::move(dxArgs.buffers), ::std::move(dxArgs.d3dBuffers));

#if TAU_NULL_CHECK
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);
#endif

    ERROR_CODE_V(Error::NoError, va);
}

bool DX12VertexArrayBuilder::processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, Error* error) noexcept
{
#if TAU_NULL_CHECK
    ERROR_CODE_COND_F(args.bufferCount == 0 || !args.bufferViews, Error::BuffersNotSet);
#endif

    for(uSys i = 0; i < args.bufferCount; ++i)
    {
#if TAU_NULL_CHECK
        ERROR_CODE_COND_F(!args.bufferViews[i].buffer, Error::BuffersNotSet);
#endif

#if TAU_GENERAL_SAFETY_CHECK
        ERROR_CODE_COND_F(args.bufferViews[i].buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);
#endif

#if TAU_RTTI_CHECK
        ERROR_CODE_COND_F(!RTTD_CHECK(args.bufferViews[i].buffer, DX12Resource, IResource), Error::InternalError);
#endif
    }

    for(uSys i = 0; i < args.bufferCount; ++i)
    { dxArgs->buffers[i] = args.bufferViews[i].buffer; }

    for(uSys i = 0; i < args.bufferCount; ++i)
    {
        DX12ResourceBuffer* const buffer = static_cast<DX12ResourceBuffer* const>(args.bufferViews[i].buffer.get());

        dxArgs->d3dBuffers[i].BufferLocation = buffer->d3dResource()->GetGPUVirtualAddress();
        dxArgs->d3dBuffers[i].SizeInBytes = static_cast<UINT>(buffer->size());
        dxArgs->d3dBuffers[i].StrideInBytes = args.bufferViews[i].descriptor.stride();
    }

    return true;
}
#endif
