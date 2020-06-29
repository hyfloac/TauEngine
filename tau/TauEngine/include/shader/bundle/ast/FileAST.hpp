#pragma once

#include <String.hpp>

#include "AST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"

namespace sbp {
class TAU_DLL FileAST final : public AST
{
    DEFAULT_DESTRUCT(FileAST);
    DEFAULT_CM_PU(FileAST);
private:
    DynString _filePath;
public:
    FileAST(const DynString& filePath) noexcept
        : _filePath(filePath)
    { }

    [[nodiscard]] const DynString& filePath() const noexcept { return _filePath; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
