#include "gl/GLBufferView.hpp"

#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLResourceBuffer.hpp"

UniformBufferView GLBufferViewBuilder::build(const UniformBufferViewArgs& args, Error* const error, DescriptorTable table, const uSys tableIndex) const noexcept
{
    ERROR_CODE_COND_N(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_N(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);

    ERROR_CODE_COND_N(!table.raw, Error::DescriptorTableIsNull);
    GLDescriptorTable* const glTable = table.get<GLDescriptorTable>();
    ERROR_CODE_COND_N(glTable->type() != DescriptorType::UniformBufferView, Error::InternalError);

    GLResource* const glResource = RTTD_CAST(args.buffer, GLResource, IResource);
    ERROR_CODE_COND_N(!glResource, Error::InternalError);

    GLResourceBuffer* const buffer = static_cast<GLResourceBuffer*>(glResource);

    glTable->uniViews()[tableIndex] = buffer->buffer();

    ERROR_CODE_V(Error::NoError, &glTable->uniViews()[tableIndex]);
}
