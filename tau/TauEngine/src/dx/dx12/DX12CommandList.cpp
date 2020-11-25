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
    trackVertexBuffersPre();

    const DX12CL::CommandDrawInstanced draw(static_cast<UINT>(vertexCount), 1, static_cast<UINT>(startVertex), 0);
    _commands.emplace(draw);

    trackVertexBuffersPost();

    // _cmdList->DrawInstanced(static_cast<UINT>(vertexCount), 1, static_cast<UINT>(startVertex), 0);
}

void DX12CommandList::drawIndexed(const uSys indexCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    trackVertexBuffersPre();
    trackIndexBufferPre();

    const DX12CL::CommandDrawIndexedInstanced drawIndexed(static_cast<UINT>(indexCount), 1, static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), 0);
    _commands.emplace(drawIndexed);
    
    trackIndexBufferPost();
    trackVertexBuffersPost();

    // _cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), 1, static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), 0);
}

void DX12CommandList::drawInstanced(const uSys vertexCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    trackVertexBuffersPre();

    const DX12CL::CommandDrawInstanced drawInstanced(static_cast<UINT>(vertexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startVertex), static_cast<UINT>(startInstance));
    _commands.emplace(drawInstanced);

    trackVertexBuffersPost();

    // _cmdList->DrawInstanced(static_cast<UINT>(vertexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startVertex), static_cast<UINT>(startInstance));
}

void DX12CommandList::drawIndexedInstanced(const uSys indexCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    trackVertexBuffersPre();
    trackIndexBufferPre();

    const DX12CL::CommandDrawIndexedInstanced drawIndexedInstanced(static_cast<UINT>(indexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), static_cast<UINT>(startInstance));
    _commands.emplace(drawIndexedInstanced);
    
    trackIndexBufferPost();
    trackVertexBuffersPost();

    // _cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), static_cast<UINT>(instanceCount), static_cast<UINT>(startIndex), static_cast<INT>(baseVertex), static_cast<UINT>(startInstance));
}

void DX12CommandList::setDrawType(const EGraphics::DrawType drawType) noexcept
{
    const DX12CL::CommandSetDrawType setDrawType(DX12Utils::getDXDrawType(drawType));
    _commands.emplace(setDrawType);

    // _cmdList->IASetPrimitiveTopology(DX12Utils::getDXDrawType(drawType));
}

void DX12CommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    const DX12CL::CommandSetPipelineState setPipelineState(pipelineState.get<ID3D12PipelineState>());
    _commands.emplace(setPipelineState);

    // _cmdList->SetPipelineState(pipelineState.get<ID3D12PipelineState>());
}

void DX12CommandList::setStencilRef(const uSys stencilRef) noexcept
{
    const DX12CL::CommandSetStencilRef setStencilRef(stencilRef);
    _commands.emplace(setStencilRef);

    // _cmdList->OMSetStencilRef(static_cast<UINT>(stencilRef));
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

    _currentVA = dxVA;

    const DX12CL::CommandSetVertexArray setVertexArray(0, static_cast<UINT>(dxVA->d3dBuffers().count()), dxVA->d3dBuffers().arr());
    _commands.emplace(setVertexArray);

    // _cmdList->IASetVertexBuffers(0, static_cast<UINT>(dxVA->d3dBuffers().count()), dxVA->d3dBuffers().arr());

    (void) _cmdAllocator->freeList().allocateT<NullableRef<IVertexArray>>(va);
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
    if(indexBufferView.indexSize != EBuffer::IndexSize::Uint16 &&
       indexBufferView.indexSize != EBuffer::IndexSize::Uint32)
    { return; }

    if(indexBufferView.buffer->resourceType() != EResource::Type::Buffer)
    { return; }
#endif

    const DX12ResourceBuffer* const buffer = static_cast<const DX12ResourceBuffer* const>(indexBufferView.buffer.get());

    _currentIndexBuffer = buffer;

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

    trackResourcePreWrite(dxDst->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_DEST);
    trackResourcePreRead(dxSrc->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_SOURCE);

    DX12CL::CommandCopyResource copyResource(dxDst->d3dResource(), dxSrc->d3dResource());
    _commands.emplace(copyResource);

    trackResourcePostWrite(dxDst->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_DEST);
    trackResourcePostRead(dxSrc->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_SOURCE);

    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(dst);
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(src);
}

void DX12CommandList::copyBuffer(const NullableRef<IResource>& dstBuffer, const u64 dstOffset, const NullableRef<IResource>& srcBuffer, const u64 srcOffset, const u64 byteCount) noexcept
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
    
    trackResourcePreWrite(dxDst->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_DEST);
    trackResourcePreRead(dxSrc->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_SOURCE);

    DX12CL::CommandCopyBuffer copyBuffer(dxDst->d3dResource(), dstOffset, dxSrc->d3dResource(), srcOffset, byteCount);
    _commands.emplace(copyBuffer);
    
    trackResourcePostWrite(dxDst->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_DEST);
    trackResourcePostRead(dxSrc->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_SOURCE);

    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(dstBuffer);
    _cmdAllocator->freeList().allocateT<NullableRef<IResource>>(srcBuffer);
}

void DX12CommandList::copyTexture(const NullableRef<IResource>& dstTexture, const u32 dstSubResource, const NullableRef<IResource>& srcTexture, const u32 srcSubResource) noexcept
{
}

void DX12CommandList::copyTexture(const NullableRef<IResource>& dstTexture, const u32 dstSubResource, const ETexture::Coord& coord, const NullableRef<IResource>& srcTexture, const u32 srcSubResource, const ETexture::EBox* srcBox) noexcept
{
}

void DX12CommandList::trackVertexBuffersPre() noexcept
{
    if(!_currentVA)
    { return; }

    for(uSys i = 0; i < _currentVA->buffers().count(); ++i)
    {
        ID3D12Resource* const resource = RefCast<DX12Resource>(_currentVA->buffers()[i])->d3dResource();
        trackResourcePreRead(resource, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    }
}

void DX12CommandList::trackVertexBuffersPost() noexcept
{
    if(!_currentVA)
    { return; }

    for(uSys i = 0; i < _currentVA->buffers().count(); ++i)
    {
        ID3D12Resource* const resource = RefCast<DX12Resource>(_currentVA->buffers()[i])->d3dResource();
        trackResourcePostRead(resource, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    }
}

void DX12CommandList::trackIndexBufferPre() noexcept
{
    if(_currentIndexBuffer)
    { return; }

    trackResourcePreRead(_currentIndexBuffer->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_INDEX_BUFFER);
}

void DX12CommandList::trackIndexBufferPost() noexcept
{
    if(_currentIndexBuffer)
    { return; }

    trackResourcePostRead(_currentIndexBuffer->d3dResource(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_INDEX_BUFFER);
}

void DX12CommandList::trackResourcePreWrite(ID3D12Resource* const resource, const uSys subresource, const D3D12_RESOURCE_STATES state) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return; }
#endif

    DX12ResourceStateManager::UsageIndices* const usageIndices = _stateManager.getUsageIndices(resource);
    
    //   This falls under general safety because returning null means that
    // the resource wasn't properly tracked.
#if TAU_GENERAL_SAFETY_CHECK
    if(!usageIndices)
    { return; }
#endif

    if(usageIndices->lastUsage(subresource) != static_cast<uSys>(-1))
    {
        //   Find the last time this resource was used and set
        // its next usage change handle to this.
        _commands[usageIndices->lastUsage(0)].resourceUsage.nextUsageChange = _commands.count();
        // Reset the last usage this this.
        usageIndices->lastUsage(subresource) = _commands.count();
    }

    // Reset first and last reads when doing a write.
    usageIndices->firstRead(subresource) = static_cast<uSys>(-1);
    usageIndices->lastRead(subresource) = static_cast<uSys>(-1);

    const DX12CL::CommandResourceUsage dstResourceUsage(resource, state, subresource, DX12CL::ResourceUsagePhase::PreUsageFirst);
    _commands.emplace(dstResourceUsage);
}

void DX12CommandList::trackResourcePostWrite(ID3D12Resource* const resource, const uSys subresource, const D3D12_RESOURCE_STATES state) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return; }
#endif

    DX12ResourceStateManager::UsageIndices* const usageIndices = _stateManager.getUsageIndices(resource);
    
    //   This falls under general safety because returning null means that
    // the resource wasn't properly tracked.
#if TAU_GENERAL_SAFETY_CHECK
    if(!usageIndices)
    { return; }
#endif

    //   Find the last time this resource was used and set
    // its next usage change handle to this.
    _commands[usageIndices->lastUsage(subresource)].resourceUsage.nextUsageChange = _commands.count();
    // Reset the last usage this this.
    usageIndices->lastUsage(subresource) = _commands.count();

    const DX12CL::CommandResourceUsage dstResourceUsage(resource, state, subresource, DX12CL::ResourceUsagePhase::PostUsageFinal);
    _commands.emplace(dstResourceUsage);
}

void DX12CommandList::trackResourcePreRead(ID3D12Resource* const resource, const uSys subresource, const D3D12_RESOURCE_STATES state) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return; }
#endif

    DX12ResourceStateManager::UsageIndices* usageIndices = _stateManager.getUsageIndices(resource);

    //   This falls under general safety because returning null means that
    // the resource wasn't properly tracked.
#if TAU_GENERAL_SAFETY_CHECK
    if(!usageIndices)
    { return; }
#endif

    bool firstUsage = false;


    if(usageIndices->lastRead(subresource) != static_cast<uSys>(-1))
    {
        _commands[usageIndices->lastRead(subresource)].resourceUsage.usage = DX12CL::ResourceUsagePhase::PostUsage;
    }
    
    if(usageIndices->firstRead(subresource) != static_cast<uSys>(-1))
    {
        _commands[usageIndices->lastRead(subresource)].resourceUsage.state |= state;
    }
    else
    {
        firstUsage = true;
        usageIndices->firstRead(subresource) = _commands.size();
        //   Find the last time this resource was used and set
        // its next usage change handle to this.
        _commands[usageIndices->lastUsage(0)].resourceUsage.nextUsageChange = _commands.count();

    }

    usageIndices->lastUsage(subresource) = _commands.count();

    const DX12CL::CommandResourceUsage resourceUsage(resource, state, subresource, firstUsage ? DX12CL::ResourceUsagePhase::PreUsageFirst : DX12CL::ResourceUsagePhase::PreUsage);
    _commands.emplace(resourceUsage);
}

void DX12CommandList::trackResourcePostRead(ID3D12Resource* const resource, const uSys subresource, const D3D12_RESOURCE_STATES state) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return; }
#endif

    DX12ResourceStateManager::UsageIndices* usageIndices = _stateManager.getUsageIndices(resource);

    //   This falls under general safety because returning null means that
    // the resource wasn't properly tracked.
#if TAU_GENERAL_SAFETY_CHECK
    if(!usageIndices)
    { return; }
#endif

    usageIndices->lastUsage(subresource) = _commands.count();
    usageIndices->lastRead(subresource) = _commands.size();

    const DX12CL::CommandResourceUsage resourceUsage(resource, state, subresource, DX12CL::ResourceUsagePhase::PostUsageFinal);
    _commands.emplace(resourceUsage);
}

void DX12CommandList::populateCommandList()
{
    for(auto cmd : _commands)
    {
#define DISPATCH(__CMD, __FUNC, __ARG) case DX12CL::CommandType::__CMD: __FUNC(cmd.__ARG); break

        switch(cmd.type)
        {
            DISPATCH(DrawInstanced, _drawInstanced, drawInstanced);
            DISPATCH(DrawIndexedInstanced, _drawIndexedInstanced, drawIndexedInstanced);
            DISPATCH(SetDrawType, _setDrawType, setDrawType);
            DISPATCH(SetPipelineState, _setPipelineState, setPipelineState);
            DISPATCH(SetStencilRef, _setStencilRef, setStencilRef);
            DISPATCH(SetVertexArray, _setVertexArray, setVertexArray);
            DISPATCH(SetIndexBuffer, _setIndexBuffer, setIndexBuffer);
            DISPATCH(CopyResource, _copyResource, copyResource);
            DISPATCH(CopyBuffer, _copyBuffer, copyBuffer);
            DISPATCH(CopyTexture, _copyTexture, copyTexture);
            DISPATCH(CopyTextureBox, _copyTextureBox, copyTextureBox);
            DISPATCH(ResourceUsage, _resourceUsage, resourceUsage);
            // DISPATCH(SetGDescriptorLayout, _setGDescriptorLayout, setGDescriptorLayout);
            // DISPATCH(SetGDescriptorTable, _setGDescriptorTable, setGDescriptorTable);
            default: break;
        }
#undef DISPATCH
    }
}

void DX12CommandList::_drawInstanced(const DX12CL::CommandDrawInstanced& cmd) noexcept
{
    _stateManager.flushResourceBarriers(_cmdList);
    _cmdList->DrawInstanced(cmd.vertexCount, cmd.instanceCount, cmd.startVertex, cmd.startInstance);
}

void DX12CommandList::_drawIndexedInstanced(const DX12CL::CommandDrawIndexedInstanced& cmd) noexcept
{
    _stateManager.flushResourceBarriers(_cmdList);
    _cmdList->DrawIndexedInstanced(cmd.indexCount, cmd.instanceCount, cmd.startIndex, cmd.baseVertex, cmd.startInstance);
}

void DX12CommandList::_setDrawType(const DX12CL::CommandSetDrawType& cmd) noexcept
{
    _cmdList->IASetPrimitiveTopology(cmd.drawType);
}

void DX12CommandList::_setPipelineState(const DX12CL::CommandSetPipelineState& cmd) noexcept
{
    _cmdList->SetPipelineState(cmd.pipelineState);
}

void DX12CommandList::_setStencilRef(const DX12CL::CommandSetStencilRef& cmd) noexcept
{
    _cmdList->OMSetStencilRef(static_cast<UINT>(cmd.stencilRef));
}

void DX12CommandList::_setVertexArray(const DX12CL::CommandSetVertexArray& cmd) noexcept
{
    _cmdList->IASetVertexBuffers(static_cast<UINT>(cmd.startSlot), static_cast<UINT>(cmd.bufferCount), cmd.buffers);
}

void DX12CommandList::_setIndexBuffer(const DX12CL::CommandSetIndexBuffer& cmd) noexcept
{
    _cmdList->IASetIndexBuffer(&cmd.view);
}

void DX12CommandList::_copyResource(const DX12CL::CommandCopyResource& cmd) noexcept
{
    _stateManager.flushResourceBarriers(_cmdList);
    _cmdList->CopyResource(cmd.dst, cmd.src);

}

void DX12CommandList::_copyBuffer(const DX12CL::CommandCopyBuffer& cmd) noexcept
{
    _stateManager.flushResourceBarriers(_cmdList);
    _cmdList->CopyBufferRegion(cmd.dst, cmd.dstOffset, cmd.src, cmd.srcOffset, cmd.byteCount);
}

void DX12CommandList::_copyTexture(const DX12CL::CommandCopyTexture& cmd) noexcept
{
}

void DX12CommandList::_copyTextureBox(const DX12CL::CommandCopyTextureBox& cmd) noexcept
{
}

void DX12CommandList::_resourceUsage(const DX12CL::CommandResourceUsage& cmd) noexcept
{
    if(cmd.usage == DX12CL::ResourceUsagePhase::PreUsageFirst)
    {
        _stateManager.transitionResource(cmd.resource, cmd.state, static_cast<UINT>(cmd.subresource));
    }
    else if(cmd.usage == DX12CL::ResourceUsagePhase::PostUsageFinal)
    {
        if(cmd.nextUsageChange != static_cast<uSys>(-1))
        {
            const DX12CL::CommandResourceUsage& nextUsageChange = _commands[cmd.nextUsageChange].resourceUsage;
            _stateManager.transitionResourceBegin(nextUsageChange.resource, nextUsageChange.state, static_cast<UINT>(nextUsageChange.subresource));
        }
    }
}
#endif
