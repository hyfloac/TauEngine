#pragma once

#include <DLL.hpp>
#include <Objects.hpp>
#include <String.hpp>
#include <RunTimeType.hpp>
#include <events/Exception.hpp>
#include "IFile.hpp"

class IRenderingContext;
class Vector2f;
class Vector3f;
class Vector3i;
class Vector4f;

#ifndef SHADER_GEN_NAMES
  #ifndef TAU_PRODUCTION
    #define SHADER_GEN_NAMES 1
  #else
    #define SHADER_GEN_NAMES 0
  #endif
#endif

#define SHADER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                public: \
                                    [[nodiscard]] static IShader::ShaderType getStaticType() noexcept \
                                    { static IShader::ShaderType type = IShader::ShaderType::define(); \
                                      return type; } \
                                    [[nodiscard]] virtual IShader::ShaderType getShaderType() const noexcept override \
                                    { return _TYPE::getStaticType(); }

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
    enum class Type
    {
        Vertex = 0,
        TessellationControl,
        Hull = TessellationControl,
        TessellationEvaluation,
        Domain = TessellationEvaluation,
        Geometry,
        Pixel,
        Fragment = Pixel,
        Compute
    };

    using ShaderType = RunTimeType<IShader>;
public:
    // static Ref<IShader> create(IRenderingContext& context, Type shaderType, const NotNull<const char>& shaderPath) noexcept;
protected:
    // Type _shaderType;
protected:
    // IShader(const Type shaderType) noexcept
    //     : _shaderType(shaderType)
    // { }
public:
    // [[nodiscard]] Type shaderType() const noexcept { return _shaderType; }
    [[nodiscard]] virtual Type shaderType() const noexcept = 0;

    [[nodiscard]] virtual IShader::ShaderType getShaderType() const noexcept = 0;

#if SHADER_GEN_NAMES
    [[nodiscard]] virtual const char* getName() const noexcept = 0;
#endif

    // virtual bool loadShader(const char* src = nullptr) noexcept = 0;
};

class TAU_DLL NOVTABLE IShaderBuilder
{
    // DEFAULT_CONSTRUCT_PO(IShaderBuilder);
    DEFAULT_DESTRUCT_VI(IShaderBuilder);
    DELETE_COPY(IShaderBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InvalidShaderType,
        CompileError,
        InvalidFile,
        InvalidInclude,
        ShaderObjectCreationFailure,
        MemoryAllocationFailure
    };
protected:
    Ref<IFile> _file;
    const char* _src;
    IShader::Type _type;
protected:
    IShaderBuilder() noexcept
        : _file(null), _src(null), _type(static_cast<IShader::Type>(IntMaxMin<i32>::Max()))
    { }
public:
    virtual void file(const Ref<IFile>& file) noexcept { _file = file; }
    virtual void src(const char* const src) noexcept { _src = src; }
    virtual void type(const IShader::Type type) noexcept { _type = type; }

    [[nodiscard]] const Ref<IFile>& file() const noexcept { return _file; }
    [[nodiscard]] const char* src() const noexcept { return _src; }
    [[nodiscard]] IShader::Type type() const noexcept { return _type; }

    [[nodiscard]] virtual IShader* build([[tau::out]] Error* error = null) noexcept = 0;
};

class IncorrectAPIShaderException final : public Exception
{
public:
    IncorrectAPIShaderException() noexcept = default;
    ~IncorrectAPIShaderException() noexcept override final = default;

    EXCEPTION_IMPL(IncorrectAPIShaderException);
};
