#pragma once

#include "DLL.hpp"
#include "model/Buffer.hpp"
#include <Objects.hpp>
#include <Safeties.hpp>
#include <IFile.hpp>

class IGraphicsInterface;
class IRenderingContext;
class IShader;

class TAU_DLL IShaderProgram
{
    DEFAULT_DESTRUCT_VI(IShaderProgram);
    DELETE_COPY(IShaderProgram);
protected:
    NullableRef<IShader> _vertexShader;
    NullableRef<IShader> _tessCtrlShader;
    NullableRef<IShader> _tessEvalShader;
    NullableRef<IShader> _geometryShader;
    NullableRef<IShader> _pixelShader;
protected:
    IShaderProgram() noexcept
        : _vertexShader(null)
        , _tessCtrlShader(null)
        , _tessEvalShader(null)
        , _geometryShader(null)
        , _pixelShader(null)
    { }
public:
    virtual bool setVertexShader(IRenderingContext& context, const NullableRef<IShader>& vs) noexcept;
    virtual bool setTessCtrlShader(IRenderingContext& context, const NullableRef<IShader>& tcs) noexcept;
    virtual bool setTessEvalShader(IRenderingContext& context, const NullableRef<IShader>& tes) noexcept;
    virtual bool setGeometryShader(IRenderingContext& context, const NullableRef<IShader>& gs) noexcept;
    virtual bool setPixelShader(IRenderingContext& context, const NullableRef<IShader>& ps) noexcept;

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;
};

struct ShaderProgramArgs final
{
    CPPRef<IFile> bundleFile;
};

class TAU_DLL TAU_NOVTABLE IShaderProgramBuilder
{
    DEFAULT_CONSTRUCT_PO(IShaderProgramBuilder);
    DEFAULT_DESTRUCT_VI(IShaderProgramBuilder);
    DELETE_COPY(IShaderProgramBuilder);
public:
    enum class Error
    {
        NoError = 0,
        CompileError,
        InvalidFile,
        InvalidInclude,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure
    };
public:
    [[nodiscard]] virtual IShaderProgram* build(const ShaderProgramArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IShaderProgram* build(const ShaderProgramArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IShaderProgram> buildCPPRef(const ShaderProgramArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IShaderProgram> buildTauRef(const ShaderProgramArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IShaderProgram> buildTauSRef(const ShaderProgramArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
