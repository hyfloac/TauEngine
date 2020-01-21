#include "dx/dx10/DX10VertexArray.hpp"

#ifdef _WIN32

#include "dx/dx10/DX10Buffer.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10ShaderProgram.hpp"
#include "dx/dx10/DX10Shader.hpp"

DX10VertexArray::DX10VertexArray(uSys bufferCount, DrawType drawType) noexcept
    : IVertexArray(bufferCount, drawType), _iaBufferCount(0),
      _iaBuffers(null), _iaStrides(null), _iaOffsets(null),
      _elementDescriptors(null)
{ }

DX10VertexArray::~DX10VertexArray() noexcept
{
    delete[] _iaBuffers;
    delete[] _iaStrides;
    delete[] _iaOffsets;
    delete[] _elementDescriptors;
}

void DX10VertexArray::bind(IRenderingContext& context) noexcept
{
}

void DX10VertexArray::unbind(IRenderingContext& context) noexcept
{
}

void DX10VertexArray::internalSetup(IRenderingContext& context) noexcept
{
    /**
     *   DirectX doesn't have the same concept of VAO's as OpenGL,
     * as such we have to design it around Input Layouts. OpenGL's
     * VAO's have the nice benefit of each buffer is bound to its
     * own layout, but in DirectX you have a single layout, and
     * all the included buffers. As such, as best as I can tell,
     * you have to load the same number of buffers as layout 
     * elements.
     *
     *   If you are using multi-type buffers (buffers which contain
     * more than one type of data) then you, as best as I can tell,
     * have to load it multiple times, once for each time it shows
     * up in the layout. You also have to give it a stride and
     * offset, where the stride is how many bytes before the next
     * element can be found, the offset is how many bytes to skip
     * from the beginning of the buffer.
     */

    if(!context.isContextType<DX10RenderingContext>())
    {
        return;
    }

    auto& dxCtx = reinterpret_cast<DX10RenderingContext&>(context);

    ID3D10Blob* shaderBlob = dxCtx.tmpShaderBlob();

    ID3D10InputLayout* inputLayout;
    const HRESULT h = dxCtx.d3d10Device()->CreateInputLayout(_elementDescriptors, _iaBufferCount, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

    if(!FAILED(h))
    {
        dxCtx.tmpInputLayout() = inputLayout;
    }
    else
    {
        dxCtx.tmpInputLayout() = null;
    }
}

void DX10VertexArray::preDraw(IRenderingContext& context) noexcept
{
    if(!context.isContextType<DX10RenderingContext>())
    {
        return;
    }

    auto& dxCtx = reinterpret_cast<DX10RenderingContext&>(context);

    dxCtx.d3d10Device()->IASetVertexBuffers(0, _iaBufferCount, _iaBuffers, _iaStrides, _iaOffsets);
    dxCtx.d3d10Device()->IASetIndexBuffer(_dxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    dxCtx.d3d10Device()->IASetPrimitiveTopology(_drawTypeCache);
}

void DX10VertexArray::postDraw(IRenderingContext& context) noexcept
{
}

void DX10VertexArray::draw(IRenderingContext& context) noexcept
{
    if(!context.isContextType<DX10RenderingContext>())
    {
        return;
    }

    auto& dxCtx = reinterpret_cast<DX10RenderingContext&>(context);

    dxCtx.d3d10Device()->Draw(_drawCount, 0);
}

void DX10VertexArray::drawIndexed(IRenderingContext& context) noexcept
{
    if(!context.isContextType<DX10RenderingContext>())
    {
        return;
    }

    auto& dxCtx = reinterpret_cast<DX10RenderingContext&>(context);

    dxCtx.d3d10Device()->DrawIndexed(_drawCount, 0, 0);
}

void DX10VertexArray::setIndexBuffer(IRenderingContext& context, const Ref<IIndexBuffer>& indexBuffer) noexcept
{
    IVertexArray::setIndexBuffer(context, indexBuffer);
    if(indexBuffer)
    {
        _dxIndexBuffer = RefCast<DX10IndexBuffer>(indexBuffer)->d3dBuffer();
    }
    else
    {
        _dxIndexBuffer = null;
    }
}

void DX10VertexArray::drawType(const DrawType drawType) noexcept
{
    IVertexArray::drawType(drawType);
    _drawTypeCache = getDXDrawType(drawType);
}

void DX10VertexArray::addVertexBuffer(IRenderingContext& context, const Ref<IBuffer>& vertexBuffer) noexcept
{
    IVertexArray::addVertexBuffer(context, vertexBuffer);

    if(_currentIndex == _buffers.count())
    {
        /**
         *   DirectX doesn't have the same concept of VAO's as OpenGL,
         * as such we have to design it around Input Layouts. OpenGL's
         * VAO's have the nice benefit of each buffer is bound to its
         * own layout, but in DirectX you have a single layout, and
         * all the included buffers. As such, as best as I can tell,
         * you have to load the same number of buffers as layout
         * elements.
         *
         *   If you are using multi-type buffers (buffers which contain
         * more than one type of data) then you, as best as I can tell,
         * have to load it multiple times, once for each time it shows
         * up in the layout. You also have to give it a stride and
         * offset, where the stride is how many bytes before the next
         * element can be found, the offset is how many bytes to skip
         * from the beginning of the buffer.
         */

        _iaBufferCount = 0;
        for(uSys i = 0; i < _buffers.size(); ++i)
        {
            auto& buffer = _buffers[i];
            const BufferDescriptor& descriptor = buffer->descriptor();

            for(uSys j = 0; j < descriptor.elements().size(); ++j)
            {
                _iaBufferCount += computeNumElements(descriptor.elements()[i].type());
            }
        }

        _iaBuffers = new(::std::nothrow) ID3D10Buffer* [_iaBufferCount];
        _iaStrides = new(::std::nothrow) UINT[_iaBufferCount];
        _iaOffsets = new(::std::nothrow) UINT[_iaBufferCount];
        _elementDescriptors = new(::std::nothrow) D3D10_INPUT_ELEMENT_DESC[_iaBufferCount];

        uSys insertIndex = 0;
        for(uSys i = 0; i < _buffers.size(); ++i)
        {
            auto& buffer = _buffers[i];
            const BufferDescriptor& descriptor = buffer->descriptor();

            Ref<DX10Buffer> dxBuffer = RefCast<DX10Buffer>(buffer);

            for(uSys j = 0; j < descriptor.elements().size(); ++j)
            {
                auto& bed = descriptor.elements()[j];
                /**
                 *   We store the underlying DX buffer multiple times due to how
                 * layouts are handled in DX. The offset will start from 0, and
                 * the stride must include the size of the element. If we are
                 * using a multi-type buffer then the offset will have to account
                 * for every element before it, and the stride will have to
                 * account for every element before, as well as its own size.
                 * Conveniently both of these are calculated in advance, for the
                 * most part.
                 */

                handleInsertion(insertIndex, dxBuffer->d3dBuffer(), bed);

                // _iaBuffers[insertIndex] = dxBuffer->d3dBuffer();
                // _iaStrides[insertIndex] = bed.offset() + bed.size();
                // _iaOffsets[insertIndex] = bed.offset();
                //
                // _elementDescriptors[insertIndex].SemanticName = "<----UNKNOWN---->";
                // _elementDescriptors[insertIndex].SemanticIndex = -1;
                // _elementDescriptors[insertIndex].Format = getDXType(bed.type());
                // _elementDescriptors[insertIndex].InputSlot = 0;
                // _elementDescriptors[insertIndex].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
                // _elementDescriptors[insertIndex].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
                // _elementDescriptors[insertIndex].InstanceDataStepRate = 0;
                //
                // ++insertIndex;
            }
        }
    }
}

uSys DX10VertexArray::computeNumElements(ShaderDataType::Type type) noexcept
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

void DX10VertexArray::handleInsertion(uSys& insertIndex, ID3D10Buffer* buffer, const BufferElementDescriptor& bed) const noexcept
{
    const ShaderDataType::Type underlyingType = ShaderDataType::underlyingTypeND(bed.type());
    const u32 underlyingSize = ShaderDataType::size(underlyingType);
    const uSys columns = computeNumElements(bed.type());
    u32 offset = bed.offset();

    for(uSys i = 0; i < columns; ++i, ++insertIndex)
    {
        _iaBuffers[insertIndex] = buffer;
        _iaStrides[insertIndex] = offset + underlyingSize;
        _iaOffsets[insertIndex] = offset; offset += underlyingSize;

        _elementDescriptors[insertIndex].SemanticName = "<----UNKNOWN---->";
        _elementDescriptors[insertIndex].SemanticIndex = -1;
        _elementDescriptors[insertIndex].Format = getDXType(underlyingType);
        _elementDescriptors[insertIndex].InputSlot = 0;
        _elementDescriptors[insertIndex].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
        _elementDescriptors[insertIndex].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
        _elementDescriptors[insertIndex].InstanceDataStepRate = 0;
    }
}

DXGI_FORMAT DX10VertexArray::getDXType(ShaderDataType::Type type) noexcept
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


D3D10_PRIMITIVE_TOPOLOGY DX10VertexArray::getDXDrawType(DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles: return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case DrawType::ConnectedTriangles: return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case DrawType::PointConnectedTriangles: return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        default: return D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}
#endif
