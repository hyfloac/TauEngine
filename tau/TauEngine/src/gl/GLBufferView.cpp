#include "gl/GLBufferView.hpp"

#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLResourceBuffer.hpp"

UniformBufferView GLBufferViewBuilder::build(const UniformBufferViewArgs& args, CPUDescriptorHandle handle, Error* error) const noexcept
{
    ERROR_CODE_COND_N(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_N(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    ERROR_CODE_COND_N(!handle, Error::DescriptorTableIsNull);

    const GLResource* const glResource = RTTD_CAST(args.buffer.get(), GLResource, IResource);
    ERROR_CODE_COND_N(!glResource, Error::InternalError);

    const GLResourceBuffer* const buffer = static_cast<const GLResourceBuffer*>(glResource);

    GLuint* const view = new(handle) GLuint(buffer->buffer());

    ERROR_CODE_V(Error::NoError, view);
}
