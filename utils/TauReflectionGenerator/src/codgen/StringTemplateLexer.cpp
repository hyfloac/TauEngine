#include "codegen/StringTemplateLexer.hpp"

namespace tau { namespace codegen { namespace string {

[[nodiscard]] static constexpr bool isWhiteSpace(char c) noexcept;
[[nodiscard]] static constexpr bool isDelimiter(char c) noexcept;
[[nodiscard]] static constexpr bool isNumber(char c) noexcept;
[[nodiscard]] static constexpr char getEscape(char c) noexcept;
[[nodiscard]] static constexpr bool isValidIdentifier(char c) noexcept;

#ifdef RETURN
  #undef RETURN
#endif

#define RETURN(_TOKEN) return _currentToken = _TOKEN
#define READ_EOF(_EXP) do { if((_EXP)) { return _currentToken = Token::EndOfFile; } } while(0)

Token Lexer::getNextToken() noexcept
{
    /*   In control sequence treat every special character as a
     * delimiter instead of part of a text block. */
    if(_inControlSequence)
    {
        // Whitespace is ignored in control sequences
        READ_EOF(skipWhiteSpace());

        // Set the token's beginning location (inclusive) for error messages.
        _currentLocationRange.begin = _currentLocation;

        // Is this a number?
        if(isNumber(_lastRead) || _lastRead == '-')
        {
            // Read the number
            READ_EOF(readInteger());
            
            // Set the token's end location (exclusive) for error messages.
            _currentLocationRange.end = _currentLocation;
            RETURN(Token::IntegerLiteral);
        }
        // Is this an identifier?
        else if(isValidIdentifier(_lastRead))
        {
            // Read the identifier
            READ_EOF(readIdentifier());
            
            // Set the token's end location (exclusive) for error messages.
            _currentLocationRange.end = _currentLocation;

    #define TOKEN_STR_CASE(_STR, _TOKEN) STR_CASE(_STR, { return _currentToken = Token::_TOKEN; })

            STR_SWITCH(_strValue, {
                TOKEN_STR_CASE("fragment", BeginFragment)
                TOKEN_STR_CASE("endfragment", EndFragment)
                TOKEN_STR_CASE("loop", BeginLoop)
                TOKEN_STR_CASE("endloop", EndLoop)
                TOKEN_STR_CASE("if", BeginIf)
                TOKEN_STR_CASE("else", Else)
                TOKEN_STR_CASE("elif", ElseIf)
                TOKEN_STR_CASE("endif", EndIf)
                TOKEN_STR_CASE("continue", Continue)
                TOKEN_STR_CASE("break", Break)
                TOKEN_STR_CASE("loopindex", LoopIndex)
                TOKEN_STR_CASE("str", Stringify)
            }, 
            { return _currentToken = Token::Identifier; })

    #undef TOKEN_STR_CASE

            // This shouldn't ever be invoked.
            RETURN(Token::Identifier);
        }
        // Character Literal
        else if(_lastRead == '\'')
        {
            // Read the actual character literal.
            READ_EOF(readChar());

            // Is this an escape character?
            if(_lastRead == '\\')
            {
                // Read the escape character literal.
                READ_EOF(readChar());

                _charValue = getEscape(_lastRead);
            }

            // Read the closing '
            READ_EOF(readChar());
            
            // Set the token's end location (exclusive) for error messages.
            _currentLocationRange.end = _currentLocation;
            
            // Is this properly closed off?
            if(_lastRead != '\'')
            { RETURN(Token::Unknown); }
            
            RETURN(Token::CharacterLiteral);
        }
        // String Literal
        else if(_lastRead == '\"')
        {
            // Read the string.
            READ_EOF(readString());
            
            // Set the token's end location (exclusive) for error messages.
            _currentLocationRange.end = _currentLocation;
            RETURN(Token::StringLiteral);
        }
        // Delimiter Character
        else
        {
            _charValue = _lastRead;

            // Advance the cursor.
            READ_EOF(readChar());
            
            // Set the token's end location (exclusive) for error messages.
            _currentLocationRange.end = _currentLocation;
            RETURN(Token::Character);
        }
    }
    else
    {
        // Set the token's beginning location (inclusive) for error messages.
        _currentLocationRange.begin = _currentLocation;

        // Have we found a control sequence?
        if(_lastRead == _escapeChar)
        {
            // Read the next char.
            READ_EOF(readChar());

            // Is this actually an escape for escape character ('%')
            if(_lastRead == _escapeChar)
            {
                // Read it as if it was a text block.
                READ_EOF(readTextBlock());
                
                // Set the token's end location (exclusive) for error messages.
                _currentLocationRange.end = _currentLocation;

                RETURN(Token::TextBlock);
            }
            else
            {
                _inControlSequence = true;
                _charValue = _escapeChar;
                // Set the token's end location (exclusive) for error messages.
                _currentLocationRange.end = _currentLocation;

                RETURN(Token::Character);
            }
        }
        else
        {
            // Read the text block.
            READ_EOF(readTextBlock());
            
            // Set the token's end location (exclusive) for error messages.
            _currentLocationRange.end = _currentLocation;

            RETURN(Token::TextBlock);
        }
    }
}

#undef READ_EOF
#undef RETURN

bool Lexer::skipWhiteSpace() noexcept
{
    while(isWhiteSpace(_lastRead))
    {
        if(readChar()) { return true; }
    }
    return false;
}

bool Lexer::readInteger() noexcept
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

bool Lexer::readString() noexcept
{
    StringBuilder sb;
    while(true)
    {
        if(readChar()) 
        { return true; }

        if(_lastRead == '"')
        {
            _strValue = sb.toString();
            (void) readChar();
            return false;
        }

        if(_lastRead == '\\')
        {
            if(readChar()) 
            { return false; }
            sb.append(getEscape(_lastRead));
        }
        else
        { sb.append(_lastRead); }
    }
}

bool Lexer::readIdentifier() noexcept
{
    StringBuilder sb;
    sb.append(_lastRead);
    while(true)
    {
        if(readChar()) 
        { return true; }

        if(isDelimiter(_lastRead))
        {
            _strValue = sb.toString();
            return false;
        }

        if(isValidIdentifier(_lastRead))
        { sb.append(_lastRead); }
        else
        {
            _strValue = sb.toString();
            return false;
        }
    }
}

bool Lexer::readTextBlock() noexcept
{
    bool isInString = false;

    StringBuilder sb;
    sb.append(_lastRead);
    while(true)
    {
        // Read next char in stream.
        if(readChar()) 
        { return true; }

        // Have we found a control code, and importantly, are we not in a string.
        // If so we've finished reading this text block.
        if(_lastRead == _escapeChar && !isInString)
        {
            // Do we have a special escape for the percent?
            if(_file.peek() == _escapeChar)
            {
                sb.append(_escapeChar);
                if(readChar())
                { return true; }
                continue;
            }

            _strValue = sb.toString();
            return false;
        }

        // Are we in a comment?
        if(_commentState == CommentState::NoComment)
        {
            // Are we in a string?
            if(isInString)
            {
                // Is the current character an escape code?
                if(_lastRead == '\\')
                {
                    // Store the '\'.
                    sb.append('\\');

                    // Read the next char.
                    // It will be appended at the end of the loop.
                    if(readChar()) 
                    { return true; }
                }
                // Are we at the end of a string?
                else if(_lastRead == '\"' || _lastRead == '\n')
                {
                    isInString = false;
                }
            }
            else
            {
                // Are we at the beginning of a string?
                if(_lastRead == '\"')
                {
                    isInString = true;
                }
            }
        }
        // Are we in a // line comment.
        else if(_commentState == CommentState::LineComment)
        {
            // Are we at the end of the line comment?
            if(_lastRead == '\n')
            {
                _commentState = CommentState::NoComment;
            }
        }
        // Are we in a /* block comment */?
        else if(_commentState == CommentState::BlockComment)
        {
            // Are we potentially at the end of the comment?
            if(_lastRead == '*')
            {
                // Store the '*'.
                sb.append(_lastRead);
                
                // Read the next char.
                // It will be appended at the end of the loop.
                if(readChar()) 
                { return true; }
                
                // Are we actually at the end of the comment?
                if(_lastRead == '/')
                {
                    _commentState = CommentState::NoComment;
                }
            }
        }
        // Are we not in a comment?
        else
        {
            // Are we in a string?
            if(!isInString)
            {
                // Are we potentially at the beginning of a comment?
                if(_lastRead == '/')
                {
                    // Store the '/'.
                    sb.append(_lastRead);
                    
                    // Read the next char.
                    // It will be appended at the end of the loop.
                    if(readChar()) 
                    { return true; }
                    
                    // Are we at the beginning of a // line comment?
                    if(_lastRead == '/')
                    {
                        _commentState = CommentState::NoComment;
                    }
                    // Are we at the beginning of a /* block comment */?
                    else if(_lastRead == '*')
                    {
                        _commentState = CommentState::NoComment;
                    }
                }
            }
        }

        // Store the last character.
        sb.append(_lastRead);
    }
}

bool Lexer::readChar() noexcept
{
    if(_isEOF)
    { return true; }

    const int ci = _file.get();

    if(ci == -1) 
    { return _isEOF = true; }

    _lastRead = static_cast<char>(ci);

    _currentLocation.increment(ci == '\n');

    return false;
}

[[nodiscard]] static constexpr bool isWhiteSpace(const char c) noexcept
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

[[nodiscard]] static constexpr bool isDelimiter(const char c) noexcept
{
    switch(c)
    {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
        case ':':
        case ',':
        case '.':
        case '<':
        case '>':
        case '(':
        case ')':
            return true;
        default: return false;
    }
}

[[nodiscard]] static constexpr bool isNumber(const char c) noexcept
{
    return c >= '0' && c <= '9';
}

[[nodiscard]] static constexpr char getEscape(const char c) noexcept
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

[[nodiscard]] static constexpr bool isValidIdentifier(const char c) noexcept
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

} } }
