#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>
#include <IFile.hpp>

#include "DLL.hpp"
#include "events/Exception.hpp"
#include "EShader.hpp"

class IRenderingContext;

#define SHADER_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IShader)

#define SHADER_IMPL(_TYPE) SHADER_IMPL_BASE(_TYPE)

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

    RTT_BASE_IMPL(IShader);
    RTT_BASE_CHECK(IShader);
    RTT_BASE_CAST(IShader);
};

struct ShaderArgs final
{
    DEFAULT_DESTRUCT(ShaderArgs);
    DEFAULT_CM(ShaderArgs);
public:
    CPPRef<IFile> file;
    EShader::Stage stage;
public:
	inline ShaderArgs() noexcept
        : file(null)
        , stage(static_cast<EShader::Stage>(0))
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
        InvalidInclude,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual IShader* build(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IShader* build(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IShader> buildCPPRef(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IShader> buildTauRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IShader> buildTauSRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

class IncorrectAPIShaderException final : public Exception
{
    DEFAULT_CONSTRUCT_PU(IncorrectAPIShaderException);
    DEFAULT_DESTRUCT(IncorrectAPIShaderException);
    DEFAULT_CM_PU(IncorrectAPIShaderException);
    EXCEPTION_IMPL(IncorrectAPIShaderException);
};
