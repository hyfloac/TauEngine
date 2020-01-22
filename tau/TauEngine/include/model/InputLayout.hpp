#pragma once

#include "DLL.hpp"
#include "BufferDescriptor.hpp"
#include <RunTimeType.hpp>

#define INPUT_LAYOUT_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                      RTT_IMPL(_TYPE, IInputLayout)

#define INPUT_LAYOUT_IMPL(_TYPE) INPUT_LAYOUT_IMPL_BASE(_TYPE)

class TAU_DLL NOVTABLE IInputLayout
{
    DEFAULT_DESTRUCT_VI(IInputLayout);
    DELETE_COPY(IInputLayout);
protected:
    DynArray<InputLayoutDescriptor> _descriptors;
protected:
    IInputLayout(const DynArray<InputLayoutDescriptor>& descriptors) noexcept
        : _descriptors(descriptors)
    { }
public:
    [[nodiscard]] const DynArray<InputLayoutDescriptor>& descriptors() const noexcept { return _descriptors; }

    RTT_BASE_IMPL(IInputLayout);
    RTT_BASE_CHECK(IInputLayout);
    RTT_BASE_CAST(IInputLayout);
};

class TAU_DLL NOVTABLE IInputLayoutBuilder
{
    DEFAULT_DESTRUCT_VI(IInputLayoutBuilder);
    DELETE_COPY(IInputLayoutBuilder);
public:
    enum class Error
    {
        NoError = 0,
        DescriptorsNotFinalized,
        MemoryAllocationFailure,
        MultipleSemanticOfInvalidType
    };
protected:
    DynArray<InputLayoutDescriptor> _descriptors;
protected:
    IInputLayoutBuilder(const uSys numDescriptors)
        : _descriptors(numDescriptors)
    { }
public:
    [[nodiscard]] const InputLayoutDescriptor* getLayoutDescriptor(uSys index) const noexcept;
    [[nodiscard]] InputLayoutDescriptor* getLayoutDescriptor(uSys index) noexcept;

    void setLayoutDescriptor(uSys index, ShaderDataType::Type type, ShaderSemantic::Semantic semantic) noexcept;

    [[nodiscard]] virtual IInputLayout* build([[tau::out]] Error* error = null) noexcept = 0;
};
