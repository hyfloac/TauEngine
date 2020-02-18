#pragma once

#include <String.hpp>
#include <IFile.hpp>

#include "DLL.hpp"

enum class SBPToken
{
    EndOfFile = -1,
    Unknown = 0,
    Identifier,
    VertexBlock,
    TessellationControlBlock,
    TessellationEvaluationBlock,
    GeometryBlock,
    PixelBlock,
    UniformsBlock,
    TexturesBlock,
    File,
    StringLiteral,
    IntegerLiteral,
    Character
};

class TAU_DLL ShaderBundleLexer
{
private:
    Ref<IFile> _file;
    SBPToken _currentToken;
    DynString _strValue;
    i32 _intValue;
    char _cValue;

    char _lastRead;
    bool _isEOF;
public:
    inline ShaderBundleLexer(const Ref<IFile>& file) noexcept
        : _file(file), _currentToken(SBPToken::Unknown),
        _strValue(""), _intValue(0), _cValue('\0'), _lastRead('\0'), _isEOF(false)
    {
        (void) readChar();
    }

    [[nodiscard]] SBPToken currentToken() const noexcept { return _currentToken; }
    [[nodiscard]] const DynString& strValue() const noexcept { return _strValue; }
    [[nodiscard]] const DynString& identifierValue() const noexcept { return _strValue; }
    [[nodiscard]] i32 intValue() const noexcept { return _intValue; }
    [[nodiscard]] char cValue() const noexcept { return _cValue; }
    [[nodiscard]] bool isEOF() const noexcept { return _isEOF; }

    [[nodiscard]] SBPToken getNextToken() noexcept;
private:
    [[nodiscard]] bool skipWhiteSpace() noexcept;

    [[nodiscard]] bool readInteger() noexcept;

    [[nodiscard]] bool readString() noexcept;

    [[nodiscard]] bool readIdentifier() noexcept;

    [[nodiscard]] bool readChar() noexcept;
};
