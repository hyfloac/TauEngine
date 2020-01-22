#pragma once

#include "model/InputLayout.hpp"

class TAU_DLL GLInputLayout final : public IInputLayout
{
    DEFAULT_DESTRUCT(GLInputLayout);
    INPUT_LAYOUT_IMPL(GLInputLayout);
private:
public:
    GLInputLayout(const DynArray<InputLayoutDescriptor>& descriptors) noexcept
        : IInputLayout(descriptors)
    { }
};

class TAU_DLL GLInputLayoutBuilder final : public IInputLayoutBuilder
{
    DEFAULT_DESTRUCT(GLInputLayoutBuilder);
    DELETE_COPY(GLInputLayoutBuilder);
public:
    GLInputLayoutBuilder(const uSys numDescriptors) noexcept
        : IInputLayoutBuilder(numDescriptors)
    { }

    [[nodiscard]] GLInputLayout* build([[tau::out]] Error* error) noexcept override;
};
