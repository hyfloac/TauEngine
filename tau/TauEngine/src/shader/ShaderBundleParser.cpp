#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ast/BlockAST.hpp"
#include "shader/bundle/ast/FileAST.hpp"
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

NullableStrongRef<sbp::AST> ShaderBundleParser::parse(Error* const error) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing bundle.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing bundle.";

    NullableStrongRef<sbp::RootAST> root(DefaultTauAllocator::Instance());
    NullableStrongRef<sbp::APIBlockAST>* curr = &root->node();
    while(true)
    {
        const NullableStrongRef<sbp::APIBlockAST> apiBlock = parseAPIBlock();
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

NullableStrongRef<sbp::APIBlockAST> ShaderBundleParser::parseAPIBlock() noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token when parsing api list.";

    NullableStrongRef<sbp::APIBlockAST> block(DefaultTauAllocator::Instance());
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

void ShaderBundleParser::parseAPIBlockContents(NullableStrongRef<sbp::APIBlockAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token when parsing API block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character when parsing API block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

    while(true)
    {
        PARSE_COND_ERROR(_lexer.getNextToken() == SBPToken::EndOfFile, Error::UnexpectedEOF, "Unexpected EOF reached while reading API block.");

        const NullableStrongRef<sbp::ShaderStageBlockAST> shaderBlock = parseShaderBlock();
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

NullableStrongRef<sbp::ShaderStageBlockAST> ShaderBundleParser::parseShaderBlock() noexcept
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
            const NullableStrongRef<sbp::ShaderStageBlockAST> shaderBlock(DefaultTauAllocator::Instance(), stage);
            parseShaderContents(shaderBlock);
            return shaderBlock;
        }
        default: return null;
    }
}

void ShaderBundleParser::parseShaderContents(NullableStrongRef<sbp::ShaderStageBlockAST> block) noexcept
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
                block->uniforms() = parseUniformsBlock();
                break;
            case SBPToken::TexturesBlock:
                PARSE_COND_ERROR(block->textures(), Error::DuplicateDeclaration, "Duplicate `Textures` block found within inner shader stage block.");
                block->textures() = parseTexturesBlock();
                break;
            default: break;
        }

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == '}')
        { return; }

        PARSE_COND_ERROR(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }
}

NullableStrongRef<sbp::UniformBlockExprAST> ShaderBundleParser::parseUniformsBlock() noexcept
{
    static constexpr const char* ErrorInvalidCRM = "Invalid Common Rendering Model token encountered while parsing uniforms block.";
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing uniforms block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing uniforms block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

    NullableStrongRef<sbp::UniformBindingAST> base(null);
    NullableStrongRef<sbp::UniformBindingAST>* curr = null;

    // For all uniforms
    while(true)
    {
        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::CRMLiteral, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR(!isTextureCRM(_lexer.crmToken()), Error::InvalidCRM, ErrorInvalidCRM);

        const CommonRenderingModelToken crmTarget = _lexer.crmToken();

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);

        const SBPToken valToken = _lexer.getNextToken();
        if(valToken == SBPToken::IntegerLiteral || valToken == SBPToken::UnsignedIntegerLiteral)
        {
            if(!base)
            {
                base = NullableStrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), null, crmTarget, sbp::BindingUnion(_lexer.intValue()));
                curr = &base;
            }
            else
            {
                curr->get()->next() = NullableStrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), null, crmTarget, sbp::BindingUnion(_lexer.intValue()));
                curr = &curr->get()->next();
            }
        }
        else if(valToken == SBPToken::StringLiteral)
        {
            if(!base)
            {
                base = NullableStrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), null, crmTarget, sbp::BindingUnion(_lexer.strValue()));
                curr = &base;
            }
            else
            {
                curr->get()->next() = NullableStrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), null, crmTarget, sbp::BindingUnion(_lexer.strValue()));
                curr = &curr->get()->next();
            }
        }
        else
        {
            PARSE_ERROR(Error::InvalidToken, ErrorInvalidToken);
        }

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        if(_lexer.cValue() == '}')
        { break; }
        PARSE_COND_ERROR(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }

    return base;
}

NullableStrongRef<sbp::TextureParamsBlockAST> ShaderBundleParser::parseTexturesBlock() noexcept
{
    static constexpr const char* ErrorInvalidCRM = "Invalid Common Rendering Model token encountered while parsing texture params block.";
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing texture params block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing texture params block.";
    static constexpr const char* ErrorDuplicateLocation = "Duplicate Location encountered while parsing texture params block.";
    static constexpr const char* ErrorDuplicateSampler = "Duplicate Sampler encountered while parsing texture params block.";
    static constexpr const char* ErrorUnexpectedEndOfBlock = "Unexpected end of block encountered while parsing texture params block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

    NullableStrongRef<sbp::TextureParamsBlockAST> base(null);
    NullableStrongRef<sbp::TextureParamsBlockAST>* curr = null;

    // For all textures
    while(true)
    {
        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::CRMLiteral, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR(!isTextureCRM(_lexer.crmToken()), Error::InvalidCRM, ErrorInvalidCRM);

        const CommonRenderingModelToken crmTarget = _lexer.crmToken();

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

        sbp::BindingUnion binding;
        u32 sampler = 0;

        // Used to determine if all texture params are set.
        uSys flags = 0;

        // For each texture attribute
        while(true)
        {
            const SBPToken valToken = _lexer.getNextToken();
            if(valToken == SBPToken::Location)
            {
                PARSE_COND_ERROR(flags & 0x01, Error::DuplicateDeclaration, ErrorDuplicateLocation);

                PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
                PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);

                if(_lexer.getNextToken() == SBPToken::UnsignedIntegerLiteral)
                {
                    binding = sampler;
                }
                else if(_lexer.currentToken() == SBPToken::StringLiteral)
                {
                    binding = _lexer.strValue();
                }
                else
                {
                    PARSE_ERROR(Error::InvalidToken, ErrorInvalidToken);
                }
                flags |= 0x01;
            }
            else if(valToken == SBPToken::Sampler)
            {
                PARSE_COND_ERROR(flags & 0x02, Error::DuplicateDeclaration, ErrorDuplicateSampler);

                PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
                PARSE_COND_ERROR(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
                PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::UnsignedIntegerLiteral, Error::InvalidToken, ErrorInvalidToken);
                sampler = _lexer.uintValue();
                flags |= 0x02;
            }
            else
            {
                PARSE_ERROR(Error::InvalidToken, ErrorInvalidToken);
            }

            PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
            if(_lexer.cValue() == '}')
            {
                PARSE_COND_ERROR(flags != 0x03, Error::UnexpectedEndOfBlock, ErrorUnexpectedEndOfBlock);

                if(!base)
                {
                    base = NullableStrongRef<sbp::TextureParamsBlockAST>(DefaultTauAllocator::Instance(), null, crmTarget, ::std::move(binding), sampler);
                    curr = &base;
                }
                else
                {
                    curr->get()->next() = NullableStrongRef<sbp::TextureParamsBlockAST>(DefaultTauAllocator::Instance(), null, crmTarget, ::std::move(binding), sampler);
                    curr = &curr->get()->next();
                }
                break;
            }
            else if(_lexer.cValue() != ',')
            {
                PARSE_ERROR(Error::InvalidCharacter, ErrorInvalidChar);
            }
        }

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        if(_lexer.cValue() == '}')
        {
            break;
        }
        PARSE_COND_ERROR(_lexer.cValue() != ',', Error::InvalidCharacter, ErrorInvalidChar);
    }

    return base;
}

NullableStrongRef<sbp::FileAST> ShaderBundleParser::parseFile() noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing file tag.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing file tag.";

    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::StringLiteral, Error::InvalidToken, ErrorInvalidToken);

    return NullableStrongRef<sbp::FileAST>(DefaultTauAllocator::Instance(), _lexer.strValue());
}

bool ShaderBundleParser::isTextureCRM(const CommonRenderingModelToken token) noexcept
{
    switch(token)
    {
        case CommonRenderingModelToken::TextureNormal:      
        case CommonRenderingModelToken::TextureDiffuse:     
        case CommonRenderingModelToken::TexturePBRCompound: 
        case CommonRenderingModelToken::TextureEmissivity:  
        case CommonRenderingModelToken::TexturePosition:    
        case CommonRenderingModelToken::TextureDepth:       
        case CommonRenderingModelToken::TextureStencil: return true;
        default: return false;
    }
}

bool ShaderBundleParser::isUniformCRM(const CommonRenderingModelToken token) noexcept
{
    switch(token)
    {
        case CommonRenderingModelToken::UniformBindingCameraDynamic:
        case CommonRenderingModelToken::UniformBindingCameraStatic: return true;
        default: return false;
    }
}
