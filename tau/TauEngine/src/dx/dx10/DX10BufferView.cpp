#include "dx/dx10/DX10BufferView.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10DescriptorHeap.hpp"

UniformBufferView DX10BufferViewBuilder::build(const UniformBufferViewArgs& args, const CPUDescriptorHandle handle, Error* error) const noexcept
{
    ERROR_CODE_COND_N(!args.buffer, Error::BufferIsNull);
    ERROR_CODE_COND_N(args.buffer->resourceType() != EResource::Type::Buffer, Error::ResourceIsNotBuffer);
    ERROR_CODE_COND_N(!handle, Error::DescriptorTableIsNull);

    const DX10Resource* const resource = RTTD_CAST(args.buffer.get(), DX10Resource, IResource);
    ERROR_CODE_COND_N(!resource, Error::InternalError);

    const DX10ResourceBuffer* const buffer = static_cast<const DX10ResourceBuffer*>(resource);

    ID3D10Buffer** const dxBuffer = new(handle) ID3D10Buffer* (buffer->d3dBuffer());
    (*dxBuffer)->AddRef();

    ERROR_CODE_V(Error::NoError, dxBuffer);
}
#endif
