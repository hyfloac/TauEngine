#pragma once

#include <clang/Lex/PPCallbacks.h>
#include <clang/Frontend/FrontendAction.h>
#include "TagDeclaration.hpp"

namespace tau {

class TagPreProcessorAction : public ::clang::PreprocessorFrontendAction
{
private:
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;
public:
    TagPreProcessorAction(TagDeclQueue& classTags, TagDeclQueue& propertyTags) noexcept
        : _classTags(classTags)
        , _propertyTags(propertyTags)
    { }
protected:
    void ExecuteAction() override;
};

class TagPreProcessorCallback : public ::clang::PPCallbacks
{
private:
    ::clang::Preprocessor& _pp;
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;

    unsigned _invalidAttributeDiagID;
    unsigned _unknownAttributeDiagID;
    unsigned _badAttributeHandlerDiagID;
    unsigned _incorrectAttribForTagDiagID;
public:
    TagPreProcessorCallback(::clang::Preprocessor& pp, TagDeclQueue& classTags, TagDeclQueue& propertyTags) noexcept
        : _pp(pp)
        , _classTags(classTags)
        , _propertyTags(propertyTags)
    { initDiagnostics(); }

    void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args) override;
private:
    void initDiagnostics() noexcept;
};

}
