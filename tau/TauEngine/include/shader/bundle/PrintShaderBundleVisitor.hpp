#pragma once

#include <NumTypes.hpp>

#include "ShaderBundleVisitor.hpp"

class TAU_DLL PrintShaderBundleVisitor final : public IShaderBundleVisitor
{
    DEFAULT_DESTRUCT(PrintShaderBundleVisitor);
    DELETE_COPY(PrintShaderBundleVisitor);
private:
    uSys _indentCount;
    uSys _currIndent;
public:
    inline PrintShaderBundleVisitor(const uSys indentCount = 4) noexcept
        : IShaderBundleVisitor(), _indentCount(indentCount), _currIndent(0)
    { }

    void visit(const FileExprAST& expr) noexcept override;
    void visit(const TypedBlockExprAST& expr) noexcept override;
    void visit(const NamedBlockExprAST& expr) noexcept override;
    void visit(const ShaderIOMapPointExprAST& expr) noexcept override;
    void visit(const ShaderIOBindPointExprAST& expr) noexcept override;
private:
    void printIndent() const noexcept;

    static void printComma(const ExprAST& expr) noexcept;
};
