#pragma once

#include "ShaderBundleLexer.hpp"
#include "ast/AST.hpp"

namespace sbp {
class AST;
class FileAST;
class UniformBlockExprAST;
class TextureParamsBlockAST;
class ShaderStageBlockAST;
class APIBlockAST;
};

class TAU_DLL ShaderBundleParser
{
    DEFAULT_DESTRUCT(ShaderBundleParser);
    DELETE_COPY(ShaderBundleParser);
    DEFAULT_MOVE_PU(ShaderBundleParser);
public:
    enum Error
    {
        NoError = 0,
        InvalidCharacter,
        UnexpectedEOF,
        DuplicateDeclaration,
        InvalidBlock,
        InvalidToken,
        InvalidCRM,
        UnexpectedEndOfBlock
    };
private:
    ShaderBundleLexer _lexer;
    NullableStrongRef<sbp::AST> _ast;

    Error _error;
    const char* _errorMsg;
    uSys _errorIndex;
    uSys _errorLine;
    uSys _errorLineIndex;
    uSys _codeLine;
public:
    inline ShaderBundleParser(const ShaderBundleLexer& lexer) noexcept
        : _lexer(lexer)
        , _ast(null)
        , _error(Error::NoError)
        , _errorMsg(null)
        , _errorIndex(0)
        , _errorLine(0)
        , _errorLineIndex(0)
        , _codeLine(0)
    { }

    NullableStrongRef<sbp::AST> parse([[tau::out]] Error* error = null) noexcept;

    [[nodiscard]] const NullableStrongRef<sbp::AST>& ast() const noexcept { return _ast; }

    [[nodiscard]] Error error() const noexcept { return _error; }
    [[nodiscard]] const char* errorMsg() const noexcept { return _errorMsg; }
    [[nodiscard]] uSys errorIndex() const noexcept { return _errorIndex; }
    [[nodiscard]] uSys errorLine() const noexcept { return _errorLine; }
    [[nodiscard]] uSys errorLineIndex() const noexcept { return _errorLineIndex; }
    [[nodiscard]] uSys codeLine() const noexcept { return _codeLine; }
private:
    NullableStrongRef<sbp::APIBlockAST> parseAPIBlock() noexcept;
    void parseAPIBlockContents(NullableStrongRef<sbp::APIBlockAST> block) noexcept;

    NullableStrongRef<sbp::ShaderStageBlockAST> parseShaderBlock() noexcept;
    void parseShaderContents(NullableStrongRef<sbp::ShaderStageBlockAST> block) noexcept;

    NullableStrongRef<sbp::UniformBlockExprAST> parseUniformsBlock() noexcept;

    NullableStrongRef<sbp::TextureParamsBlockAST> parseTexturesBlock() noexcept;

    NullableStrongRef<sbp::FileAST> parseFile() noexcept;

    [[nodiscard]] static bool isTextureCRM(CommonRenderingModelToken token) noexcept;
    [[nodiscard]] static bool isUniformCRM(CommonRenderingModelToken token) noexcept;
};
