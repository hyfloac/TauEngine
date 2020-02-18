#pragma once

#include "ShaderBundleLexer.hpp"
#include "ast/ExprAST.hpp"

class ExprAST;
class BlockExprAST;
class FileExprAST;
class ShaderIOPointExprAST;

class TAU_DLL ShaderBundleParser
{
private:
    ShaderBundleLexer _lexer;
    NullableStrongRef<ExprAST> _ast;
public:
    inline ShaderBundleParser(const ShaderBundleLexer& lexer) noexcept
        : _lexer(lexer), _ast(null)
    { }

    NullableStrongRef<ExprAST> parse() noexcept;

    [[nodiscard]] const NullableStrongRef<ExprAST>& ast() const noexcept { return _ast; }
private:
    NullableStrongRef<ExprAST> parsePrimary() noexcept;

    NullableStrongRef<BlockExprAST> parseBlock() noexcept;
    void parseNextBlockEntry(NullableStrongRef<BlockExprAST> block) noexcept;

    NullableStrongRef<FileExprAST> parseFile() noexcept;

    NullableStrongRef<ShaderIOPointExprAST> parseIOPoint() noexcept;
};
