#pragma once

#include "shader/IShader.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL NOVTABLE DX10Shader : public IShader
{
    DEFAULT_CONSTRUCT_PO(DX10Shader);
    DEFAULT_DESTRUCT_VI(DX10Shader);
    SHADER_IMPL(DX10Shader);
public:
    virtual void bind(DX10RenderingContext& context) noexcept = 0;
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
    {
        shaderBlob->AddRef();
    }

    ~DX10VertexShader() noexcept
    {
        _shader->Release();
        _shaderBlob->Release();
    }

    [[nodiscard]] Type shaderType() const noexcept override { return IShader::Type::Vertex; }

    [[nodiscard]] const ID3D10VertexShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10VertexShader* shader() noexcept { return _shader; }

    [[nodiscard]] ID3D10Blob* shaderBlob() noexcept { return _shaderBlob; }

    void bind(DX10RenderingContext& context) noexcept override;
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
    {
        _shader->Release();
    }

    [[nodiscard]] Type shaderType() const noexcept override { return IShader::Type::Geometry; }

    [[nodiscard]] const ID3D10GeometryShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10GeometryShader* shader() noexcept { return _shader; }

    void bind(DX10RenderingContext& context) noexcept override;
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
    {
        _shader->Release();
    }

    [[nodiscard]] Type shaderType() const noexcept override { return IShader::Type::Pixel; }

    [[nodiscard]] const ID3D10PixelShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10PixelShader* shader() noexcept { return _shader; }

    void bind(DX10RenderingContext& context) noexcept override;
};

class TAU_DLL DX10ShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(DX10ShaderBuilder);
    DELETE_COPY(DX10ShaderBuilder);
private:
    DX10RenderingContext* _context;
public:
    DX10ShaderBuilder(DX10RenderingContext* context) noexcept
        : _context(context)
    { }

    [[nodiscard]] DX10Shader* build([[tau::out]] Error* error) noexcept override;
private:
    [[nodiscard]] DX10VertexShader* buildVertexShader([[tau::out]] Error* error, ID3D10Blob* dataBlob) const noexcept;

    [[nodiscard]] DX10GeometryShader* buildGeometryShader([[tau::out]] Error* error, ID3D10Blob* dataBlob) const noexcept;

    [[nodiscard]] DX10PixelShader* buildPixelShader([[tau::out]] Error* error, ID3D10Blob* dataBlob) const noexcept;
};
#endif
