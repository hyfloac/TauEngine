#pragma once

#include "Attribute.hpp"
#include "TagDeclaration.hpp"
#include <vector>

namespace tau { namespace reflection {

class Class;

class Function final
{
private:
    Ref<TagDeclaration> _declaration;
    Ref<Class> _parentClass;
    DynString _name;
public:
    Function(const Ref<TagDeclaration>& declaration, const Ref<Class>& parentClass, const DynString& name) noexcept
        : _declaration(declaration)
        , _parentClass(parentClass)
        , _name(name)
    { }

    [[nodiscard]] const Ref<TagDeclaration>& declaration() const noexcept { return _declaration; }
    [[nodiscard]] const Ref<Class>& parentClass() const noexcept { return _parentClass; }
    [[nodiscard]] const DynString& name() const noexcept { return _name; }
};

using FunctionList = ::std::vector<Ref<Function>>;

} }
