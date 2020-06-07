#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <IFile.hpp>
#include "DLL.hpp"

class IGraphicsInterface;
class IRenderingContext;
class IShader;

class TAU_DLL TAU_NOVTABLE IShaderProgram
{
    DEFAULT_CONSTRUCT_PO(IShaderProgram);
    DEFAULT_DESTRUCT_VI(IShaderProgram);
    DEFAULT_CM_PO(IShaderProgram);
public:
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
    DEFAULT_CM_PO(IShaderProgramBuilder);
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
