#pragma once

#include "PropertyAttribute.hpp"

namespace tau { namespace attribs {

class GetPropertyAttribute final : public IPropertyAttribute
{
public:
    PropertyAttributeData parseAttribute(const DynString& attribName, const clang::MacroArgs* Args, const clang::Token*& currentToken) noexcept override;
};

}}
