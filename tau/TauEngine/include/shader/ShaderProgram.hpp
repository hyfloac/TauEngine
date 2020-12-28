#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>
#include "DLL.hpp"
#include "graphics/_GraphicsOpaqueObjects.hpp"

class IFile;

struct ShaderProgramArgs final
{
    CPPRef<IFile> bundleFile;
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
        CompileError,
        InvalidFile,
        InvalidInclude,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual ShaderProgram build(const ShaderProgramArgs& args, [[tau::out]] Error* error) noexcept = 0;
    virtual void destroy(ShaderProgram program) noexcept = 0;
};
