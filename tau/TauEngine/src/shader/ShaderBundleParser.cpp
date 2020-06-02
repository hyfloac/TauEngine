#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ast/BlockExprAST.hpp"
#include "shader/bundle/ast/FileExprAST.hpp"
#include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"
#include "shader/bundle/ast/RootExprAST.hpp"

#pragma region Error Handling
#define _PARSE_ERROR_INTERNAL(_ERR, _MSG) \
    if(_error == ShaderBundleParser::Error::NoError) { _error = (_ERR); } \
    _errorMsg = (_MSG); \
    _errorIndex = _lexer.fileIndex(); \
    _errorLine = _lexer.fileLine(); \
    _errorLineIndex = _lexer.lineIndex(); \
    _codeLine = __LINE__

#define PARSE_ERROR(_ERR, _MSG) do { \
    _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return; } while(0)

#define PARSE_ERROR_V(_ERR, _MSG, _VAL) do { \
    _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return _VAL; } while(0)

#define PARSE_ERROR_N(_ERR, _MSG) do { \
    _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return nullptr; } while(0)

#define PARSE_ERROR_T(_ERR, _MSG) do { \
    _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return true; } while(0)

#define PARSE_ERROR_F(_ERR, _MSG) do { \
    _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return false; } while(0)

#define PARSE_COND_ERROR(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return; } } while(0)

#define PARSE_COND_ERROR_V(_COND, _ERR, _MSG, _VAL) do { \
    if((_COND)) { \
        _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return _VAL; } } while(0)

#define PARSE_COND_ERROR_N(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return nullptr; } } while(0)

#define PARSE_COND_ERROR_T(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return true; } } while(0)

#define PARSE_COND_ERROR_F(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        _PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return false; } } while(0)
#pragma endregion 

NullableStrongRef<sbp::ExprAST> ShaderBundleParser::parse(Error* const error) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing bundle.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing bundle.";

    NullableStrongRef<sbp::RootExprAST> root(DefaultTauAllocator::Instance());
    NullableStrongRef<sbp::APIBlockExprAST>* curr = &root->node();
    while(true)
    {
        const NullableStrongRef<sbp::APIBlockExprAST> apiBlock = parseAPIBlock();
        if(!apiBlock)
        { return null; }
        *curr = apiBlock;
        curr = &curr->get()->next();

        if(_lexer.getNextToken() == SBPToken::EndOfFile)
        { break; }

        PARSE_COND_ERROR_N(_lexer.currentToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }
    _ast = root;

    ERROR_CODE_V(_error, _ast);
}

NullableStrongRef<sbp::APIBlockExprAST> ShaderBundleParser::parseAPIBlock() noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token when parsing api list.";

    NullableStrongRef<sbp::APIBlockExprAST> block(DefaultTauAllocator::Instance());
    while(true)
    {
        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::RenderingMode, Error::InvalidToken, ErrorInvalidToken);
        block->apis().set(_lexer.rmValue());

        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == ':')
        { break; }

        PARSE_COND_ERROR_N(_lexer.cValue() != ',', Error::InvalidCharacter, "Invalid character when parsing api list.");
    }

    parseAPIBlockContents(block);

    return block;
}

void ShaderBundleParser::parseAPIBlockContents(NullableStrongRef<sbp::APIBlockExprAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token when parsing API block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character when parsing API block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

    while(true)
    {
        PARSE_COND_ERROR(_lexer.getNextToken() == SBPToken::EndOfFile, Error::UnexpectedEOF, "Unexpected EOF reached while reading API block.");

        const NullableStrongRef<sbp::ShaderStageBlockExprAST> shaderBlock = parseShaderBlock();
        if(!shaderBlock)
        { return; }

        switch(shaderBlock->stage())
        {
            case EShader::Stage::Vertex:                 block->vertex()   = shaderBlock; break;
            case EShader::Stage::TessellationControl:    block->tessCtrl() = shaderBlock; break;
            case EShader::Stage::TessellationEvaluation: block->tessEval() = shaderBlock; break;
            case EShader::Stage::Geometry:               block->geometry() = shaderBlock; break;
            case EShader::Stage::Pixel:                  block->pixel()    = shaderBlock; break;
            default: return;
        }

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == '}')
        { break; }

        PARSE_COND_ERROR(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }
}

NullableStrongRef<sbp::ShaderStageBlockExprAST> ShaderBundleParser::parseShaderBlock() noexcept
{
    EShader::Stage stage;

    switch(_lexer.currentToken())
    {
        case SBPToken::VertexBlock:
            stage = EShader::Stage::Vertex;
            break;
        case SBPToken::TessellationControlBlock:
            stage = EShader::Stage::TessellationControl;
            break;
        case SBPToken::TessellationEvaluationBlock:
            stage = EShader::Stage::TessellationEvaluation;
            break;
        case SBPToken::GeometryBlock:
            stage = EShader::Stage::Geometry;
            break;
        case SBPToken::PixelBlock:
            stage = EShader::Stage::Pixel;
            break;
        default: PARSE_ERROR_N(Error::InvalidToken, "Invalid shader block type.");
    }

    switch(stage)
    {
        case EShader::Stage::Vertex:
        case EShader::Stage::Pixel:
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        case EShader::Stage::Geometry:
        {
            const NullableStrongRef<sbp::ShaderStageBlockExprAST> shaderBlock(DefaultTauAllocator::Instance(), stage);
            parseShaderContents(shaderBlock);
            return shaderBlock;
        }
        default: return null;
    }
}

void ShaderBundleParser::parseShaderContents(NullableStrongRef<sbp::ShaderStageBlockExprAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token found within inner shader stage block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character found within inner shader stage block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

    while(true)
    {
        const SBPToken token = _lexer.getNextToken();
        PARSE_COND_ERROR(token == SBPToken::EndOfFile, Error::UnexpectedEOF, "Unexpected EOF in inner shader stage block.");

        switch(token)
        {
            case SBPToken::File:
                PARSE_COND_ERROR(block->file(), Error::DuplicateDeclaration, "Duplicate `File` entry in inner shader stage block.");
                block->file() = parseFile();
                break;
            case SBPToken::UniformsBlock:
                PARSE_COND_ERROR(block->uniforms(), Error::DuplicateDeclaration, "Duplicate `Uniforms` block found within inner shader stage block.");
                block->uniforms() = parseBlock();
                break;
            case SBPToken::TexturesBlock:
                PARSE_COND_ERROR(block->textures(), Error::DuplicateDeclaration, "Duplicate `Textures` block found within inner shader stage block.");
                block->textures() = parseBlock();
                break;
            default: break;
        }

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == '}')
        { return; }

        PARSE_COND_ERROR(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }
}

NullableStrongRef<sbp::BlockExprAST> ShaderBundleParser::parseBlock() noexcept
{
    sbp::BlockType blockType;

    switch(_lexer.currentToken())
    {
        case SBPToken::UniformsBlock:
            blockType = sbp::BlockType::Uniforms;
            break;
        case SBPToken::TexturesBlock:
            blockType = sbp::BlockType::Textures;
            break;
        default: return null;
    }

    _currentBlock = blockType;

    const NullableStrongRef<sbp::BlockExprAST> block(DefaultTauAllocator::Instance(), blockType, null);
    parseBlockContents(block);
    return block;
}

void ShaderBundleParser::parseBlockContents(NullableStrongRef<sbp::BlockExprAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

    NullableStrongRef<sbp::ShaderIOPointExprAST>* curr = &block->container();

    while(true)
    {
        PARSE_COND_ERROR(_lexer.currentToken() != SBPToken::CRMLiteral, Error::InvalidToken, ErrorInvalidToken);
        NullableStrongRef<sbp::ShaderIOPointExprAST> bindPoint = parseIOPoint();
        if(!bindPoint)
        { return; }

        *curr = bindPoint;
        curr = &bindPoint->next();

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == '}')
        { break; }
        
        PARSE_COND_ERROR(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }
}

NullableStrongRef<sbp::FileExprAST> ShaderBundleParser::parseFile() noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing file tag.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing file tag.";

    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::StringLiteral, Error::InvalidToken, ErrorInvalidToken);

    return NullableStrongRef<sbp::FileExprAST>(DefaultTauAllocator::Instance(), _lexer.strValue());
}

NullableStrongRef<sbp::ShaderIOPointExprAST> ShaderBundleParser::parseIOPoint() noexcept
{
    static constexpr const char* ErrorInvalidCRM = "Invalid Common Rendering Model token encountered while parsing io point.";
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing io point.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing io point.";

    CommonRenderingModelToken crmTarget = _lexer.crmToken();

    PARSE_COND_ERROR_N(getAssociatedBlock(crmTarget) != _currentBlock, Error::InvalidCRM, ErrorInvalidCRM);
    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);

    const SBPToken valToken = _lexer.getNextToken();
    if(valToken == SBPToken::IntegerLiteral || valToken == SBPToken::UnsignedIntegerLiteral)
    {
        return NullableStrongRef<sbp::ShaderIOMapPointExprAST>(DefaultTauAllocator::Instance(), null, crmTarget, _lexer.intValue());
    }
    if(valToken == SBPToken::StringLiteral)
    {
        return NullableStrongRef<sbp::ShaderIOBindPointExprAST>(DefaultTauAllocator::Instance(), null, crmTarget, _lexer.strValue());
    }

    PARSE_ERROR_N(Error::InvalidToken, ErrorInvalidToken);
}

sbp::BlockType ShaderBundleParser::getAssociatedBlock(const CommonRenderingModelToken token) noexcept
{
    switch(token)
    {
        case CommonRenderingModelToken::UniformBindingCameraDynamic:
        case CommonRenderingModelToken::UniformBindingCameraStatic: return sbp::BlockType::Uniforms;

        case CommonRenderingModelToken::TextureNormal:      
        case CommonRenderingModelToken::TextureDiffuse:     
        case CommonRenderingModelToken::TexturePBRCompound: 
        case CommonRenderingModelToken::TextureEmissivity:  
        case CommonRenderingModelToken::TexturePosition:    
        case CommonRenderingModelToken::TextureDepth:       
        case CommonRenderingModelToken::TextureStencil: return sbp::BlockType::Textures;
        default: return static_cast<sbp::BlockType>(0);
    }
}
