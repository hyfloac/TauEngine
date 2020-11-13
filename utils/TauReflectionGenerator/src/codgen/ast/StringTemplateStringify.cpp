#include "codegen/ast/StringTemplateStringifyAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

void StringTemplateStringifyAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

} } } }
