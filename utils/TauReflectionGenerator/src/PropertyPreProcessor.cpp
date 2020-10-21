#include "PropertyPreProcessor.hpp"
#include <clang/Lex/Token.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Lex/MacroInfo.h>
#include <clang/Lex/MacroArgs.h>
#include <clang/Frontend/CompilerInstance.h>

namespace tau {

void PropertyPreProcessorAction::ExecuteAction()
{
    ::clang::Preprocessor& pp = getCompilerInstance().getPreprocessor();
    
    pp.addPPCallbacks(::std::make_unique<PropertyPreProcessorCallback>(_propertyTags));

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

void PropertyPreProcessorCallback::MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, const clang::SourceRange Range, const clang::MacroArgs* const Args)
{
    if(!MacroNameTok.isAnyIdentifier())
    { return; }

    if(MacroNameTok.getIdentifierInfo()->getName() != "TAU_PROPERTY")
    { return; }

    _propertyTags.emplace_back(DefaultTauAllocator::Instance(), Range);

    if(Args->isVarargsElidedUse()) // Are there 0 arguments/attributes.
    { return; }

    Ref<PropertyDeclaration>& propertyDecl = _propertyTags.back();

    const ::clang::Token* token = Args->getUnexpArgument(0u);

    for(; token->isNot(::clang::tok::eof); ++token)
    {
        if(!token->isAnyIdentifier())
        {
            llvm::errs() << "Token attributes are malformed.\n";
            return;
        }

        const DynString attribName(token->getIdentifierInfo()->getName().str().c_str());

        Ref<IPropertyAttribute> attributeHandler = PropertyAttributeManager::getAttribute(attribName);

        if(!attributeHandler)
        {
            llvm::errs() << "Unknown property attribute \"" << attribName << "\".\n";
            return;
        }

        PropertyAttributeData attribData = attributeHandler->parseAttribute(attribName, Args, token);
        propertyDecl->addAttribute(::std::move(attribData));

        if(!token || token->is(::clang::tok::eof))
        { break; }
        
        if(token->isNot(::clang::tok::comma))
        {
            llvm::errs() << "Property attribute \"" << attribName << "\" has a malformed handler.\n";
            return;
        }
    }
}

}
