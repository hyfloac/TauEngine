#pragma once

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"
#include <Safeties.hpp>

namespace sbp {
class TAU_DLL RootExprAST final : public ExprAST
{
    DEFAULT_DESTRUCT(RootExprAST);
    DEFAULT_CM_PU(RootExprAST);
private:
    NullableStrongRef<APIBlockExprAST> _node;
public:
    inline RootExprAST() noexcept
        : _node(null)
    { }

    [[nodiscard]]       NullableStrongRef<APIBlockExprAST>& node()       noexcept { return _node; }
    [[nodiscard]] const NullableStrongRef<APIBlockExprAST>& node() const noexcept { return _node; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
