#include "attribs/GetAttribute.hpp"

namespace tau { namespace attribs {

PropertyAttributeData GetPropertyAttribute::parseAttribute(const DynString& attribName, const clang::MacroArgs*, const clang::Token*& currentToken) noexcept
{
    currentToken = getNextToken(currentToken);

    return PropertyAttributeData(this, nullptr, attribName);
}

}}
