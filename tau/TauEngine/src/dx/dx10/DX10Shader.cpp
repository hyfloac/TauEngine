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
{ context.d3dDevice()->VSSetShader(NULL); }

void DX10GeometryShader::unbind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->GSSetShader(NULL); }

void DX10PixelShader::unbind(DX10RenderingContext& context) noexcept
{ context.d3dDevice()->PSSetShader(NULL); }

DX10Shader* DX10ShaderBuilder::build(const ShaderArgs& args, Error* const error) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    DX10Shader* shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = new(::std::nothrow) DX10VertexShader(objects.vertex, dxArgs.dataBlob);
            break;
        case EShader::Stage::Geometry:
            shader = new(::std::nothrow) DX10GeometryShader(objects.geometry);
            break;
        case EShader::Stage::Pixel:
            shader = new(::std::nothrow) DX10PixelShader(objects.pixel);
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

DX10Shader* DX10ShaderBuilder::build(const ShaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    DX10Shader* shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = allocator.allocateT<DX10VertexShader>(objects.vertex, dxArgs.dataBlob);
            break;
        case EShader::Stage::Geometry:
            shader = allocator.allocateT<DX10GeometryShader>(objects.geometry);
            break;
        case EShader::Stage::Pixel:
            shader = allocator.allocateT<DX10PixelShader>(objects.pixel);
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

CPPRef<IShader> DX10ShaderBuilder::buildCPPRef(const ShaderArgs& args, Error* const error) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    CPPRef<DX10Shader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = CPPRef<DX10Shader>(new(::std::nothrow) DX10VertexShader(objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = CPPRef<DX10Shader>(new(::std::nothrow) DX10GeometryShader(objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = CPPRef<DX10Shader>(new(::std::nothrow) DX10PixelShader(objects.pixel));
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

NullableRef<IShader> DX10ShaderBuilder::buildTauRef(const ShaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableRef<IShader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RCPCast<IShader>(NullableRef<DX10VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = RCPCast<IShader>(NullableRef<DX10GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RCPCast<IShader>(NullableRef<DX10PixelShader>(allocator, objects.pixel));
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

NullableStrongRef<IShader> DX10ShaderBuilder::buildTauSRef(const ShaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableStrongRef<IShader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RCPCast<IShader>(NullableStrongRef<DX10VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = RCPCast<IShader>(NullableStrongRef<DX10GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RCPCast<IShader>(NullableStrongRef<DX10PixelShader>(allocator, objects.pixel));
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

bool DX10ShaderBuilder::processArgs(const ShaderArgs& args, DXShaderArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.file, Error::InvalidFile);

    const DynString path = args.file->name();

    if(VFS::getFileExt(path, false) == "tausi")
    {
        return processBundle(args, dxArgs, error);
    }
    else
    {
        return processShader(args.file, args.stage, dxArgs, error);
    }
}

bool DX10ShaderBuilder::processBundle(const ShaderArgs& args, DXShaderArgs* dxArgs, Error* error) const noexcept
{
    ShaderBundleParser parser(args.file);

    ShaderBundleParser::Error parseError;
    auto ast = parser.parse(&parseError);

    _visitor->reset();
    _visitor->visit(ast.get());
    const sbp::ShaderInfo& info = _visitor->get(args.stage);

    const CPPRef<IFile> file = VFS::Instance().openFile(info.fileName, FileProps::Read);
    if(!processShader(file, args.stage, dxArgs, error))
    { return false; }

    return true;
}

bool DX10ShaderBuilder::processShader(const CPPRef<IFile>& file, const EShader::Stage stage, DXShaderArgs* dxArgs, Error* error) const noexcept
{
    const i64 fileSize = file->size();

    const HRESULT h = D3DCreateBlob(fileSize, &dxArgs->dataBlob);
    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    void* const dataBuffer = dxArgs->dataBlob->GetBufferPointer();
    (void)file->readBytes(reinterpret_cast<u8*>(dataBuffer), fileSize);

    return true;
}

DX10ShaderBuilder::D3D10ShaderObjects DX10ShaderBuilder::createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, Error* const error) const noexcept
{
    D3D10ShaderObjects objects = { null };

    HRESULT h;
	
    switch(args.stage)
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
