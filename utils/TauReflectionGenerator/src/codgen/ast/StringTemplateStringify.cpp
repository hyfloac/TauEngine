#include "codegen/ast/StringTemplateStringifyAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau::codegen::string::ast {

void StringTemplateStringifyAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

}
