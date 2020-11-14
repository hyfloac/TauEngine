#pragma once

#include "StringTemplateAST.hpp"
#include "StringTemplateExprAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateVarDeclAST final : public StringTemplateSequenceAST
{
    DEFAULT_DESTRUCT(StringTemplateVarDeclAST);
    DELETE_CM(StringTemplateVarDeclAST);
private:
    DynString _varName;
public:
    StringTemplateVarDeclAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev, const DynString& varName) noexcept
        : StringTemplateSequenceAST(next, prev)
        , _varName(varName)
    { }

    StringTemplateVarDeclAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev, DynString&& varName) noexcept
        : StringTemplateSequenceAST(next, prev)
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
    StringTemplateVarAssignExprAST(const DynString& varName, const StrongRef<StringTemplateExprAST>& assignmentExpr) noexcept
        : _varName(varName)
        , _assignmentExpr(assignmentExpr)
    { }

    StringTemplateVarAssignExprAST(DynString&& varName, const StrongRef<StringTemplateExprAST>& assignmentExpr) noexcept
        : _varName(::std::move(varName))
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
    StringTemplateVarRetrieveExprAST(const DynString& varName) noexcept
        : _varName(varName)
    { }

    StringTemplateVarRetrieveExprAST(DynString&& varName) noexcept
        : _varName(::std::move(varName))
    { }

    [[nodiscard]] const DynString& varName() const noexcept { return _varName; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
