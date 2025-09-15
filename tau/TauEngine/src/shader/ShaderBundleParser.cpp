#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ast/BlockAST.hpp"
#include "shader/bundle/ast/FileAST.hpp"
#include "shader/bundle/ast/RootExprAST.hpp"

#pragma region Error Handling
#define PARSE_ERROR_INTERNAL(_ERR, _MSG) \
    if(_error == ShaderBundleParser::Error::NoError) { _error = (_ERR); } \
    _errorMsg = (_MSG); \
    _errorIndex = _lexer.fileIndex(); \
    _errorLine = _lexer.fileLine(); \
    _errorLineIndex = _lexer.lineIndex(); \
    _codeLine = __LINE__

#define PARSE_ERROR(_ERR, _MSG) do { \
    PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return; } while(false)

#define PARSE_ERROR_V(_ERR, _MSG, _VAL) do { \
    PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return _VAL; } while(false)

#define PARSE_ERROR_N(_ERR, _MSG) do { \
    PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return nullptr; } while(false)

#define PARSE_ERROR_T(_ERR, _MSG) do { \
    PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return true; } while(false)

#define PARSE_ERROR_F(_ERR, _MSG) do { \
    PARSE_ERROR_INTERNAL(_ERR, _MSG); \
    return false; } while(false)

#define PARSE_COND_ERROR(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return; } } while(false)

#define PARSE_COND_ERROR_V(_COND, _ERR, _MSG, _VAL) do { \
    if((_COND)) { \
        PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return _VAL; } } while(false)

#define PARSE_COND_ERROR_N(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return nullptr; } } while(false)

#define PARSE_COND_ERROR_T(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return true; } } while(false)

#define PARSE_COND_ERROR_F(_COND, _ERR, _MSG) do { \
    if((_COND)) { \
        PARSE_ERROR_INTERNAL(_ERR, _MSG); \
        return false; } } while(false)
#pragma endregion 

static constexpr C8ConstExprString TextureNormal(u8"CRMTextureNormal");
static constexpr C8ConstExprString TextureDiffuse(u8"CRMTextureDiffuse");
static constexpr C8ConstExprString TextureAlbedo(u8"CRMTextureAlbedo");
static constexpr C8ConstExprString TexturePBRCompound(u8"CRMTexturePBRCompound");
static constexpr C8ConstExprString TextureEmissivity(u8"CRMTextureEmissivity");
static constexpr C8ConstExprString TexturePosition(u8"CRMTexturePosition");
static constexpr C8ConstExprString TextureDepth(u8"CRMTextureDepth");
static constexpr C8ConstExprString TextureStencil(u8"CRMTextureStencil");

static constexpr C8ConstExprString UniformBindingCameraDynamic(u8"CRMUniformBindingCameraDynamic");
static constexpr C8ConstExprString UniformBindingCameraStatic(u8"CRMUniformBindingCameraStatic");

StrongRef<sbp::AST> ShaderBundleParser::parse(Error* const error) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing bundle.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing bundle.";

    StrongRef<sbp::RootAST> root(DefaultTauAllocator::Instance());
    StrongRef<sbp::APIBlockAST>* curr = &root->node();
    while(true)
    {
        const StrongRef<sbp::APIBlockAST> apiBlock = parseAPIBlock();
        if(!apiBlock)
        { return nullptr; }
        *curr = apiBlock;
        curr = &curr->Get()->next();

        if(_lexer.getNextToken() == SBPToken::EndOfFile)
        { break; }

        PARSE_COND_ERROR_N(_lexer.currentToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.cValue() != U',', Error::InvalidCharacter, ErrorInvalidChar);
    }
    _ast = root;

    ERROR_CODE_V(_error, _ast);
}

StrongRef<sbp::APIBlockAST> ShaderBundleParser::parseAPIBlock() noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token when parsing api list.";
    static constexpr const char* ErrorInvalidRenderingMode = "Invalid rendering mode when parsing api list.";

    StrongRef<sbp::APIBlockAST> block(DefaultTauAllocator::Instance());
    while(true)
    {
        // PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::RenderingMode, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Identifier, Error::InvalidToken, ErrorInvalidToken);
        // block->apis().set(_lexer.rmValue());

    #define RM_STR_CASE(API) C8STR_CASE(u8#API, {\
        mode = RenderingMode::Mode::API; \
    })

        RenderingMode::Mode mode = RenderingMode::Mode::MAX_VALUE;
        
        STR_SWITCH(_lexer.identifierValue(),
        {
            C8STR_CASE(u8"DirectX10", { mode = RenderingMode::Mode::DirectX10; })
            RM_STR_CASE(DirectX10)
            RM_STR_CASE(DirectX11)
            RM_STR_CASE(DirectX12)
            RM_STR_CASE(DirectX12_1)
            RM_STR_CASE(Vulkan)
            RM_STR_CASE(OpenGL4_1)
            RM_STR_CASE(OpenGL4_2)
            RM_STR_CASE(OpenGL4_3)
            RM_STR_CASE(OpenGL4_4)
            RM_STR_CASE(OpenGL4_5)
            RM_STR_CASE(OpenGL4_6)
        },
        { PARSE_ERROR_N(Error::InvalidToken, ErrorInvalidRenderingMode); })

    #undef RM_STR_CASE

        block->apis().set(mode);

        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == U':')
        { break; }

        PARSE_COND_ERROR_N(_lexer.cValue() != U',', Error::InvalidCharacter, "Invalid character when parsing api list.");
    }

    parseAPIBlockContents(block);

    return block;
}

void ShaderBundleParser::parseAPIBlockContents(StrongRef<sbp::APIBlockAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token when parsing API block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character when parsing API block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

    while(true)
    {
        PARSE_COND_ERROR(_lexer.getNextToken() == SBPToken::EndOfFile, Error::UnexpectedEOF, "Unexpected EOF reached while reading API block.");

        const StrongRef<sbp::ShaderStageBlockAST> shaderBlock = parseShaderBlock();
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

        if(_lexer.cValue() == U'}')
        { break; }

        PARSE_COND_ERROR(_lexer.cValue() != U',', Error::InvalidCharacter, ErrorInvalidChar);
    }
}

StrongRef<sbp::ShaderStageBlockAST> ShaderBundleParser::parseShaderBlock() noexcept
{
    EShader::Stage stage = EShader::Stage::Vertex;

    PARSE_COND_ERROR_N(_lexer.currentToken() != SBPToken::Identifier, Error::InvalidToken, "Identifier expected for shader block type.");

    static constexpr C8ConstExprString Vertex(u8"Vertex");
    static constexpr C8ConstExprString TessCtrl(u8"TessellationControl");
    static constexpr C8ConstExprString Hull(u8"Hull");
    static constexpr C8ConstExprString TessEval(u8"TessellationEvaluation");
    static constexpr C8ConstExprString Domain(u8"Domain");
    static constexpr C8ConstExprString Geometry(u8"Geometry");
    static constexpr C8ConstExprString Pixel(u8"Pixel");
    static constexpr C8ConstExprString Fragment(u8"Fragment");

    bool switchDefaulted = true;

    switch(_lexer.identifierValue().HashCode())
    {
        case Vertex.HashCode():
            if(_lexer.identifierValue().Equals(Vertex))
            {
                switchDefaulted = false;
                stage = EShader::Stage::Vertex;
            }
            break;
        case TessCtrl.HashCode():
        case Hull.HashCode():
            if(_lexer.identifierValue().Equals(TessCtrl) || _lexer.identifierValue().Equals(Hull))
            {
                switchDefaulted = false;
                stage = EShader::Stage::TessellationControl;
            }
            break;
        case TessEval.HashCode():
        case Domain.HashCode():
            if(_lexer.identifierValue().Equals(TessEval) || _lexer.identifierValue().Equals(Domain))
            {
                switchDefaulted = false;
                stage = EShader::Stage::TessellationEvaluation;
            }
            break;
        case Geometry.HashCode():
            if(_lexer.identifierValue().Equals(Geometry))
            {
                switchDefaulted = false;
                stage = EShader::Stage::Geometry;
            }
            break;
        case Pixel.HashCode():
        case Fragment.HashCode():
            if(_lexer.identifierValue().Equals(Pixel) || _lexer.identifierValue().Equals(Fragment))
            {
                switchDefaulted = false;
                stage = EShader::Stage::Pixel;
            }
            break;
        default: break;
    }

    PARSE_COND_ERROR_N(switchDefaulted, Error::InvalidToken, "Invalid shader block type.");

    switch(stage)
    {
        case EShader::Stage::Vertex:
        case EShader::Stage::Pixel:
        case EShader::Stage::TessellationControl:
        case EShader::Stage::TessellationEvaluation:
        case EShader::Stage::Geometry:
        {
            const StrongRef<sbp::ShaderStageBlockAST> shaderBlock(DefaultTauAllocator::Instance(), stage);
            parseShaderContents(shaderBlock);
            return shaderBlock;
        }
        default: return nullptr;
    }
}

void ShaderBundleParser::parseShaderContents(StrongRef<sbp::ShaderStageBlockAST> block) noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token found within inner shader stage block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character found within inner shader stage block.";

    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR(_lexer.cValue() != U'{', Error::InvalidCharacter, ErrorInvalidChar);

    while(true)
    {
        const SBPToken token = _lexer.getNextToken();
        PARSE_COND_ERROR(token == SBPToken::EndOfFile, Error::UnexpectedEOF, "Unexpected EOF in inner shader stage block.");
        PARSE_COND_ERROR(token != SBPToken::Identifier, Error::UnexpectedEOF, "Expected identifier in inner shader stage block.");

        if(_lexer.identifierValue().Equals(C8ConstExprString(u8"File")))
        {
            PARSE_COND_ERROR(block->file(), Error::DuplicateDeclaration, "Duplicate `File` entry in inner shader stage block.");
            block->file() = parseFile();
        }
        else if(_lexer.identifierValue().Equals(C8ConstExprString(u8"Uniforms")))
        {
            PARSE_COND_ERROR(block->uniforms(), Error::DuplicateDeclaration, "Duplicate `Uniforms` block found within inner shader stage block.");
            block->uniforms() = parseUniformsBlock();
        }
        else if(_lexer.identifierValue().Equals(C8ConstExprString(u8"Textures")))
        {
            PARSE_COND_ERROR(block->textures(), Error::DuplicateDeclaration, "Duplicate `Textures` block found within inner shader stage block.");
            block->textures() = parseTexturesBlock();
        }

        // switch(_lexer.identifierValue())
        // {
        //     case SBPToken::File:
        //         PARSE_COND_ERROR(block->file(), Error::DuplicateDeclaration, "Duplicate `File` entry in inner shader stage block.");
        //         block->file() = parseFile();
        //         break;
        //     case SBPToken::UniformsBlock:
        //         PARSE_COND_ERROR(block->uniforms(), Error::DuplicateDeclaration, "Duplicate `Uniforms` block found within inner shader stage block.");
        //         block->uniforms() = parseUniformsBlock();
        //         break;
        //     case SBPToken::TexturesBlock:
        //         PARSE_COND_ERROR(block->textures(), Error::DuplicateDeclaration, "Duplicate `Textures` block found within inner shader stage block.");
        //         block->textures() = parseTexturesBlock();
        //         break;
        //     default: break;
        // }

        PARSE_COND_ERROR(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);

        if(_lexer.cValue() == U'}')
        { return; }

        PARSE_COND_ERROR(_lexer.cValue() != U',', Error::InvalidCharacter, ErrorInvalidChar);
    }
}

StrongRef<sbp::UniformBlockExprAST> ShaderBundleParser::parseUniformsBlock() noexcept
{
    static constexpr const char* ErrorInvalidCRM = "Invalid Common Rendering Model token encountered while parsing uniforms block.";
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing uniforms block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing uniforms block.";

    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != U'{', Error::InvalidCharacter, ErrorInvalidChar);

    StrongRef<sbp::UniformBindingAST> base(nullptr);
    StrongRef<sbp::UniformBindingAST>* curr = nullptr;

    // For all uniforms
    while(true)
    {
        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Identifier, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(!isTextureCRM(_lexer.identifierValue()), Error::InvalidCRM, ErrorInvalidCRM);

        const CommonRenderingModelToken crmTarget = parseCRM(_lexer.identifierValue());
        

        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);

        const SBPToken valToken = _lexer.getNextToken();
        if(valToken == SBPToken::IntegerLiteral || valToken == SBPToken::UnsignedIntegerLiteral)
        {
            if(!base)
            {
                base = StrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), nullptr, crmTarget, sbp::BindingUnion(_lexer.intValue()));
                curr = &base;
            }
            else
            {
                curr->Get()->next() = StrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), nullptr, crmTarget, sbp::BindingUnion(_lexer.intValue()));
                curr = &curr->Get()->next();
            }
        }
        else if(valToken == SBPToken::StringLiteral)
        {
            if(!base)
            {
                base = StrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), nullptr, crmTarget, sbp::BindingUnion(_lexer.strValue()));
                curr = &base;
            }
            else
            {
                curr->Get()->next() = StrongRef<sbp::UniformBlockExprAST>(DefaultTauAllocator::Instance(), nullptr, crmTarget, sbp::BindingUnion(_lexer.strValue()));
                curr = &curr->Get()->next();
            }
        }
        else
        {
            PARSE_ERROR_N(Error::InvalidToken, ErrorInvalidToken);
        }

        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        if(_lexer.cValue() == U'}')
        { break; }
        PARSE_COND_ERROR_N(_lexer.cValue() != U',', Error::InvalidCharacter, ErrorInvalidChar);
    }

    return base;
}

StrongRef<sbp::TextureParamsBlockAST> ShaderBundleParser::parseTexturesBlock() noexcept
{
    static constexpr C8ConstExprString Location(u8"Location");
    static constexpr C8ConstExprString Sampler(u8"Sampler");

    static constexpr const char* ErrorInvalidCRM = "Invalid Common Rendering Model token encountered while parsing texture params block.";
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing texture params block.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing texture params block.";
    static constexpr const char* ErrorDuplicateLocation = "Duplicate Location encountered while parsing texture params block.";
    static constexpr const char* ErrorDuplicateSampler = "Duplicate Sampler encountered while parsing texture params block.";
    static constexpr const char* ErrorUnexpectedEndOfBlock = "Unexpected end of block encountered while parsing texture params block.";

    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != ':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != '{', Error::InvalidCharacter, ErrorInvalidChar);

    StrongRef<sbp::TextureParamsBlockAST> base(nullptr);
    StrongRef<sbp::TextureParamsBlockAST>* curr = nullptr;

    // For all textures
    while(true)
    {
        // PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::CRMLiteral, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Identifier, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(!isTextureCRM(_lexer.identifierValue()), Error::InvalidCRM, ErrorInvalidCRM);

        const CommonRenderingModelToken crmTarget = parseCRM(_lexer.identifierValue());

        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);
        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        PARSE_COND_ERROR_N(_lexer.cValue() != U'{', Error::InvalidCharacter, ErrorInvalidChar);
                        
        sbp::BindingUnion binding;
        u32 sampler = 0;

        // Used to determine if all texture params are set.
        uSys flags = 0;

        // For each texture attribute
        while(true)
        {
            const SBPToken valToken = _lexer.getNextToken();
            PARSE_COND_ERROR_N(valToken != SBPToken::Identifier, Error::InvalidToken, ErrorInvalidToken);

            if(_lexer.identifierValue().Equals(Location))
            {
                PARSE_COND_ERROR_N(flags & 0x01, Error::DuplicateDeclaration, ErrorDuplicateLocation);

                PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
                PARSE_COND_ERROR_N(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);

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
                    PARSE_ERROR_N(Error::InvalidToken, ErrorInvalidToken);
                }
                flags |= 0x01;
            }
            else if(_lexer.identifierValue().Equals(Sampler))
            {
                PARSE_COND_ERROR_N(flags & 0x02, Error::DuplicateDeclaration, ErrorDuplicateSampler);

                PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
                PARSE_COND_ERROR_N(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);
                PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::UnsignedIntegerLiteral, Error::InvalidToken, ErrorInvalidToken);
                sampler = _lexer.uintValue();
                flags |= 0x02;
            }
            else
            {
                PARSE_ERROR_N(Error::InvalidToken, ErrorInvalidToken);
            }

            PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
            if(_lexer.cValue() == U'}')
            {
                PARSE_COND_ERROR_N(flags != 0x03, Error::UnexpectedEndOfBlock, ErrorUnexpectedEndOfBlock);

                if(!base)
                {
                    base = StrongRef<sbp::TextureParamsBlockAST>(DefaultTauAllocator::Instance(), nullptr, crmTarget, ::std::move(binding), sampler);
                    curr = &base;
                }
                else
                {
                    curr->Get()->next() = StrongRef<sbp::TextureParamsBlockAST>(DefaultTauAllocator::Instance(), nullptr, crmTarget, ::std::move(binding), sampler);
                    curr = &curr->Get()->next();
                }
                break;
            }
            else if(_lexer.cValue() != U',')
            {
                PARSE_ERROR_N(Error::InvalidCharacter, ErrorInvalidChar);
            }
        }

        PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
        if(_lexer.cValue() == U'}')
        {
            break;
        }
        PARSE_COND_ERROR_N(_lexer.cValue() != U',', Error::InvalidCharacter, ErrorInvalidChar);
    }

    return base;
}

StrongRef<sbp::FileAST> ShaderBundleParser::parseFile() noexcept
{
    static constexpr const char* ErrorInvalidToken = "Invalid token encountered while parsing file tag.";
    static constexpr const char* ErrorInvalidChar = "Invalid character encountered while parsing file tag.";

    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::Character, Error::InvalidToken, ErrorInvalidToken);
    PARSE_COND_ERROR_N(_lexer.cValue() != U':', Error::InvalidCharacter, ErrorInvalidChar);
    PARSE_COND_ERROR_N(_lexer.getNextToken() != SBPToken::StringLiteral, Error::InvalidToken, ErrorInvalidToken);

    return StrongRef<sbp::FileAST>(DefaultTauAllocator::Instance(), _lexer.strValue());
}

CommonRenderingModelToken ShaderBundleParser::parseCRM(const C8DynString& token) noexcept
{
#define CRM_STR_CASE(TOKEN) C8STR_CASE(u8"CRM" u8#TOKEN, { \
        return CommonRenderingModelToken::TOKEN; \
    })

    STR_SWITCH(token, {
        CRM_STR_CASE(UniformBindingCameraDynamic)
        CRM_STR_CASE(UniformBindingCameraStatic)
        CRM_STR_CASE(TextureNormal)
        CRM_STR_CASE(TextureDiffuse)
        CRM_STR_CASE(TextureAlbedo)
        CRM_STR_CASE(TexturePBRCompound)
        CRM_STR_CASE(TextureEmissivity)
        CRM_STR_CASE(TexturePosition)
        CRM_STR_CASE(TextureDepth)
        CRM_STR_CASE(TextureStencil)
    },
    { return static_cast<CommonRenderingModelToken>(0); })

#undef CRM_STR_CASE

    return static_cast<CommonRenderingModelToken>(0);
}

bool ShaderBundleParser::isTextureCRM(const C8DynString& token) noexcept
{
    if(token.Equals(TextureNormal) ||
       token.Equals(TextureDiffuse) ||
       token.Equals(TextureAlbedo) ||
       token.Equals(TexturePBRCompound) ||
       token.Equals(TextureEmissivity) ||
       token.Equals(TexturePosition) ||
       token.Equals(TextureDepth) ||
       token.Equals(TextureStencil))
    {
        return true;
    }

    return false;
    
    // switch(token)
    // {
    //     case CommonRenderingModelToken::TextureNormal:      
    //     case CommonRenderingModelToken::TextureDiffuse:     
    //     case CommonRenderingModelToken::TexturePBRCompound: 
    //     case CommonRenderingModelToken::TextureEmissivity:  
    //     case CommonRenderingModelToken::TexturePosition:    
    //     case CommonRenderingModelToken::TextureDepth:       
    //     case CommonRenderingModelToken::TextureStencil: return true;
    //     default: return false;
    // }
}

bool ShaderBundleParser::isUniformCRM(const C8DynString& token) noexcept
{
    if(token.Equals(UniformBindingCameraDynamic) ||
        token.Equals(UniformBindingCameraStatic))
    {
        return true;
    }

    return false;
    
    // switch(token)
    // {
    //     case CommonRenderingModelToken::UniformBindingCameraDynamic:
    //     case CommonRenderingModelToken::UniformBindingCameraStatic: return true;
    //     default: return false;
    // }
}
