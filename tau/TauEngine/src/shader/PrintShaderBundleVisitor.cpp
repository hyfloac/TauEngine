#include "shader/bundle/PrintShaderBundleVisitor.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"
#include <cstdio>

PrintShaderBundleVisitor::PrintShaderBundleVisitor(const PrintSBVArgs* args) noexcept
    : IShaderBundleVisitor(), _currIndent(0)
{
    if(args)
    {
        _indentCount = args->indentCount;
        _indentChar = args->indentChar == PrintSBVArgs::Spaces ? ' ' : '\t';
        _newLine = args->newLineChar == PrintSBVArgs::LF ? "\n" : (args->newLineChar == PrintSBVArgs::CR ? "\r" : "\r\n");
        _tessCtrlName = args->tessCtrlName == PrintSBVArgs::TessellationControl ? "TessellationControl" : "Hull";
        _tessEvalName = args->tessEvalName == PrintSBVArgs::TessellationEvaluation ? "TessellationEvaluation" : "Domain";
        _pixelName = args->pixelName == PrintSBVArgs::Pixel ? "Pixel" : "Fragment";
        _bracesSameLine = args->braces == PrintSBVArgs::SameLine ? true : false;
    }
    else
    {
        _indentCount = 4;
        _indentChar = ' ';
        _newLine = "\n";
        _tessCtrlName = "TessellationControl";
        _tessEvalName = "TessellationEvaluation";
        _pixelName = "Pixel";
        _bracesSameLine = true;
    }
}

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
        case BlockType::TessellationControl:    fputs(_tessCtrlName, stdout); break;
        case BlockType::TessellationEvaluation: fputs(_tessEvalName, stdout); break;
        case BlockType::Geometry:               fputs("Geometry", stdout); break;
        case BlockType::Pixel:                  fputs(_pixelName, stdout); break;
        case BlockType::Uniforms:               fputs("Uniforms", stdout); break;
        case BlockType::Textures:               fputs("Textures", stdout); break;
        case BlockType::Inputs:                 fputs("Inputs", stdout); break;
        case BlockType::Outputs:                fputs("Outputs", stdout); break;
        case BlockType::Global: break;
    }
    fputs(": ", stdout);
    printBrace();
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
    fputs(": ", stdout);
    printBrace();
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
        fputc(_indentChar, stdout);
    }
}

void PrintShaderBundleVisitor::printComma(const ExprAST& expr) const noexcept
{
    if(expr.next())
    {
        fputc(',', stdout);
    }
    fputs(_newLine, stdout);
}

void PrintShaderBundleVisitor::printBrace() const noexcept
{
    if(_bracesSameLine)
    {
        fputc('{', stdout);
        fputs(_newLine, stdout);
    }
    else
    {
        fputs(_newLine, stdout);
        printIndent();
        fputc('{', stdout);
        fputs(_newLine, stdout);
    }
}

