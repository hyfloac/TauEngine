#pragma once

#include <String.hpp>

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"

class TAU_DLL FileExprAST final : public ExprAST
{
    DEFAULT_DESTRUCT(FileExprAST);
    DELETE_COPY(FileExprAST);
private:
    DynString _filePath;
public:
    FileExprAST(const NullableStrongRef<ExprAST>& next, const DynString& filePath) noexcept
        : ExprAST(next), _filePath(filePath)
    { }

    [[nodiscard]] const DynString& filePath() const noexcept { return _filePath; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
