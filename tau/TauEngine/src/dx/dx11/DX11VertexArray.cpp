#include "dx/dx11/DX11VertexArray.hpp"
#include "TauEngine.hpp"

#ifdef _WIN32
#include <array>

#include "dx/dx11/DX11Buffer.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "dx/dx11/DX11ShaderProgram.hpp"
#include "dx/dx11/DX11Shader.hpp"
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "TauConfig.hpp"

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX11RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#endif

void DX11VertexArray::preDraw(IRenderingContext& context) noexcept
{
    CTX();
    ctx.d3d11DeviceContext()->IASetInputLayout(_inputLayout);
    ctx.d3d11DeviceContext()->IASetVertexBuffers(0, _iaBufferCount, _iaBuffers, _iaStrides, _iaOffsets);
    ctx.d3d11DeviceContext()->IASetIndexBuffer(_indexBuffer ? _indexBuffer->d3dBuffer() : NULL, DXGI_FORMAT_R32_UINT, 0);
    ctx.d3d11DeviceContext()->IASetPrimitiveTopology(_drawTypeCache);
}

void DX11VertexArray::draw(IRenderingContext& context, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = this->_drawCount; }

    CTX();

    if(this->_indexBuffer)
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

uSys DX11VertexArray::computeNumElements(const ShaderDataType::Type type) noexcept
{
    switch(type)
    {
        case ShaderDataType::Unknown: return 0;

        case ShaderDataType::Vector2Bool: 
        case ShaderDataType::Vector3Bool:
        case ShaderDataType::Vector4Bool: 
        case ShaderDataType::Bool: 

        case ShaderDataType::Vector2Int: 
        case ShaderDataType::Vector3Int: 
        case ShaderDataType::Vector4Int: 
        case ShaderDataType::Int: 

        case ShaderDataType::Vector2UInt: 
        case ShaderDataType::Vector3UInt: 
        case ShaderDataType::Vector4UInt: 
        case ShaderDataType::UInt:

        case ShaderDataType::Vector2Float:
        case ShaderDataType::Vector3Float:
        case ShaderDataType::Vector4Float: 
        case ShaderDataType::Float: return 1;

        case ShaderDataType::Matrix2x2Float: 
        case ShaderDataType::Matrix2x3Float: 
        case ShaderDataType::Matrix2x4Float: return 2;
        case ShaderDataType::Matrix3x2Float: 
        case ShaderDataType::Matrix3x3Float: 
        case ShaderDataType::Matrix3x4Float: return 3;
        case ShaderDataType::Matrix4x2Float: 
        case ShaderDataType::Matrix4x3Float: 
        case ShaderDataType::Matrix4x4Float: return 4;

        /**
         *   Doubles aren't natively allowed to be passed to HLSL in
         * DirectX, as such they have to passed as 2 UInts, and then
         * reinterpreted as a double; this gets all the more
         * exasperating when dealing with vectors, god forbid matrices.
         */
        case ShaderDataType::Vector2Double: return 1;
        case ShaderDataType::Vector3Double: return 3;
        case ShaderDataType::Vector4Double: return 2;
        case ShaderDataType::Matrix2x2Double: return 2 * 1;
        case ShaderDataType::Matrix2x3Double: return 2 * 3;
        case ShaderDataType::Matrix2x4Double: return 2 * 2;
        case ShaderDataType::Matrix3x2Double: return 3 * 1;
        case ShaderDataType::Matrix3x3Double: return 3 * 3;
        case ShaderDataType::Matrix3x4Double: return 3 * 2;
        case ShaderDataType::Matrix4x2Double: return 4 * 1;
        case ShaderDataType::Matrix4x3Double: return 4 * 3;
        case ShaderDataType::Matrix4x4Double: return 4 * 2;
        case ShaderDataType::Double: return 1;
        default: break;
    }
    return 0;
}

static void handleInsertion(uSys& insertIndex, u32 bufferIndex, const BufferElementDescriptor& bed, DynArray<D3D11_INPUT_ELEMENT_DESC>& inputElements, ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>& semanticIndices) noexcept;

DX11VertexArray* DX11VertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error) noexcept
{
    DXVertexArrayArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11VertexArray* const va = new(::std::nothrow) DX11VertexArray(args, dxArgs);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaBuffers = null;
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

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
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

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
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

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
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

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
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

bool DX11VertexArrayBuilder::processArgs(const VertexArrayArgs& args, DXVertexArrayArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_F(args.drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < args.buffers.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.buffers[i], Error::BuffersNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(args.buffers[i].get(), DX11Buffer), Error::InternalError);
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

    ERROR_CODE_COND_F(!args.shader, Error::ShaderNotSet);
    ERROR_CODE_COND_F(args.shader->shaderStage() != EShader::Stage::Vertex, Error::ShaderMustBeVertexShader);
    ERROR_CODE_COND_F(!RTT_CHECK(args.shader, DX11Shader), Error::InternalError);

    ID3DBlob* const shaderBlob = reinterpret_cast<DX11VertexShader*>(args.shader)->shaderBlob();

    uSys bufferDescriptorCount = 0;

    for(uSys i = 0; i < args.buffers.size(); ++i)
    {
        const auto& elements = args.buffers[i]->descriptor().elements();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            bufferDescriptorCount += DX11VertexArray::computeNumElements(elements[j].type());
        }
    }

    DynArray<D3D11_INPUT_ELEMENT_DESC> inputElements(bufferDescriptorCount);
    ERROR_CODE_COND_F(!inputElements, Error::SystemMemoryAllocationFailure);

    ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1> semanticIndices { };
    ::std::memset(semanticIndices.data(), 0, semanticIndices.size() * sizeof(uSys));

    dxArgs->iaBuffers = new ID3D11Buffer* [args.buffers.size()];
    dxArgs->iaStrides = new UINT[args.buffers.size()];
    dxArgs->iaOffsets = new UINT[args.buffers.size()];

    if(!dxArgs->iaBuffers || !dxArgs->iaStrides || !dxArgs->iaOffsets)
    {
        ERROR_CODE_F(Error::SystemMemoryAllocationFailure);
    }

    ::std::memset(dxArgs->iaOffsets, 0, args.buffers.size() * sizeof(UINT));

    uSys insertIndex = 0;
    for(uSys i = 0; i < args.buffers.size(); ++i)
    {
        auto& buffer = args.buffers[i];
        auto& elements = buffer->descriptor().elements();

        CPPRef<DX11Buffer> dxBuffer = RefCast<DX11Buffer>(buffer);

        dxArgs->iaBuffers[i] = dxBuffer->d3dBuffer();
        dxArgs->iaStrides[i] = dxBuffer->descriptor().stride();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            handleInsertion(insertIndex, i, elements[j], inputElements, semanticIndices);
        }
    }

    for(uSys i = 0; i < semanticIndices.size(); ++i)
    {
        const ShaderSemantic::Semantic semantic = static_cast<ShaderSemantic::Semantic>(i);
        if(!ShaderSemantic::hasIndices(semantic) && semanticIndices[i] > 1)
        {
            ERROR_CODE_F(Error::MultipleSemanticOfInvalidType);
        }
    }

    const HRESULT h = _gi.d3d11Device()->CreateInputLayout(inputElements, inputElements.count(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &dxArgs->inputLayout);
    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    return true;
}

static void handleInsertion(uSys& insertIndex, const u32 bufferIndex, const BufferElementDescriptor& bed, DynArray<D3D11_INPUT_ELEMENT_DESC>& inputElements, ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>& semanticIndices) noexcept
{
    const ShaderDataType::Type underlyingType = ShaderDataType::underlyingTypeND(bed.type());
    const uSys columns = DX11VertexArray::computeNumElements(bed.type());
    uSys& semanticIndice = semanticIndices[static_cast<uSys>(bed.semantic())];

    for(uSys i = 0; i < columns; ++i, ++insertIndex)
    {
        inputElements[insertIndex].SemanticName = DX11VertexArray::getDXSemanticName(bed.semantic());
        inputElements[insertIndex].SemanticIndex = semanticIndice++;
        inputElements[insertIndex].Format = DX11VertexArray::getDXType(underlyingType);
        inputElements[insertIndex].InputSlot = bufferIndex;
        inputElements[insertIndex].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        inputElements[insertIndex].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        inputElements[insertIndex].InstanceDataStepRate = 0;
    }
}

DXGI_FORMAT DX11VertexArray::getDXType(const ShaderDataType::Type type) noexcept
{
    switch(type)
    {
        case ShaderDataType::Unknown: return DXGI_FORMAT_UNKNOWN;

        case ShaderDataType::Vector2Bool: return DXGI_FORMAT_R8G8_UINT;
        case ShaderDataType::Vector3Bool: 
        case ShaderDataType::Vector4Bool: return DXGI_FORMAT_R8G8B8A8_UINT;
        case ShaderDataType::Bool: return DXGI_FORMAT_R8_UINT;

        case ShaderDataType::Vector2Int: return DXGI_FORMAT_R32G32_SINT;
        case ShaderDataType::Vector3Int: return DXGI_FORMAT_R32G32B32_SINT;
        case ShaderDataType::Vector4Int: return DXGI_FORMAT_R32G32B32A32_SINT;
        case ShaderDataType::Int: return DXGI_FORMAT_R32_SINT;

        case ShaderDataType::Vector2UInt: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Vector3UInt: return DXGI_FORMAT_R32G32B32_UINT;
        case ShaderDataType::Vector4UInt: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::UInt: return DXGI_FORMAT_R32_UINT;

        case ShaderDataType::Vector2Float:   return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Vector3Float:   return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Vector4Float:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Matrix2x2Float: return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Matrix2x3Float: return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Matrix2x4Float: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Matrix3x2Float: return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Matrix3x3Float: return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Matrix3x4Float: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Matrix4x2Float: return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Matrix4x3Float: return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Matrix4x4Float: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Float: return DXGI_FORMAT_R32_FLOAT;

        case ShaderDataType::Vector2Double:   return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Vector3Double:   return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Vector4Double:   return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix2x2Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix2x3Double: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Matrix2x4Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix3x2Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix3x3Double: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Matrix3x4Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix4x2Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix4x3Double: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Matrix4x4Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Double: return DXGI_FORMAT_R32G32_UINT;
        default: break;
    }
    return DXGI_FORMAT_UNKNOWN;
}

D3D11_PRIMITIVE_TOPOLOGY DX11VertexArray::getDXDrawType(const DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles:      return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case DrawType::ConnectedTriangles:      return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case DrawType::PointConnectedTriangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        default: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}

const char* DX11VertexArray::getDXSemanticName(const ShaderSemantic::Semantic semantic) noexcept
{
    switch(semantic)
    {
        case ShaderSemantic::BiNormal: return "BiNormal";
        case ShaderSemantic::BlendIndices: return "BlendIndices";
        case ShaderSemantic::BlendWeight: return "BlendWeight";
        case ShaderSemantic::Color: return "Color";
        case ShaderSemantic::Normal: return "Normal";
        case ShaderSemantic::Position: return "Position";
        case ShaderSemantic::PositionTransformed: return "PositionT";
        case ShaderSemantic::PointSize: return "PSize";
        case ShaderSemantic::Tangent: return "Tangent";
        case ShaderSemantic::TextureCoord: return "TexCoord";

        case ShaderSemantic::Unknown:
        default: return "Unknown";
    }
}
#endif
