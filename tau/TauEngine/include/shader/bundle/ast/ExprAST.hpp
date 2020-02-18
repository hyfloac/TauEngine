#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>

#include "DLL.hpp"

class IShaderBundleVisitor;

class TAU_DLL ExprAST
{
    DEFAULT_DESTRUCT_VI(ExprAST);
    DELETE_COPY(ExprAST);
protected:
    NullableStrongRef<ExprAST> _next;
public:
    inline ExprAST(const NullableStrongRef<ExprAST>& next) noexcept
        : _next(next)
    { }

    [[nodiscard]] const NullableStrongRef<ExprAST>& next() const noexcept { return _next; }
    [[nodiscard]] NullableStrongRef<ExprAST>& next() noexcept { return _next; }

    virtual void visit(IShaderBundleVisitor& visitor) const noexcept
    { _next->visit(visitor); }
};

