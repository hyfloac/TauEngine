#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>
#include <ResourceSelector.hpp>
#include <ReferenceCountingPointer.hpp>

#include "DLL.hpp"
#include "events/Exception.hpp"
#include "EShader.hpp"

class RenderingMode;

#ifndef SHADER_GEN_NAMES
  #ifndef TAU_PRODUCTION
    #define SHADER_GEN_NAMES 1
  #else
    #define SHADER_GEN_NAMES 0
  #endif
#endif

#define SHADER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                RTT_IMPL(_TYPE, IShader)

#if SHADER_GEN_NAMES
  #define SHADER_IMPL(_TYPE) SHADER_IMPL_BASE(_TYPE); \
                             [[nodiscard]] virtual const char* getName() const noexcept override \
                             { return #_TYPE; }
  #define SHADER_GET_NAME(_SHADER_PTR) (_SHADER_PTR)->getName()
#else
  #define SHADER_IMPL(_TYPE) SHADER_IMPL_BASE(_TYPE)
  #define SHADER_GET_NAME(_SHADER_PTR) ""
#endif

/**
 * Represents an abstract, library independent shader.
 */
class TAU_DLL NOVTABLE IShader
{
    DEFAULT_CONSTRUCT_PO(IShader);
    DEFAULT_DESTRUCT_VI(IShader);
    DELETE_COPY(IShader);
public:
    using ShaderType = RunTimeType<IShader>;
public:
    [[nodiscard]] virtual EShader::Stage shaderType() const noexcept = 0;

    RTT_BASE_IMPL(IShader);
    RTT_BASE_CHECK(IShader);
    RTT_BASE_CAST(IShader);

#if SHADER_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif
};

struct ShaderArgs final
{
    DEFAULT_DESTRUCT(ShaderArgs);
    DEFAULT_COPY(ShaderArgs);
public:
    const char* vfsMount;
    const char* path;
    const char* fileName;
    EShader::Stage stage;
public:
	inline ShaderArgs() noexcept
        : vfsMount(null), path(null), fileName(null),
		  stage(static_cast<EShader::Stage>(0))
    { }
};

class RenderingModeResourceSelectorTransformer final : public IResourceSelectorTransformer
{
    DEFAULT_CONSTRUCT_PU(RenderingModeResourceSelectorTransformer);
    DEFAULT_DESTRUCT(RenderingModeResourceSelectorTransformer);
    DELETE_COPY(RenderingModeResourceSelectorTransformer);
public:
    [[nodiscard]] ResIndex transform(const DynString& key) noexcept override;

    [[nodiscard]] static ResIndex transform(const RenderingMode& rm) noexcept;
};

class TAU_DLL NOVTABLE IShaderBuilder
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
        ShaderObjectCreationFailure,
        MemoryAllocationFailure,
        InputLayoutNotSpecified,
        InputLayoutFinalizationFailure
    };
protected:
    static Ref<RenderingModeResourceSelectorTransformer> rsTransformer;
public:
    [[nodiscard]] virtual IShader* build(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IShader* build(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual Ref<IShader> buildCPPRef(const ShaderArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableReferenceCountingPointer<IShader> buildTauRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongReferenceCountingPointer<IShader> buildTauSRef(const ShaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

class IncorrectAPIShaderException final : public Exception
{
public:
    IncorrectAPIShaderException() noexcept = default;
    ~IncorrectAPIShaderException() noexcept override = default;

    EXCEPTION_IMPL(IncorrectAPIShaderException);
};
