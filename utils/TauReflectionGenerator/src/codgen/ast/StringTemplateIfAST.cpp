#include "codegen/ast/StringTemplateIfAST.hpp"
#include "codegen/StringTemplateVisitor.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

void StringTemplateBeginIfAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateElseIfAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateElseAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

void StringTemplateEndIfAST::visit(IStringTemplateVisitor& visitor) noexcept
{ visitor.visit(*this); }

} } } }
