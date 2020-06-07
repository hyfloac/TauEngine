#pragma once

#include "shader/Shader.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class DX10GraphicsInterface;
class DX10RenderingContext;
class ShaderInfoExtractorVisitor;

class TAU_DLL TAU_NOVTABLE DX10Shader : public IShader
{
    DEFAULT_CONSTRUCT_PO(DX10Shader);
    DEFAULT_DESTRUCT_VI(DX10Shader);
    DEFAULT_CM_PO(DX10Shader);
    SHADER_IMPL(DX10Shader);
public:
    virtual void bind(DX10RenderingContext& context) noexcept = 0;
    virtual void unbind(DX10RenderingContext& context) noexcept = 0;
};

class TAU_DLL DX10VertexShader final : public DX10Shader
{
private:
    ID3D10VertexShader* _shader;
    ID3D10Blob* _shaderBlob;
public:
    DX10VertexShader(ID3D10VertexShader* const shader, ID3D10Blob* const shaderBlob) noexcept
        : _shader(shader), _shaderBlob(shaderBlob)
    { shaderBlob->AddRef(); }

    ~DX10VertexShader() noexcept
    {
        RELEASE_DX(_shader);
        RELEASE_DX(_shaderBlob);
    }

    DX10VertexShader(const DX10VertexShader& copy) noexcept
        : DX10Shader(copy)
        , _shader(copy._shader)
        , _shaderBlob(copy._shaderBlob)
    {
        _shader->AddRef();
        _shaderBlob->AddRef();
    }

    DX10VertexShader(DX10VertexShader&& move) noexcept
        : DX10Shader(::std::move(move))
        , _shader(move._shader)
        , _shaderBlob(move._shaderBlob)
    {
        move._shader = null;
        move._shaderBlob = null;
    }

    DX10VertexShader& operator=(const DX10VertexShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX10Shader::operator=(copy);

        _shader = copy._shader;
        _shaderBlob = copy._shaderBlob;

        _shader->AddRef();
        _shaderBlob->AddRef();

        return *this;
    }

    DX10VertexShader& operator=(DX10VertexShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX10Shader::operator=(::std::move(move));

        _shader = move._shader;
        _shaderBlob = move._shaderBlob;

        move._shader = null;
        move._shaderBlob = null;

        return *this;
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
private:
    ID3D10GeometryShader* _shader;
public:
    DX10GeometryShader(ID3D10GeometryShader* const shader) noexcept
        : _shader(shader)
    { }

    ~DX10GeometryShader() noexcept
    { RELEASE_DX(_shader); }

    DX10GeometryShader(const DX10GeometryShader& copy) noexcept
        : DX10Shader(copy)
        , _shader(copy._shader)
    { _shader->AddRef(); }

    DX10GeometryShader(DX10GeometryShader&& move) noexcept
        : DX10Shader(::std::move(move))
        , _shader(move._shader)
    { move._shader = null; }

    DX10GeometryShader& operator=(const DX10GeometryShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX10Shader::operator=(copy);

        _shader = copy._shader;
        _shader->AddRef();

        return *this;
    }

    DX10GeometryShader& operator=(DX10GeometryShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX10Shader::operator=(::std::move(move));

        _shader = move._shader;
        move._shader = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Geometry; }

    [[nodiscard]] const ID3D10GeometryShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10GeometryShader* shader() noexcept { return _shader; }

    void bind(DX10RenderingContext& context) noexcept override;
    void unbind(DX10RenderingContext& context) noexcept override;
};

class TAU_DLL DX10PixelShader final : public DX10Shader
{
private:
    ID3D10PixelShader* _shader;
public:
    DX10PixelShader(ID3D10PixelShader* const shader) noexcept
        : _shader(shader)
    { }

    ~DX10PixelShader() noexcept
    { RELEASE_DX(_shader); }

    DX10PixelShader(const DX10PixelShader& copy) noexcept
        : DX10Shader(copy)
        , _shader(copy._shader)
    { _shader->AddRef(); }

    DX10PixelShader(DX10PixelShader&& move) noexcept
        : DX10Shader(::std::move(move))
        , _shader(move._shader)
    { move._shader = null; }

    DX10PixelShader& operator=(const DX10PixelShader& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        DX10Shader::operator=(copy);

        _shader = copy._shader;
        _shader->AddRef();

        return *this;
    }

    DX10PixelShader& operator=(DX10PixelShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        DX10Shader::operator=(::std::move(move));

        _shader = move._shader;
        move._shader = null;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return EShader::Stage::Pixel; }

    [[nodiscard]] const ID3D10PixelShader* shader() const noexcept { return _shader; }
    [[nodiscard]] ID3D10PixelShader* shader() noexcept { return _shader; }

    void bind(DX10RenderingContext& context) noexcept override;
    void unbind(DX10RenderingContext& context) noexcept override;
};

class TAU_DLL DX10ShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(DX10ShaderBuilder);
    DEFAULT_CM_PU(DX10ShaderBuilder);
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
    ShaderInfoExtractorVisitor* _visitor;
public:
    DX10ShaderBuilder(DX10GraphicsInterface& gi, ShaderInfoExtractorVisitor* const visitor) noexcept
        : _gi(gi)
        , _visitor(visitor)
    { }

    [[nodiscard]] DX10Shader* build(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10Shader* build(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShader> buildCPPRef(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IShader> buildTauRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IShader> buildTauSRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderArgs& args, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] bool processBundle(const ShaderArgs& args, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processShader(const CPPRef<IFile>& file, EShader::Stage stage, [[tau::out]] DXShaderArgs* dxArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] D3D10ShaderObjects createD3DShader(const ShaderArgs& args, const DXShaderArgs& dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
