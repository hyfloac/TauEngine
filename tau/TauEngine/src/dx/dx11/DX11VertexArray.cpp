#include "dx/dx11/DX11VertexArray.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11Buffer.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "TauConfig.hpp"

#if TAU_RTTI_CHECK
#include "TauEngine.hpp"
  #define CTX() \
      if(!RTT_CHECK(context, DX11RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#endif

void DX11VertexArray::bind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.setBuffers(_iaBufferCount, _iaBuffers);
    if(_indexBuffer)
    {
        ctx.d3d11DeviceContext()->IASetIndexBuffer(_indexBuffer->d3dBuffer(), _indexBuffer->dxIndexSize(), 0);
    }
    ctx.d3d11DeviceContext()->IASetPrimitiveTopology(_drawTypeCache);
}

void DX11VertexArray::unbind(IRenderingContext& context) noexcept
{
    CTX();
    ID3D11Buffer* buffers[1];
    ctx.setBuffers(0, buffers);
    ctx.d3d11DeviceContext()->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
}

void DX11VertexArray::draw(IRenderingContext& context, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    CTX();

    if(_indexBuffer)
    {
        ctx.d3d11DeviceContext()->DrawIndexed(drawCount, drawOffset, 0);
    }
    else
    {
        ctx.d3d11DeviceContext()->Draw(drawCount, drawOffset);
    }
}

void DX11VertexArray::drawInstanced(IRenderingContext& context, const uSys instanceCount, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    CTX();

    if(_indexBuffer)
    {
        ctx.d3d11DeviceContext()->DrawIndexedInstanced(drawCount, instanceCount, drawOffset, 0, 0);
    }
    else
    {
        ctx.d3d11DeviceContext()->DrawInstanced(drawCount, instanceCount, drawOffset, 0);
    }
}

DX11VertexArray* DX11VertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11VertexArray* const va = new(::std::nothrow) DX11VertexArray(args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, va);
}

DX11VertexArray* DX11VertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11VertexArray* const va = allocator.allocateT<DX11VertexArray>(args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, va);
}

CPPRef<IVertexArray> DX11VertexArrayBuilder::buildCPPRef(const VertexArrayArgs& args, Error* const error) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX11VertexArray> va = CPPRef<DX11VertexArray>(new(::std::nothrow) DX11VertexArray(args, dxArgs));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, va);
}

NullableRef<IVertexArray> DX11VertexArrayBuilder::buildTauRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX11VertexArray> va(allocator, args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

NullableStrongRef<IVertexArray> DX11VertexArrayBuilder::buildTauSRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX11VertexArray> va(allocator, args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

bool DX11VertexArrayBuilder::processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_F(args.drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < args.buffers.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.buffers[i], Error::BuffersNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(args.buffers[i].get(), DX11VertexBuffer), Error::InternalError);
    }

    if(args.indexBuffer)
    {
        ERROR_CODE_COND_F(!RTT_CHECK(args.indexBuffer.get(), DX11IndexBuffer), Error::InternalError);
        dxArgs->indexBuffer = RefCast<DX11IndexBuffer>(args.indexBuffer);
    }
    else
    {
        dxArgs->indexBuffer = null;
    }

    dxArgs->iaBuffers = new ID3D11Buffer* [args.buffers.size()];
    ERROR_CODE_COND_F(!dxArgs->iaBuffers, Error::SystemMemoryAllocationFailure);

    for(uSys i = 0; i < args.buffers.size(); ++i)
    {
        dxArgs->iaBuffers[i] = RefCast<DX11VertexBuffer>(args.buffers[i])->d3dBuffer();
    }

    return true;
}

D3D11_PRIMITIVE_TOPOLOGY DX11VertexArray::getDXDrawType(const DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles:      return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case DrawType::ConnectedTriangles:      return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case DrawType::PointConnectedTriangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        default:                                return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}
#endif
