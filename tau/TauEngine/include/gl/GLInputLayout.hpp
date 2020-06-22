#pragma once

#include "graphics/InputLayout.hpp"

class TAU_DLL GLInputLayout : public IInputLayout
{
    DEFAULT_CONSTRUCT_PU(GLInputLayout);
    DEFAULT_DESTRUCT(GLInputLayout);
    DEFAULT_CM_PU(GLInputLayout);
public:
    void bind(IRenderingContext & context) noexcept override { }
    void unbind(IRenderingContext & context) noexcept override { }
};

class TAU_DLL GLInputLayoutBuilder : public IInputLayoutBuilder
{
    DEFAULT_CONSTRUCT_PU(GLInputLayoutBuilder);
    DEFAULT_DESTRUCT(GLInputLayoutBuilder);
    DEFAULT_CM_PU(GLInputLayoutBuilder);
public:
    [[nodiscard]] GLInputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] GLInputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IInputLayout> buildCPPRef(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IInputLayout> buildTauRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IInputLayout> buildTauSRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
};
