#pragma once

#include "shader/IShader.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11RenderingContext;

class TAU_DLL NOVTABLE DX11Shader : public IShader
{
    DEFAULT_CONSTRUCT_PO(DX11Shader);
    DEFAULT_DESTRUCT_VI(DX11Shader);
    SHADER_IMPL(DX11Shader);
public:
    virtual void bind(DX11RenderingContext& context) noexcept = 0;
    virtual void unbind(DX11RenderingContext& context) noexcept = 0;
};

class TAU_DLL DX11VertexShader final : public DX11Shader
{
    DELETE_COPY(DX11VertexShader);
private:
    ID3D11VertexShader* _shader;
    ID3DBlob* _shaderBlob;
public:
    DX11VertexShader(ID3D11VertexShader* shader, ID3DBlob* shaderBlob) noexcept
        : _shader(shader), _shaderBlob(shaderBlob)
    { shaderBlob->AddRef(); }

    ~DX11VertexShader() noexcept
    {
        _shader->Release();
        _shaderBlob->Release();
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Vertex; }

    [[nodiscard]] const ID3D11VertexShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11VertexShader* shader() noexcept { return _shader; }

    [[nodiscard]] ID3DBlob* shaderBlob() noexcept { return _shaderBlob; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11HullShader final : public DX11Shader
{
    DELETE_COPY(DX11HullShader);
private:
    ID3D11HullShader* _shader;
public:
    DX11HullShader(ID3D11HullShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX11HullShader() noexcept
    { _shader->Release(); }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Hull; }

    [[nodiscard]] const ID3D11HullShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11HullShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11DomainShader final : public DX11Shader
{
    DELETE_COPY(DX11DomainShader);
private:
    ID3D11DomainShader* _shader;
public:
    DX11DomainShader(ID3D11DomainShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX11DomainShader() noexcept
    { _shader->Release(); }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Domain; }

    [[nodiscard]] const ID3D11DomainShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11DomainShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11GeometryShader final : public DX11Shader
{
    DELETE_COPY(DX11GeometryShader);
private:
    ID3D11GeometryShader* _shader;
public:
    DX11GeometryShader(ID3D11GeometryShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX11GeometryShader() noexcept
    { _shader->Release(); }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Geometry; }

    [[nodiscard]] const ID3D11GeometryShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11GeometryShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11PixelShader final : public DX11Shader
{
    DELETE_COPY(DX11PixelShader);
private:
    ID3D11PixelShader* _shader;
public:
    DX11PixelShader(ID3D11PixelShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX11PixelShader() noexcept
    { _shader->Release(); }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Pixel; }

    [[nodiscard]] const ID3D11PixelShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11PixelShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11ShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(DX11ShaderBuilder);
    DELETE_COPY(DX11ShaderBuilder);
private:
    struct DXShaderArgs final
    {
        ID3DBlob* dataBlob;
    };

    union D3D11ShaderObjects final
    {
        ID3D11VertexShader* vertex;
        ID3D11HullShader* hull;
        ID3D11DomainShader* domain;
        ID3D11GeometryShader* geometry;
        ID3D11PixelShader* pixel;
    };
private:
    DX11RenderingContext& _ctx;
    IResourceSelectorTransformer::ResIndex _resIndex;
public:
    DX11ShaderBuilder(DX11RenderingContext& ctx) noexcept;

    [[nodiscard]] DX11Shader* build(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11Shader* build(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShader> buildCPPRef(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IShader> buildTauRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IShader> buildTauSRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderArgs& args, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] D3D11ShaderObjects createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
