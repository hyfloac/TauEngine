#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "shader/bundle/ast/AST.hpp"
#include "shader/bundle/ast/RootExprAST.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "shader/bundle/ast/BlockAST.hpp"

void IShaderBundleVisitor::visit(const sbp::AST* expr) noexcept
{
    if(expr)
    { expr->visit(*this); }
}

void IShaderBundleVisitor::visit(const sbp::AST& expr) noexcept
{ expr.visit(*this); }

void IShaderBundleVisitor::visit(const sbp::RootAST& expr) noexcept
{ visit(expr.node().get()); }
