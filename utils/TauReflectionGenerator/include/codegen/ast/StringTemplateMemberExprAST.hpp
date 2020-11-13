#pragma once

#include "StringTemplateAST.hpp"
#include "StringTemplateExprAST.hpp"

#include <vector>

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateMemberAssignExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateMemberAssignExprAST);
    DELETE_CM(StringTemplateMemberAssignExprAST);
private:
    StrongRef<StringTemplateExprAST> _objectExpr;
    DynString _memberName;
    StrongRef<StringTemplateExprAST> _assignmentExpr;
public:
    StringTemplateMemberAssignExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& objectExpr, const DynString& memberName, const StrongRef<StringTemplateExprAST>& assignmentExpr) noexcept
        : StringTemplateExprAST(next, prev)
        , _objectExpr(objectExpr)
        , _memberName(memberName)
        , _assignmentExpr(assignmentExpr)
    { }

    StringTemplateMemberAssignExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& objectExpr, DynString&& memberName, const StrongRef<StringTemplateExprAST>& assignmentExpr) noexcept
        : StringTemplateExprAST(next, prev)
        , _objectExpr(objectExpr)
        , _memberName(::std::move(memberName))
        , _assignmentExpr(assignmentExpr)
    { }

    [[nodiscard]] const DynString& memberName() const noexcept { return _memberName; }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& objectExpr()       noexcept { return _objectExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& objectExpr() const noexcept { return _objectExpr; }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& assignmentExpr()       noexcept { return _assignmentExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& assignmentExpr() const noexcept { return _assignmentExpr; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateMemberAccessExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateMemberAccessExprAST);
    DELETE_CM(StringTemplateMemberAccessExprAST);
private:
    StrongRef<StringTemplateExprAST> _objectExpr;
    DynString _memberName;
public:
    StringTemplateMemberAccessExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& objectExpr, const DynString& memberName) noexcept
        : StringTemplateExprAST(next, prev)
        , _objectExpr(objectExpr)
        , _memberName(memberName)
    { }

    StringTemplateMemberAccessExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& objectExpr, DynString&& memberName) noexcept
        : StringTemplateExprAST(next, prev)
        , _objectExpr(objectExpr)
        , _memberName(::std::move(memberName))
    { }

    [[nodiscard]] const DynString& memberName() const noexcept { return _memberName; }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& objectExpr()       noexcept { return _objectExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& objectExpr() const noexcept { return _objectExpr; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateMethodAccessExprAST final : public StringTemplateExprAST
{
    DEFAULT_DESTRUCT(StringTemplateMethodAccessExprAST);
    DELETE_CM(StringTemplateMethodAccessExprAST);
private:
    StrongRef<StringTemplateExprAST> _objectExpr;
    DynString _methodName;
    ::std::vector<StrongRef<StringTemplateExprAST>> _arguments;
public:
    StringTemplateMethodAccessExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& objectExpr, const DynString& methodName) noexcept
        : StringTemplateExprAST(next, prev)
        , _objectExpr(objectExpr)
        , _methodName(methodName)
    { }

    StringTemplateMethodAccessExprAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& objectExpr, DynString&& methodName) noexcept
        : StringTemplateExprAST(next, prev)
        , _objectExpr(objectExpr)
        , _methodName(::std::move(methodName))
    { }

    [[nodiscard]] const DynString& methodName() const noexcept { return _methodName; }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& objectExpr()       noexcept { return _objectExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& objectExpr() const noexcept { return _objectExpr; }

    [[nodiscard]]       ::std::vector<StrongRef<StringTemplateExprAST>>& arguments()       noexcept { return _arguments; }
    [[nodiscard]] const ::std::vector<StrongRef<StringTemplateExprAST>>& arguments() const noexcept { return _arguments; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
