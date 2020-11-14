#pragma once

#include <Safeties.hpp>
#include <String.hpp>

namespace tau { namespace codegen { namespace string {

class IStringTemplateVisitor;

} } }

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateAST
{
    DEFAULT_CONSTRUCT_PO(StringTemplateAST);
    DEFAULT_DESTRUCT_VI(StringTemplateAST);
    DELETE_CM(StringTemplateAST);
public:
    virtual void visit(IStringTemplateVisitor& visitor) noexcept = 0;
};

class StringTemplateSequenceAST : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateSequenceAST);
    DELETE_CM(StringTemplateSequenceAST);
protected:
    StrongRef<StringTemplateSequenceAST> _next;
    WeakRef<StringTemplateSequenceAST> _prev;
protected:
    StringTemplateSequenceAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev) noexcept
        : _next(next)
        , _prev(prev)
    { }
public:
    [[nodiscard]]       StrongRef<StringTemplateSequenceAST>& next()       noexcept { return _next; }
    [[nodiscard]] const StrongRef<StringTemplateSequenceAST>& next() const noexcept { return _next; }
    
    [[nodiscard]]       WeakRef<StringTemplateSequenceAST>& prev()       noexcept { return _prev; }
    [[nodiscard]] const WeakRef<StringTemplateSequenceAST>& prev() const noexcept { return _prev; }

    virtual void visit(IStringTemplateVisitor& visitor) noexcept = 0;
};

class StringTemplateRootAST final: public StringTemplateSequenceAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateRootAST);
    DELETE_CM(StringTemplateRootAST);
public:
    StringTemplateRootAST(const StrongRef<StringTemplateSequenceAST>& next) noexcept
        : StringTemplateSequenceAST(next, nullptr)
    { }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateTextBlockAST final : public StringTemplateSequenceAST
{
    DEFAULT_DESTRUCT(StringTemplateTextBlockAST);
    DELETE_CM(StringTemplateTextBlockAST);
private:
    DynString _text;
public:
    StringTemplateTextBlockAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev, const DynString& text) noexcept
        : StringTemplateSequenceAST(next, prev)
        , _text(text)
    { }

    StringTemplateTextBlockAST(const StrongRef<StringTemplateSequenceAST>& next, const WeakRef<StringTemplateSequenceAST>& prev, DynString&& text) noexcept
        : StringTemplateSequenceAST(next, prev)
        , _text(::std::move(text))
    { }

    [[nodiscard]] const DynString& text() const noexcept { return _text; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
