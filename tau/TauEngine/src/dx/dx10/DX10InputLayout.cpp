#include "dx/dx10/DX10InputLayout.hpp"

#ifdef _WIN32
#include <Safeties.hpp>
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"

ID3D10InputLayout* DX10InputLayout::inputLayoutCache(DX10RenderingContext& context, ID3D10Blob* dataBlob) noexcept
{
    if(!_inputLayoutCache)
    {
        const HRESULT h = context.d3d10Device()->CreateInputLayout(_inputElements, _inputElements.count(), 
                                                                   dataBlob->GetBufferPointer(), dataBlob->GetBufferSize(), &_inputLayoutCache);

        if(FAILED(h))
        {
            _inputLayoutCache = null;
        }
    }

    return _inputLayoutCache;
}

static void handleInsertion(uSys& insertIndex, const InputLayoutDescriptor& desc, DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements, DynArray<uSys>& semanticIndices) noexcept;

DX10InputLayout* DX10InputLayoutBuilder::build(Error* error) noexcept
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

    for(uSys i = 0; i < _descriptors.count(); ++i)
    {
        ERROR_CODE_COND_N(_descriptors[i].type() == ShaderDataType::Unknown, Error::DescriptorsNotFinalized);
        ERROR_CODE_COND_N(_descriptors[i].semantic() == ShaderSemantic::Unknown, Error::DescriptorsNotFinalized);
    }

    uSys bufferCount = 0;
    for(uSys i = 0; i < _descriptors.count(); ++i)
    {
        bufferCount += DX10VertexArray::computeNumElements(_descriptors[i].type());
    }

    DynArray<D3D10_INPUT_ELEMENT_DESC> inputElements(bufferCount);
    ERROR_CODE_COND_N(!inputElements, Error::MemoryAllocationFailure);
    DynArray<uSys> semanticIndices(static_cast<uSys>(ShaderSemantic::TextureCoord));
    ERROR_CODE_COND_N(!semanticIndices, Error::MemoryAllocationFailure);

    uSys insertIndex = 0;
    for(uSys i = 0; i < _descriptors.count(); ++i)
    {
        handleInsertion(insertIndex, _descriptors[i], inputElements, semanticIndices);
    }

    for(uSys i = 0; i < semanticIndices.count(); ++i)
    {
        const ShaderSemantic::Semantic semantic = static_cast<ShaderSemantic::Semantic>(i);
        ERROR_CODE_COND_N(!ShaderSemantic::hasIndices(semantic) && semanticIndices[i] > 1, Error::MultipleSemanticOfInvalidType);
    }

    DX10InputLayout* inputLayout = new(::std::nothrow) DX10InputLayout(_descriptors, inputElements);
    ERROR_CODE_COND_N(!inputLayout, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

static void handleInsertion(uSys& insertIndex, const InputLayoutDescriptor& desc, DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements, DynArray<uSys>& semanticIndices) noexcept
{
    const ShaderDataType::Type underlyingType = ShaderDataType::underlyingTypeND(desc.type());
    const uSys columns = DX10VertexArray::computeNumElements(desc.type());
    uSys& semanticIndice = semanticIndices[static_cast<uSys>(desc.semantic())];

    for(uSys i = 0; i < columns; ++i, ++insertIndex)
    {
        inputElements[insertIndex].SemanticName = DX10InputLayoutBuilder::getDXSemanticName(desc.semantic());
        inputElements[insertIndex].SemanticIndex = semanticIndice++;
        inputElements[insertIndex].Format = DX10VertexArray::getDXType(underlyingType);
        inputElements[insertIndex].InputSlot = 0;
        inputElements[insertIndex].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
        inputElements[insertIndex].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
        inputElements[insertIndex].InstanceDataStepRate = 0;
    }
}

const char* DX10InputLayoutBuilder::getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept
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
