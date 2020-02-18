#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "shader/bundle/ast/ExprAST.hpp"

void IShaderBundleVisitor::visit(const ExprAST& expr) noexcept
{ expr.visit(*this); }

void IShaderBundleVisitor::visitNext(const ExprAST& expr) noexcept
{
    if(expr.next())
    { expr.next()->visit(*this); }
}
