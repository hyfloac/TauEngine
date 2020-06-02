#include "dx/dx10/DX10VertexArray.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10Buffer.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauConfig.hpp"

#if TAU_RTTI_CHECK
#include "TauEngine.hpp"
  #define CTX() \
      if(!RTT_CHECK(context, DX10RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#endif

void DX10VertexArray::bind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.setBuffers(_iaBufferCount, _iaBuffers);
    if(_indexBuffer)
    {
        ctx.d3dDevice()->IASetIndexBuffer(_indexBuffer->d3dBuffer(), _indexBuffer->dxIndexSize(), 0);
    }
    ctx.d3dDevice()->IASetPrimitiveTopology(_drawTypeCache);
}

void DX10VertexArray::unbind(IRenderingContext& context) noexcept
{
    CTX();
    ID3D10Buffer* buffers[1];
    ctx.setBuffers(0, buffers);
    ctx.d3dDevice()->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
}

void DX10VertexArray::draw(IRenderingContext& context, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    CTX();

    if(_indexBuffer)
    {
        ctx.d3dDevice()->DrawIndexed(drawCount, drawOffset, 0);
    }
    else
    {
        ctx.d3dDevice()->Draw(drawCount, drawOffset);
    }
}

void DX10VertexArray::drawInstanced(IRenderingContext& context, const uSys instanceCount, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    CTX();

    if(_indexBuffer)
    {
        ctx.d3dDevice()->DrawIndexedInstanced(drawCount, instanceCount, drawOffset, 0, 0);
    }
    else
    {
        ctx.d3dDevice()->DrawInstanced(drawCount, instanceCount, drawOffset, 0);
    }
}

DX10VertexArray* DX10VertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10VertexArray* const va = new(::std::nothrow) DX10VertexArray(args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, va);
}

DX10VertexArray* DX10VertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10VertexArray* const va = allocator.allocateT<DX10VertexArray>(args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, va);
}

CPPRef<IVertexArray> DX10VertexArrayBuilder::buildCPPRef(const VertexArrayArgs& args, Error* const error) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10VertexArray> va = CPPRef<DX10VertexArray>(new(::std::nothrow) DX10VertexArray(args, dxArgs));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, va);
}

NullableRef<IVertexArray> DX10VertexArrayBuilder::buildTauRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10VertexArray> va(allocator, args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

NullableStrongRef<IVertexArray> DX10VertexArrayBuilder::buildTauSRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10VertexArray> va(allocator, args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

bool DX10VertexArrayBuilder::processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(args.drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_F(args.drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < args.buffers.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.buffers[i], Error::BuffersNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(args.buffers[i].get(), DX10VertexBuffer), Error::InternalError);
    }

    if(args.indexBuffer)
    {
        ERROR_CODE_COND_F(!RTT_CHECK(args.indexBuffer.get(), DX10IndexBuffer), Error::InternalError);
        dxArgs->indexBuffer = RefCast<DX10IndexBuffer>(args.indexBuffer);
    }
    else
    {
        dxArgs->indexBuffer = null;
    }

    dxArgs->iaBuffers = new ID3D10Buffer* [args.buffers.size()];
    ERROR_CODE_COND_F(!dxArgs->iaBuffers, Error::SystemMemoryAllocationFailure);

    for(uSys i = 0; i < args.buffers.size(); ++i)
    {
        dxArgs->iaBuffers[i] = RefCast<DX10VertexBuffer>(args.buffers[i])->d3dBuffer();
    }

    return true;
}

D3D10_PRIMITIVE_TOPOLOGY DX10VertexArray::getDXDrawType(const DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles:      return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case DrawType::ConnectedTriangles:      return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case DrawType::PointConnectedTriangles: return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        default:                                return D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}
#endif
