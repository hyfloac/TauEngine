#pragma once

#include "graphics/CommandQueue.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "DX10CommandList.hpp"
#include "DX10DescriptorLayout.hpp"

class DX10InputLayout;

class TAU_DLL DX10CommandQueue final : public ICommandQueue
{
    DEFAULT_DESTRUCT(DX10CommandQueue);
    DELETE_CM(DX10CommandQueue);
private:
    ID3D10Device* _d3d10Device;
    ID3D10DepthStencilState* _currentDepthStencilState;
    const PipelineState* _currentPipelineState;
    NullableRef<DX10InputLayout> _currentInputLayout;
    const DX10DescriptorLayout* _currentLayout;
public:
    DX10CommandQueue(ID3D10Device* const d3d10Device) noexcept
        : _d3d10Device(d3d10Device)
        , _currentDepthStencilState(null)
        , _currentPipelineState(null)
        , _currentInputLayout(null)
        , _currentLayout(null)
    { }

    void executeCommandLists(uSys count, const ICommandList* const * lists) noexcept override;

    void executeCommandList(const ICommandList* list) noexcept;
private:
    void _draw(const DX10CL::CommandDraw& cmd) noexcept;
    void _drawIndexed(const DX10CL::CommandDrawIndexed& cmd) noexcept;
    void _drawInstanced(const DX10CL::CommandDrawInstanced& cmd) noexcept;
    void _drawIndexedInstanced(const DX10CL::CommandDrawIndexedInstanced& cmd) noexcept;
    void _setDrawType(const DX10CL::CommandSetDrawType& cmd) noexcept;
    void _setPipelineState(const DX10CL::CommandSetPipelineState& cmd) noexcept;
    void _setStencilRef(const DX10CL::CommandSetStencilRef& cmd) noexcept;
    void _setVertexArray(const DX10CL::CommandSetVertexArray& cmd) noexcept;
    void _setIndexBuffer(const DX10CL::CommandSetIndexBuffer& cmd) noexcept;

    void _setGDescriptorLayout(const DX10CL::CommandSetGDescriptorLayout& cmd) noexcept;
    void _setGDescriptorTable(const DX10CL::CommandSetGDescriptorTable& cmd) noexcept;
};

#endif
