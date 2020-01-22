#include "gl/GLInputLayout.hpp"
#include <Safeties.hpp>

GLInputLayout* GLInputLayoutBuilder::build(Error* error) noexcept
{
    GLInputLayout* layout = new(::std::nothrow) GLInputLayout(_descriptors);
    ERROR_CODE_COND_N(!layout, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, layout);
}
