#include "dx/dx10/DX10CommandList.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "dx/dx10/DX10Enums.hpp"
#include "graphics/BufferView.hpp"
#include "TauConfig.hpp"

void DX10CommandList::reset() noexcept
{
    _commands = ArrayList<DX10CL::Command>(_maxCommands);
    _refCountList.reset(true);
}

void DX10CommandList::finish() noexcept
{ }

void DX10CommandList::draw(const uSys exCount, const uSys startVertex) noexcept
{
    const DX10CL::CommandDraw draw(exCount, startVertex);
    _commands.emplace(draw);
}

void DX10CommandList::drawIndexed(const uSys exCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    const DX10CL::CommandDrawIndexed drawIndexed(exCount, startIndex, baseVertex);
    _commands.emplace(drawIndexed);
}

void DX10CommandList::drawInstanced(const uSys exCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    const DX10CL::CommandDrawInstanced drawInstanced(exCount, instanceCount, startVertex, startInstance);
    _commands.emplace(drawInstanced);
}

void DX10CommandList::drawIndexedInstanced(const uSys exCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    const DX10CL::CommandDrawIndexedInstanced drawIndexedInstanced(exCount, instanceCount, startIndex, baseVertex, startInstance);
    _commands.emplace(drawIndexedInstanced);
}

void DX10CommandList::setDrawType(const EGraphics::DrawType drawType) noexcept
{
    const DX10CL::CommandSetDrawType setDrawType(DX10Utils::getDXDrawType(drawType));
    _commands.emplace(setDrawType);
}

void DX10CommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    const DX10CL::CommandSetPipelineState setPipelineState(&pipelineState);
    _commands.emplace(setPipelineState);
}

void DX10CommandList::setStencilRef(const uSys stencilRef) noexcept
{
    const DX10CL::CommandSetStencilRef setStencilRef(stencilRef);
    _commands.emplace(setStencilRef);
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
    (void) _refCountList.allocateT<NullableRef<IVertexArray>>(va);

    const NullableRef<DX10VertexArray> dxVA = RefCast<DX10VertexArray>(va);
    const DX10CL::CommandSetVertexArray setVertexArray(0, dxVA->iaBufferCount(), dxVA->iaBuffers());
    _commands.emplace(setVertexArray);
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
    (void) _refCountList.allocateT<NullableRef<IResource>>(indexBufferView.buffer);


    NullableRef<DX10ResourceBuffer> buffer = RefCast<DX10ResourceBuffer>(indexBufferView.buffer);
    const DX10CL::CommandSetIndexBuffer setIndexBuffer(buffer->d3dBuffer(), DX10ResourceBuffer::dxIndexSize(indexBufferView.indexSize));
    _commands.emplace(setIndexBuffer);
}

void DX10CommandList::setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept
{
    const DX10CL::CommandSetGDescriptorLayout setGDescriptorLayout(layout);
    _commands.emplace(setGDescriptorLayout);
}

void DX10CommandList::setGraphicsDescriptorTable(const uSys index, DescriptorTable table) noexcept
{
    const DX10CL::CommandSetGDescriptorTable setGDescriptorTable(index, table);
    _commands.emplace(setGDescriptorTable);
}

void DX10CommandList::setGraphicsDescriptorTable(const uSys index, DescriptorSamplerTable table) noexcept
{
    const DX10CL::CommandSetGDescriptorSamplerTable setGDescriptorSamplerTable(index, table);
    _commands.emplace(setGDescriptorSamplerTable);
}
#endif
