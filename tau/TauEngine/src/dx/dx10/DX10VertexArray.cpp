#include "dx/dx10/DX10VertexArray.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10Buffer.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Resource.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"

NullableRef<IVertexArray> DX10VertexArrayBuilder::buildTauRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs(args.bufferCount);
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10VertexArray> va(allocator, args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

bool DX10VertexArrayBuilder::processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, Error* error) const noexcept
{
    for(uSys i = 0; i < args.bufferCount; ++i)
    {
        ERROR_CODE_COND_F(!args.bufferViews[i].buffer, Error::BuffersNotSet);
        ERROR_CODE_COND_F(args.bufferViews[i].buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);
        ERROR_CODE_COND_F(!RTTD_CHECK(args.bufferViews[i].buffer, DX10Resource, IResource), Error::InternalError);
    }

    dxArgs->iaBuffers = new ID3D10Buffer* [args.bufferCount];
    ERROR_CODE_COND_F(!dxArgs->iaBuffers, Error::SystemMemoryAllocationFailure);

    for(uSys i = 0; i < args.bufferCount; ++i)
    {
        dxArgs->iaBuffers[i] = RefCast<DX10ResourceBuffer>(args.bufferViews[i].buffer)->d3dBuffer();
    }

    return true;
}
#endif
