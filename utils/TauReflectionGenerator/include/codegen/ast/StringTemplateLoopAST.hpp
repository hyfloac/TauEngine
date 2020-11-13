#pragma once

#include "StringTemplateAST.hpp"
#include "StringTemplateVarAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateEndLoopAST;

class StringTemplateBeginLoopAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateBeginLoopAST);
    DELETE_CM(StringTemplateBeginLoopAST);
private:
    StrongRef<StringTemplateVarDeclAST> _varDecl;
    StrongRef<StringTemplateExprAST> _rangeExpr;
    WeakRef<StringTemplateEndLoopAST> _end;
public:
    StringTemplateBeginLoopAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const StrongRef<StringTemplateVarDeclAST>& varDecl, const StrongRef<StringTemplateExprAST>& rangeExpr, const WeakRef<StringTemplateEndLoopAST>& end) noexcept
        : StringTemplateAST(next, prev)
        , _varDecl(varDecl)
        , _rangeExpr(rangeExpr)
        , _end(end)
    { }

    [[nodiscard]]       StrongRef<StringTemplateVarDeclAST>& varDecl()       noexcept { return _varDecl; }
    [[nodiscard]] const StrongRef<StringTemplateVarDeclAST>& varDecl() const noexcept { return _varDecl; }

    [[nodiscard]]       StrongRef<StringTemplateExprAST>& rangeExpr()       noexcept { return _rangeExpr; }
    [[nodiscard]] const StrongRef<StringTemplateExprAST>& rangeExpr() const noexcept { return _rangeExpr; }

    [[nodiscard]]       WeakRef<StringTemplateEndLoopAST>& end()       noexcept { return _end; }
    [[nodiscard]] const WeakRef<StringTemplateEndLoopAST>& end() const noexcept { return _end; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateEndLoopAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateEndLoopAST);
    DELETE_CM(StringTemplateEndLoopAST);
private:
    WeakRef<StringTemplateBeginLoopAST> _begin;
public:
    StringTemplateEndLoopAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const WeakRef<StringTemplateBeginLoopAST>& begin) noexcept
        : StringTemplateAST(next, prev)
        , _begin(begin)
    { }

    [[nodiscard]]       WeakRef<StringTemplateBeginLoopAST>& begin()       noexcept { return _begin; }
    [[nodiscard]] const WeakRef<StringTemplateBeginLoopAST>& begin() const noexcept { return _begin; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

enum class LoopControlType
{
    Continue = 0,
    Break,
    LoopIndex
};

class StringTemplateLoopControlAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateLoopControlAST);
    DELETE_CM(StringTemplateLoopControlAST);
public:
private:
    WeakRef<StringTemplateBeginLoopAST> _loop;
    LoopControlType _controlType;
    u32 _loopIndex;
public:
    StringTemplateLoopControlAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const WeakRef<StringTemplateBeginLoopAST>& loop, const LoopControlType controlType, const u32 loopIndex) noexcept
        : StringTemplateAST(next, prev)
        , _loop(loop)
        , _controlType(controlType)
        , _loopIndex(loopIndex)
    { }

    [[nodiscard]]       WeakRef<StringTemplateBeginLoopAST>& loop()       noexcept { return _loop; }
    [[nodiscard]] const WeakRef<StringTemplateBeginLoopAST>& loop() const noexcept { return _loop; }

    [[nodiscard]] LoopControlType controlType() const noexcept { return _controlType; }
    [[nodiscard]] u32 loopIndex() const noexcept { return _loopIndex; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
