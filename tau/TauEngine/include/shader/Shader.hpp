#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>
#include <ResourceSelector.hpp>
#include <ReferenceCountingPointer.hpp>

#include "DLL.hpp"
#include "events/Exception.hpp"
#include "EShader.hpp"

class IRenderingContext;

#define SHADER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                RTT_IMPL(_TYPE, IShader)

#define SHADER_IMPL(_TYPE) SHADER_IMPL_BASE(_TYPE);

/**
 * Represents an abstract, library independent shader.
 */
class TAU_DLL TAU_NOVTABLE IShader
{
    DEFAULT_CONSTRUCT_PO(IShader);
    DEFAULT_DESTRUCT_VI(IShader);
    DELETE_COPY(IShader);
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
    DEFAULT_COPY(ShaderArgs);
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
    DELETE_COPY(IShaderBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InvalidShaderStage,
        CompileError,
        InvalidFile,
        InvalidInclude,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure
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
public:
    IncorrectAPIShaderException() noexcept = default;
    ~IncorrectAPIShaderException() noexcept override = default;

    EXCEPTION_IMPL(IncorrectAPIShaderException);
};
