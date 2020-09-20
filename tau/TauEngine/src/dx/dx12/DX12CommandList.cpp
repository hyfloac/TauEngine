#include "dx/dx12/DX12CommandList.hpp"

#ifdef _WIN32
#include "dx/dx12/DX12CommandAllocator.hpp"
#include "dx/dx12/DX12Resource.hpp"
#include "dx/dx12/DX12ResourceBuffer.hpp"
#include "dx/dx12/DX12VertexArray.hpp"
#include "dx/dx12/DX12Enums.hpp"
#include "graphics/PipelineState.hpp"
#include "TauConfig.hpp"

void DX12CommandList::reset(const NullableRef<ICommandAllocator>& allocator, const PipelineState* const initialState) noexcept
{
#ifdef TAU_NULL_CHECK
    if(!allocator)
    { return; }
#endif

#ifdef TAU_RTTI_CHECK
    if(!RTT_CHECK(allocator, DX12CommandAllocator))
    { return; }
#endif

    _cmdAllocator = RefCast<DX12CommandAllocator>(allocator);

    ID3D12PipelineState* pipelineState = null;
    if(initialState)
    {
        pipelineState = initialState->get<ID3D12PipelineState>();
    }
    
    _cmdList->Reset(_cmdAllocator->allocator(), pipelineState);
}

void DX12CommandList::begin() noexcept
{ }

void DX12CommandList::finish() noexcept
{
    _cmdList->Close();
}

void DX12CommandList::draw(const uSys vertexCount, const uSys startVertex) noexcept
{
    _cmdList->DrawInstanced(static_cast<UINT>(vertexCount), 1, static_cast<UINT>(startVertex), 0);
}

void DX12CommandList::drawIndexed(const uSys indexCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    _cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), 1, static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), 0);
}

void DX12CommandList::drawInstanced(const uSys vertexCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    _cmdList->DrawInstanced(static_cast<UINT>(vertexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startVertex), static_cast<UINT>(startInstance));
}

void DX12CommandList::drawIndexedInstanced(const uSys indexCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    _cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), static_cast<UINT>(startInstance));
}

void DX12CommandList::setDrawType(const EGraphics::DrawType drawType) noexcept
{
    _cmdList->IASetPrimitiveTopology(DX12Utils::getDXDrawType(drawType));
}

void DX12CommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    _cmdList->SetPipelineState(pipelineState.get<ID3D12PipelineState>());
}

void DX12CommandList::setStencilRef(const uSys stencilRef) noexcept
{
    _cmdList->OMSetStencilRef(static_cast<UINT>(stencilRef));
}

void DX12CommandList::setVertexArray(const NullableRef<IVertexArray>& va) noexcept
{
#if TAU_NULL_CHECK
    if(!va)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTT_CHECK(va, DX12VertexArray))
    { return; }
#endif

    const DX12VertexArray* const dxVA = static_cast<const DX12VertexArray* const>(va.get());

    _cmdList->IASetVertexBuffers(0, static_cast<UINT>(dxVA->d3dBuffers().count()), dxVA->d3dBuffers().arr());
    _cmdAllocator->freeList().allocateT<NullableRef<IVertexArray>>(va);
}

void DX12CommandList::setIndexBuffer(const IndexBufferView& indexBufferView) noexcept
{
#if TAU_NULL_CHECK
    if(!indexBufferView.buffer)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(indexBufferView.buffer, DX12Resource, IResource))
    { return; }
#endif

#if TAU_GENERAL_SAFETY_CHECK
    if(indexBufferView.indexSize != EBuffer::IndexSize::Uint16 ||
       indexBufferView.indexSize != EBuffer::IndexSize::Uint32)
    { return; }

    if(indexBufferView.buffer->resourceType() != EResource::Type::Buffer)
    { return; }
#endif

    const DX12ResourceBuffer* const buffer = static_cast<const DX12ResourceBuffer* const>(indexBufferView.buffer.get());

    D3D12_INDEX_BUFFER_VIEW d3dView;
    d3dView.BufferLocation = buffer->d3dResource()->GetGPUVirtualAddress();
    d3dView.SizeInBytes = static_cast<UINT>(indexBufferView.buffer->size());
    d3dView.Format = DX12Utils::getDXIndexSize(indexBufferView.indexSize);
    _cmdList->IASetIndexBuffer(&d3dView);
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(indexBufferView.buffer);
}

void DX12CommandList::setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept
{
}

void DX12CommandList::setGraphicsDescriptorTable(uSys index, EGraphics::DescriptorType type, uSys descriptorCount, GPUDescriptorHandle handle) noexcept
{
}

void DX12CommandList::executeBundle(const NullableRef<ICommandList>& bundle) noexcept
{
#if TAU_NULL_CHECK
    if(!bundle)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTT_CHECK(bundle, DX12CommandList))
    { return; }
#endif

    const DX12CommandList* dxBundle = static_cast<const DX12CommandList*>(bundle.get());
    _cmdList->ExecuteBundle(dxBundle->_cmdList);
    _cmdAllocator->freeList().allocateT<NullableRef<ICommandList>>(bundle);
}

void DX12CommandList::copyResource(const NullableRef<IResource>& dst, const NullableRef<IResource>& src) noexcept
{
#if TAU_NULL_CHECK
    if(!dst || !src)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(dst, DX12Resource, IResource) ||
       !RTTD_CHECK(src, DX12Resource, IResource))
    { return; }
#endif

    const DX12Resource* dxDst = static_cast<const DX12Resource*>(dst.get());
    const DX12Resource* dxSrc = static_cast<const DX12Resource*>(src.get());

    _cmdList->CopyResource(dxDst->d3dResource(), dxSrc->d3dResource());
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(dst);
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(src);
}

void DX12CommandList::copyBuffer(const NullableRef<IResource>& dstBuffer, u64 dstOffset, const NullableRef<IResource>& srcBuffer, u64 srcOffset, u64 byteCount) noexcept
{
#if TAU_NULL_CHECK
    if(!dstBuffer || !srcBuffer)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(dstBuffer, DX12Resource, IResource) ||
       !RTTD_CHECK(srcBuffer, DX12Resource, IResource))
    { return; }
#endif

    const DX12Resource* dxDst = static_cast<const DX12Resource*>(dstBuffer.get());
    const DX12Resource* dxSrc = static_cast<const DX12Resource*>(srcBuffer.get());

    _cmdList->CopyBufferRegion(dxDst->d3dResource(), dstOffset, dxSrc->d3dResource(), srcOffset, byteCount);
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(dstBuffer);
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(srcBuffer);
}

void DX12CommandList::copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const NullableRef<IResource>& srcTexture, u32 srcSubResource) noexcept
{
}

void DX12CommandList::copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const ETexture::Coord& coord, const NullableRef<IResource>& srcTexture, u32 srcSubResource, const ETexture::EBox* srcBox) noexcept
{
}
#endif
