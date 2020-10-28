#pragma once

#include <clang/Lex/PPCallbacks.h>
#include <clang/Frontend/FrontendAction.h>
#include "reflection/TagDeclaration.hpp"

namespace tau { namespace reflection { namespace processing {

enum class TagType
{
    Unknown = 0,
    Class,
    Property,
    Function
};

class TagPreProcessorAction : public ::clang::PreprocessorFrontendAction
{
private:
    TagDeclQueue& _classTags;
    TagDeclQueue& _propertyTags;
    TagDeclQueue& _functionTags;
public:
    TagPreProcessorAction(TagDeclQueue& classTags, TagDeclQueue& propertyTags, TagDeclQueue& functionTags) noexcept
        : _classTags(classTags)
        , _propertyTags(propertyTags)
        , _functionTags(functionTags)
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
    TagDeclQueue& _functionTags;

    unsigned _invalidAttributeDiagID;
    unsigned _unknownAttributeDiagID;
    unsigned _badAttributeHandlerDiagID;
    unsigned _incorrectAttribForTagDiagID;
public:
    TagPreProcessorCallback(::clang::Preprocessor& pp, TagDeclQueue& classTags, TagDeclQueue& propertyTags, TagDeclQueue& functionTags) noexcept
        : _pp(pp)
        , _classTags(classTags)
        , _propertyTags(propertyTags)
        , _functionTags(functionTags)
    { initDiagnostics(); }

    void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args) override;
private:
    void initDiagnostics() noexcept;
};

} } }
