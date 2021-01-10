#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>
#include <IFile.hpp>

#include "DLL.hpp"
#include "events/Exception.hpp"
#include "EShader.hpp"

class IRenderingContext;

#define SHADER_IMPL(_TYPE) RTTD_IMPL(_TYPE, IShader)

/**
 * Represents an abstract, library independent shader.
 */
class TAU_DLL TAU_NOVTABLE IShader
{
    DEFAULT_CONSTRUCT_PO(IShader);
    DEFAULT_DESTRUCT_VI(IShader);
    DEFAULT_CM_PO(IShader);
public:
    [[nodiscard]] virtual EShader::Stage shaderStage() const noexcept = 0;

    [[nodiscard]] virtual uSys mapUniform(const uSys virtualIndex) const noexcept { return virtualIndex; }
    [[nodiscard]] virtual uSys mapTexture(const uSys virtualIndex) const noexcept { return virtualIndex; }

    RTTD_BASE_IMPL(IShader);
    RTTD_BASE_CHECK(IShader);
    RTTD_BASE_CAST(IShader);
};

struct ShaderFileArgs final
{
    DEFAULT_DESTRUCT(ShaderFileArgs);
    DEFAULT_CM_PU(ShaderFileArgs);
public:
    CPPRef<IFile> file;
    EShader::Stage stage;
public:
	ShaderFileArgs() noexcept
        : file(nullptr)
        , stage(static_cast<EShader::Stage>(0))
    { }

	ShaderFileArgs(const CPPRef<IFile>& _file, const EShader::Stage _stage) noexcept
        : file(_file)
        , stage(_stage)
    { }
};

struct ShaderSourceArgs final
{
    DEFAULT_DESTRUCT(ShaderSourceArgs);
    DEFAULT_CM_PU(ShaderSourceArgs);
public:
    DynString source;
    EShader::Stage stage;
public:
	ShaderSourceArgs() noexcept
        : stage(static_cast<EShader::Stage>(0))
    { }
};

class TAU_DLL TAU_NOVTABLE IShaderBuilder
{
    DEFAULT_CONSTRUCT_PO(IShaderBuilder);
    DEFAULT_DESTRUCT_VI(IShaderBuilder);
    DEFAULT_CM_PO(IShaderBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InvalidShaderStage,
        CompileError,
        InvalidFile,
        InvalidSource,
        InvalidInclude,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual NullableRef<IShader> buildTauRef(const ShaderFileArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IShader> buildTauRef(const ShaderSourceArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

class IncorrectAPIShaderException final : public Exception
{
    DEFAULT_CONSTRUCT_PU(IncorrectAPIShaderException);
    DEFAULT_DESTRUCT(IncorrectAPIShaderException);
    DEFAULT_CM_PU(IncorrectAPIShaderException);
    EXCEPTION_IMPL(IncorrectAPIShaderException);
};
