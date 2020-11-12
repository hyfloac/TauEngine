#include "codegen/StringTemplateVisitor.hpp"
#include "codegen/ast/StringTemplateAST.hpp"
#include "codegen/ast/StringTemplateVarAST.hpp"
#include "codegen/ast/StringTemplateFragmentAST.hpp"
#include "codegen/ast/StringTemplateLoopAST.hpp"
#include "codegen/ast/StringTemplateIfAST.hpp"

namespace tau { namespace codegen { namespace string {

void IStringTemplateVisitor::visit(ast::StringTemplateAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateExprAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateTextBlockAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateVarDeclAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateBeginFragmentAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateEndFragmentAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateBeginLoopAST& ast) noexcept
{
    visit(ast.rangeExpr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateEndLoopAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateBeginIfAST& ast) noexcept
{
    visit(ast.controlExpr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateElseIfAST& ast) noexcept
{
    visit(ast.controlExpr());
    visit(ast.next());
}

void IStringTemplateVisitor::visit(ast::StringTemplateElseAST& ast) noexcept
{ visit(ast.next()); }

void IStringTemplateVisitor::visit(ast::StringTemplateEndIfAST& ast) noexcept
{ visit(ast.next()); }

} } }

