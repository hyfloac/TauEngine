#include "codegen/ast/StringTemplateLoopAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau::codegen::string::ast {

void StringTemplateBeginLoopAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateEndLoopAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateLoopControlAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

}
