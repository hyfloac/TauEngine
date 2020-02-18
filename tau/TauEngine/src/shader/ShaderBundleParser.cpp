#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"

NullableStrongRef<ExprAST> ShaderBundleParser::parse() noexcept
{
    _ast = NullableStrongRef<ExprAST>(DefaultTauAllocator::Instance(), null);
    NullableStrongRef<ExprAST> curr = _ast;
    while(true)
    {
        const NullableStrongRef<ExprAST> x = parsePrimary();
        curr->next() = x;
        curr = curr->next();
        if(_lexer.getNextToken() == SBPToken::EndOfFile)
        { return _ast; }
        else if(_lexer.currentToken() != SBPToken::Character || _lexer.cValue() != ',')
        { return null; }
    }
}

NullableStrongRef<ExprAST> ShaderBundleParser::parsePrimary() noexcept
{
    const SBPToken token = _lexer.getNextToken();
    if(token == SBPToken::EndOfFile)
    {
        return _ast;
    }

    switch(token)
    {
        case SBPToken::Identifier:
        case SBPToken::VertexBlock:
        case SBPToken::TessellationControlBlock:
        case SBPToken::TessellationEvaluationBlock:
        case SBPToken::GeometryBlock:
        case SBPToken::PixelBlock:
        case SBPToken::UniformsBlock:
        case SBPToken::TexturesBlock:
            return RCPCast<ExprAST>(parseBlock());
        case SBPToken::File:
            return RCPCast<ExprAST>(parseFile());
        case SBPToken::IntegerLiteral:
            return RCPCast<ExprAST>(parseIOPoint());
        default: return null;
    }
}

NullableStrongRef<BlockExprAST> ShaderBundleParser::parseBlock() noexcept
{
    BlockType blockType;

    switch(_lexer.currentToken())
    {
        case SBPToken::VertexBlock:
            blockType = BlockType::Vertex;
            break;
        case SBPToken::TessellationControlBlock:
            blockType = BlockType::TessellationControl;
            break;
        case SBPToken::TessellationEvaluationBlock:
            blockType = BlockType::TessellationEvaluation;
            break;
        case SBPToken::GeometryBlock:
            blockType = BlockType::Geometry;
            break;
        case SBPToken::PixelBlock:
            blockType = BlockType::Pixel;
            break;
        case SBPToken::UniformsBlock:
            blockType = BlockType::Uniforms;
            break;
        case SBPToken::TexturesBlock:
            blockType = BlockType::Textures;
            break;
        case SBPToken::Identifier:
        {
            const NullableStrongRef<NamedBlockExprAST> namedBlock(DefaultTauAllocator::Instance(), null, null, _lexer.identifierValue());
            parseNextBlockEntry(RCPCast<BlockExprAST>(namedBlock));
            return RCPCast<BlockExprAST>(namedBlock);
        }
        default: return null;
    }

    const NullableStrongRef<TypedBlockExprAST> typedBlock(DefaultTauAllocator::Instance(), null, null, blockType);
    parseNextBlockEntry(RCPCast<BlockExprAST>(typedBlock));
    return RCPCast<BlockExprAST>(typedBlock);
}

void ShaderBundleParser::parseNextBlockEntry(NullableStrongRef<BlockExprAST> block) noexcept
{
    if(_lexer.getNextToken() != SBPToken::Character)
    { return; }
    if(_lexer.cValue() != ':')
    { return; }
    if(_lexer.getNextToken() != SBPToken::Character)
    { return; }
    if(_lexer.cValue() != '{')
    { return; }

    block->container() = parsePrimary();


    NullableStrongRef<ExprAST> expr = RCPCast<ExprAST>(block->container());

    while(true)
    {
        if(_lexer.getNextToken() != SBPToken::Character)
        { return; }
        if(_lexer.cValue() == '}')
        { return; }
        if(_lexer.cValue() != ',')
        { return; }
        expr->next() = parsePrimary();
        expr = expr->next();
    }
}

NullableStrongRef<FileExprAST> ShaderBundleParser::parseFile() noexcept
{
    if(_lexer.getNextToken() != SBPToken::Character)
    { return null; }
    if(_lexer.cValue() != ':')
    { return null; }

    if(_lexer.getNextToken() != SBPToken::StringLiteral)
    { return null; }

    return NullableStrongRef<FileExprAST>(DefaultTauAllocator::Instance(), null, _lexer.strValue());
}

NullableStrongRef<ShaderIOPointExprAST> ShaderBundleParser::parseIOPoint() noexcept
{
    const i32 bindPoint = _lexer.intValue();

    if(_lexer.getNextToken() != SBPToken::Character)
    { return null; }
    if(_lexer.cValue() != ':')
    { return null; }

    const SBPToken valToken = _lexer.getNextToken();
    if(valToken == SBPToken::IntegerLiteral)
    {
        return RCPCast<ShaderIOPointExprAST>(NullableStrongRef<ShaderIOMapPointExprAST>(DefaultTauAllocator::Instance(), null, bindPoint, _lexer.intValue()));
    }
    else if(valToken == SBPToken::StringLiteral)
    {
        return RCPCast<ShaderIOPointExprAST>(NullableStrongRef<ShaderIOBindPointExprAST>(DefaultTauAllocator::Instance(), null, bindPoint, _lexer.strValue()));
    }

    return null;
}
