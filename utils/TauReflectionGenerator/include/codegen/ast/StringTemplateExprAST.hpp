#pragma once

#include "StringTemplateAST.hpp"

namespace tau { namespace codegen { namespace string {

class IStringTemplateVisitor;

} } }

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateExprAST : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateExprAST);
    DELETE_CM(StringTemplateExprAST);
protected:
    StringTemplateExprAST(const StrongRef<StringTemplateExprAST>& next, const WeakRef<StringTemplateAST>& prev) noexcept
        : StringTemplateAST(next, prev)
    { }
};

class StringTemplateStringExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateStringExprAST);
    DELETE_CM(StringTemplateStringExprAST);
private:
    DynString _string;
public:
    StringTemplateStringExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const DynString& string) noexcept
        : StringTemplateExprAST(next, prev)
        , _string(string)
    { }

    StringTemplateStringExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, DynString&& string) noexcept
        : StringTemplateExprAST(next, prev)
        , _string(::std::move(string))
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
    StringTemplateIntegerExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const int intVal) noexcept
        : StringTemplateExprAST(next, prev)
        , _int(intVal)
    { }

    [[nodiscard]] int intVal() const noexcept { return _int; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
