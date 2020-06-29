/**
 *@file
 */
#pragma once

#include <Objects.hpp>
#include "DLL.hpp"

namespace sbp {
class AST;
class RootAST;
class FileAST;
class UniformBindingAST;
class TextureParamsBlockAST;
class ShaderStageBlockAST;
class APIBlockAST;
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
    virtual void visit(const sbp::AST* expr) noexcept;
    virtual void visit(const sbp::AST& expr) noexcept;
    virtual void visit(const sbp::RootAST& expr) noexcept;

    virtual void visit(const sbp::FileAST& expr) noexcept = 0;
    virtual void visit(const sbp::UniformBindingAST& expr) noexcept = 0;
    virtual void visit(const sbp::TextureParamsBlockAST& expr) noexcept = 0;
    virtual void visit(const sbp::ShaderStageBlockAST& expr) noexcept = 0;
    virtual void visit(const sbp::APIBlockAST& expr) noexcept = 0;
};
