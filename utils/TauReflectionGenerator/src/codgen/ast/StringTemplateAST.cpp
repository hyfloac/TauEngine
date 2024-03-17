#include "codegen/ast/StringTemplateAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau::codegen::string::ast {

void StringTemplateRootAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateTextBlockAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

}
