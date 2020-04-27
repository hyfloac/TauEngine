#pragma once

#include <NumTypes.hpp>
#include <String.hpp>

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"

// enum class ShaderIO
// {
//     Uniform = 0,
//     Texture,
//     Input,
//     Output
// };

class TAU_DLL TAU_NOVTABLE ShaderIOPointExprAST : public ExprAST
{
    DEFAULT_DESTRUCT_VI(ShaderIOPointExprAST);
    DELETE_COPY(ShaderIOPointExprAST);
private:
    // ShaderIO _ioType;
    i32 _bindPoint;
public:
    inline ShaderIOPointExprAST(const NullableStrongRef<ExprAST>& next,/* const ShaderIO ioType,*/ const i32 bindPoint) noexcept
        : ExprAST(next),/* _ioType(ioType),*/ _bindPoint(bindPoint)
    { }

    // [[nodiscard]] ShaderIO ioType() const noexcept { return _ioType; }
    [[nodiscard]] i32 bindPoint() const noexcept { return _bindPoint; }
};

class TAU_DLL ShaderIOMapPointExprAST final : public ShaderIOPointExprAST
{
    DEFAULT_DESTRUCT(ShaderIOMapPointExprAST);
    DELETE_COPY(ShaderIOMapPointExprAST);
private:
    i32 _shaderBind;
public:
    inline ShaderIOMapPointExprAST(const NullableStrongRef<ExprAST>& next,/* const ShaderIO ioType,*/ const i32 bindPoint, const i32 shaderBind) noexcept
        : ShaderIOPointExprAST(next,/* ioType,*/ bindPoint), _shaderBind(shaderBind)
    { }

    [[nodiscard]] i32 shaderBind() const noexcept { return _shaderBind; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL ShaderIOBindPointExprAST final : public ShaderIOPointExprAST
{
    DEFAULT_DESTRUCT(ShaderIOBindPointExprAST);
    DELETE_COPY(ShaderIOBindPointExprAST);
private:
    DynString _uniformName;
public:
    inline ShaderIOBindPointExprAST(const NullableStrongRef<ExprAST>& next,/* const ShaderIO ioType,*/ const i32 bindPoint, const DynString& uniformName) noexcept
        : ShaderIOPointExprAST(next,/* ioType,*/ bindPoint), _uniformName(uniformName)
    { }

    [[nodiscard]] const DynString& uniformName() const noexcept { return _uniformName; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
