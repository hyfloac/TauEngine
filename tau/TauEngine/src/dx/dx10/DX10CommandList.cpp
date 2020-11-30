#include "dx/dx10/DX10CommandList.hpp"

#include "graphics/ResourceRawInterface.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10CommandAllocator.hpp"
#include "dx/dx10/DX10Enums.hpp"
#include "graphics/BufferView.hpp"
#include "TauConfig.hpp"

static inline const void* computeHead(const NullableRef<DX10CommandAllocator>& allocator) noexcept
{
    const void* const head = allocator->head();
    return reinterpret_cast<const u8*>(head) + allocator->allocIndex();
}

DX10CommandList::DX10CommandList(const NullableRef<DX10CommandAllocator>& allocator) noexcept
    : _commandAllocator(allocator)
    , _head(computeHead(allocator))
    , _commandCount(0)
{ }

void DX10CommandList::reset(const NullableRef<ICommandAllocator>& allocator, const PipelineState* const initialState) noexcept
{
#if TAU_NULL_CHECK
    if(!allocator)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTT_CHECK(allocator.get(), DX10CommandAllocator))
    { return; }
#endif
    
    _commandAllocator = RefCast<DX10CommandAllocator>(allocator);
    _head = computeHead(_commandAllocator);
    _commandCount = 0;

    if(initialState)
    { setPipelineState(*initialState); }
}

void DX10CommandList::begin() noexcept
{ }

void DX10CommandList::finish() noexcept
{ }

void DX10CommandList::draw(const uSys vertexCount, const uSys startVertex) noexcept
{
    const DX10CL::CommandDraw draw(static_cast<UINT>(vertexCount), static_cast<UINT>(startVertex));
    (void) _commandAllocator->allocateT<DX10CL::Command>(draw);
    ++_commandCount;
}

void DX10CommandList::drawIndexed(const uSys indexCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    const DX10CL::CommandDrawIndexed drawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startIndex), static_cast<UINT>(baseVertex));
    (void) _commandAllocator->allocateT<DX10CL::Command>(drawIndexed);
    ++_commandCount;
}

void DX10CommandList::drawInstanced(const uSys vertexCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    const DX10CL::CommandDrawInstanced drawInstanced(static_cast<UINT>(vertexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startVertex), static_cast<UINT>(startInstance));
    (void) _commandAllocator->allocateT<DX10CL::Command>(drawInstanced);
    ++_commandCount;
}

void DX10CommandList::drawIndexedInstanced(const uSys indexCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    const DX10CL::CommandDrawIndexedInstanced drawIndexedInstanced(static_cast<UINT>(indexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), static_cast<UINT>(startInstance));
    (void) _commandAllocator->allocateT<DX10CL::Command>(drawIndexedInstanced);
    ++_commandCount;
}

void DX10CommandList::setDrawType(const EGraphics::DrawType drawType) noexcept
{
    const DX10CL::CommandSetDrawType setDrawType(DX10Utils::getDXDrawType(drawType));
    (void) _commandAllocator->allocateT<DX10CL::Command>(setDrawType);
    ++_commandCount;
}

void DX10CommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    const DX10CL::CommandSetPipelineState setPipelineState(&pipelineState);
    (void) _commandAllocator->allocateT<DX10CL::Command>(setPipelineState);
    ++_commandCount;
}

void DX10CommandList::setBlendFactor(const float blendFactor[4]) noexcept
{
    if(!blendFactor)
    {
        const DX10CL::CommandSetBlendFactor setBlendFactor;
        (void) _commandAllocator->allocateT<DX10CL::Command>(setBlendFactor);
    }
    else
    {
        const DX10CL::CommandSetBlendFactor setBlendFactor(blendFactor);
        (void) _commandAllocator->allocateT<DX10CL::Command>(setBlendFactor);   
    }
    ++_commandCount;
}

void DX10CommandList::setStencilRef(const uSys stencilRef) noexcept
{
    const DX10CL::CommandSetStencilRef setStencilRef(stencilRef);
    (void) _commandAllocator->allocateT<DX10CL::Command>(setStencilRef);
    ++_commandCount;
}

void DX10CommandList::setVertexArray(const NullableRef<IVertexArray>& va) noexcept
{
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(va.get(), DX10VertexArray))
    { return; }
#endif

    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IVertexArray>>(va);

    const NullableRef<DX10VertexArray> dxVA = RefCast<DX10VertexArray>(va);
    const DX10CL::CommandSetVertexArray setVertexArray(0, dxVA->iaBufferCount(), dxVA->iaBuffers());
    (void) _commandAllocator->allocateT<DX10CL::Command>(setVertexArray);
    ++_commandCount;
}

void DX10CommandList::setIndexBuffer(const IndexBufferView& indexBufferView) noexcept
{
    if(!indexBufferView.buffer || 
        indexBufferView.buffer->resourceType() != EResource::Type::Buffer || 
       !RTTD_CHECK(indexBufferView.buffer.get(), DX10Resource, IResource))
    { return; }

    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(indexBufferView.buffer);

    NullableRef<DX10ResourceBuffer> buffer = RefCast<DX10ResourceBuffer>(indexBufferView.buffer);
    const DX10CL::CommandSetIndexBuffer setIndexBuffer(buffer->d3dBuffer(), DX10ResourceBuffer::dxIndexSize(indexBufferView.indexSize));
    (void) _commandAllocator->allocateT<DX10CL::Command>(setIndexBuffer);
    ++_commandCount;
}

void DX10CommandList::setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept
{
    const DX10CL::CommandSetGDescriptorLayout setGDescriptorLayout(layout);
    (void) _commandAllocator->allocateT<DX10CL::Command>(setGDescriptorLayout);
    ++_commandCount;
}

void DX10CommandList::setGraphicsDescriptorTable(const uSys index, const EGraphics::DescriptorType type, const uSys descriptorCount, const GPUDescriptorHandle handle) noexcept
{
    const DX10CL::CommandSetGDescriptorTable setGDescriptorTable(index, type, descriptorCount, handle);
    (void) _commandAllocator->allocateT<DX10CL::Command>(setGDescriptorTable);
    ++_commandCount;
}

void DX10CommandList::executeBundle(const NullableRef<ICommandList>& bundle) noexcept
{
}

void DX10CommandList::copyResource(const NullableRef<IResource>& dst, const NullableRef<IResource>& src) noexcept
{
#if TAU_NULL_CHECK
    if(!dst || !src)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(dst, DX10Resource, IResource) ||
       !RTTD_CHECK(src, DX10Resource, IResource))
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(dst);
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(src);

    const DX10CL::CommandCopyResource copyResource(dst->_getRawHandle().dx10Resource(), src->_getRawHandle().dx10Resource());
    (void) _commandAllocator->allocateT<DX10CL::Command>(copyResource);
    ++_commandCount;
}

void DX10CommandList::copyBuffer(const NullableRef<IResource>& dstBuffer, const u64 dstOffset, const NullableRef<IResource>& srcBuffer, const u64 srcOffset, const u64 byteCount) noexcept
{
#if TAU_NULL_CHECK
    if(!dstBuffer || !srcBuffer)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(dstBuffer, DX10Resource, IResource) ||
       !RTTD_CHECK(srcBuffer, DX10Resource, IResource))
    { return; }
#endif

#if TAU_GENERAL_SAFETY_CHECK
    if(dstBuffer->resourceType() != EResource::Type::Buffer ||
       srcBuffer->resourceType() != EResource::Type::Buffer)
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(dstBuffer);
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(srcBuffer);

    const ETexture::EBox srcBox = ETexture::RBox{ static_cast<u32>(srcOffset), static_cast<u32>(byteCount), 0, 0, 0, 0 }.toExact();

    const DX10CL::CommandCopySubresourceRegion0 copySubresourceRegion0(dstBuffer->_getRawHandle().dx10Resource(), 0, srcBuffer->_getRawHandle().dx10Resource(), 0);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion0);

    const DX10CL::CommandCopySubresourceRegion1 copySubresourceRegion1({ static_cast<u32>(dstOffset), 0, 0 }, &srcBox);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion1);

    const DX10CL::CommandCopySubresourceRegion2 copySubresourceRegion2(&srcBox);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion2);

    _commandCount += 3;
}

void DX10CommandList::copyTexture(const NullableRef<IResource>& dstTexture, const u32 dstSubResource, const NullableRef<IResource>& srcTexture, const u32 srcSubResource) noexcept
{
    static constexpr ETexture::Coord BASE_COORD{ 0, 0, 0 };

#if TAU_NULL_CHECK
    if(!dstTexture || !srcTexture)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(dstTexture, DX10Resource, IResource) ||
       !RTTD_CHECK(srcTexture, DX10Resource, IResource))
    { return; }
#endif

#if TAU_GENERAL_SAFETY_CHECK
    if(dstTexture->resourceType() == EResource::Type::Buffer ||
       srcTexture->resourceType() == EResource::Type::Buffer)
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(dstTexture);
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(srcTexture);

    const DX10CL::CommandCopySubresourceRegion0 copySubresourceRegion0(dstTexture->_getRawHandle().dx10Resource(), dstSubResource, srcTexture->_getRawHandle().dx10Resource(), srcSubResource);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion0);

    const DX10CL::CommandCopySubresourceRegion1 copySubresourceRegion1(BASE_COORD, nullptr);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion1);

    const DX10CL::CommandCopySubresourceRegion2 copySubresourceRegion2(nullptr);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion2);
}

void DX10CommandList::copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const ETexture::Coord& coord, const NullableRef<IResource>& srcTexture, u32 srcSubResource, const ETexture::EBox* srcBox) noexcept
{
#if TAU_NULL_CHECK
    if(!dstTexture || !srcTexture)
    { return; }
#endif

#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(dstTexture, DX10Resource, IResource) ||
       !RTTD_CHECK(srcTexture, DX10Resource, IResource))
    { return; }
#endif

#if TAU_GENERAL_SAFETY_CHECK
    if(dstTexture->resourceType() == EResource::Type::Buffer ||
       srcTexture->resourceType() == EResource::Type::Buffer)
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(dstTexture);
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(srcTexture);

    const DX10CL::CommandCopySubresourceRegion0 copySubresourceRegion0(dstTexture->_getRawHandle().dx10Resource(), dstSubResource, srcTexture->_getRawHandle().dx10Resource(), srcSubResource);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion0);

    const DX10CL::CommandCopySubresourceRegion1 copySubresourceRegion1(coord, srcBox);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion1);

    const DX10CL::CommandCopySubresourceRegion2 copySubresourceRegion2(srcBox);
    (void) _commandAllocator->allocateT<DX10CL::Command>(copySubresourceRegion2);
}
#endif
