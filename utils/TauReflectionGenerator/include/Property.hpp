#pragma once

#include <clang/Basic/SourceLocation.h>
#include <ds/AVLTree.hpp>
#include "PropertyAttribute.hpp"

namespace tau {

class PropertyDeclaration final
{
    DELETE_CM(PropertyDeclaration);
public:
    using AttributeSet = AVLTree<PropertyAttributeData, u16, InsertMethod::Replace>;
private:
    clang::SourceLocation _location;
    AttributeSet _attributes;
public:
    PropertyDeclaration(const clang::SourceLocation& location) noexcept
        : _location(location)
        , _attributes(PropertyAttributeManager::getAllocator())
    { }

    ~PropertyDeclaration() noexcept;

    [[nodiscard]] clang::SourceLocation location() const noexcept { return _location; }

    void emplaceAttribute(IPropertyAttribute* const handler, void* const data, const DynString& attribName) noexcept
    { _attributes.emplace(handler, data, attribName); }

    void emplaceAttribute(IPropertyAttribute* const handler, void* const data, DynString&& attribName) noexcept
    { _attributes.emplace(handler, data, ::std::move(attribName)); }

    void addAttribute(const PropertyAttributeData& data) noexcept
    { _attributes.insert(data); }

    void addAttribute(PropertyAttributeData&& data) noexcept
    { _attributes.insert(::std::move(data)); }

    bool hasAttribute(const DynString& attribName) const noexcept
    { return _attributes.get(attribName); }

    PropertyAttributeData* getAttribute(const DynString& attribName) noexcept
    { return _attributes.find(attribName); }

    const PropertyAttributeData* getAttribute(const DynString& attribName) const noexcept
    { return _attributes.find(attribName); }
};

class Property final
{
private:
    Ref<PropertyDeclaration> _declaration;
    DynString _name;
};

}
