#include "dx/dx10/DX10CommandQueue.hpp"

#ifdef _WIN32
#include "graphics/PipelineState.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10InputLayout.hpp"

void DX10CommandQueue::executeCommandLists(uSys count, const ICommandList** lists) noexcept
{
}

void DX10CommandQueue::executeCommandList(const ICommandList* list) noexcept
{
}

void DX10CommandQueue::_draw(const DX10CL::CommandDraw& cmd) noexcept
{
    _d3d10Device->Draw(cmd.vertexCount, cmd.startVertex);
}

void DX10CommandQueue::_drawIndexed(const DX10CL::CommandDrawIndexed& cmd) noexcept
{
    _d3d10Device->DrawIndexed(cmd.indexCount, cmd.startIndex, cmd.baseVertex);
}

void DX10CommandQueue::_drawInstanced(const DX10CL::CommandDrawInstanced& cmd) noexcept
{
    _d3d10Device->DrawInstanced(cmd.vertexCount, cmd.instanceCount, cmd.startVertex, cmd.startInstance);
}

void DX10CommandQueue::_drawIndexedInstanced(const DX10CL::CommandDrawIndexedInstanced& cmd) noexcept
{
    _d3d10Device->DrawIndexedInstanced(cmd.indexCount, cmd.instanceCount, cmd.startIndex, cmd.baseVertex, cmd.startInstance);
}

void DX10CommandQueue::_setPipelineState(const DX10CL::CommandSetPipelineState& cmd) noexcept
{
    _currentPipelineState = cmd.pipelineState;

    _currentInputLayout = RefCast<DX10InputLayout>(cmd.pipelineState->args().inputLayout);

    const DX10DepthStencilState* const depthStencilState = RTT_CAST(cmd.pipelineState->args().depthStencilState.get(), DX10DepthStencilState);
    _currentDepthStencilState = depthStencilState->d3dDepthStencilState();
}

void DX10CommandQueue::_setStencilRef(const DX10CL::CommandSetStencilRef& cmd) noexcept
{
    _d3d10Device->OMSetDepthStencilState(_currentDepthStencilState, cmd.stencilRef);
}

void DX10CommandQueue::_setVertexArray(const DX10CL::CommandSetVertexArray& cmd) noexcept
{
    _d3d10Device->IASetVertexBuffers(cmd.startSlot, cmd.bufferCount, cmd.buffers, _currentInputLayout->strides(), _currentInputLayout->offsets());
}

void DX10CommandQueue::_setIndexBuffer(const DX10CL::CommandSetIndexBuffer& cmd) noexcept
{
    _d3d10Device->IASetIndexBuffer(cmd.buffer, cmd.format, 0);
}

void DX10CommandQueue::_setGDescriptorLayout(const DX10CL::CommandSetGDescriptorLayout& cmd) noexcept
{
}

void DX10CommandQueue::_setGDescriptorTable(const DX10CL::CommandSetGDescriptorTable& cmd) noexcept
{
}
#endif
