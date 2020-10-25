#pragma once

#include <clang/Basic/SourceLocation.h>
#include <ds/AVLTree.hpp>
#include "Attribute.hpp"
#include <deque>

namespace tau {

class TagDeclaration final
{
    DELETE_CM(TagDeclaration);
public:
    using AttributeSet = AVLTree<AttributeData, u16, InsertMethod::Replace>;
private:
    ::clang::SourceRange _range;
    AttributeSet _attributes;
public:
    TagDeclaration(const ::clang::SourceRange& range) noexcept
        : _range(range)
        , _attributes(AttributeManager::getAllocator())
    { }

    ~TagDeclaration() noexcept
    {
        _attributes.iterate([](AttributeSet::Node* const node) { node->value.handler()->destroyData(node->value); });
    }

    [[nodiscard]] const ::clang::SourceRange& range() const noexcept { return _range; }

    void emplaceAttribute(IAttribute* const handler, void* const data, const DynString& attribName) noexcept
    { _attributes.emplace(handler, data, attribName); }

    void emplaceAttribute(IAttribute* const handler, void* const data, DynString&& attribName) noexcept
    { _attributes.emplace(handler, data, ::std::move(attribName)); }

    void addAttribute(const AttributeData& data) noexcept
    { _attributes.insert(data); }

    void addAttribute(AttributeData&& data) noexcept
    { _attributes.insert(::std::move(data)); }

    bool hasAttribute(const DynString& attribName) const noexcept
    { return _attributes.get(attribName); }

    AttributeData* getAttribute(const DynString& attribName) noexcept
    { return _attributes.find(attribName); }

    const AttributeData* getAttribute(const DynString& attribName) const noexcept
    { return _attributes.find(attribName); }
};

using TagDeclQueue = ::std::deque<Ref<TagDeclaration>>;

}
