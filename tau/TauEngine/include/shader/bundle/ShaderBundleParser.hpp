#pragma once

#include "ShaderBundleLexer.hpp"
#include "ast/ExprAST.hpp"

class ExprAST;
class BlockExprAST;
class FileExprAST;
class ShaderIOPointExprAST;

enum class BlockType;

class TAU_DLL ShaderBundleParser
{
private:
    ShaderBundleLexer _lexer;
    NullableStrongRef<ExprAST> _ast;
    BlockType _currentBlock;
public:
    inline ShaderBundleParser(const ShaderBundleLexer& lexer) noexcept
        : _lexer(lexer), _ast(null), _currentBlock(static_cast<BlockType>(0))
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
