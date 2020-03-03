#pragma once

#include <String.hpp>

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"

enum class BlockType
{
    Global = 0,
    Vertex,
    TessellationControl,
    TessellationEvaluation,
    Geometry,
    Pixel,
    Uniforms,
    Textures,
    Inputs,
    Outputs,
    STAGE_BLOCK_BEGIN = Vertex,
    STAGE_BLOCK_END = Pixel
};

class TAU_DLL BlockExprAST : public ExprAST
{
    DEFAULT_DESTRUCT_VI(BlockExprAST);
    DELETE_COPY(BlockExprAST);
protected:
    NullableStrongRef<ExprAST> _container;
public:
    inline BlockExprAST(const NullableStrongRef<ExprAST>& next, const NullableStrongRef<ExprAST>& container) noexcept
        : ExprAST(next), _container(container)
    { }

    [[nodiscard]] const NullableStrongRef<ExprAST>& container() const noexcept { return _container; }
    [[nodiscard]] NullableStrongRef<ExprAST>& container() noexcept { return _container; }
};

class TAU_DLL TypedBlockExprAST final : public BlockExprAST
{
    DEFAULT_DESTRUCT(TypedBlockExprAST);
    DELETE_COPY(TypedBlockExprAST);
private:
    BlockType _type;
public:
    inline TypedBlockExprAST(const NullableStrongRef<ExprAST>& next, const NullableStrongRef<ExprAST>& container, const BlockType type) noexcept
        : BlockExprAST(next, container), _type(type)
    { }

    [[nodiscard]] BlockType type() const noexcept { return _type; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL NamedBlockExprAST final : public BlockExprAST
{
    DEFAULT_DESTRUCT(NamedBlockExprAST);
    DELETE_COPY(NamedBlockExprAST);
private:
    DynString _name;
public:
    inline NamedBlockExprAST(const NullableStrongRef<ExprAST>& next, const NullableStrongRef<ExprAST>& container, const DynString& name) noexcept
        : BlockExprAST(next, container), _name(name)
    { }

    [[nodiscard]] const DynString& name() const noexcept { return _name; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};


