#include "dx/dx10/DX10InputLayout.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Shader.hpp"
#include "TauConfig.hpp"
#include "dx/DXShaderStubGenerator.hpp"
#include <array>
#include <d3dcompiler.h>

NullableRef<IInputLayout> DX10InputLayoutBuilder::buildTauRef(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXInputLayoutArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10InputLayout> inputLayout(allocator, dxArgs);
#if TAU_NULL_CHECK
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);
#endif

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaStrides = nullptr;
    dxArgs.inputLayout = nullptr;

    ERROR_CODE_V(Error::NoError, inputLayout);
}

using SemanticSet = ::std::array<UINT, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>;

static void handleInsertion(uSys& insertIndex, u32 bufferIndex, bool instanced, const BufferElementDescriptor& bed, DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements, SemanticSet& semanticIndices) noexcept;

bool DX10InputLayoutBuilder::processArgs(const InputLayoutArgs& args, DXInputLayoutArgs* dxArgs, Error* error) const noexcept
{
    uSys bufferDescriptorCount = 0;

    for(uSys i = 0; i < args.descriptorCount; ++i)
    {
        const auto& elements = args.descriptors[i].elements();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            bufferDescriptorCount += computeNumElements(elements[j].type());
        }
    }

    DynArray<D3D10_INPUT_ELEMENT_DESC> inputElements(bufferDescriptorCount);
#if TAU_NULL_CHECK
    ERROR_CODE_COND_F(!inputElements, Error::SystemMemoryAllocationFailure);
#endif

    SemanticSet semanticIndices{ };
    ::std::memset(semanticIndices.data(), 0, semanticIndices.size() * sizeof(unsigned));

    dxArgs->iaStrides = new(::std::nothrow) UINT[args.descriptorCount * 2];
    dxArgs->iaOffsets = dxArgs->iaStrides + args.descriptorCount;
    
#if TAU_NULL_CHECK
    ERROR_CODE_COND_F(!dxArgs->iaStrides, Error::SystemMemoryAllocationFailure);
#endif
    ::std::memset(dxArgs->iaOffsets, 0, args.descriptorCount * 2 * sizeof(UINT));

    uSys insertIndex = 0;
    for(uSys i = 0; i < args.descriptorCount; ++i)
    {
        const auto& elements = args.descriptors[i].elements();

        dxArgs->iaStrides[i] = args.descriptors[i].stride();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            handleInsertion(insertIndex, static_cast<UINT>(i), args.descriptors[i].instanced(), elements[j], inputElements, semanticIndices);
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

    ID3D10Blob* shaderBlob;
    bool shouldRelease;
    
    if(args.shader)
    {
#if TAU_GENERAL_SAFETY_CHECK
        ERROR_CODE_COND_F(args.shader->shaderStage() != EShader::Stage::Vertex, Error::ShaderMustBeVertexShader);
#endif
#if TAU_RTTI_CHECK
        ERROR_CODE_COND_F(!RTTD_CHECK(args.shader, DX10Shader, IShader), Error::InternalError);
#endif
        shaderBlob = static_cast<DX10VertexShader*>(args.shader)->shaderBlob();
        shouldRelease = false;
    }
    else
    {
        shaderBlob = DXShaderStubGenerator::genShader(args.descriptorCount, args.descriptors, "vs_4_0");
#if TAU_GENERAL_SAFETY_CHECK
        ERROR_CODE_COND_F(!shaderBlob, Error::ShaderNotSet);
#endif
        shouldRelease = true;
    }

    const HRESULT h = _gi.d3d10Device()->CreateInputLayout(inputElements, static_cast<UINT>(inputElements.count()), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &dxArgs->inputLayout);
    if(shouldRelease)
    { shaderBlob->Release(); }

    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    return true;
}

static void handleInsertion(uSys& insertIndex, const u32 bufferIndex, const bool instanced, const BufferElementDescriptor& bed, DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements, SemanticSet& semanticIndices) noexcept
{
    const ShaderDataType::Type underlyingType = ShaderDataType::underlyingTypeND(bed.type());
    const uSys columns = DX10InputLayoutBuilder::computeNumElements(bed.type());
    UINT& semanticIndice = semanticIndices[static_cast<uSys>(bed.semantic())];

    for(uSys i = 0; i < columns; ++i, ++insertIndex)
    {
        inputElements[insertIndex].SemanticName = DX10InputLayoutBuilder::getDXSemanticName(bed.semantic());
        inputElements[insertIndex].SemanticIndex = semanticIndice++;
        inputElements[insertIndex].Format = DX10InputLayoutBuilder::getDXType(underlyingType);
        inputElements[insertIndex].InputSlot = bufferIndex;
        inputElements[insertIndex].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
        inputElements[insertIndex].InputSlotClass = instanced ? D3D10_INPUT_PER_INSTANCE_DATA : D3D10_INPUT_PER_VERTEX_DATA;
        inputElements[insertIndex].InstanceDataStepRate = 0;
    }
}

uSys DX10InputLayoutBuilder::computeNumElements(const ShaderDataType::Type type) noexcept
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

DXGI_FORMAT DX10InputLayoutBuilder::getDXType(const ShaderDataType::Type type) noexcept
{
    switch(type)
    {
        case ShaderDataType::Unknown:         return DXGI_FORMAT_UNKNOWN;

        case ShaderDataType::Vector2Bool:     return DXGI_FORMAT_R8G8_UINT;
        case ShaderDataType::Vector3Bool:
        case ShaderDataType::Vector4Bool:     return DXGI_FORMAT_R8G8B8A8_UINT;
        case ShaderDataType::Bool:            return DXGI_FORMAT_R8_UINT;

        case ShaderDataType::Vector2Int:      return DXGI_FORMAT_R32G32_SINT;
        case ShaderDataType::Vector3Int:      return DXGI_FORMAT_R32G32B32_SINT;
        case ShaderDataType::Vector4Int:      return DXGI_FORMAT_R32G32B32A32_SINT;
        case ShaderDataType::Int:             return DXGI_FORMAT_R32_SINT;

        case ShaderDataType::Vector2UInt:     return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataType::Vector3UInt:     return DXGI_FORMAT_R32G32B32_UINT;
        case ShaderDataType::Vector4UInt:     return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataType::UInt:            return DXGI_FORMAT_R32_UINT;

        case ShaderDataType::Vector2Float:    return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Vector3Float:    return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Vector4Float:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Matrix2x2Float:  return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Matrix2x3Float:  return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Matrix2x4Float:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Matrix3x2Float:  return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Matrix3x3Float:  return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Matrix3x4Float:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Matrix4x2Float:  return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataType::Matrix4x3Float:  return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataType::Matrix4x4Float:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataType::Float:           return DXGI_FORMAT_R32_FLOAT;

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
        case ShaderDataType::Double:          return DXGI_FORMAT_R32G32_UINT;
        default: break;
    }
    return DXGI_FORMAT_UNKNOWN;
}

const char* DX10InputLayoutBuilder::getDXSemanticName(const ShaderSemantic::Semantic semantic) noexcept
{
    switch(semantic)
    {
        case ShaderSemantic::BiNormal:            return "BiNormal";
        case ShaderSemantic::BlendIndices:        return "BlendIndices";
        case ShaderSemantic::BlendWeight:         return "BlendWeight";
        case ShaderSemantic::Color:               return "Color";
        case ShaderSemantic::Normal:              return "Normal";
        case ShaderSemantic::Position:            return "Position";
        case ShaderSemantic::PositionTransformed: return "PositionT";
        case ShaderSemantic::PointSize:           return "PSize";
        case ShaderSemantic::Tangent:             return "Tangent";
        case ShaderSemantic::TextureCoord:        return "TexCoord";

        case ShaderSemantic::Unknown:
        default: return "Unknown";
    }
}
#endif
