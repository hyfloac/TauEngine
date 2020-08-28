#include "dx/dx10/DX10CommandQueue.hpp"

#ifdef _WIN32
#include "graphics/PipelineState.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "dx/dx10/DX10DescriptorHeap.hpp"
#include "TauConfig.hpp"

void DX10CommandQueue::executeCommandLists(uSys count, const ICommandList* const * lists) noexcept
{
#if TAU_NULL_CHECK
    if(!lists)
    { return; }

    for(uSys i = 0; i < count; ++i)
    {
        if(!lists[i])
        { return; }
    }
#endif

#if TAU_RTTI_CHECK
    for(uSys i = 0; i < count; ++i)
    {
        if(!RTT_CHECK(lists[i], DX10CommandList))
        { return; }
    }
#endif

    for(uSys i = 0; i < count; ++i)
    {
        executeCommandList(lists[i]);
    }
}

void DX10CommandQueue::executeCommandList(const ICommandList* list) noexcept
{
    const DX10CommandList* const glList = static_cast<const DX10CommandList*>(list);
    /*
     * Copy the command array so that the list can be reset.
     * This will perform a reference count copy.
     */
    const ArrayList<DX10CL::Command> commands = glList->commands();  // NOLINT(performance-unnecessary-copy-initialization)

    for(auto cmd : commands)
    {
#define DISPATCH(__CMD, __FUNC, __ARG) case DX10CL::CommandType::__CMD: __FUNC(cmd.__ARG); break

        switch(cmd.type)
        {
            DISPATCH(Draw, _draw, draw);
            DISPATCH(DrawIndexed, _drawIndexed, drawIndexed);
            DISPATCH(DrawInstanced, _drawInstanced, drawInstanced);
            DISPATCH(DrawIndexedInstanced, _drawIndexedInstanced, drawIndexedInstanced);
            DISPATCH(SetDrawType, _setDrawType, setDrawType);
            DISPATCH(SetPipelineState, _setPipelineState, setPipelineState);
            DISPATCH(SetStencilRef, _setStencilRef, setStencilRef);
            DISPATCH(SetVertexArray, _setVertexArray, setVertexArray);
            DISPATCH(SetIndexBuffer, _setIndexBuffer, setIndexBuffer);
            DISPATCH(SetGDescriptorLayout, _setGDescriptorLayout, setGDescriptorLayout);
            DISPATCH(SetGDescriptorTable, _setGDescriptorTable, setGDescriptorTable);
            default: break;
        }
#undef DISPATCH
    }
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

void DX10CommandQueue::_setDrawType(const DX10CL::CommandSetDrawType& cmd) noexcept
{
    _d3d10Device->IASetPrimitiveTopology(cmd.drawType);
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
    _d3d10Device->OMSetDepthStencilState(_currentDepthStencilState, static_cast<UINT>(cmd.stencilRef));
}

void DX10CommandQueue::_setVertexArray(const DX10CL::CommandSetVertexArray& cmd) noexcept
{
    _d3d10Device->IASetVertexBuffers(static_cast<UINT>(cmd.startSlot), static_cast<UINT>(cmd.bufferCount), cmd.buffers, _currentInputLayout->strides(), _currentInputLayout->offsets());
}

void DX10CommandQueue::_setIndexBuffer(const DX10CL::CommandSetIndexBuffer& cmd) noexcept
{
    _d3d10Device->IASetIndexBuffer(cmd.buffer, cmd.format, 0);
}

void DX10CommandQueue::_setGDescriptorLayout(const DX10CL::CommandSetGDescriptorLayout& cmd) noexcept
{
    _currentLayout = cmd.layout.get<DX10DescriptorLayout>();
}

void DX10CommandQueue::_setGDescriptorTable(const DX10CL::CommandSetGDescriptorTable& cmd) noexcept
{
#if TAU_GENERAL_SAFETY_CHECK
    // Counts don't match
    if(cmd.descriptorCount != _currentLayout->entries()[cmd.index].count)
    { return; }
#endif

    if(cmd.type == EGraphics::DescriptorType::TextureView)
    {
#if TAU_GENERAL_SAFETY_CHECK
        // Types don't match
        if(_currentLayout->entries()[cmd.index].type != DescriptorLayoutEntry::Type::TextureView)
        { return; }
        // Counts don't match
        if(_currentLayout->entries()[cmd.index].count != cmd.descriptorCount)
        { return; }
#endif

        ID3D10ShaderResourceView** const texViews = cmd.handle.as<ID3D10ShaderResourceView*>();

        const UINT begin = static_cast<UINT>(_currentLayout->entries()[cmd.index].begin);
        const UINT count = static_cast<UINT>(cmd.descriptorCount);
        switch(_currentLayout->entries()[cmd.index].shaderAccess)
        {
            case EGraphics::ShaderAccess::All:
                _d3d10Device->VSSetShaderResources(begin, count, texViews);
                _d3d10Device->GSSetShaderResources(begin, count, texViews);
                _d3d10Device->PSSetShaderResources(begin, count, texViews);
                break;
            case EGraphics::ShaderAccess::Vertex:
                _d3d10Device->VSSetShaderResources(begin, count, texViews);
                break;
            case EGraphics::ShaderAccess::TessCtrl:
            case EGraphics::ShaderAccess::TessEval:
                break;
            case EGraphics::ShaderAccess::Geometry:
                _d3d10Device->GSSetShaderResources(begin, count, texViews);
                break;
            case EGraphics::ShaderAccess::Pixel:
                _d3d10Device->PSSetShaderResources(begin, count, texViews);
                break;
        }
    }
    else if(cmd.type == EGraphics::DescriptorType::UniformBufferView)
    {
#if TAU_GENERAL_SAFETY_CHECK
        // Types don't match
        if(_currentLayout->entries()[cmd.index].type != DescriptorLayoutEntry::Type::UniformBufferView)
        { return; }
        // Counts don't match
        if(_currentLayout->entries()[cmd.index].count != cmd.descriptorCount)
        { return; }
#endif

        ID3D10Buffer** const buffers = cmd.handle.as<ID3D10Buffer*>();

        const UINT begin = static_cast<UINT>(_currentLayout->entries()[cmd.index].begin);
        const UINT count = static_cast<UINT>(cmd.descriptorCount);
        switch(_currentLayout->entries()[cmd.index].shaderAccess)
        {
            case EGraphics::ShaderAccess::All:
                _d3d10Device->VSSetConstantBuffers(begin, count, buffers);
                _d3d10Device->GSSetConstantBuffers(begin, count, buffers);
                _d3d10Device->PSSetConstantBuffers(begin, count, buffers);
                break;
            case EGraphics::ShaderAccess::Vertex:
                _d3d10Device->VSSetConstantBuffers(begin, count, buffers);
                break;
            case EGraphics::ShaderAccess::TessCtrl:
            case EGraphics::ShaderAccess::TessEval:
                break;
            case EGraphics::ShaderAccess::Geometry:
                _d3d10Device->GSSetConstantBuffers(begin, count, buffers);
                break;
            case EGraphics::ShaderAccess::Pixel:
                _d3d10Device->PSSetConstantBuffers(begin, count, buffers);
                break;
        }
    }
}
#endif
