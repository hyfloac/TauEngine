#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>

#include "reflection/processing/FrontendFactoryHelper.hpp"
#include "reflection/processing/ReflectionASTWalker.hpp"
#include "reflection/processing/TagPreProcessor.hpp"
#include "reflection/processing/HeaderGenerator.hpp"
#include "reflection/attribs/GetAttribute.hpp"
#include "reflection/attribs/SetAttribute.hpp"
#include "reflection/attribs/ImplicitAttribute.hpp"
#include "reflection/attribs/NoListAttribute.hpp"

#include "codegen/StringTemplateLexer.hpp"
#include <fstream>

static ::llvm::cl::OptionCategory tauReflCategory("tau-reflection-generator options");

static ::llvm::cl::opt<::std::string> baseHeaderLoc("base-loc", ::llvm::cl::desc("The path to where the base header should be placed"), ::llvm::cl::cat(tauReflCategory));
static ::llvm::cl::opt<::std::string> outHeader("o", ::llvm::cl::desc("The path and name of the output file"), ::llvm::cl::cat(tauReflCategory));

static void dumpTokens(::std::istream& file) noexcept;

int main(int argCount, const char* args[])
{
    static ::llvm::cl::extrahelp commonHelp(::clang::tooling::CommonOptionsParser::HelpMessage);
    UNUSED(commonHelp);


    // {
    //     ::std::fstream file(R"(D:\TauEngine\concepts\StringTemplateExample.template)");
    //     dumpTokens(file);
    // }

    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::ImplicitAttribute>("__implicit_base_0__");
    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::NoListAttribute>("nolist");
    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::GetPropertyAttribute>("get");
    tau::reflection::AttributeManager::registerAttribute<tau::reflection::attribs::SetPropertyAttribute>("set");

    ::clang::tooling::CommonOptionsParser op(argCount, args, tauReflCategory);

    if(!baseHeaderLoc.getValue().empty())
    {
        StringBuilder pathBuilder(baseHeaderLoc.getValue().length() + 9);
        pathBuilder.append(baseHeaderLoc.getValue().c_str());

        if(pathBuilder[pathBuilder.length() - 1] != '\\' && pathBuilder[pathBuilder.length() - 1] != '/')
        { pathBuilder.append('/'); }

        pathBuilder.append("Base.hpp");

        tau::reflection::processing::BaseGenerator baseGen(pathBuilder.toString());
        baseGen.generate();
    }

    if(outHeader.getValue().empty())
    { return 0; }

    {
        tau::reflection::processing::HeaderGenerator headerGen(outHeader.getValue().c_str());
        headerGen.generateBegin();
        headerGen.generateDummy();
    }

    ::clang::tooling::ClangTool tool(op.getCompilations(), op.getSourcePathList());

    tau::reflection::TagDeclQueue classTags;
    tau::reflection::TagDeclQueue propertyTags;
    tau::reflection::TagDeclQueue functionTags;
    tau::reflection::ClassList classes;

    int result = tool.run(::clang::helperExt::newFrontendActionFactory<::tau::reflection::processing::TagPreProcessorAction>(classTags, propertyTags, functionTags).get());
    if(result)
    { return result; }

    result = tool.run(::clang::helperExt::newFrontendActionFactory<::tau::reflection::processing::ReflectionASTWalkerAction>(classTags, propertyTags, functionTags, classes).get());
    if(result)
    { return result; }

    {
        tau::reflection::processing::HeaderGenerator headerGen(outHeader.getValue().c_str());
        headerGen.generateBegin();
        for(const auto& clazz : classes)
        {
            headerGen.generateClassBody(clazz);
        }
    }

    return result;
}

static void dumpTokens(::std::istream& file) noexcept
{
    using namespace tau::codegen::string;
    Lexer lexer(file);

    int parenDepth = 0;

    bool simpleCtrlIdent = false;

    do
    {
        switch(lexer.getNextToken())
        {
            case Token::Identifier:
                llvm::outs() << "Generic Identifier: " << lexer.identifierValue() << "\n";
                break;
            case Token::BeginFragment:
            case Token::EndFragment:
            case Token::BeginLoop:
            case Token::EndLoop:
            case Token::BeginIf:
            case Token::Else:
            case Token::ElseIf:
            case Token::EndIf:
            case Token::Continue:
            case Token::Break:
            case Token::LoopIndex:
            case Token::Stringify:
                llvm::outs() << "Control Identifier: " << lexer.identifierValue() << "\n";
                break;
            case Token::TextBlock:
                llvm::outs() << "Text Block:\n" << lexer.strValue() << "\n";
                break;
            case Token::Character:
                llvm::outs() << "Raw Character: " << lexer.charValue() << "\n";
                if(lexer.charValue() == '(')
                {
                    ++parenDepth;
                }
                else if(lexer.charValue() == ')')
                {
                    if(--parenDepth == 0)
                    {
                        lexer.inControlSequence() = false;
                    }
                }
                else if(lexer.charValue() == '>' && simpleCtrlIdent)
                {
                    lexer.inControlSequence() = false;
                }
                break;
            case Token::StringLiteral:
                llvm::outs() << "String Literal: \"" << lexer.strValue() << "\"\n";
                break;
            case Token::CharacterLiteral:
                llvm::outs() << "Character Literal: '" << lexer.charValue() << "'\n";
                break;
            case Token::IntegerLiteral:
                llvm::outs() << "Integer Literal: " << lexer.intValue() << "\n";
                break;
            default: break;
        }

        switch(lexer.currentToken())
        {
            case Token::EndFragment:
            case Token::EndLoop:
            case Token::EndIf:
                simpleCtrlIdent = true;
                break;
            case Token::BeginFragment:
            case Token::BeginLoop:
            case Token::BeginIf:
            case Token::Else:
            case Token::ElseIf:
            case Token::Continue:
            case Token::Break:
            case Token::LoopIndex:
            case Token::Stringify:
                simpleCtrlIdent = false;
                break;
            default: break;
        }

    } while(lexer.currentToken() != Token::EndOfFile && lexer.currentToken() != Token::Unknown);

    if(lexer.currentToken() == Token::Unknown)
    {
        llvm::outs() << "Unknown Token Encountered.\n";
    }
    else
    {
        llvm::outs() << "End of file.\n";
    }
}
