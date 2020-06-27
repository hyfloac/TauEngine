/**
 *@file
 */
#pragma once

#include <Objects.hpp>
#include "DLL.hpp"

namespace sbp {
class ExprAST;
class RootExprAST;
class FileExprAST;
class UniformBlockExprAST;
class TextureParamsBlockExprAST;
class ShaderStageBlockExprAST;
class APIBlockExprAST;
class ShaderIOPointExprAST;
class ShaderIOMapPointExprAST;
class ShaderIOBindPointExprAST;
}

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
    DEFAULT_CM_PO(IShaderBundleVisitor);
public:
    virtual void visit(const sbp::ExprAST* expr) noexcept;
    virtual void visit(const sbp::ExprAST& expr) noexcept;
    virtual void visit(const sbp::RootExprAST& expr) noexcept;

    virtual void visit(const sbp::FileExprAST& expr) noexcept = 0;
    virtual void visit(const sbp::UniformBlockExprAST& expr) noexcept = 0;
    virtual void visit(const sbp::TextureParamsBlockExprAST& expr) noexcept = 0;
    virtual void visit(const sbp::ShaderStageBlockExprAST& expr) noexcept = 0;
    virtual void visit(const sbp::APIBlockExprAST& expr) noexcept = 0;
    virtual void visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept = 0;
    virtual void visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept = 0;
};
