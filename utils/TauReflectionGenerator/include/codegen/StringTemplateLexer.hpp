#pragma once

#include <String.hpp>
#include <istream>

namespace tau { namespace codegen { namespace string { 

struct SourceLocation final
{
    DEFAULT_DESTRUCT(SourceLocation);
    DEFAULT_CM_PU(SourceLocation);
public:
    /**
     * The current index within the file.
     */
    uSys index;
    /**
     * The current line within the file.
     */
    uSys line;
    /**
     * The current index within the current line.
     */
    uSys column;

    SourceLocation(const uSys _index = 0, const uSys _line = 0, const uSys _column = 0) noexcept
        : index(_index)
        , line(_line)
        , column(_column)
    { }

    void increment(const bool _resetLine = false) noexcept
    {
        ++index;
        ++column;
        if(_resetLine)
        { resetLine(); }
    }

    void resetLine() noexcept
    {
        ++line;
        column = 0;
    }
};

struct SourceRange final
{
    DEFAULT_DESTRUCT(SourceRange);
    DEFAULT_CM_PU(SourceRange);
public:
    /**
     * The begin location, inclusive.
     */
    SourceLocation begin;
    /**
     * The end location, exclusive.
     */
    SourceLocation end;

    SourceRange() noexcept = default;

    SourceRange(const SourceLocation& _begin, const SourceLocation& _end) noexcept
        : begin(_begin)
        , end(_end)
    { }
};

enum class Token
{
    EndOfFile = -1,
    Unknown = 0,
    Identifier,
    BeginFragment,
    EndFragment,
    BeginLoop,
    EndLoop,
    BeginIf,
    Else,
    ElseIf,
    EndIf,
    Continue,
    Break,
    LoopIndex,
    Stringify,
    TextBlock,
    Character,
    StringLiteral,
    CharacterLiteral,
    IntegerLiteral
};

class Lexer final
{
    DEFAULT_DESTRUCT(Lexer);
    DELETE_CM(Lexer);
public:
    enum class CommentState
    {
        NoComment = 0,
        LineComment,
        BlockComment
    };
private:
    ::std::istream& _file;
    Token _currentToken;
    DynString _strValue;
    union
    {
        char _charValue;
        int _intValue;
    };

    bool _inControlSequence;
    CommentState _commentState;
    char _escapeChar;

    char _lastRead;
    bool _isEOF;

    SourceLocation _currentLocation;
    SourceRange _currentLocationRange;
public:
    Lexer(::std::istream& file, const char escapeChar = '%') noexcept
        : _file(file)
        , _currentToken(Token::Unknown)
        , _strValue("")
        , _charValue('\0')
        , _inControlSequence(false)
        , _commentState(CommentState::NoComment)
        , _escapeChar(escapeChar)
        , _lastRead('\0')
        , _isEOF(false)
    { (void) readChar(); }

    [[nodiscard]] Token currentToken() const noexcept { return _currentToken; }
    [[nodiscard]] const DynString& strValue() const noexcept { return _strValue; }
    [[nodiscard]] const DynString& identifierValue() const noexcept { return _strValue; }
    [[nodiscard]] char charValue() const noexcept { return _charValue; }
    [[nodiscard]] int intValue() const noexcept { return _intValue; }

    [[nodiscard]] bool& inControlSequence()       noexcept { return _inControlSequence; }
    [[nodiscard]] bool  inControlSequence() const noexcept { return _inControlSequence; }

    [[nodiscard]] const SourceLocation& currentLocation() const noexcept { return _currentLocation; }
    [[nodiscard]] const SourceRange& currentTokenRange() const noexcept { return _currentLocationRange; }
    
    [[nodiscard]] Token getNextToken() noexcept;
private:
    [[nodiscard]] bool skipWhiteSpace() noexcept;

    [[nodiscard]] bool readInteger() noexcept;

    [[nodiscard]] bool readString() noexcept;

    [[nodiscard]] bool readIdentifier() noexcept;

    [[nodiscard]] bool readTextBlock() noexcept;

    /**
     * Returns true if EOF is encountered.
     *
     * Every read function acts the same way.
     */
    [[nodiscard]] bool readChar() noexcept;
};

} } }
