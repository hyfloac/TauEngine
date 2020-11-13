#include "codegen/StringTemplateParser.hpp"
#include "codegen/ast/StringTemplateAST.hpp"
#include "codegen/ast/StringTemplateVarAST.hpp"
#include "codegen/ast/StringTemplateFragmentAST.hpp"
#include "codegen/ast/StringTemplateLoopAST.hpp"
#include "codegen/ast/StringTemplateIfAST.hpp"

namespace tau { namespace codegen { namespace string {

StrongRef<ast::StringTemplateRootAST> Parser::parse() noexcept
{
    const StrongRef<ast::StringTemplateRootAST> root(nullptr, nullptr);
    StrongRef<ast::StringTemplateAST> current = root;

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
            if(_lexer.getNextToken() == Token::Character && _lexer.charValue() == '<')
            {
                if(_lexer.getNextToken() == Token::BeginFragment)
                {
                    currentFragment = parseBeginFragment();
                    if(!currentFragment)
                    { return nullptr; }

                    current->next() = currentFragment;
                    currentFragment->prev() = current;
                    current = currentFragment;

                    inFragment = true;
                }
                else if(_lexer.currentToken() == Token::EndFragment)
                {
                    StrongRef<ast::StringTemplateEndFragmentAST> endFragment = parseEndFragment();
                    if(!endFragment)
                    { return nullptr; }

                    currentFragment->end() = endFragment;
                    endFragment->begin() = currentFragment;

                    currentFragment = nullptr;

                    current->next() = endFragment;
                    endFragment->prev() = current;
                    current = endFragment;
                    inFragment = false;
                }
                else if(_lexer.currentToken() == Token::Continue)
                {
                    StrongRef<ast::StringTemplateLoopControlAST> loopControl = parseLoopControl(ast::LoopControlType::Continue);
                    if(!loopControl)
                    { return nullptr; }

                    current->next() = loopControl;
                    loopControl->prev() = current;
                    current = loopControl;
                }
                else if(_lexer.currentToken() == Token::Break)
                {
                    StrongRef<ast::StringTemplateLoopControlAST> loopControl = parseLoopControl(ast::LoopControlType::Break);
                    if(!loopControl)
                    { return nullptr; }

                    current->next() = loopControl;
                    loopControl->prev() = current;
                    current = loopControl;
                }
                else if(_lexer.currentToken() == Token::LoopIndex)
                {
                    StrongRef<ast::StringTemplateLoopControlAST> loopControl = parseLoopControl(ast::LoopControlType::LoopIndex);
                    if(!loopControl)
                    { return nullptr; }

                    current->next() = loopControl;
                    loopControl->prev() = current;
                    current = loopControl;
                }
            }
        }
    }

    return root;
}

StrongRef<ast::StringTemplateTextBlockAST> Parser::parseTextBlock() noexcept
{
    return StrongRef<ast::StringTemplateTextBlockAST>(nullptr, nullptr, _lexer.strValue());
}

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

    return StrongRef<ast::StringTemplateBeginFragmentAST>(nullptr, nullptr, ::std::move(fragmentName), nullptr);
}

StrongRef<ast::StringTemplateEndFragmentAST> Parser::parseEndFragment() noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
    { return nullptr; }

    return StrongRef<ast::StringTemplateEndFragmentAST>(nullptr, nullptr, nullptr);
}

StrongRef<ast::StringTemplateLoopControlAST> Parser::parseLoopControl(const ast::LoopControlType controlType) noexcept
{
    if(_lexer.getNextToken() != Token::Character)
    { return nullptr; }

    if(_lexer.charValue() != '>')
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

    return StrongRef<ast::StringTemplateLoopControlAST>(nullptr, nullptr, nullptr, controlType, loopIndex);
}

StrongRef<ast::StringTemplateExprAST> Parser::parseExpression() noexcept
{
    if(_lexer.getNextToken() == Token::StringLiteral)
    {
        return parseStringExpression();
    }
    else if(_lexer.currentToken() == Token::IntegerLiteral)
    {
        return parseIntegerExpression();
    }

    return nullptr;
}

StrongRef<ast::StringTemplateStringExprAST> Parser::parseStringExpression() noexcept
{
    return StrongRef<ast::StringTemplateStringExprAST>(nullptr, nullptr, _lexer.strValue());
}

StrongRef<ast::StringTemplateIntegerExprAST> Parser::parseIntegerExpression() noexcept
{
    return StrongRef<ast::StringTemplateIntegerExprAST>(nullptr, nullptr, _lexer.intValue());
}

} } }
