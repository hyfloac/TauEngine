#pragma once

#include "TagDeclaration.hpp"
#include "property/Property.hpp"

namespace tau {

class Property;

class ReflClass final
{
private:
    Ref<TagDeclaration> _declaration;
    DynString _name;
    PropertyList _properties;
public:
    ReflClass(const Ref<TagDeclaration>& declaration, const DynString& name) noexcept
        : _declaration(declaration)
        , _name(name)
    { }

    [[nodiscard]] const Ref<TagDeclaration>& declaration() const noexcept { return _declaration; }
    [[nodiscard]] const DynString& name() const noexcept { return _name; }

    [[nodiscard]]       PropertyList& properties()       noexcept { return _properties; }
    [[nodiscard]] const PropertyList& properties() const noexcept { return _properties; }
};

using ClassList = ::std::vector<Ref<ReflClass>>;

}
