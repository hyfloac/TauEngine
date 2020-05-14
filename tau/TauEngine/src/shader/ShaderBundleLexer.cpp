#include "shader/bundle/ShaderBundleLexer.hpp"

[[nodiscard]] static bool isWhiteSpace(char c) noexcept;
[[nodiscard]] static bool isDelimiter(char c) noexcept;
[[nodiscard]] static bool isNumber(char c) noexcept;
[[nodiscard]] static char getEscape(char c) noexcept;
[[nodiscard]] static bool isValidIdentifier(char c) noexcept;

void ShaderBundleLexer::reset(const CPPRef<IFile>& file) noexcept
{
    _file = file;
    _currentToken = SBPToken::Unknown;
    _strValue = "";
    _intValue = 0;
    _lastRead = '\0';
    _isEOF = false;
    _fileIndex = 0;
    _fileLine = 1;
    _lineIndex = 0;

    (void) readChar();
}

SBPToken ShaderBundleLexer::getNextToken() noexcept
{
    if(skipWhiteSpace())
    {
        return _currentToken = SBPToken::EndOfFile;
    }

    if(isNumber(_lastRead))
    {
        if(readUnsignedInteger())
        { return _currentToken = SBPToken::EndOfFile; }
        _currentToken = SBPToken::UnsignedIntegerLiteral;
    }
    else if(_lastRead == '-')
    {
        if(readInteger())
        { return _currentToken = SBPToken::EndOfFile; }
        _currentToken = SBPToken::IntegerLiteral;
    }
    else if(isValidIdentifier(_lastRead))
    {
        if(readIdentifier())
        { return _currentToken = SBPToken::EndOfFile; }

#define TOKEN_STR_CASE(_STR, _TOKEN) STR_CASE(_STR, { return _currentToken = SBPToken::_TOKEN; })
#define TOKEN_BLOCK_STR_CASE(_TOKEN) STR_CASE(#_TOKEN, { return _currentToken = SBPToken::_TOKEN##Block; })
#define RM_STR_CASE(_API) STR_CASE(#_API, {\
        _rmValue = RenderingMode::Mode::_API; \
        return _currentToken = SBPToken::RenderingMode; \
    })
#define CRM_STR_CASE(_TOKEN) STR_CASE("CRM" #_TOKEN, { \
        _crmToken = CommonRenderingModelToken::_TOKEN; \
        return _currentToken = SBPToken::CRMLiteral; \
    })

        STR_SWITCH(_strValue, {
            TOKEN_BLOCK_STR_CASE(Vertex)
            TOKEN_BLOCK_STR_CASE(TessellationControl)
            TOKEN_STR_CASE("Hull", TessellationControlBlock)
            TOKEN_BLOCK_STR_CASE(TessellationEvaluation)
            TOKEN_STR_CASE("Domain", TessellationEvaluationBlock)
            TOKEN_BLOCK_STR_CASE(Geometry)
            TOKEN_BLOCK_STR_CASE(Pixel)
            TOKEN_STR_CASE("Fragment", PixelBlock)
            TOKEN_BLOCK_STR_CASE(Uniforms)
            TOKEN_BLOCK_STR_CASE(Textures)
            TOKEN_BLOCK_STR_CASE(Inputs)
            TOKEN_BLOCK_STR_CASE(Outputs)
            TOKEN_STR_CASE("File", File)
            RM_STR_CASE(DirectX10)
            RM_STR_CASE(DirectX11)
            RM_STR_CASE(DirectX12)
            RM_STR_CASE(DirectX12_1)
            RM_STR_CASE(Vulkan)
            RM_STR_CASE(OpenGL4_2)
            RM_STR_CASE(OpenGL4_3)
            RM_STR_CASE(OpenGL4_4)
            RM_STR_CASE(OpenGL4_5)
            RM_STR_CASE(OpenGL4_6)
            CRM_STR_CASE(InputPosition)
            CRM_STR_CASE(InputNormal)
            CRM_STR_CASE(InputTangent)
            CRM_STR_CASE(InputTextureCoord)
            CRM_STR_CASE(InputBoneID)
            CRM_STR_CASE(InputBoneWeight)
            CRM_STR_CASE(OutputNormal)
            CRM_STR_CASE(OutputDiffuse)
            CRM_STR_CASE(OutputAlbedo)
            CRM_STR_CASE(OutputPBRCompound)
            CRM_STR_CASE(OutputEmissivity)
            CRM_STR_CASE(OutputPosition)
            CRM_STR_CASE(InputFBPosition)
            CRM_STR_CASE(InputFBTextureCoord)
            CRM_STR_CASE(OutputFBColor)
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
        { return _currentToken = SBPToken::Identifier; })
    }
    else if(_lastRead == '\"')
    {
        if(readString())
        { return _currentToken = SBPToken::EndOfFile; }
        _currentToken = SBPToken::StringLiteral;
    }
    else
    {
        _cValue = _lastRead;
        (void) readChar();
        _currentToken = SBPToken::Character;
    }


    return _currentToken;
}

bool ShaderBundleLexer::skipWhiteSpace() noexcept
{
    while(isWhiteSpace(_lastRead))
    {
        if(readChar()) { return true; }
    }
    return false;
}

bool ShaderBundleLexer::readInteger() noexcept
{
    bool negative = false;
    if(_lastRead == '-')
    {
        negative = true;
        if(readChar()) { return true; }
    }
    i32 x = static_cast<i32>(_lastRead - '0');
    while(true)
    {
        if(readChar()) { return true; }
        if(isDelimiter(_lastRead))
        {
            if(negative)
            {
                x *= -1;
            }
            _intValue = x;
            return false;
        }
        
        if(isNumber(_lastRead))
        {
            x *= 10;
            x += static_cast<i32>(_lastRead - '0');
        }
        else
        { return true; }
    }
}

bool ShaderBundleLexer::readUnsignedInteger() noexcept
{
    u32 x = static_cast<u32>(_lastRead - '0');
    while(true)
    {
        if(readChar()) { return true; }
        if(isDelimiter(_lastRead))
        {
            _uintValue = x;
            return false;
        }

        if(isNumber(_lastRead))
        {
            x *= 10;
            x += static_cast<u32>(_lastRead - '0');
        }
        else
        { return true; }
    }
}

bool ShaderBundleLexer::readString() noexcept
{
    StringBuilder sb;
    while(true)
    {
        if(readChar()) { return true; }
        if(_lastRead == '"')
        {
            _strValue = sb.toString();
            (void) readChar();
            return false;
        }

        if(_lastRead == '\\')
        {
            if(readChar()) { return false; }
            sb.append(getEscape(_lastRead));
        }
        else
        { sb.append(_lastRead); }
    }
}

bool ShaderBundleLexer::readIdentifier() noexcept
{
    StringBuilder sb;
    sb.append(_lastRead);
    while(true)
    {
        if(readChar()) { return true; }
        if(isDelimiter(_lastRead))
        {
            _strValue = sb.toString();
            return false;
        }

        if(isValidIdentifier(_lastRead))
        { sb.append(_lastRead); }
        else
        { return true; }
    }
}

bool ShaderBundleLexer::readChar() noexcept
{
    if(_isEOF) { return true; }
    const int ci = _file->readChar();
    if(ci == -1) { return _isEOF = true; }
    _lastRead = static_cast<char>(ci);

    ++_fileIndex;
    ++_lineIndex;
    if(ci == '\n')
    {
        ++_fileLine;
        _lineIndex = 0;
    }

    return false;
}

[[nodiscard]] static bool isWhiteSpace(const char c) noexcept
{
    switch(c)
    {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
            return true;
        default: return false;
    }
}

[[nodiscard]] static bool isDelimiter(const char c) noexcept
{
    switch(c)
    {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
        case ':':
        case ',':
            return true;
        default: return false;
    }
}

[[nodiscard]] static bool isNumber(const char c) noexcept
{
    return c >= '0' && c <= '9';
}

[[nodiscard]] static char getEscape(char c) noexcept
{
    switch(c)
    {
        case '\\': return '\\';
        case 'r':  return '\r';
        case 'n':  return '\n';
        case '0':  return '\0';
        case 't':  return '\t';
        case '\"': return '\"';
        case '\'': return '\'';
        default:   return static_cast<char>(0x7F);
    }
}

[[nodiscard]] static bool isValidIdentifier(char c) noexcept
{
    if(c >= 'A' && c <= 'Z')
    { return true; }
    if(c >= 'a' && c <= 'z')
    { return true; }
    if(c >= '0' && c <= '9')
    { return true; }
    if(c == '_')
    { return true; }
    return false;
}
