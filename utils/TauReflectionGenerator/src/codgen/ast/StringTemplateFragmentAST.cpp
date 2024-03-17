#include "codegen/ast/StringTemplateFragmentAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau::codegen::string::ast {

void StringTemplateBeginFragmentAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateEndFragmentAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

}
