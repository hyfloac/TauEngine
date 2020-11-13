#pragma once

#include "StringTemplateAST.hpp"
#include "StringTemplateExprAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateVarDeclAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT(StringTemplateVarDeclAST);
    DELETE_CM(StringTemplateVarDeclAST);
private:
    DynString _varName;
public:
    StringTemplateVarDeclAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const DynString& varName) noexcept
        : StringTemplateAST(next, prev)
        , _varName(varName)
    { }

    StringTemplateVarDeclAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, DynString&& varName) noexcept
        : StringTemplateAST(next, prev)
        , _varName(::std::move(varName))
    { }

    [[nodiscard]] const DynString& varName() const noexcept { return _varName; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateVarAssignExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateVarAssignExprAST);
    DELETE_CM(StringTemplateVarAssignExprAST);
private:
    DynString _varName;
    StrongRef<StringTemplateExprAST> _assignmentExpr;
public:
    StringTemplateVarAssignExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const DynString& varName, const StrongRef<StringTemplateExprAST>& assignmentExpr) noexcept
        : StringTemplateExprAST(next, prev)
        , _varName(varName)
        , _assignmentExpr(assignmentExpr)
    { }

    StringTemplateVarAssignExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, DynString&& varName, const StrongRef<StringTemplateExprAST>& assignmentExpr) noexcept
        : StringTemplateExprAST(next, prev)
        , _varName(::std::move(varName))
        , _assignmentExpr(assignmentExpr)
    { }

    [[nodiscard]] const DynString& varName() const noexcept { return _varName; }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& assignmentExpr()       noexcept { return _assignmentExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& assignmentExpr() const noexcept { return _assignmentExpr; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateVarRetrieveExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateVarRetrieveExprAST);
    DELETE_CM(StringTemplateVarRetrieveExprAST);
private:
    DynString _varName;
public:
    StringTemplateVarRetrieveExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const DynString& varName) noexcept
        : StringTemplateExprAST(next, prev)
        , _varName(varName)
    { }

    StringTemplateVarRetrieveExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, DynString&& varName) noexcept
        : StringTemplateExprAST(next, prev)
        , _varName(::std::move(varName))
    { }

    [[nodiscard]] const DynString& varName() const noexcept { return _varName; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
