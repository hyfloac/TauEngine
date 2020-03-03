#pragma once

#include "shader/bundle/ShaderBundleVisitor.hpp"

#include <String.hpp>
#include <DynArray.hpp>

struct GLShaderInfo
{
    DynString fileName;
    DynArray<DynString> uniformPoints;
    DynArray<DynString> texturePoints;

    inline GLShaderInfo() noexcept
        : fileName(""), uniformPoints(0), texturePoints(0)
    { }
};

struct GLOuterShaderInfo final : public GLShaderInfo
{
    DynArray<DynString> ioPoints;

    inline GLOuterShaderInfo() noexcept
        : GLShaderInfo(), ioPoints(0)
    { }
};

class TAU_DLL GLShaderBundleVisitor : public IShaderBundleVisitor
{
    DEFAULT_CONSTRUCT_PU(GLShaderBundleVisitor);
    DEFAULT_DESTRUCT(GLShaderBundleVisitor);
    DELETE_COPY(GLShaderBundleVisitor);
protected:
    GLOuterShaderInfo _vertex;
    GLShaderInfo _tessCtrl;
    GLShaderInfo _tessEval;
    GLShaderInfo _geometry;
    GLOuterShaderInfo _pixel;
public:
    void visit(const FileExprAST& expr) noexcept override;
    void visit(const TypedBlockExprAST& expr) noexcept override;
    void visit(const ShaderIOMapPointExprAST& expr) noexcept override;
    void visit(const ShaderIOBindPointExprAST& expr) noexcept override;
};
