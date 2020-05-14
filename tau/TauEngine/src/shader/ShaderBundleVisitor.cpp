#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "shader/bundle/ast/ExprAST.hpp"
#include "shader/bundle/ast/RootExprAST.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "shader/bundle/ast/BlockExprAST.hpp"

void IShaderBundleVisitor::visit(const sbp::ExprAST* expr) noexcept
{
    if(expr)
    { expr->visit(*this); }
}

void IShaderBundleVisitor::visit(const sbp::ExprAST& expr) noexcept
{ expr.visit(*this); }

void IShaderBundleVisitor::visit(const sbp::RootExprAST& expr) noexcept
{ visit(expr.node().get()); }
