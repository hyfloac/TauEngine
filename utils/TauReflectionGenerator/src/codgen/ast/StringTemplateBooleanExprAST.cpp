#include "codegen/ast/StringTemplateBooleanExprAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau::codegen::string::ast {

void StringTemplateBooleanInvertExprAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

}
