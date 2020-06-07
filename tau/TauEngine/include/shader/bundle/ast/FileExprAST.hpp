#pragma once

#include <String.hpp>

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"

namespace sbp {
class TAU_DLL FileExprAST final : public ExprAST
{
    DEFAULT_DESTRUCT(FileExprAST);
    DEFAULT_CM_PU(FileExprAST);
private:
    DynString _filePath;
public:
    FileExprAST(const DynString& filePath) noexcept
        : _filePath(filePath)
    { }

    [[nodiscard]] const DynString& filePath() const noexcept { return _filePath; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
