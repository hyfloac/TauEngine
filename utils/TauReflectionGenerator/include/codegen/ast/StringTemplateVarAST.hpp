#pragma once

#include "StringTemplateAST.hpp"

namespace tau { namespace codegen { namespace string {

class IStringTemplateVisitor;

} } }

namespace tau { namespace codegen { namespace string { namespace ast {

class StringTemplateVarDeclAST final : public StringTemplateAST
{
    DEFAULT_DESTRUCT_VI(StringTemplateVarDeclAST);
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

} } } }
