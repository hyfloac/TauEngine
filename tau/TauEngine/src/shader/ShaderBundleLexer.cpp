#include "shader/bundle/ShaderBundleLexer.hpp"

[[nodiscard]] static bool isWhiteSpace(char c) noexcept;
[[nodiscard]] static bool isDelimiter(char c) noexcept;
[[nodiscard]] static bool isNumber(char c) noexcept;
[[nodiscard]] static char getEscape(char c) noexcept;
[[nodiscard]] static bool isValidIdentifier(char c) noexcept;

SBPToken ShaderBundleLexer::getNextToken() noexcept
{
    if(skipWhiteSpace())
    {
        return _currentToken = SBPToken::EndOfFile;
    }

    if(isNumber(_lastRead))
    {
        if(readInteger())
        { return _currentToken = SBPToken::EndOfFile; }
        _currentToken = SBPToken::IntegerLiteral;
    }
    else if(isValidIdentifier(_lastRead))
    {
        if(readIdentifier())
        { return _currentToken = SBPToken::EndOfFile; }

        STR_SWITCH(_strValue, {
            STR_CASE("Vertex", { return _currentToken = SBPToken::VertexBlock; })
            STR_CASE("TessellationControl", { return _currentToken = SBPToken::TessellationControlBlock; })
            STR_CASE("Hull", { return _currentToken = SBPToken::TessellationControlBlock; })
            STR_CASE("TessellationEvaluation", { return _currentToken = SBPToken::TessellationEvaluationBlock; })
            STR_CASE("Domain", { return _currentToken = SBPToken::TessellationEvaluationBlock; })
            STR_CASE("Geometry", { return _currentToken = SBPToken::GeometryBlock; })
            STR_CASE("Pixel", { return _currentToken = SBPToken::PixelBlock; })
            STR_CASE("Fragment", { return _currentToken = SBPToken::PixelBlock; })
            STR_CASE("File", { return _currentToken = SBPToken::File; })
            STR_CASE("UniformsBlock", { return _currentToken = SBPToken::UniformsBlock; })
            STR_CASE("TexturesBlock", { return _currentToken = SBPToken::TexturesBlock; })
        },
        {
            _currentToken = SBPToken::Identifier;
        })
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
    int x = static_cast<int>(_lastRead - '0');
    while(true)
    {
        if(readChar()) { return true; }
        if(isDelimiter(_lastRead))
        {
            _intValue = x;
            return false;
        }
        
        if(isNumber(_lastRead))
        {
            x *= 10;
            x += static_cast<int>(_lastRead - '0');
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
