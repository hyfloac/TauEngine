#pragma once

#include "reflection/Attribute.hpp"

namespace tau { namespace reflection { namespace attribs {

/**
 * Used to indicate that this property should not be listed.
 *
 *   This should be used when you don't want to expose the
 * property in the TauClass, but still want to use some
 * attribute. This prevents it from showing up in the index
 * lists and from having its name stringified anywhere.
 */
class NoListAttribute final : public IAttribute
{
public:
    [[nodiscard]] bool isForClass()    const noexcept override { return true; }
    [[nodiscard]] bool isForProperty() const noexcept override { return true; }
    [[nodiscard]] bool isForFunction() const noexcept override { return true; }

    AttributeData parseAttribute(const DynString& attribName, const ::clang::MacroArgs* args, const ::clang::Token*& currentToken) const noexcept override;
};

} } }
