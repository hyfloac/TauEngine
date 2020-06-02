#include "gl/GLInputLayout.hpp"

GLInputLayout* GLInputLayoutBuilder::build(const InputLayoutArgs& args, Error* error) noexcept
{
    GLInputLayout* const inputLayout = new(::std::nothrow) GLInputLayout;
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

GLInputLayout* GLInputLayoutBuilder::build(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    GLInputLayout* const inputLayout = allocator.allocateT<GLInputLayout>();
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

CPPRef<IInputLayout> GLInputLayoutBuilder::buildCPPRef(const InputLayoutArgs& args, Error* error) noexcept
{
    const CPPRef<GLInputLayout> inputLayout = CPPRef<GLInputLayout>(new(::std::nothrow) GLInputLayout);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

NullableRef<IInputLayout> GLInputLayoutBuilder::buildTauRef(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    const NullableRef<GLInputLayout> inputLayout(allocator);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

NullableStrongRef<IInputLayout> GLInputLayoutBuilder::buildTauSRef(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    const NullableStrongRef<GLInputLayout> inputLayout(allocator);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}
