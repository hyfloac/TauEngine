#include "dx/dx10/DX10CommandQueue.hpp"

#ifdef _WIN32
#include "graphics/PipelineState.hpp"
#include "dx/dx10/DX10BlendingState.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "dx/dx10/DX10DescriptorHeap.hpp"
#include "dx/dx10/DX10CommandAllocator.hpp"
#include "TauConfig.hpp"

void DX10CommandQueue::executeCommandLists(const uSys count, const ICommandList* const * lists) noexcept
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
    const DX10CommandList* const dxList = static_cast<const DX10CommandList*>(list);

    const u8* const head = reinterpret_cast<const u8*>(dxList->_head);
    const uSys commandCount = dxList->_commandCount;
    const uSys blockSize = dxList->_commandAllocator->blockSize();

    DX10CL::CommandCopySubresourceRegion0 copy0;
    DX10CL::CommandCopySubresourceRegion1 copy1;

    for(uSys i = 0, offset = 0; i < commandCount; ++i, offset += blockSize)
    {
        const DX10CL::Command& cmd = *reinterpret_cast<const DX10CL::Command*>(head + offset);

#define DISPATCH(__CMD, __FUNC, __ARG) case DX10CL::CommandType::__CMD: __FUNC(cmd.__ARG); break

        switch(cmd.type)
        {
            DISPATCH(Draw, _draw, draw);
            DISPATCH(DrawIndexed, _drawIndexed, drawIndexed);
            DISPATCH(DrawInstanced, _drawInstanced, drawInstanced);
            DISPATCH(DrawIndexedInstanced, _drawIndexedInstanced, drawIndexedInstanced);
            DISPATCH(SetDrawType, _setDrawType, setDrawType);
            DISPATCH(SetPipelineState, _setPipelineState, setPipelineState);
            DISPATCH(SetBlendFactor, _setBlendFactor, setBlendFactor);
            DISPATCH(SetStencilRef, _setStencilRef, setStencilRef);
            DISPATCH(SetVertexArray, _setVertexArray, setVertexArray);
            DISPATCH(SetIndexBuffer, _setIndexBuffer, setIndexBuffer);
            DISPATCH(SetGDescriptorLayout, _setGDescriptorLayout, setGDescriptorLayout);
            DISPATCH(SetGDescriptorTable, _setGDescriptorTable, setGDescriptorTable);
            DISPATCH(CopyResource, _copyResource, copyResource);
            case DX10CL::CommandType::CopySubresourceRegion0:
                copy0 = cmd.copySubresourceRegion0;
                break;
            case DX10CL::CommandType::CopySubresourceRegion1:
                copy1 = cmd.copySubresourceRegion1;
                break;
            case DX10CL::CommandType::CopySubresourceRegion2:
                _copySubresourceRegion(copy0, copy1, cmd.copySubresourceRegion2);
                break;
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

    const auto& args = cmd.pipelineState->args();

    _currentInputLayout = RefCast<DX10InputLayout>(args.inputLayout);

    const DX10BlendingState* const blendingState = rtt_cast<DX10BlendingState>(args.blendingState);
    if(_currentBlendState != blendingState->d3dBlendState())
    {
        _currentBlendState = blendingState->d3dBlendState();
        _d3d10Device->OMSetBlendState(_currentBlendState, _blendingFactors, 0xFFFFFFFF);
    }

    const DX10DepthStencilState* const depthStencilState = rtt_cast<DX10DepthStencilState>(args.depthStencilState);
    if(_currentDepthStencilState != depthStencilState->d3dDepthStencilState())
    {
        _currentDepthStencilState = depthStencilState->d3dDepthStencilState();
        _d3d10Device->OMSetDepthStencilState(_currentDepthStencilState, static_cast<UINT>(_stencilRef));
    }

    const DX10RasterizerState* const rasterizerState = rtt_cast<DX10RasterizerState>(args.rasterizerState);
    if(_currentRasterizerState != rasterizerState->d3dRasterizerState())
    {
        _currentRasterizerState = rasterizerState->d3dRasterizerState();
        _d3d10Device->RSSetState(_currentRasterizerState);
    }
}

void DX10CommandQueue::_setBlendFactor(const DX10CL::CommandSetBlendFactor& cmd) noexcept
{
    if(::std::memcmp(cmd.blendFactor, _blendingFactors, sizeof(float) * 4) != 0)
    {
        ::std::memcpy(_blendingFactors, cmd.blendFactor, sizeof(float) * 4);
        _d3d10Device->OMSetBlendState(_currentBlendState, cmd.blendFactor, 0xFFFFFFFF);
    }
}

void DX10CommandQueue::_setStencilRef(const DX10CL::CommandSetStencilRef& cmd) noexcept
{
    if(cmd.stencilRef != _stencilRef)
    {
        _stencilRef = cmd.stencilRef;
        _d3d10Device->OMSetDepthStencilState(_currentDepthStencilState, static_cast<UINT>(cmd.stencilRef));
    }
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

void DX10CommandQueue::_copyResource(const DX10CL::CommandCopyResource& cmd) noexcept
{
    _d3d10Device->CopyResource(cmd.dst, cmd.src);
}

void DX10CommandQueue::_copySubresourceRegion(const DX10CL::CommandCopySubresourceRegion0& cmd0, const DX10CL::CommandCopySubresourceRegion1& cmd1, const DX10CL::CommandCopySubresourceRegion2& cmd2) noexcept
{
    D3D10_BOX box;
    const D3D10_BOX* pBox = nullptr;

    if(cmd1.hasSrcBox)
    {
        box = D3D10_BOX { cmd2.srcBox.left, cmd2.srcBox.top, cmd2.srcBox.front, cmd2.srcBox.right, cmd2.srcBox.bottom, cmd2.srcBox.back };
        pBox = &box;
    }

    _d3d10Device->CopySubresourceRegion(cmd0.dst, cmd0.dstSubresource, cmd1.coord.x, cmd1.coord.y, cmd1.coord.z, cmd0.src, cmd0.srcSubresource, pBox);
}
#endif
