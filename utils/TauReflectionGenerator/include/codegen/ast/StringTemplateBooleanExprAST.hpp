#pragma once

#include "StringTemplateExprAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateBooleanInvertExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateBooleanInvertExprAST);
    DELETE_CM(StringTemplateBooleanInvertExprAST);
private:
    StrongRef<StringTemplateExprAST> _expr;
public:
    StringTemplateBooleanInvertExprAST(const StrongRef<StringTemplateExprAST>& expr) noexcept
        : _expr(expr)
    { }
    
    [[nodiscard]]       StrongRef<StringTemplateExprAST>& expr()       noexcept { return _expr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& expr() const noexcept { return _expr; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } } 
