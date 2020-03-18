#include "dx/dx11/DX11VertexArray.hpp"
#include "TauEngine.hpp"

#ifdef _WIN32
#include <array>

#include "dx/dx11/DX11Buffer.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "dx/dx11/DX11ShaderProgram.hpp"
#include "dx/dx11/DX11Shader.hpp"

DX11VertexArray::DX11VertexArray(const u32 drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers, ID3D11InputLayout* const inputLayout, const uSys bufferCount, ID3D11Buffer** const iaBuffers,
                                 UINT* const iaStrides, UINT* const iaOffsets, ID3D11Buffer* const indexBuffer, const DrawType drawType) noexcept
    : IVertexArray(drawCount, buffers), _inputLayout(inputLayout), _iaBufferCount(bufferCount),
      _iaBuffers(iaBuffers), _iaStrides(iaStrides), _iaOffsets(iaOffsets),
      _dxIndexBuffer(indexBuffer), _drawTypeCache(getDXDrawType(drawType))
{
    if(_dxIndexBuffer)
    {
        _dxIndexBuffer->AddRef();
    }
}

DX11VertexArray::~DX11VertexArray() noexcept
{
    delete[] _iaBuffers;
    delete[] _iaStrides;
    delete[] _iaOffsets;
    _inputLayout->Release();
    if(_dxIndexBuffer)
    {
        _dxIndexBuffer->Release();
        _dxIndexBuffer = null;
    }
}

void DX11VertexArray::bind(IRenderingContext& context) noexcept
{
}

void DX11VertexArray::unbind(IRenderingContext& context) noexcept
{
}

// void DX11VertexArray::internalSetup(IRenderingContext& context) noexcept
// {
//     /**
//      *   DirectX doesn't have the same concept of VAO's as OpenGL,
//      * as such we have to design it around Input Layouts. OpenGL's
//      * VAO's have the nice benefit of each buffer is bound to its
//      * own layout, but in DirectX you have a single layout, and
//      * all the included buffers. As such, as best as I can tell,
//      * you have to load the same number of buffers as layout 
//      * elements.
//      *
//      *   If you are using interleaved buffers (buffers which contain
//      * more than one type of data) then you, as best as I can tell,
//      * have to load it multiple times, once for each time it shows
//      * up in the layout. You also have to give it a stride and
//      * offset, where the stride is how many bytes before the next
//      * element can be found, the offset is how many bytes to skip
//      * from the beginning of the buffer.
//      */
//
//     // if(!context.isContextType<DX11RenderingContext>())
//     // {
//     //     return;
//     // }
//     //
//     // auto& dxCtx = reinterpret_cast<DX11RenderingContext&>(context);
//     //
//     // ID3D11Blob* shaderBlob = dxCtx.tmpShaderBlob();
//     //
//     // ID3D11InputLayout* inputLayout;
//     // const HRESULT h = dxCtx.d3d11Device()->CreateInputLayout(_elementDescriptors, _iaBufferCount, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
//     //
//     // if(!FAILED(h))
//     // {
//     //     dxCtx.tmpInputLayout() = inputLayout;
//     // }
//     // else
//     // {
//     //     dxCtx.tmpInputLayout() = null;
//     // }
// }

void DX11VertexArray::preDraw(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        return;
    }

    auto& dxCtx = reinterpret_cast<DX11RenderingContext&>(context);
    dxCtx.d3d11DeviceContext()->IASetInputLayout(_inputLayout);
    dxCtx.d3d11DeviceContext()->IASetVertexBuffers(0, _iaBufferCount, _iaBuffers, _iaStrides, _iaOffsets);
    dxCtx.d3d11DeviceContext()->IASetIndexBuffer(_dxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    dxCtx.d3d11DeviceContext()->IASetPrimitiveTopology(_drawTypeCache);
}

void DX11VertexArray::postDraw(IRenderingContext& context) noexcept
{
}

void DX11VertexArray::draw(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& dxCtx = reinterpret_cast<DX11RenderingContext&>(context);

    if(_dxIndexBuffer)
    {
        dxCtx.d3d11DeviceContext()->DrawIndexed(_drawCount, 0, 0);
    }
    else
    {
        dxCtx.d3d11DeviceContext()->Draw(_drawCount, 0);
    }
}

void DX11VertexArray::drawInstanced(IRenderingContext& context, const uSys instanceCount) noexcept
{
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        return;
    }

    auto& dxCtx = reinterpret_cast<DX11RenderingContext&>(context);

    if(_dxIndexBuffer)
    {
        dxCtx.d3d11DeviceContext()->DrawIndexedInstanced(_drawCount, instanceCount, 0, 0, 0);
    }
    else
    {
        dxCtx.d3d11DeviceContext()->DrawInstanced(_drawCount, instanceCount, 0, 0);
    }
}

uSys DX11VertexArray::computeNumElements(ShaderDataType::Type type) noexcept
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

// void DX11VertexArray::handleInsertion(uSys& insertIndex, ID3D11Buffer* buffer, const BufferElementDescriptor& bed) const noexcept
// {
//     const ShaderDataType::Type underlyingType = ShaderDataType::underlyingTypeND(bed.type());
//     const u32 underlyingSize = ShaderDataType::size(underlyingType);
//     const uSys columns = computeNumElements(bed.type());
//     u32 offset = bed.offset();
//
//     for(uSys i = 0; i < columns; ++i, ++insertIndex)
//     {
//         _iaBuffers[insertIndex] = buffer;
//         _iaStrides[insertIndex] = offset + underlyingSize;
//         _iaOffsets[insertIndex] = offset;
//         offset += underlyingSize;
//
//         _elementDescriptors[insertIndex].SemanticName = "<----UNKNOWN---->";
//         _elementDescriptors[insertIndex].SemanticIndex = -1;
//         _elementDescriptors[insertIndex].Format = getDXType(underlyingType);
//         _elementDescriptors[insertIndex].InputSlot = 0;
//         _elementDescriptors[insertIndex].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
//         _elementDescriptors[insertIndex].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//         _elementDescriptors[insertIndex].InstanceDataStepRate = 0;
//     }
// }

static void handleInsertion(uSys& insertIndex, u32 bufferIndex, const BufferElementDescriptor& bed, DynArray<D3D11_INPUT_ELEMENT_DESC>& inputElements, ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>& semanticIndices) noexcept;

DX11VertexArray* DX11VertexArrayBuilder::build(Error* error) noexcept
{
    // ERROR_CODE_COND_N(!_inputLayout, Error::InputLayoutNotSet);
    ERROR_CODE_COND_N(_drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_N(_drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < _buffers.count(); ++i)
    {
        ERROR_CODE_COND_N(!_buffers[i], Error::BuffersNotSet);
    }

    uSys bufferDescriptorCount = 0;

    for(uSys i = 0; i < _buffers.size(); ++i)
    {
        auto elements = _buffers[i]->descriptor().elements();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            bufferDescriptorCount += DX11VertexArray::computeNumElements(elements[j].type());
        }
    }

    DynArray<D3D11_INPUT_ELEMENT_DESC> inputElements(bufferDescriptorCount);
    ERROR_CODE_COND_N(!inputElements, Error::MemoryAllocationFailure);
    // DynArray<uSys> semanticIndices(static_cast<uSys>(ShaderSemantic::MAX_VALUE + 1));
    ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1> semanticIndices{};
    // ERROR_CODE_COND_N(!semanticIndices, Error::MemoryAllocationFailure);
    ::std::memset(semanticIndices.data(), 0, semanticIndices.size() * sizeof(uSys));
    
    auto* iaBuffers = new(::std::nothrow) ID3D11Buffer* [_buffers.size()];
    auto* iaStrides = new(::std::nothrow) UINT[_buffers.size()];
    auto* iaOffsets = new(::std::nothrow) UINT[_buffers.size()];
    if(!iaBuffers || !iaStrides || !iaOffsets)
    {
        delete[] iaBuffers;
        delete[] iaStrides;
        delete[] iaOffsets;
        ERROR_CODE_N(Error::MemoryAllocationFailure);
    }

    ::std::memset(iaOffsets, 0, _buffers.size() * sizeof(UINT));

    uSys insertIndex = 0;
    for(uSys i = 0; i < _buffers.size(); ++i)
    {
        auto& buffer = _buffers[i];
        auto& elements = buffer->descriptor().elements();

        CPPRef<DX11Buffer> dxBuffer = RefCast<DX11Buffer>(buffer);

        iaBuffers[i] = dxBuffer->d3dBuffer();
        iaStrides[i] = dxBuffer->descriptor().stride();

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
            delete[] iaBuffers;
            delete[] iaStrides;
            delete[] iaOffsets;
            ERROR_CODE_N(Error::MultipleSemanticOfInvalidType);
        }
    }

    ID3D11InputLayout* inputLayout;
    const HRESULT h = _ctx.d3d11Device()->CreateInputLayout(inputElements, inputElements.count(), _shaderBlobCache->GetBufferPointer(), _shaderBlobCache->GetBufferSize(), &inputLayout);

    if(FAILED(h))
    {
        delete[] iaBuffers;
        delete[] iaStrides;
        delete[] iaOffsets;
        ERROR_CODE_N(Error::InternalError);
    }

    DX11VertexArray* va = new(::std::nothrow) DX11VertexArray(_drawCount, _buffers, inputLayout, _buffers.count(), iaBuffers, iaStrides, iaOffsets, _indexBufferCache, _drawType);
    if(!va)
    {
        delete[] iaBuffers;
        delete[] iaStrides;
        delete[] iaOffsets;
        inputLayout->Release();
        ERROR_CODE_N(Error::MemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, va);
}

static void handleInsertion(uSys& insertIndex, u32 bufferIndex, const BufferElementDescriptor& bed, DynArray<D3D11_INPUT_ELEMENT_DESC>& inputElements, ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>& semanticIndices) noexcept
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

void DX11VertexArrayBuilder::shader(const CPPRef<IShader>& shader) noexcept
{
    if(!shader)
    {
        _shaderBlobCache = null;
        IVertexArrayBuilder::shader(shader);
    }
    if(shader->shaderStage() == EShader::Stage::Vertex && RTT_CHECK(shader.get(), DX11Shader))
    {
        _shaderBlobCache = RefCast<DX11VertexShader>(shader)->shaderBlob();
        IVertexArrayBuilder::shader(shader);
    }
}

void DX11VertexArrayBuilder::setVertexBuffer(uSys index, const CPPRef<IBuffer>& vertexBuffer) noexcept
{
    if(RTT_CHECK(vertexBuffer.get(), DX11Buffer))
    {
        IVertexArrayBuilder::setVertexBuffer(index, vertexBuffer);
    }
}

void DX11VertexArrayBuilder::indexBuffer(const CPPRef<IIndexBuffer>& indexBuffer) noexcept
{
    if(!indexBuffer)
    {
        IVertexArrayBuilder::indexBuffer(null);
        _indexBufferCache = null;
    }
    else if(RTT_CHECK(indexBuffer.get(), DX11IndexBuffer))
    {
        IVertexArrayBuilder::indexBuffer(indexBuffer);
        _indexBufferCache = RefCast<DX11IndexBuffer>(indexBuffer)->d3dBuffer();
    }
}

DXGI_FORMAT DX11VertexArray::getDXType(ShaderDataType::Type type) noexcept
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
        case ShaderDataType::Vector4Double:
        case ShaderDataType::Matrix2x2Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix2x3Double: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Matrix2x4Double:
        case ShaderDataType::Matrix3x2Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix3x3Double: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Matrix3x4Double:
        case ShaderDataType::Matrix4x2Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Matrix4x3Double: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Matrix4x4Double: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::Double: return DXGI_FORMAT_R32G32_UINT;
        default: break;
    }
    return DXGI_FORMAT_UNKNOWN;
}

D3D11_PRIMITIVE_TOPOLOGY DX11VertexArray::getDXDrawType(DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case DrawType::ConnectedTriangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case DrawType::PointConnectedTriangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        default: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}

const char* DX11VertexArray::getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept
{
    switch(semantic)
    {
        case ShaderSemantic::BiNormal: return "BINORMAL";
        case ShaderSemantic::BlendIndices: return "BLENDINDICES";
        case ShaderSemantic::BlendWeight: return "BLENDWEIGHT";
        case ShaderSemantic::Color: return "COLOR";
        case ShaderSemantic::Normal: return "NORMAL";
        case ShaderSemantic::Position: return "POSITION";
        case ShaderSemantic::PositionTransformed: return "POSITIONT";
        case ShaderSemantic::PointSize: return "PSIZE";
        case ShaderSemantic::Tangent: return "TANGENT";
        case ShaderSemantic::TextureCoord: return "TEXCOORD";

        case ShaderSemantic::Unknown:
        default: return "UNKNOWN";
    }
}
#endif
