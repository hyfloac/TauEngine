#include "TagPreProcessor.hpp"
#include <clang/Lex/Token.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Lex/MacroInfo.h>
#include <clang/Lex/MacroArgs.h>
#include <clang/Frontend/CompilerInstance.h>

namespace tau {

void TagPreProcessorAction::ExecuteAction()
{
    ::clang::Preprocessor& pp = getCompilerInstance().getPreprocessor();

    pp.addPPCallbacks(::std::make_unique<TagPreProcessorCallback>(pp, _classTags, _propertyTags));

    // Ignore unknown pragmas.
    pp.IgnorePragmas();

    ::clang::Token tok;
    // Start parsing the specified input file.
    pp.EnterMainSourceFile();

    do 
    {
        pp.Lex(tok);
    } while(tok.isNot(::clang::tok::eof));
}

enum class TagType
{
    Unknown = 0,
    Class,
    Property,
    Function
};

static void reportIncorrectAttribForTag(TagType tagType, const Ref<IAttribute>& attribHandler, const ::clang::Token* token, ::clang::DiagnosticsEngine& diagEngine, unsigned diagID) noexcept;

void TagPreProcessorCallback::MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, const clang::SourceRange Range, const clang::MacroArgs* const Args)
{
    if(!MacroNameTok.isAnyIdentifier())
    { return; }

    TagType tagType;
    if(MacroNameTok.getIdentifierInfo()->getName() == "TAU_CLASS")
    {
        tagType = TagType::Class;
        _classTags.emplace_back(Range);
    }
    else if(MacroNameTok.getIdentifierInfo()->getName() == "TAU_PROPERTY")
    {
        tagType = TagType::Property;
        _propertyTags.emplace_back(Range);
    }
    else
    { return; }

    if(!Args || Args->isVarargsElidedUse()) // Are there 0 arguments/attributes.
    { return; }

    Ref<TagDeclaration> decl;
    if(tagType == TagType::Class)
    {
        decl = _classTags.back();
    }
    else if(tagType == TagType::Property)
    {
        decl = _propertyTags.back();
    }

    const ::clang::Token* token = Args->getUnexpArgument(0u);

    for(; token->isNot(::clang::tok::eof); ++token)
    {
        if(!token->isAnyIdentifier())
        {
            ::clang::DiagnosticsEngine& diagEngine = _pp.getDiagnostics();

            diagEngine.Report(token->getLocation(), _invalidAttributeDiagID)
                .AddString(token->getName());

            return;
        }

        const DynString attribName(token->getIdentifierInfo()->getName().str().c_str());

        Ref<IAttribute> attribHandler = AttributeManager::getAttribute(attribName);

        if(!attribHandler)
        {
            ::clang::DiagnosticsEngine& diagEngine = _pp.getDiagnostics();

            diagEngine.Report(token->getLocation(), _unknownAttributeDiagID)
                .AddString(token->getIdentifierInfo()->getName());

            return;
        }

        reportIncorrectAttribForTag(tagType, attribHandler, token, _pp.getDiagnostics(), _incorrectAttribForTagDiagID);

        const ::clang::Token* const preDigressionToken = token;

        AttributeData attribData = attribHandler->parseAttribute(attribName, Args, token);
        decl->addAttribute(::std::move(attribData));

        if(!token || token->is(::clang::tok::eof))
        { break; }
        
        if(token->isNot(::clang::tok::comma))
        {
            ::clang::DiagnosticsEngine& diagEngine = _pp.getDiagnostics();

            diagEngine.Report(token->getLocation(), _badAttributeHandlerDiagID)
                .AddString(preDigressionToken->getIdentifierInfo()->getName());

            return;
        }
    }
}

void TagPreProcessorCallback::initDiagnostics() noexcept
{
    ::clang::DiagnosticsEngine& diagEngine = _pp.getDiagnostics();

    _invalidAttributeDiagID = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Invalid token of type '%0' in attributes. Starting token must always be an identifier.");
    _unknownAttributeDiagID = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Unknown attribute '%0'.");
    _badAttributeHandlerDiagID = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Attribute '%0' may have a bad handler. The token following an attribute sequence must either be a ',' or the end of the varargs list.");
    _incorrectAttribForTagDiagID = diagEngine.getCustomDiagID(::clang::DiagnosticsEngine::Error, "Attribute '%0' cannot be applied to a %select{class|property|function}1. It can only be applied to a %select{class|property|function|class or property|class or function|property or function|_error_}2.");
}

static ::std::intptr_t buildValidAttribType(const Ref<IAttribute>& attribHandler) noexcept
{
    enum class ValidAttribType
    {
        Class = 0,
        Property,
        Function,
        ClassOrProperty,
        ClassOrFunction,
        PropertyOrFunction,
        _Error_
    };

    if(attribHandler->isForClass())
    {
        if(attribHandler->isForProperty())
        {
            return static_cast<::std::intptr_t>(ValidAttribType::ClassOrProperty);
        }
        else if(attribHandler->isForFunction())
        {
            return static_cast<::std::intptr_t>(ValidAttribType::ClassOrFunction);
        }
        
        return static_cast<::std::intptr_t>(ValidAttribType::Class);
    }
    else if(attribHandler->isForProperty())
    {
        if(attribHandler->isForFunction())
        {
            return static_cast<::std::intptr_t>(ValidAttribType::PropertyOrFunction);
        }
        
        return static_cast<::std::intptr_t>(ValidAttribType::Property);
    }
    else if(attribHandler->isForFunction())
    {
        return static_cast<::std::intptr_t>(ValidAttribType::Function);
    }
    
    return static_cast<::std::intptr_t>(ValidAttribType::_Error_);
}

static void reportIncorrectAttribForTag(const TagType tagType, const Ref<IAttribute>& attribHandler, const ::clang::Token* const token, ::clang::DiagnosticsEngine& diagEngine, const unsigned diagID) noexcept
{
    if(tagType == TagType::Class && !attribHandler->isForClass())
    {
        const auto db = diagEngine.Report(token->getLocation(), diagID);
        db.AddString(token->getIdentifierInfo()->getName());
        db.AddTaggedVal(0, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
        db.AddTaggedVal(buildValidAttribType(attribHandler), ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }
    else if(tagType == TagType::Property && !attribHandler->isForProperty())
    {
        const auto db = diagEngine.Report(token->getLocation(), diagID);
        db.AddString(token->getIdentifierInfo()->getName());
        db.AddTaggedVal(1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
        db.AddTaggedVal(buildValidAttribType(attribHandler), ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }
    else if(tagType == TagType::Function && !attribHandler->isForFunction())
    {
        const auto db = diagEngine.Report(token->getLocation(), diagID);
        db.AddString(token->getIdentifierInfo()->getName());
        db.AddTaggedVal(1, ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
        db.AddTaggedVal(buildValidAttribType(attribHandler), ::clang::DiagnosticsEngine::ArgumentKind::ak_uint);
    }
}

}
