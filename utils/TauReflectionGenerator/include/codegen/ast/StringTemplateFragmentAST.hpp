#pragma once

#include "StringTemplateAST.hpp"

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateEndFragmentAST;

class StringTemplateBeginFragmentAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateBeginFragmentAST);
    DELETE_CM(StringTemplateBeginFragmentAST);
private:
    DynString _fragmentName;
    WeakRef<StringTemplateEndFragmentAST> _end;
public:
    StringTemplateBeginFragmentAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const DynString& fragmentName, const WeakRef<StringTemplateEndFragmentAST>& end) noexcept
        : StringTemplateAST(next, prev)
        , _fragmentName(fragmentName)
        , _end(end)
    { }

    StringTemplateBeginFragmentAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, DynString&& fragmentName, const WeakRef<StringTemplateEndFragmentAST>& end) noexcept
        : StringTemplateAST(next, prev)
        , _fragmentName(::std::move(fragmentName))
        , _end(end)
    { }

    [[nodiscard]] const DynString& fragmentName() const noexcept { return _fragmentName; }
    [[nodiscard]]       WeakRef<StringTemplateEndFragmentAST>& end()       noexcept { return _end; }
    [[nodiscard]] const WeakRef<StringTemplateEndFragmentAST>& end() const noexcept { return _end; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

class StringTemplateEndFragmentAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateEndFragmentAST);
    DELETE_CM(StringTemplateEndFragmentAST);
private:
    WeakRef<StringTemplateBeginFragmentAST> _begin;
public:
    StringTemplateEndFragmentAST(const StrongRef<StringTemplateAST>& next, const WeakRef<StringTemplateAST>& prev, const WeakRef<StringTemplateBeginFragmentAST>& begin) noexcept
        : StringTemplateAST(next, prev)
        , _begin(begin)
    { }

    [[nodiscard]]       WeakRef<StringTemplateBeginFragmentAST>& begin()       noexcept { return _begin; }
    [[nodiscard]] const WeakRef<StringTemplateBeginFragmentAST>& begin() const noexcept { return _begin; }

    void visit(IStringTemplateVisitor& visitor) noexcept override;
};

} } } }
