#include "reflection/processing/TagPreProcessor.hpp"
#include <clang/Lex/Token.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Lex/MacroInfo.h>
#include <clang/Lex/MacroArgs.h>
#include <clang/Frontend/CompilerInstance.h>

namespace tau { namespace reflection { namespace processing { 

void TagPreProcessorAction::ExecuteAction()
{
    ::clang::Preprocessor& pp = getCompilerInstance().getPreprocessor();

    pp.addPPCallbacks(::std::make_unique<TagPreProcessorCallback>(pp, _classTags, _propertyTags, _functionTags));

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

static void reportIncorrectAttribForTag(TagType tagType, const Ref<IAttribute>& attribHandler, const ::clang::Token* token, ::clang::DiagnosticsEngine& diagEngine, unsigned diagID) noexcept;

void TagPreProcessorCallback::MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition&, const clang::SourceRange Range, const clang::MacroArgs* const Args)
{
    // Macro is not named for some reason.
    if(!MacroNameTok.isAnyIdentifier())
    { return; }
    
    /*   Check if the macro is one of our reflection types and then
       add the declaration to its queue. */
    Ref<TagDeclaration> decl;
    TagType tagType;
    if(MacroNameTok.getIdentifierInfo()->getName() == "TAU_CLASS")
    {
        tagType = TagType::Class;
        _classTags.emplace_back(Range);
        decl = _classTags.back();
    }
    else if(MacroNameTok.getIdentifierInfo()->getName() == "TAU_PROPERTY")
    {
        tagType = TagType::Property;
        _propertyTags.emplace_back(Range);
        decl = _propertyTags.back();
    }
    else if(MacroNameTok.getIdentifierInfo()->getName() == "TAU_FUNCTION")
    {
        tagType = TagType::Function;
        _functionTags.emplace_back(Range);
        decl = _functionTags.back();
    }
    else
    { return; }

    // Are there 0 arguments/attributes.
    if(!Args || Args->isVarargsElidedUse()) 
    { return; }

    /*   Get the argument list for this macro, because it's a
       varargs function all arguments are stored in argument 0. */
    const ::clang::Token* token = Args->getUnexpArgument(0u);

    // All tokens for the argument are stored in sequence.
    for(; token->isNot(::clang::tok::eof); ++token)
    {
        // The beginning to every argument must be a name.
        if(!token->isAnyIdentifier())
        {
            ::clang::DiagnosticsEngine& diagEngine = _pp.getDiagnostics();

            diagEngine.Report(token->getLocation(), _invalidAttributeDiagID)
                .AddString(token->getName());

            return;
        }

        const DynString attribName(token->getIdentifierInfo()->getName().str().c_str());

        Ref<IAttribute> attribHandler = AttributeManager::getAttribute(attribName);

        // Attribute was not registered.
        if(!attribHandler)
        {
            ::clang::DiagnosticsEngine& diagEngine = _pp.getDiagnostics();

            diagEngine.Report(token->getLocation(), _unknownAttributeDiagID)
                .AddString(token->getIdentifierInfo()->getName());

            return;
        }

        /*   Check to see if the attribute is applicable to this
          reflection type (class, property, function) and report an
          error otherwise. */
        reportIncorrectAttribForTag(tagType, attribHandler, token, _pp.getDiagnostics(), _incorrectAttribForTagDiagID);

        /*   parseAttribute takes a reference to the token and
           increments during its processing procedure. */
        const ::clang::Token* const preDigressionToken = token;

        AttributeData attribData = attribHandler->parseAttribute(attribName, Args, token);
        decl->addAttribute(::std::move(attribData));

        // Exit if this was the final argument.
        if(!token || token->is(::clang::tok::eof))
        { break; }

        /*   If this was not the final argument ensure that it is
           part of a comma delimited list. */
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

} } }
