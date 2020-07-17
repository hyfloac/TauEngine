#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include "DLL.hpp"
#include "graphics/_GraphicsOpaqueObjects.hpp"

class IFile;

struct ShaderProgramArgs final
{
    CPPRef<IFile> bundleFile;
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
