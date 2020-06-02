#pragma once

#include <NumTypes.hpp>
#include <String.hpp>

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleLexer.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"

namespace sbp {
class TAU_DLL TAU_NOVTABLE ShaderIOPointExprAST : public ExprAST
{
    DEFAULT_DESTRUCT_VI(ShaderIOPointExprAST);
    DELETE_COPY(ShaderIOPointExprAST);
private:
    NullableStrongRef<ShaderIOPointExprAST> _next;
    CommonRenderingModelToken _crmTarget;
public:
    inline ShaderIOPointExprAST(const NullableStrongRef<ShaderIOPointExprAST>& next, const CommonRenderingModelToken crmTarget) noexcept
        : _next(next)
        , _crmTarget(crmTarget)
    { }

    [[nodiscard]]       NullableStrongRef<ShaderIOPointExprAST>& next()       noexcept { return _next; }
    [[nodiscard]] const NullableStrongRef<ShaderIOPointExprAST>& next() const noexcept { return _next; }

    [[nodiscard]] CommonRenderingModelToken crmTarget() const noexcept { return _crmTarget; }
};

class TAU_DLL ShaderIOMapPointExprAST final : public ShaderIOPointExprAST
{
    DEFAULT_DESTRUCT(ShaderIOMapPointExprAST);
    DELETE_COPY(ShaderIOMapPointExprAST);
private:
    i32 _shaderBind;
public:
    inline ShaderIOMapPointExprAST(const NullableStrongRef<ShaderIOPointExprAST>& next, const CommonRenderingModelToken crmTarget, const i32 shaderBind) noexcept
        : ShaderIOPointExprAST(next, crmTarget)
        , _shaderBind(shaderBind)
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
    inline ShaderIOBindPointExprAST(const NullableStrongRef<ShaderIOPointExprAST>& next, const CommonRenderingModelToken crmTarget, const DynString& uniformName) noexcept
        : ShaderIOPointExprAST(next, crmTarget)
        , _uniformName(uniformName)
    { }

    [[nodiscard]] const DynString& uniformName() const noexcept { return _uniformName; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
