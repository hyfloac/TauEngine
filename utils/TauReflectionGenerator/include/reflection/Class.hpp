#pragma once

#include "TagDeclaration.hpp"
#include "Property.hpp"
#include "Function.hpp"

namespace tau { namespace reflection {

class Property;

class Class final
{
private:
    Ref<TagDeclaration> _declaration;
    DynString _name;
    PropertyList _properties;
    FunctionList _functions;
public:
    Class(const Ref<TagDeclaration>& declaration, const DynString& name) noexcept
        : _declaration(declaration)
        , _name(name)
    { }

    [[nodiscard]] const Ref<TagDeclaration>& declaration() const noexcept { return _declaration; }
    [[nodiscard]] const DynString& name() const noexcept { return _name; }

    [[nodiscard]]       PropertyList& properties()       noexcept { return _properties; }
    [[nodiscard]] const PropertyList& properties() const noexcept { return _properties; }

    [[nodiscard]]       FunctionList& functions()       noexcept { return _functions; }
    [[nodiscard]] const FunctionList& functions() const noexcept { return _functions; }
};

using ClassList = ::std::vector<Ref<Class>>;

} }
