#pragma once

#include <Safeties.hpp>
#include "StringTemplateLexer.hpp"

namespace tau { namespace codegen { namespace string { 

namespace ast {

class StringTemplateAST;
class StringTemplateRootAST;
class StringTemplateExprAST;

class StringTemplateStringExprAST;
class StringTemplateIntegerExprAST;
class StringTemplateVarAssignExprAST;
class StringTemplateVarRetrieveExprAST;
class StringTemplateMemberAccessExprAST;
class StringTemplateMethodAccessExprAST;

class StringTemplateTextBlockAST;
class StringTemplateStringifyAST;
class StringTemplateVarDeclAST;
class StringTemplateBeginFragmentAST;
class StringTemplateEndFragmentAST;
class StringTemplateBeginLoopAST;
class StringTemplateEndLoopAST;
class StringTemplateLoopControlAST;
class StringTemplateBeginIfAST;
class StringTemplateElseIfAST;
class StringTemplateElseAST;
class StringTemplateEndIfAST;

enum class LoopControlType;

}

class Parser final
{
private:
    char _escapeChar;
    Lexer _lexer;
    StrongRef<ast::StringTemplateAST> _ast;
public:
    Parser(::std::istream& file, const char escapeChar = '%') noexcept
        : _escapeChar(escapeChar)
        , _lexer(file, escapeChar)
    { }

    StrongRef<ast::StringTemplateRootAST> parse() noexcept;
private:
    StrongRef<ast::StringTemplateTextBlockAST> parseTextBlock() noexcept;

    StrongRef<ast::StringTemplateBeginFragmentAST> parseBeginFragment() noexcept;
    StrongRef<ast::StringTemplateEndFragmentAST> parseEndFragment() noexcept;

    StrongRef<ast::StringTemplateLoopControlAST> parseLoopControl(ast::LoopControlType controlType) noexcept;

    StrongRef<ast::StringTemplateExprAST> parseExpression() noexcept;
    
    StrongRef<ast::StringTemplateStringExprAST> parseStringExpression() noexcept;
    StrongRef<ast::StringTemplateIntegerExprAST> parseIntegerExpression() noexcept;
};

} } }
