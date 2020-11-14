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
class StringTemplateBooleanInvertExprAST;

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

    StrongRef<ast::StringTemplateBeginLoopAST> parseBeginLoop() noexcept;
    StrongRef<ast::StringTemplateEndLoopAST> parseEndLoop() noexcept;

    StrongRef<ast::StringTemplateLoopControlAST> parseLoopControl(ast::LoopControlType controlType) noexcept;

    StrongRef<ast::StringTemplateBeginIfAST> parseIf() noexcept;
    StrongRef<ast::StringTemplateElseIfAST> parseElseIf() noexcept;
    StrongRef<ast::StringTemplateElseAST> parseElse() noexcept;
    StrongRef<ast::StringTemplateEndIfAST> parseEndIf() noexcept;

    StrongRef<ast::StringTemplateStringifyAST> parseStringify() noexcept;

    StrongRef<ast::StringTemplateVarDeclAST> parseVarDecl() noexcept;

    StrongRef<ast::StringTemplateExprAST> parseExpression() noexcept;
    
    StrongRef<ast::StringTemplateStringExprAST> parseStringExpression() noexcept;
    StrongRef<ast::StringTemplateIntegerExprAST> parseIntegerExpression() noexcept;

    StrongRef<ast::StringTemplateVarAssignExprAST> parseVarAssignExpression() noexcept;
    StrongRef<ast::StringTemplateVarRetrieveExprAST> parseVarRetrieveExpression() noexcept;

    StrongRef<ast::StringTemplateExprAST> parseMemberExpression(StrongRef<ast::StringTemplateExprAST> objectExpr) noexcept;

    StrongRef<ast::StringTemplateMethodAccessExprAST> parseMethodAccessExpression(StrongRef<ast::StringTemplateExprAST>& objectExpr, DynString&& methodName) noexcept;

    StrongRef<ast::StringTemplateBooleanInvertExprAST> parseBooleanInvert() noexcept;
};

} } }
