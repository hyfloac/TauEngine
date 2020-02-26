#include "shader/bundle/PrintShaderBundleVisitor.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"
#include <cstdio>

void PrintShaderBundleVisitor::visit(const FileExprAST& expr) noexcept
{
    printIndent();
    printf("File: \"%s\"", expr.filePath().c_str());
    printComma(expr);
    visitNext(expr);
}

void PrintShaderBundleVisitor::visit(const TypedBlockExprAST& expr) noexcept
{
    printIndent();
    switch(expr.type())
    {
        case BlockType::Vertex:                 fputs("Vertex", stdout); break;
        case BlockType::TessellationControl:    fputs("TessellationControl", stdout); break;
        case BlockType::TessellationEvaluation: fputs("TessellationEvaluation", stdout); break;
        case BlockType::Geometry:               fputs("Geometry", stdout); break;
        case BlockType::Pixel:                  fputs("Pixel", stdout); break;
        case BlockType::Uniforms:               fputs("Uniforms", stdout); break;
        case BlockType::Textures:               fputs("Textures", stdout); break;
        case BlockType::Inputs:                 fputs("Inputs", stdout); break;
        case BlockType::Outputs:                fputs("Outputs", stdout); break;
    }
    fputs(": {\n", stdout);
    ++_currIndent;
    visitNext(expr.container());
    --_currIndent;
    printIndent();
    fputc('}', stdout);
    printComma(expr);
    visitNext(expr);
}

void PrintShaderBundleVisitor::visit(const NamedBlockExprAST& expr) noexcept
{
    printIndent();
    fputs(expr.name(), stdout);
    fputs(": {\n", stdout);
    ++_currIndent;
    visitNext(expr.container());
    --_currIndent;
    printIndent();
    fputc('}', stdout);
    printComma(expr);
    visitNext(expr);
}

void PrintShaderBundleVisitor::visit(const ShaderIOMapPointExprAST& expr) noexcept
{
    printIndent();
    printf("%d: %d", expr.bindPoint(), expr.shaderBind());
    printComma(expr);
    visitNext(expr);
}

void PrintShaderBundleVisitor::visit(const ShaderIOBindPointExprAST& expr) noexcept
{
    printIndent();
    printf("%d: \"%s\"", expr.bindPoint(), expr.uniformName().c_str());
    printComma(expr);
    visitNext(expr);
}

void PrintShaderBundleVisitor::printIndent() const noexcept
{
    const uSys numSpaces = _indentCount * _currIndent;

    for(uSys i = 0; i < numSpaces; ++i)
    {
        fputc(' ', stdout);
    }
}

void PrintShaderBundleVisitor::printComma(const ExprAST& expr) noexcept
{
    if(expr.next())
    {
        fputc(',', stdout);
    }
    fputc('\n', stdout);
}
