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
        {
            const NullableStrongRef<sbp::OuterShaderStageBlockExprAST> shaderBlock(DefaultTauAllocator::Instance(), stage);
            parseOuterShaderContents(shaderBlock);
            return shaderBlock;
        }
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
            case SBPToken::InputsBlock:
                PARSE_ERROR(Error::InvalidBlock, "Inputs block can only be in `Vertex` block");
            case SBPToken::OutputsBlock:
                PARSE_ERROR(Error::InvalidBlock, "Outputs block can only be in `Pixel` block");
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

void ShaderBundleParser::parseOuterShaderContents(NullableStrongRef<sbp::OuterShaderStageBlockExprAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token found within outer shader stage block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character found within outer shader stage block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

    while(true)
    {
        const SBPToken token = _lexer.getNextToken();
        PARSE_COND_ERROR(token == SBPToken::EndOfFile, Error::UnexpectedEOF, "Unexpected EOF in outer shader stage block.");

        switch(token)
        {
            case SBPToken::File:
                PARSE_COND_ERROR(block->file(), Error::DuplicateDeclaration, "Duplicate `File` entry in outer shader stage block.");
                block->file() = parseFile();
                break;
            case SBPToken::InputsBlock:
                PARSE_COND_ERROR(block->stage() != EShader::Stage::Vertex, Error::InvalidBlock, "Inputs block can only be in `Vertex` block");
                PARSE_COND_ERROR(block->io(), Error::DuplicateDeclaration, "Duplicate `Inputs` block found within `Vertex` shader stage block.");
                block->io() = parseBlock();
                break;
            case SBPToken::OutputsBlock:
                PARSE_COND_ERROR(block->stage() != EShader::Stage::Pixel, Error::InvalidBlock, "Outputs block can only be in `Pixel` block");
                PARSE_COND_ERROR(block->io(), Error::DuplicateDeclaration, "Duplicate `Outputs` block found within `Pixel` shader stage block.");
                block->io() = parseBlock();
                break;
            case SBPToken::UniformsBlock:
                PARSE_COND_ERROR(block->uniforms(), Error::DuplicateDeclaration, "Duplicate `Uniforms` block found within outer shader stage block.");
                block->uniforms() = parseBlock();
                break;
            case SBPToken::TexturesBlock:
                PARSE_COND_ERROR(block->textures(), Error::DuplicateDeclaration, "Duplicate `Textures` block found within outer shader stage block.");
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
        case SBPToken::InputsBlock:
            blockType = sbp::BlockType::Inputs;
            break;
        case SBPToken::OutputsBlock:
            blockType = sbp::BlockType::Outputs;
            break;
        default: return null;
    }

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
        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::UnsignedIntegerLiteral || _lexer.currentToken() != SBPToken::CRMLiteral, Error::InvalidToken, ErrorInvalidToken);
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
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing io point.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing io point.";

    u32 bindPoint;
    if(_lexer.currentToken() == SBPToken::UnsignedIntegerLiteral)
    {
        bindPoint = _lexer.uintValue();
    }
    else
    {
        PARSE_COND_ERROR(getAssociatedBlock(_lexer.crmToken()) == _currentBlock, Error::InvalidToken, ErrorInvalidToken);
        bindPoint = mapCRM(_lexer.crmToken());
    }

    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);

    const SBPToken valToken = _lexer.getNextToken();
    if(valToken == SBPToken::IntegerLiteral || valToken == SBPToken::UnsignedIntegerLiteral)
    {
        return NullableStrongRef<sbp::ShaderIOMapPointExprAST>(DefaultTauAllocator::Instance(), null, bindPoint, _lexer.intValue());
    }
    if(valToken == SBPToken::StringLiteral)
    {
        return NullableStrongRef<sbp::ShaderIOBindPointExprAST>(DefaultTauAllocator::Instance(), null, bindPoint, _lexer.strValue());
    }

    PARSE_ERROR_N(Error::InvalidToken, ErrorInvalidToken);
}

u32 ShaderBundleParser::mapCRM(const CommonRenderingModelToken token) noexcept
{
    switch(token)
    {
        case CommonRenderingModelToken::InputPosition:     return 0;
        case CommonRenderingModelToken::InputNormal:       return 1;
        case CommonRenderingModelToken::InputTangent:      return 2;
        case CommonRenderingModelToken::InputTextureCoord: return 3;
        case CommonRenderingModelToken::InputBoneID:       return 4;
        case CommonRenderingModelToken::InputBoneWeight:   return 5;

        case CommonRenderingModelToken::OutputNormal:      return 0;
        case CommonRenderingModelToken::OutputDiffuse:     return 1;
        case CommonRenderingModelToken::OutputPBRCompound: return 2;
        case CommonRenderingModelToken::OutputEmissivity:  return 3;
        case CommonRenderingModelToken::OutputPosition:    return 4;

        case CommonRenderingModelToken::InputFBPosition:     return 0;
        case CommonRenderingModelToken::InputFBTextureCoord: return 1;

        case CommonRenderingModelToken::OutputFBColor: return 0;

        case CommonRenderingModelToken::UniformBindingCameraDynamic: return 0;
        case CommonRenderingModelToken::UniformBindingCameraStatic:  return 1;

        case CommonRenderingModelToken::TextureNormal:      return 0;
        case CommonRenderingModelToken::TextureDiffuse:     return 1;
        case CommonRenderingModelToken::TexturePBRCompound: return 2;
        case CommonRenderingModelToken::TextureEmissivity:  return 3;
        case CommonRenderingModelToken::TexturePosition:    return 4;
        case CommonRenderingModelToken::TextureDepth:       return 5;
        case CommonRenderingModelToken::TextureStencil:     return 6;
        default: return 16;
    }
}

sbp::BlockType ShaderBundleParser::getAssociatedBlock(const CommonRenderingModelToken token) noexcept
{
    switch(token)
    {
        case CommonRenderingModelToken::InputPosition:     
        case CommonRenderingModelToken::InputNormal:       
        case CommonRenderingModelToken::InputTangent:      
        case CommonRenderingModelToken::InputTextureCoord: 
        case CommonRenderingModelToken::InputBoneID:       
        case CommonRenderingModelToken::InputBoneWeight: return sbp::BlockType::Inputs;

        case CommonRenderingModelToken::OutputNormal:
        case CommonRenderingModelToken::OutputDiffuse:
        case CommonRenderingModelToken::OutputPBRCompound:
        case CommonRenderingModelToken::OutputEmissivity:;
        case CommonRenderingModelToken::OutputPosition: return sbp::BlockType::Outputs;

        case CommonRenderingModelToken::InputFBPosition:
        case CommonRenderingModelToken::InputFBTextureCoord: return sbp::BlockType::Inputs;

        case CommonRenderingModelToken::OutputFBColor: return sbp::BlockType::Outputs;

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
