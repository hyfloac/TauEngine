#include "dx/dx10/DX10InputLayout.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Shader.hpp"
#include "TauConfig.hpp"
#include <array>
#include <d3dcompiler.h>

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

void DX10InputLayout::bind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.d3dDevice()->IASetInputLayout(_inputLayout);
    ctx.setBufferData(_iaStrides, _iaOffsets);
}

void DX10InputLayout::unbind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.d3dDevice()->IASetInputLayout(null);
    ctx.setBufferData(null, null);
}

DX10InputLayout* DX10InputLayoutBuilder::build(const InputLayoutArgs& args, Error* error) noexcept
{
    DXInputLayoutArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10InputLayout* const inputLayout = new(::std::nothrow) DX10InputLayout(dxArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

    ERROR_CODE_V(Error::NoError, inputLayout);
}

DX10InputLayout* DX10InputLayoutBuilder::build(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DXInputLayoutArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10InputLayout* const inputLayout = allocator.allocateT<DX10InputLayout>(dxArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

    ERROR_CODE_V(Error::NoError, inputLayout);
}

CPPRef<IInputLayout> DX10InputLayoutBuilder::buildCPPRef(const InputLayoutArgs& args, Error* error) noexcept
{
    DXInputLayoutArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10InputLayout> inputLayout = CPPRef<DX10InputLayout>(new(::std::nothrow) DX10InputLayout(dxArgs));
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

    ERROR_CODE_V(Error::NoError, inputLayout);
}

NullableRef<IInputLayout> DX10InputLayoutBuilder::buildTauRef(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DXInputLayoutArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10InputLayout> inputLayout(allocator, dxArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

    ERROR_CODE_V(Error::NoError, inputLayout);
}

NullableStrongRef<IInputLayout> DX10InputLayoutBuilder::buildTauSRef(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DXInputLayoutArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10InputLayout> inputLayout(allocator, dxArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    // Prevent the arrays from being deleted at destruction.
    dxArgs.iaStrides = null;
    dxArgs.iaOffsets = null;
    dxArgs.inputLayout = null;

    ERROR_CODE_V(Error::NoError, inputLayout);
}

using SemanticSet = ::std::array<uSys, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>;

static void handleInsertion(uSys& insertIndex, u32 bufferIndex, bool instanced, const BufferElementDescriptor& bed, DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements, SemanticSet& semanticIndices) noexcept;

static ID3D10Blob* generateILShader(const RefDynArray<BufferDescriptor>& descriptors) noexcept;

bool DX10InputLayoutBuilder::processArgs(const InputLayoutArgs& args, DXInputLayoutArgs* dxArgs, Error* error) const noexcept
{
    uSys bufferDescriptorCount = 0;

    for(uSys i = 0; i < args.descriptors.size(); ++i)
    {
        const auto& elements = args.descriptors[i].elements();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            bufferDescriptorCount += computeNumElements(elements[j].type());
        }
    }

    DynArray<D3D10_INPUT_ELEMENT_DESC> inputElements(bufferDescriptorCount);
    ERROR_CODE_COND_F(!inputElements, Error::SystemMemoryAllocationFailure);

    SemanticSet semanticIndices{ };
    ::std::memset(semanticIndices.data(), 0, semanticIndices.size() * sizeof(uSys));

    dxArgs->iaStrides = new UINT[args.descriptors.size()];
    dxArgs->iaOffsets = new UINT[args.descriptors.size()];

    ERROR_CODE_COND_F(!dxArgs->iaStrides, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_COND_F(!dxArgs->iaOffsets, Error::SystemMemoryAllocationFailure);
    ::std::memset(dxArgs->iaOffsets, 0, args.descriptors.size() * sizeof(UINT));

    uSys insertIndex = 0;
    for(uSys i = 0; i < args.descriptors.size(); ++i)
    {
        auto& elements = args.descriptors[i].elements();

        dxArgs->iaStrides[i] = args.descriptors[i].stride();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            handleInsertion(insertIndex, i, args.descriptors[i].instanced(), elements[j], inputElements, semanticIndices);
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
        ERROR_CODE_COND_F(args.shader->shaderStage() != EShader::Stage::Vertex, Error::ShaderMustBeVertexShader);
        ERROR_CODE_COND_F(!RTT_CHECK(args.shader, DX10Shader), Error::InternalError);
        shaderBlob = reinterpret_cast<DX10VertexShader*>(args.shader)->shaderBlob();
        shouldRelease = false;
    }
    else
    {
        shaderBlob = generateILShader(args.descriptors);
        ERROR_CODE_COND_F(!shaderBlob, Error::ShaderNotSet);
        shouldRelease = true;
    }

    const HRESULT h = _gi.d3d10Device()->CreateInputLayout(inputElements, inputElements.count(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &dxArgs->inputLayout);
    if(shouldRelease)
    { shaderBlob->Release(); }

    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    return true;
}

static void handleInsertion(uSys& insertIndex, const u32 bufferIndex, const bool instanced, const BufferElementDescriptor& bed, DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements, SemanticSet& semanticIndices) noexcept
{
    const ShaderDataType::Type underlyingType = ShaderDataType::underlyingTypeND(bed.type());
    const uSys columns = DX10InputLayoutBuilder::computeNumElements(bed.type());
    uSys& semanticIndice = semanticIndices[static_cast<uSys>(bed.semantic())];

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

const char* getHLSLTypeName(const ShaderDataType::Type type) noexcept;

void genVarName(char buf[14], u32 i) noexcept;

ID3D10Blob* generateILShader(const RefDynArray<BufferDescriptor>& descriptors) noexcept
{
    StringBuilder sb(256);

    sb.append("struct VS_Input {");

    for(uSys i = 0, varIndex = 0; i < descriptors.size(); ++i)
    {
        auto& elements = descriptors[i].elements();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            auto& element = elements[j];
            char buf[14];
            genVarName(buf, varIndex++);
            sb.append(getHLSLTypeName(element.type())).append(buf);
        }
    }

    sb.append("}; struct VS_Output { }; VS_Output main(VS_Input input) { VS_Output output; return output; }");

    D3D_SHADER_MACRO shaderMacros = { NULL, NULL };
    ID3D10Blob* ret;
    ID3D10Blob* errors;
    const HRESULT res = D3DCompile(sb.c_str(), sb.size(), NULL, &shaderMacros, NULL, "main", "vs_4_0", D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ret, &errors);

    if(FAILED(res) || !ret || errors)
    { return null; }

    return ret;
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

const char* DX10InputLayoutBuilder::getDXSemanticName(const ShaderSemantic::Semantic semantic) noexcept
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

const char* getHLSLTypeName(const ShaderDataType::Type type) noexcept
{
    switch(type)
    {
        case ShaderDataType::Bool:        return "bool1";
        case ShaderDataType::Vector2Bool: return "bool2";
        case ShaderDataType::Vector3Bool: return "bool3";
        case ShaderDataType::Vector4Bool: return "bool4";

        case ShaderDataType::Int:        return "int1";
        case ShaderDataType::Vector2Int: return "int2";
        case ShaderDataType::Vector3Int: return "int3";
        case ShaderDataType::Vector4Int: return "int4";

        case ShaderDataType::UInt:        return "uint1";
        case ShaderDataType::Vector2UInt: return "uint2";
        case ShaderDataType::Vector3UInt: return "uint3";
        case ShaderDataType::Vector4UInt: return "uint4";

        case ShaderDataType::Float:          return "float1";
        case ShaderDataType::Vector2Float:   return "float2";
        case ShaderDataType::Vector3Float:   return "float3";
        case ShaderDataType::Vector4Float:   return "float4";
        case ShaderDataType::Matrix2x2Float: return "float2x2";
        case ShaderDataType::Matrix2x3Float: return "float2x3";
        case ShaderDataType::Matrix2x4Float: return "float2x4";
        case ShaderDataType::Matrix3x2Float: return "float3x2";
        case ShaderDataType::Matrix3x3Float: return "float3x3";
        case ShaderDataType::Matrix3x4Float: return "float3x4";
        case ShaderDataType::Matrix4x2Float: return "float4x2";
        case ShaderDataType::Matrix4x3Float: return "float4x3";
        case ShaderDataType::Matrix4x4Float: return "float4x4";

        case ShaderDataType::Double:          return "double1";
        case ShaderDataType::Vector2Double:   return "double2";
        case ShaderDataType::Vector3Double:   return "double3";
        case ShaderDataType::Vector4Double:   return "double4";
        case ShaderDataType::Matrix2x2Double: return "double2x2";
        case ShaderDataType::Matrix2x3Double: return "double2x3";
        case ShaderDataType::Matrix2x4Double: return "double2x4";
        case ShaderDataType::Matrix3x2Double: return "double3x2";
        case ShaderDataType::Matrix3x3Double: return "double3x3";
        case ShaderDataType::Matrix3x4Double: return "double3x4";
        case ShaderDataType::Matrix4x2Double: return "double4x2";
        case ShaderDataType::Matrix4x3Double: return "double4x3";
        case ShaderDataType::Matrix4x4Double: return "double4x4";
        default: break;
    }
    return "";
}

/**
 *   Generates a name very quickly, basically itoa, but the number
 * will be backwards. Doesn't really matter, just need a unique name.
 */
void genVarName(char buf[14], u32 i) noexcept
{
    buf[0] = ' ';
    buf[1] = 'v';
    uSys index = 2;
    while(i != 0)
    {
        const u32 v = i % 10;
        i /= 10;
        switch(v)
        {
            case 0: buf[index++] = '0';
            case 1: buf[index++] = '1';
            case 2: buf[index++] = '2';
            case 3: buf[index++] = '3';
            case 4: buf[index++] = '4';
            case 5: buf[index++] = '5';
            case 6: buf[index++] = '6';
            case 7: buf[index++] = '7';
            case 8: buf[index++] = '8';
            case 9: buf[index++] = '9';
            default: break;
        }
    }
    buf[index++] = ';';
    buf[index] = '\0';
}
#endif
