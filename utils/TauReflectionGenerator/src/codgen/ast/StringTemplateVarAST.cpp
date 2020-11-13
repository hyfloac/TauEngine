#include "codegen/ast/StringTemplateVarAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

void StringTemplateVarDeclAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateVarAssignExprAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateVarRetrieveExprAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

} } } }
