#include "dx/dx10/DX10Shader.hpp"

#ifdef _WIN32
#include <d3dcompiler.h>
#include <VFS.hpp>
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"

void DX10VertexShader::bind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->VSSetShader(_shader); }

void DX10GeometryShader::bind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->GSSetShader(_shader); }

void DX10PixelShader::bind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->PSSetShader(_shader); }

void DX10VertexShader::unbind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->VSSetShader(nullptr); }

void DX10GeometryShader::unbind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->GSSetShader(nullptr); }

void DX10PixelShader::unbind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->PSSetShader(nullptr); }

NullableRef<IShader> DX10ShaderBuilder::buildTauRef(const ShaderFileArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs { };
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args.stage, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableRef<IShader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RefCast<IShader>(NullableRef<DX10VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = RefCast<IShader>(NullableRef<DX10GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RefCast<IShader>(NullableRef<DX10PixelShader>(allocator, objects.pixel));
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

NullableRef<IShader> DX10ShaderBuilder::buildTauRef(const ShaderSourceArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs { };
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args.stage, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableRef<IShader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RefCast<IShader>(NullableRef<DX10VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = RefCast<IShader>(NullableRef<DX10GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RefCast<IShader>(NullableRef<DX10PixelShader>(allocator, objects.pixel));
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

bool DX10ShaderBuilder::processArgs(const ShaderFileArgs& args, DXShaderArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.file, Error::InvalidFile);

    const WDynString path = args.file->name();

    if(VFS::getFileExt(path, false) == L"tausi")
    {
        return processBundle(args, dxArgs, error);
    }
    else
    {
        return processShader(args.file, dxArgs, error);
    }
}

bool DX10ShaderBuilder::processArgs(const ShaderSourceArgs& args, DXShaderArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.source, Error::InvalidSource);
    
    return processShader(args.source, dxArgs, error);
}

bool DX10ShaderBuilder::processBundle(const ShaderFileArgs& args, DXShaderArgs* dxArgs, Error* error) const noexcept
{
    ShaderBundleParser parser(args.file);

    ShaderBundleParser::Error parseError;
    auto ast = parser.parse(&parseError);

    _visitor->reset();
    _visitor->visit(ast.get());
    const sbp::ShaderInfo& info = _visitor->get(args.stage);

    const CPPRef<IFile> file = VFS::Instance().openFile(info.fileName, FileProps::Read);
    if(!processShader(file, dxArgs, error))
    { return false; }

    return true;
}

bool DX10ShaderBuilder::processShader(const CPPRef<IFile>& file, DXShaderArgs* dxArgs, Error* error) const noexcept
{
    const i64 fileSize = file->size();

    const HRESULT h = D3DCreateBlob(fileSize, &dxArgs->dataBlob);
    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    void* const dataBuffer = dxArgs->dataBlob->GetBufferPointer();
    (void) file->read(dataBuffer, fileSize);

    return true;
}

bool DX10ShaderBuilder::processShader(const DynString& source, DXShaderArgs* dxArgs, Error* error) const noexcept
{
    const uSys fileSize = source.length() * sizeof(char);

    const HRESULT h = D3DCreateBlob(fileSize, &dxArgs->dataBlob);
    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    void* const dataBuffer = dxArgs->dataBlob->GetBufferPointer();
    ::std::memcpy(dataBuffer, source.c_str(), fileSize);

    return true;
}

DX10ShaderBuilder::D3D10ShaderObjects DX10ShaderBuilder::createD3DShader(const EShader::Stage stage, const DXShaderArgs& dxArgs, Error* const error) const noexcept
{
    D3D10ShaderObjects objects = { null };

    HRESULT h;
	
    switch(stage)
    {
        case EShader::Stage::Vertex:
            h = _gi.d3d10Device()->CreateVertexShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), &objects.vertex);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::Geometry:
            h = _gi.d3d10Device()->CreateGeometryShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), &objects.geometry);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::Pixel:
            h = _gi.d3d10Device()->CreatePixelShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), &objects.pixel);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            break;
    }

    return objects;
}
#endif
