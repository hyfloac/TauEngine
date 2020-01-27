#include "dx/dx10/DX10Shader.hpp"
#include <d3d9.h>

#ifdef _WIN32

#include <d3dcompiler.h>
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10VertexArray.hpp"

void DX10VertexShader::bind(DX10RenderingContext& context) noexcept
{
    // context.d3d10Device()->IASetInputLayout(_inputLayout);
    context.d3d10Device()->VSSetShader(_shader);
}

void DX10GeometryShader::bind(DX10RenderingContext& context) noexcept
{
    context.d3d10Device()->GSSetShader(_shader);
}

void DX10PixelShader::bind(DX10RenderingContext& context) noexcept
{
    context.d3d10Device()->PSSetShader(_shader);
}

DX10Shader* DX10ShaderBuilder::build(Error* error) noexcept
{
    ERROR_CODE_COND_N(!_file, Error::InvalidFile);

    ID3D10Blob* dataBlob;
    // HRESULT h = D3DReadFileToBlob(L"", &dataBlob);

    const i64 fileSize = _file->size();
    const HRESULT h = D3DCreateBlob(fileSize, &dataBlob);
    ERROR_CODE_COND_N(FAILED(h), Error::ShaderObjectCreationFailure);

    void* const dataBuffer = dataBlob->GetBufferPointer();
    (void) _file->readBytes(reinterpret_cast<u8*>(dataBuffer), fileSize);

    DX10Shader* ret = null;

    switch(_type)
    {
        case IShader::Type::Vertex:
            ret = buildVertexShader(error, dataBlob);
            break;
        case IShader::Type::TessellationControl:
            break;
        case IShader::Type::TessellationEvaluation:
            break;
        case IShader::Type::Geometry:
            ret = buildGeometryShader(error, dataBlob);
            break;
        case IShader::Type::Pixel:
            ret = buildPixelShader(error, dataBlob);
            break;
        default: break;
    }

    dataBlob->Release();

    ERROR_CODE_COND_N(!ret, Error::InvalidShaderType);

    ERROR_CODE_V(Error::NoError, ret);
}

DX10VertexShader* DX10ShaderBuilder::buildVertexShader(Error* error, ID3D10Blob* dataBlob) const noexcept
{
    ID3D10VertexShader* d3dShader;
    const HRESULT h = _context->d3d10Device()->CreateVertexShader(dataBlob->GetBufferPointer(), dataBlob->GetBufferSize(), &d3dShader);
    ERROR_CODE_COND_N(FAILED(h), Error::ShaderObjectCreationFailure);

    DX10VertexShader* shader = new(::std::nothrow) DX10VertexShader(d3dShader, dataBlob);
    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

DX10GeometryShader* DX10ShaderBuilder::buildGeometryShader(Error* error, ID3D10Blob* dataBlob) const noexcept
{
    ID3D10GeometryShader* d3dShader;
    const HRESULT h = _context->d3d10Device()->CreateGeometryShader(dataBlob->GetBufferPointer(), dataBlob->GetBufferSize(), &d3dShader);
    ERROR_CODE_COND_N(FAILED(h), Error::ShaderObjectCreationFailure);

    DX10GeometryShader* shader = new(::std::nothrow) DX10GeometryShader(d3dShader);
    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

DX10PixelShader* DX10ShaderBuilder::buildPixelShader(Error* error, ID3D10Blob* dataBlob) const noexcept
{
    ID3D10PixelShader* d3dShader;
    const HRESULT h = _context->d3d10Device()->CreatePixelShader(dataBlob->GetBufferPointer(), dataBlob->GetBufferSize(), &d3dShader);
    ERROR_CODE_COND_N(FAILED(h), Error::ShaderObjectCreationFailure);

    DX10PixelShader* shader = new(::std::nothrow) DX10PixelShader(d3dShader);
    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}
#endif
