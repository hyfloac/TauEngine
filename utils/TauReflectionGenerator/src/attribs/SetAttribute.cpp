#include "attribs/SetAttribute.hpp"

namespace tau { namespace attribs {

PropertyAttributeData SetPropertyAttribute::parseAttribute(const DynString& attribName, const clang::MacroArgs*, const clang::Token*& currentToken) noexcept
{
    currentToken = getNextToken(currentToken);

    return PropertyAttributeData(this, nullptr, attribName);
}

}}
