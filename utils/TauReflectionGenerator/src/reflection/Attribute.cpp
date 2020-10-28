#include <clang/Lex/Token.h>

#include "reflection/Attribute.hpp"
#include "reflection/TagDeclaration.hpp"

namespace tau { namespace reflection {

AttributeManager::AttribHandlerSet AttributeManager::_attributeHandlers;
AttributeManager::FBAllocator AttributeManager::_attribTreeAllocator(sizeof(TagDeclaration::AttributeSet::Node), 8192);

const clang::Token* IAttribute::getNextToken(const clang::Token* currentToken) noexcept
{
    if(!currentToken || currentToken->is(clang::tok::eof))
    { return nullptr; }

    const clang::Token* const nextToken = currentToken + 1;
    if(nextToken->is(clang::tok::eof))
    { return nullptr; }

    return nextToken;
}

Ref<IAttribute> AttributeManager::getAttribute(const DynString& attribName) noexcept
{
    if(_attributeHandlers.count(attribName) == 0)
    { return nullptr; }

    return _attributeHandlers[attribName];
}

} }
