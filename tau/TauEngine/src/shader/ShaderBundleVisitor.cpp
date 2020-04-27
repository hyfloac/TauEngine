#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "shader/bundle/ast/ExprAST.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"

void IShaderBundleVisitor::visit(const ExprAST* expr) noexcept
{
    if(expr)
    { expr->visit(*this); }
}

void IShaderBundleVisitor::visit(const ExprAST& expr) noexcept
{ expr.visit(*this); }

void IShaderBundleVisitor::visit(const FileExprAST& expr) noexcept
{ visitNext(expr); }

void IShaderBundleVisitor::visit(const TypedBlockExprAST& expr) noexcept
{ visitNext(expr); }

void IShaderBundleVisitor::visit(const NamedBlockExprAST& expr) noexcept
{ visitNext(expr); }

void IShaderBundleVisitor::visit(const ShaderIOMapPointExprAST& expr) noexcept
{ visitNext(expr); }

void IShaderBundleVisitor::visit(const ShaderIOBindPointExprAST& expr) noexcept
{ visitNext(expr); }

void IShaderBundleVisitor::visitNext(const ExprAST& expr) noexcept
{
    if(expr.next())
    { expr.next()->visit(*this); }
}
