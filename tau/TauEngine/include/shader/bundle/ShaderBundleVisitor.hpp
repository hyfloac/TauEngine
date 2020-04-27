/**
 *@file
 */
#pragma once

#include <Safeties.hpp>
#include <Objects.hpp>
#include "DLL.hpp"
#include "shader/ShaderBindMap.hpp"

class IRenderingContext;
class IGraphicsInterface;
class ExprAST;
class FileExprAST;
class BlockExprAST;
class TypedBlockExprAST;
class NamedBlockExprAST;
class ShaderIOPointExprAST;
class ShaderIOMapPointExprAST;
class ShaderIOBindPointExprAST;
class IShaderProgram;

/**
 * A basic visitor for shader bundles.
 *
 *   Contains a default implementation of each visit function.
 * For all visits of a specific type the default
 * implementation simply visits the next expression. The visit
 * function for the basic `ExprAST` type calls visit on the
 * expr variable. This is because `ExprAST` is not a concrete
 * type, and thus visiting it will get back to the visitor
 * with the more concrete type. All of this functionality can
 * of course be overriden.
 */
class TAU_DLL NOVTABLE IShaderBundleVisitor
{
    DEFAULT_CONSTRUCT_PO(IShaderBundleVisitor);
    DEFAULT_DESTRUCT_VI(IShaderBundleVisitor);
    DELETE_COPY(IShaderBundleVisitor);
public:
    virtual CPPRef<IShaderProgram> getShader(IRenderingContext& ctx, IGraphicsInterface& gi) noexcept = 0;
    [[nodiscard]] virtual ShaderBindMap getBindMap() noexcept = 0;

    virtual void visit(const ExprAST* expr) noexcept;
    virtual void visit(const ExprAST& expr) noexcept;

    virtual void visit(const FileExprAST& expr) noexcept;
    virtual void visit(const TypedBlockExprAST& expr) noexcept;
    virtual void visit(const NamedBlockExprAST& expr) noexcept;
    virtual void visit(const ShaderIOMapPointExprAST& expr) noexcept;
    virtual void visit(const ShaderIOBindPointExprAST& expr) noexcept;
protected:
    virtual void visitNext(const ExprAST& expr) noexcept;
};
