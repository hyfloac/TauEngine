#pragma once

#include "StringTemplateExprAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateStringifyAST final : public StringTemplateSequenceAST
{
    DEFAULT_DESTRUCT(StringTemplateStringifyAST);
    DELETE_CM(StringTemplateStringifyAST);
private:
    StrongRef<StringTemplateExprAST> _expr;
public:
    StringTemplateStringifyAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev, const StrongRef<StringTemplateExprAST>& expr) noexcept
        : StringTemplateSequenceAST(next, prev)
        , _expr(expr)
    { }
    
    [[nodiscard]]       StrongRef<StringTemplateExprAST>& expr()       noexcept { return _expr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& expr() const noexcept { return _expr; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } } 
