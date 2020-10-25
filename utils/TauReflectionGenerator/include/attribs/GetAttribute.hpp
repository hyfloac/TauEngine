#pragma once

#include "Attribute.hpp"

namespace tau { namespace attribs {

class GetPropertyAttribute final : public IAttribute
{
public:
    [[nodiscard]] bool isForProperty() const noexcept override { return true; }

    AttributeData parseAttribute(const DynString& attribName, const clang::MacroArgs* Args, const clang::Token*& currentToken) const noexcept override;

    void generateBaseClass(::llvm::raw_fd_ostream& base) const noexcept override;
    void generateImplClass(::llvm::raw_fd_ostream& base, const Ref<ReflClass>& clazz) const noexcept override;
};

}}
