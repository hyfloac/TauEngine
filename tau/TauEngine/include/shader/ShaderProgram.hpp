#pragma once

#include "DLL.hpp"
#include "model/Buffer.hpp"
#include <Objects.hpp>
#include <Safeties.hpp>

class IGraphicsInterface;
class IShader;
class IRenderingContext;

class Vector2f;
class Vector3i;
class Vector3f;
class Vector4f;

class TAU_DLL TAU_NOVTABLE IShaderProgram
{
    DEFAULT_DESTRUCT_VI(IShaderProgram);
    DELETE_COPY(IShaderProgram);
public:
    static CPPRef<IShaderProgram> create(IGraphicsInterface& gi) noexcept;
protected:
    CPPRef<IShader> _vertexShader;
    CPPRef<IShader> _tessellationControlShader;
    CPPRef<IShader> _tessellationEvaluationShader;
    CPPRef<IShader> _geometryShader;
    CPPRef<IShader> _pixelShader;
protected:
    IShaderProgram() noexcept;
public:
    virtual bool setVertexShader(IRenderingContext& context, const CPPRef<IShader>& vs) noexcept;
    virtual bool setTessellationControlShader(IRenderingContext& context, const CPPRef<IShader>& tcs) noexcept;
    virtual bool setTessellationEvaluationShader(IRenderingContext& context, const CPPRef<IShader>& tes) noexcept;
    virtual bool setGeometryShader(IRenderingContext& context, const CPPRef<IShader>& gs) noexcept;
    virtual bool setPixelShader(IRenderingContext& context, const CPPRef<IShader>& ps) noexcept;

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual bool link(IRenderingContext& context) noexcept = 0;
protected:
    virtual bool attach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept = 0;
    virtual void detach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept = 0;
};

struct ShaderProgramArgs final
{
    DynString bundlePath;
};

class TAU_DLL TAU_NOVTABLE IShaderProgramBuilder
{
    
};
