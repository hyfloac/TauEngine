#include "dx/DXShaderStubGenerator.hpp"

#ifdef _WIN32
#include <String.hpp>
#include <array>

using SemanticSet = ::std::array<UINT, static_cast<uSys>(ShaderSemantic::MAX_VALUE) + 1>;

static const char* getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept;
static const char* getHLSLTypeName(ShaderDataType::Type type) noexcept;
static void genVarName(char buf[14], u32 i) noexcept;
static void genSemanticIndex(char buf[12], u32 i) noexcept;

ID3DBlob* DXShaderStubGenerator::genShader(const uSys bufferCount, const BufferDescriptor* const buffers, const char* const shaderVersion) noexcept
{
    StringBuilder sb(512);

    sb.append("struct VS_Input {");

    SemanticSet semanticIndices { };
    (void) ::std::memset(semanticIndices.data(), 0, semanticIndices.size() * sizeof(UINT));

    u32 varIndex = 0;
    for(uSys i = 0; i < bufferCount; ++i)
    {
        const auto& elements = buffers[i].elements();

        for(uSys j = 0; j < elements.size(); ++j)
        {
            const auto& element = elements[j];
            UINT& semanticIndice = semanticIndices[element.semantic()];

            char buf[14];
            genVarName(buf, varIndex++);
            sb.append(getHLSLTypeName(element.type())).append(buf).append(getDXSemanticName(element.semantic()));
            if(semanticIndice == 0)
            {
                ++semanticIndice;
                sb.append(';');
            }
            else
            {
                genSemanticIndex(buf, semanticIndice++);
                sb.append(buf);
            }
        }
    }

    sb.append("}; struct VS_Output { }; VS_Output main(VS_Input input) { VS_Output output; return output; }");

    D3D_SHADER_MACRO shaderMacros = { null, null };
    ID3DBlob* ret;
    ID3DBlob* errors;
    const HRESULT res = D3DCompile(sb.c_str(), sb.size(), null, &shaderMacros, null, "main", shaderVersion, D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ret, &errors);

    if(FAILED(res) || !ret || errors)
    { return null; }

    errors->Release();

    return ret;
}

static const char* getDXSemanticName(const ShaderSemantic::Semantic semantic) noexcept
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

static const char* getHLSLTypeName(const ShaderDataType::Type type) noexcept
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
static void genVarName(char buf[14], u32 i) noexcept
{
    buf[0] = ' ';
    buf[1] = 'v';
    uSys index = 2;
    while(i != 0)
    {
        const u32 v = i % 16;
        i /= 16;
        switch(v)
        {
            case 0x00: buf[index++] = '0'; break;
            case 0x01: buf[index++] = '1'; break;
            case 0x02: buf[index++] = '2'; break;
            case 0x03: buf[index++] = '3'; break;
            case 0x04: buf[index++] = '4'; break;
            case 0x05: buf[index++] = '5'; break;
            case 0x06: buf[index++] = '6'; break;
            case 0x07: buf[index++] = '7'; break;
            case 0x08: buf[index++] = '8'; break;
            case 0x09: buf[index++] = '9'; break;
            case 0x0A: buf[index++] = 'A'; break;
            case 0x0B: buf[index++] = 'B'; break;
            case 0x0C: buf[index++] = 'C'; break;
            case 0x0D: buf[index++] = 'D'; break;
            case 0x0E: buf[index++] = 'E'; break;
            case 0x0F: buf[index++] = 'F'; break;
            default: break;
        }
    }
    buf[index++] = ':';
    buf[index] = '\0';
}

static void reverseArr(char* buf, const uSys length) noexcept
{
    const uSys halfLength = length / 2;
    for(uSys i = 0; i < halfLength; ++i)
    {
        const char tmp = buf[i];
        buf[i] = buf[length - i - 1];
        buf[length - i - 1] = tmp;
    }
}

static void genSemanticIndex(char buf[12], u32 i) noexcept
{
    uSys index = 0;
    while(i != 0)
    {
        const u32 v = i % 10;
        i /= 10;
        switch(v)
        {
            case 0: buf[index++] = '0'; break;
            case 1: buf[index++] = '1'; break;
            case 2: buf[index++] = '2'; break;
            case 3: buf[index++] = '3'; break;
            case 4: buf[index++] = '4'; break;
            case 5: buf[index++] = '5'; break;
            case 6: buf[index++] = '6'; break;
            case 7: buf[index++] = '7'; break;
            case 8: buf[index++] = '8'; break;
            case 9: buf[index++] = '9'; break;
            default: break;
        }
    }
    buf[index++] = ';';
    buf[index] = '\0';

    reverseArr(buf, index - 2);
}
#endif
