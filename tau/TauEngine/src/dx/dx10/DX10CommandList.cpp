#include "dx/dx10/DX10CommandList.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10ResourceBuffer.hpp"
#include "graphics/BufferView.hpp"
#include "TauConfig.hpp"

void DX10CommandList::reset() noexcept
{
    _commands = ArrayList<DX10CL::Command>(_maxCommands);
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

void DX10CommandList::setDrawType(const DrawType drawType) noexcept
{
    const DX10CL::CommandSetDrawType setDrawType(DX10VertexArray::getDXDrawType(drawType));
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

void DX10CommandList::setVertexArray(const IVertexArray& va) noexcept
{
#if TAU_RTTI_CHECK
    const DX10VertexArray* const dxVA = RTT_CAST(va, DX10VertexArray);

    if(!dxVA)
    { return; }
#else
    const DX10VertexArray* const dxVA = static_cast<DX10VertexArray* const>(&va);
#endif

    const DX10CL::CommandSetVertexArray setVertexArray(0, dxVA->iaBufferCount(), dxVA->iaBuffers());
    _commands.emplace(setVertexArray);
}

void DX10CommandList::setIndexBuffer(const IndexBufferView& indexBufferView) noexcept
{
    if(indexBufferView.buffer->resourceType() == EResource::Type::Buffer)
    {
        DX10Resource* const resource = RTTD_CAST(indexBufferView.buffer, DX10Resource, IResource);

        if(resource)
        {
            DX10ResourceBuffer* const buffer = static_cast<DX10ResourceBuffer* const>(resource);

            const DX10CL::CommandSetIndexBuffer setIndexBuffer(buffer->d3dBuffer(), DX10ResourceBuffer::dxIndexSize(indexBufferView.indexSize));
            _commands.emplace(setIndexBuffer);
        }
    }
}

void DX10CommandList::setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept
{
}

void DX10CommandList::setGraphicsDescriptorTable(const uSys index, DescriptorTable table) noexcept
{
}

void DX10CommandList::setGraphicsDescriptorTable(const uSys index, DescriptorSamplerTable table) noexcept
{
}
#endif
