#pragma once

#include <Safeties.hpp>
#include <String.hpp>

namespace tau { namespace codegen { namespace string {

class IStringTemplateVisitor;

} } }

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateAST);
    DELETE_CM(StringTemplateAST);
protected:
    StrongRef<StringTemplateAST> _next;
    WeakRef<StringTemplateAST> _prev;
protected:
    StringTemplateAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev) noexcept
        : _next(next)
        , _prev(prev)
    { }
public:
    [[nodiscard]]       StrongRef<StringTemplateAST>& next()       noexcept { return _next; }
    [[nodiscard]] const StrongRef<StringTemplateAST>& next() const noexcept { return _next; }
    
    [[nodiscard]]       WeakRef<StringTemplateAST>& prev()       noexcept { return _prev; }
    [[nodiscard]] const WeakRef<StringTemplateAST>& prev() const noexcept { return _prev; }

    virtual void visit(IStringTemplateVisitor& visitor) noexcept = 0;
};

class StringTemplateExprAST : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateExprAST);
    DELETE_CM(StringTemplateExprAST);
protected:
    StringTemplateExprAST(const StrongRef<StringTemplateExprAST>& next, const WeakRef<StringTemplateAST>& prev) noexcept
        : StringTemplateAST(next, prev)
    { }
};

class StringTemplateTextBlockAST final : public StringTemplateAST
{
private:
    DynString _text;
public:
    StringTemplateTextBlockAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const DynString& text) noexcept
        : StringTemplateAST(next, prev)
        , _text(text)
    { }

    StringTemplateTextBlockAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, DynString&& text) noexcept
        : StringTemplateAST(next, prev)
        , _text(::std::move(text))
    { }

    [[nodiscard]] const DynString& text() const noexcept { return _text; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
