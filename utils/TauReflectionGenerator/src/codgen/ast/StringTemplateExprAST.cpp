#include "codegen/ast/StringTemplateExprAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau::codegen::string::ast {

void StringTemplateStringExprAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateWildExprAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateIntegerExprAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

}
