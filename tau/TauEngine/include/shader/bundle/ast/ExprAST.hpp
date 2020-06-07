#pragma once

#include <Objects.hpp>

#include "DLL.hpp"

class IShaderBundleVisitor;

namespace sbp {
class TAU_DLL TAU_NOVTABLE ExprAST
{
    DEFAULT_CONSTRUCT_PU(ExprAST);
    DEFAULT_DESTRUCT_VI(ExprAST);
    DEFAULT_CM_PO(ExprAST);
public:
    virtual void visit(IShaderBundleVisitor& visitor) const noexcept = 0;
};
}
