#pragma once

#include <NumTypes.hpp>
#include <ConPrinter.hpp>

#include "ShaderBundleVisitor.hpp"

namespace sbp {
    struct BindingUnion;
}

struct PrintSBVArgs final
{
    DEFAULT_CONSTRUCT_PU(PrintSBVArgs);
    DEFAULT_DESTRUCT(PrintSBVArgs);
    DEFAULT_CM_PU(PrintSBVArgs);
public:
    enum TabsOrSpaces
    {
        Spaces = 0,
        Tabs
    };

    enum NewLineChar
    {
        LF = 0,
        CR,
        CRLF
    };

    enum TessellationControlName
    {
        TessellationControl = 0,
        Hull
    };

    enum TessellationEvaluationName
    {
        TessellationEvaluation = 0,
        Domain
    };

    enum PixelName
    {
        Pixel = 0,
        Fragment
    };

    enum Braces
    {
        SameLine = 0,
        NextLine
    };
public:
    uSys indentCount;
    TabsOrSpaces indentChar;
    NewLineChar newLineChar;
    TessellationControlName tessCtrlName;
    TessellationEvaluationName tessEvalName;
    PixelName pixelName;
    Braces braces;
    FILE* file;
};

class TAU_DLL PrintShaderBundleVisitor final : public IShaderBundleVisitor
{
    DEFAULT_DESTRUCT(PrintShaderBundleVisitor);
    DEFAULT_CM_PU(PrintShaderBundleVisitor);
private:
    uSys _indentCount;
    char _indentChar;
    const char* _newLine;
    const char* _tessCtrlName;
    const char* _tessEvalName;
    const char* _pixelName;
    bool _bracesSameLine;
    uSys _currIndent;
    FILE* _file;
public:
    PrintShaderBundleVisitor(const PrintSBVArgs* args = null) noexcept;

    void visit(const sbp::ExprAST* expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::ExprAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::RootExprAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::FileExprAST& expr) noexcept override;
    void visit(const sbp::UniformBlockExprAST& expr) noexcept override;
    void visit(const sbp::TextureParamsBlockExprAST& expr) noexcept override;
    void visit(const sbp::ShaderStageBlockExprAST& expr) noexcept override;
    void visit(const sbp::APIBlockExprAST& expr) noexcept override;
    void visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept override;
    void visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept override;
private:
    void printIndent() const noexcept;

    void printComma(bool shouldPrint = true) const noexcept;

    void printBindingUnion(const sbp::BindingUnion& bindingUnion) const noexcept;

    void printBrace() const noexcept;
};
