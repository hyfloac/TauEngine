#pragma once

#include <clang/Lex/PPCallbacks.h>
#include <deque>
#include "Property.hpp"

namespace tau {

class PropertyPreProcessorCallback : public ::clang::PPCallbacks
{
private:
    ::std::deque<PropertyDeclaration>& _propertyTags;
public:
    PropertyPreProcessorCallback(::std::deque<PropertyDeclaration>& propertyTags) noexcept
        : _propertyTags(propertyTags)
    { }

    void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args) override;
};

}
