#include "dx/dx11/DX11Shader.hpp"
// #include <d3d9.h>

#ifdef _WIN32
#include <d3dcompiler.h>
#include <IFile.hpp>
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"

void DX11VertexShader::bind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->VSSetShader(_shader, NULL, 0); }

void DX11HullShader::bind(DX11RenderingContext & context) noexcept
{ context.d3d11DeviceContext()->HSSetShader(_shader, NULL, 0); }

void DX11DomainShader::bind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->DSSetShader(_shader, NULL, 0); }

void DX11GeometryShader::bind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->GSSetShader(_shader, NULL, 0); }

void DX11PixelShader::bind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->PSSetShader(_shader, NULL, 0); }


void DX11VertexShader::unbind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->VSSetShader(NULL, NULL, 0); }

void DX11HullShader::unbind(DX11RenderingContext & context) noexcept
{ context.d3d11DeviceContext()->HSSetShader(_shader, NULL, 0); }

void DX11DomainShader::unbind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->DSSetShader(_shader, NULL, 0); }

void DX11GeometryShader::unbind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->GSSetShader(NULL, NULL, 0); }

void DX11PixelShader::unbind(DX11RenderingContext& context) noexcept
{ context.d3d11DeviceContext()->PSSetShader(NULL, NULL, 0); }

DX11ShaderBuilder::DX11ShaderBuilder(DX11GraphicsInterface& gi) noexcept
	: _gi(gi), _resIndex(IShaderBuilder::rsTransformer->transform(gi.renderingMode()))
{ }

DX11Shader* DX11ShaderBuilder::build(const ShaderArgs& args, Error* const error) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D11ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    DX11Shader* shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = new(::std::nothrow) DX11VertexShader(objects.vertex, dxArgs.dataBlob);
            break;
        case EShader::Stage::Hull:
            shader = new(::std::nothrow) DX11HullShader(objects.hull);
            break;
        case EShader::Stage::Domain:
            shader = new(::std::nothrow) DX11DomainShader(objects.domain);
            break;
        case EShader::Stage::Geometry:
            shader = new(::std::nothrow) DX11GeometryShader(objects.geometry);
            break;
        case EShader::Stage::Pixel:
            shader = new(::std::nothrow) DX11PixelShader(objects.pixel);
            break;
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

DX11Shader* DX11ShaderBuilder::build(const ShaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D11ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    DX11Shader* shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = allocator.allocateT<DX11VertexShader>(objects.vertex, dxArgs.dataBlob);
            break;
        case EShader::Stage::Hull:
            shader = allocator.allocateT<DX11HullShader>(objects.hull);
            break;
        case EShader::Stage::Domain:
            shader = allocator.allocateT<DX11DomainShader>(objects.domain);
            break;
        case EShader::Stage::Geometry:
            shader = allocator.allocateT<DX11GeometryShader>(objects.geometry);
            break;
        case EShader::Stage::Pixel:
            shader = allocator.allocateT<DX11PixelShader>(objects.pixel);
            break;
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

CPPRef<IShader> DX11ShaderBuilder::buildCPPRef(const ShaderArgs& args, Error* const error) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D11ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    CPPRef<DX11Shader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = CPPRef<DX11Shader>(new(::std::nothrow) DX11VertexShader(objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Hull:
            shader = CPPRef<DX11Shader>(new(::std::nothrow) DX11HullShader(objects.hull));
            break;
        case EShader::Stage::Domain:
            shader = CPPRef<DX11Shader>(new(::std::nothrow) DX11DomainShader(objects.domain));
            break;
        case EShader::Stage::Geometry:
            shader = CPPRef<DX11Shader>(new(::std::nothrow) DX11GeometryShader(objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = CPPRef<DX11Shader>(new(::std::nothrow) DX11PixelShader(objects.pixel));
            break;
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

NullableRef<IShader> DX11ShaderBuilder::buildTauRef(const ShaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D11ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableRef<IShader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RCPCast<IShader>(NullableRef<DX11VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Hull:
            shader = RCPCast<IShader>(NullableRef<DX11HullShader>(allocator, objects.hull));
            break;
        case EShader::Stage::Domain:
            shader = RCPCast<IShader>(NullableRef<DX11DomainShader>(allocator, objects.domain));
            break;
        case EShader::Stage::Geometry:
            shader = RCPCast<IShader>(NullableRef<DX11GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RCPCast<IShader>(NullableRef<DX11PixelShader>(allocator, objects.pixel));
            break;
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

NullableStrongRef<IShader> DX11ShaderBuilder::buildTauSRef(const ShaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXShaderArgs dxArgs {};
	if(!processArgs(args, &dxArgs, error))
    { return null; }

    const D3D11ShaderObjects objects = createD3DShader(args, dxArgs, error);
	if(!objects.vertex)
    { return null; }
	
    NullableStrongRef<IShader> shader;

    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            shader = RCPCast<IShader>(NullableStrongRef<DX11VertexShader>(allocator, objects.vertex, dxArgs.dataBlob));
            break;
        case EShader::Stage::Hull:
            shader = RCPCast<IShader>(NullableStrongRef<DX11HullShader>(allocator, objects.hull));
            break;
        case EShader::Stage::Domain:
            shader = RCPCast<IShader>(NullableStrongRef<DX11DomainShader>(allocator, objects.domain));
            break;
        case EShader::Stage::Geometry:
            shader = RCPCast<IShader>(NullableStrongRef<DX11GeometryShader>(allocator, objects.geometry));
            break;
        case EShader::Stage::Pixel:
            shader = RCPCast<IShader>(NullableStrongRef<DX11PixelShader>(allocator, objects.pixel));
            break;
        default:
            dxArgs.dataBlob->Release();
            ERROR_CODE_N(Error::InvalidShaderStage);
    }

    dxArgs.dataBlob->Release();

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);
	
    ERROR_CODE_V(Error::NoError, shader);
}

bool DX11ShaderBuilder::processArgs(const ShaderArgs& args, DXShaderArgs* dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.vfsMount, Error::InvalidFile);
    ERROR_CODE_COND_F(!args.path, Error::InvalidFile);
    ERROR_CODE_COND_F(!args.fileName, Error::InvalidFile);

    const ResourceSelector shaderSelector = ResourceSelectorLoader::load(args.vfsMount, args.path, args.fileName, IShaderBuilder::rsTransformer);

    ERROR_CODE_COND_F(shaderSelector.count() == 0, Error::InvalidFile);
    auto& selected = shaderSelector.select(_resIndex);
    ERROR_CODE_COND_F(!selected.loader() || selected.path().length() == 0 || selected.name().length() == 0, Error::InvalidFile);
    const CPPRef<IFile> shaderFile = selected.loadFile(FileProps::Read);

    const i64 fileSize = shaderFile->size();
	
    const HRESULT h = D3DCreateBlob(fileSize, &dxArgs->dataBlob);
    ERROR_CODE_COND_F(FAILED(h), Error::DriverMemoryAllocationFailure);

    void* const dataBuffer = dxArgs->dataBlob->GetBufferPointer();
    (void) shaderFile->readBytes(reinterpret_cast<u8*>(dataBuffer), fileSize);

    return true;
}

DX11ShaderBuilder::D3D11ShaderObjects DX11ShaderBuilder::createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, Error* const error) const noexcept
{
    D3D11ShaderObjects objects = { null };

    HRESULT h;
	
    switch(args.stage)
    {
        case EShader::Stage::Vertex:
            h = _gi.d3d11Device()->CreateVertexShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), NULL, &objects.vertex);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::Hull:
            h = _gi.d3d11Device()->CreateHullShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), NULL, &objects.hull);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::Domain:
            h = _gi.d3d11Device()->CreateDomainShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), NULL, &objects.domain);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::Geometry:
            h = _gi.d3d11Device()->CreateGeometryShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), NULL, &objects.geometry);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        case EShader::Stage::Pixel:
            h = _gi.d3d11Device()->CreatePixelShader(dxArgs.dataBlob->GetBufferPointer(), dxArgs.dataBlob->GetBufferSize(), NULL, &objects.pixel);
            ERROR_CODE_COND_V(FAILED(h), Error::DriverMemoryAllocationFailure, objects);
            break;
        default:
            break;
    }

    return objects;
}
#endif
