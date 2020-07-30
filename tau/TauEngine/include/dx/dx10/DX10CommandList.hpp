#pragma once

#include "graphics/CommandList.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include <ArrayList.hpp>

class DX10VertexArray;

namespace DX10CL {
enum class CommandType
{
    Draw = 1,
    DrawIndexed,
    DrawInstanced,
    DrawIndexedInstanced,
    SetDrawType,
    SetPipelineState,
    SetStencilRef,
    SetVertexArray,
    SetIndexBuffer,
    SetGDescriptorLayout,
    SetGDescriptorTable,
    SetGDescriptorSamplerTable
};

struct CommandDraw final
{
    DEFAULT_CONSTRUCT_PU(CommandDraw);
    DEFAULT_DESTRUCT(CommandDraw);
    DEFAULT_CM_PU(CommandDraw);
public:
    UINT vertexCount;
    UINT startVertex;
public:
    CommandDraw(const UINT _vertexCount, const UINT _startVertex) noexcept
        : vertexCount(_vertexCount)
        , startVertex(_startVertex)
    { }
};

struct CommandDrawIndexed final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexed);
    DEFAULT_DESTRUCT(CommandDrawIndexed);
    DEFAULT_CM_PU(CommandDrawIndexed);
public:
    UINT indexCount;
    UINT startIndex;
    INT baseVertex;
public:
    CommandDrawIndexed(const UINT _indexCount, const UINT _startIndex, const INT _baseVertex) noexcept
        : indexCount(_indexCount)
        , startIndex(_startIndex)
        , baseVertex(_baseVertex)
    { }
};

struct CommandDrawInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawInstanced);
    DEFAULT_DESTRUCT(CommandDrawInstanced);
    DEFAULT_CM_PU(CommandDrawInstanced);
public:
    UINT vertexCount;
    UINT instanceCount;
    UINT startVertex;
    UINT startInstance;
public:
    CommandDrawInstanced(const UINT _vertexCount, const UINT _instanceCount, const UINT _startVertex, const UINT _startInstance) noexcept
        : vertexCount(_vertexCount)
        , instanceCount(_instanceCount)
        , startVertex(_startVertex)
        , startInstance(_startInstance)
    { }
};

struct CommandDrawIndexedInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedInstanced);
    DEFAULT_DESTRUCT(CommandDrawIndexedInstanced);
    DEFAULT_CM_PU(CommandDrawIndexedInstanced);
public:
    UINT indexCount;
    UINT instanceCount;
    UINT startIndex;
    INT baseVertex;
    UINT startInstance;
public:
    CommandDrawIndexedInstanced(const UINT _indexCount, const UINT _instanceCount, const UINT _startIndex, const INT _baseVertex, const UINT _startInstance) noexcept
        : indexCount(_indexCount)
        , instanceCount(_instanceCount)
        , startIndex(_startIndex)
        , baseVertex(_baseVertex)
        , startInstance(_startInstance)
    { }
};

struct CommandSetDrawType final
{
    DEFAULT_CONSTRUCT_PU(CommandSetDrawType);
    DEFAULT_DESTRUCT(CommandSetDrawType);
    DEFAULT_CM_PU(CommandSetDrawType);
public:
    D3D10_PRIMITIVE_TOPOLOGY drawType;
public:
    CommandSetDrawType(const D3D10_PRIMITIVE_TOPOLOGY _drawType) noexcept
        : drawType(_drawType)
    { }
};

struct CommandSetPipelineState final
{
    DEFAULT_CONSTRUCT_PU(CommandSetPipelineState);
    DEFAULT_DESTRUCT(CommandSetPipelineState);
    DEFAULT_CM_PU(CommandSetPipelineState);
public:
    const PipelineState* pipelineState;
public:
    CommandSetPipelineState(const PipelineState* const _pipelineState) noexcept
        : pipelineState(_pipelineState)
    { }
};

struct CommandSetStencilRef final
{
    DEFAULT_CONSTRUCT_PU(CommandSetStencilRef);
    DEFAULT_DESTRUCT(CommandSetStencilRef);
    DEFAULT_CM_PU(CommandSetStencilRef);
public:
    uSys stencilRef;
public:
    CommandSetStencilRef(const uSys _stencilRef) noexcept
        : stencilRef(_stencilRef)
    { }
};

struct CommandSetVertexArray final
{
    DEFAULT_CONSTRUCT_PU(CommandSetVertexArray);
    DEFAULT_DESTRUCT(CommandSetVertexArray);
    DEFAULT_CM_PU(CommandSetVertexArray);
public:
    uSys startSlot;
    uSys bufferCount;
    ID3D10Buffer** buffers;
public:
    CommandSetVertexArray(const uSys _startSlot, const uSys _bufferCount, ID3D10Buffer** const _buffers) noexcept
        : startSlot(_startSlot)
        , bufferCount(_bufferCount)
        , buffers(_buffers)
    { }
};

struct CommandSetIndexBuffer final
{
    DEFAULT_CONSTRUCT_PU(CommandSetIndexBuffer);
    DEFAULT_DESTRUCT(CommandSetIndexBuffer);
    DEFAULT_CM_PU(CommandSetIndexBuffer);
public:
    ID3D10Buffer* buffer;
    DXGI_FORMAT format;
public:
    CommandSetIndexBuffer(ID3D10Buffer* const _buffer, const DXGI_FORMAT _format) noexcept
        : buffer(_buffer)
        , format(_format)
    { }
};

struct CommandSetGDescriptorLayout final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorLayout);
    DEFAULT_DESTRUCT(CommandSetGDescriptorLayout);
    DEFAULT_CM_PU(CommandSetGDescriptorLayout);
public:
    DescriptorLayout layout;
public:
    CommandSetGDescriptorLayout(const DescriptorLayout _layout) noexcept
        : layout(_layout)
    { }
};

struct CommandSetGDescriptorTable final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorTable);
    DEFAULT_DESTRUCT(CommandSetGDescriptorTable);
    DEFAULT_CM_PU(CommandSetGDescriptorTable);
public:
    uSys index;
    DescriptorTable table;
public:
    CommandSetGDescriptorTable(const uSys _index, const DescriptorTable _table) noexcept
        : index(_index)
        , table(_table)
    { }
};

struct CommandSetGDescriptorSamplerTable final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorSamplerTable);
    DEFAULT_DESTRUCT(CommandSetGDescriptorSamplerTable);
    DEFAULT_CM_PU(CommandSetGDescriptorSamplerTable);
public:
    uSys index;
    DescriptorSamplerTable table;
public:
    CommandSetGDescriptorSamplerTable(const uSys _index, const DescriptorSamplerTable _table) noexcept
        : index(_index)
        , table(_table)
    { }
};

struct Command final
{
    DEFAULT_DESTRUCT(Command);
    DEFAULT_CM_PU(Command);
public:
    CommandType type;
    union
    {
        uSys _;
        CommandDraw draw;
        CommandDrawIndexed drawIndexed;
        CommandDrawInstanced drawInstanced;
        CommandDrawIndexedInstanced drawIndexedInstanced;
        CommandSetDrawType setDrawType;
        CommandSetPipelineState setPipelineState;
        CommandSetStencilRef setStencilRef;
        CommandSetVertexArray setVertexArray;
        CommandSetIndexBuffer setIndexBuffer;
        CommandSetGDescriptorLayout setGDescriptorLayout;
        CommandSetGDescriptorTable setGDescriptorTable;
        CommandSetGDescriptorSamplerTable setGDescriptorSamplerTable;
    };
public:
    Command() noexcept
        : type(static_cast<CommandType>(0))
        , _(0)
    { }

    Command(const CommandDraw& _draw) noexcept
        : type(CommandType::Draw)
        , draw(_draw)
    { }

    Command(const CommandDrawIndexed& _drawIndexed) noexcept
        : type(CommandType::DrawIndexed)
        , drawIndexed(_drawIndexed)
    { }

    Command(const CommandDrawInstanced& _drawInstanced) noexcept
        : type(CommandType::DrawInstanced)
        , drawInstanced(_drawInstanced)
    { }

    Command(const CommandDrawIndexedInstanced& _drawIndexedInstanced) noexcept
        : type(CommandType::DrawIndexedInstanced)
        , drawIndexedInstanced(_drawIndexedInstanced)
    { }

    Command(const CommandSetDrawType& _setDrawType) noexcept
        : type(CommandType::SetDrawType)
        , setDrawType(_setDrawType)
    { }

    Command(const CommandSetPipelineState& _setPipelineState) noexcept
        : type(CommandType::SetPipelineState)
        , setPipelineState(_setPipelineState)
    { }

    Command(const CommandSetStencilRef& _setStencilRef) noexcept
        : type(CommandType::SetStencilRef)
        , setStencilRef(_setStencilRef)
    { }

    Command(const CommandSetVertexArray& _setVertexArray) noexcept
        : type(CommandType::SetVertexArray)
        , setVertexArray(_setVertexArray)
    { }

    Command(const CommandSetIndexBuffer& _setIndexBuffer) noexcept
        : type(CommandType::SetIndexBuffer)
        , setIndexBuffer(_setIndexBuffer)
    { }
};
}

class TAU_DLL DX10CommandList final : public ICommandList
{
    DEFAULT_DESTRUCT(DX10CommandList);
    DELETE_CM(DX10CommandList);
    COMMAND_LIST_IMPL(DX10CommandList);
private:
    uSys _maxCommands;
    ArrayList<DX10CL::Command> _commands;
    const DX10VertexArray* _currentVA;
public:
    DX10CommandList(const uSys maxCommands = 4096) noexcept
        : _maxCommands(maxCommands)
        , _commands(maxCommands)
        , _currentVA(null)
    { }

    [[nodiscard]] const ArrayList<DX10CL::Command>& commands() const noexcept { return _commands; }

    void reset() noexcept override;
    void finish() noexcept override;
    void draw(uSys exCount, uSys startVertex) noexcept override;
    void drawIndexed(uSys exCount, uSys startIndex, iSys baseVertex) noexcept override;
    void drawInstanced(uSys exCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void drawIndexedInstanced(uSys exCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void setDrawType(DrawType drawType) noexcept override;
    void setPipelineState(const PipelineState& pipelineState) noexcept override;
    void setStencilRef(uSys stencilRef) noexcept override;
    void setVertexArray(const IVertexArray& va) noexcept override;
    void setIndexBuffer(const IndexBufferView& indexBufferView) noexcept override;
    void setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept override;
    void setGraphicsDescriptorTable(uSys index, DescriptorTable table) noexcept override;
    void setGraphicsDescriptorTable(uSys index, DescriptorSamplerTable table) noexcept override;
};

#endif
