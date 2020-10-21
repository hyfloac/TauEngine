#include <clang/Lex/Token.h>
#include "Property.hpp"
#include "PropertyAttribute.hpp"

#include <ds/AVLTree.hpp>

::tau::PropertyAttributeManager::AttribHandlerSet tau::PropertyAttributeManager::_attributeHandlers;
::tau::PropertyAttributeManager::FBAllocator tau::PropertyAttributeManager::_attribTreeAllocator(sizeof(::tau::PropertyDeclaration::AttributeSet::Node), 8192);

const clang::Token* tau::IPropertyAttribute::getNextToken(const clang::Token* currentToken) noexcept
{
    if(!currentToken || currentToken->is(clang::tok::eof))
    { return nullptr; }

    const clang::Token* const nextToken = currentToken + 1;
    if(nextToken->is(clang::tok::eof))
    { return nullptr; }

    return nextToken;
}

Ref<tau::IPropertyAttribute> tau::PropertyAttributeManager::getAttribute(const DynString& attribName) noexcept
{
    if(_attributeHandlers.count(attribName) == 0)
    { return nullptr; }

    return _attributeHandlers[attribName];
}
