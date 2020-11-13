#pragma once

#include "StringTemplateAST.hpp"
#include "StringTemplateExprAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateEndLoopAST;

class StringTemplateBeginIfAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateBeginIfAST);
    DELETE_CM(StringTemplateBeginIfAST);
private:
    StrongRef<StringTemplateExprAST> _controlExpr;
    WeakRef<StringTemplateEndLoopAST> _end;
public:
    StringTemplateBeginIfAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& controlExpr, const WeakRef<StringTemplateEndLoopAST>& end) noexcept
        : StringTemplateAST(next, prev)
        , _controlExpr(controlExpr)
        , _end(end)
    { }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& controlExpr()       noexcept { return _controlExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& controlExpr() const noexcept { return _controlExpr; }

    [[nodiscard]]       WeakRef<StringTemplateEndLoopAST>& end()       noexcept { return _end; }
    [[nodiscard]] const WeakRef<StringTemplateEndLoopAST>& end() const noexcept { return _end; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateElseIfAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateElseIfAST);
    DELETE_CM(StringTemplateElseIfAST);
private:
    StrongRef<StringTemplateExprAST> _controlExpr;
    WeakRef<StringTemplateBeginIfAST> _begin;
    WeakRef<StringTemplateEndLoopAST> _end;
public:
    StringTemplateElseIfAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateExprAST>& controlExpr, const WeakRef<StringTemplateBeginIfAST>& begin, const WeakRef<StringTemplateEndLoopAST>& end) noexcept
        : StringTemplateAST(next, prev)
        , _controlExpr(controlExpr)
        , _begin(begin)
        , _end(end)
    { }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& controlExpr()       noexcept { return _controlExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& controlExpr() const noexcept { return _controlExpr; }

    [[nodiscard]]       WeakRef<StringTemplateBeginIfAST>& begin()       noexcept { return _begin; }
    [[nodiscard]] const WeakRef<StringTemplateBeginIfAST>& begin() const noexcept { return _begin; }

    [[nodiscard]]       WeakRef<StringTemplateEndLoopAST>& end()       noexcept { return _end; }
    [[nodiscard]] const WeakRef<StringTemplateEndLoopAST>& end() const noexcept { return _end; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateElseAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateElseAST);
    DELETE_CM(StringTemplateElseAST);
private:
    WeakRef<StringTemplateBeginIfAST> _begin;
    WeakRef<StringTemplateEndLoopAST> _end;
public:
    StringTemplateElseAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const WeakRef<StringTemplateBeginIfAST>& begin, const WeakRef<StringTemplateEndLoopAST>& end) noexcept
        : StringTemplateAST(next, prev)
        , _begin(begin)
        , _end(end)
    { }

    [[nodiscard]]       WeakRef<StringTemplateBeginIfAST>& begin()       noexcept { return _begin; }
    [[nodiscard]] const WeakRef<StringTemplateBeginIfAST>& begin() const noexcept { return _begin; }

    [[nodiscard]]       WeakRef<StringTemplateEndLoopAST>& end()       noexcept { return _end; }
    [[nodiscard]] const WeakRef<StringTemplateEndLoopAST>& end() const noexcept { return _end; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateEndIfAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateEndIfAST);
    DELETE_CM(StringTemplateEndIfAST);
private:
    WeakRef<StringTemplateBeginIfAST> _begin;
public:
    StringTemplateEndIfAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const WeakRef<StringTemplateBeginIfAST>& begin) noexcept
        : StringTemplateAST(next, prev)
        , _begin(begin)
    { }

    [[nodiscard]]       WeakRef<StringTemplateBeginIfAST>& begin()       noexcept { return _begin; }
    [[nodiscard]] const WeakRef<StringTemplateBeginIfAST>& begin() const noexcept { return _begin; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
