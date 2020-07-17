#pragma once

#include "shader/ShaderProgram.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#include <Objects.hpp>
#pragma warning(pop)

#include "GLShader.hpp"

class GLShader;
class GLShaderData;

class TAU_DLL GLShaderProgram final
{
    DELETE_CM(GLShaderProgram);
private:
    GLuint _programHandle;
    GLShaderManager* _shaderManager;
    union
    {
        struct
        {
            GLShaderData* _vertex;
            GLShaderData* _tessCtrl;
            GLShaderData* _tessEval;
            GLShaderData* _geometry;
            GLShaderData* _pixel;
        };
        GLShaderData* _shaders[5];
    };
public:
    GLShaderProgram(const GLuint programHandle, GLShaderManager* const shaderManager, GLShaderData* const vertex, GLShaderData* const tessCtrl, GLShaderData* const tessEval, GLShaderData* const geometry, GLShaderData* const pixel) noexcept
        : _programHandle(programHandle)
        , _shaderManager(shaderManager)
        , _vertex(vertex)
        , _tessCtrl(tessCtrl)
        , _tessEval(tessEval)
        , _geometry(geometry)
        , _pixel(pixel)
    { }

    ~GLShaderProgram() noexcept;

    [[nodiscard]] GLuint programHandle() const noexcept { return _programHandle; }

    [[nodiscard]] const GLShaderData* const* shaders() const noexcept { return _shaders; }

    [[nodiscard]] const GLShaderData*   vertex() const noexcept { return _vertex;   }
    [[nodiscard]] const GLShaderData* tessCtrl() const noexcept { return _tessCtrl; }
    [[nodiscard]] const GLShaderData* tessEval() const noexcept { return _tessEval; }
    [[nodiscard]] const GLShaderData* geometry() const noexcept { return _geometry; }
    [[nodiscard]] const GLShaderData*    pixel() const noexcept { return _pixel;    }

    [[nodiscard]] const GLShaderData* get(const EShader::Stage stage) const noexcept
    {
        switch(stage)
        {
            case EShader::Stage::Vertex:                 return _vertex;
            case EShader::Stage::TessellationControl:    return _tessCtrl;
            case EShader::Stage::TessellationEvaluation: return _tessEval;
            case EShader::Stage::Geometry:               return _geometry;
            case EShader::Stage::Pixel:                  return _pixel;
            default:                                     return null;
        }
    }
};

class ShaderInfoExtractorVisitor;
namespace sbp { struct ShaderInfo; }

class TAU_DLL GLShaderProgramBuilder final : public IShaderProgramBuilder
{
    DEFAULT_DESTRUCT(GLShaderProgramBuilder);
    DELETE_CM(GLShaderProgramBuilder);
private:
    struct GLShaderProgramArgs final
    {
        GLuint programHandle;
        union
        {
            struct
            {
                GLShaderData* vertex;
                GLShaderData* tessCtrl;
                GLShaderData* tessEval;
                GLShaderData* geometry;
                GLShaderData* pixel;
            };
            GLShaderData* shaders[5];
        };
    };
private:
    ShaderInfoExtractorVisitor* _visitor;
    GLShaderBuilder* _shaderBuilder;
    GLShaderManager* _shaderManager;
public:
    GLShaderProgramBuilder(ShaderInfoExtractorVisitor* const visitor, GLShaderBuilder* const shaderBuilder) noexcept
        : _visitor(visitor)
        , _shaderBuilder(shaderBuilder)
        , _shaderManager(new GLShaderManager(4096))
    { }

    [[nodiscard]] ShaderProgram build(const ShaderProgramArgs& args, Error* error) noexcept override;
    void destroy(ShaderProgram program) noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderProgramArgs& args, [[tau::out]] GLShaderProgramArgs* glArgs, [[tau::out]] Error* error) noexcept;

    [[nodiscard]] bool processBundle(const ShaderProgramArgs& args, [[tau::out]] GLShaderProgramArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] bool processShader(const DynString& path, EShader::Stage stage, [[tau::out]] GLShaderData** shaderData, [[tau::out]] Error* error) noexcept;

    [[nodiscard]] static uSys shaderIndex(const EShader::Stage stage) noexcept
    {
        switch(stage)
        {
            case EShader::Stage::Vertex:                 return 0;
            case EShader::Stage::TessellationControl:    return 1;
            case EShader::Stage::TessellationEvaluation: return 2;
            case EShader::Stage::Geometry:               return 3;
            case EShader::Stage::Pixel:                  return 4;
            default:                                     return 0;
        }
    }
};
