#pragma once

#include <clang/Lex/PPCallbacks.h>
#include <clang/Frontend/FrontendAction.h>
#include "Property.hpp"

namespace tau {

class PropertyPreProcessorAction : public ::clang::PreprocessorFrontendAction
{
private:
    PropertyDeclQueue& _propertyTags;
public:
    PropertyPreProcessorAction(PropertyDeclQueue& propertyTags) noexcept
        : _propertyTags(propertyTags)
    { }
protected:
    void ExecuteAction() override;
};

class PropertyPreProcessorCallback : public ::clang::PPCallbacks
{
private:
    PropertyDeclQueue& _propertyTags;
public:
    PropertyPreProcessorCallback(PropertyDeclQueue& propertyTags) noexcept
        : _propertyTags(propertyTags)
    { }

    void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args) override;
};

}
