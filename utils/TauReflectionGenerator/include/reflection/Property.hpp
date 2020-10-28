#pragma once

// #include <clang/Basic/SourceLocation.h>
#include "Attribute.hpp"
#include "TagDeclaration.hpp"
#include <vector>

namespace tau { namespace reflection {

class Class;

class Property final
{
private:
    Ref<TagDeclaration> _declaration;
    Ref<Class> _parentClass;
    DynString _name;
    DynString _typeName;
public:
    Property(const Ref<TagDeclaration>& declaration, const Ref<Class>& parentClass, const DynString& name, const DynString& typeName) noexcept
        : _declaration(declaration)
        , _parentClass(parentClass)
        , _name(name)
        , _typeName(typeName)
    { }

    [[nodiscard]] const Ref<TagDeclaration>& declaration() const noexcept { return _declaration; }
    [[nodiscard]] const Ref<Class>& parentClass() const noexcept { return _parentClass; }
    [[nodiscard]] const DynString& name() const noexcept { return _name; }
    [[nodiscard]] const DynString& typeName() const noexcept { return _typeName; }
};

using PropertyList = ::std::vector<Ref<Property>>;

} }
