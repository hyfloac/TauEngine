#include "dx/dx10/DX10CommandList.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10CommandAllocator.hpp"
#include "dx/dx10/DX10FrameBuffer.hpp"
#include "dx/dx10/DX10RenderTarget.hpp"
#include "dx/dx10/DX10Enums.hpp"
#include "dx/dx10/DX10PipelineState.hpp"
#include "dx/dx10/DX10DescriptorLayout.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "dx/dx10/DX10BlendingState.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "graphics/BufferView.hpp"
#include "graphics/ResourceRawInterface.hpp"
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
    , _currentLayout(nullptr)
{ }

void DX10CommandList::reset(const NullableRef<ICommandAllocator>& allocator, const NullableRef<IPipelineState>& initialState) noexcept
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

    _currentLayout = nullptr;

    if(initialState)
    { setPipelineState(initialState); }
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

void DX10CommandList::setPipelineState(const NullableRef<IPipelineState>& pipelineState) noexcept
{
    const auto& args = pipelineState->args();

#if TAU_RTTI_CHECK
    if(!rtt_check<SimpleDescriptorLayout>(args.descriptorLayout) ||
       !rtt_check<DX10InputLayout>(args.inputLayout) ||
       !rtt_check<DX10BlendingState>(args.blendingState) ||
       !rtt_check<DX10DepthStencilState>(args.depthStencilState) ||
       !rtt_check<DX10RasterizerState>(args.rasterizerState))
    { return; }
#endif

    _currentLayout = static_cast<const SimpleDescriptorLayout*>(args.descriptorLayout.get());

    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IPipelineState>>(pipelineState);

    const DX10CL::CommandSetPipelineState setPipelineState(pipelineState.get());
    (void) _commandAllocator->allocateT<DX10CL::Command>(setPipelineState);
    ++_commandCount;
}

void DX10CommandList::setFrameBuffer(const NullableRef<IFrameBuffer>& frameBuffer) noexcept
{
#if TAU_RTTI_CHECK
    if(!rtt_check<DX10FrameBuffer>(frameBuffer))
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IFrameBuffer>>(frameBuffer);

    const DX10FrameBuffer* const dxFrameBuffer = static_cast<const DX10FrameBuffer*>(frameBuffer.get());

    const DX10CL::CommandSetRenderTargets setRenderTargets(static_cast<UINT>(frameBuffer->colorAttachments().count()), dxFrameBuffer->d3dColorAttachments(), RefCast<DX10DepthStencilView>(frameBuffer->depthStencilAttachment())->d3dDepthStencilView());
    (void) _commandAllocator->allocateT<DX10CL::Command>(setRenderTargets);
    ++_commandCount;
}

void DX10CommandList::clearRenderTargetView(const NullableRef<IFrameBuffer>& frameBuffer, const uSys renderTargetIndex, const float color[4], uSys, const ETexture::ERect*) noexcept
{
#if TAU_RTTI_CHECK
    if(!rtt_check<DX10FrameBuffer>(frameBuffer))
    { return; }
#endif

#if TAU_GENERAL_SAFETY_CHECK
    if(renderTargetIndex >= frameBuffer->colorAttachments().count())
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IRenderTargetView>>(frameBuffer->colorAttachments()[renderTargetIndex]);

    const DX10RenderTargetView* const dxRenderTarget = static_cast<const DX10RenderTargetView*>(frameBuffer->colorAttachments()[renderTargetIndex].get());

    const DX10CL::CommandClearRenderTarget clearRenderTarget(dxRenderTarget->d3dRenderTargetView(), color);
    (void) _commandAllocator->allocateT<DX10CL::Command>(clearRenderTarget);
    ++_commandCount;
}

void DX10CommandList::clearDepthStencilView(const NullableRef<IFrameBuffer>& frameBuffer, const bool clearDepth, const bool clearStencil, const float depth, const u8 stencil, uSys, const ETexture::ERect*) noexcept
{
#if TAU_RTTI_CHECK
    if(!rtt_check<DX10FrameBuffer>(frameBuffer))
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IDepthStencilView>>(frameBuffer->depthStencilAttachment());

    const DX10DepthStencilView* const dxDepthStencil = static_cast<const DX10DepthStencilView*>(frameBuffer->depthStencilAttachment().get());

    UINT clearFlags = 0;
    if(clearDepth)   { clearFlags  = D3D10_CLEAR_DEPTH;   }
    if(clearStencil) { clearFlags |= D3D10_CLEAR_STENCIL; }

    const DX10CL::CommandClearDepthStencil clearDepthStencil(dxDepthStencil->d3dDepthStencilView(), clearFlags, depth, stencil);
    (void) _commandAllocator->allocateT<DX10CL::Command>(clearDepthStencil);
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
    if(!rtt_check<DX10VertexArray>(va))
    { return; }
#endif

    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IVertexArray>>(va);

    const DX10VertexArray* const dxVA = static_cast<const DX10VertexArray*>(va.get());

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

    const DX10ResourceBuffer* const buffer = static_cast<const DX10ResourceBuffer*>(indexBufferView.buffer.get());

    const DX10CL::CommandSetIndexBuffer setIndexBuffer(buffer->d3dBuffer(), DX10ResourceBuffer::dxIndexSize(indexBufferView.indexSize));
    (void) _commandAllocator->allocateT<DX10CL::Command>(setIndexBuffer);
    ++_commandCount;
}

// void DX10CommandList::setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept
// {
//     const DX10CL::CommandSetGDescriptorLayout setGDescriptorLayout(layout);
//     (void) _commandAllocator->allocateT<DX10CL::Command>(setGDescriptorLayout);
//     ++_commandCount;
// }

void DX10CommandList::setGraphicsDescriptorTable(const uSys index, const EGraphics::DescriptorType type, const uSys descriptorCount, const GPUDescriptorHandle handle) noexcept
{
#if TAU_GENERAL_SAFETY_CHECK
    // Counts don't match
    if(descriptorCount != _currentLayout->entries()[index].count)
    { return; }
#endif
    
#if TAU_GENERAL_SAFETY_CHECK
    if(type == EGraphics::DescriptorType::TextureView)
    {
        // Types don't match
        if(_currentLayout->entries()[index].type != DescriptorLayoutEntry::Type::TextureView)
        { return; }
        // Counts don't match
        if(_currentLayout->entries()[index].count != descriptorCount)
        { return; }
    }
    else if(type == EGraphics::DescriptorType::UniformBufferView)
    {
        // Types don't match
        if(_currentLayout->entries()[index].type != DescriptorLayoutEntry::Type::UniformBufferView)
        { return; }
        // Counts don't match
        if(_currentLayout->entries()[index].count != descriptorCount)
        { return; }
    }
#endif

    const DX10CL::CommandSetGDescriptorTable setGDescriptorTable(index, type, descriptorCount, handle);
    (void) _commandAllocator->allocateT<DX10CL::Command>(setGDescriptorTable);
    ++_commandCount;
}

void DX10CommandList::executeBundle(const NullableRef<ICommandList>& bundle) noexcept
{
#if TAU_RTTI_CHECK
    if(!rtt_check<DX10CommandList>(bundle))
    { return; }
#endif

    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<ICommandList>>(bundle);
    
    const DX10CommandList* const dxBundle = static_cast<const DX10CommandList*>(bundle.get());

    const DX10CL::CommandExecuteBundle executeBundle(dxBundle);
    (void) _commandAllocator->allocateT<DX10CL::Command>(executeBundle);
    ++_commandCount;
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
