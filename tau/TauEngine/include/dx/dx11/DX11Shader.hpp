#pragma once

#include "shader/Shader.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;
class ShaderInfoExtractorVisitor;

class TAU_DLL NOVTABLE DX11Shader : public IShader
{
    DEFAULT_CONSTRUCT_PO(DX11Shader);
    DEFAULT_DESTRUCT_VI(DX11Shader);
    DEFAULT_CM_PO(DX11Shader);
    SHADER_IMPL(DX11Shader);
public:
    virtual void bind(DX11RenderingContext& context) noexcept = 0;
    virtual void unbind(DX11RenderingContext& context) noexcept = 0;
};

class TAU_DLL DX11VertexShader final : public DX11Shader
{
private:
    ID3D11VertexShader* _shader;
    ID3DBlob* _shaderBlob;
public:
    DX11VertexShader(ID3D11VertexShader* const shader, ID3DBlob* const shaderBlob) noexcept
        : _shader(shader), _shaderBlob(shaderBlob)
    { shaderBlob->AddRef(); }

    ~DX11VertexShader() noexcept
    {
        RELEASE_DX(_shader);
        RELEASE_DX(_shaderBlob);
    }

    DX11VertexShader(const DX11VertexShader& copy) noexcept
        : DX11Shader(copy)
        , _shader(copy._shader)
        , _shaderBlob(copy._shaderBlob)
    {
        _shader->AddRef();
        _shaderBlob->AddRef();
    }

    DX11VertexShader(DX11VertexShader&& move) noexcept
        : DX11Shader(::std::move(move))
        , _shader(move._shader)
        , _shaderBlob(move._shaderBlob)
    {
        move._shader = null;
        move._shaderBlob = null;
    }

    DX11VertexShader& operator=(const DX11VertexShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX11Shader::operator=(copy);

        _shader = copy._shader;
        _shaderBlob = copy._shaderBlob;

        _shader->AddRef();
        _shaderBlob->AddRef();

        return *this;
    }

    DX11VertexShader& operator=(DX11VertexShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX11Shader::operator=(::std::move(move));

        _shader = move._shader;
        _shaderBlob = move._shaderBlob;

        move._shader = null;
        move._shaderBlob = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Vertex; }

    [[nodiscard]] const ID3D11VertexShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11VertexShader* shader() noexcept { return _shader; }

    [[nodiscard]] ID3DBlob* shaderBlob() const noexcept { return _shaderBlob; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11HullShader final : public DX11Shader
{
private:
    ID3D11HullShader* _shader;
public:
    DX11HullShader(ID3D11HullShader* const shader) noexcept
        : _shader(shader)
    { }

    ~DX11HullShader() noexcept
    { RELEASE_DX(_shader); }

    DX11HullShader(const DX11HullShader& copy) noexcept
        : DX11Shader(copy)
        , _shader(copy._shader)
    { _shader->AddRef(); }

    DX11HullShader(DX11HullShader&& move) noexcept
        : DX11Shader(::std::move(move))
        , _shader(move._shader)
    { move._shader = null; }

    DX11HullShader& operator=(const DX11HullShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX11Shader::operator=(copy);

        _shader = copy._shader;
        _shader->AddRef();

        return *this;
    }

    DX11HullShader& operator=(DX11HullShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX11Shader::operator=(::std::move(move));

        _shader = move._shader;
        move._shader = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Hull; }

    [[nodiscard]] const ID3D11HullShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11HullShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11DomainShader final : public DX11Shader
{
private:
    ID3D11DomainShader* _shader;
public:
    DX11DomainShader(ID3D11DomainShader* const shader) noexcept
        : _shader(shader)
    { }

    ~DX11DomainShader() noexcept
    { RELEASE_DX(_shader); }

    DX11DomainShader(const DX11DomainShader& copy) noexcept
        : DX11Shader(copy)
        , _shader(copy._shader)
    { _shader->AddRef(); }

    DX11DomainShader(DX11DomainShader&& move) noexcept
        : DX11Shader(::std::move(move))
        , _shader(move._shader)
    { move._shader = null; }

    DX11DomainShader& operator=(const DX11DomainShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX11Shader::operator=(copy);

        _shader = copy._shader;
        _shader->AddRef();

        return *this;
    }

    DX11DomainShader& operator=(DX11DomainShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX11Shader::operator=(::std::move(move));

        _shader = move._shader;
        move._shader = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Domain; }

    [[nodiscard]] const ID3D11DomainShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11DomainShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11GeometryShader final : public DX11Shader
{
private:
    ID3D11GeometryShader* _shader;
public:
    DX11GeometryShader(ID3D11GeometryShader* const shader) noexcept
        : _shader(shader)
    { }

    ~DX11GeometryShader() noexcept
    { RELEASE_DX(_shader); }

    DX11GeometryShader(const DX11GeometryShader& copy) noexcept
        : DX11Shader(copy)
        , _shader(copy._shader)
    { _shader->AddRef(); }

    DX11GeometryShader(DX11GeometryShader&& move) noexcept
        : DX11Shader(::std::move(move))
        , _shader(move._shader)
    { move._shader = null; }

    DX11GeometryShader& operator=(const DX11GeometryShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX11Shader::operator=(copy);

        _shader = copy._shader;
        _shader->AddRef();

        return *this;
    }

    DX11GeometryShader& operator=(DX11GeometryShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX11Shader::operator=(::std::move(move));

        _shader = move._shader;
        move._shader = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Geometry; }

    [[nodiscard]] const ID3D11GeometryShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11GeometryShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11PixelShader final : public DX11Shader
{
private:
    ID3D11PixelShader* _shader;
public:
    DX11PixelShader(ID3D11PixelShader* shader) noexcept
        : _shader(shader)
    { }

    ~DX11PixelShader() noexcept
    { RELEASE_DX(_shader); }

    DX11PixelShader(const DX11PixelShader& copy) noexcept
        : DX11Shader(copy)
        , _shader(copy._shader)
    { _shader->AddRef(); }

    DX11PixelShader(DX11PixelShader&& move) noexcept
        : DX11Shader(::std::move(move))
        , _shader(move._shader)
    { move._shader = null; }

    DX11PixelShader& operator=(const DX11PixelShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX11Shader::operator=(copy);

        _shader = copy._shader;
        _shader->AddRef();

        return *this;
    }

    DX11PixelShader& operator=(DX11PixelShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX11Shader::operator=(::std::move(move));

        _shader = move._shader;
        move._shader = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Pixel; }

    [[nodiscard]] const ID3D11PixelShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D11PixelShader* shader() noexcept { return _shader; }

    void bind(DX11RenderingContext& context) noexcept override;
    void unbind(DX11RenderingContext& context) noexcept override;
};

class TAU_DLL DX11ShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(DX11ShaderBuilder);
    DEFAULT_CM_PU(DX11ShaderBuilder);
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
    DX11GraphicsInterface& _gi;
    ShaderInfoExtractorVisitor* _visitor;
public:
    DX11ShaderBuilder(DX11GraphicsInterface& gi, ShaderInfoExtractorVisitor* const visitor) noexcept
        : _gi(gi)
        , _visitor(visitor)
    { }

    [[nodiscard]] DX11Shader* build(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11Shader* build(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShader> buildCPPRef(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IShader> buildTauRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IShader> buildTauSRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderArgs& args, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] bool processBundle(const ShaderArgs& args, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processShader(const CPPRef<IFile>& file, EShader::Stage stage, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] D3D11ShaderObjects createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
