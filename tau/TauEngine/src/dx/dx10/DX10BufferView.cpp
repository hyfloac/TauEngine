#include "dx/dx10/DX10BufferView.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10DescriptorHeap.hpp"

UniformBufferView DX10BufferViewBuilder::build(const UniformBufferViewArgs& args, Error* const error, DescriptorTable table, const uSys tableIndex) const noexcept
{
    ERROR_CODE_COND_N(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_N(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);
    ERROR_CODE_COND_N(!table.raw, Error::DescriptorTableIsNull);

    DX10Resource* const resource = RTTD_CAST(args.buffer, DX10Resource, IResource);
    ERROR_CODE_COND_N(!resource, Error::InternalError);

    DX10ResourceBuffer* const buffer = static_cast<DX10ResourceBuffer*>(resource);

    DX10DescriptorTable* const dxTable = table.get<DX10DescriptorTable>();
    ID3D10Buffer** const dxBuffer = new(dxTable->bufferViews() + tableIndex) ID3D10Buffer* (buffer->d3dBuffer());
    buffer->d3dBuffer()->AddRef();

    ERROR_CODE_V(Error::NoError, dxBuffer);
}
#endif
