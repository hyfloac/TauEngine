#pragma once

#include "StringTemplateAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateExprAST : public StringTemplateAST
{
    DEFAULT_CONSTRUCT_PO(StringTemplateExprAST);
    DEFAULT_DESTRUCT_VI(StringTemplateExprAST);
    DELETE_CM(StringTemplateExprAST);
};

class StringTemplateWildExprAST final : public StringTemplateSequenceAST
{
    DEFAULT_DESTRUCT(StringTemplateWildExprAST);
    DELETE_CM(StringTemplateWildExprAST);
private:
    StrongRef<StringTemplateExprAST> _expr;
public:
    StringTemplateWildExprAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev, const StrongRef<StringTemplateExprAST>& expr) noexcept
        : StringTemplateSequenceAST(next, prev)
        , _expr(expr)
    { }
    
    [[nodiscard]]       StrongRef<StringTemplateExprAST>& expr()       noexcept { return _expr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& expr() const noexcept { return _expr; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateStringExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateStringExprAST);
    DELETE_CM(StringTemplateStringExprAST);
private:
    DynString _string;
public:
    StringTemplateStringExprAST(const DynString& string) noexcept
        : _string(string)
    { }

    StringTemplateStringExprAST(DynString&& string) noexcept
        : _string(::std::move(string))
    { }

    [[nodiscard]] const DynString& string() const noexcept { return _string; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateIntegerExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateIntegerExprAST);
    DELETE_CM(StringTemplateIntegerExprAST);
private:
    int _int;
public:
    StringTemplateIntegerExprAST(const int intVal) noexcept
        : _int(intVal)
    { }

    [[nodiscard]] int intVal() const noexcept { return _int; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
