/**
 * @file 
 * Basic BNF elements.
 *
 * <num> ::= [0-9]                              // A single number
 * <alpha-lower> ::= [a-z]                      // A single lowercase letter from a to z
 * <alpha-upper> ::= [A-Z]                      // An single uppercase letter from A to Z
 * <alpha> ::= <alpha-lower> | <alpha-upper>    // An single letter from a-Z
 * <alpha-num> ::= <alpha> | <num>              // A single letter or a number
 * <alpha-lower-u> ::= <alpha-lower> | '_'      // A single lowercase letter or an underscore
 * <alpha-upper-u> ::= <alpha-upper> | '_'      // A single uppercase letter or an underscore
 * <alpha-u> ::= <alpha> | '_'                  // A single letter or underscore
 * <alpha-num-u> ::= <alpha-num> | '_'          // A single letter, number, or an underscore
 * <all-chars> ::= [^]                          // All possible characters
 * <ws-chars> ::= ' ' | '\t' | '\n' | '\r'      // One of the 4 whitespace characters
 * <non-ws-chars> ::= ^<ws-chars>               // All non-whitespace characters
 * <ws> ::= { <ws-chars> }                      // A string of whitespace characters
 * <opt-ws> ::= [ <ws> ]                        // An optional string of whitespace characters
 */

#include "codegen/StringTemplateParser.hpp"

#include <vector>

#include "codegen/ast/StringTemplateAST.hpp"
#include "codegen/ast/StringTemplateStringifyAST.hpp"
#include "codegen/ast/StringTemplateExprAST.hpp"
#include "codegen/ast/StringTemplateVarAST.hpp"
#include "codegen/ast/StringTemplateMemberExprAST.hpp"
#include "codegen/ast/StringTemplateBooleanExprAST.hpp"
#include "codegen/ast/StringTemplateFragmentAST.hpp"
#include "codegen/ast/StringTemplateLoopAST.hpp"
#include "codegen/ast/StringTemplateIfAST.hpp"

#include "llvm/Support/raw_ostream.h"


namespace tau { namespace codegen { namespace string {

StrongRef<ast::StringTemplateRootAST> Parser::parse() noexcept
{
    const StrongRef<ast::StringTemplateRootAST> root(DefaultTauAllocator::Instance(), nullptr);
    StrongRef<ast::StringTemplateSequenceAST> current = root;

    StrongRef<ast::StringTemplateBeginFragmentAST> currentFragment = nullptr;

    bool inFragment = false;

    while(true)
    {
        if(_lexer.getNextToken() == Token::EndOfFile)
        {
            break;
        }
        else if(_lexer.currentToken() == Token::TextBlock)
        {
            if(!inFragment)
            { continue; }

            StrongRef<ast::StringTemplateTextBlockAST> textBlock = parseTextBlock();
            if(!textBlock)
            { return nullptr; }

            current->next() = textBlock;
            textBlock->prev() = current;
            current = textBlock;
        }
        else if(_lexer.currentToken() == Token::Character && _lexer.charValue() == _escapeChar)
        {
            if(_lexer.getNextToken() == Token::Character)
            {
                if(_lexer.charValue() == '<')
                {
                    StrongRef<ast::StringTemplateSequenceAST> ast;

                    switch(_lexer.getNextToken())
                    {
                        case Token::BeginFragment:
                        {
                            currentFragment = parseBeginFragment();
                            ast = currentFragment;

                            inFragment = true;

                            break;
                        }
                        case Token::EndFragment:
                        {
                            StrongRef<ast::StringTemplateEndFragmentAST> endFragment = parseEndFragment();
                            if(!endFragment)
                            { return nullptr; }

                            currentFragment->end() = endFragment;
                            endFragment->begin() = currentFragment;

                            currentFragment = nullptr;

                            ast = endFragment;

                            inFragment = false;

                            break;
                        }
                        case Token::BeginLoop:
                        {
                            ast = parseBeginLoop();
                            break;
                        }
                        case Token::EndLoop:
                        {
                            ast = parseEndLoop();
                            break;
                        }
                        case Token::Continue:
                        {
                            ast = parseLoopControl(ast::LoopControlType::Continue);
                            break;
                        }
                        case Token::Break:
                        {
                            ast = parseLoopControl(ast::LoopControlType::Break);
                            break;
                        }
                        case Token::LoopIndex:
                        {
                            ast = parseLoopControl(ast::LoopControlType::LoopIndex);
                            break;
                        }
                        case Token::BeginIf:
                        {
                            ast = parseIf();
                            break;
                        }
                        case Token::ElseIf:
                        {
                            ast = parseElseIf();
                            break;
                        }
                        case Token::Else:
                        {
                            ast = parseElse();
                            break;
                        }
                        case Token::EndIf:
                        {
                            ast = parseEndIf();
                            break;
                        }
                        case Token::Stringify:
                        {
                            ast = parseStringify();
                            break;
                        }
                        default: break;
                    }

                    if(!ast)
                    { return nullptr; }

                    current->next() = ast;
                    ast->prev() = current;
                    current = ast;
                }
                else if(_lexer.charValue() == '(')
                {
                    StrongRef<ast::StringTemplateExprAST> expr = parseVarRetrieveExpression();

                    if(_lexer.currentToken() != Token::Character)
                    { return nullptr; }

                    if(_lexer.charValue() != ')')
                    { return nullptr; }

                    _lexer.inControlSequence() = false;

                    if(!expr)
                    { return nullptr; }

                    StrongRef<ast::StringTemplateWildExprAST> ast(nullptr, nullptr, expr);

                    current->next() = ast;
                    ast->prev() = current;
                    current = ast;
                }
            }
            else if(_lexer.currentToken() == Token::Identifier)
            {
                StrongRef<ast::StringTemplateExprAST> expr = parseVarRetrieveExpression();

                _lexer.inControlSequence() = false;

                if(!expr)
                { return nullptr; }
                
                StrongRef<ast::StringTemplateWildExprAST> ast(nullptr, nullptr, expr);

                current->next() = ast;
                ast->prev() = current;
                current = ast;
            }
        }
    }

    return root;
}

StrongRef<ast::StringTemplateTextBlockAST> Parser::parseTextBlock() noexcept
{
    llvm::outs() << "Parsing text block: \n" << _lexer.strValue() << "\n";

    return StrongRef<ast::StringTemplateTextBlockAST>(nullptr, nullptr, _lexer.strValue());
}

/**
 * <begin-fragment> ::= '%' <opt-ws> '<' <opt-ws> "fragment" <opt-ws> '>' <opt-ws> '(' <opt-ws> <identifier> <opt-ws> ')'
 *
 * %<fragment>(FragmentName)
 */
StrongRef<ast::StringTemplateBeginFragmentAST> Parser::parseBeginFragment() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '(')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Identifier)
    { return nullptr; }

    DynString fragmentName = _lexer.identifierValue();

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != ')')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    llvm::outs() << "Parsing begin fragment: " << fragmentName << "\n";

    return StrongRef<ast::StringTemplateBeginFragmentAST>(nullptr, nullptr, ::std::move(fragmentName), nullptr);
}

/**
 * <end-fragment> ::= '%' <opt-ws> '<' <opt-ws> "endfragment" <opt-ws> '>'
 *
 * %<endfragment>
 */
StrongRef<ast::StringTemplateEndFragmentAST> Parser::parseEndFragment() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    _lexer.inControlSequence() = false;
    
    llvm::outs() << "Parsing end fragment\n";

    return StrongRef<ast::StringTemplateEndFragmentAST>(nullptr, nullptr, nullptr);
}

/**
 * <begin-loop> ::= '%' <opt-ws> '<' <opt-ws> "loop" <opt-ws> '>' <opt-ws> '(' <opt-ws> <var-decl> <opt-ws> ':' <opt-ws> <expression> <opt-ws> ')'
 *
 * %<loop>(%var : %iterator)
 */
StrongRef<ast::StringTemplateBeginLoopAST> Parser::parseBeginLoop() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '(')
    { return nullptr; }
    
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != _escapeChar)
    { return nullptr; }
    
    if(_lexer.getNextToken() != Token::Identifier)
    { return nullptr; }

    llvm::outs() << "Parsing begin loop\n";

    StrongRef<ast::StringTemplateVarDeclAST> varDecl = parseVarDecl();

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != ':')
    { return nullptr; }

    // Prime token for expression parsing.
    if(_lexer.getNextToken() == Token::EndOfFile)
    { return nullptr; }

    StrongRef<ast::StringTemplateExprAST> rangeExpr = parseExpression();

    // if(_lexer.getNextToken() != Token::Character)
    // { return nullptr; }

    if(_lexer.charValue() != ')')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    return StrongRef<ast::StringTemplateBeginLoopAST>(nullptr, nullptr, ::std::move(varDecl), ::std::move(rangeExpr), nullptr);
}

/**
 * <end-loop> ::= '%' <opt-ws> '<' <opt-ws> "endloop" <opt-ws> '>'
 *
 * %<endloop>
 */
StrongRef<ast::StringTemplateEndLoopAST> Parser::parseEndLoop() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    llvm::outs() << "Parsing end loop\n";

    return StrongRef<ast::StringTemplateEndLoopAST>(nullptr, nullptr, nullptr);
}

/**
 * <continue> ::= '%' <opt-ws> '<' <opt-ws> "continue" <opt-ws> '>' <opt-ws> '(' <opt-ws> <int> <opt-ws> ')'
 * <break> ::= '%' <opt-ws> '<' <opt-ws> "break" <opt-ws> '>' <opt-ws> '(' <opt-ws> <int> <opt-ws> ')'
 * <loopindex> ::= '%' <opt-ws> '<' <opt-ws> "loopindex" <opt-ws> '>' <opt-ws> '(' <opt-ws> <int> <opt-ws> ')'
 *
 * %<continue>(0)
 * %<break>(0)
 * %<loopindex>(0)
 */
StrongRef<ast::StringTemplateLoopControlAST> Parser::parseLoopControl(const ast::LoopControlType controlType) noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '(')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::IntegerLiteral)
    { return nullptr; }

    const u32 loopIndex = _lexer.intValue();

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != ')')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    switch(controlType)
    {
        case ast::LoopControlType::Continue:
            llvm::outs() << "Parsing continue: " << loopIndex << "\n";
            break;
        case ast::LoopControlType::Break:
            llvm::outs() << "Parsing break: " << loopIndex << "\n";
            break;
        case ast::LoopControlType::LoopIndex:
            llvm::outs() << "Parsing loop index: " << loopIndex << "\n";
            break;
    }

    return StrongRef<ast::StringTemplateLoopControlAST>(nullptr, nullptr, nullptr, controlType, loopIndex);
}

/**
 * <begin-if> ::= '%' <opt-ws> '<' <opt-ws> "if" <opt-ws> '>' <opt-ws> '(' <opt-ws> <expression> <opt-ws> ')'
 *
 * %<if>(%expr)
 */
StrongRef<ast::StringTemplateBeginIfAST> Parser::parseIf() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '(')
    { return nullptr; }

    // Prime token for expression parsing.
    if(_lexer.getNextToken() == Token::EndOfFile)
    { return nullptr; }

    llvm::outs() << "Parsing if\n";

    StrongRef<ast::StringTemplateExprAST> controlExpr = parseExpression();

    if(!controlExpr)
    { return nullptr; }

    if(_lexer.currentToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != ')')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    return StrongRef<ast::StringTemplateBeginIfAST>(nullptr, nullptr, ::std::move(controlExpr), nullptr);
}

/**
 * <else-if> ::= '%' <opt-ws> '<' <opt-ws> "elif" <opt-ws> '>' <opt-ws> '(' <opt-ws> <expression> <opt-ws> ')'
 *
 * %<elif>(%expr)
 */
StrongRef<ast::StringTemplateElseIfAST> Parser::parseElseIf() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '(')
    { return nullptr; }

    // Prime token for expression parsing.
    if(_lexer.getNextToken() == Token::EndOfFile)
    { return nullptr; }

    llvm::outs() << "Parsing else if\n";

    StrongRef<ast::StringTemplateExprAST> controlExpr = parseExpression();

    if(!controlExpr)
    { return nullptr; }

    if(_lexer.currentToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != ')')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    return StrongRef<ast::StringTemplateElseIfAST>(nullptr, nullptr, ::std::move(controlExpr), nullptr, nullptr);
}

/**
 * <else> ::= '%' <opt-ws> '<' <opt-ws> "else" <opt-ws> '>'
 *
 * %<else>
 */
StrongRef<ast::StringTemplateElseAST> Parser::parseElse() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    llvm::outs() << "Parsing else\n";

    return StrongRef<ast::StringTemplateElseAST>(nullptr, nullptr, nullptr, nullptr);
}

/**
 * <end-if> ::= '%' <opt-ws> '<' <opt-ws> "endif" <opt-ws> '>'
 *
 * %<endif>
 */
StrongRef<ast::StringTemplateEndIfAST> Parser::parseEndIf() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    llvm::outs() << "Parsing end if\n";

    return StrongRef<ast::StringTemplateEndIfAST>(nullptr, nullptr, nullptr);
}

/**
 * <stringify> ::= '%' <opt-ws> '<' <opt-ws> "str" <opt-ws> '>' <opt-ws> '(' <opt-ws> <expression> <opt-ws> ')'
 *
 * %<str>(%expr)
 */
StrongRef<ast::StringTemplateStringifyAST> Parser::parseStringify() noexcept
{
    llvm::outs() << "Parsing stringify\n";

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '(')
    { return nullptr; }
    
    // Prime token for expression parsing.
    if(_lexer.getNextToken() == Token::EndOfFile)
    { return nullptr; }

    StrongRef<ast::StringTemplateExprAST> expr = parseExpression();

    if(_lexer.currentToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != ')')
    { return nullptr; }

    _lexer.inControlSequence() = false;

    return StrongRef<ast::StringTemplateStringifyAST>(nullptr, nullptr, expr);
}

/**
 * <var-decl> ::= '%' <opt-ws> [ '(' <opt-ws> ] <identifier> [ <opt-ws> ')' ]
 *
 * %var
 * %(var)
 */
StrongRef<ast::StringTemplateVarDeclAST> Parser::parseVarDecl() noexcept
{
    llvm::outs() << "Parsing var decl: " << _lexer.identifierValue() << "\n";

    return StrongRef<ast::StringTemplateVarDeclAST>(nullptr, nullptr, _lexer.identifierValue());
}

StrongRef<ast::StringTemplateExprAST> Parser::parseExpression() noexcept
{
    llvm::outs() << "Parsing expression\n";

    if(_lexer.currentToken() == Token::StringLiteral)
    {
        return parseStringExpression();
    }
    else if(_lexer.currentToken() == Token::IntegerLiteral)
    {
        return parseIntegerExpression();
    }
    else if(_lexer.currentToken() == Token::Character)
    {
        if(_lexer.charValue() == _escapeChar)
        {
            return parseVarRetrieveExpression();
        }
        else if(_lexer.charValue() == '!')
        {
            return parseBooleanInvert();
        }
    }

    return nullptr;
}

/**
 * <string> ::= '"' [ { <all-chars> } ] '"'
 *
 * "string"
 */
StrongRef<ast::StringTemplateStringExprAST> Parser::parseStringExpression() noexcept
{
    llvm::outs() << "Parsing string expression: \"" << _lexer.strValue() << "\"\n";

    return StrongRef<ast::StringTemplateStringExprAST>(_lexer.strValue());
}

/**
 * <int> ::= [ '-' | '+' ] { <num> }
 *
 * 13
 * -13
 * +13
 */
StrongRef<ast::StringTemplateIntegerExprAST> Parser::parseIntegerExpression() noexcept
{
    llvm::outs() << "Parsing integer: " << _lexer.intValue() << "\n";

    return StrongRef<ast::StringTemplateIntegerExprAST>(_lexer.intValue());
}

StrongRef<ast::StringTemplateVarAssignExprAST> Parser::parseVarAssignExpression() noexcept
{
    DynString varName = _lexer.identifierValue();

    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '=')
    { return nullptr; }

    // Prime token for expression parsing.
    if(_lexer.getNextToken() == Token::EndOfFile)
    { return nullptr; }

    StrongRef<ast::StringTemplateExprAST> assignmentExpr = parseExpression();

    llvm::outs() << "Parsing var assign: " << _lexer.identifierValue() << "\n";

    return StrongRef<ast::StringTemplateVarAssignExprAST>(::std::move(varName), ::std::move(assignmentExpr));
}

StrongRef<ast::StringTemplateVarRetrieveExprAST> Parser::parseVarRetrieveExpression() noexcept
{
    if(_lexer.getNextToken() != Token::Identifier)
    { return nullptr; }

    llvm::outs() << "Parsing var retrieve: " << _lexer.identifierValue() << "\n";

    StrongRef<ast::StringTemplateVarRetrieveExprAST> varRetrieveExpr(_lexer.identifierValue());

    if(_lexer.getNextToken() == Token::Character)
    {
        if(_lexer.charValue() == '.')
        {
            return parseMemberExpression(varRetrieveExpr);
        }
    }

    return varRetrieveExpr;
}

StrongRef<ast::StringTemplateExprAST> Parser::parseMemberExpression(StrongRef<ast::StringTemplateExprAST> objectExpr) noexcept
{
    if(_lexer.getNextToken() != Token::Identifier)
    { return nullptr; }

    llvm::outs() << "Parsing member: " << _lexer.identifierValue() << "\n";

    DynString memberName = _lexer.identifierValue();

    if(_lexer.getNextToken() == Token::Character)
    {
        if(_lexer.charValue() == '(')
        {
            StrongRef<ast::StringTemplateMethodAccessExprAST> methodAccessExpr = parseMethodAccessExpression(objectExpr, ::std::move(memberName));

            if(_lexer.getNextToken() == Token::Character)
            {
                if(_lexer.charValue() == '.')
                {
                    return parseMemberExpression(::std::move(methodAccessExpr));
                }
            }

            return ::std::move(methodAccessExpr);
        }
    }

    StrongRef<ast::StringTemplateMemberAccessExprAST> memberAccessExpr(objectExpr, ::std::move(memberName));

    if(_lexer.currentToken() == Token::Character)
    {
        if(_lexer.charValue() == '.')
        {
            return parseMemberExpression(::std::move(memberAccessExpr));
        }
    }

    return ::std::move(memberAccessExpr);
}

StrongRef<ast::StringTemplateMethodAccessExprAST> Parser::parseMethodAccessExpression(StrongRef<ast::StringTemplateExprAST>& objectExpr, DynString&& methodName) noexcept
{
    StrongRef<ast::StringTemplateMethodAccessExprAST> methodAst(objectExpr, ::std::move(methodName));

    llvm::outs() << "Parsing method access expression.\n";
    
    if(_lexer.getNextToken() == Token::Character)
    {
        if(_lexer.charValue() == ')')
        {
            return methodAst;
        }
    }

    llvm::outs() << "Parsing method args\n";

    while(true)
    {
        StrongRef<ast::StringTemplateExprAST> expr = parseExpression();
        if(!expr)
        { return nullptr; }

        methodAst->arguments().push_back(::std::move(expr));

        if(_lexer.getNextToken() != Token::Character)
        { return nullptr; }

        if(_lexer.charValue() == ')')
        {
            return methodAst;
        }
        else if(_lexer.charValue() != ',')
        { return nullptr; } 
    }
}

StrongRef<ast::StringTemplateBooleanInvertExprAST> Parser::parseBooleanInvert() noexcept
{
    llvm::outs() << "Parsing boolean invert\n";

    // Prime token for expression parsing.
    if(_lexer.getNextToken() == Token::EndOfFile)
    { return nullptr; }

    StrongRef<ast::StringTemplateExprAST> expr = parseExpression();

    if(!expr)
    { return nullptr; }

    return StrongRef<ast::StringTemplateBooleanInvertExprAST>(::std::move(expr));
}

} } }
