#pragma once


#include <Objects.hpp>

#include "DLL.hpp"

class ExprAST;
class FileExprAST;
class BlockExprAST;
class TypedBlockExprAST;
class NamedBlockExprAST;
class ShaderIOPointExprAST;
class ShaderIOMapPointExprAST;
class ShaderIOBindPointExprAST;

class TAU_DLL NOVTABLE IShaderBundleVisitor
{
    DEFAULT_CONSTRUCT_PO(IShaderBundleVisitor);
    DEFAULT_DESTRUCT_VI(IShaderBundleVisitor);
    DELETE_COPY(IShaderBundleVisitor);
public:
    virtual void visit(const ExprAST& expr) noexcept;

    virtual void visit(const FileExprAST& expr) noexcept { }
    virtual void visit(const TypedBlockExprAST& expr) noexcept { }
    virtual void visit(const NamedBlockExprAST& expr) noexcept { }
    virtual void visit(const ShaderIOMapPointExprAST& expr) noexcept { }
    virtual void visit(const ShaderIOBindPointExprAST& expr) noexcept { }
protected:
    virtual void visitNext(const ExprAST& expr) noexcept;
};
