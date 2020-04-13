#pragma once

#include "shader/IShader.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL TAU_NOVTABLE DX10Shader : public IShader
{
    DEFAULT_CONSTRUCT_PO(DX10Shader);
    DEFAULT_DESTRUCT_VI(DX10Shader);
    SHADER_IMPL(DX10Shader);
public:
    virtual void bind(DX10RenderingContext& context) noexcept = 0;
    virtual void unbind(DX10RenderingContext& context) noexcept = 0;
};

class TAU_DLL DX10VertexShader final : public DX10Shader
{
    DELETE_COPY(DX10VertexShader);
private:
    ID3D10VertexShader* _shader;
    ID3D10Blob* _shaderBlob;
public:
    DX10VertexShader(ID3D10VertexShader* shader, ID3D10Blob* shaderBlob) noexcept
        : _shader(shader), _shaderBlob(shaderBlob)
    { shaderBlob->AddRef(); }

    ~DX10VertexShader() noexcept
    {
        _shader->Release();
        _shaderBlob->Release();
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Vertex; }

    [[nodiscard]] const ID3D10VertexShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10VertexShader* shader() noexcept { return _shader; }

    [[nodiscard]] ID3D10Blob* shaderBlob() const noexcept { return _shaderBlob; }

    void bind(DX10RenderingContext& context) noexcept override;
    void unbind(DX10RenderingContext& context) noexcept override;
};

class TAU_DLL DX10GeometryShader final : public DX10Shader
{
    DELETE_COPY(DX10GeometryShader);
private:
    ID3D10GeometryShader* _shader;
public:
    DX10GeometryShader(ID3D10GeometryShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX10GeometryShader() noexcept
    { _shader->Release(); }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Geometry; }

    [[nodiscard]] const ID3D10GeometryShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10GeometryShader* shader() noexcept { return _shader; }

    void bind(DX10RenderingContext& context) noexcept override;
    void unbind(DX10RenderingContext& context) noexcept override;
};

class TAU_DLL DX10PixelShader final : public DX10Shader
{
    DELETE_COPY(DX10PixelShader);
private:
    ID3D10PixelShader* _shader;
public:
    DX10PixelShader(ID3D10PixelShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX10PixelShader() noexcept
    { _shader->Release(); }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Pixel; }

    [[nodiscard]] const ID3D10PixelShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10PixelShader* shader() noexcept { return _shader; }

    void bind(DX10RenderingContext& context) noexcept override;
    void unbind(DX10RenderingContext& context) noexcept override;
};

class TAU_DLL DX10ShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(DX10ShaderBuilder);
    DELETE_COPY(DX10ShaderBuilder);
private:
	struct DXShaderArgs final
	{
        ID3D10Blob* dataBlob;
	};

	union D3D10ShaderObjects final
	{
        ID3D10VertexShader* vertex;
        ID3D10GeometryShader* geometry;
        ID3D10PixelShader* pixel;
	};
private:
    DX10GraphicsInterface& _gi;
    IResourceSelectorTransformer::ResIndex _resIndex;
public:
    DX10ShaderBuilder(DX10GraphicsInterface& gi) noexcept;

    [[nodiscard]] DX10Shader* build(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10Shader* build(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShader> buildCPPRef(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IShader> buildTauRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IShader> buildTauSRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderArgs& args, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] D3D10ShaderObjects createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
