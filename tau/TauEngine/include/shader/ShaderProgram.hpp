#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>
#include "DLL.hpp"

class IFile;
class IShader;

struct ShaderProgramAutoArgs final
{
    CPPRef<IFile> bundleFile;
};

struct ShaderProgramManualArgs final
{
    NullableRef<IShader> vertexShader;
    NullableRef<IShader> tessCtrlShader;
    NullableRef<IShader> tessEvalShader;
    NullableRef<IShader> geometryShader;
    NullableRef<IShader> pixelShader;
};

struct ShaderProgramComputeArgs final
{
    NullableRef<IShader> computeShader;
};

#define SHADER_PROGRAM_IMPL(_TYPE) RTT_IMPL(_TYPE, IShaderProgram)

class TAU_DLL TAU_NOVTABLE IShaderProgram
{
    DEFAULT_CONSTRUCT_PO(IShaderProgram);
    DEFAULT_DESTRUCT_VI(IShaderProgram);
    DEFAULT_CM_PO(IShaderProgram);
public:
    RTT_BASE_IMPL(IShaderProgram);
    RTT_BASE_CHECK(IShaderProgram);
    RTT_BASE_CAST(IShaderProgram);
};

class TAU_DLL TAU_NOVTABLE IShaderProgramBuilder
{
    DEFAULT_CONSTRUCT_PO(IShaderProgramBuilder);
    DEFAULT_DESTRUCT_VI(IShaderProgramBuilder);
    DEFAULT_CM_PO(IShaderProgramBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InvalidShaderStage,
        MissingVertexShader,
        MissingTessellationControlShader,
        MissingHullShader = MissingTessellationControlShader,
        MissingTessellationEvaluationShader,
        MissingDomainShader = MissingTessellationEvaluationShader,
        MissingPixelShader,
        CompileError,
        InvalidFile,
        InvalidInclude,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual NullableRef<IShaderProgram> build(const ShaderProgramAutoArgs&   args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IShaderProgram> build(const ShaderProgramManualArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
