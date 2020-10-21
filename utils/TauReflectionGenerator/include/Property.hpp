#pragma once

#include <clang/Basic/SourceLocation.h>
#include <ds/AVLTree.hpp>
#include "PropertyAttribute.hpp"
#include <deque>
#include <vector>

namespace tau {

class PropertyDeclaration final
{
    DELETE_CM(PropertyDeclaration);
public:
    using AttributeSet = AVLTree<PropertyAttributeData, u16, InsertMethod::Replace>;
private:
    clang::SourceRange _range;
    AttributeSet _attributes;
public:
    PropertyDeclaration(const clang::SourceRange& range) noexcept
        : _range(range)
        , _attributes(PropertyAttributeManager::getAllocator())
    { }

    ~PropertyDeclaration() noexcept;

    [[nodiscard]] const clang::SourceRange& range() const noexcept { return _range; }

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
    DynString _typeName;
    DynString _className;
public:
    Property(const Ref<PropertyDeclaration>& declaration, const DynString& name, const DynString& typeName, const DynString& className) noexcept
        : _declaration(declaration)
        , _name(name)
        , _typeName(typeName)
        , _className(className)
    { }

    [[nodiscard]] const Ref<PropertyDeclaration>& declaration() const noexcept { return _declaration; }
    [[nodiscard]] const DynString& name() const noexcept { return _name; }
    [[nodiscard]] const DynString& typeName() const noexcept { return _typeName; }
    [[nodiscard]] const DynString& className() const noexcept { return _className; }
};

using PropertyDeclQueue = ::std::deque<Ref<PropertyDeclaration>>;
using PropertyList = ::std::vector<Property>;

}
