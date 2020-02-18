#include "dx/dx10/DX10Shader.hpp"
#include <d3d9.h>

#ifdef _WIN32
#include <d3dcompiler.h>
#include <IFile.hpp>
#include "dx/dx10/DX10RenderingContext.hpp"

void DX10VertexShader::bind(DX10RenderingContext& context) noexcept
{
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

DX10ShaderBuilder::DX10ShaderBuilder(DX10RenderingContext& ctx) noexcept
	: _ctx(ctx), _resIndex(IShaderBuilder::rsTransformer->transform(ctx.mode()))
{ }

DX10Shader* DX10ShaderBuilder::build(const ShaderArgs& args, Error* error) const noexcept
{
    DXShaderArgs dxArgs;
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

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

DX10Shader* DX10ShaderBuilder::build(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs;
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

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

Ref<IShader> DX10ShaderBuilder::buildCPPRef(const ShaderArgs& args, Error* error) const noexcept
{
    DXShaderArgs dxArgs;
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    Ref<DX10Shader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = Ref<DX10Shader>(new(::std::nothrow) DX10VertexShader(objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = Ref<DX10Shader>(new(::std::nothrow) DX10GeometryShader(objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = Ref<DX10Shader>(new(::std::nothrow) DX10PixelShader(objects.pixel));
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

NullableReferenceCountingPointer<IShader> DX10ShaderBuilder::buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs;
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableReferenceCountingPointer<IShader> shader(null);

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RCPCast<IShader>(NullableReferenceCountingPointer<DX10VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = RCPCast<IShader>(NullableReferenceCountingPointer<DX10GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RCPCast<IShader>(NullableReferenceCountingPointer<DX10PixelShader>(allocator, objects.pixel));
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

NullableStrongReferenceCountingPointer<IShader> DX10ShaderBuilder::buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs;
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D10ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableStrongReferenceCountingPointer<IShader> shader(null);

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RCPCast<IShader>(NullableStrongReferenceCountingPointer<DX10VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Geometry:
            shader = RCPCast<IShader>(NullableStrongReferenceCountingPointer<DX10GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RCPCast<IShader>(NullableStrongReferenceCountingPointer<DX10PixelShader>(allocator, objects.pixel));
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

bool DX10ShaderBuilder::processArgs(const ShaderArgs& args, DXShaderArgs* dxArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.vfsMount, Error::InvalidFile);
    ERROR_CODE_COND_F(!args.path, Error::InvalidFile);
    ERROR_CODE_COND_F(!args.fileName, Error::InvalidFile);

    const ResourceSelector shaderSelector = ResourceSelectorLoader::load(args.vfsMount, args.path, args.fileName, IShaderBuilder::rsTransformer);
	
    const Ref<IFile> shaderFile = shaderSelector.select(_resIndex).loadFile(FileProps::Read);
    const i64 fileSize = shaderFile->size();
	
    const HRESULT h = D3DCreateBlob(fileSize, &dxArgs->dataBlob);
    ERROR_CODE_COND_F(FAILED(h), Error::ShaderObjectCreationFailure);

    void* const dataBuffer = dxArgs->dataBlob->GetBufferPointer();
    (void) shaderFile->readBytes(reinterpret_cast<u8*>(dataBuffer), fileSize);

    return true;
}

DX10ShaderBuilder::D3D10ShaderObjects DX10ShaderBuilder::createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, Error* error) const noexcept
{
    D3D10ShaderObjects objects = { null };

    HRESULT h;
	
    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            h = _ctx.d3d10Device()->CreateVertexShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), &objects.vertex);
            ERROR_CODE_COND_V(FAILED(h), Error::ShaderObjectCreationFailure, objects);
            break;
        case EShader::Stage::Geometry:
            h = _ctx.d3d10Device()->CreateGeometryShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), &objects.geometry);
            ERROR_CODE_COND_V(FAILED(h), Error::ShaderObjectCreationFailure, objects);
            break;
        case EShader::Stage::Pixel:
            h = _ctx.d3d10Device()->CreatePixelShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), &objects.pixel);
            ERROR_CODE_COND_V(FAILED(h), Error::ShaderObjectCreationFailure, objects);
            break;
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        default:
            break;
    }

    return objects;
}
#endif
