#pragma once

#include "DLL.hpp"
#include <NumTypes.hpp>
#include <String.hpp>
#include <IFile.hpp>
#include <utility>

namespace Console {

struct Token final
{
    enum TokenType : u8
    {
        Eof = 1,
        Error,

        Identifier,
        Integer,
        Float,
        String,
        Char
    };

    bool isToken;
    union
    {
        TokenType token;
        char c;
    };

    Token() noexcept
        : isToken(true), token(TokenType::Error)
    { }

    Token(const TokenType _token) noexcept
        : isToken(true), token(_token)
    { }

    Token(const char _c) noexcept
        : isToken(false), c(_c)
    { }
};

union TokenVal final
{
    DynString str;
    float f;
    char c;
    int i;

    TokenVal(DynString str) noexcept
        : str(std::move(str))
    { }

    TokenVal(const float f) noexcept
        : f(f)
    { }

    TokenVal(const char c) noexcept
        : c(c)
    { }

    TokenVal(const int i) noexcept
        : i(i)
    { }

    ~TokenVal() noexcept
    { }
};

class TAU_DLL Lexer final
{
private:
    Ref<IFile> _file;
    Token _lastToken;
    TokenVal _lastTokenVal;
    char _lastChar;
public:
    Lexer(Ref<IFile> file) noexcept
        : _file(std::move(file)), _lastToken(), _lastTokenVal(0), _lastChar('\0')
    {
        const int c = file->readChar();

        if(c < 0)
        {
            _lastToken = Token(Token::Eof);
            _lastChar = '\0';
        }
        else
        {
            _lastChar = static_cast<char>(c);
        }
    }

    [[nodiscard]] Token lastToken() noexcept { return _lastToken; }
    // [[nodiscard]] TokenVal lastTokenVal() noexcept { return _lastTokenVal; }

    [[nodiscard]] Token getNextToken() noexcept;
private:
    void clearWhiteSpace()
    {
        
    }
};

}
