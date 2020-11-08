#include "reflection/attribs/NoListAttribute.hpp"

namespace tau { namespace reflection { namespace attribs {

AttributeData NoListAttribute::parseAttribute(const DynString& attribName, const ::clang::MacroArgs*, const ::clang::Token*& currentToken) const noexcept
{
    currentToken = getNextToken(currentToken);

    return AttributeData(this, nullptr, attribName);
}

} } }
