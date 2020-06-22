#include "dx/dx10/DX10BufferView.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10DescriptorHeap.hpp"

UniformBufferView DX10BufferViewBuilder::build(const UniformBufferViewArgs& args, Error* const error, const DescriptorTable table, const uSys tableIndex) const noexcept
{
    ERROR_CODE_COND_V(!args.buffer, Error::BufferIsNull, { null });
    ERROR_CODE_COND_V(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer, { null });
    ERROR_CODE_COND_V(!table.raw, Error::DescriptorTableIsNull, { null });

    DX10Resource* const resource = RTTD_CAST(args.buffer, DX10Resource, IResource);
    ERROR_CODE_COND_V(!resource, Error::InternalError, { null });

    DX10ResourceBuffer* const buffer = static_cast<DX10ResourceBuffer*>(resource);

    DX10DescriptorTable* const dxTable = reinterpret_cast<DX10DescriptorTable*>(table.raw);
    ID3D10Buffer** const dxBuffer = new(dxTable->placement() + sizeof(ID3D10Buffer*) * tableIndex) ID3D10Buffer* (buffer->d3dBuffer());
    buffer->d3dBuffer()->AddRef();

    ERROR_CODE_V(Error::NoError, { dxBuffer });
}
#endif
