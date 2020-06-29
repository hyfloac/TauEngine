#pragma once

#include "AST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"
#include <Safeties.hpp>

namespace sbp {
class TAU_DLL RootAST final : public AST
{
    DEFAULT_CONSTRUCT_PU(RootAST);
    DEFAULT_DESTRUCT(RootAST);
    DEFAULT_CM_PU(RootAST);
private:
    NullableStrongRef<APIBlockAST> _node;
public:
    [[nodiscard]]       NullableStrongRef<APIBlockAST>& node()       noexcept { return _node; }
    [[nodiscard]] const NullableStrongRef<APIBlockAST>& node() const noexcept { return _node; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
