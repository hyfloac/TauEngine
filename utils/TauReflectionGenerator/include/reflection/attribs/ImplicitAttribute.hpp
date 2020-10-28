#pragma once

#include "reflection/Attribute.hpp"

namespace tau { namespace reflection { namespace attribs {

class ImplicitAttribute final : public IAttribute
{
public:
    [[nodiscard]] bool isForClass()    const noexcept override { return true; }
    [[nodiscard]] bool isForProperty() const noexcept override { return true; }
    [[nodiscard]] bool isForFunction() const noexcept override { return true; }

    AttributeData parseAttribute(const DynString& attribName, const ::clang::MacroArgs* args, const ::clang::Token*& currentToken) const noexcept override;

    void generateBaseTauClass(::llvm::raw_fd_ostream& base) const noexcept override;
    void generateImplTauClass(::llvm::raw_fd_ostream& base, const Ref<Class>& clazz) const noexcept override;
};

} } }
